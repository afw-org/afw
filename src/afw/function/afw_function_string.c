// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for string
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_string.c
 * @brief Adaptive function execute implementations for category `string`.
 */

#include "afw_internal.h"



/*
 * add<string>(), string() and concat(): each argument as a string,
 * concatenated. string() and concat() are documented to convert, so they
 * convert here; the parameter evaluator never converts (#583). add<string>
 * declares its parameters string, so its arguments are already strings.
 * When first_evaluated, concat() has already evaluated argument 1 (to
 * first, NULL for undefined) to see whether it is an array.
 */
static const afw_value_t *
impl_concat_as_strings(
    afw_function_execute_t *x,
    afw_boolean_t first_evaluated,
    const afw_value_t *first)
{
    const afw_value_string_t **args;
    const afw_value_string_t * *v;
    const afw_value_t *value;
    afw_size_t n;
    afw_size_t len;
    afw_utf8_octet_t *s;
    afw_utf8_octet_t *c;

    /* Evaluate args and determine total len and allocate. */
    args = afw_pool_malloc(x->p, sizeof(afw_value_t *) * x->argc, x->xctx);
    for (v = args, n = 1, len = 0; n <= x->argc; v++, n++) {
        if (n == 1 && first_evaluated) {
            value = first;
        }
        else {
            AFW_FUNCTION_EVALUATE_PARAMETER(value, n);
        }
        *v = (value)
            ? (const afw_value_string_t *)afw_value_convert_to_string(
                value, true, x->p, x->xctx)
            : &afw_self_v_undefined;
        len += (*v)->internal.len;
    }

    /* If length is 0, return empty string. */
    if (len == 0) {
        return afw_v_a_empty_string;
    }

    /* Concatenate strings. */
    s = afw_pool_calloc(x->p, len, x->xctx);
    for (v = args, c = s, n = 0; n < x->argc;
        c += (*v)->internal.len, v++, n++)
    {
        if ((*v)->internal.len > 0) {
            memcpy(c, (*v)->internal.s, (*v)->internal.len);
        }
    }

    /* Return String value. */
    return afw_value_create_unmanaged_string(
        afw_utf8_create(s, len, x->p, x->xctx), x->p, x->xctx);
}



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
    return impl_concat_as_strings(x, false, NULL);
}



/*
 * Adaptive function: concat
 *
 * afw_function_execute_concat
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * concat follows its first value. If it is an array, return a new array of the
 * entries of every value in order; every value must be an array (write [...a,
 * v] or push(a, v) to add a single value). Otherwise convert every value of any
 * data type to string and return the concatenated result; a value with an
 * undefined value is represented by 'undefined'.
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
    const afw_value_t *first;
    const afw_value_t *value;
    const afw_array_t *result;
    const afw_value_t *entry;
    const afw_iterator_old_t *iterator;
    afw_size_t n;

    /*
     * concat follows its first argument (#599): an array concatenates
     * arrays; anything else converts every value to a string and joins.
     */
    AFW_FUNCTION_EVALUATE_PARAMETER(first, 1);
    if (!afw_value_is_array(first)) {
        return impl_concat_as_strings(x, true, first);
    }

    /*
     * Every argument must be an array: ECMAScript appends a non-array
     * argument, so whether a.concat(v) flattens would depend on what v
     * happens to hold.
     */
    /* New array: create_managed RC 1. Last-release of that hold on dest p. */
    result = ((const afw_value_array_t *)
        afw_pool_scope_release_value_at_cleanup(
            afw_array_create_managed(NULL, x->p, x->xctx)->value,
            x->p, x->xctx))->internal;
    for (n = 1, value = first; n <= x->argc; n++) {
        if (n > 1) {
            AFW_FUNCTION_EVALUATE_PARAMETER(value, n);
        }
        if (!afw_value_is_array(value)) {
            AFW_THROW_ERROR_FZ(argument_error, x->xctx,
                "concat of an array takes only arrays, but parameter "
                AFW_SIZE_T_FMT " is %ku; write [...a, v] or push(a, v)"
                " to add a value",
                n, value
                ? &afw_value_get_data_type(value, x->xctx)->data_type_id
                : afw_s_undefined);
        }
        for (iterator = NULL;;) {
            entry = afw_array_get_next_value(
                ((const afw_value_array_t *)value)->internal,
                &iterator, x->xctx);
            if (!entry) {
                break;
            }
            afw_array_push_value(result, entry, x->xctx);
        }
    }

    return result->value;
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
    const afw_value_string_t *arg1;
    const afw_value_string_t *arg2;
    int cmp;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg1, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg2, 2, string);

    cmp = afw_utf8_compare_ignore_case(
        &arg1->internal, &arg2->internal, x->xctx);

    return (cmp == 0) ? afw_boolean_v_true : afw_boolean_v_false;
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
    const afw_value_string_t *arg;
    const afw_utf8_t *s;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, 1, string);

    s = afw_utf8_normalize_space(&arg->internal, x->p, x->xctx);

    /* Return String value. */
    return afw_value_create_unmanaged_string(s, x->p, x->xctx);
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
    const afw_value_string_t *arg;
    const afw_utf8_t *s;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, 1, string);

    s = afw_utf8_to_lower(&arg->internal, x->p, x->xctx);
    
    /* Return String value. */
    return afw_value_create_unmanaged_string(s, x->p, x->xctx);
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
    return impl_concat_as_strings(x, false, NULL);
}



/*
 * Adaptive function: url_decode
 *
 * afw_function_execute_url_decode
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * URL decode a string: the inverse of url_encode. Percent escapes are decoded
 * as UTF-8, except an escape of ; / ? : @ & = + $ , # is kept as is, since
 * url_encode never encodes those. A malformed escape, or escapes that are not
 * valid UTF-8, are an error. The result is NFC, as every string is.
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
    const afw_value_string_t *encoded;
    const afw_utf8_t *s;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(encoded, 1, string);

    s = afw_uri_decode_uri(&encoded->internal, x->p, x->xctx);

    return afw_value_create_unmanaged_string(s, x->p, x->xctx);
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
    const afw_value_string_t *script;
    const afw_value_t *compiled;
    const afw_value_t *value;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(script, 1, string);

    /* compile dest is x->p (last-releases dest p). Evaluate dest is
     * x->p. Evaluate is caller does not release. */
    compiled = afw_compile_to_value(
        &script->internal, AFW_FUNCTION_SOURCE_LOCATION,
        afw_compile_type_script,
        NULL, x->p, x->xctx);

    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        value = afw_value_evaluate_with_additional_untrusted_qualified_variables(
            compiled, x->argv[2], x->p, x->xctx);
    }
    else {
        value = afw_value_evaluate(compiled, x->p, x->xctx);
    }

    afw_xctx_statement_flow_reset_all_except_rethrow(x->xctx);
    return value;
}



/*
 * Adaptive function: from_code_point
 *
 * afw_function_execute_from_code_point
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return a string of the code points passed, in order. A code point must be
 * from 0 to 0x10FFFF and not a surrogate (U+D800 to U+DFFF): a string can not
 * hold a lone surrogate. The result is NFC, like every string.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function from_code_point(
 *       ...codePoints: integer[]
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   codePoints - (0 or more integer) The code points.
 *
 * Returns:
 *
 *   (string) The string of those code points.
 *
 * Errors thrown:
 *
 *   argument_error - a code point is negative, above 0x10FFFF, or a surrogate
 */
const afw_value_t *
afw_function_execute_from_code_point(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *cp;
    afw_utf8_octet_t *octets;
    afw_utf8_octet_t one[5];
    afw_size_t len;
    afw_size_t n;
    afw_size_t one_len;

    /* At most 4 octets a code point; NFC after (afw_utf8_create). */
    octets = afw_pool_malloc(x->p, 4 * x->argc + 1, x->xctx);
    for (len = 0, n = 1; n <= x->argc; n++) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(cp, n, integer);
        if (cp->internal < 0 || cp->internal > 0x10FFFF ||
            (cp->internal >= 0xD800 && cp->internal <= 0xDFFF))
        {
            afw_xctx_evaluation_stack_push_parameter_number(n, x->xctx);
            AFW_THROW_ERROR_FZ(argument_error, x->xctx,
                "Parameter " AFW_SIZE_T_FMT " of function 'from_code_point'"
                " is " AFW_INTEGER_FMT ", which is not a code point a string"
                " can hold (0 to 0x10FFFF, not a surrogate)",
                n, cp->internal);
        }
        if (!afw_utf8_from_code_point(one, (afw_code_point_t)cp->internal,
            x->xctx))
        {
            AFW_THROW_ERROR_Z(general, "Invalid code point", x->xctx);
        }
        one_len = strlen(one);
        /* U+0000 encodes as one zero octet. */
        if (one_len == 0) {
            one_len = 1;
        }
        memcpy(octets + len, one, one_len);
        len += one_len;
    }

    return afw_value_create_unmanaged_string(
        afw_utf8_create(octets, len, x->p, x->xctx), x->p, x->xctx);
}
