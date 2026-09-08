#!/usr/bin/env python3
"""
Proves, via the "trace:adapterId:lmdb" trace flag, that
afw_adapter_impl_index_cursor_list_merge() orders a genuine OR's cursors
lowest-cardinality-first, not highest-first (issue #303).

Correctness of the OR doesn't depend on this ordering (fixed in #296/#297
regardless of order) -- only the volume of afw_adapter_impl_index_applies()
duplicate-elimination checks does, so there's no way to observe this from
retrieve_objects() results alone. afw_adapter_impl_index_query() now traces
its total applies() count per query, the same technique already used for
the sargable-vs-full-scan decision (index_sargable_trace.py) and the
cardinality estimate itself (index_cardinality_probe_trace.py).

Measured by hand while fixing #303, toggling the merge comparison's
direction and rerunning this exact scenario (three disjoint eq cursors,
cardinalities 3/10/50, OR'd together -- 63 total objects, no overlap):

  - lowest-cardinality-first (this fix):  16 applies() checks
  - highest-cardinality-first (pre-fix produced by the code
    prior to this branch): 110 applies() checks

Same correct 63-object result either way -- this is a pure efficiency
regression test, not a correctness one (see index_or_merge_cardinality.as
for correctness).
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
    description = (
        "cursor_list_merge() sorts an OR's cursors lowest-cardinality-first, "
        "minimizing total duplicate-elimination checks (issue #303)"
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

    body = """
        const ot = "TestMergeSortDirection";
        index_create("lmdb", "group_sort", undefined, [ot], undefined, undefined, false, false);

        // Three disjoint groups, deliberately unbalanced (3/10/50 -- 63
        // total, no object in more than one group, so every applies()
        // check this makes is "real" dedup-check volume, not masked by
        // actual duplicates).
        let i = 0;
        while (i < 3) {
            add_object("lmdb", ot, { group_sort: "small" }, generate_uuid());
            i = i + 1;
        }
        i = 0;
        while (i < 10) {
            add_object("lmdb", ot, { group_sort: "medium" }, generate_uuid());
            i = i + 1;
        }
        i = 0;
        while (i < 50) {
            add_object("lmdb", ot, { group_sort: "large" }, generate_uuid());
            i = i + 1;
        }

        flag_set(["trace:adapterId:lmdb"], true);
        const objects = retrieve_objects("lmdb", ot, { "filter": {
            "op": "or",
            "filters": [
                { "op": "eq", "property": "group_sort", "value": "small" },
                { "op": "eq", "property": "group_sort", "value": "medium" },
                { "op": "eq", "property": "group_sort", "value": "large" }
            ]
        }});
        print("RESULT_COUNT=" + length(objects));

        safe_evaluate(index_remove("lmdb", "group_sort"), null);

        return 0;
    """

    r = _run_script(work_dir, afw_conf, body)
    out = (r.stdout or "") + (r.stderr or "")
    passed = (
        r.returncode == 0
        and "index query: 16 applies() checks for dedup" in out
        and "RESULT_COUNT=63" in out
    )

    tests = [
        {
            "test": "unbalanced_three_way_or_minimizes_applies_checks",
            "description": (
                "3/10/50-object disjoint OR: 16 applies() checks (lowest "
                "cardinality first), not 110 (highest first, pre-#303), "
                "same correct 63-object result either way"
            ),
            "passed": bool(passed),
            "skip": False,
            "stdout": r.stdout,
            "stderr": r.stderr,
            "returncode": r.returncode,
        }
    ]

    return {"description": description, "tests": tests}
