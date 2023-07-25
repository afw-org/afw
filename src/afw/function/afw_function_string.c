// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for String
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_string.c
 * @brief afw_function_execute_* functions for string.
 */

#include "afw_internal.h"



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
 *       ...values_rest: (list of any)
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
    const afw_value_string_t **args;
    const afw_value_string_t * *v;
    afw_size_t n;
    afw_size_t len;
    afw_utf8_octet_t *s;
    afw_utf8_octet_t *c;

    /* Evaluate args and determine total len and allocate. */
    args = afw_pool_malloc(x->p, sizeof(afw_value_t *) * x->argc, x->xctx);
    for (v = args, n = 1, len = 0; n <= x->argc; v++, n++) {
        AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER((*v), n, string);
        if (!*v) {
            *v = (const afw_value_string_t *)afw_value_undefined_as_string;
        }
        len += (*v)->internal.len;
    }

    /* If length is 0, return empty string. */
    if (len == 0) {
        return afw_value_empty_string;
    }

    /* Concatenate strings. */
    s = afw_pool_calloc(x->p, len, x->xctx);
    for (v = args, c = s, n = 0; n < x->argc;
        c += (*v)->internal.len, v++, n++)
    {
        memcpy(c, (*v)->internal.s, (*v)->internal.len);
    }

    /* Return String value. */
    return afw_value_create_string(
        afw_utf8_create(s, len, x->p, x->xctx), x->p, x->xctx);
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
 * `<undefined>`.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function concat(
 *       values_1: any,
 *       ...values_rest: (list of any)
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
    const afw_value_string_t **args;
    const afw_value_string_t * *v;
    afw_size_t n;
    afw_size_t len;
    afw_utf8_octet_t *s;
    afw_utf8_octet_t *c;

    /* Evaluate args and determine total len and allocate. */
    args = afw_pool_malloc(x->p, sizeof(afw_value_t *) * x->argc, x->xctx);
    for (v = args, n = 1, len = 0; n <= x->argc; v++, n++) {
        AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER((*v), n, string);
        if (!*v) {
            *v = (const afw_value_string_t *)afw_value_undefined_as_string;
        }
        len += (*v)->internal.len;
    }

    /* If length is 0, return empty string. */
    if (len == 0) {
        return afw_value_empty_string;
    }

    /* Concatenate strings. */
    s = afw_pool_calloc(x->p, len, x->xctx);
    for (v = args, c = s, n = 0; n < x->argc;
        c += (*v)->internal.len, v++, n++)
    {
        memcpy(c, (*v)->internal.s, (*v)->internal.len);
    }

    /* Return String value. */
    return afw_value_create_string(
        afw_utf8_create(s, len, x->p, x->xctx), x->p, x->xctx);
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
    const afw_value_string_t *arg1;
    const afw_value_string_t *arg2;
    int cmp;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg1, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg2, 2, string);

    cmp = afw_utf8_compare_ignore_case(
        &arg1->internal, &arg2->internal, x->xctx);

    return (cmp == 0) ? afw_value_true : afw_value_false;
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
    const afw_value_string_t *arg;
    const afw_utf8_t *s;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, 1, string);

    s = afw_utf8_normalize_space(&arg->internal, x->p, x->xctx);

    /* Return String value. */
    return afw_value_create_string(s, x->p, x->xctx);
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
    const afw_value_string_t *arg;
    const afw_utf8_t *s;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, 1, string);

    s = afw_utf8_to_lower(&arg->internal, x->p, x->xctx);
    
    /* Return String value. */
    return afw_value_create_string(s, x->p, x->xctx);
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
 * `<undefined>`.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function string(
 *       values_1: any,
 *       ...values_rest: (list of any)
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
    const afw_value_string_t **args;
    const afw_value_string_t * *v;
    afw_size_t n;
    afw_size_t len;
    afw_utf8_octet_t *s;
    afw_utf8_octet_t *c;

    /* Evaluate args and determine total len and allocate. */
    args = afw_pool_malloc(x->p, sizeof(afw_value_t *) * x->argc, x->xctx);
    for (v = args, n = 1, len = 0; n <= x->argc; v++, n++) {
        AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER((*v), n, string);
        if (!*v) {
            *v = (const afw_value_string_t *)afw_value_undefined_as_string;
        }
        len += (*v)->internal.len;
    }

    /* If length is 0, return empty string. */
    if (len == 0) {
        return afw_value_empty_string;
    }

    /* Concatenate strings. */
    s = afw_pool_calloc(x->p, len, x->xctx);
    for (v = args, c = s, n = 0; n < x->argc;
        c += (*v)->internal.len, v++, n++)
    {
        memcpy(c, (*v)->internal.s, (*v)->internal.len);
    }

    /* Return String value. */
    return afw_value_create_string(
        afw_utf8_create(s, len, x->p, x->xctx), x->p, x->xctx);
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
    /** @todo should this be here or in anyURI?. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
