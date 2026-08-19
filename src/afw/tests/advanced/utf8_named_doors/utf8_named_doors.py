#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Named utf8 doors: create/set copy vs no_copy, forced_safe encode, property
name, printf, from_memory / as_memory.
"""

from _afwdev.test.c_probe import run_c_probe


def run():
    return run_c_probe(
        "utf8_named_doors_probe.c",
        "UTF-8 named doors (create/set/no_copy/forced_safe)",
        [
            (
                "create-set-copy",
                "create/set copy bytes; trunc throws",
            ),
            (
                "no-copy",
                "create_no_copy/set_no_copy point at input; trunc throws",
            ),
            (
                "forced-safe",
                "forced_safe encode: caret, 0xff, NUL, LF, invalid run",
            ),
            (
                "property-name",
                "create_property_name is encode then NFC",
            ),
            (
                "printf-safe",
                "printf uses forced_safe",
            ),
            (
                "printf-nul",
                "AFW_UTF8_FMT copies n bytes including interior 0",
            ),
            (
                "error-backtrace",
                "error object backtrace is forced_safe then NFC",
            ),
            (
                "icu-error-name",
                "icu error name wrap, no unicode include in env",
            ),
            (
                "from-memory",
                "as_memory / from_memory",
            ),
        ],
    )
