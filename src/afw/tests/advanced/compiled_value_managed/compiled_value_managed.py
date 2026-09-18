#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Managed compiled_value create, RC, evaluate; unmanaged compile unchanged.
"""

from _afwdev.test.c_probe import run_c_probe


def run():
    return run_c_probe(
        "compiled_value_managed_probe.c",
        "managed compiled_value",
        [
            (
                "unmanaged",
                "compile_to_value still unmanaged with a child heap",
            ),
            (
                "managed_eval",
                "compile_to_managed_value evaluates a script",
            ),
            (
                "managed_rc",
                "managed get_assignable bumps RC, last release frees",
            ),
            (
                "managed_job_heap",
                "managed unit lives in dest p->managed_p",
            ),
        ],
    )
