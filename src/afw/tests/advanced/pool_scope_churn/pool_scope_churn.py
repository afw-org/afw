#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Short-lived pool churn: tracker vs inherit-heap (scope-shaped).

Prints METRICS (poolBytesInUse, peakPoolBytesInUse, poolChunkBytes,
peakPoolChunkBytes, elapsed_ns) using the same names as thread/xctx
counters so a later harvest or flag can reuse them. Pass/fail is leak
only; heap vs tracker speed is not a gate.
"""

import os
import shutil
import subprocess
import tempfile

from _afwdev.test import context as test_context
from _afwdev.test.c_probe import compile_c_probe
from _afwdev.test.valgrind_report import valgrind_error_message


def _case(name, description, passed, error=None):
    return {
        "test": name,
        "description": description,
        "passed": bool(passed),
        "skip": False,
        "error": error,
    }


def _want_valgrind():
    return test_context.options().get("mode") == "valgrind"


def _run(probe, name, work, timeout, use_valgrind):
    cmd = [probe, name]
    xml_path = None
    if use_valgrind:
        valgrind = shutil.which("valgrind")
        if not valgrind:
            return 1, "", "valgrind not found on PATH"
        xml_path = os.path.join(work, "valgrind-" + name + ".xml")
        log_path = os.path.join(work, "valgrind-" + name + ".log")
        cmd = [
            valgrind,
            "--xml=yes",
            "--xml-file=" + xml_path,
            "--log-file=" + log_path,
            "--show-possibly-lost=no",
        ] + cmd
    r = subprocess.run(
        cmd, capture_output=True, text=True, timeout=timeout)
    out = (r.stdout or "").strip()
    err = (r.stderr or "").strip()
    if r.returncode != 0:
        return r.returncode, out, err or "exit {}".format(r.returncode)
    if use_valgrind and xml_path and os.path.isfile(xml_path):
        try:
            with open(xml_path, encoding="utf-8", errors="replace") as f:
                xml_text = f.read()
        except OSError:
            xml_text = ""
        if "<error>" in xml_text:
            return 1, out, valgrind_error_message(xml_text)
    return 0, out, err


def _metrics_line(stdout):
    for line in (stdout or "").splitlines():
        if line.startswith("METRICS "):
            return line[len("METRICS "):].strip()
    return ""


def run():
    here = os.path.dirname(os.path.abspath(__file__))
    src = os.path.join(here, "pool_scope_churn_probe.c")
    use_valgrind = _want_valgrind()
    timeout = 300 if use_valgrind else 60
    cases = [
        (
            "tracker",
            "3000 short trackers under xctx->p (today's scope store)",
        ),
        (
            "heap_4k",
            "3000 inherit heaps, 4k chunks (compile-sized)",
        ),
        (
            "heap_64k",
            "3000 inherit heaps, default 64k chunks",
        ),
    ]
    tests = []
    work_cm = tempfile.TemporaryDirectory(prefix="afw_pool_scope_churn_")
    try:
        dest = os.path.join(work_cm.name, "pool_scope_churn_probe")
        try:
            compile_c_probe(dest, src)
        except subprocess.CalledProcessError as e:
            detail = (
                (e.stderr or "").strip()
                or (e.stdout or "").strip()
                or str(e))
            return {
                "description": "Short-lived pool churn (scope-shaped)",
                "tests": [
                    _case(
                        "compile_probe",
                        "Compile pool_scope_churn_probe.c",
                        False,
                        detail,
                    )
                ],
            }
        for name, desc in cases:
            try:
                rc, out, err = _run(
                    dest, name, work_cm.name, timeout, use_valgrind)
            except subprocess.TimeoutExpired:
                tests.append(_case(
                    name, desc, False,
                    "timed out after {}s".format(timeout)))
                continue
            metrics = _metrics_line(out)
            shown = desc
            if metrics:
                shown = desc + " [" + metrics + "]"
                print(name + ": " + metrics, flush=True)
            tests.append(_case(
                name,
                shown,
                passed=(rc == 0),
                error=None if rc == 0 else (err or out),
            ))
    finally:
        work_cm.cleanup()
    return {
        "description": "Short-lived pool churn (scope-shaped)",
        "tests": tests,
    }
