#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Local-mode chunk length must stay within the protocol cap.

A length line over the cap is an invalid chunk. Do not send that many
bytes — the parse rejects before the read loop.
"""

from __future__ import print_function

import subprocess

from _afwdev.test.orchestrated.hosts.local import (
    build_action_session_stdin,
    parse_local_json_responses,
)


# Must stay above AFW_COMMAND_LOCAL_MAX_CHUNK in
# src/afw_command/afw_command_local_server.c (10,000,000).
OVER_CAP = b"10000001\n"


def _case(name, description, passed, error=None):
    return {
        "test": name,
        "description": description,
        "passed": bool(passed),
        "skip": False,
        "error": error,
    }


def _run_local(stdin, timeout=10):
    return subprocess.run(
        ["afw", "--local", "1"],
        input=stdin,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        timeout=timeout,
    )


def run():
    description = (
        "afw --local rejects a chunk length over the protocol cap; "
        "a small valid session still works"
    )
    tests = []

    try:
        valid = _run_local(build_action_session_stdin(
            "application/json",
            {"function": "eval<script>", "source": "return 3;"},
        ))
        responses = parse_local_json_responses(valid.stdout or b"")
        valid_ok = False
        if responses:
            last = responses[-1]
            if isinstance(last, dict) and last.get("status") == "success":
                result = last.get("result")
                valid_ok = result == 3 or result == "3"
        tests.append(_case(
            "small-valid-chunk",
            "A small local-mode session still runs",
            passed=valid_ok,
            error=None if valid_ok else (
                "returncode={} stdout={!r} stderr={!r}".format(
                    valid.returncode,
                    (valid.stdout or b"")[:300],
                    (valid.stderr or b"")[:300])),
        ))

        over = _run_local(OVER_CAP)
        over_text = ((over.stdout or b"") + (over.stderr or b"")).decode(
            "utf-8", errors="replace")
        over_ok = over.returncode != 0 and "Invalid chunk" in over_text
        tests.append(_case(
            "over-cap-length",
            "A chunk length over the cap is Invalid chunk",
            passed=over_ok,
            error=None if over_ok else (
                "returncode={} out={!r}".format(
                    over.returncode, over_text[:400])),
        ))

        junk = _run_local(b"12x\n")
        junk_text = ((junk.stdout or b"") + (junk.stderr or b"")).decode(
            "utf-8", errors="replace")
        junk_ok = junk.returncode != 0 and "Invalid chunk" in junk_text
        tests.append(_case(
            "non-digit-length",
            "A non-digit in the length line is Invalid chunk",
            passed=junk_ok,
            error=None if junk_ok else (
                "returncode={} out={!r}".format(
                    junk.returncode, junk_text[:400])),
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
            "Run local-mode chunk length cases",
            passed=False,
            error=str(e),
        ))

    return {
        "description": description,
        "tests": tests,
    }
