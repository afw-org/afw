// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for Double
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_double.c
 * @brief afw_function_execute_* functions for double.
 *
 * An error is thrown an argument is NaN, -Infinity, or Infinity.
 */

#include "afw_internal.h"

/** @fixme
 * A lot of the argument check may be able to be replaced by signals.
 */

/*
 * Adaptive function: abs<double>
 *
 * afw_function_execute_abs_double
 *
 * See afw_function_bindings.h for more information.
 *
 * Compute the absolute value of the double value and return the double result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function abs<double>(
 *       value: double
 *   ): double;
 * ```
 *
 * Parameters:
 *
 *   value - (double)
 *
 * Returns:
 *
 *   (double)
 */
const afw_value_t *
afw_function_execute_abs_double(
    afw_function_execute_t *x)
{
    const afw_value_double_t *arg;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, 1, double);

    if (afw_number_is_NaN(arg->internal)) {
        AFW_THROW_ERROR_Z(arg_error, "arg is not a number", x->xctx);
    };
    if (!afw_number_is_finite(arg->internal)) {
        AFW_THROW_ERROR_Z(arg_error, "arg is infinite", x->xctx);
    };

    return afw_value_create_double(fabs(arg->internal), x->p, x->xctx);
}



/*
 * Adaptive function: add<double>
 *
 * afw_function_execute_add_double
 *
 * See afw_function_bindings.h for more information.
 *
 * Add 2 or more double values and return the double result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function add<double>(
 *       values_1: double,
 *       values_2: double,
 *       ...values_rest: (list of double)
 *   ): double;
 * ```
 *
 * Parameters:
 *
 *   values - (2 or more double)
 *
 * Returns:
 *
 *   (double)
 */
const afw_value_t *
afw_function_execute_add_double(
    afw_function_execute_t *x)
{
    const afw_value_double_t *arg;
    double sum;
    afw_size_t n;

    /* If sum become infinite before last parameter, error is thrown. */
    for (sum = 0, n = 1; n <= x->argc; n++) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, n, double);
        sum += arg->internal;
    }

    return afw_value_create_double(sum, x->p, x->xctx);
}



/*
 * Adaptive function: ceil<double>
 *
 * afw_function_execute_ceil_double
 *
 * See afw_function_bindings.h for more information.
 *
 * Determine the smallest integer that is greater then or equal to the double
 * value and return the double result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function ceil<double>(
 *       value: double
 *   ): double;
 * ```
 *
 * Parameters:
 *
 *   value - (double)
 *
 * Returns:
 *
 *   (double)
 */
const afw_value_t *
afw_function_execute_ceil_double(
    afw_function_execute_t *x)
{
    const afw_value_double_t *arg;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, 1, double);

    return afw_value_create_double(ceil(arg->internal), x->p, x->xctx);
}



/*
 * Adaptive function: divide<double>
 *
 * afw_function_execute_divide_double
 *
 * See afw_function_bindings.h for more information.
 *
 * Divide double dividend by double divisor and return the double quotient.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function divide<double>(
 *       dividend: double,
 *       divisor: double
 *   ): double;
 * ```
 *
 * Parameters:
 *
 *   dividend - (double)
 *
 *   divisor - (double)
 *
 * Returns:
 *
 *   (double)
 */
const afw_value_t *
afw_function_execute_divide_double(
    afw_function_execute_t *x)
{
    const afw_value_double_t *dividend;
    const afw_value_double_t *divisor;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(dividend, 1, double);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(divisor, 2, double);

    return afw_value_create_double(
        dividend->internal / divisor->internal, x->p, x->xctx);
}



/*
 * Adaptive function: is_finite
 *
 * afw_function_execute_is_finite
 *
 * See afw_function_bindings.h for more information.
 *
 * Checks if the argument 'number' is finite and returns the boolean result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function is_finite(
 *       number: double
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   number - (double) Number to check.
 *
 * Returns:
 *
 *   (boolean) True if the argument 'number' is finite.
 */
const afw_value_t *
afw_function_execute_is_finite(
    afw_function_execute_t *x)
{
    const afw_value_double_t *number;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(number, 1, double);

    return afw_number_is_finite(number->internal)
        ? afw_value_true
        : afw_value_false;
}



/*
 * Adaptive function: floor<double>
 *
 * afw_function_execute_floor_double
 *
 * See afw_function_bindings.h for more information.
 *
 * Determine the largest integer that is smaller then or equal to the double
 * value and return the double result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function floor<double>(
 *       number: double
 *   ): double;
 * ```
 *
 * Parameters:
 *
 *   number - (double)
 *
 * Returns:
 *
 *   (double)
 */
const afw_value_t *
afw_function_execute_floor_double(
    afw_function_execute_t *x)
{
    const afw_value_double_t *arg;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, 1, double);

    return afw_value_create_double(floor(arg->internal), x->p, x->xctx);
}



/*
 * Adaptive function: multiply<double>
 *
 * afw_function_execute_multiply_double
 *
 * See afw_function_bindings.h for more information.
 *
 * Multiply 2 or more double values and return the double result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function multiply<double>(
 *       values_1: double,
 *       values_2: double,
 *       ...values_rest: (list of double)
 *   ): double;
 * ```
 *
 * Parameters:
 *
 *   values - (2 or more double)
 *
 * Returns:
 *
 *   (double)
 */
const afw_value_t *
afw_function_execute_multiply_double(
    afw_function_execute_t *x)
{
    const afw_value_double_t *arg;
    double result;
    afw_size_t n;

    /* Loop multiplying parameters. */
    for (result = 1, n = 1; n <= x->argc; n++) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, n, double);
        result *= arg->internal;
    }

    return afw_value_create_double(result, x->p, x->xctx);
}



/*
 * Adaptive function: is_NaN
 *
 * afw_function_execute_is_NaN
 *
 * See afw_function_bindings.h for more information.
 *
 * Checks if the argument 'number' is not a number(NaN) and returns the boolean
 * result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function is_NaN(
 *       number: double
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   number - (double) Number to check.
 *
 * Returns:
 *
 *   (boolean) True if the argument 'number' is not a number.
 */
const afw_value_t *
afw_function_execute_is_NaN(
    afw_function_execute_t *x)
{
    const afw_value_double_t *number;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(number, 1, double);

    return afw_number_is_NaN(number->internal)
        ? afw_value_true
        : afw_value_false;
}



/*
 * Adaptive function: pow<double>
 *
 * afw_function_execute_pow_double
 *
 * See afw_function_bindings.h for more information.
 *
 * This returns the value of base raised to a power. Multiple exponents can be
 * specified to raise the previous exponent to the power of the latter
 * exponent.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function pow<double>(
 *       base: double,
 *       exponent_1: double,
 *       ...exponent_rest: (list of double)
 *   ): double;
 * ```
 *
 * Parameters:
 *
 *   base - (double) Base value.
 *
 *   exponent - (1 or more double) Exponent value.
 *
 * Returns:
 *
 *   (double) Base raised to the power.
 */
const afw_value_t *
afw_function_execute_pow_double(
    afw_function_execute_t *x)
{
    const afw_value_double_t *base;
    const afw_value_double_t *exponent;
    afw_value_double_t *result;
    afw_size_t i;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(exponent,
        x->argc, double);
    result = afw_value_allocate_double(x->p, x->xctx);

    for (i = x->argc - 1; i >= 1; i--) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(base, i, double);
        /*
         * If absolute value of base is 1, and exponent is Infinity, then result
         * is NaN.
         */
        if (fabs(base->internal) == 1.0 &&
            !afw_number_is_finite(exponent->internal))
        {
            result->internal = x->xctx->env->NaN;
            break;      
        }
        else {
            result->internal = pow(base->internal, exponent->internal);
            if (!afw_number_is_finite(result->internal)) {
                break;
            }
            exponent = (const afw_value_double_t *)result;
        }
    }

    return (const afw_value_t *)result;
}



/*
 * Adaptive function: round<double>
 *
 * afw_function_execute_round_double
 *
 * See afw_function_bindings.h for more information.
 *
 * Determine the integer closest to double value and return the double result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function round<double>(
 *       number: double
 *   ): double;
 * ```
 *
 * Parameters:
 *
 *   number - (double)
 *
 * Returns:
 *
 *   (double)
 */
const afw_value_t *
afw_function_execute_round_double(
    afw_function_execute_t *x)
{
    const afw_value_double_t *arg;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, 1, double);

    /* Note: Uses round(), which seems right.  Might should use rint()? */
    return afw_value_create_double(round(arg->internal), x->p, x->xctx);
}



/*
 * Adaptive function: subtract<double>
 *
 * afw_function_execute_subtract_double
 *
 * See afw_function_bindings.h for more information.
 *
 * Subtract double arg2 from double arg1 and return the double result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function subtract<double>(
 *       arg1: double,
 *       arg2: double
 *   ): double;
 * ```
 *
 * Parameters:
 *
 *   arg1 - (double)
 *
 *   arg2 - (double)
 *
 * Returns:
 *
 *   (double)
 */
const afw_value_t *
afw_function_execute_subtract_double(
    afw_function_execute_t *x)
{
    const afw_value_double_t *arg1;
    const afw_value_double_t *arg2;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg1, 1, double);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg2, 2, double);

    return afw_value_create_double(arg1->internal - arg2->internal,
        x->p, x->xctx);
}



/*
 * Adaptive function: to_integer<double>
 *
 * afw_function_execute_to_integer_double
 *
 * See afw_function_bindings.h for more information.
 *
 * Truncate double arg to a whole number and returns integer result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters.
 *
 * Parameters:
 *    arg - (double)
 *
 * Returns:
 *    (integer)
 */
const afw_value_t *
afw_function_execute_to_integer_double(
    afw_function_execute_t *x)
{
    const afw_value_double_t *arg;
    double d;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, 1, double);

    if (afw_number_is_NaN(arg->internal)) {
        AFW_THROW_ERROR_Z(arg_error, "arg is not a number", x->xctx);
    };
    if (!afw_number_is_finite(arg->internal)) {
        AFW_THROW_ERROR_Z(arg_error, "arg is infinite", x->xctx);
    };

    d = trunc(arg->internal);
    return afw_value_create_integer((afw_integer_t)d, x->p, x->xctx);
}



/*
 * Adaptive function: negative<double>
 *
 * afw_function_execute_negative_double
 *
 * See afw_function_bindings.h for more information.
 *
 * Return negative of double value.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function negative<double>(
 *       value: double
 *   ): double;
 * ```
 *
 * Parameters:
 *
 *   value - (double)
 *
 * Returns:
 *
 *   (double)
 */
const afw_value_t *
afw_function_execute_negative_double(
    afw_function_execute_t *x)
{
    const afw_value_double_t *arg;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, 1, double);

    return afw_value_create_double(-arg->internal, x->p, x->xctx);
}
