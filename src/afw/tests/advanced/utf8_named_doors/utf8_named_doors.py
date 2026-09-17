#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Named utf8 doors: create/set copy vs no_copy, ks encode, property
name, printf, from_memory / as_memory.
"""

from _afwdev.test.c_probe import run_c_probe


def run():
    return run_c_probe(
        "utf8_named_doors_probe.c",
        "UTF-8 named doors (create/set/no_copy/ks)",
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
                "ks",
                "ks encode: caret, 0xff, NUL, LF, invalid run",
            ),
            (
                "property-name",
                "create_property_name is encode then NFC",
            ),
            (
                "printf-ks",
                "%s throws on invalid UTF-8; %ks is ks encoding",
            ),
            (
                "printf-nul",
                "AFW_UTF8_FMT copies n bytes including interior 0",
            ),
            (
                "printf-k",
                "%ku / %km / %kx / %kX, width/precision, C %d/%s, dest size",
            ),
            (
                "printf-throws",
                "%k rejects other flags/length; %n throws",
            ),
            (
                "error-backtrace",
                "error object backtrace is ks then NFC",
            ),
            (
                "printf-ks-walk",
                "printf_ks encodes %s and %ku; format caret stays",
            ),
            (
                "error-fz-dirty",
                "FZ dirty %s/%ku keep original error; object encodes",
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
