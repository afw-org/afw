// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for string
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_string.c
 * @brief afw_function_execute_* functions for string.
 */

#include "afw.h"



/*
 * Adaptive function: add<string>
 *
 * afw_function_execute_add_string
 *
 * See afw_function_bindings.h for more information.
 *
 * Add (concatenate) a string with 1 or more values of any data type converted
 * to their string value and return the string result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function add<string>(
 *       string: string,
 *       values_1: any,
 *       ...values_rest: (array of any)
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   string - (string)
 *
 *   values - (1 or more any dataType)
 *
 * Returns:
 *
 *   (string)
 */
const afw_value_t *
afw_function_execute_add_string(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: concat
 *
 * afw_function_execute_concat
 *
 * See afw_function_bindings.h for more information.
 *
 * Convert two or more values of any data type to string and return the
 * concatenated result. A value with an undefined value is represented by
 * 'undefined'.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function concat(
 *       values_1: any,
 *       ...values_rest: (array of any)
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   values - (1 or more any dataType) Value to convert.
 *
 * Returns:
 *
 *   (string) The concatenated string values.
 *
 * Errors thrown:
 *
 *   cast_error - value could not be converted
 */
const afw_value_t *
afw_function_execute_concat(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: eq_ignore_case<string>
 *
 * afw_function_execute_eq_ignore_case_string
 *
 * See afw_function_bindings.h for more information.
 *
 * Checks for string arg1 is equal to string arg2 ignoring case and return the
 * boolean result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function eq_ignore_case<string>(
 *       arg1: string,
 *       arg2: string
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   arg1 - (string)
 *
 *   arg2 - (string)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_eq_ignore_case_string(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: normalize_space<string>
 *
 * afw_function_execute_normalize_space_string
 *
 * See afw_function_bindings.h for more information.
 *
 * Remove whitespace from the beginning and end of a string value.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function normalize_space<string>(
 *       string: string
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   string - (string)
 *
 * Returns:
 *
 *   (string)
 */
const afw_value_t *
afw_function_execute_normalize_space_string(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: normalize_to_lower_case<string>
 *
 * afw_function_execute_normalize_to_lower_case_string
 *
 * See afw_function_bindings.h for more information.
 *
 * Normalize string value to lower case and returns string result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function normalize_to_lower_case<string>(
 *       string: string
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   string - (string)
 *
 * Returns:
 *
 *   (string)
 */
const afw_value_t *
afw_function_execute_normalize_to_lower_case_string(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: string
 *
 * afw_function_execute_string
 *
 * See afw_function_bindings.h for more information.
 *
 * Convert one or more values of any data type to string and return the
 * concatenated result. A value with an undefined value is represented by
 * 'undefined'.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function string(
 *       values_1: any,
 *       ...values_rest: (array of any)
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   values - (1 or more any dataType) Value to convert.
 *
 * Returns:
 *
 *   (string) The concatenated string values.
 *
 * Errors thrown:
 *
 *   cast_error - value could not be converted
 */
const afw_value_t *
afw_function_execute_string(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: url_decode
 *
 * afw_function_execute_url_decode
 *
 * See afw_function_bindings.h for more information.
 *
 * URL decode a value or bag of values.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function url_decode(
 *       encoded: string
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   encoded - (string) URL decode a single string or a bag of string.
 *
 * Returns:
 *
 *   (string) A string or bag of strings.
 */
const afw_value_t *
afw_function_execute_url_decode(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
