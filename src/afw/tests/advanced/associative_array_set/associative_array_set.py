#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Associative-array set() releases the existing object, not the incoming one.

Script never calls this impl. Clearing a key used to pass NULL into
release(); replacing used to drop the new object and leak the old one.
"""

from _afwdev.test.c_probe import run_c_probe


def run():
    return run_c_probe(
        "associative_array_set_probe.c",
        "Associative-array set releases the existing object",
        [
            (
                "clear",
                "set then clear: key is removed and NULL is not released",
            ),
            (
                "replace",
                "replace: stored object is the incoming one",
            ),
            (
                "replace_clear",
                "replace then clear: key is removed",
            ),
        ],
    )
