// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for random
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_random.c
 * @brief afw_function_execute_* functions for random.
 */

#include "afw.h"



/*
 * Adaptive function: random_base64Binary
 *
 * afw_function_execute_random_base64Binary
 *
 * See afw_function_bindings.h for more information.
 *
 * This returns a specified number of random octets as dataType base64Binary.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function random_base64Binary(
 *       numberOfOctets: integer
 *   ): base64Binary;
 * ```
 *
 * Parameters:
 *
 *   numberOfOctets - (integer) The number of random octets to generate.
 *
 * Returns:
 *
 *   (base64Binary)
 */
const afw_value_t *
afw_function_execute_random_base64Binary(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: random_digits
 *
 * afw_function_execute_random_digits
 *
 * See afw_function_bindings.h for more information.
 *
 * Generate a string containing a specified number of random decimal digits.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function random_digits(
 *       numberOfDigits: integer
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   numberOfDigits - (integer) The number (1-18) of digits to generate.
 *
 * Returns:
 *
 *   (string)
 */
const afw_value_t *
afw_function_execute_random_digits(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: random_hexBinary
 *
 * afw_function_execute_random_hexBinary
 *
 * See afw_function_bindings.h for more information.
 *
 * This returns a specified number of random octets as dataType hexBinary.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function random_hexBinary(
 *       numberOfOctets: integer
 *   ): hexBinary;
 * ```
 *
 * Parameters:
 *
 *   numberOfOctets - (integer) The number of random octets to generate.
 *
 * Returns:
 *
 *   (hexBinary)
 */
const afw_value_t *
afw_function_execute_random_hexBinary(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: random_integer
 *
 * afw_function_execute_random_integer
 *
 * See afw_function_bindings.h for more information.
 *
 * This returns a random integer between specified values inclusive.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function random_integer(
 *       min: integer,
 *       max: integer
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   min - (integer) Minimum integer inclusive.
 *
 *   max - (integer) Maximum integer inclusive.
 *
 * Returns:
 *
 *   (integer) A random integer.
 */
const afw_value_t *
afw_function_execute_random_integer(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: random_number
 *
 * afw_function_execute_random_number
 *
 * See afw_function_bindings.h for more information.
 *
 * This returns a random double between specified values
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function random_number(
 *       min?: double,
 *       max?: double
 *   ): double;
 * ```
 *
 * Parameters:
 *
 *   min - (optional double) Minimum double inclusive. The default is 0.0.
 *
 *   max - (optional double) Maximum double exclusive. The default is 1.0.
 *
 * Returns:
 *
 *   (double)
 */
const afw_value_t *
afw_function_execute_random_number(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
