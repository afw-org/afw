// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework C probe skeleton
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

#include "afw.h"

#include <stdio.h>
#include <string.h>

/**
 * @file skeleton_probe.c
 * @brief Closet template for a C probe (prime-test-c-probe copies this).
 *
 * Prefer an Adaptive test script. Use a probe only when script cannot
 * reach the hole. argv[1] is the case name; exit 0 is pass.
 *
 * @todo Replace this file comment and the sample case with the hole.
 */

/* @todo Add one helper per case, or inline in main. */

static int
impl_ok(afw_xctx_t *xctx)
{
    (void)xctx;
    return 0;
}

/*
 * Expected throw (uncomment and adapt):
 *
 * static int
 * impl_expect_throw(afw_xctx_t *xctx)
 * {
 *     int threw = 0;
 *
 *     AFW_TRY {
 *         @todo call the C API that should throw
 *     }
 *     AFW_CATCH_UNHANDLED {
 *         if (AFW_ERROR_THROWN->code == afw_error_code_memory)
 *             threw = 1;
 *         else
 *             fprintf(stderr, "unexpected: %s\n",
 *                 AFW_ERROR_THROWN->message_z
 *                     ? AFW_ERROR_THROWN->message_z : "?");
 *     }
 *     AFW_ENDTRY;
 *     if (!threw) {
 *         fprintf(stderr, "did not throw\n");
 *         return 1;
 *     }
 *     return 0;
 * }
 */

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

    /* ----- add cases here; keep names in sync with the sibling .py ----- */
    if (strcmp(case_name, "ok") == 0) {
        rc = impl_ok(xctx);
    }
    /* else if (strcmp(case_name, "your_case") == 0) {
        rc = impl_your_case(xctx);
    } */
    else {
        fprintf(stderr, "usage: skeleton_probe ok\n");
        rc = 2;
    }

    afw_environment_release(xctx);
    return rc;
}
