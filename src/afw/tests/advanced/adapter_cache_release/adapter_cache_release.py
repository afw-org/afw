#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Adapter session cache commit/release walks every entry.

A throw from one transaction or session must not skip the rest.
The first error is what the caller sees.
"""

import os

from _afwdev.test.c_probe import run_c_probe


def _afw_src():
    """src/afw — xctx and adapter internal headers are not installed."""
    d = os.path.dirname(os.path.abspath(__file__))
    while True:
        cand = os.path.join(d, "adapter", "afw_adapter_internal.h")
        if os.path.isfile(cand):
            return d
        parent = os.path.dirname(d)
        if parent == d:
            raise RuntimeError("src/afw not found from " + __file__)
        d = parent


def run():
    afw = _afw_src()
    return run_c_probe(
        "adapter_cache_release_probe.c",
        "commit_and_release_cache finishes every release",
        [
            (
                "commit_keeps_first",
                "a commit throw still releases later transactions "
                "and sessions; the first error is rethrown",
            ),
            (
                "abort_release_continues",
                "abort skips commit; a release throw still "
                "releases the rest",
            ),
            (
                "runtime_throw",
                "a runtime session throw still clears the cache",
            ),
            (
                "all_ok",
                "with no throw, every commit and release runs "
                "and the cache is cleared",
            ),
            (
                "no_cache",
                "a NULL cache returns without throwing",
            ),
            (
                "both_throw",
                "commit and release of one transaction both throw; "
                "the commit error is kept and the next transaction runs",
            ),
            (
                "quiet_inside_catch",
                "abort from inside an in-flight error releases "
                "and leaves that error in place",
            ),
        ],
        extra_cflags=(
            "-I", os.path.join(afw, "xctx"),
            "-I", os.path.join(afw, "adapter"),
            "-DAFW_XCTX_INTERNAL_MEMBERS",
        ),
    )
