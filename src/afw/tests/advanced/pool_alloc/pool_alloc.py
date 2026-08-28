#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Pool malloc rejects a wrapping size.

Script integers cannot reach a wrapping size_t on 64-bit hosts.
"""

from _afwdev.test.c_probe import run_c_probe


def run():
    return run_c_probe(
        "pool_alloc_probe.c",
        "Pool malloc rejects a wrapping size",
        [
            (
                "overflow",
                "pool malloc of SIZE_MAX throws memory",
            ),
            (
                "overflow-tracker",
                "tracker malloc of SIZE_MAX throws memory",
            ),
        ],
    )
