#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
LDAP filter splice helpers: RFC 4515 escape, attr token, nesting.

A retrieve through the adapter needs a live directory and schema
metadata (value_to_bv). Script cannot reach the splice. This boots a
C probe against installed libafwldap and calls the helpers directly.
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
    src = os.path.join(here, "ldap_filter_probe.c")
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
        "-lafwldap", "-lafw",
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
    description = "LDAP filter escape, attr token, and nesting"
    tests = []
    work = tempfile.mkdtemp(prefix="afw_ldap_filter_")
    probe = os.path.join(work, "ldap_filter_probe")

    try:
        _compile_probe(probe)
    except Exception as e:
        return {
            "description": description,
            "tests": [
                _case(
                    "compile_probe",
                    "Compile LDAP filter probe against libafwldap",
                    False,
                    str(e),
                )
            ],
        }

    cases = [
        (
            "escape-plain",
            "plain and UTF-8 values are unchanged",
        ),
        (
            "escape-specials",
            "NUL ( ) * \\ become RFC 4515 hex escapes",
        ),
        (
            "attr-ok",
            "descr, numericoid, and option names are accepted",
        ),
        (
            "attr-bad",
            "empty or illegal attribute types are rejected",
        ),
        (
            "depth-ok",
            "256-deep query-criteria chain is accepted",
        ),
        (
            "depth-too-deep",
            "257-deep query-criteria chain is query_too_complex",
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
