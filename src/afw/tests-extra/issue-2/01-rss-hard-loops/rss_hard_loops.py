#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Opt-in RSS / in_use soaks for issue #2 hard-loop Adaptive Scripts.

Not in default `afwdev test -j`. Each workload has a class: flat
(tight in_use), climb (known leftover, fail at ~2x last 15s), or
grow (array_append; sampler must see growth). Disaster RSS bar is
still 8 MiB/s. Live table: README.md. Measure-only:
AFW_ISSUE2_RSS_ASSERT=0.

    afwdev test -T src/afw/tests-extra/issue-2/01-rss-hard-loops --show-all
    AFW_ISSUE2_WORKLOAD=empty_stmt,empty_loop,integer_assign_no_brace \\
        AFW_ISSUE2_DURATION_S=15 afwdev test -T src/afw/tests-extra/issue-2/01-rss-hard-loops --show-all
"""

from __future__ import print_function

import os
import shutil
import sys
import tempfile
import threading
from concurrent.futures import ThreadPoolExecutor, as_completed

HERE = os.path.dirname(os.path.abspath(__file__))
if HERE not in sys.path:
    sys.path.insert(0, HERE)

from _rss import format_report, sample_afw_script, workload_path  # noqa: E402


# Disaster RSS. Kernel wander with flat in_use does not use this.
STABLE_MAX_KIB_S = 8 * 1024  # 8 MiB/s
# Flat class: leftover RC at 0.15 MiB/s must fail; gdb noise is B/s.
FLAT_MAX_IN_USE_B_S = 64 * 1024  # 64 KiB/s
GROWTH_MIN_KIB_S = 256  # harness RSS
GROWTH_MIN_IN_USE_B_S = 256 * 1024  # harness in_use


def _mib_s(n):
    """n MiB/s as integer bytes/s."""
    return int(n * 1024 * 1024)


def _flat(name, description, **extra):
    w = {
        "name": name,
        "description": description,
        "kind": "flat",
    }
    w.update(extra)
    return w


def _climb(name, description, max_in_use_mib_s, **extra):
    w = {
        "name": name,
        "description": description,
        "kind": "climb",
        # ~2x last 15s in_use so a jump (clone 0.2 -> 2.3) fails.
        "max_in_use_b_s": _mib_s(max_in_use_mib_s),
    }
    w.update(extra)
    return w


WORKLOADS = [
    _flat("empty_stmt",
          "while (true);  no block, no assign (flat control)"),
    _flat("empty_loop",
          "while (true) {}  empty body frame last-releases"),
    _flat("integer_assign_no_brace", "unbraced i = i + 1"),
    _flat("integer_assign", "braced i = i + 1"),
    _flat("object_prop_assign_no_brace", "unbraced o.x = i overlay set"),
    _flat("object_prop_assign", "braced o.x = i overlay set"),
    _flat("array_index_assign_no_brace", "unbraced a[0] = i"),
    _flat("array_index_assign", "braced a[0] = i"),
    _flat("object_rebind", "o = { n: i } each iteration"),
    _flat("array_rebind", "a = [i] each iteration"),
    _flat("string_same_size",
          "overwrite a string with another same-length literal"),
    _flat("function_return", "i = f() (flat; leftover wrapper gone)"),
    _flat("try_catch", "throw and catch every iteration"),
    _flat("closure_rebind",
          "rebind a closure that captures a per-iteration let"),
    _flat("compile_once_eval",
          "compile once, evaluate in a loop (inner heap wrap)"),
    _flat("eval_closure_rebind",
          "eval<script> closure overwrite (keep_unit transfer)"),
    _climb("eval_object_rebind",
           "eval<script> object-of-functions overwrite (nested pin leftover; #458)",
           12),
    _flat("array_push_pop", "push then pop (temp on current scope)"),
    _flat("splice_assign", "splice copy-out then assign (length-stable)"),
    _flat("splice_unassigned",
          "splice copy-out never assigned (last stmt add())"),
    _flat("unassigned_temps",
          "unmanaged add() temps (last stmt isolates a scalar)"),
    _flat("readln_loop",
          "readln loop (short lines plus one that grows the buffer)",
          needs_readln_conf=True,
          max_in_use_b_s=128 * 1024),
    _flat("managed_create_assign",
          "create_managed extra-hold then assign "
          "(reverse/slice/filter/map/sort/bag/keys/entries/…)"),
    _flat("managed_create_unassigned",
          "create_managed extra-hold never assigned (last stmt add())"),
    _flat("clone_assign",
          "clone array/object then assign "
          "(create_managed extra-hold root, take nested)"),
    _flat("clone_unassigned",
          "clone array/object never assigned "
          "(last stmt add() so not script_result)"),
    _flat("clone_nested_assign",
          "clone nested object/array then assign and mutate "
          "(clone(o).child / clone(o).arr)"),
    _flat("clone_nested_unassigned",
          "clone nested object/array never assigned "
          "(discard(clone(o).child); last stmt add())"),
    _flat("test_script_assign",
          "test_script create_managed extra-hold then assign"),
    _flat("test_script_unassigned",
          "test_script create_managed extra-hold never assigned "
          "(last stmt add())"),
    _flat("object_rest_assign",
          "object pattern rest dest p unmanaged then assign"),
    _flat("object_rest_unassigned",
          "object pattern rest dest p unmanaged never assigned "
          "(last stmt add())"),
    _flat("compile_listing_assign",
          "compile listing assigned; unit last-released after the dump"),
    _flat("compile_listing_unassigned",
          "compile listing last-releases the unit (last stmt add())"),
    _flat("object_literal_frame",
          "const o = { n: i } held in the body frame"),
    _flat("object_spread_frame",
          "const o = { ...extra, host } held in the body frame"),
    _flat("object_spread_assign",
          "o = { ...extra, host } into an outer slot"),
    _flat("add_properties_frame",
          "const o = add_properties(undefined, ...) in the body frame"),
    _flat("add_properties_unassigned",
          "add_properties(undefined, ...) never assigned (last stmt add())"),
    _flat("array_literal_frame",
          "const a = [i, i] held in the body frame"),
    _flat("array_spread_frame",
          "const a = [0, ...pair] held in the body frame"),
    _flat("create_array_unassigned",
          "create_array(4) never assigned (last stmt add())"),
    {
        "name": "array_append",
        "description": "unbounded push (harness: RSS and in_use must grow)",
        "kind": "grow",
    },
]


def _prepare_readln_conf():
    """Temp application conf + lines file so readln can open_file."""
    tmp = tempfile.mkdtemp(prefix="afw-issue2-readln-")
    files = os.path.join(tmp, "files")
    os.makedirs(files)
    with open(os.path.join(files, "lines.txt"), "w") as f:
        for i in range(50):
            f.write("line-%d\n" % i)
        f.write(("x" * 2000) + "\n")
    conf_path = os.path.join(tmp, "afw.conf")
    with open(conf_path, "w") as f:
        f.write(
            "[\n  {\n    type: \"application\",\n"
            "    applicationId: \"issue2-readln\",\n"
            "    rootFilePaths: { \"data\": \"%s\" }\n"
            "  }\n]\n" % files.replace("\\", "\\\\").replace("\"", "\\\"")
        )
    return tmp, ["--conf", conf_path]


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


def _env_int(name, default):
    raw = os.environ.get(name)
    if raw is None or raw == "":
        return default
    return int(raw)


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


def _kind(workload):
    kind = workload.get("kind")
    if kind in ("flat", "climb", "grow"):
        return kind
    if workload.get("expect_rss_growth") or workload.get("expect_growth"):
        return "grow"
    return "flat"


def _max_in_use_b_s(workload):
    if "max_in_use_b_s" in workload:
        return workload["max_in_use_b_s"]
    kind = _kind(workload)
    if kind == "flat":
        return FLAT_MAX_IN_USE_B_S
    return None


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
    kind = _kind(workload)
    iu = result.get("in_use_slope_b_s")

    if kind == "grow":
        if slope < GROWTH_MIN_KIB_S:
            problems.append(
                "RSS expected to grow >= %.0f KiB/s, got %.1f"
                % (GROWTH_MIN_KIB_S, slope))
        if iu is not None and iu < GROWTH_MIN_IN_USE_B_S:
            problems.append(
                "in_use expected to grow >= %.0f B/s, got %.0f"
                % (GROWTH_MIN_IN_USE_B_S, iu))
    else:
        # Climb leftover is judged by in_use cap (~2x last 15s).
        # Disaster RSS is the flat/grow backstop.
        if kind == "flat" and slope > STABLE_MAX_KIB_S:
            problems.append(
                "RSS leak %.1f KiB/s > %.0f"
                % (slope, STABLE_MAX_KIB_S))
        max_iu = _max_in_use_b_s(workload)
        # gdb miss: first or last in_use 0, a huge pointer as size_t,
        # or a warmup sample far below last while RSS is not climbing
        # (60s object_rest_unassigned: 262144 -> ~4 MiB, RSS 0).
        first_iu = result.get("in_use_first")
        last_iu = result.get("in_use_last")
        if first_iu == 0 or last_iu == 0:
            iu = None
        elif first_iu is not None and last_iu is not None:
            samples = result.get("samples") or []
            last = samples[-1] if samples else {}
            rss_kib = last.get("VmRSS")
            if rss_kib:
                cap = rss_kib * 1024 * 8
                if first_iu > cap or last_iu > cap:
                    iu = None
            if iu is not None and first_iu * 4 < last_iu and (
                    slope is None or slope < 64):
                iu = None
        if iu is not None and max_iu is not None and iu > max_iu:
            problems.append(
                "in_use leak %.0f B/s > %.0f (%s)"
                % (iu, max_iu, kind))

    if problems:
        return False, report + " (" + "; ".join(problems) + ")"
    return True, summary


def _run_one(w, duration_s, interval_s, warmup_s, assert_on, print_lock):
    path = workload_path(w["name"])
    extra_argv = None
    readln_tmp = None
    if w.get("needs_readln_conf"):
        readln_tmp, extra_argv = _prepare_readln_conf()
    try:
        result = sample_afw_script(
            path,
            duration_s=duration_s,
            interval_s=interval_s,
            warmup_s=warmup_s,
            extra_argv=extra_argv,
        )
    finally:
        if readln_tmp:
            shutil.rmtree(readln_tmp, ignore_errors=True)
    passed, error = _judge(w, result, assert_on)
    summary = error if passed else _one_line(result)
    with print_lock:
        print("%s  %s" % (w["name"], summary), file=sys.stderr)
        sys.stderr.flush()
    rec = {
        "test": w["name"] + "  " + summary,
        "description": w["description"],
        "passed": bool(passed),
        "skip": False,
        "error": None if passed else error,
    }
    if passed:
        rec["description"] = w["description"] + " — " + error
    return rec


def run():
    duration_s = _env_float("AFW_ISSUE2_DURATION_S", 15.0)
    interval_s = _env_float("AFW_ISSUE2_INTERVAL_S", 5.0)
    warmup_s = _env_float("AFW_ISSUE2_WARMUP_S", 5.0)
    assert_on = _env_bool("AFW_ISSUE2_RSS_ASSERT", True)
    jobs = max(1, _env_int("AFW_ISSUE2_JOBS", 1))

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

    print_lock = threading.Lock()
    if jobs == 1 or len(selected) == 1:
        tests = [
            _run_one(w, duration_s, interval_s, warmup_s, assert_on,
                     print_lock)
            for w in selected
        ]
    else:
        tests = [None] * len(selected)
        workers = min(jobs, len(selected))
        with ThreadPoolExecutor(max_workers=workers) as pool:
            futs = {
                pool.submit(
                    _run_one, w, duration_s, interval_s, warmup_s,
                    assert_on, print_lock): i
                for i, w in enumerate(selected)
            }
            for fut in as_completed(futs):
                tests[futs[fut]] = fut.result()

    return {
        "description": (
            "issue #2 hard-loop RSS lab "
            "(duration=%.1fs interval=%.1fs warmup=%.1fs assert=%s jobs=%s)"
            % (duration_s, interval_s, warmup_s, assert_on, jobs)
        ),
        "tests": tests,
    }
