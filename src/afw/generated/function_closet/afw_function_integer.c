// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for integer
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_integer.c
 * @brief afw_function_execute_* functions for integer.
 */

#include "afw.h"



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
 *   function abs<integer>(
 *       value: integer
 *   ): integer;
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function add<integer>(
 *       values_1: integer,
 *       values_2: integer,
 *       ...values_rest: (list of integer)
 *   ): integer;
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function divide<integer>(
 *       dividend: integer,
 *       divisor: integer
 *   ): integer;
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function mod<integer>(
 *       dividend: integer,
 *       divisor: integer
 *   ): integer;
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function multiply<integer>(
 *       values_1: integer,
 *       values_2: integer,
 *       ...values_rest: (list of integer)
 *   ): integer;
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function negative<integer>(
 *       value: integer
 *   ): integer;
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function subtract<integer>(
 *       arg1: integer,
 *       arg2: integer
 *   ): integer;
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}
