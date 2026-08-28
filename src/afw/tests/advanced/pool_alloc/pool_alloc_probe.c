// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework pool alloc/free-list probe
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

#include "afw.h"

#include <stdio.h>
#include <string.h>

/**
 * @file pool_alloc_probe.c
 * @brief C probe for pool size wrap.
 *
 * Script integers cannot reach a wrapping size_t on 64-bit, so this
 * boots a core environment and calls the pool C API directly.
 */

static int
impl_expect_overflow(
    const afw_pool_t *p,
    afw_xctx_t *xctx,
    const char *label)
{
    int threw;
    int unexpected;

    threw = 0;
    unexpected = 0;
    AFW_TRY {
        (void)afw_pool_malloc(p, (afw_size_t)-1, xctx);
    }
    AFW_CATCH_UNHANDLED {
        if (AFW_ERROR_THROWN->code == afw_error_code_memory &&
            AFW_ERROR_THROWN->message_z &&
            strstr(AFW_ERROR_THROWN->message_z, "too large"))
        {
            threw = 1;
        }
        else {
            unexpected = 1;
            fprintf(stderr, "%s: threw %s\n", label,
                AFW_ERROR_THROWN->message_z
                ? AFW_ERROR_THROWN->message_z : "?");
        }
    }
    AFW_ENDTRY;

    if (unexpected) {
        return 1;
    }
    if (!threw) {
        fprintf(stderr, "%s: did not throw\n", label);
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
    const afw_pool_t *sub;
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

    if (strcmp(case_name, "overflow") == 0) {
        p = afw_pool_create(xctx->p, xctx);
        rc = impl_expect_overflow(p, xctx, "overflow");
        afw_pool_release(p, xctx);
    }
    else if (strcmp(case_name, "overflow-tracker") == 0) {
        p = afw_pool_create_xctx_p(xctx->p, xctx);
        sub = afw_pool_tracker_create(p, xctx);
        rc = impl_expect_overflow(sub, xctx, "overflow-tracker");
        afw_pool_release(sub, xctx);
        afw_pool_release(p, xctx);
    }
    else {
        fprintf(stderr, "usage: pool_alloc_probe "
            "overflow|overflow-tracker\n");
        rc = 2;
    }

    afw_environment_release(xctx);
    return rc;
}
