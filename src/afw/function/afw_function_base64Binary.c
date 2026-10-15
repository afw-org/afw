// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for base64Binary
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_base64Binary.c
 * @brief Adaptive function execute implementations for category `base64Binary`.
 */

#include "afw_internal.h"



/*
 * Adaptive function: decode_to_string<base64Binary>
 *
 * afw_function_execute_decode_to_string_base64Binary
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Decode base64Binary value to string. An error is thrown if decoded value is
 * not valid UTF-8.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function decode_to_string<base64Binary>(
 *       value: base64Binary
 *   ): string;
 * ```
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
    const afw_value_base64Binary_t *value;
    const afw_utf8_octet_t *s;
    afw_size_t len;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(value, 1, base64Binary);

    s = (const afw_utf8_octet_t *)value->internal.ptr;
    len = value->internal.size;
    if (!afw_utf8_is_valid(s, len, x->xctx))
    {
        AFW_THROW_ERROR_Z(conversion_error, "value does not decode to valid utf-8",
            x->xctx);
    }

    /* A copy in dest p, NFC and without U+FEFF, like every string. */
    return afw_value_create_unmanaged_string(
        afw_utf8_create(s, len, x->p, x->xctx), x->p, x->xctx);
}
