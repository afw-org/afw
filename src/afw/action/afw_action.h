// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework action header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_ACTION_H__
#define __AFW_ACTION_H__

#include "afw_interface.h"

/**
 * @defgroup afw_action Action perform
 * @ingroup afw_c_api_public
 *
 * Provides action perform.
 * @{
 */

/**
 * @file afw_action.h
 * @brief Functions for working with _AdaptiveAction_ objects.
 */

AFW_BEGIN_DECLARES

/**
 * @brief Perform actions(s) specified in _AdaptiveActions_ object.
 * @param request is _AdaptiveActions_ containing actions to perform.
 * @param response_content_type to use or NULL for default (application/json).
 * @param response is existing response object or if NULL, new one is created.
 * @param p used for result.
 * @param xctx of caller.
 * @return _AdaptiveResponse_ object.
 *
 * Request object can have a "function" parameter for a single function call
 * or an "actions" parameter with a list for objects for mutiple function
 * calls.
 * 
 * These parameters for the function call are specified as properties with the
 * name of the parameter.  In the case of multiple actions, the root request
 * object will be checked for missing parameters.
 */
AFW_DECLARE(const afw_object_t *)
afw_action_perform(
    const afw_object_t *request,
    const afw_content_type_t *response_content_type,
    const afw_object_t *response,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_ACTION_H__ */
