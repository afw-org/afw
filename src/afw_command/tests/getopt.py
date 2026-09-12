#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
afw table-driven getopt: help, version, unknown flag, missing argument.
"""

from __future__ import print_function

import subprocess


def _case(name, description, passed, error=None):
    return {
        "test": name,
        "description": description,
        "passed": bool(passed),
        "skip": False,
        "error": error,
    }


def _run(args):
    return subprocess.run(
        ["afw"] + args,
        stdin=subprocess.DEVNULL,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        timeout=10,
    )


def run():
    description = "afw command-line options (table-driven getopt)"
    tests = []

    try:
        help_run = _run(["--help"])
        err = (help_run.stderr or b"").decode("utf-8", errors="replace")
        help_ok = (
            help_run.returncode == 0
            and "Usage:" in err
            and "--syntax" in err
        )
        tests.append(_case(
            "help-long",
            "afw --help prints usage from the option table and exits 0",
            passed=help_ok,
            error=None if help_ok else (
                "returncode={} stderr={!r}".format(
                    help_run.returncode, err[:400])),
        ))

        h_run = _run(["-h"])
        h_err = (h_run.stderr or b"").decode("utf-8", errors="replace")
        h_ok = h_run.returncode == 0 and "Usage:" in h_err
        tests.append(_case(
            "help-short",
            "afw -h prints usage and exits 0",
            passed=h_ok,
            error=None if h_ok else (
                "returncode={} stderr={!r}".format(
                    h_run.returncode, h_err[:400])),
        ))

        ver = _run(["--version"])
        ver_err = (ver.stderr or b"").decode("utf-8", errors="replace")
        ver_ok = ver.returncode == 0 and len(ver_err.strip()) > 0
        tests.append(_case(
            "version-long",
            "afw --version prints a version string and exits 0",
            passed=ver_ok,
            error=None if ver_ok else (
                "returncode={} stderr={!r}".format(
                    ver.returncode, ver_err[:400])),
        ))

        eq = _run(["--syntax=script", "-x", "1"])
        eq_err = (eq.stderr or b"").decode("utf-8", errors="replace")
        eq_ok = eq.returncode == 0
        tests.append(_case(
            "long-equals",
            "afw --syntax=script -x 1 is accepted",
            passed=eq_ok,
            error=None if eq_ok else (
                "returncode={} stderr={!r}".format(
                    eq.returncode, eq_err[:400])),
        ))

        missing = _run(["--syntax"])
        miss_err = (missing.stderr or b"").decode("utf-8", errors="replace")
        missing_ok = missing.returncode != 0 and "Try --help." in miss_err
        tests.append(_case(
            "missing-argument",
            "afw --syntax with no value is non-zero and says Try --help",
            passed=missing_ok,
            error=None if missing_ok else (
                "returncode={} stderr={!r}".format(
                    missing.returncode, miss_err[:400])),
        ))

        bad = _run(["--nope"])
        bad_err = (bad.stderr or b"").decode("utf-8", errors="replace")
        bad_ok = bad.returncode != 0 and "Try --help." in bad_err
        tests.append(_case(
            "unknown-long",
            "afw --nope is non-zero and says Try --help",
            passed=bad_ok,
            error=None if bad_ok else (
                "returncode={} stderr={!r}".format(
                    bad.returncode, bad_err[:400])),
        ))
    except FileNotFoundError as e:
        tests.append(_case(
            "setup",
            "afw on PATH",
            passed=False,
            error="afw not on PATH (./afwdev build --cdev --install): "
            + str(e),
        ))
    except Exception as e:
        tests.append(_case(
            "setup",
            "Run afw CLI getopt cases",
            passed=False,
            error=str(e),
        ))

    return {
        "description": description,
        "tests": tests,
    }
