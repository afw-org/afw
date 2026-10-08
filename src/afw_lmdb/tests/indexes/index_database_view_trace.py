#!/usr/bin/env python3
"""
Proves, via the "trace:adapterId:lmdb" trace flag, how retrieve_objects()
treats index databases (#511):

- An index database another process created is opened when the adapter
  starts, so a query in a new process uses the index. A read never opens
  a database itself, so without that it would scan.
- A query on an indexed property scans when the index database does not
  exist yet (no object of that type was written).
"""

import os
import subprocess
import tempfile

from _afwdev.test import context as test_context


INDEX_QUERY = "retrieve_objects: using index query"
NOT_IN_VIEW = "retrieve_objects: using full scan (index database not in this transaction's view)"


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


def run():
    description = "retrieve_objects() and index databases not in a read's view, proven via trace (#511)"

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

    # One process writes, so the per-type index database exists on disk.
    seed = _run_script(work_dir, afw_conf, """
        index_create("lmdb", "pq", undefined, [], undefined, undefined, false, false);
        add_object("lmdb", "TestIndexViewSeeded", { pq: "a" }, generate_uuid());
        return 0;
        """)

    # A new process queries it.
    r = _run_script(work_dir, afw_conf, """
        flag_set(["trace:adapterId:lmdb"], true);
        const n = length(retrieve_objects("lmdb", "TestIndexViewSeeded",
            { "filter": { "op": "eq", "property": "pq", "value": "a" } }));
        assert(n === 1, "expected the seeded object");
        return 0;
        """)
    out = (r.stdout or "") + (r.stderr or "")
    tests.append(_result(
        "index_database_from_another_process",
        "An index database another process created is opened at start, so a new process's query uses the index",
        r,
        seed.returncode == 0 and r.returncode == 0
        and INDEX_QUERY in out and NOT_IN_VIEW not in out,
    ))

    r = _run_script(work_dir, afw_conf, """
        flag_set(["trace:adapterId:lmdb"], true);
        const n = length(retrieve_objects("lmdb", "TestIndexViewNeverWritten",
            { "filter": { "op": "eq", "property": "pq", "value": "a" } }));
        assert(n === 0, "expected nothing");
        return 0;
        """)
    out = (r.stdout or "") + (r.stderr or "")
    tests.append(_result(
        "index_database_missing_scans",
        "A query on an indexed property scans when no object of the type was written, so its index database does not exist",
        r,
        r.returncode == 0 and NOT_IN_VIEW in out and INDEX_QUERY not in out,
    ))

    return {"description": description, "tests": tests}
