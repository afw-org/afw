#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Pool memory checker annotations under AddressSanitizer.

Script cannot ask ASAN which pool bytes are marked no-access. Runs
only against an ASAN libafw (AFW_LIB_DIR); every case skips otherwise.
"""

from _afwdev.test.c_probe import libafw_sanitizers, run_c_probe

_CASES = [
    (
        "heap",
        "heap: live block accessible, slack after it no-access, free "
        "makes USER no-access, same-size reuse makes it live",
    ),
    (
        "tracker",
        "tracker: live block accessible, slack no-access, marked "
        "free makes USER no-access",
    ),
    (
        "release",
        "released heap: its chunk on the region free list is "
        "no-access",
    ),
]


def run():
    if "address" not in libafw_sanitizers():
        return {
            "description": "Pool ASAN annotations",
            "tests": [
                {
                    "test": name,
                    "description": desc,
                    "passed": True,
                    "skip": True,
                    "skipReason": "libafw built without -fsanitize=address",
                }
                for name, desc in _CASES
            ],
        }
    return run_c_probe(
        "pool_asan_probe.c",
        "Pool ASAN annotations",
        _CASES,
    )
