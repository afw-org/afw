// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework AFW_CATCH(code) probe
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

#include "afw.h"

#include <stdio.h>
#include <string.h>

/**
 * @file afw_catch_code_probe.c
 * @brief C probe for AFW_CATCH(code) matching the thrown error code.
 *
 * Issue #270: AFW_CATCH(code)'s condition tested xctx->error->code, but
 * AFW_TRY's setjmp path clears xctx->error (AFW_ERROR_CLEAR_PARTIAL) before
 * any AFW_CATCH(code) condition runs, so the comparison always failed and
 * every catch fell through to AFW_ENDTRY's rethrow. Adaptive Script cannot
 * reach this - it is a property of the C macro expansion - so this is a C
 * probe over the public AFW_TRY/AFW_CATCH/AFW_ENDTRY macros.
 */

static int
impl_matched(afw_xctx_t *xctx)
{
    int caught;
    int saw_thrown_code;

    caught = 0;
    saw_thrown_code = 0;
    AFW_TRY {
        AFW_THROW_ERROR_Z(not_found, "probe forced not_found", xctx);
    }
    AFW_CATCH(not_found) {
        caught = 1;
        saw_thrown_code =
            (AFW_ERROR_THROWN->code == afw_error_code_not_found);
    }
    AFW_ENDTRY;

    if (!caught) {
        fprintf(stderr, "matched: AFW_CATCH(not_found) did not run\n");
        return 1;
    }
    if (!saw_thrown_code) {
        fprintf(stderr,
            "matched: AFW_ERROR_THROWN->code was not not_found inside "
            "the catch body\n");
        return 1;
    }
    return 0;
}

static int
impl_unmatched_falls_through(afw_xctx_t *xctx)
{
    int wrong_catch_ran;
    int unhandled_ran;
    int unhandled_saw_code;

    wrong_catch_ran = 0;
    unhandled_ran = 0;
    unhandled_saw_code = 0;
    AFW_TRY {
        AFW_TRY {
            AFW_THROW_ERROR_Z(not_found, "probe forced not_found", xctx);
        }
        AFW_CATCH(memory) {
            wrong_catch_ran = 1;
        }
        AFW_ENDTRY;
    }
    AFW_CATCH_UNHANDLED {
        unhandled_ran = 1;
        unhandled_saw_code =
            (AFW_ERROR_THROWN->code == afw_error_code_not_found);
    }
    AFW_ENDTRY;

    if (wrong_catch_ran) {
        fprintf(stderr,
            "unmatched: AFW_CATCH(memory) incorrectly caught not_found\n");
        return 1;
    }
    if (!unhandled_ran) {
        fprintf(stderr,
            "unmatched: not_found did not propagate to outer try\n");
        return 1;
    }
    if (!unhandled_saw_code) {
        fprintf(stderr,
            "unmatched: outer AFW_ERROR_THROWN->code was not not_found\n");
        return 1;
    }
    return 0;
}

static int
impl_second_catch_matches(afw_xctx_t *xctx)
{
    int first_caught;
    int second_caught;

    first_caught = 0;
    second_caught = 0;
    AFW_TRY {
        AFW_THROW_ERROR_Z(memory, "probe forced memory", xctx);
    }
    AFW_CATCH(not_found) {
        first_caught = 1;
    }
    AFW_CATCH(memory) {
        second_caught = 1;
    }
    AFW_ENDTRY;

    if (first_caught) {
        fprintf(stderr,
            "second_catch: AFW_CATCH(not_found) incorrectly caught memory\n");
        return 1;
    }
    if (!second_caught) {
        fprintf(stderr,
            "second_catch: AFW_CATCH(memory) did not catch memory\n");
        return 1;
    }
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

    if (strcmp(case_name, "matched") == 0) {
        rc = impl_matched(xctx);
    }
    else if (strcmp(case_name, "unmatched_falls_through") == 0) {
        rc = impl_unmatched_falls_through(xctx);
    }
    else if (strcmp(case_name, "second_catch_matches") == 0) {
        rc = impl_second_catch_matches(xctx);
    }
    else {
        fprintf(stderr, "usage: afw_catch_code_probe "
            "matched|unmatched_falls_through|second_catch_matches\n");
        rc = 2;
    }

    afw_environment_release(xctx);
    return rc;
}
