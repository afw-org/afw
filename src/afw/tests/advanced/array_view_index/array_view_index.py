#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
C-array view get at index === count is not found.

Script memory arrays already used >=. The view used >, so a script
a[length] on a literal does not catch this impl.
"""

from __future__ import print_function

import os
import subprocess
import tempfile


def _apr_includes():
    try:
        out = subprocess.check_output(
            ["apr-1-config", "--includes"], text=True)
        return out.split()
    except (OSError, subprocess.CalledProcessError):
        return ["-I/usr/include/apr-1.0"]


def _compile_probe(dest):
    here = os.path.dirname(os.path.abspath(__file__))
    src = os.path.join(here, "array_view_index_probe.c")
    include_afw = os.environ.get("AFW_INCLUDE_DIR", "/usr/local/include/afw")
    libdir = os.environ.get("AFW_LIB_DIR", "/usr/local/lib/afw")
    cmd = [
        "cc", "-O0", "-g",
        "-I", include_afw,
    ]
    cmd.extend(_apr_includes())
    cmd.extend([
        "-o", dest, src,
        "-L", libdir, "-Wl,-rpath," + libdir,
        "-lafw",
    ])
    subprocess.check_call(cmd)


def _case(name, description, passed, error=None):
    return {
        "test": name,
        "description": description,
        "passed": bool(passed),
        "skip": False,
        "error": error,
    }


def run():
    description = "C-array view index === count is not found"
    tests = []
    work = tempfile.mkdtemp(prefix="afw_array_view_index_")
    probe = os.path.join(work, "array_view_index_probe")

    try:
        _compile_probe(probe)
    except Exception as e:
        return {
            "description": description,
            "tests": [
                _case(
                    "compile_probe",
                    "Compile array view index probe against libafw",
                    False,
                    str(e),
                )
            ],
        }

    cases = [
        (
            "direct",
            "counted non-indirect view: index === count is not found",
        ),
        (
            "empty",
            "empty view with live storage: index 0 is not found",
        ),
        (
            "indirect",
            "counted indirect view: index === count is not found",
        ),
    ]
    for name, desc in cases:
        r = subprocess.run(
            [probe, name],
            capture_output=True,
            text=True,
            timeout=60,
        )
        err = (r.stderr or "").strip() or (r.stdout or "").strip()
        tests.append(_case(
            name,
            desc,
            passed=(r.returncode == 0),
            error=None if r.returncode == 0 else (
                err or "exit {}".format(r.returncode)),
        ))

    return {
        "description": description,
        "tests": tests,
    }
