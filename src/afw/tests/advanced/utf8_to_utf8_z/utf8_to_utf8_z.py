#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
to_utf8_z / z_create / array-to-z refuse an embedded 0.

Adaptive strings are length-prefixed, so script can hold U+0000.
The C-string door is the hole. A probe sets .s/.len by hand.
"""

from _afwdev.test.c_probe import run_c_probe


def run():
    return run_c_probe(
        "utf8_to_utf8_z_probe.c",
        "UTF-8 to C string rejects an embedded NUL",
        [
            (
                "empty",
                "NULL or zero-length converts to empty C string",
            ),
            (
                "ok",
                "ordinary utf-8 converts; strlen matches len",
            ),
            (
                "embedded",
                "embedded 0 in to_utf8_z throws",
            ),
            (
                "z-create",
                "embedded 0 in z_create throws; ordinary create works",
            ),
            (
                "array-z",
                "array-to-z throws on piece or separator; utf8 concat keeps NUL",
            ),
            (
                "concat-z",
                "concat .len then to_utf8_z throws if a piece has 0",
            ),
        ],
    )
