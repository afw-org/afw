// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for string
 *
 * Copyright (c) 2010-2024 Clemson University
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
 * See afw_function_bindings_internal.h for more information.
 *
 * Add (concatenate) 2 or more string values and return the string result. Every
 * value must be a string, the same as the + operator: to convert values while
 * concatenating, use concat() or string().
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function add<string>(
 *       values_1: string,
 *       values_2: string,
 *       ...values_rest: string[]
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   values - (2 or more string)
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
 * See afw_function_bindings_internal.h for more information.
 *
 * concat follows its first value. If it is an array, return a new array of the
 * entries of every value in order, as ECMAScript array concat does, except that
 * every value must be an array (write [...a, v] or push(a, v) to add a single
 * value). Otherwise convert every value of any data type to string and return
 * the concatenated result; a value with an undefined value is represented by
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
 *       ...values_rest: any[]
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   values - (1 or more any) Arrays to concatenate, or values to convert to
 *       strings and concatenate.
 *
 * Returns:
 *
 *   (any) A new array if the first value is an array, otherwise the
 *       concatenated string.
 *
 * Errors thrown:
 *
 *   conversion_error - value could not be converted
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
 * See afw_function_bindings_internal.h for more information.
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
 * Adaptive function: eval<string>
 *
 * afw_function_execute_eval_string
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Compile and evaluate string value as adaptive script.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function eval<string>(
 *       source: string,
 *       additionalUntrustedQualifiedVariables?: object // _AdaptiveTemplatePropertiesObjects_
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   source - (string) string to compile and evaluate.
 *
 *   additionalUntrustedQualifiedVariables - (optional object
 *       _AdaptiveTemplatePropertiesObjects_) This parameter supplies additional
 *       qualified variables that can be accessed during evaluation. These
 *       variables will not be used by anything that needs to ensure its
 *       qualified variables must come from a trusted source, such as
 *       authorization. This parameter is intended to be used for testing only
 *       and should not be used for anything running in production.
 *
 * Returns:
 *
 *   (any)
 */
const afw_value_t *
afw_function_execute_eval_string(
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
 * See afw_function_bindings_internal.h for more information.
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
 * See afw_function_bindings_internal.h for more information.
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
 * See afw_function_bindings_internal.h for more information.
 *
 * Conversion function for string: convert one or more values of any data type
 * to string and return the concatenated result. With one argument this is
 * convert-to-string; with more arguments each is converted then concatenated in
 * order (no separator). A value with an undefined value is represented by
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
 *       ...values_rest: any[]
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   values - (1 or more any) Value to convert.
 *
 * Returns:
 *
 *   (string) The concatenated string values.
 *
 * Errors thrown:
 *
 *   conversion_error - value could not be converted
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
 * See afw_function_bindings_internal.h for more information.
 *
 * URL decode a string.
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
 *   encoded - (string) String to URL decode.
 *
 * Returns:
 *
 *   (string) The URL-decoded string.
 */
const afw_value_t *
afw_function_execute_url_decode(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
