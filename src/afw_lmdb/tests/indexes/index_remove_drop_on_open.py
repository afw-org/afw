#!/usr/bin/env python3
"""
index_remove only clears an index's databases: deleting one closes its
handle for the whole process under any transaction still reading it. The
next process to open the environment deletes the index databases no
definition covers (#511). Each step here is a separate afw process.
"""

import os
import subprocess
import tempfile

from _afwdev.test import context as test_context


def _run_script(work_dir, afw_conf, script_body):
    with tempfile.NamedTemporaryFile(
        "w", suffix=".as", dir=work_dir, delete=False
    ) as tf:
        tf.write(script_body)
        script_path = tf.name
    try:
        cmd = ["afw"]
        if afw_conf:
            cmd += ["--conf", "afw.conf"]
        cmd += ["-s", "script", script_path]
        return subprocess.run(
            cmd, cwd=work_dir, capture_output=True, text=True, timeout=60
        )
    finally:
        os.unlink(script_path)


def _result(name, desc, r, passed):
    return {
        "test": name,
        "description": desc,
        "passed": bool(passed),
        "skip": False,
        "stdout": r.stdout,
        "stderr": r.stderr,
        "returncode": r.returncode,
    }


DATABASE_NAMES = """
    const names = function () {
        const a = get_object("afw", "_AdaptiveAdapter_", "lmdb");
        return stringify(a.metrics.additional.statistics);
    };
"""


def run():
    description = "Index databases left by index_remove are deleted when a new process opens the environment (#511)"

    ctx = test_context.current()
    testEnvironment = ctx.get("testEnvironment") or {}
    work_dir = testEnvironment.get("work_dir")
    afw_conf = testEnvironment.get("afw_conf")

    if not work_dir:
        return {
            "description": description,
            "tests": [
                {
                    "test": "environment_available",
                    "description": "lmdb-adapter test environment is available",
                    "passed": False,
                    "skip": False,
                    "error": "No testEnvironment/work_dir in test context",
                }
            ],
        }

    tests = []

    # Leave cleared databases from two removed indexes (one scoped to a
    # type, one on all types) next to one index still defined.
    seed = _run_script(work_dir, afw_conf, """
        index_create("lmdb", "xq", undefined, ["TestDropOnOpenX"], undefined, undefined, false, false);
        add_object("lmdb", "TestDropOnOpenX", { xq: "a" }, generate_uuid());
        index_create("lmdb", "aq", undefined, [], undefined, undefined, false, false);
        add_object("lmdb", "TestDropOnOpenA", { aq: "a" }, generate_uuid());
        index_create("lmdb", "kq", undefined, ["TestDropOnOpenK"], undefined, undefined, false, false);
        add_object("lmdb", "TestDropOnOpenK", { kq: "a" }, generate_uuid());
        index_remove("lmdb", "xq");
        index_remove("lmdb", "aq");
        return 0;
        """)

    r = _run_script(work_dir, afw_conf, DATABASE_NAMES + """
        const before = names();
        assert(!includes<string>(before, "Index#TestDropOnOpenX#xq"), "left database of a scoped index not deleted: " + before);
        assert(!includes<string>(before, "Index#TestDropOnOpenA#aq"), "left database of an all-types index not deleted: " + before);
        assert(includes<string>(before, "Index#TestDropOnOpenK#kq"), "database of a defined index deleted: " + before);

        flag_set(["trace:adapterId:lmdb"], true);
        const n = length(retrieve_objects("lmdb", "TestDropOnOpenK",
            { "filter": { "op": "eq", "property": "kq", "value": "a" } }));
        assert(n === 1, "expected the object indexed by kq");

        // The left database is gone, so different options are fine now.
        index_create("lmdb", "xq", undefined, ["TestDropOnOpenX"], undefined, ["unique"], false, false);
        return 0;
        """)
    out = (r.stdout or "") + (r.stderr or "")
    tests.append(_result(
        "index_databases_left_deleted_on_open",
        "A new process deletes the databases removed indexes left, keeps a defined index's, and allows different options again",
        r,
        seed.returncode == 0 and r.returncode == 0
        and "retrieve_objects: using index query" in out,
    ))

    return {"description": description, "tests": tests}
