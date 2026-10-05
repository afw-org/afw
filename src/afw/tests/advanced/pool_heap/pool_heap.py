#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Heap and heap-tracker pool implementations.

Script cannot see the allocated list or free-list reuse.
This probe calls the C pool API and, for list/header checks, includes
afw_pool_tracker_internal.h from the src tree (pulls heap + shared).
"""

import os

from _afwdev.test.c_probe import (
    libafw_build_cache, libafw_sanitizers, run_c_probe)

# These read memory an ASAN libafw marks no-access (a freed USER, a
# foreign block's prefix). ASAN reports that read before the debug
# check can throw.
_ASAN_SKIP = (
    "debug_free_wrong_pool",
    "debug_free_poisons_user",
)


def _afw_src():
    """src/afw — internal headers are not installed."""
    d = os.path.dirname(os.path.abspath(__file__))
    while True:
        cand = os.path.join(d, "pool", "afw_pool_internal.h")
        if os.path.isfile(cand):
            return os.path.dirname(os.path.dirname(cand))
        parent = os.path.dirname(d)
        if parent == d:
            return os.path.join(d, "src", "afw")
        d = parent


def _pool_src():
    """src/afw/pool — pool *_internal.h are not installed headers.

    run_c_probe() searches extra -I before the install include dir so a
    leftover copy in the prefix cannot win.
    """
    return os.path.join(_afw_src(), "pool")


def _lib_has_debug_pool():
    """True if the cmake libafw build defined AFW_DEBUG_POOL.

    Probe -D must match the installed lib layout. --cdev/--fulldev
    define it; a production cmake does not. Same test -j file either
    way: throw cases run only when the lib was built with the prefix.
    """
    root = os.path.abspath(os.path.join(_pool_src(), "..", "..", ".."))
    # A --sanitize build (build/asan/...) names its own cmake dir.
    cache = libafw_build_cache() or \
        os.path.join(root, "build", "cmake", "CMakeCache.txt")
    if os.path.isfile(cache):
        with open(cache, encoding="utf-8", errors="replace") as f:
            for line in f:
                if "AFWDEV_C_DEFINES" in line and "AFW_DEBUG_POOL" in line:
                    return True
        return False
    ccjson = os.path.join(root, "build", "cmake", "compile_commands.json")
    if os.path.isfile(ccjson):
        with open(ccjson, encoding="utf-8", errors="replace") as f:
            return "AFW_DEBUG_POOL" in f.read()
    return False


def run():
    debug_pool = _lib_has_debug_pool()
    asan = "address" in libafw_sanitizers()
    extra = [
        "-I", _pool_src(),
        "-I", os.path.join(_afw_src(), "memory"),
        "-I", os.path.join(_afw_src(), "environment"),
        "-DAFW_ENVIRONMENT_INTERNAL_MEMBERS",
    ]
    if debug_pool:
        extra.append("-DAFW_DEBUG_POOL")
    cases = [
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
                "tracker header is calloc from the parent heap, not a "
                "user block on the allocated or free list",
            ),
            (
                "mixed_sizes",
                "just-freed 32- and 200-byte blocks reuse independently",
            ),
            (
                "heap_whole_block",
                "remainder too small stays on the free list so the "
                "original USER size can reuse",
            ),
            (
                "general_free_noop",
                "env->p is a multithreaded heap; optional free recycles",
            ),
            (
                "tracker_parent",
                "heap_tracker_create under env->p (multithreaded heap) works",
            ),
            (
                "multithread_chunk_min",
                "multithread create: 0 is env chunk_min; "
                "small_chunk_min stays the floor",
            ),
            (
                "create_child_of_heap",
                "afw_pool_create of a ST heap parent is a ST heap; "
                "free recycles",
            ),
            (
                "leftover_child_heap",
                "parent run_cleanups then destroy runs leftover child "
                "heap and extra-held tracker cleanups",
            ),
            (
                "unhandled_alloc",
                "calloc_unhandled never throws; NULL xctx does not "
                "move in_use; block still dies with the heap",
            ),
            (
                "heap_chunks",
                "heap store is 64k-min, 4k-aligned mapped "
                "chunks; large alloc adds a chunk; release walks "
                "the list",
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
                "thread_parent_hold",
                "thread pool holds its parent once; get_reference "
                "does not pin that parent",
            ),
            (
                "double_free_throws",
                "second free_memory throws already freed, also after "
                "the block coalesced; in_use unchanged",
            ),
            (
                "tracker_return_chunk",
                "blocks a tracker returns to the heap carry a NULL "
                "chunk, not the tracker list link",
            ),
    ]
    if debug_pool:
        cases.extend([
            (
                "debug_free_wrong_size",
                "AFW_DEBUG_POOL: free with the wrong size throws",
            ),
            (
                "debug_free_wrong_pool",
                "AFW_DEBUG_POOL: free with a different pool throws",
            ),
            (
                "debug_free_poisons_user",
                "AFW_DEBUG_POOL: free fills USER with poison inf",
            ),
        ])
    asan_skipped = []
    if asan:
        asan_skipped = [c for c in cases if c[0] in _ASAN_SKIP]
        cases = [c for c in cases if c[0] not in _ASAN_SKIP]
    result = run_c_probe(
        "pool_heap_probe.c",
        "Heap and heap-tracker pool implementations",
        cases,
        extra_cflags=tuple(extra),
    )
    tests = result.get("tests")
    if not isinstance(tests, list):
        tests = []
        result["tests"] = tests
    if not debug_pool:
        tests.append({
            "test": "debug_free_wrong_size",
            "description": "AFW_DEBUG_POOL + debug:pool: free with the wrong size throws",
            "passed": True,
            "skip": True,
            "skipReason": "libafw built without AFW_DEBUG_POOL",
        })
        tests.append({
            "test": "debug_free_wrong_pool",
            "description": "AFW_DEBUG_POOL + debug:pool: free with a different pool throws",
            "passed": True,
            "skip": True,
            "skipReason": "libafw built without AFW_DEBUG_POOL",
        })
        tests.append({
            "test": "debug_free_poisons_user",
            "description": "AFW_DEBUG_POOL: free fills USER with poison inf",
            "passed": True,
            "skip": True,
            "skipReason": "libafw built without AFW_DEBUG_POOL",
        })
    for name, desc in asan_skipped:
        tests.append({
            "test": name,
            "description": desc,
            "passed": True,
            "skip": True,
            "skipReason": "ASAN reports this read of no-access memory",
        })
    return result
