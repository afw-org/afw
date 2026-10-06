// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework memory_region trim probe
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#include "afw.h"

#include <stdio.h>
#include <string.h>

/**
 * @file memory_region_trim_probe.c
 * @brief memory_region lists, trim(), and eviction order.
 *
 * Each case uses its own region configured from the env (small
 * 4k, large 64k, keep 8 small and 1 large, cap 256k), so the
 * thread's region and its traffic do not change the counts.
 * argv[1] is the case name; exit 0 is pass.
 */

#define IMPL_PAGE ((afw_size_t)4096)
#define IMPL_SMALL ((afw_size_t)4096)
#define IMPL_LARGE ((afw_size_t)65536)
#define IMPL_OTHER ((afw_size_t)8192)
#define IMPL_FILL 0xab

#define IMPL_CHECK(_cond, _msg) \
    if (!(_cond)) { \
        fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, _msg); \
        return 1; \
    }


static void *
impl_get(const afw_memory_region_t *region, afw_size_t size,
    afw_xctx_t *xctx)
{
    void *mem;

    mem = NULL;
    afw_memory_region_get(region, &mem, &size, xctx);
    if (mem) {
        memset(mem, IMPL_FILL, size);
    }
    return mem;
}


static const afw_memory_region_t *
impl_region(afw_xctx_t *xctx)
{
    const afw_memory_region_t *region;

    region = afw_memory_region_create(0, xctx);
    afw_memory_region_configure(region, xctx->env, xctx);
    return region;
}


/* Every byte of [mem, mem + size) is c. */
static afw_boolean_t
impl_all(const void *mem, afw_size_t size, int c)
{
    const unsigned char *s;
    afw_size_t i;

    for (s = mem, i = 0; i < size; i++) {
        if (s[i] != (unsigned char)c) {
            return false;
        }
    }
    return true;
}


static int
impl_configure(afw_xctx_t *xctx)
{
    const afw_memory_region_t *region;

    region = impl_region(xctx);
    IMPL_CHECK(region->small_size == IMPL_SMALL, "small_size");
    IMPL_CHECK(region->large_size == IMPL_LARGE, "large_size");
    IMPL_CHECK(region->keep_small_count == 8, "keep_small_count");
    IMPL_CHECK(region->keep_large_count == 1, "keep_large_count");
    IMPL_CHECK(region->free_list_max_bytes == 262144,
        "free_list_max_bytes");
    afw_memory_region_release(region, xctx);
    return 0;
}


/*
 * 3 large, 10 small, 1 other on the lists. trim() keeps the newest
 * large and 8 small resident, discards 2 large and the other, and
 * unmaps 2 small. A second trim() changes nothing. get() takes the
 * resident large first; the next one is discarded and reads zero
 * past its first page.
 */
static int
impl_trim(afw_xctx_t *xctx)
{
    const afw_memory_region_t *region;
    void *large[3];
    void *small[10];
    void *other;
    void *mem;
    afw_size_t discarded;
    int i;

    region = impl_region(xctx);
    for (i = 0; i < 3; i++) {
        large[i] = impl_get(region, IMPL_LARGE, xctx);
        IMPL_CHECK(large[i], "get large");
    }
    for (i = 0; i < 10; i++) {
        small[i] = impl_get(region, IMPL_SMALL, xctx);
        IMPL_CHECK(small[i], "get small");
    }
    other = impl_get(region, IMPL_OTHER, xctx);
    IMPL_CHECK(other, "get other");
    IMPL_CHECK(region->get_misses == 14, "14 misses");

    for (i = 0; i < 3; i++) {
        afw_memory_region_free(region, large[i], IMPL_LARGE, xctx);
    }
    for (i = 0; i < 10; i++) {
        afw_memory_region_free(region, small[i], IMPL_SMALL, xctx);
    }
    afw_memory_region_free(region, other, IMPL_OTHER, xctx);
    IMPL_CHECK(region->free_list_count == 14, "14 on lists");
    IMPL_CHECK(region->free_over_cap == 0, "none over cap");

    afw_memory_region_trim(region, xctx);
    discarded = 2 * (IMPL_LARGE - IMPL_PAGE) + (IMPL_OTHER - IMPL_PAGE);
    IMPL_CHECK(region->discards == 3, "3 discards");
    IMPL_CHECK(region->trim_unmaps == 2, "2 trim unmaps");
    IMPL_CHECK(region->free_list_count == 12, "12 on lists");
    IMPL_CHECK(region->free_list_discarded_bytes == discarded,
        "discarded bytes");
    IMPL_CHECK(region->free_list_bytes ==
        3 * IMPL_LARGE + 8 * IMPL_SMALL + IMPL_OTHER, "list bytes");

    afw_memory_region_trim(region, xctx);
    IMPL_CHECK(region->discards == 3, "second trim discards none");
    IMPL_CHECK(region->trim_unmaps == 2, "second trim unmaps none");

    /* Newest large is resident: past the list node, contents stay. */
    mem = NULL;
    {
        afw_size_t size = IMPL_LARGE;
        afw_memory_region_get(region, &mem, &size, xctx);
    }
    IMPL_CHECK(mem == large[2], "newest large first");
    IMPL_CHECK(impl_all((char *)mem + IMPL_PAGE, IMPL_LARGE - IMPL_PAGE,
        IMPL_FILL), "resident kept");
    IMPL_CHECK(region->free_list_discarded_bytes == discarded,
        "resident get leaves discarded bytes");

    /* Next is discarded: zero past the node page. */
    other = NULL;
    {
        afw_size_t size = IMPL_LARGE;
        afw_memory_region_get(region, &other, &size, xctx);
    }
    IMPL_CHECK(other == large[1], "discarded large next");
    IMPL_CHECK(impl_all((char *)other + IMPL_PAGE,
        IMPL_LARGE - IMPL_PAGE, 0), "discarded pages read zero");
    IMPL_CHECK(region->free_list_discarded_bytes ==
        discarded - (IMPL_LARGE - IMPL_PAGE),
        "discarded get lowers discarded bytes");
    IMPL_CHECK(region->get_hits == 2, "2 hits");

    afw_memory_region_free(region, mem, IMPL_LARGE, xctx);
    afw_memory_region_free(region, other, IMPL_LARGE, xctx);
    afw_memory_region_cleanup(region, xctx);
    IMPL_CHECK(region->free_list_count == 0, "cleanup count");
    IMPL_CHECK(region->free_list_bytes == 0, "cleanup bytes");
    IMPL_CHECK(region->free_list_discarded_bytes == 0,
        "cleanup discarded bytes");
    afw_memory_region_release(region, xctx);
    return 0;
}


/*
 * Cap fits one large, one small, one other. Freeing one more small
 * unmaps the other first, so a get of the other size misses and a
 * get of small hits.
 */
static int
impl_evict(afw_xctx_t *xctx)
{
    const afw_memory_region_t *region;
    void *large;
    void *small[2];
    void *other;
    afw_size_t misses;

    region = impl_region(xctx);
    ((afw_memory_region_t *)region)->free_list_max_bytes =
        IMPL_LARGE + IMPL_SMALL + IMPL_OTHER;
    large = impl_get(region, IMPL_LARGE, xctx);
    small[0] = impl_get(region, IMPL_SMALL, xctx);
    small[1] = impl_get(region, IMPL_SMALL, xctx);
    other = impl_get(region, IMPL_OTHER, xctx);
    IMPL_CHECK(large && small[0] && small[1] && other, "gets");

    afw_memory_region_free(region, large, IMPL_LARGE, xctx);
    afw_memory_region_free(region, other, IMPL_OTHER, xctx);
    afw_memory_region_free(region, small[0], IMPL_SMALL, xctx);
    IMPL_CHECK(region->free_list_count == 3, "full");
    afw_memory_region_free(region, small[1], IMPL_SMALL, xctx);
    IMPL_CHECK(region->free_over_cap == 1, "one evicted");
    IMPL_CHECK(region->free_list_count == 3, "still 3");
    IMPL_CHECK(region->free_list_bytes == IMPL_LARGE + 2 * IMPL_SMALL,
        "other evicted");

    misses = region->get_misses;
    other = impl_get(region, IMPL_OTHER, xctx);
    IMPL_CHECK(region->get_misses == misses + 1, "other misses");
    small[0] = impl_get(region, IMPL_SMALL, xctx);
    IMPL_CHECK(small[0] == small[1], "newest small first");
    IMPL_CHECK(region->get_misses == misses + 1, "small hits");

    afw_memory_region_free(region, other, IMPL_OTHER, xctx);
    afw_memory_region_free(region, small[0], IMPL_SMALL, xctx);
    afw_memory_region_release(region, xctx);
    return 0;
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

    if (strcmp(case_name, "configure") == 0) {
        rc = impl_configure(xctx);
    }
    else if (strcmp(case_name, "trim") == 0) {
        rc = impl_trim(xctx);
    }
    else if (strcmp(case_name, "evict") == 0) {
        rc = impl_evict(xctx);
    }
    else {
        fprintf(stderr,
            "usage: memory_region_trim_probe configure|trim|evict\n");
        rc = 2;
    }

    afw_environment_release(xctx);
    return rc;
}
