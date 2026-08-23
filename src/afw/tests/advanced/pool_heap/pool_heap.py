#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Heap and heap-tracker pool implementations.

Script cannot see the allocated list or free-list reuse.
This probe calls the C pool API and, for list/header checks, includes
afw_pool_heap_internal.h from the src tree.
"""

import os

from _afwdev.test.c_probe import run_c_probe


def _pool_src():
    """src/afw/pool — afw_pool_heap_internal.h is not an installed header."""
    d = os.path.dirname(os.path.abspath(__file__))
    while True:
        cand = os.path.join(d, "pool")
        if os.path.isfile(os.path.join(cand, "afw_pool_heap_internal.h")):
            return cand
        parent = os.path.dirname(d)
        if parent == d:
            return cand
        d = parent


def run():
    return run_c_probe(
        "pool_heap_probe.c",
        "Heap and heap-tracker pool implementations",
        [
            (
                "heap_malloc_free",
                "heap malloc/free: whole chunk back on the free list, "
                "same-size reuse, in_use moves",
            ),
            (
                "tracker_malloc",
                "tracker malloc: block on that tracker's allocated "
                "list",
            ),
            (
                "tracker_optional_free",
                "tracker free unlinks then returns the whole chunk; "
                "destroy does not free it twice",
            ),
            (
                "tracker_last_release",
                "tracker last-release returns leftovers to the heap; "
                "sibling tracker can reuse",
            ),
            (
                "tracker_header",
                "tracker header is APR/RSS, not a user block on the "
                "allocated or free list",
            ),
            (
                "mixed_sizes",
                "just-freed 32- and 200-byte blocks reuse independently",
            ),
            (
                "heap_whole_block",
                "taking a whole free block records that block's size, "
                "so the original size can reuse",
            ),
            (
                "general_free_noop",
                "general APR pool optional free stays a no-op",
            ),
            (
                "tracker_parent",
                "heap_tracker_create parent must be a heap",
            ),
            (
                "get_apr_pool",
                "tracker get_apr_pool is lazy, a child of the heap "
                "reservoir, and dies with the tracker",
            ),
            (
                "deregister_cleanup",
                "deregister unlinks the entry and optional-frees it "
                "on the tracker",
            ),
            (
                "nonadjacent_reuse",
                "non-adjacent frees land on the free list and reuse",
            ),
            (
                "for_clone_churn",
                "tracker calloc ~56 after mixed optional free and "
                "tracker last-release does not hang",
            ),
            (
                "double_free_throws",
                "second free_memory throws on the running xctx",
            ),
        ],
        extra_cflags=("-I", _pool_src()),
        extra_ldflags=("-lapr-1",),
    )
