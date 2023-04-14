// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for base64Binary
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_base64Binary.c
 * @brief afw_function_execute_* functions for base64Binary.
 */

#include "afw_internal.h"



/*
 * Adaptive function: decode_to_string<base64Binary>
 *
 * afw_function_execute_decode_to_string_base64Binary
 *
 * See afw_function_bindings.h for more information.
 *
 * Decode base64Binary value to string. An error is thrown if decoded value is
 * not valid UTF-8.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 *   function decode_to_string<base64Binary>(
 *       value: base64Binary
 *   ): string;
 *
 * Parameters:
 *
 *   value - (base64Binary) The base64Binary value to decode.
 *
 * Returns:
 *
 *   (string) The decoded string.
 */
const afw_value_t *
afw_function_execute_decode_to_string_base64Binary(
    afw_function_execute_t *x)
{
    afw_value_string_t *result;
    const afw_value_base64Binary_t *value;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(value, 1, base64Binary);

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
