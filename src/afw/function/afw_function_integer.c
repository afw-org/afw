// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for Integer
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_integer.c
 * @brief afw_function_execute_* functions for integer.
 */

#include "afw_internal.h"



/*
 * Adaptive function: abs<integer>
 *
 * afw_function_execute_abs_integer
 *
 * See afw_function_bindings.h for more information.
 *
 * Compute the absolute value of the integer value and return the integer
 * result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function abs<integer>(
 *       value: integer
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   value - (integer)
 *
 * Returns:
 *
 *   (integer)
 */
const afw_value_t *
afw_function_execute_abs_integer(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *arg;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, 1, integer);

    return (arg->internal >= 0)
        ? &arg->pub
        : afw_value_create_integer(-(arg->internal), x->p, x->xctx);
}



/*
 * Adaptive function: add<integer>
 *
 * afw_function_execute_add_integer
 *
 * See afw_function_bindings.h for more information.
 *
 * Add 2 or more integer values and return the integer result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function add<integer>(
 *       values_1: integer,
 *       values_2: integer,
 *       ...values_rest: (array of integer)
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   values - (2 or more integer)
 *
 * Returns:
 *
 *   (integer)
 */
const afw_value_t *
afw_function_execute_add_integer(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *arg;
    afw_integer_t sum;
    afw_size_t n;

    for (sum = 0, n = 1; n <= x->argc; n++) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, n, integer);
        if ((arg->internal > 0 && (sum > AFW_INTEGER_MAX - arg->internal)) ||
            (arg->internal < 0 && (sum < AFW_INTEGER_MIN - arg->internal)))
        {
            AFW_THROW_ERROR_Z(arg_error, "Integer add overflow", x->xctx);
        }
        sum += arg->internal;
    }

    return afw_value_create_integer(sum, x->p, x->xctx);
}



/*
 * Adaptive function: divide<integer>
 *
 * afw_function_execute_divide_integer
 *
 * See afw_function_bindings.h for more information.
 *
 * Divide integer dividend by integer divisor and return the integer quotient.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function divide<integer>(
 *       dividend: integer,
 *       divisor: integer
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   dividend - (integer)
 *
 *   divisor - (integer)
 *
 * Returns:
 *
 *   (integer)
 */
const afw_value_t *
afw_function_execute_divide_integer(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *arg1;
    const afw_value_integer_t *arg2;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg1, 1, integer);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg2, 2, integer);

    if (arg2->internal == 0) {
        AFW_THROW_ERROR_Z(arg_error, "Integer divide by zero error", x->xctx);
    }

    return afw_value_create_integer(
        arg1->internal / arg2->internal,
        x->p, x->xctx);
}



/*
 * Adaptive function: mod<integer>
 *
 * afw_function_execute_mod_integer
 *
 * See afw_function_bindings.h for more information.
 *
 * Divide integer dividend by integer divisor and return the integer remainder.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function mod<integer>(
 *       dividend: integer,
 *       divisor: integer
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   dividend - (integer)
 *
 *   divisor - (integer)
 *
 * Returns:
 *
 *   (integer)
 */
const afw_value_t *
afw_function_execute_mod_integer(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *arg1;
    const afw_value_integer_t *arg2;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg1, 1, integer);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg2, 2, integer);

    if (arg2->internal == 0) {
        AFW_THROW_ERROR_Z(arg_error, "Integer divide by zero error", x->xctx);
    }

    return afw_value_create_integer(
        arg1->internal % arg2->internal,
        x->p, x->xctx);
}



/*
 * Adaptive function: multiply<integer>
 *
 * afw_function_execute_multiply_integer
 *
 * See afw_function_bindings.h for more information.
 *
 * Multiply 2 or more integer values and return the integer result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function multiply<integer>(
 *       values_1: integer,
 *       values_2: integer,
 *       ...values_rest: (array of integer)
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   values - (2 or more integer)
 *
 * Returns:
 *
 *   (integer)
 */
const afw_value_t *
afw_function_execute_multiply_integer(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *arg;
    afw_integer_t result, next;
    afw_size_t n;

    for (result = 1, next = 1, n = 1; n <= x->argc; n++) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, n, integer);
        next *= arg->internal;
        if (result != 0 && next / result != arg->internal) {
            AFW_THROW_ERROR_Z(arg_error, "Integer multiply overflow", x->xctx);
        } 
        result = next;       
    }

    return afw_value_create_integer(result, x->p, x->xctx);
}



/*
 * Adaptive function: subtract<integer>
 *
 * afw_function_execute_subtract_integer
 *
 * See afw_function_bindings.h for more information.
 *
 * Subtract integer arg2 from integer arg1 and return the integer result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function subtract<integer>(
 *       arg1: integer,
 *       arg2: integer
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   arg1 - (integer)
 *
 *   arg2 - (integer)
 *
 * Returns:
 *
 *   (integer)
 */
const afw_value_t *
afw_function_execute_subtract_integer(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *arg1;
    const afw_value_integer_t *arg2;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg1, 1, integer);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg2, 2, integer);


    if ((-arg2->internal < 0 && (arg1->internal < AFW_INTEGER_MIN - -arg2->internal)) ||
        (-arg2->internal > 0 && (arg1->internal > AFW_INTEGER_MAX - -arg2->internal)))
    {
        AFW_THROW_ERROR_Z(arg_error, "Integer subtract overflow", x->xctx);
    }

    return afw_value_create_integer(
        arg1->internal - arg2->internal,
        x->p, x->xctx);
}



/*
 * Adaptive function: to_double<integer>
 *
 * afw_function_execute_to_double_integer
 *
 * See afw_function_bindings.h for more information.
 *
 * Converts integer arg to double and returns double result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters.
 *
 * Parameters:
 *    arg - (integer)
 *
 * Returns:
 *    (double)
 */
const afw_value_t *
afw_function_execute_to_double_integer(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *arg;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, 1, integer);

    return afw_value_create_double(
        (afw_double_t)arg->internal,
        x->p, x->xctx);
}



/*
 * Adaptive function: negative<integer>
 *
 * afw_function_execute_negative_integer
 *
 * See afw_function_bindings.h for more information.
 *
 * Return negative of integer value.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function negative<integer>(
 *       value: integer
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   value - (integer)
 *
 * Returns:
 *
 *   (integer)
 */
const afw_value_t *
afw_function_execute_negative_integer(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *arg;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, 1, integer);

    return afw_value_create_integer(-arg->internal, x->p, x->xctx);
}
