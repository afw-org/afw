#!/usr/bin/env python3
"""
Proves, via the "trace:adapterId:lmdb" trace flag, the behavior of the
bounded cardinality probe used for non-eq (range / "starts with") adapter-
index cursors: impl_afw_adapter_impl_index_cursor_get_count() in
afw_lmdb_internal.c. Issue #298.

Correctness of OR/AND across indexed cursors no longer depends on this
value at all (issue #296/#297) -- it is purely an ordering hint for the
later duplicate-elimination pass -- so there is no way to observe it from
retrieve_objects() results. The trace line it emits is the only external
signal, the same technique index_sargable_trace.py already uses for the
sargable-vs-full-scan decision.

Requires the test environment's afw.conf to set limits.cardinalityProbeCap
to a small, deliberate value (5, as of this test) rather than the built-in
default of 100 -- both to keep the object counts in these tests small and
to prove the *configured* value is what's actually used, not the default.
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


def _or_query(age_key, status_key, as_expr=False):
    # A genuine OR forces afw_adapter_impl_index_cursor_list_merge() to call
    # get_count() on both cursors; the eq(status,nonexistent) side is always
    # cardinality 0 (empty, still exact), so the trace line for the *other*,
    # non-eq (gt) cursor is the one under test.
    call = (
        'retrieve_objects("lmdb", ot, { "filter": {\n'
        '    "op": "or",\n'
        '    "filters": [\n'
        f'        {{ "op": "gt", "property": "{age_key}", "value": 0 }},\n'
        f'        {{ "op": "eq", "property": "{status_key}", "value": "nonexistent" }}\n'
        "    ]\n"
        "}})"
    )
    return call if as_expr else call + ";"


def run():
    description = (
        "Adapter-index cardinality probe for non-eq cursors: exact under the "
        "configured cap, capped (never wrong results) over it (issue #298)"
    )

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

    # Distinct index keys per case (not just distinct object types) --
    # index_create() silently no-ops when a key already exists, even for a
    # different object type, so a reused key across these cases would make
    # a later case's property never actually get indexed (proven by hand
    # while writing this test: it silently fell back to a full scan, which
    # never reaches the code under test at all).
    cases = [
        (
            "exact_under_configured_cap",
            "A range cursor matching fewer entries than the configured cap (5) reports its exact count",
            """
            const ot = "TestCardinalityProbeExact";
            index_create("lmdb", "age_exact", undefined, [ot], undefined, undefined, false, false);
            index_create("lmdb", "status_exact", undefined, [ot], undefined, undefined, false, false);
            add_object("lmdb", ot, { age_exact: 10, status_exact: "x" }, generate_uuid());
            add_object("lmdb", ot, { age_exact: 20, status_exact: "x" }, generate_uuid());
            add_object("lmdb", ot, { age_exact: 30, status_exact: "x" }, generate_uuid());
            flag_set(["trace:adapterId:lmdb"], true);
            """
            + _or_query("age_exact", "status_exact")
            + """
            return 0;
            """,
            "index cursor cardinality estimate: 3 (cap 5)",
            "index cursor cardinality estimate: 5 (cap 5)",
        ),
        (
            "capped_over_configured_cap",
            "A range cursor matching more entries than the configured cap (5) reports the cap, not the true count -- and proves it's the *configured* cap (5), not the built-in default (100)",
            """
            const ot = "TestCardinalityProbeCapped";
            index_create("lmdb", "age_capped", undefined, [ot], undefined, undefined, false, false);
            index_create("lmdb", "status_capped", undefined, [ot], undefined, undefined, false, false);
            add_object("lmdb", ot, { age_capped: 10, status_capped: "x" }, generate_uuid());
            add_object("lmdb", ot, { age_capped: 20, status_capped: "x" }, generate_uuid());
            add_object("lmdb", ot, { age_capped: 30, status_capped: "x" }, generate_uuid());
            add_object("lmdb", ot, { age_capped: 40, status_capped: "x" }, generate_uuid());
            add_object("lmdb", ot, { age_capped: 50, status_capped: "x" }, generate_uuid());
            add_object("lmdb", ot, { age_capped: 60, status_capped: "x" }, generate_uuid());
            add_object("lmdb", ot, { age_capped: 70, status_capped: "x" }, generate_uuid());
            add_object("lmdb", ot, { age_capped: 80, status_capped: "x" }, generate_uuid());
            flag_set(["trace:adapterId:lmdb"], true);
            """
            + _or_query("age_capped", "status_capped")
            + """
            return 0;
            """,
            "index cursor cardinality estimate: 5 (cap 5)",
            "index cursor cardinality estimate: 8 (cap 5)",
        ),
        (
            "capped_estimate_never_affects_results",
            "Even though the cardinality estimate is capped (defect this guards: an underestimate must never drop matches), all 8 matching objects still come back",
            """
            const ot = "TestCardinalityProbeCappedCorrectness";
            index_create("lmdb", "age_correct", undefined, [ot], undefined, undefined, false, false);
            index_create("lmdb", "status_correct", undefined, [ot], undefined, undefined, false, false);
            add_object("lmdb", ot, { age_correct: 10, status_correct: "x" }, generate_uuid());
            add_object("lmdb", ot, { age_correct: 20, status_correct: "x" }, generate_uuid());
            add_object("lmdb", ot, { age_correct: 30, status_correct: "x" }, generate_uuid());
            add_object("lmdb", ot, { age_correct: 40, status_correct: "x" }, generate_uuid());
            add_object("lmdb", ot, { age_correct: 50, status_correct: "x" }, generate_uuid());
            add_object("lmdb", ot, { age_correct: 60, status_correct: "x" }, generate_uuid());
            add_object("lmdb", ot, { age_correct: 70, status_correct: "x" }, generate_uuid());
            add_object("lmdb", ot, { age_correct: 80, status_correct: "x" }, generate_uuid());
            const objects: array = """
            + _or_query("age_correct", "status_correct", as_expr=True)
            + """;
            print("RESULT_COUNT=" + length(objects));
            return 0;
            """,
            "RESULT_COUNT=8",
            "RESULT_COUNT=5",
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
