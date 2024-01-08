// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Authorization
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_AUTHORIZATION_IMPL_H__
#define __AFW_AUTHORIZATION_IMPL_H__

#include "afw_interface.h"

/**
 * @defgroup afw_authorization_handler_impl afw_authorization_helper
 * @ingroup afw_c_api_impl
 * 
 * Helpers for afw_authorization_helper implementations.
 * 
 * @{
 */

/**
 * @file afw_authorization_impl.h
 * @brief Adaptive Framework authorization implementation support.
 *
 *  These are functions, macros, and structs for use by authorization handler
 *  implementation developers.
 */

AFW_BEGIN_DECLARES

/**
 * @brief Developers should call this in all create functions for a
 *      afw_authorization_handler.
 * @param inf afw_authorization_handler_inf_t pointer for implementation.
 * @param instance_size 0 or size greater than
 *      sizeof(afw_authorization_handler_t).
 * @param properties config object.
 * @param start is function to call to start authorization_handler.
 * @param stop is function to call to stop authorization_handler.
 * @param p to use as parent when creating authorization_handler pool.
 * @param xctx of caller.
 * @return instance of afw_authorization_handler_t, optionally with
 *      extra memory based on instance_size parameter.
 *
 *  This function creates and initializes an afw_authorization_handler
 *  instance.  After calling this function, the caller should do additional
 *  authorization_handler type specific processing.  The instance_size
 *  parameter can be specified to get a larger instance size than
 *  sizeof(afw_authorization_handler_t) for authorization_handler type
 *  specific use.
 *
 *  A new pool is created in the callings xctx's pool for use by the
 *  authorization_handler.
 *
 *  This function validates and normalizes the authorization_handler
 *  properties based on
 *  `/afw/_AdaptiveObjectType_/` ...
 *  `_AdaptiveConf_authorization_handler_<type>`,
 *  where `<type>` is the value of the authorization_handlerType property.
 *
 *  All properties from
 *  `/afw/_AdaptiveObjectType_/_AdaptiveConf_authorization_handler` are
 *  processed.
 *
 *  Some additional properties are processed if they pass validation.
 *  Properties isModelLocation and isPolicyLocation cause the appropriate core
 *  object types to be supported.
 *
 *  See afw_authorization_handler_impl_s for more information on how on how
 *  common authorization_handler code uses this information.
 */
AFW_DECLARE(afw_authorization_handler_t *)
afw_authorization_handler_impl_create_cede_p(
    const afw_authorization_handler_inf_t *inf,
    afw_size_t instance_size,
    const afw_object_t *properties,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_END_DECLARES

/** @} */

#endif /* __AFW_AUTHORIZATION_IMPL_H__ */
