// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework pool ASAN annotation probe
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#include "afw.h"

#include <stdio.h>
#include <string.h>

#if defined(__SANITIZE_ADDRESS__)
#define IMPL_ASAN 1
#elif defined(__has_feature)
#if __has_feature(address_sanitizer)
#define IMPL_ASAN 1
#endif
#endif

#ifdef IMPL_ASAN
#include <sanitizer/asan_interface.h>
#endif

/**
 * @file pool_asan_probe.c
 * @brief C probe for pool memory checker annotations.
 *
 * Heaps carve mapped chunks themselves, so AddressSanitizer sees a
 * chunk as one valid block unless the pool marks its bytes. Script
 * cannot ask which bytes are marked. This probe asks ASAN directly
 * (`__asan_address_is_poisoned`) after pool malloc, free, and
 * release. Built only against an ASAN libafw; pool_asan.py skips
 * every case otherwise. See `designs/asan-opt-in.md`.
 */

#ifdef IMPL_ASAN

#define IMPL_NOACCESS(_addr) __asan_address_is_poisoned(_addr)

/* First accessible byte in [_addr, _addr + _size), or NULL. */
static const char *
impl_first_accessible(const void *addr, afw_size_t size)
{
    const char *c;
    const char *end;

    for (c = addr, end = c + size; c < end; c++) {
        if (!__asan_address_is_poisoned(c)) {
            return c;
        }
    }
    return NULL;
}


static int
impl_expect_accessible(const char *label, const void *addr,
    afw_size_t size)
{
    const void *bad;

    bad = __asan_region_is_poisoned((void *)addr, size);
    if (bad) {
        fprintf(stderr, "%s: byte %td of %zu is no-access\n", label,
            (const char *)bad - (const char *)addr, (size_t)size);
        return 1;
    }
    return 0;
}


static int
impl_expect_noaccess(const char *label, const void *addr,
    afw_size_t size)
{
    const char *bad;

    bad = impl_first_accessible(addr, size);
    if (bad) {
        fprintf(stderr, "%s: byte %td of %zu is accessible\n", label,
            bad - (const char *)addr, (size_t)size);
        return 1;
    }
    return 0;
}


/* Live block is accessible; the byte after the asked-for size is not. */
static int
impl_live_and_slack(const afw_pool_t *p, afw_xctx_t *xctx)
{
    char *b;
    int rc;

    b = afw_pool_malloc(p, 20, xctx);
    rc = impl_expect_accessible("live", b, 20);
    if (!IMPL_NOACCESS(b + 20)) {
        fprintf(stderr, "slack: byte 20 after a 20-byte malloc is "
            "accessible\n");
        rc = 1;
    }
    afw_pool_free_memory(p, b, 20, xctx);
    return rc;
}


/* Every USER byte is no-access after free. Reuse makes it live again. */
static int
impl_free_and_reuse(const afw_pool_t *p, afw_xctx_t *xctx,
    afw_boolean_t is_tracker)
{
    char *b;
    char *again;
    int rc;

    b = afw_pool_malloc(p, 64, xctx);
    memset(b, 1, 64);
    afw_pool_free_memory(p, b, 64, xctx);
    rc = impl_expect_noaccess("freed", b, 64);
    if (is_tracker) {
        /* Tracker free only marks; reuse waits for collect. */
        return rc;
    }
    again = afw_pool_malloc(p, 64, xctx);
    if (again != b) {
        fprintf(stderr, "reuse: same-size malloc did not take the "
            "freed block\n");
        return 1;
    }
    rc |= impl_expect_accessible("reused", again, 64);
    afw_pool_free_memory(p, again, 64, xctx);
    return rc;
}


static int
impl_heap(afw_xctx_t *xctx)
{
    const afw_pool_t *p;
    int rc;

    p = afw_pool_heap_create(xctx->p, 0, xctx);
    rc = impl_live_and_slack(p, xctx);
    rc |= impl_free_and_reuse(p, xctx, false);
    afw_pool_release(p, xctx);
    return rc;
}


static int
impl_tracker(afw_xctx_t *xctx)
{
    const afw_pool_t *p;
    int rc;

    p = afw_pool_tracker_create(xctx->p, xctx);
    rc = impl_live_and_slack(p, xctx);
    rc |= impl_free_and_reuse(p, xctx, true);
    afw_pool_release(p, xctx);
    return rc;
}


/*
 * A released heap's chunk goes to the thread region free list
 * (default cap keeps a 64k chunk), which leaves it no-access. A
 * pointer into a released pool is then a reported use after free.
 */
static int
impl_release(afw_xctx_t *xctx)
{
    const afw_pool_t *p;
    char *b;

    p = afw_pool_heap_create(xctx->p, 0, xctx);
    b = afw_pool_malloc(p, 128, xctx);
    memset(b, 1, 128);
    afw_pool_release(p, xctx);
    return impl_expect_noaccess("released", b, 128);
}

#endif


int
main(int argc, char **argv)
{
    const afw_error_t *create_error;
    afw_xctx_t *xctx;
    const char *case_name;
    int rc;

#ifndef IMPL_ASAN
    (void)argc;
    (void)argv;
    (void)create_error;
    (void)xctx;
    (void)case_name;
    (void)rc;
    fprintf(stderr, "pool_asan_probe: not built with "
        "-fsanitize=address\n");
    return 2;
#else
    xctx = afw_environment_create(afw_version(), argc,
        (const char * const *)argv, &create_error);
    if (!xctx) {
        fprintf(stderr, "environment create failed\n");
        return 2;
    }

    case_name = (argc > 1) ? argv[1] : "";
    rc = 0;

    if (strcmp(case_name, "heap") == 0) {
        rc = impl_heap(xctx);
    }
    else if (strcmp(case_name, "tracker") == 0) {
        rc = impl_tracker(xctx);
    }
    else if (strcmp(case_name, "release") == 0) {
        rc = impl_release(xctx);
    }
    else {
        fprintf(stderr, "usage: pool_asan_probe heap|tracker|release\n");
        rc = 2;
    }

    afw_environment_release(xctx);
    return rc;
#endif
}
