// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework short-lived pool churn (scope-shaped)
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#include "afw.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

/**
 * @file pool_scope_churn_probe.c
 * @brief Compare tracker vs inherit-heap for many short-lived pools.
 *
 * Scopes are short and bulk-freed. This is not a switch of
 * afw_pool_scope_create; it measures create / a few mallocs /
 * last-release. Metrics names match thread/xctx counters
 * (poolBytesInUse, peakPoolBytesInUse, poolChunkBytes,
 * peakPoolChunkBytes) so test runs and a later flag can harvest
 * the same fields.
 */

#define IMPL_ITERS     ((afw_size_t)3000)
#define IMPL_ALLOCS    8
#define IMPL_SIZE      ((afw_size_t)48)
#define IMPL_CHUNK_4K  ((afw_size_t)4096)

static afw_size_t
impl_ns_since(const struct timespec *start)
{
    struct timespec now;
    afw_size_t sec;
    afw_size_t nsec;

    clock_gettime(CLOCK_MONOTONIC, &now);
    sec = (afw_size_t)(now.tv_sec - start->tv_sec);
    nsec = (afw_size_t)now.tv_nsec - (afw_size_t)start->tv_nsec;
    return sec * (afw_size_t)1000000000 + nsec;
}

static void
impl_print_metrics(
    const char *kind,
    afw_size_t n,
    afw_size_t elapsed_ns,
    afw_size_t bytes,
    afw_size_t peak_bytes,
    afw_size_t chunks,
    afw_size_t peak_chunks)
{
    printf(
        "METRICS kind=%s n=" AFW_SIZE_T_FMT
        " elapsed_ns=" AFW_SIZE_T_FMT
        " poolBytesInUse=" AFW_SIZE_T_FMT
        " peakPoolBytesInUse=" AFW_SIZE_T_FMT
        " poolChunkBytes=" AFW_SIZE_T_FMT
        " peakPoolChunkBytes=" AFW_SIZE_T_FMT "\n",
        kind, n, elapsed_ns, bytes, peak_bytes, chunks, peak_chunks);
}

static int
impl_churn(
    afw_xctx_t *xctx,
    const char *kind,
    const afw_pool_t *(*make)(
        const afw_pool_t *parent, afw_size_t chunk_min, afw_xctx_t *xctx),
    afw_size_t chunk_min)
{
    afw_size_t i;
    afw_size_t a;
    afw_size_t bytes0;
    afw_size_t peak_bytes0;
    afw_size_t chunks0;
    afw_size_t peak_chunks0;
    afw_size_t bytes1;
    afw_size_t chunks1;
    afw_size_t elapsed;
    struct timespec start;
    const afw_pool_t *p;
    void *block[IMPL_ALLOCS];
    const afw_thread_t *thread;

    thread = xctx->thread;
    if (!thread) {
        fprintf(stderr, "%s: no thread\n", kind);
        return 1;
    }
    bytes0 = thread->pool_bytes_in_use;
    peak_bytes0 = thread->peak_pool_bytes_in_use;
    chunks0 = thread->pool_chunk_bytes;
    peak_chunks0 = thread->peak_pool_chunk_bytes;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (i = 0; i < IMPL_ITERS; i++) {
        p = make(xctx->p, chunk_min, xctx);
        for (a = 0; a < IMPL_ALLOCS; a++) {
            block[a] = afw_pool_malloc(p, IMPL_SIZE, xctx);
        }
        (void)block;
        afw_pool_release(p, xctx);
    }
    elapsed = impl_ns_since(&start);

    bytes1 = thread->pool_bytes_in_use;
    chunks1 = thread->pool_chunk_bytes;
    impl_print_metrics(
        kind, IMPL_ITERS, elapsed,
        bytes1 > bytes0 ? bytes1 - bytes0 : 0,
        thread->peak_pool_bytes_in_use > peak_bytes0
            ? thread->peak_pool_bytes_in_use - peak_bytes0 : 0,
        chunks1 > chunks0 ? chunks1 - chunks0 : 0,
        thread->peak_pool_chunk_bytes > peak_chunks0
            ? thread->peak_pool_chunk_bytes - peak_chunks0 : 0);

    if (bytes1 > bytes0) {
        fprintf(stderr, "%s: poolBytesInUse leaked " AFW_SIZE_T_FMT "\n",
            kind, bytes1 - bytes0);
        return 1;
    }
    if (chunks1 > chunks0) {
        fprintf(stderr, "%s: poolChunkBytes leaked " AFW_SIZE_T_FMT "\n",
            kind, chunks1 - chunks0);
        return 1;
    }
    return 0;
}

static const afw_pool_t *
impl_make_tracker(
    const afw_pool_t *parent, afw_size_t chunk_min, afw_xctx_t *xctx)
{
    (void)chunk_min;
    return afw_pool_tracker_create(parent, xctx);
}

static const afw_pool_t *
impl_make_heap(
    const afw_pool_t *parent, afw_size_t chunk_min, afw_xctx_t *xctx)
{
    return afw_pool_heap_create(parent, chunk_min, xctx);
}

int
main(int argc, char **argv)
{
    const afw_error_t *create_error;
    afw_xctx_t *xctx;
    const char *case_name;
    int rc;

    xctx = afw_environment_create(afw_version(), argc,
        (const char * const *)argv, &create_error);
    if (!xctx) {
        fprintf(stderr, "environment create failed\n");
        return 2;
    }

    case_name = (argc > 1) ? argv[1] : "";
    rc = 0;
    if (strcmp(case_name, "tracker") == 0) {
        rc = impl_churn(xctx, "tracker", impl_make_tracker, 0);
    }
    else if (strcmp(case_name, "heap_4k") == 0) {
        rc = impl_churn(xctx, "heap_4k", impl_make_heap, IMPL_CHUNK_4K);
    }
    else if (strcmp(case_name, "heap_64k") == 0) {
        rc = impl_churn(xctx, "heap_64k", impl_make_heap, 0);
    }
    else {
        fprintf(stderr, "usage: pool_scope_churn_probe "
            "tracker|heap_4k|heap_64k\n");
        rc = 2;
    }

    afw_environment_release(xctx);
    return rc;
}
