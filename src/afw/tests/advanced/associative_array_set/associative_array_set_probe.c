// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework associative-array set probe
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

#include "afw.h"

#include <stdio.h>
#include <string.h>

/**
 * @file associative_array_set_probe.c
 * @brief C probe for associative-array set() releasing the existing object.
 *
 * Script cannot reach this impl. set() used to release the incoming object
 * when replacing, then store a dangling pointer and leak the previous one.
 * Clearing a key passed NULL as that incoming object.
 *
 * Same shape as tests/advanced/array_view_index/array_view_index_probe.c.
 */

static const afw_utf8_t impl_key = AFW_UTF8_LITERAL("k");

static const afw_object_t *
impl_create_pinned(const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_object_t *object;

    object = afw_object_create_unmanaged_new_p(p, xctx);
    afw_object_get_reference(object, xctx);
    return object;
}

static const afw_object_t *
impl_stored(
    const afw_object_associative_array_t *aa,
    afw_xctx_t *xctx)
{
    return afw_object_associative_array_get_associated_object_reference(
        aa, &impl_key, xctx);
}

static int
impl_clear(const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_object_associative_array_t *aa;
    const afw_object_t *a;
    const afw_object_t *got;

    aa = afw_object_memory_associative_array_create(p, xctx);
    a = impl_create_pinned(p, xctx);
    afw_object_associative_array_set(aa, &impl_key, a, xctx);
    afw_object_associative_array_set(aa, &impl_key, NULL, xctx);
    got = impl_stored(aa, xctx);
    if (got != NULL) {
        fprintf(stderr, "clear: key still associated\n");
        return 1;
    }
    return 0;
}

static int
impl_replace(const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_object_associative_array_t *aa;
    const afw_object_t *a;
    const afw_object_t *b;
    const afw_object_t *got;

    aa = afw_object_memory_associative_array_create(p, xctx);
    a = impl_create_pinned(p, xctx);
    b = impl_create_pinned(p, xctx);
    afw_object_associative_array_set(aa, &impl_key, a, xctx);
    afw_object_associative_array_set(aa, &impl_key, b, xctx);
    got = impl_stored(aa, xctx);
    if (got != b) {
        fprintf(stderr, "replace: stored object is not the incoming one\n");
        return 1;
    }
    return 0;
}

static int
impl_replace_clear(const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_object_associative_array_t *aa;
    const afw_object_t *a;
    const afw_object_t *b;
    const afw_object_t *got;

    aa = afw_object_memory_associative_array_create(p, xctx);
    a = impl_create_pinned(p, xctx);
    b = impl_create_pinned(p, xctx);
    afw_object_associative_array_set(aa, &impl_key, a, xctx);
    afw_object_associative_array_set(aa, &impl_key, b, xctx);
    afw_object_associative_array_set(aa, &impl_key, NULL, xctx);
    got = impl_stored(aa, xctx);
    if (got != NULL) {
        fprintf(stderr, "replace_clear: key still associated\n");
        return 1;
    }
    return 0;
}

int
main(int argc, char **argv)
{
    const afw_error_t *create_error;
    afw_xctx_t *xctx;
    const afw_pool_t *p;
    const char *case_name;
    int rc;

    xctx = afw_environment_create(afw_version(), argc,
        (const char * const *)argv, &create_error);
    if (!xctx) {
        fprintf(stderr, "environment create failed\n");
        return 2;
    }

    case_name = (argc > 1) ? argv[1] : "";
    p = afw_pool_create(xctx->p, xctx);
    rc = 0;

    if (strcmp(case_name, "clear") == 0) {
        rc = impl_clear(p, xctx);
    }
    else if (strcmp(case_name, "replace") == 0) {
        rc = impl_replace(p, xctx);
    }
    else if (strcmp(case_name, "replace_clear") == 0) {
        rc = impl_replace_clear(p, xctx);
    }
    else {
        fprintf(stderr, "usage: associative_array_set_probe "
            "clear|replace|replace_clear\n");
        rc = 2;
    }

    afw_pool_release(p, xctx);
    afw_environment_release(xctx);
    return rc;
}
