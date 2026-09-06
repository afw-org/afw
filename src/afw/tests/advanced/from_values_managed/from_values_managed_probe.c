// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework managed from_values probe
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

#include "afw.h"

#include <stdio.h>
#include <string.h>

/**
 * @file from_values_managed_probe.c
 * @brief C probe for create_managed_from_values / from_c_array.
 */

static int
impl_expect_integer(
    const afw_array_t *array,
    afw_integer_t index,
    afw_integer_t expect,
    afw_xctx_t *xctx,
    const char *label)
{
    const afw_value_t *value;

    value = afw_array_get_entry_value(array, index, xctx->p, xctx);
    if (!value ||
        !afw_value_is_integer(value) ||
        ((const afw_value_integer_t *)value)->internal != expect)
    {
        fprintf(stderr, "%s: index %d missing or wrong value\n",
            label, (int)index);
        return 1;
    }
    return 0;
}

static int
impl_c_array(afw_xctx_t *xctx)
{
    afw_integer_t ints[2];
    const afw_array_t *array;
    int rc;

    ints[0] = 10;
    ints[1] = 20;
    array = afw_array_create_managed_from_c_array(
        ints, false, afw_data_type_integer, 2, xctx);
    if (afw_array_get_setter(array, xctx) != NULL) {
        fprintf(stderr, "c_array: setter should be NULL\n");
        return 1;
    }
    if (afw_array_get_data_type(array, xctx) != afw_data_type_integer) {
        fprintf(stderr, "c_array: data type\n");
        return 1;
    }
    if (afw_array_get_count(array, xctx) != 2) {
        fprintf(stderr, "c_array: count\n");
        return 1;
    }
    rc = 0;
    rc |= impl_expect_integer(array, 0, 10, xctx, "c_array");
    rc |= impl_expect_integer(array, 1, 20, xctx, "c_array");
    afw_array_get_reference(array, xctx);
    afw_array_release(array, xctx);
    afw_array_release(array, xctx);
    return rc;
}

static int
impl_values(afw_xctx_t *xctx)
{
    const afw_value_t *vals[2];
    const afw_array_t *array;
    int rc;

    vals[0] = afw_value_create_unmanaged_integer(3, xctx->p, xctx);
    vals[1] = afw_value_create_unmanaged_integer(4, xctx->p, xctx);
    array = afw_array_create_managed_from_values(
        afw_data_type_integer, vals, 2, xctx);
    rc = 0;
    rc |= impl_expect_integer(array, 0, 3, xctx, "values");
    rc |= impl_expect_integer(array, 1, 4, xctx, "values");
    afw_array_release(array, xctx);
    return rc;
}

static int
impl_empty(afw_xctx_t *xctx)
{
    const afw_array_t *array;

    array = afw_array_create_managed_from_values(
        afw_data_type_string, NULL, 0, xctx);
    if (afw_array_get_count(array, xctx) != 0) {
        fprintf(stderr, "empty: count\n");
        return 1;
    }
    if (afw_array_get_data_type(array, xctx) != afw_data_type_string) {
        fprintf(stderr, "empty: data type\n");
        return 1;
    }
    if (afw_array_get_entry_value(array, 0, xctx->p, xctx) != NULL) {
        fprintf(stderr, "empty: index 0 found\n");
        return 1;
    }
    afw_array_release(array, xctx);
    return 0;
}

static int
impl_as_type(afw_xctx_t *xctx)
{
    const afw_value_t *v;
    const afw_value_integer_t *typed;

    v = afw_value_create_unmanaged_integer(7, xctx->p, xctx);
    typed = afw_value_as_integer(v, xctx);
    if (typed != (const afw_value_integer_t *)v) {
        fprintf(stderr, "as_integer did not return same pointer\n");
        return 1;
    }
    if (typed->internal != 7) {
        fprintf(stderr, "as_integer internal\n");
        return 1;
    }
    if (afw_value_as_integer_internal(v, xctx) != 7) {
        fprintf(stderr, "as_integer_internal\n");
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

    if (strcmp(case_name, "c_array") == 0) {
        rc = impl_c_array(xctx);
    }
    else if (strcmp(case_name, "values") == 0) {
        rc = impl_values(xctx);
    }
    else if (strcmp(case_name, "empty") == 0) {
        rc = impl_empty(xctx);
    }
    else if (strcmp(case_name, "as_type") == 0) {
        rc = impl_as_type(xctx);
    }
    else {
        fprintf(stderr, "usage: from_values_managed_probe "
            "c_array|values|empty|as_type\n");
        rc = 2;
    }

    afw_environment_release(xctx);
    return rc;
}
