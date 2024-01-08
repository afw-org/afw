// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Model Support
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_model.c
 * @brief Adaptive Framework model support
 */

#include "afw_internal.h"



/* Get the model object type for an object type id from model. */
AFW_DEFINE(const afw_model_object_type_t *)
afw_model_get_object_type(const afw_model_t *model,
    const afw_utf8_t *object_type_id, afw_xctx_t *xctx)
{
    return apr_hash_get(model->model_object_types,
        object_type_id->s, object_type_id->len);
}
