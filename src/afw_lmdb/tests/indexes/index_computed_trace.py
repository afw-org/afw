#!/usr/bin/env python3
"""
Queries through index definitions (issue #516), proven via the
"trace:adapterId:lmdb" trace flag and across processes.

A filter that names a computed index (value script, maybe a filter) or a
case-insensitive one is tested through the index definitions, in the
index query's re-test and in a scan. A filter that names neither keeps
afw_query_criteria_test_object(): no "index query test:" trace line, so
it pays nothing per object.

A query that finds an object type declaring a computed index's name
throws. Object types are cached for the life of a request (one afw run),
so the change and the query are separate runs here.
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


def _result(name, description, passed, results):
    return {
        "test": name,
        "description": description,
        "passed": bool(passed),
        "skip": False,
        "stdout": "\n".join((r.stdout or "") for r in results),
        "stderr": "\n".join((r.stderr or "") for r in results),
        "returncode": results[-1].returncode,
    }


SETUP = """
const ot = "TestComputedTraceType";
add_object("lmdb", ot, { given: "Ada", surname_ct: "Smith", plain: "p", k: "x" }, generate_uuid());
index_create("lmdb", "FullName_ct", "current::object.given", [ot], undefined, undefined, true, false);
index_create("lmdb", "surname_ct", undefined, [ot], undefined, ["case-insensitive-string"], true, false);
index_create("lmdb", "plain", undefined, [ot], undefined, undefined, true, false);
return 0;
"""

TRACE_LINE = "index query test:"

TRACE_CASES = [
    (
        "plain_index_query_plain_test",
        "A filter on a plain index (no script, case-sensitive) is tested as before: no index query test",
        """{ op: "and", filters: [
            { op: "eq", property: "plain", value: "p" },
            { op: "eq", property: "k", value: "x" } ] }""",
        ["retrieve_objects: using index query"],
        [TRACE_LINE],
    ),
    (
        "plain_scan_plain_test",
        "A scan whose filter names no computed or case-insensitive index is tested as before",
        """{ op: "or", filters: [
            { op: "eq", property: "plain", value: "p" },
            { op: "eq", property: "k", value: "never" } ] }""",
        ["retrieve_objects: using full scan (not sargable)"],
        [TRACE_LINE],
    ),
    (
        "computed_index_query_test",
        "An index query on a computed name re-tests through the index definition",
        """{ op: "and", filters: [
            { op: "eq", property: "FullName_ct", value: "Ada" },
            { op: "eq", property: "k", value: "x" } ] }""",
        [
            "retrieve_objects: using index query",
            TRACE_LINE + " FullName_ct through its index definition (computed)",
        ],
        [],
    ),
    (
        "computed_scan_test",
        "A scan whose filter names a computed index tests through the index definition",
        """{ op: "or", filters: [
            { op: "eq", property: "FullName_ct", value: "Ada" },
            { op: "eq", property: "k", value: "never" } ] }""",
        [
            "retrieve_objects: using full scan (not sargable)",
            TRACE_LINE + " FullName_ct through its index definition (computed)",
        ],
        [],
    ),
    (
        "case_insensitive_scan_test",
        "A scan whose filter names a case-insensitive index tests through the index definition",
        """{ op: "or", filters: [
            { op: "eq", property: "surname_ct", value: "SMITH" },
            { op: "eq", property: "k", value: "never" } ] }""",
        [
            "retrieve_objects: using full scan (not sargable)",
            TRACE_LINE + " surname_ct through its index definition (case-insensitive)",
        ],
        [],
    ),
]


def run():
    description = "Queries through index definitions: trace and object type changes (issue #516)"

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

    setup = _run_script(work_dir, afw_conf, SETUP)
    for name, desc, filter_text, must_contain, must_not_contain in TRACE_CASES:
        body = """
            const ot = "TestComputedTraceType";
            flag_set(["trace:adapterId:lmdb"], true);
            const found = retrieve_objects("lmdb", ot, { filter: %s });
            assert(length(found) === 1, "one object: " + string(length(found)));
            return 0;
            """ % filter_text
        r = _run_script(work_dir, afw_conf, body)
        out = (r.stdout or "") + (r.stderr or "")
        passed = (
            setup.returncode == 0
            and r.returncode == 0
            and all(s in out for s in must_contain)
            and not any(s in out for s in must_not_contain)
        )
        tests.append(_result(name, desc, passed, [setup, r]))

    # An object type that declares a computed name after the index exists.
    first = _run_script(work_dir, afw_conf, """
        const ot = "TestComputedLaterType";
        add_object("lmdb", "_AdaptiveObjectType_", {
            propertyTypes: { given: { dataType: "string", allowQuery: true } },
            otherProperties: { dataType: "string", allowQuery: true }
        }, ot);
        add_object("lmdb", ot, { given: "Ada" }, generate_uuid());
        index_create("lmdb", "FullName_cl", "current::object.given", [ot],
            undefined, undefined, true, false);
        const found = retrieve_objects("lmdb", ot,
            { filter: { op: "eq", property: "FullName_cl", value: "Ada" } });
        assert(length(found) === 1, "before the change");
        replace_object("lmdb", "_AdaptiveObjectType_", ot, {
            propertyTypes: {
                given: { dataType: "string", allowQuery: true },
                FullName_cl: { dataType: "string", allowQuery: true }
            },
            otherProperties: { dataType: "string", allowQuery: true }
        });
        return 0;
        """)
    second = _run_script(work_dir, afw_conf, """
        const ot = "TestComputedLaterType";
        let message = "";
        try {
            retrieve_objects("lmdb", ot,
                { filter: { op: "eq", property: "FullName_cl", value: "Ada" } });
        }
        catch (e) {
            message = e.message;
        }
        assert(includes(message, "FullName_cl") && includes(message, "declares"),
            "declared after the index: " + message);
        const plain = retrieve_objects("lmdb", ot,
            { filter: { op: "eq", property: "given", value: "Ada" } });
        assert(length(plain) === 1, "a query on another property");
        return 0;
        """)
    tests.append(_result(
        "computed_name_declared_later",
        "A query on a computed name that an object type declares since the index was created throws",
        first.returncode == 0 and second.returncode == 0,
        [first, second],
    ))

    return {"description": description, "tests": tests}
