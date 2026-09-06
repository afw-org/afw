#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
create_view_of_c_array get at index === count is not found.

Copies C internals onto const_array_of_values. A script a[length]
on a literal does not catch this path.
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
