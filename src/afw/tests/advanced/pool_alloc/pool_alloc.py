#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Pool malloc rejects a wrapping size.

Boots a tiny C probe against installed libafw. Script integers cannot
reach a wrapping size_t on 64-bit hosts.
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
    src = os.path.join(here, "pool_alloc_probe.c")
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
    description = "Pool malloc rejects a wrapping size"
    tests = []
    work = tempfile.mkdtemp(prefix="afw_pool_alloc_")
    probe = os.path.join(work, "pool_alloc_probe")

    try:
        _compile_probe(probe)
    except Exception as e:
        return {
            "description": description,
            "tests": [
                _case(
                    "compile_probe",
                    "Compile pool alloc probe against libafw",
                    False,
                    str(e),
                )
            ],
        }

    cases = [
        (
            "overflow",
            "pool malloc of SIZE_MAX throws memory",
        ),
        (
            "overflow-subpool",
            "subpool malloc of SIZE_MAX throws memory",
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
