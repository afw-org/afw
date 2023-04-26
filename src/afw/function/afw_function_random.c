// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for random
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_random.c
 * @brief afw_function_execute_* functions for random.
 */

#include "afw_internal.h"



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
    const afw_value_integer_t *numberOfOctets;
    afw_value_base64Binary_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(numberOfOctets,
        1, integer);

    if (numberOfOctets->internal < 0) {
        AFW_THROW_ERROR_Z(general, "numberOfOctets must be greater than 0",
            x->xctx);
    }


    result = afw_value_allocate_base64Binary(x->p, x->xctx);
    result->internal.size = (afw_size_t)numberOfOctets->internal;
    result->internal.ptr = afw_pool_malloc(x->p,
        result->internal.size, x->xctx);
    apr_generate_random_bytes((unsigned char *)result->internal.ptr,
        result->internal.size);

    return (const afw_value_t *)result;
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
    const afw_value_integer_t *numberOfDigits;
    afw_value_string_t *result;
    afw_uint64_t n;
    int count;
    char *c;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(numberOfDigits,
        1, integer);

    if (numberOfDigits->internal < 1 || numberOfDigits->internal > 18) {
        AFW_THROW_ERROR_Z(general, "numberOfDigits can be 1-18", x->xctx);
    }


    result = afw_value_allocate_string(x->p, x->xctx);
    result->internal.len = (afw_size_t)numberOfDigits->internal;
    result->internal.s = afw_pool_malloc(x->p, result->internal.len, x->xctx);
    apr_generate_random_bytes((unsigned char *)&n, sizeof(n));

    for (count = (int)numberOfDigits->internal,
        c = (char *)result->internal.s;
        count > 0;
        count--, c++)
    {
        *c = (n % 10) + '0';
        n /= 10;
    }

    return (const afw_value_t *)result;
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
    const afw_value_integer_t *numberOfOctets;
    afw_value_hexBinary_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(numberOfOctets,
        1, integer);

    if (numberOfOctets->internal < 0) {
        AFW_THROW_ERROR_Z(general, "numberOfOctets must be greater than 0",
            x->xctx);
    }


    result = afw_value_allocate_hexBinary(x->p, x->xctx);
    result->internal.size = (afw_size_t)numberOfOctets->internal;
    result->internal.ptr = afw_pool_malloc(x->p,
        result->internal.size, x->xctx);
    apr_generate_random_bytes((unsigned char *)result->internal.ptr,
        result->internal.size);

    return (const afw_value_t *)result;
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
    const afw_value_integer_t *min;
    const afw_value_integer_t *max;
    afw_integer_t range;
    afw_value_integer_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(min, 1, integer);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(max, 2, integer);

    range = max->internal - min->internal + 1;
    if (range <= 0) {
        AFW_THROW_ERROR_Z(general,
            "range between min and max is out of bounds",
            x->xctx);
    }

    result = afw_value_allocate_integer(x->p, x->xctx);
    apr_generate_random_bytes((unsigned char*)&result->internal,
        sizeof(afw_integer_t));
    if (result->internal < 0) {
        if (result->internal == AFW_INTEGER_MIN) {
            result->internal = 0;
        }
        else {
            result->internal = -result->internal;
        }
    }
    result->internal = min->internal + (result->internal % range); 

    return (const afw_value_t *)result;
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
    const afw_value_double_t *d;
    afw_double_t min;
    afw_double_t max;
    afw_double_t range;
    afw_integer_t random;
    afw_value_double_t *result;

    min = 0.0;
    max = 1.0;

    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(1)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(d, 1, double);
        min = d->internal;
    }

    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(d, 2, double);
        max = d->internal;
    }

    range = max - min;
    if (range <= 0.0) {
        AFW_THROW_ERROR_Z(general,
            "range between min and max is out of bounds",
            x->xctx);
    }

    apr_generate_random_bytes((unsigned char*)&random, sizeof(random));
    if (random < 0) {
        if (random == AFW_INTEGER_MIN) {
            random = 0;
        }
        else {
            random = -random;
        }
    }

    result = afw_value_allocate_double(x->p, x->xctx);
    result->internal = min + (random / ((afw_double_t)AFW_INTEGER_MAX / range));

    return (const afw_value_t *)result;
}
