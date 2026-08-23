#!/usr/bin/env python3
"""
Proves, via the "trace:adapterId:lmdb" trace flag, which code path
retrieve_objects() actually took: an index-accelerated query vs a full
scan. Correctness tests (index_objecttype_scoping.as, etc.) only prove the
right objects came back -- they cannot prove *how*. Issue #57.

Requires the trace instrumentation added to
impl_afw_adapter_session_retrieve_objects() in afw_lmdb_adapter_session.c
(the same "sargable?" branch afw_adapter_impl_index_sargable() gates).
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


def run():
    description = "retrieve_objects() index-query vs full-scan, proven via trace (issue #57)"

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

    cases = [
        (
            "not_sargable_before_index",
            "No index exists yet: retrieve_objects falls back to a full scan",
            """
            const ot = "TestSargableTraceType";
            flag_set(["trace:adapterId:lmdb"], true);
            add_object("lmdb", ot, { email: "a@example.com" }, generate_uuid());
            retrieve_objects("lmdb", ot,
                { "filter": { "op": "eq", "property": "email", "value": "a@example.com" } });
            return 0;
            """,
            "retrieve_objects: using full scan (not sargable)",
            "retrieve_objects: using index query",
        ),
        (
            "sargable_after_index",
            "An eq filter on an indexed property is sargable: retrieve_objects uses the index",
            """
            const ot = "TestSargableTraceType2";
            add_object("lmdb", ot, { email: "a@example.com" }, generate_uuid());
            index_create("lmdb", "email", undefined, [ot], undefined, undefined, true, false);
            flag_set(["trace:adapterId:lmdb"], true);
            retrieve_objects("lmdb", ot,
                { "filter": { "op": "eq", "property": "email", "value": "a@example.com" } });
            return 0;
            """,
            "retrieve_objects: using index query",
            "retrieve_objects: using full scan (not sargable)",
        ),
    ]

    for name, desc, body, must_contain, must_not_contain in cases:
        r = _run_script(work_dir, afw_conf, body)
        out = (r.stdout or "") + (r.stderr or "")
        passed = (
            r.returncode == 0
            and must_contain in out
            and must_not_contain not in out
        )
        tests.append(
            {
                "test": name,
                "description": desc,
                "passed": bool(passed),
                "skip": False,
                "stdout": r.stdout,
                "stderr": r.stderr,
                "returncode": r.returncode,
            }
        )

    return {"description": description, "tests": tests}
