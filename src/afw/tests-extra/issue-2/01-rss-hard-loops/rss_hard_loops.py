#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Opt-in RSS / in_use soaks for issue #2 hard-loop Adaptive Scripts.

Not in default `afwdev test -j`. Braced empty `{ }` fails RSS (APR tracker)
with in_use flat; unbraced `i = i + 1` fails both until optional free.
empty_stmt should pass today. Measure-only: AFW_ISSUE2_RSS_ASSERT=0.

    afwdev test -T src/afw/tests-extra/issue-2/01-rss-hard-loops --show-all
    AFW_ISSUE2_WORKLOAD=empty_stmt,empty_loop,integer_assign_no_brace \\
        AFW_ISSUE2_DURATION_S=15 afwdev test -T src/afw/tests-extra/issue-2/01-rss-hard-loops --show-all
"""

from __future__ import print_function

import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
if HERE not in sys.path:
    sys.path.insert(0, HERE)

from _rss import format_report, sample_afw_script, workload_path  # noqa: E402


# After warmup, RSS above this rate is "still leaking".
STABLE_MAX_KIB_S = 8 * 1024  # 8 MiB/s
# in_use bytes/s. empty `{ }` is ~0; unbraced assign was ~270 MiB/s.
STABLE_MAX_IN_USE_B_S = 2 * 1024 * 1024  # 2 MiB/s

GROWTH_MIN_KIB_S = 256  # harness RSS
GROWTH_MIN_IN_USE_B_S = 256 * 1024  # harness in_use


WORKLOADS = [
    {
        "name": "empty_stmt",
        "description": "while (true);  no block, no assign (flat control)",
        "expect_rss_growth": False,
        "expect_in_use_growth": False,
    },
    {
        "name": "empty_loop",
        "description": "while (true) {}  tracker APR; RSS up, in_use flat",
        "expect_rss_growth": False,
        "expect_in_use_growth": False,
    },
    {
        "name": "integer_assign_no_brace",
        "description": "unbraced i = i + 1 (scalar in_use)",
        "expect_rss_growth": False,
        "expect_in_use_growth": False,
    },
    {
        "name": "integer_assign",
        "description": "braced i = i + 1 (tracker RSS + scalar in_use)",
        "expect_rss_growth": False,
        "expect_in_use_growth": False,
    },
    {
        "name": "object_prop_assign_no_brace",
        "description": "unbraced o.x = i overlay set (S4)",
        "expect_rss_growth": False,
        "expect_in_use_growth": False,
    },
    {
        "name": "object_prop_assign",
        "description": "braced o.x = i overlay set (S4)",
        "expect_rss_growth": False,
        "expect_in_use_growth": False,
    },
    {
        "name": "array_index_assign_no_brace",
        "description": "unbraced a[0] = i",
        "expect_rss_growth": False,
        "expect_in_use_growth": False,
    },
    {
        "name": "array_index_assign",
        "description": "braced a[0] = i",
        "expect_rss_growth": False,
        "expect_in_use_growth": False,
    },
    {
        "name": "object_rebind",
        "description": "o = { n: i } each iteration (unmanaged face)",
        "expect_rss_growth": False,
        "expect_in_use_growth": False,
    },
    {
        "name": "array_rebind",
        "description": "a = [i] each iteration (unmanaged face)",
        "expect_rss_growth": False,
        "expect_in_use_growth": False,
    },
    {
        "name": "string_same_size",
        "description": "overwrite a string with another same-length literal",
        "expect_rss_growth": False,
        "expect_in_use_growth": False,
    },
    {
        "name": "function_return",
        "description": "i = f() where f returns a temp integer",
        "expect_rss_growth": False,
        "expect_in_use_growth": False,
    },
    {
        "name": "try_catch",
        "description": "throw and catch every iteration",
        "expect_rss_growth": False,
        "expect_in_use_growth": False,
    },
    {
        "name": "closure_rebind",
        "description": "rebind a closure that captures a per-iteration let",
        "expect_rss_growth": False,
        "expect_in_use_growth": False,
    },
    {
        "name": "compile_once_eval",
        "description": "compile once, evaluate in a loop (inner heap wrap)",
        "expect_rss_growth": False,
        "expect_in_use_growth": False,
    },
    {
        "name": "array_push_pop",
        "description": "push then pop; capacity may grow once",
        "expect_rss_growth": False,
        "expect_in_use_growth": False,
    },
    {
        "name": "array_append",
        "description": "unbounded push (harness: RSS and in_use must grow)",
        "expect_rss_growth": True,
        "expect_in_use_growth": True,
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
    rss = "rss=n/a"
    if slope is not None:
        rss = "rss=%.1f KiB/s last=%s kB" % (slope, last.get("VmRSS"))
    iu = result.get("in_use_slope_b_s")
    if iu is None:
        in_use = "in_use=n/a"
    else:
        in_use = "in_use=%.2f MiB/s %s->%s" % (
            iu / 1024.0 / 1024.0,
            result.get("in_use_first"),
            result.get("in_use_last"))
    return rss + "  " + in_use


def _expect_rss_growth(workload):
    if "expect_rss_growth" in workload:
        return workload["expect_rss_growth"]
    return bool(workload.get("expect_growth"))


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

    problems = []
    want_rss = _expect_rss_growth(workload)
    if want_rss:
        if slope < GROWTH_MIN_KIB_S:
            problems.append(
                "RSS expected to grow >= %.0f KiB/s, got %.1f"
                % (GROWTH_MIN_KIB_S, slope))
    elif slope > STABLE_MAX_KIB_S:
        iu0 = result.get("in_use_slope_b_s")
        if iu0 is not None and iu0 <= STABLE_MAX_IN_USE_B_S:
            why = "APR; in_use flat"
        else:
            why = "APR and/or asked-for"
        problems.append(
            "RSS leak %.1f KiB/s > %.0f (%s)"
            % (slope, STABLE_MAX_KIB_S, why))

    iu = result.get("in_use_slope_b_s")
    want_iu = workload.get("expect_in_use_growth")
    if iu is not None and want_iu is not None:
        if want_iu:
            if iu < GROWTH_MIN_IN_USE_B_S:
                problems.append(
                    "in_use expected to grow >= %.0f B/s, got %.0f"
                    % (GROWTH_MIN_IN_USE_B_S, iu))
        elif iu > STABLE_MAX_IN_USE_B_S:
            problems.append(
                "in_use leak %.0f B/s > %.0f (AFW malloc not given back)"
                % (iu, STABLE_MAX_IN_USE_B_S))

    if problems:
        return False, report + " (" + "; ".join(problems) + ")"
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
