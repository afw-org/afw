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

#include "afw.h"



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
 *   function decode_to_string<hexBinary>(
 *       value: hexBinary
 *   ): string;
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}
