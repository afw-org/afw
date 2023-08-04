// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for hexBinary
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_hexBinary.c
 * @brief afw_function_execute_* functions for hexBinary.
 */

#include "afw_internal.h"



/*
 * Adaptive function: decode_to_string<hexBinary>
 *
 * afw_function_execute_decode_to_string_hexBinary
 *
 * See afw_function_bindings.h for more information.
 *
 * Decode hexBinary value to string. An error is thrown if decoded value is not
 * valid UTF-8.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function decode_to_string<hexBinary>(
 *       value: hexBinary
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   value - (hexBinary) The hexBinary value to decode.
 *
 * Returns:
 *
 *   (string) The decoded string.
 */
const afw_value_t *
afw_function_execute_decode_to_string_hexBinary(
    afw_function_execute_t *x)
{
    afw_value_string_t *result;
    const afw_value_hexBinary_t *value;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(value, 1, hexBinary);

    result = afw_value_allocate_string(x->p, x->xctx);
    result->internal.s = (const afw_utf8_octet_t *)value->internal.ptr;
    result->internal.len = value->internal.size;

    if (!afw_utf8_is_valid(result->internal.s, result->internal.len, x->xctx))
    {
        AFW_THROW_ERROR_Z(general, "value does not decode to valid utf-8",
            x->xctx);
    }

    return (const afw_value_t *)result;
}
