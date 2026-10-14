// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for double
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_double.c
 * @brief afw_function_execute_* functions for double.
 */

#include "afw.h"



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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
 * An integer result is exact: a negative exponent or a result too large for an
 * integer is an error.
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: round<double>
 *
 * afw_function_execute_round_double
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Determine the integer closest to double value and return the double result. A
 * value halfway between two integers rounds toward positive infinity, as
 * ECMAScript Math.round and XPath fn:round do: round(2.5) is 3 and round(-2.5)
 * is -2.
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
