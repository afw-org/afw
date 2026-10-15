// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for hexBinary
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_hexBinary.c
 * @brief Adaptive function execute implementations for category `hexBinary`.
 */

#include "afw_internal.h"



/*
 * Adaptive function: decode_to_string<hexBinary>
 *
 * afw_function_execute_decode_to_string_hexBinary
 *
 * See afw_function_bindings_internal.h for more information.
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
    const afw_value_hexBinary_t *value;
    const afw_utf8_octet_t *s;
    afw_size_t len;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(value, 1, hexBinary);

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
