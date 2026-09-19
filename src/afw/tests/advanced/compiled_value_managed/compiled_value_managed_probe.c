// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework managed compiled_value probe
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#include "afw.h"

#include <stdio.h>
#include <string.h>

/**
 * @file compiled_value_managed_probe.c
 * @brief C probe: compile_to_value returns a managed compiled_value.
 */

static const afw_utf8_t impl_source =
    AFW_UTF8_LITERAL("return 7;");


static int
impl_expect_seven(const afw_value_t *result, const char *label)
{
    if (!result || !afw_value_is_integer(result) ||
        ((const afw_value_integer_t *)result)->internal != 7)
    {
        fprintf(stderr, "%s: expected integer 7\n", label);
        return 1;
    }
    return 0;
}


static int
impl_front_door(afw_xctx_t *xctx)
{
    const afw_value_t *value;
    const afw_value_t *result;
    const afw_value_t *again;

    value = afw_compile_to_value(&impl_source, NULL,
        afw_compile_type_script, NULL, xctx->p, xctx);
    if (!value ||
        value->inf != &afw_value_managed_compiled_value_inf)
    {
        fprintf(stderr, "front_door: expected managed compiled_value inf\n");
        return 1;
    }
    if (!afw_value_is_compiled_value(value) || !value->inf->is_managed) {
        fprintf(stderr, "front_door: is_compiled_value / is_managed\n");
        return 1;
    }
    again = afw_value_get_assignable(value, xctx->p, xctx);
    if (again != value) {
        fprintf(stderr, "front_door: get_assignable is not self\n");
        return 1;
    }
    afw_value_release(value, xctx);
    result = afw_value_evaluate(value, xctx->p, xctx);
    if (impl_expect_seven(result, "front_door") != 0) {
        return 1;
    }
    afw_value_release(value, xctx);
    return 0;
}


static int
impl_job_heap(afw_xctx_t *xctx)
{
    const afw_pool_t *job;
    const afw_value_t *value;
    const afw_value_t *result;
    afw_boolean_t threw;

    job = afw_pool_heap_create_as_managed_p(xctx->p, 0, xctx);
    value = afw_compile_to_value(&impl_source, NULL,
        afw_compile_type_script, NULL, job, xctx);
    result = afw_value_evaluate(value, xctx->p, xctx);
    if (impl_expect_seven(result, "job_heap") != 0) {
        afw_value_release(value, xctx);
        afw_pool_release(job, xctx);
        return 1;
    }
    afw_value_release(value, xctx);
    threw = false;
    AFW_TRY {
        afw_pool_release(job, xctx);
    }
    AFW_CATCH_UNHANDLED {
        threw = true;
    }
    AFW_ENDTRY;
    if (threw) {
        fprintf(stderr, "job_heap: dest p last-release threw\n");
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

    if (strcmp(case_name, "front_door") == 0) {
        rc = impl_front_door(xctx);
    }
    else if (strcmp(case_name, "job_heap") == 0) {
        rc = impl_job_heap(xctx);
    }
    else {
        fprintf(stderr, "usage: compiled_value_managed_probe "
            "front_door|job_heap\n");
        rc = 2;
    }

    afw_environment_release(xctx);
    return rc;
}
