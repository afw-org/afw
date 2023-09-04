// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for authorization
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_authorization.c
 * @brief afw_function_execute_* functions for authorization.
 */

#include "afw.h"



/*
 * Adaptive function: authorization_check
 *
 * afw_function_execute_authorization_check
 *
 * See afw_function_bindings.h for more information.
 *
 * This function can be used to check if the current subject has a given access
 * to the specified resource in the current environment.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function authorization_check(
 *       requestId: string,
 *       resourceId: string,
 *       actionId: string,
 *       object?: object,
 *       enforce?: boolean
 *   ): (object _AdaptiveAuthorizationResult_);
 * ```
 *
 * Parameters:
 *
 *   requestId - (string) This is a request id to associate with the
 *       authorization request. Variable current::requestId can be used to
 *       access this value in authorization policies.
 *
 *   resourceId - (string) This is the URI of the resource being checked.
 *       Variable current::resourceId can be used to access this value in
 *       authorization policies.
 *
 *   actionId - (string) This is the action that on the resource that's being
 *       checked.  Variable current::actionId can be used to access this value
 *       in authorization policies.
 *
 *   object - (optional object) Variable current::object can be used to access
 *       this value in authorization policies.
 *
 *   enforce - (optional boolean) This specifies whether an error is thrown if
 *       access is denied. If false or not specified, an error is not thrown and
 *       the decision can be checked in the returned object.
 *
 * Returns:
 *
 *   (object _AdaptiveAuthorizationResult_) The authorization result.
 */
const afw_value_t *
afw_function_execute_authorization_check(
    afw_function_execute_t *x)
{
    const afw_value_t *requestId;
    const afw_value_t *resourceId;
    const afw_value_t *actionId;
    const afw_value_t *object;
    const afw_value_boolean_t *enforce;
    const afw_value_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(requestId, 1);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(resourceId, 2);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(actionId, 3);
    AFW_FUNCTION_EVALUATE_PARAMETER(object, 4);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(enforce, 5, boolean);

    result = afw_authorization_check(
        enforce ? enforce->internal : false,
        requestId,
        resourceId,
        object,
        actionId,
        x->p,
        x->xctx);

    return result;
}
