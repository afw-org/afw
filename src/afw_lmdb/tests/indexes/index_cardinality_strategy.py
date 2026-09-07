#!/usr/bin/env python3
"""
Proves the three limits.cardinalityStrategy conf values on
_AdaptiveConf_adapter_lmdb_limits (issue #298):

  - unset (default) behaves as "totalEntries": mdb_stat()'s O(1) total
    entry count for the index DB, no cursor movement.
  - "off": always reports cardinality as unknown (pre-#298 behavior).
  - an unrecognized value throws at adapter-creation time rather than
    silently falling back to a default (afw_lmdb_adapter_parse_limits()
    in afw_lmdb_adapter.c).

Unlike index_cardinality_probe_trace.py (which exercises the "probe"
strategy via the shared lmdb-adapter test environment, pinned to "probe"
specifically so that file's assertions stay meaningful), each case here
builds its own isolated LMDB environment and afw.conf, since each needs
a different limits.cardinalityStrategy value than the shared environment
uses.
"""

import os
import subprocess
import tempfile


_ENV_TEMPLATE = """[
    {{
        "adapterId": "lmdb",
        "type": "adapter",
        "adapterType": "lmdb",
        "limits": {{
            "time": {{ "hard": 14400, "soft": 3600 }},
            "size": {{ "hard": 1000, "soft": 500 }}{strategy_clause}
        }},
        "env": {{
            "mapsize": 104857600,
            "maxdbs": 128,
            "maxreaders": 126,
            "mode": 384,
            "path": "${{environment::AFW_LMDB_TEST_PATH}}"
        }}
    }}
]
"""

_SCRIPT = """#!/usr/bin/env afw
const ot = "TestCardinalityStrategy";
index_create("lmdb", "age", undefined, [ot], undefined, undefined, false, false);
index_create("lmdb", "status", undefined, [ot], undefined, undefined, false, false);
add_object("lmdb", ot, { age: 10, status: "x" }, generate_uuid());
add_object("lmdb", ot, { age: 20, status: "x" }, generate_uuid());
add_object("lmdb", ot, { age: 30, status: "x" }, generate_uuid());
flag_set(["trace:adapterId:lmdb"], true);
const objects = retrieve_objects("lmdb", ot, { "filter": { "op": "or", "filters": [
    { "op": "gt", "property": "age", "value": 0 },
    { "op": "eq", "property": "status", "value": "nonexistent" }
]}});
print("RESULT_COUNT=" + length(objects));
return 0;
"""


def _run_case(strategy):
    """Run _SCRIPT in a fresh, isolated LMDB environment configured with
    the given cardinalityStrategy (None to omit the property entirely,
    exercising the default)."""
    strategy_clause = (
        ',\n            "cardinalityStrategy": "{}"'.format(strategy)
        if strategy is not None
        else ""
    )
    conf = _ENV_TEMPLATE.format(strategy_clause=strategy_clause)

    with tempfile.TemporaryDirectory() as work_dir:
        with open(os.path.join(work_dir, "afw.conf"), "w") as f:
            f.write(conf)
        script_path = os.path.join(work_dir, "probe.as")
        with open(script_path, "w") as f:
            f.write(_SCRIPT)

        env = dict(os.environ)
        env["AFW_LMDB_TEST_PATH"] = work_dir

        return subprocess.run(
            ["afw", "--conf", "afw.conf", "-s", "script", script_path],
            cwd=work_dir,
            env=env,
            capture_output=True,
            text=True,
            timeout=60,
        )


def run():
    description = (
        "limits.cardinalityStrategy: default (totalEntries), \"off\", and "
        "rejection of an unrecognized value (issue #298)"
    )

    tests = []

    r = _run_case(None)
    out = (r.stdout or "") + (r.stderr or "")
    tests.append(
        {
            "test": "default_strategy_is_total_entries",
            "description": "With cardinalityStrategy unset, a non-eq cursor reports mdb_stat's total entry count",
            "passed": bool(
                r.returncode == 0
                and "index cursor cardinality estimate: 3 (total entries)" in out
                and "RESULT_COUNT=3" in out
            ),
            "skip": False,
            "stdout": r.stdout,
            "stderr": r.stderr,
            "returncode": r.returncode,
        }
    )

    r = _run_case("off")
    out = (r.stdout or "") + (r.stderr or "")
    tests.append(
        {
            "test": "off_strategy_reports_no_cardinality_but_stays_correct",
            "description": "With cardinalityStrategy \"off\", no cardinality trace line is emitted, and results are still correct",
            "passed": bool(
                r.returncode == 0
                and "index cursor cardinality estimate" not in out
                and "RESULT_COUNT=3" in out
            ),
            "skip": False,
            "stdout": r.stdout,
            "stderr": r.stderr,
            "returncode": r.returncode,
        }
    )

    r = _run_case("bogus")
    out = (r.stdout or "") + (r.stderr or "")
    tests.append(
        {
            "test": "unrecognized_strategy_throws_at_adapter_creation",
            "description": "An unrecognized cardinalityStrategy value throws a clear error rather than silently falling back to a default",
            "passed": bool(
                r.returncode != 0
                and 'cardinalityStrategy' in out
                and 'bogus' in out
            ),
            "skip": False,
            "stdout": r.stdout,
            "stderr": r.stderr,
            "returncode": r.returncode,
        }
    )

    return {"description": description, "tests": tests}
