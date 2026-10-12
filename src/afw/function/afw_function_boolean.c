// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for boolean
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_boolean.c
 * @brief Adaptive function execute implementations for category `boolean`.
 */

#include "afw_internal.h"



/*
 * Adaptive function: string_to_boolean
 *
 * afw_function_execute_string_to_boolean
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Reads boolean text: "true" or "false" in any case, "1" or "0", or the single
 * letter "t", "T", "f", or "F". Any other text is an error. This is not
 * boolean(), which is ECMAScript Boolean() truthiness: boolean("false") is
 * true, and string_to_boolean("false") is false.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function string_to_boolean(
 *       value: string
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   value - (string) Boolean text to read.
 *
 * Returns:
 *
 *   (boolean) The boolean the text names.
 *
 * Errors thrown:
 *
 *   conversion_error - value is not valid boolean text
 */
const afw_value_t *
afw_function_execute_string_to_boolean(
    afw_function_execute_t *x)
{
    const afw_value_string_t *value;
    afw_boolean_t result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(value, 1, string);

    /* The boolean parser, not afw_value_convert(), which is truthiness. */
    afw_data_type_utf8_to_internal(afw_data_type_boolean,
        &result, &value->internal, x->p, x->xctx);

    return afw_value_for_boolean(result);
}
