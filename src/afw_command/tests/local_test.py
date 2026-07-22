#!/usr/bin/env python3

#
# Custom Python test for `afw --local` (stdin protocol).
#
# Why a .py test (not .as)?
#   Most AFW tests are Adaptive Script. afwdev also discovers `*.py` under
#   tests/ and runs them in "python" mode: import the module and call run(),
#   which must return a result dict. That is how we add *kinds* of tests the
#   script runner cannot do alone (spawn hosts, binary I/O, multi-step
#   protocols, compile helpers, etc.).
#
#   Contract (see also .cursor/rules/afw-tests.mdc and
#   src/afw_dev/_afwdev/test/modes/python.py):
#     def run() -> {
#       "description": str,
#       "tests": [ {
#         "test": str, "description": str,
#         "passed": bool, "skip": bool, ...
#       }, ... ]
#     }
#   Runner may chdir to a temp work dir with fixtures copied; resolve package
#   root from __file__ if you need the repo (this test only needs PATH `afw`).
#
# Fixtures (same directory, names = test basename):
#   local_test_*_input.txt       — bytes fed to afw --local stdin
#   local_test_*_expect.txt      — expected stdout (after banner normalize)
#   local_test_*_description.txt — optional human description
#   local_test_*_ignore.txt      — if present, skip (counts as passed)
#   local_test_*_actual.txt      — written on run (work dir) for debugging
#
# Version independence:
#   Local mode starts with a length-prefixed banner, e.g.:
#     24\n
#     afw 0.12.0\n
#     \n
#     Local mode.\n
#   The leading number is the byte length of the banner body, so it changes
#   when the version string length changes. We normalize that banner on both
#   actual and expected before compare so package version bumps do not require
#   editing every *_expect.txt.
#

import glob
import os
import re
import subprocess

response = {
    "description": "Test afw --local",
    "tests": []
}

# Length-prefixed local-mode greeting (first stdout chunk).
_LOCAL_BANNER_RE = re.compile(
    br"^\d+\n"
    br"afw [0-9A-Za-z._+-]+\n"
    br"\n"
    br"Local mode\.\n"
)


def normalize_local_stdout(data):
    """Stabilize version-dependent local-mode banner for comparison."""
    if not isinstance(data, (bytes, bytearray)):
        data = str(data).encode("utf-8", errors="surrogateescape")
    return _LOCAL_BANNER_RE.sub(b"@@AFW_LOCAL_BANNER@@\n", data, count=1)


def run():
    # Find local tests and perform
    for input_path in sorted(glob.glob("local_test_*_input.txt")):

        test = input_path.replace("_input.txt", "")
        passed = False

        # input
        with open(input_path, "rb") as fd:
            input_test = fd.read()

        # description
        description_path = test + "_description.txt"
        if os.path.exists(description_path):
            with open(description_path) as fd:
                description = fd.read().strip()
        else:
            description = test.replace("_", " ")

        # expect
        expect_path = test + "_expect.txt"
        if os.path.exists(expect_path):
            with open(expect_path, "rb") as fd:
                expected = fd.read()
        else:
            expected = None

        # skip?
        ignore_path = test + "_ignore.txt"
        if os.path.exists(ignore_path):
            skip = True
            passed = True

        # not skip
        else:
            skip = False

            result = subprocess.run(
                [
                    "afw",
                    "--local",
                    "1",
                ],
                input=input_test,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
            )

            # Work-dir copy for debugging failed compares
            with open(test + "_actual.txt", "wb") as fd:
                fd.write(result.stdout)

            if expected is None:
                passed = False
            else:
                passed = (
                    normalize_local_stdout(result.stdout)
                    == normalize_local_stdout(expected)
                )

        # add test to response
        response["tests"].append({
            "description": description,
            "skip": skip,
            "passed": passed,
            "test": test
        })

    return response
