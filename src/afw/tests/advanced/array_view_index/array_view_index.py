#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
C-array view get at index === count is not found.

Script memory arrays already used >=. The view used >, so a script
a[length] on a literal does not catch this impl.
"""

from _afwdev.test.c_probe import run_c_probe


def run():
    return run_c_probe(
        "array_view_index_probe.c",
        "C-array view index === count is not found",
        [
            (
                "direct",
                "counted non-indirect view: index === count is not found",
            ),
            (
                "empty",
                "empty view with live storage: index 0 is not found",
            ),
            (
                "indirect",
                "counted indirect view: index === count is not found",
            ),
        ],
    )
