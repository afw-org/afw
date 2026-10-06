#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
memory_region lists, trim(), and eviction order on a private region.

Script only sees process-wide totals while its own xctx holds chunks.
"""

from _afwdev.test.c_probe import run_c_probe


def run():
    return run_c_probe(
        "memory_region_trim_probe.c",
        "memory_region lists, trim, and eviction",
        [
            ("configure", "configure copies the env knobs"),
            ("trim", "trim keeps newest resident, discards or unmaps the rest"),
            ("evict", "over cap unmaps the other list before small"),
        ],
        # configure checks the defaults, which afwdev raises under
        # --env-mode asan and valgrind.
        env_unset=("AFW_MEMORY_REGION_KEEP_FREED_BYTES",),
    )
