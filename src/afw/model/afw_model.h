// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework model support
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_MODEL_H__
#define __AFW_MODEL_H__

#include "afw_interface.h"

/**
 * @defgroup afw_model Model
 * @ingroup afw_c_api_public
 *
 * Model API.
 *
 * @{
 */

/**
 * @file afw_model.h
 * @brief Header file for Adaptive Framework model support
 */

AFW_BEGIN_DECLARES


/** Model adaptor factory instance. */
AFW_DECLARE_CONST_DATA(afw_adaptor_factory_t)
afw_adaptor_factory_model;


/** @brief Typedef for model adapt type enum.  */
typedef enum afw_model_adapt_e {
    afw_model_adapt_to_adaptor,
    afw_model_adapt_from_adaptor,
    afw_model_adapt_to_requestor,
    afw_model_adapt_from_requestor
} afw_model_adapt_t;


/**
 * @brief Create a model adaptor.
 * @param properties configuration.
 * @param p to use for adaptor resources.
 * @param xctx The execution context (xctx) adaptor will use.
 * @return New instance of model adaptor.
 */
AFW_DECLARE(const afw_adaptor_t *)
afw_model_adaptor_create_cede_p(
    const afw_object_t *properties,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Compile a model and add object types to associative array.
 * @param adaptor_id to use for object type paths.
 * @param model_object.
 * @param p to use.
 * @param xctx of caller.
 * @return model.
 */
AFW_DECLARE(afw_model_t *)
afw_model_compile(
    const afw_utf8_t *adaptor_id,
    const afw_object_t *model_object,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Get the model object type for an object type id from model.
 * @param model
 * @param object_type_id
 * @param xctx of caller.
 * @return afw_model_object_type_t or NULL if not found.
 */
AFW_DECLARE(const afw_model_object_type_t *)
afw_model_get_object_type(const afw_model_t *model,
    const afw_utf8_t *object_type_id, afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_MODEL_H__ */
