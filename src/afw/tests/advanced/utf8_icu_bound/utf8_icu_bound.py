#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
to_lower / compare_ignore_case bound a truncated UTF-8 sequence.

Script constructors validate UTF-8. Hand-set .s/.len can still present
a truncated multi-byte sequence to the ICU walk.
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
    src = os.path.join(here, "utf8_icu_bound_probe.c")
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
    description = "UTF-8 to_lower/compare bound truncated sequences"
    tests = []
    work = tempfile.mkdtemp(prefix="afw_utf8_icu_bound_")
    probe = os.path.join(work, "utf8_icu_bound_probe")

    try:
        _compile_probe(probe)
    except Exception as e:
        return {
            "description": description,
            "tests": [
                _case(
                    "compile_probe",
                    "Compile UTF-8 ICU bound probe against libafw",
                    False,
                    str(e),
                )
            ],
        }

    cases = [
        (
            "to_lower-valid",
            "to_lower of valid UTF-8, including already-lower",
        ),
        (
            "to_lower-truncated",
            "to_lower of a truncated multi-byte sequence throws",
        ),
        (
            "compare-valid",
            "compare_ignore_case of valid UTF-8",
        ),
        (
            "compare-truncated",
            "compare_ignore_case of a truncated sequence throws",
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
