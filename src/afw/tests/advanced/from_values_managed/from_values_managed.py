#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
create_managed_from_c_array / from_values RC and get_entry.
"""

from _afwdev.test.c_probe import run_c_probe


def run():
    return run_c_probe(
        "from_values_managed_probe.c",
        "managed from_values create and release",
        [
            (
                "c_array",
                "managed from_c_array of two integers",
            ),
            (
                "values",
                "managed from_values of two integer values",
            ),
            (
                "empty",
                "managed empty typed from_values",
            ),
            (
                "as_type",
                "as_integer returns typed value pointer",
            ),
        ],
    )
