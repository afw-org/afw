// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for value
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_value.c
 * @brief afw_function_execute_* functions for value.
 */

#include "afw.h"



/*
 * Adaptive function: meta
 *
 * afw_function_execute_meta
 *
 * See afw_function_bindings.h for more information.
 *
 * This function returns an object containing meta information for an adaptive
 * value.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function meta(
 *       value: any
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   value - (any dataType) This is the adaptive value whose meta object is to
 *       be returned.
 *
 * Returns:
 *
 *   (object) An object containing the meta information for the adaptive value.
 *       The objectType of this object is normally _AdaptiveValueMeta_ or one of
 *       its descendants.
 */
const afw_value_t *
afw_function_execute_meta(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: metas
 *
 * afw_function_execute_metas
 *
 * See afw_function_bindings.h for more information.
 *
 * This function returns an array of the result of calling meta() for each value
 * in an array or object adaptive value.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function metas(
 *       value: any
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   value - (any dataType) This is an adaptive array or object.
 *
 * Returns:
 *
 *   (object) An array containing the result of calling meta() of each value in
 *       array or object adaptive value.
 */
const afw_value_t *
afw_function_execute_metas(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
