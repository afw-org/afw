// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for double
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_double.c
 * @brief Adaptive function execute implementations for category `double`.
 *
 * No double operation takes or produces NaN (#628); dividing by zero is
 * an error too. Infinity is a value like any other.
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
 * See afw_function_bindings_internal.h for more information.
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
    afw_double_t v;

    v = afw_function_evaluate_double_parameter(x, 1);
    return afw_function_double_result(x, fabs(v));
}



/*
 * Adaptive function: add<double>
 *
 * afw_function_execute_add_double
 *
 * See afw_function_bindings_internal.h for more information.
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
 *       ...values_rest: double[]
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
    afw_double_t sum;
    afw_size_t n;

    for (sum = 0, n = 1; n <= x->argc; n++) {
        sum += afw_function_evaluate_double_parameter(x, n);
    }
    return afw_function_double_result(x, sum);
}



/*
 * Adaptive function: ceil<double>
 *
 * afw_function_execute_ceil_double
 *
 * See afw_function_bindings_internal.h for more information.
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
    afw_double_t v;

    v = afw_function_evaluate_double_parameter(x, 1);
    return afw_function_double_result(x, ceil(v));
}



/*
 * Adaptive function: divide<double>
 *
 * afw_function_execute_divide_double
 *
 * See afw_function_bindings_internal.h for more information.
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
    afw_double_t dividend;
    afw_double_t divisor;

    dividend = afw_function_evaluate_double_parameter(x, 1);
    divisor = afw_function_evaluate_double_parameter(x, 2);

    /* As for integers: dividing by zero is the accident (#628). */
    if (divisor == 0.0) {
        AFW_THROW_ERROR_Z(argument_error, "Double divide by zero error",
            x->xctx);
    }
    return afw_function_double_result(x, dividend / divisor);
}



/*
 * Adaptive function: mod<double>
 *
 * afw_function_execute_mod_double
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Divide double dividend by double divisor and return the double remainder.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function mod<double>(
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
afw_function_execute_mod_double(
    afw_function_execute_t *x)
{
    afw_double_t dividend;
    afw_double_t divisor;

    dividend = afw_function_evaluate_double_parameter(x, 1);
    divisor = afw_function_evaluate_double_parameter(x, 2);
    if (divisor == 0.0) {
        AFW_THROW_ERROR_Z(argument_error, "Double divide by zero error",
            x->xctx);
    }

    /* fmod: remainder with sign of dividend. */
    return afw_function_double_result(x, fmod(dividend, divisor));
}



/*
 * Adaptive function: is_finite
 *
 * afw_function_execute_is_finite
 *
 * See afw_function_bindings_internal.h for more information.
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
        ? afw_boolean_v_true
        : afw_boolean_v_false;
}



/*
 * Adaptive function: floor<double>
 *
 * afw_function_execute_floor_double
 *
 * See afw_function_bindings_internal.h for more information.
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
    afw_double_t v;

    v = afw_function_evaluate_double_parameter(x, 1);
    return afw_function_double_result(x, floor(v));
}



/*
 * Adaptive function: multiply<double>
 *
 * afw_function_execute_multiply_double
 *
 * See afw_function_bindings_internal.h for more information.
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
 *       ...values_rest: double[]
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
    afw_double_t result;
    afw_size_t n;

    for (result = 1, n = 1; n <= x->argc; n++) {
        result *= afw_function_evaluate_double_parameter(x, n);
    }
    return afw_function_double_result(x, result);
}



/*
 * Adaptive function: is_NaN
 *
 * afw_function_execute_is_NaN
 *
 * See afw_function_bindings_internal.h for more information.
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
        ? afw_boolean_v_true
        : afw_boolean_v_false;
}



/*
 * Adaptive function: pow<double>
 *
 * afw_function_execute_pow_double
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * This returns the value of base raised to a power. Multiple exponents can be
 * specified to raise the previous exponent to the power of the latter exponent.
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
 *       ...exponent_rest: double[]
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
    afw_double_t exponent;
    afw_double_t base;
    afw_size_t i;

    /* Right to left: pow(a, b, c) is a raised to (b raised to c). */
    exponent = afw_function_evaluate_double_parameter(x, x->argc);
    for (i = x->argc - 1; i >= 1; i--) {
        base = afw_function_evaluate_double_parameter(x, i);
        /* 1 raised to an infinity is NaN, as in ECMAScript (C gives 1). */
        exponent = (fabs(base) == 1.0 && !afw_number_is_finite(exponent))
            ? x->xctx->env->NaN
            : pow(base, exponent);
        if (afw_number_is_NaN(exponent)) {
            break;
        }
    }
    return afw_function_double_result(x, exponent);
}



/*
 * Adaptive function: round<double>
 *
 * afw_function_execute_round_double
 *
 * See afw_function_bindings_internal.h for more information.
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
    afw_double_t v;

    v = afw_function_evaluate_double_parameter(x, 1);
    return afw_function_double_result(x, round(v));
}



/*
 * Adaptive function: subtract<double>
 *
 * afw_function_execute_subtract_double
 *
 * See afw_function_bindings_internal.h for more information.
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
    afw_double_t arg1;
    afw_double_t arg2;

    arg1 = afw_function_evaluate_double_parameter(x, 1);
    arg2 = afw_function_evaluate_double_parameter(x, 2);
    return afw_function_double_result(x, arg1 - arg2);
}



/*
 * Adaptive function: to_integer<double>
 *
 * afw_function_execute_to_integer_double
 *
 * See afw_function_bindings_internal.h for more information.
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
        AFW_THROW_ERROR_Z(argument_error, "arg is not a number", x->xctx);
    };
    if (!afw_number_is_finite(arg->internal)) {
        AFW_THROW_ERROR_Z(argument_error, "arg is infinite", x->xctx);
    };

    d = trunc(arg->internal);
    /* Outside the integer range the cast is undefined (gave INT64_MIN). */
    if (!(d >= -9223372036854775808.0 && d < 9223372036854775808.0)) {
        AFW_THROW_ERROR_Z(argument_error,
            "arg is out of the integer range", x->xctx);
    }
    return afw_value_create_unmanaged_integer((afw_integer_t)d, x->p, x->xctx);
}



/*
 * Adaptive function: negative<double>
 *
 * afw_function_execute_negative_double
 *
 * See afw_function_bindings_internal.h for more information.
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
    afw_double_t v;

    v = afw_function_evaluate_double_parameter(x, 1);
    return afw_function_double_result(x, -v);
}



/*
 * Adaptive function: acos<double>
 *
 * afw_function_execute_acos_double
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the arccosine, in radians from 0 to #pi, of the double value, as
 * ECMAScript Math.acos does. A value outside -1.0 to 1.0 is an error. A result
 * that would be NaN, or a NaN argument, is an error. An integer is not
 * accepted: write to_double(v).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function acos<double>(
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
 *
 * Errors thrown:
 *
 *   argument_error - the result would be NaN, or an operand is NaN
 */
const afw_value_t *
afw_function_execute_acos_double(
    afw_function_execute_t *x)
{
    afw_double_t v;

    v = afw_function_evaluate_double_parameter(x, 1);
    return afw_function_double_result(x, acos(v));
}



/*
 * Adaptive function: asin<double>
 *
 * afw_function_execute_asin_double
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the arcsine, in radians from -#pi/2 to #pi/2, of the double value, as
 * ECMAScript Math.asin does. A value outside -1.0 to 1.0 is an error. A result
 * that would be NaN, or a NaN argument, is an error. An integer is not
 * accepted: write to_double(v).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function asin<double>(
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
 *
 * Errors thrown:
 *
 *   argument_error - the result would be NaN, or an operand is NaN
 */
const afw_value_t *
afw_function_execute_asin_double(
    afw_function_execute_t *x)
{
    afw_double_t v;

    v = afw_function_evaluate_double_parameter(x, 1);
    return afw_function_double_result(x, asin(v));
}



/*
 * Adaptive function: atan2<double>
 *
 * afw_function_execute_atan2_double
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the angle, in radians from -#pi to #pi, from the positive x axis to
 * the point (x, y), as ECMAScript Math.atan2(y, x) does; y comes first. A
 * result that would be NaN, or a NaN argument, is an error. An integer is not
 * accepted: write to_double(v).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function atan2<double>(
 *       y: double,
 *       x: double
 *   ): double;
 * ```
 *
 * Parameters:
 *
 *   y - (double) The y coordinate.
 *
 *   x - (double) The x coordinate.
 *
 * Returns:
 *
 *   (double)
 *
 * Errors thrown:
 *
 *   argument_error - the result would be NaN, or an operand is NaN
 */
const afw_value_t *
afw_function_execute_atan2_double(
    afw_function_execute_t *x)
{
    afw_double_t y;
    afw_double_t v;

    y = afw_function_evaluate_double_parameter(x, 1);
    v = afw_function_evaluate_double_parameter(x, 2);
    return afw_function_double_result(x, atan2(y, v));
}



/*
 * Adaptive function: atan<double>
 *
 * afw_function_execute_atan_double
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the arctangent, in radians from -#pi/2 to #pi/2, of the double value,
 * as ECMAScript Math.atan does. A result that would be NaN, or a NaN argument,
 * is an error. An integer is not accepted: write to_double(v).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function atan<double>(
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
 *
 * Errors thrown:
 *
 *   argument_error - the result would be NaN, or an operand is NaN
 */
const afw_value_t *
afw_function_execute_atan_double(
    afw_function_execute_t *x)
{
    afw_double_t v;

    v = afw_function_evaluate_double_parameter(x, 1);
    return afw_function_double_result(x, atan(v));
}



/*
 * Adaptive function: cbrt<double>
 *
 * afw_function_execute_cbrt_double
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the cube root of the double value, as ECMAScript Math.cbrt does. A
 * result that would be NaN, or a NaN argument, is an error. An integer is not
 * accepted: write to_double(v).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function cbrt<double>(
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
 *
 * Errors thrown:
 *
 *   argument_error - the result would be NaN, or an operand is NaN
 */
const afw_value_t *
afw_function_execute_cbrt_double(
    afw_function_execute_t *x)
{
    afw_double_t v;
    afw_double_t r;
    afw_double_t n;

    /*
     * Some C libraries miss a perfect cube by one unit in the last place
     * (cbrt(27.0) gave 3.0000000000000004); ECMAScript gives 3.
     */
    v = afw_function_evaluate_double_parameter(x, 1);
    r = cbrt(v);
    n = nearbyint(r);
    if (n * n * n == v) {
        r = n;
    }
    return afw_function_double_result(x, r);
}



/*
 * Adaptive function: cos<double>
 *
 * afw_function_execute_cos_double
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the cosine of the double value in radians, as ECMAScript Math.cos does
 * (#pi is π). An infinite value is an error. A result that would be NaN, or a
 * NaN argument, is an error. An integer is not accepted: write to_double(v).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function cos<double>(
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
 *
 * Errors thrown:
 *
 *   argument_error - the result would be NaN, or an operand is NaN
 */
const afw_value_t *
afw_function_execute_cos_double(
    afw_function_execute_t *x)
{
    afw_double_t v;

    v = afw_function_evaluate_double_parameter(x, 1);
    return afw_function_double_result(x, cos(v));
}



/*
 * Adaptive function: exp<double>
 *
 * afw_function_execute_exp_double
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return #e raised to the double value, as ECMAScript Math.exp does. A result
 * that would be NaN, or a NaN argument, is an error. An integer is not
 * accepted: write to_double(v).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function exp<double>(
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
 *
 * Errors thrown:
 *
 *   argument_error - the result would be NaN, or an operand is NaN
 */
const afw_value_t *
afw_function_execute_exp_double(
    afw_function_execute_t *x)
{
    afw_double_t v;

    v = afw_function_evaluate_double_parameter(x, 1);
    return afw_function_double_result(x, exp(v));
}



/*
 * Adaptive function: hypot<double>
 *
 * afw_function_execute_hypot_double
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the square root of the sum of the squares of the double values, as
 * ECMAScript Math.hypot does, without overflow or underflow in between. At
 * least one value is required (ECMAScript's hypot() is 0); an infinite value
 * makes the result Infinity. A result that would be NaN, or a NaN argument, is
 * an error. An integer is not accepted: write to_double(v).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function hypot<double>(
 *       values_1: double,
 *       ...values_rest: double[]
 *   ): double;
 * ```
 *
 * Parameters:
 *
 *   values - (1 or more double) The values.
 *
 * Returns:
 *
 *   (double)
 *
 * Errors thrown:
 *
 *   argument_error - the result would be NaN, or an operand is NaN
 */
const afw_value_t *
afw_function_execute_hypot_double(
    afw_function_execute_t *x)
{
    afw_double_t result;
    afw_size_t n;

    /* hypot two at a time: no overflow or underflow in between. */
    result = afw_function_evaluate_double_parameter(x, 1);
    result = fabs(result);
    for (n = 2; n <= x->argc; n++) {
        result = hypot(result, afw_function_evaluate_double_parameter(x, n));
    }
    return afw_function_double_result(x, result);
}



/*
 * Adaptive function: log10<double>
 *
 * afw_function_execute_log10_double
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the base 10 logarithm of the double value, as ECMAScript Math.log10
 * does. log10(0.0) is -Infinity; a negative value is an error. A result that
 * would be NaN, or a NaN argument, is an error. An integer is not accepted:
 * write to_double(v).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function log10<double>(
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
 *
 * Errors thrown:
 *
 *   argument_error - the result would be NaN, or an operand is NaN
 */
const afw_value_t *
afw_function_execute_log10_double(
    afw_function_execute_t *x)
{
    afw_double_t v;

    v = afw_function_evaluate_double_parameter(x, 1);
    return afw_function_double_result(x, log10(v));
}



/*
 * Adaptive function: log2<double>
 *
 * afw_function_execute_log2_double
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the base 2 logarithm of the double value, as ECMAScript Math.log2
 * does. log2(0.0) is -Infinity; a negative value is an error. A result that
 * would be NaN, or a NaN argument, is an error. An integer is not accepted:
 * write to_double(v).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function log2<double>(
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
 *
 * Errors thrown:
 *
 *   argument_error - the result would be NaN, or an operand is NaN
 */
const afw_value_t *
afw_function_execute_log2_double(
    afw_function_execute_t *x)
{
    afw_double_t v;

    v = afw_function_evaluate_double_parameter(x, 1);
    return afw_function_double_result(x, log2(v));
}



/*
 * Adaptive function: log<double>
 *
 * afw_function_execute_log_double
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the natural logarithm (base #e) of the double value, as ECMAScript
 * Math.log does. log(0.0) is -Infinity; a negative value is an error. A result
 * that would be NaN, or a NaN argument, is an error. An integer is not
 * accepted: write to_double(v).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function log<double>(
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
 *
 * Errors thrown:
 *
 *   argument_error - the result would be NaN, or an operand is NaN
 */
const afw_value_t *
afw_function_execute_log_double(
    afw_function_execute_t *x)
{
    afw_double_t v;

    v = afw_function_evaluate_double_parameter(x, 1);
    return afw_function_double_result(x, log(v));
}



/*
 * Adaptive function: sign<double>
 *
 * afw_function_execute_sign_double
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the sign of the double value, as ECMAScript Math.sign does: -1, 0 or 1
 * of the same data type (`sign<double>` keeps the sign of a zero: sign(-0.0) is
 * -0.0). A NaN argument is an error.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function sign<double>(
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
 *
 * Errors thrown:
 *
 *   argument_error - the value is NaN
 */
const afw_value_t *
afw_function_execute_sign_double(
    afw_function_execute_t *x)
{
    afw_double_t v;

    /* A zero keeps its sign, as in ECMAScript. */
    v = afw_function_evaluate_double_parameter(x, 1);
    return afw_function_double_result(x,
        (v > 0.0) ? 1.0 : (v < 0.0) ? -1.0 : v);
}



/*
 * Adaptive function: sin<double>
 *
 * afw_function_execute_sin_double
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the sine of the double value in radians, as ECMAScript Math.sin does
 * (#pi is π). An infinite value is an error. A result that would be NaN, or a
 * NaN argument, is an error. An integer is not accepted: write to_double(v).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function sin<double>(
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
 *
 * Errors thrown:
 *
 *   argument_error - the result would be NaN, or an operand is NaN
 */
const afw_value_t *
afw_function_execute_sin_double(
    afw_function_execute_t *x)
{
    afw_double_t v;

    v = afw_function_evaluate_double_parameter(x, 1);
    return afw_function_double_result(x, sin(v));
}



/*
 * Adaptive function: sqrt<double>
 *
 * afw_function_execute_sqrt_double
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the square root of the double value, as ECMAScript Math.sqrt does;
 * sqrt(-0.0) is -0.0. A negative value is an error. A result that would be NaN,
 * or a NaN argument, is an error. An integer is not accepted: write
 * to_double(v).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function sqrt<double>(
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
 *
 * Errors thrown:
 *
 *   argument_error - the result would be NaN, or an operand is NaN
 */
const afw_value_t *
afw_function_execute_sqrt_double(
    afw_function_execute_t *x)
{
    afw_double_t v;

    v = afw_function_evaluate_double_parameter(x, 1);
    return afw_function_double_result(x, sqrt(v));
}



/*
 * Adaptive function: tan<double>
 *
 * afw_function_execute_tan_double
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the tangent of the double value in radians, as ECMAScript Math.tan
 * does (#pi is π). An infinite value is an error. A result that would be NaN,
 * or a NaN argument, is an error. An integer is not accepted: write
 * to_double(v).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function tan<double>(
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
 *
 * Errors thrown:
 *
 *   argument_error - the result would be NaN, or an operand is NaN
 */
const afw_value_t *
afw_function_execute_tan_double(
    afw_function_execute_t *x)
{
    afw_double_t v;

    v = afw_function_evaluate_double_parameter(x, 1);
    return afw_function_double_result(x, tan(v));
}



/*
 * Adaptive function: trunc<double>
 *
 * afw_function_execute_trunc_double
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the integer part of the double value, removing any fraction, as
 * ECMAScript Math.trunc does (toward zero: trunc(-1.5) is -1.0). The result is
 * a double: write to_integer(trunc(v)) for an integer. A result that would be
 * NaN, or a NaN argument, is an error. An integer is not accepted: write
 * to_double(v).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function trunc<double>(
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
 *
 * Errors thrown:
 *
 *   argument_error - the result would be NaN, or an operand is NaN
 */
const afw_value_t *
afw_function_execute_trunc_double(
    afw_function_execute_t *x)
{
    afw_double_t v;

    v = afw_function_evaluate_double_parameter(x, 1);
    return afw_function_double_result(x, trunc(v));
}
