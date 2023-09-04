// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for logical
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_logical.c
 * @brief afw_function_execute_* functions for logical.
 */

#include "afw.h"



/*
 * Adaptive function: and
 *
 * afw_function_execute_and
 *
 * See afw_function_bindings.h for more information.
 *
 * Evaluates 0 or more boolean conditions returning boolean true if there are no
 * conditions and boolean false if any condition evaluate to false. All
 * conditions after the first false remain unevaluated.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function and(
 *       ...conditions: (array of boolean)
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   conditions - (0 or more boolean)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_and(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: n_of
 *
 * afw_function_execute_n_of
 *
 * See afw_function_bindings.h for more information.
 *
 * integer n specifies the number of boolean conditions that follow that must
 * evaluate to true for boolean true to be returned. If n is 0, true is
 * returned. Once n conditions evaluate to true, true is returned and the
 * remaining conditions remain unevaluated.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function n_of(
 *       n: integer,
 *       ...conditions: (array of boolean)
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   n - (integer)
 *
 *   conditions - (0 or more boolean)
 *
 * Returns:
 *
 *   (boolean)
 *
 * Errors thrown:
 *
 *   arg_error - there are less than n conditions
 */
const afw_value_t *
afw_function_execute_n_of(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: not
 *
 * afw_function_execute_not
 *
 * See afw_function_bindings.h for more information.
 *
 * Evaluates boolean condition returning boolean true if condition evaluates to
 * false and false if condition evaluates to true.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function not(
 *       condition: boolean
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   condition - (boolean)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_not(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: or
 *
 * afw_function_execute_or
 *
 * See afw_function_bindings.h for more information.
 *
 * Evaluates 0 or more boolean conditions returning boolean false if there are
 * no conditions and boolean true if any condition evaluate to true. All
 * conditions after the first true remain unevaluated.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function or(
 *       ...conditions: (array of boolean)
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   conditions - (0 or more boolean)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_or(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
