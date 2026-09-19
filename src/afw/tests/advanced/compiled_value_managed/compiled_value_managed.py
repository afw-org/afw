#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
compile_to_value returns a managed compiled_value (natural RC).
"""

from _afwdev.test.c_probe import run_c_probe


def run():
    return run_c_probe(
        "compiled_value_managed_probe.c",
        "managed compiled_value",
        [
            (
                "front_door",
                "compile_to_value is managed; get_assignable is self",
            ),
            (
                "job_heap",
                "unit child of dest p->managed_p; last RC last-releases it",
            ),
        ],
    )
