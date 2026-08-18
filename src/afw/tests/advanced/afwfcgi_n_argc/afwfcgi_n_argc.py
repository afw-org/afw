#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
afwfcgi -n without a thread count must print usage and exit, not start.
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


def run():
    description = (
        "afwfcgi -n with no following argument prints usage and exits"
    )
    tests = []

    try:
        missing = subprocess.run(
            ["afwfcgi", "-n"],
            stdin=subprocess.DEVNULL,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            timeout=10,
        )
        err = (missing.stderr or b"").decode("utf-8", errors="replace")
        missing_ok = (
            missing.returncode != 0
            and "Usage:" in err
        )
        tests.append(_case(
            "n-missing-argument",
            "afwfcgi -n with no count prints usage and is non-zero",
            passed=missing_ok,
            error=None if missing_ok else (
                "returncode={} stderr={!r}".format(
                    missing.returncode, err[:400])),
        ))

        help_run = subprocess.run(
            ["afwfcgi", "--help"],
            stdin=subprocess.DEVNULL,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            timeout=10,
        )
        help_err = (help_run.stderr or b"").decode("utf-8", errors="replace")
        help_ok = help_run.returncode == 0 and "Usage:" in help_err
        tests.append(_case(
            "help-still-works",
            "afwfcgi --help still prints usage and exits 0",
            passed=help_ok,
            error=None if help_ok else (
                "returncode={} stderr={!r}".format(
                    help_run.returncode, help_err[:400])),
        ))
    except FileNotFoundError as e:
        tests.append(_case(
            "setup",
            "afwfcgi on PATH",
            passed=False,
            error="afwfcgi not on PATH (./afwdev build --cdev --install): "
            + str(e),
        ))
    except Exception as e:
        tests.append(_case(
            "setup",
            "Run afwfcgi CLI cases",
            passed=False,
            error=str(e),
        ))

    return {
        "description": description,
        "tests": tests,
    }
