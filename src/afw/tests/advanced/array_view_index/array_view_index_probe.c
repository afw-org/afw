// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework C-array view index probe
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

#include "afw.h"

#include <stdio.h>
#include <string.h>

/**
 * @file array_view_index_probe.c
 * @brief C probe for create_view_of_c_array index === count.
 *
 * create_view_of_c_array copies C internals onto const_array_of_values.
 * A script test of a[length] does not hit this path, so this boots a
 * core environment and calls get_entry on a counted copy.
 *
 * Same shape as tests/advanced/pool_alloc/pool_alloc_probe.c.
 */

static int
impl_expect_missing(
    const afw_array_t *array,
    afw_integer_t index,
    afw_xctx_t *xctx,
    const char *label)
{
    const afw_data_type_t *data_type;
    const void *internal;
    afw_boolean_t found;

    data_type = NULL;
    internal = (const void *)(afw_size_t)1;
    found = afw_array_get_entry_internal(array, index,
        &data_type, &internal, xctx);
    if (found || internal != NULL ||
        afw_array_get_entry_value(array, index, xctx->p, xctx) != NULL)
    {
        fprintf(stderr, "%s: index %d treated as found\n",
            label, (int)index);
        return 1;
    }
    return 0;
}

static int
impl_expect_integer(
    const afw_array_t *array,
    afw_integer_t index,
    afw_integer_t expect,
    afw_xctx_t *xctx,
    const char *label)
{
    const afw_data_type_t *data_type;
    const void *internal;
    const afw_value_t *value;

    data_type = NULL;
    internal = NULL;
    if (!afw_array_get_entry_internal(array, index,
            &data_type, &internal, xctx) ||
        !internal ||
        data_type != afw_data_type_integer ||
        *(const afw_integer_t *)internal != expect)
    {
        fprintf(stderr, "%s: index %d missing or wrong internal\n",
            label, (int)index);
        return 1;
    }
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
impl_counted_direct(const afw_pool_t *p, afw_xctx_t *xctx)
{
    /*
     * Third slot is a sentinel: with `i > count` the view would return
     * 999 at index 2. With `i >= count` it must be not found.
     */
    afw_integer_t ints[3];
    const afw_array_t *array;
    int rc;

    ints[0] = 10;
    ints[1] = 20;
    ints[2] = 999;
    array = afw_array_create_view_of_c_array(
        ints, false, afw_data_type_integer, 2, p, xctx);
    rc = 0;
    rc |= impl_expect_integer(array, 0, 10, xctx, "direct");
    rc |= impl_expect_integer(array, 1, 20, xctx, "direct");
    rc |= impl_expect_missing(array, 2, xctx, "direct");
    rc |= impl_expect_missing(array, 3, xctx, "direct");
    rc |= impl_expect_missing(array, -1, xctx, "direct");
    return rc;
}

static int
impl_empty_with_storage(const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_integer_t dummy;
    const afw_array_t *array;

    dummy = 99;
    array = afw_array_create_view_of_c_array(
        &dummy, false, afw_data_type_integer, 0, p, xctx);
    return impl_expect_missing(array, 0, xctx, "empty");
}

static int
impl_counted_indirect(const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_integer_t ints[3];
    const void *slots[3];
    const afw_array_t *array;
    int rc;

    ints[0] = 10;
    ints[1] = 20;
    ints[2] = 999;
    slots[0] = &ints[0];
    slots[1] = &ints[1];
    slots[2] = &ints[2];
    array = afw_array_create_view_of_c_array(
        slots, true, afw_data_type_integer, 2, p, xctx);
    rc = 0;
    rc |= impl_expect_integer(array, 0, 10, xctx, "indirect");
    rc |= impl_expect_integer(array, 1, 20, xctx, "indirect");
    rc |= impl_expect_missing(array, 2, xctx, "indirect");
    return rc;
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

    if (strcmp(case_name, "direct") == 0) {
        rc = impl_counted_direct(p, xctx);
    }
    else if (strcmp(case_name, "empty") == 0) {
        rc = impl_empty_with_storage(p, xctx);
    }
    else if (strcmp(case_name, "indirect") == 0) {
        rc = impl_counted_indirect(p, xctx);
    }
    else {
        fprintf(stderr, "usage: array_view_index_probe "
            "direct|empty|indirect\n");
        rc = 2;
    }

    afw_pool_release(p, xctx);
    afw_environment_release(xctx);
    return rc;
}
