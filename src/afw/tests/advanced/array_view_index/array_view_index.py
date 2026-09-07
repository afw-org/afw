#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
create_unmanaged_from_c_array get at index === count is not found.

Copies C internals onto from_values. A script a[length]
on a literal does not catch this path.
"""

from _afwdev.test.c_probe import run_c_probe


def run():
    return run_c_probe(
        "array_view_index_probe.c",
        "from_c_array index === count is not found",
        [
            (
                "direct",
                "counted non-indirect from_c_array: index === count is not found",
            ),
            (
                "empty",
                "empty from_c_array with storage: index 0 is not found",
            ),
            (
                "indirect",
                "counted indirect from_c_array: index === count is not found",
            ),
        ],
    )
