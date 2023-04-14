// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for double
 *
 * Copyright (c) 2010-2023 Clemson University
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
 * See afw_function_bindings.h for more information.
 *
 * Compute the absolute value of the double value and return the double result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 *   function abs<double>(
 *       value: double
 *   ): double;
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function add<double>(
 *       values_1: double,
 *       values_2: double,
 *       ...values_rest: (list of double)
 *   ): double;
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function ceil<double>(
 *       value: double
 *   ): double;
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function divide<double>(
 *       dividend: double,
 *       divisor: double
 *   ): double;
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function floor<double>(
 *       number: double
 *   ): double;
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function is_NaN(
 *       number: double
 *   ): boolean;
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function is_finite(
 *       number: double
 *   ): boolean;
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function multiply<double>(
 *       values_1: double,
 *       values_2: double,
 *       ...values_rest: (list of double)
 *   ): double;
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function negative<double>(
 *       value: double
 *   ): double;
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function pow<double>(
 *       base: double,
 *       exponent_1: double,
 *       ...exponent_rest: (list of double)
 *   ): double;
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function round<double>(
 *       number: double
 *   ): double;
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function subtract<double>(
 *       arg1: double,
 *       arg2: double
 *   ): double;
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}
