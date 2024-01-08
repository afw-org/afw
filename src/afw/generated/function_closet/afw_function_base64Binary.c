// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for base64Binary
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_base64Binary.c
 * @brief afw_function_execute_* functions for base64Binary.
 */

#include "afw.h"



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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
