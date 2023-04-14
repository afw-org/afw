// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for Logical
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_Logical.c
 * @brief afw_function_execute_* functions for Logical.
 */

#include "afw_internal.h"



/*
 * Adaptive function: and
 *
 * afw_function_execute_and
 *
 * See afw_function_bindings.h for more information.
 *
 * Evaluates 0 or more boolean conditions returning boolean true if there are
 * no conditions and boolean false if any condition evaluate to false. All
 * conditions after the first false remain unevaluated.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 *   function and(
 *       ...conditions: (list of boolean)
 *   ): boolean;
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
    const afw_value_boolean_t *v;
    afw_size_t n;

    /* Return false if any arg evaluates to false. */
    for (n = 1; n <= x->argc; n++) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(v, n, boolean);
        if (!v->internal) {
            return afw_value_false;
        }
    }

    /* Return true if no args evaluated to false. */
    return afw_value_true;
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
 *   function n_of(
 *       n: integer,
 *       ...conditions: (list of boolean)
 *   ): boolean;
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
    const afw_value_integer_t *n;
    const afw_value_boolean_t *v;
    afw_size_t i;
    afw_integer_t trues;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(n, 1, integer);

    /* Evaluated args until end or there are enough trues. */
    for (i = 2, trues = 0; i <= x->argc &&
        trues < n->internal; i++)
    {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(v, i, boolean);
        if (v->internal) {
            trues++;
        }
    }

    /* Return true if there were enough trues. */
    return (trues >= n->internal)
        ? afw_value_true
        : afw_value_false;
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
 *   function not(
 *       condition: boolean
 *   ): boolean;
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
    const afw_value_boolean_t *condition;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(condition, 1, boolean);

    return (condition->internal)
        ? afw_value_false
        : afw_value_true;
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
 *   function or(
 *       ...conditions: (list of boolean)
 *   ): boolean;
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
    const afw_value_boolean_t *v;
    afw_size_t n;

    /* Return true if any arg evaluates to true. */
    for (n = 1; n <= x->argc; n++) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(v, n, boolean);
        if (v->internal) {
            return afw_value_true;
        }
    }

    /* Return false if no args evaluated to true. */
    return afw_value_false;
}
