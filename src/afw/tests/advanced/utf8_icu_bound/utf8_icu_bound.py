#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
to_lower / compare_ignore_case bound a truncated UTF-8 sequence.

Script constructors validate UTF-8. Hand-set .s/.len can still present
a truncated multi-byte sequence to the ICU walk.
"""

from _afwdev.test.c_probe import run_c_probe


def run():
    return run_c_probe(
        "utf8_icu_bound_probe.c",
        "UTF-8 to_lower/compare bound truncated sequences",
        [
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
        ],
    )
