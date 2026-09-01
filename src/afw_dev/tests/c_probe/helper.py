#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
afwdev C-probe helper: compile, named cases, valgrind wrap.
"""

import os
import shutil
import tempfile

from _afwdev.test.c_probe import run_c_probe

_HERE = os.path.dirname(os.path.abspath(__file__))
_FIXTURES = os.path.join(_HERE, "_fixtures")


def _result_by_name(result):
    return {t["test"]: t for t in result.get("tests") or []}


def run():
    description = "afwdev C-probe helper compile and named cases"
    tests = []

    named = run_c_probe(
        os.path.join(_FIXTURES, "named_cases_probe.c"),
        "named cases",
        [
            ("ok", "exit 0 is pass"),
            ("fail", "nonzero is fail"),
            ("no-such-case", "unknown name is fail"),
        ],
        valgrind=False,
    )
    by_name = _result_by_name(named)
    ok = by_name.get("ok") or {}
    fail = by_name.get("fail") or {}
    missing = by_name.get("no-such-case") or {}
    tests.append({
        "test": "named-ok",
        "description": "exit 0 is reported as passed",
        "passed": ok.get("passed") is True,
        "skip": False,
        "error": ok.get("error"),
    })
    tests.append({
        "test": "named-fail",
        "description": "nonzero exit is reported as failed with stderr",
        "passed": (
            fail.get("passed") is False
            and "intentional fail" in (fail.get("error") or "")
        ),
        "skip": False,
        "error": None if fail.get("passed") is False else (
            fail.get("error") or "expected fail case to fail"),
    })
    tests.append({
        "test": "named-unknown",
        "description": "unknown case name is failed",
        "passed": missing.get("passed") is False,
        "skip": False,
        "error": None if missing.get("passed") is False else (
            missing.get("error") or "expected unknown case to fail"),
    })

    compile_fail = run_c_probe(
        os.path.join(_FIXTURES, "compile_fail_probe.c"),
        "compile fail",
        [("ok", "never run")],
        valgrind=False,
    )
    compile_cases = compile_fail.get("tests") or []
    compile_one = compile_cases[0] if compile_cases else {}
    tests.append({
        "test": "compile-fail",
        "description": "compiler error is a compile_probe failure",
        "passed": bool(
            compile_one.get("test") == "compile_probe"
            and compile_one.get("passed") is False
            and compile_one.get("error")
        ),
        "skip": False,
        "error": None if compile_one.get("passed") is False else (
            compile_one.get("error") or "expected compile to fail"),
    })

    extra_include = {"tests": []}
    stale_dir = tempfile.mkdtemp(prefix="afw_c_probe_stale_")
    good_dir = tempfile.mkdtemp(prefix="afw_c_probe_good_")
    prev_include = os.environ.get("AFW_INCLUDE_DIR")
    try:
        with open(os.path.join(stale_dir, "override_marker.h"), "w",
                  encoding="utf-8") as f:
            f.write("#error stale installed header\n")
        with open(os.path.join(good_dir, "override_marker.h"), "w",
                  encoding="utf-8") as f:
            f.write("#define MARKER 1\n")
        os.environ["AFW_INCLUDE_DIR"] = stale_dir
        extra_include = run_c_probe(
            os.path.join(_FIXTURES, "extra_include_probe.c"),
            "extra -I before install include",
            [("ok", "source-tree -I wins over leftover install header")],
            extra_cflags=("-I", good_dir),
            libraries=(),
            valgrind=False,
        )
    finally:
        if prev_include is None:
            os.environ.pop("AFW_INCLUDE_DIR", None)
        else:
            os.environ["AFW_INCLUDE_DIR"] = prev_include
        shutil.rmtree(stale_dir, ignore_errors=True)
        shutil.rmtree(good_dir, ignore_errors=True)
    extra_one = (extra_include.get("tests") or [{}])[0]
    tests.append({
        "test": "extra-include-wins",
        "description":
            "extra -I is searched before the install include dir",
        "passed": extra_one.get("passed") is True,
        "skip": False,
        "error": extra_one.get("error"),
    })

    missing_src = run_c_probe(
        os.path.join(_FIXTURES, "does_not_exist_probe.c"),
        "missing source",
        [("ok", "never run")],
        valgrind=False,
    )
    missing_one = (missing_src.get("tests") or [{}])[0]
    tests.append({
        "test": "missing-source",
        "description": "missing .c is a compile_probe failure",
        "passed": (
            missing_one.get("test") == "compile_probe"
            and missing_one.get("passed") is False
        ),
        "skip": False,
        "error": None if missing_one.get("passed") is False else (
            missing_one.get("error") or "expected missing source to fail"),
    })

    have_valgrind = shutil.which("valgrind") is not None
    if not have_valgrind:
        tests.append({
            "test": "valgrind-throw",
            "description": "throwing probe under valgrind with suite suppressions",
            "passed": True,
            "skip": True,
            "skipReason": "valgrind not on PATH",
        })
        tests.append({
            "test": "valgrind-ok",
            "description": "non-throwing probe under valgrind",
            "passed": True,
            "skip": True,
            "skipReason": "valgrind not on PATH",
        })
    else:
        vg_throw = run_c_probe(
            os.path.join(_FIXTURES, "throw_probe.c"),
            "throw under valgrind",
            [("throw", "expected throw does not count unwind as failure")],
            valgrind=True,
        )
        throw_one = (vg_throw.get("tests") or [{}])[0]
        tests.append({
            "test": "valgrind-throw",
            "description":
                "throwing probe under valgrind with suite suppressions",
            "passed": throw_one.get("passed") is True,
            "skip": False,
            "error": throw_one.get("error"),
        })
        vg_ok = run_c_probe(
            os.path.join(_FIXTURES, "throw_probe.c"),
            "ok under valgrind",
            [("ok", "environment create only")],
            valgrind=True,
        )
        ok_one = (vg_ok.get("tests") or [{}])[0]
        tests.append({
            "test": "valgrind-ok",
            "description": "non-throwing probe under valgrind",
            "passed": ok_one.get("passed") is True,
            "skip": False,
            "error": ok_one.get("error"),
        })

    return {
        "description": description,
        "tests": tests,
    }
