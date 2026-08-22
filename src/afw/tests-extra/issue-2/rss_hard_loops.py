#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Opt-in RSS soaks for issue #2 hard-loop Adaptive Scripts.

Not in default `afwdev test -j`. Expected to fail on develop until optional
free / same-size reuse is real. Measure-only: AFW_ISSUE2_RSS_ASSERT=0.

    afwdev test -T src/afw/tests-extra/issue-2 --show-all
    AFW_ISSUE2_WORKLOAD=integer_assign,object_prop_assign \\
        AFW_ISSUE2_DURATION_S=15 afwdev test -T src/afw/tests-extra/issue-2 --show-all
"""

from __future__ import print_function

import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
if HERE not in sys.path:
    sys.path.insert(0, HERE)

from _rss import format_report, sample_afw_script, workload_path  # noqa: E402


# After warmup, RSS above this rate is "still leaking".
# Live integer_assign on develop (2026-08-22) was ~187 MiB/s.
STABLE_MAX_KIB_S = 8 * 1024  # 8 MiB/s

# Harness control (array_append) must grow at least this fast.
GROWTH_MIN_KIB_S = 256  # 256 KiB/s


WORKLOADS = [
    {
        "name": "integer_assign",
        "description": "i = i + 1 in a hard loop (scalar slot overwrite)",
        "expect_growth": False,
    },
    {
        "name": "object_prop_assign",
        "description": "o.x = i overlay set (S4)",
        "expect_growth": False,
    },
    {
        "name": "array_index_assign",
        "description": "a[0] = i element overwrite",
        "expect_growth": False,
    },
    {
        "name": "object_rebind",
        "description": "o = { n: i } each iteration (unmanaged face)",
        "expect_growth": False,
    },
    {
        "name": "array_rebind",
        "description": "a = [i] each iteration (unmanaged face)",
        "expect_growth": False,
    },
    {
        "name": "string_same_size",
        "description": "overwrite a string with another same-length literal",
        "expect_growth": False,
    },
    {
        "name": "function_return",
        "description": "i = f() where f returns a temp integer",
        "expect_growth": False,
    },
    {
        "name": "try_catch",
        "description": "throw and catch every iteration",
        "expect_growth": False,
    },
    {
        "name": "closure_rebind",
        "description": "rebind a closure that captures a per-iteration let",
        "expect_growth": False,
    },
    {
        "name": "compile_once_eval",
        "description": "compile once, evaluate in a loop (inner heap wrap)",
        "expect_growth": False,
    },
    {
        "name": "array_push_pop",
        "description": "push then pop; capacity may grow once",
        "expect_growth": False,
    },
    {
        "name": "empty_loop",
        "description": "empty while (true) baseline",
        "expect_growth": False,
    },
    {
        "name": "array_append",
        "description": "unbounded push (harness control: RSS must grow)",
        "expect_growth": True,
    },
]


def _env_float(name, default):
    raw = os.environ.get(name)
    if raw is None or raw == "":
        return default
    return float(raw)


def _env_bool(name, default):
    raw = os.environ.get(name)
    if raw is None or raw == "":
        return default
    return raw.strip().lower() not in ("0", "false", "no", "off")


def _selected():
    raw = os.environ.get("AFW_ISSUE2_WORKLOAD") or ""
    raw = raw.strip()
    if not raw:
        return list(WORKLOADS)
    wanted = [p.strip() for p in raw.split(",") if p.strip()]
    by_name = {w["name"]: w for w in WORKLOADS}
    missing = [n for n in wanted if n not in by_name]
    if missing:
        raise ValueError(
            "unknown AFW_ISSUE2_WORKLOAD: %s (have %s)" % (
                ", ".join(missing),
                ", ".join(w["name"] for w in WORKLOADS)))
    return [by_name[n] for n in wanted]


def _one_line(result):
    slope = result.get("slope_kib_s")
    samples = result.get("samples") or []
    last = samples[-1] if samples else {}
    if slope is None:
        return "no slope"
    return "slope=%.1f KiB/s (%.2f MiB/s) last_rss=%s kB" % (
        slope, slope / 1024.0, last.get("VmRSS"))


def _judge(workload, result, assert_on):
    name = workload["name"]
    report = format_report(name, result)
    early = result.get("died_early")
    if early:
        return False, report + " (process exited before samples finished)"

    slope = result.get("slope_kib_s")
    if slope is None:
        return False, report + " (not enough RSS samples)"

    summary = _one_line(result)
    if not assert_on:
        return True, summary + " (assert off)"

    if workload.get("expect_growth"):
        if slope < GROWTH_MIN_KIB_S:
            return False, (
                report
                + " (expected growth >= %.0f KiB/s; sampler or script may be broken)"
                % GROWTH_MIN_KIB_S
            )
        return True, summary

    if slope > STABLE_MAX_KIB_S:
        return False, (
            report
            + " (leak: slope > %.0f KiB/s after warmup; optional free not holding)"
            % STABLE_MAX_KIB_S
        )
    return True, summary


def run():
    duration_s = _env_float("AFW_ISSUE2_DURATION_S", 8.0)
    interval_s = _env_float("AFW_ISSUE2_INTERVAL_S", 2.0)
    warmup_s = _env_float("AFW_ISSUE2_WARMUP_S", 2.0)
    assert_on = _env_bool("AFW_ISSUE2_RSS_ASSERT", True)

    tests = []
    try:
        selected = _selected()
    except ValueError as e:
        return {
            "description": "issue #2 hard-loop RSS lab",
            "tests": [{
                "test": "select",
                "description": str(e),
                "passed": False,
                "skip": False,
                "error": str(e),
            }],
        }

    for w in selected:
        path = workload_path(w["name"])
        result = sample_afw_script(
            path,
            duration_s=duration_s,
            interval_s=interval_s,
            warmup_s=warmup_s,
        )
        passed, error = _judge(w, result, assert_on)
        tests.append({
            "test": w["name"],
            "description": w["description"],
            "passed": bool(passed),
            "skip": False,
            "error": None if passed else error,
        })
        # Always keep the numbers in error-or-description for --show-all.
        if passed:
            tests[-1]["description"] = w["description"] + " — " + error

    return {
        "description": (
            "issue #2 hard-loop RSS lab "
            "(duration=%.1fs interval=%.1fs warmup=%.1fs assert=%s)"
            % (duration_s, interval_s, warmup_s, assert_on)
        ),
        "tests": tests,
    }
