// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for evaluated
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_value_evaluated.c
 * @brief Implementation of afw_value interface for evaluated
 */

#include "afw_internal.h"



AFW_DEFINE(afw_value_common_t *)
afw_value_common_allocate(
    const afw_data_type_t *data_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_common_t *result;

    if (!data_type->evaluated_value_inf) {
        AFW_THROW_ERROR_Z(general,
            "afw_value_common_allocate() called with data type special "
            "data type that does not have evaluated_value_inf",
            xctx);
    }

    result = afw_pool_calloc(p,
        offsetof(afw_value_common_t, internal) + data_type->c_type_size,
        xctx);
    result->inf = data_type->evaluated_value_inf;
    return result;
}


/* Create function for an evaluated data type value. */
AFW_DEFINE(const afw_value_t *)
afw_value_common_create(
    const void * value,
    const afw_data_type_t *data_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_common_t *result;

    if (!data_type) {
        AFW_THROW_ERROR_Z(general,
            "afw_value_common_create() called with NULL data type",
            xctx);
    }
    result = afw_pool_calloc(p,
        offsetof(afw_value_common_t, internal) + data_type->c_type_size,
        xctx);
    result->inf = data_type->evaluated_value_inf;
    memcpy(&result->internal, value, data_type->c_type_size);

    return &result->pub;
}
