// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for compiler
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_compiler.c
 * @brief afw_function_execute_* functions for compiler.
 */

#include "afw_internal.h"





/*
 * Adaptive function: assert
 *
 * afw_function_execute_assert
 *
 * See afw_function_bindings.h for more information.
 *
 * Assert that a value is true. If not, an assertion_failed error is thrown.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function assert(
 *       assertion: boolean,
 *       reason?: string
 *   ): null;
 * ```
 *
 * Parameters:
 *
 *   assertion - (boolean) This is the assertion to make. If not true, an
 *       assertion_failed error is thrown.
 *
 *   reason - (optional string) This is an optional reason to include in the
 *       assertion_failed message.
 *
 * Returns:
 *
 *   (null)
 */
const afw_value_t *
afw_function_execute_assert(
    afw_function_execute_t *x)
{
    const afw_value_t *assertion;
    const afw_value_string_t *reason;

    AFW_FUNCTION_EVALUATE_PARAMETER(assertion, 1);

    if (!assertion ||
        !afw_value_is_boolean(assertion) ||
        !((const afw_value_boolean_t *)assertion)->internal)
    {
        AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(reason, 2, string);
        if (reason) {
            AFW_THROW_ERROR_FZ(assertion_failed, x->xctx,
                "Assertion failed: " AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(&reason->internal));
        }
        else {
            AFW_THROW_ERROR_Z(assertion_failed, "Assertion failed", x->xctx);
        }
    }

    return afw_value_null;
}



AFW_DEFINE(const afw_utf8_t *)
afw_function_evaluate_whitespace_parameter(
    afw_function_execute_t *x,
    afw_size_t n)
{
    const afw_value_t *value;
    const afw_utf8_t *result;
    afw_size_t len;

    result = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(n)) {
        AFW_FUNCTION_EVALUATE_PARAMETER(value, n);
        if (afw_value_is_string(value) &&
            ((const afw_value_string_t *)value)->internal.len < 10)
        {
            result = &((const afw_value_string_t *)value)->internal;
        }
        else if (afw_value_is_integer(value) &&
            ((const afw_value_integer_t *)value)->internal >= 0 &&
            ((const afw_value_integer_t *)value)->internal <= 10)
        {
            len = (afw_size_t)((const afw_value_integer_t *)value)->
                internal;
            if (len != 0) {
                result = afw_utf8_create("        ", len, x->p, x->xctx);
            }
        }
        else if (afw_value_is_boolean(value)) {
            if (((const afw_value_boolean_t *)value)->internal) {
                result = &afw_s_a_tab;
            }
        }
        else {
            AFW_THROW_ERROR_FZ(general, x->xctx,
                "parameter " AFW_SIZE_T_FMT
                " must be a string no longer than 10 or an "
                "integer between 0 and 10",
                n);
        }
    }

    return result;
}



/*
 * Adaptive function: compile_expression_tuple
 *
 * afw_function_execute_compile_expression_tuple
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile a string containing adaptive expression tuple syntax and return
 * either an unevaluated expression tuple adaptive value or a string containing
 * the compiler listing.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function compile_expression_tuple(
 *       expression_tuple: array,
 *       listing?: any
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   expression_tuple - (array) expression tuple to compile.
 *
 *   listing - (optional any dataType) If specified, a compiler listing is
 *       produced instead of an unevaluated expression tuple value.
 *       
 *       This parameter can be an integer between 0 and 10 of a string that is
 *       used for indentation. If 0 is specified, no whitespace is added to the
 *       resulting string. If 1 through 10 is specified, that number of spaces
 *       is used.
 *
 * Returns:
 *
 *   (any dataType) An unevaluated expression tuple value ready for use by
 *       function evaluate() or a string containing the compiler listing.
 */
const afw_value_t *
afw_function_execute_compile_expression_tuple(
    afw_function_execute_t *x)
{
    const afw_value_array_t *expression_tuple;
    const afw_utf8_t *s;
    const afw_value_t *result;
    const afw_utf8_t *listing;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(expression_tuple,
        1, array);

    s = afw_json_from_value((const afw_value_t *)expression_tuple, NULL,
        x->p, x->xctx);

    result = afw_compile_to_value(
        s, AFW_FUNCTION_SOURCE_LOCATION, afw_compile_type_hybrid,
        NULL, NULL, x->p, x->xctx);

    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        listing = afw_function_evaluate_whitespace_parameter(x, 2);
        result = afw_value_create_string(
            afw_value_compiler_listing_to_string(result, listing,
                x->p, x->xctx),
            x->p, x->xctx);
    }

    return result;
}



/*
 * Adaptive function: compile_json
 *
 * afw_function_execute_compile_json
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile a string containing adaptive JSON syntax and return either an
 * unevaluated JSON adaptive value or a string containing the compiler listing.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function compile_json(
 *       json: string,
 *       listing?: any
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   json - (string) JSON string to compile.
 *
 *   listing - (optional any dataType) If specified, a compiler listing is
 *       produced instead of an unevaluated JSON value.
 *       
 *       This parameter can be an integer between 0 and 10 of a string that is
 *       used for indentation. If 0 is specified, no whitespace is added to the
 *       resulting string. If 1 through 10 is specified, that number of spaces
 *       is used.
 *
 * Returns:
 *
 *   (any dataType) An unevaluated JSON value ready for use by function
 *       evaluate() or a string containing the compiler listing.
 */
const afw_value_t *
afw_function_execute_compile_json(
    afw_function_execute_t *x)
{
    const afw_value_string_t *json;
    const afw_value_t *result;
    const afw_utf8_t *listing;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(json, 1, string);

    result = afw_compile_to_value(
        &json->internal, AFW_FUNCTION_SOURCE_LOCATION,
        afw_compile_type_json,
        NULL, NULL, x->p, x->xctx);

    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        listing = afw_function_evaluate_whitespace_parameter(x, 2);
        result = afw_value_create_string(
            afw_value_compiler_listing_to_string(result, listing,
                x->p, x->xctx),
            x->p, x->xctx);
    }

    return result;
}



/*
 * Adaptive function: compile_relaxed_json
 *
 * afw_function_execute_compile_relaxed_json
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile an adaptive relaxed JSON syntax string and return an adaptive value.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function compile_relaxed_json(
 *       json: string,
 *       listing?: any
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   json - (string) Adaptive relaxed JSON syntax string to compile.
 *
 *   listing - (optional any dataType) If specified, a compiler listing is
 *       produced instead of an unevaluated relaxed JSON value.
 *       
 *       This parameter can be an integer between 0 and 10 of a string that is
 *       used for indentation. If 0 is specified, no whitespace is added to the
 *       resulting string. If 1 through 10 is specified, that number of spaces
 *       is used.
 *
 * Returns:
 *
 *   (any dataType) An unevaluated relaxed JSON value ready for use by function
 *       evaluate() or a string containing the compiler listing.
 */
const afw_value_t *
afw_function_execute_compile_relaxed_json(
    afw_function_execute_t *x)
{
    const afw_value_string_t *json;
    const afw_value_t *result;
    const afw_utf8_t *listing;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(json, 1, string);

    result = afw_compile_to_value(
        &json->internal, AFW_FUNCTION_SOURCE_LOCATION,
        afw_compile_type_relaxed_json,
        NULL, NULL, x->p, x->xctx);

    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        listing = afw_function_evaluate_whitespace_parameter(x, 2);
        result = afw_value_create_string(
            afw_value_compiler_listing_to_string(result, listing,
                x->p, x->xctx),
            x->p, x->xctx);
    }

    return result;
}



/*
 * Adaptive function: convert_syntax_hybrid_to_expression
 *
 * afw_function_execute_convert_syntax_hybrid_to_expression
 *
 * See afw_function_bindings.h for more information.
 *
 * Convert a string containing adaptive hybrid syntax, which can be an adaptive
 * template or adaptive expression tuple, to adaptive expression syntax.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function convert_syntax_hybrid_to_expression(
 *       hybrid: string,
 *       whitespace?: any
 *   ): expression;
 * ```
 *
 * Parameters:
 *
 *   hybrid - (string) The hybrid to convert.
 *
 *   whitespace - (optional any dataType) Add whitespace for readability if
 *       present and not 0. This parameter can be an integer between 0 and 10
 *       or a string that is used for indentation. If 0 is specified, no
 *       whitespace is added to the resulting string. If 1 through 10 is
 *       specified, that number of spaces is used.
 *
 * Returns:
 *
 *   (expression) The converted value in adaptive expression syntax.
 */
const afw_value_t *
afw_function_execute_convert_syntax_hybrid_to_expression(
    afw_function_execute_t *x)
{
    const afw_utf8_t *s;
    const afw_value_t *value;
    const afw_value_string_t *hybrid;
    const afw_utf8_t *whitespace;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(hybrid, 1, string);
    whitespace = afw_function_evaluate_whitespace_parameter(x, 2);

    value = afw_compile_to_value(
        &hybrid->internal, AFW_FUNCTION_SOURCE_LOCATION,
        afw_compile_type_hybrid,
        NULL, NULL, x->p, x->xctx);

    s = afw_value_decompile_to_string(value, whitespace, x->p, x->xctx);

    return afw_value_create_string(s, x->p, x->xctx);
}



/*
 * Adaptive function: decompile
 *
 * afw_function_execute_decompile
 *
 * See afw_function_bindings.h for more information.
 *
 * Decompile an adaptive value to string.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function decompile(
 *       value: any,
 *       whitespace?: any
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   value - (any dataType) Value to decompile.
 *
 *   whitespace - (optional any dataType) Add whitespace for readability if
 *       present and not 0. This parameter can be an integer between 0 and 10
 *       or a string that is used for indentation. If 0 is specified, no
 *       whitespace is added to the resulting string. If 1 through 10 is
 *       specified, that number of spaces is used.
 *
 * Returns:
 *
 *   (string) Decompiled value.
 */
const afw_value_t *
afw_function_execute_decompile(
    afw_function_execute_t *x)
{
    const afw_utf8_t *whitespace;
    const afw_utf8_t *s;

    whitespace = afw_function_evaluate_whitespace_parameter(x, 2);
    s = afw_value_decompile_to_string(x->argv[1], whitespace, x->p, x->xctx);

    return afw_value_create_string(s, x->p, x->xctx);
}



/*
 * Adaptive function: evaluate_expression_tuple
 *
 * afw_function_execute_evaluate_expression_tuple
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile a string containing adaptive expression tuple syntax and then
 * evaluate the result.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function evaluate_expression_tuple(
 *       expression_tuple: string,
 *       additionalUntrustedQualifiedVariables?: (object _AdaptiveHybridPropertiesObjects_)
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   expression_tuple - (string) Expression tuple to compile and evaluate.
 *
 *   additionalUntrustedQualifiedVariables - (optional object
 *       _AdaptiveHybridPropertiesObjects_) This parameter supplies additional
 *       qualified variables that can be accessed during evaluation. These
 *       variables will not be used by anything that needs to ensure its
 *       qualified variables must come from a trusted source, such as
 *       authorization. This parameter is intended to be used for testing only
 *       and should not be used for anything running in production.
 *
 * Returns:
 *
 *   (any dataType) Evaluated adaptive expression tuple.
 */
const afw_value_t *
afw_function_execute_evaluate_expression_tuple(
    afw_function_execute_t *x)
{
    const afw_value_string_t *arg;
    const afw_value_t *compiled;
    const afw_value_t *value;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, 1, string);

    compiled = afw_compile_to_value(
        &arg->internal, AFW_FUNCTION_SOURCE_LOCATION,
        afw_compile_type_expression_tuple,
        NULL, NULL, x->p, x->xctx);

    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        value = afw_value_evaluate_with_additional_untrusted_qualified_variables(
            compiled, x->argv[2], x->p, x->xctx);
    }
    else {
        value = afw_value_evaluate(compiled, x->p, x->xctx);
    }

    return value;
}



/*
 * Adaptive function: evaluate_value
 *
 * afw_function_execute_evaluate_value
 *
 * See afw_function_bindings.h for more information.
 *
 * Evaluate an adaptive value.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function evaluate_value(
 *       value: any,
 *       additionalUntrustedQualifiedVariables?: (object _AdaptiveHybridPropertiesObjects_)
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   value - (any dataType)
 *
 *   additionalUntrustedQualifiedVariables - (optional object
 *       _AdaptiveHybridPropertiesObjects_) This parameter supplies additional
 *       qualified variables that can be accessed during evaluation. These
 *       variables will not be used by anything that needs to ensure its
 *       qualified variables must come from a trusted source, such as
 *       authorization. This parameter is intended to be used for testing only
 *       and should not be used for anything running in production.
 *
 * Returns:
 *
 *   (any dataType) Evaluated adaptive value.
 */
const afw_value_t *
afw_function_execute_evaluate_value(
    afw_function_execute_t *x)
{
    const afw_value_t *value;

    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        value =
            afw_value_evaluate_with_additional_untrusted_qualified_variables(
                x->argv[1], x->argv[2], x->p, x->xctx);
    }
    else {
        value = afw_value_evaluate(x->argv[0], x->p, x->xctx);
    }

    return value;
}



/*
 * Adaptive function: evaluate_with_retry
 *
 * afw_function_execute_evaluate_with_retry
 *
 * See afw_function_bindings.h for more information.
 *
 * Evaluate a value and retry up to a limit if an exception occurs.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function evaluate_with_retry(
 *       value: any,
 *       limit: integer
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   value - (any dataType) Value to evaluated.
 *
 *   limit - (integer) Maximum number to retry if an exception occurs.
 *
 * Returns:
 *
 *   (any dataType) Evaluated value.
 */
const afw_value_t *
afw_function_execute_evaluate_with_retry(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_value_t *value;
    const afw_value_integer_t *limit;
    afw_integer_t failures;
    afw_boolean_t success;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(limit, 2, integer);

    value = NULL;
    failures = 0;

    /* Try to evaluate value up to limit. */
    for (success = false; !success;) {

        AFW_TRY {
            AFW_FUNCTION_EVALUATE_PARAMETER(value, 1);
            success = true;
        }

        AFW_CATCH_UNHANDLED {
            failures++;
            if (failures > limit->internal) {
                AFW_ERROR_RETHROW;
            }
        }

        AFW_ENDTRY;
    }

    if (!value) {
        AFW_THROW_ERROR_Z(general,
            "Evaluation produced an undefined result", xctx);
    }

    return value;
}



/*
 * Adaptive function: safe_evaluate
 *
 * afw_function_execute_safe_evaluate
 *
 * See afw_function_bindings.h for more information.
 *
 * Return the evaluated adaptive value. If an exception occurs, return
 * evaluated error instead.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function safe_evaluate(
 *       value: any,
 *       error: any
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   value - (any dataType) Value to evaluated.
 *
 *   error - (any dataType) Value to evaluate and return if exception occurs.
 *       If an error occurs evaluating this value, the exception will continue.
 *
 * Returns:
 *
 *   (any dataType) Evaluated adaptive value or error value.
 */
const afw_value_t *
afw_function_execute_safe_evaluate(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_value_t *value;

    value = NULL;
    AFW_TRY {
        AFW_FUNCTION_EVALUATE_PARAMETER(value, 1);
    }

    AFW_CATCH_UNHANDLED{
        AFW_FUNCTION_EVALUATE_PARAMETER(value, 2);
    }

    AFW_ENDTRY;

    if (!value) {
        AFW_THROW_ERROR_Z(general,
            "Evaluation produced an undefined result", xctx);
    }
    return value;
}



/*
 * Adaptive function: stringify
 *
 * afw_function_execute_stringify
 *
 * See afw_function_bindings.h for more information.
 *
 * Evaluate and decompile an adaptive value to string. For most values this has
 * the effect of producing a string containing json.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function stringify(
 *       value: any,
 *       replacer?: any,
 *       whitespace?: any
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   value - (any dataType) Value to stringify.
 *
 *   replacer - (optional any dataType) Optional replacer function.
 *
 *   whitespace - (optional any dataType) Add whitespace for readability if
 *       present and not 0. This parameter can be an integer between 0 and 10
 *       or a string that is used for indentation. If 0 is specified, no
 *       whitespace is added to the resulting string. If 1 through 10 is
 *       specified, that number of spaces is used.
 *
 * Returns:
 *
 *   (string) Evaluated and decompiled value.
 */
const afw_value_t *
afw_function_execute_stringify(
    afw_function_execute_t *x)
{

    const afw_utf8_t *whitespace;
    const afw_utf8_t *s;
    const afw_value_t *value;
    const afw_value_t *replacer;

    AFW_FUNCTION_EVALUATE_PARAMETER(value, 1);

    if (!value) {
        return afw_value_empty_string;
    }

    /** @fixme add support for replacer */
    AFW_FUNCTION_EVALUATE_PARAMETER(replacer, 2);
    if (!afw_value_is_nullish(replacer)) {
        AFW_THROW_ERROR_Z(general,
            "replacer parameter is not implemented yet",
            x->xctx);
    }

    whitespace = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        whitespace = afw_function_evaluate_whitespace_parameter(x, 3);
    }

    s = afw_value_decompile_to_string(value, whitespace, x->p, x->xctx);

    return afw_value_create_string(s, x->p, x->xctx);
}



/*
 * Adaptive function: test_expression
 *
 * afw_function_execute_test_expression
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile and evaluate an adaptive expression and compare the results to an
 * expected value. Return object with the test's results.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function test_expression(
 *       id: string,
 *       description: string,
 *       expression: string,
 *       expected: any,
 *       additionalUntrustedQualifiedVariables?: (object _AdaptiveHybridPropertiesObjects_)
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   id - (string) Id of test.
 *
 *   description - (string) Description of test.
 *
 *   expression - (string) Expression to compile and evaluate.
 *
 *   expected - (any dataType) Expected result.
 *
 *   additionalUntrustedQualifiedVariables - (optional object
 *       _AdaptiveHybridPropertiesObjects_) This parameter supplies additional
 *       qualified variables that can be accessed during evaluation. These
 *       variables will not be used by anything that needs to ensure its
 *       qualified variables must come from a trusted source, such as
 *       authorization. This parameter is intended to be used for testing only
 *       and should not be used for anything running in production.
 *
 * Returns:
 *
 *   (object) Test results.
 */
const afw_value_t *
afw_function_execute_test_expression(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_object_t *result;
    const afw_value_string_t *id;
    const afw_value_string_t *description;
    const afw_value_string_t *expression;
    const afw_value_t *expected;
    const afw_value_t *compiled;
    const afw_value_t *evaluated;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(id, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(description, 2, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(expression, 3, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(expected, 4);

    result = afw_object_create_managed(x->p, x->xctx);
    afw_object_set_property(result, &afw_s_passed, afw_value_true, x->xctx);
    afw_object_set_property_as_string(result,
        &afw_s_id, &id->internal, x->xctx);
    afw_object_set_property_as_string(result,
        &afw_s_description, &description->internal, xctx);
    afw_object_set_property_as_string(result,
        &afw_s_expression, &expression->internal, xctx);
    afw_object_set_property_as_string(result,
        &afw_s_expected, 
        afw_value_as_casted_utf8(expected, x->p, x->xctx),
        xctx);

    AFW_TRY {

        compiled = afw_compile_to_value(
            &expression->internal, AFW_FUNCTION_SOURCE_LOCATION,
            afw_compile_type_expression,
            NULL, NULL, x->p, x->xctx);

        if (AFW_FUNCTION_PARAMETER_IS_PRESENT(5)) {
            evaluated = afw_value_evaluate_with_additional_untrusted_qualified_variables(
                compiled, x->argv[5], x->p, x->xctx);
        }
        else {
            evaluated = afw_value_evaluate(compiled, x->p, x->xctx);
        }

        afw_object_set_property_as_string(result, &afw_s_result,
            afw_value_as_casted_utf8(evaluated, x->p, x->xctx),
            xctx);
        
        if (!afw_value_equal(evaluated, expected, xctx)) {
            afw_object_set_property(result, &afw_s_passed, afw_value_false,
                xctx);
        }
    }

    AFW_CATCH_UNHANDLED{

        /* If 'error' is not expected value, set passed false. */
        if (!afw_value_is_string(expected) ||
            !afw_utf8_equal(
                &((const afw_value_string_t *)expected)->internal,
                &afw_s_error))
        {
            afw_object_set_property(result, &afw_s_passed, afw_value_false,
                xctx);
        }

        /* Set error property. */
        afw_object_set_property_as_object(result, &afw_s_error,
            afw_error_to_object(AFW_ERROR_THROWN, x->p, x->xctx), x->xctx);
    }

    AFW_ENDTRY;

    return afw_value_create_object(result, x->p, x->xctx);
}



/*
 * Adaptive function: test_expression_tuple
 *
 * afw_function_execute_test_expression_tuple
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile and evaluate an adaptive expression tuple and compare the results to
 * an expected value. Return object with the test's results.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function test_expression_tuple(
 *       id: string,
 *       description: string,
 *       expression: string,
 *       expected: any,
 *       additionalUntrustedQualifiedVariables?: (object _AdaptiveHybridPropertiesObjects_)
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   id - (string) Id of test.
 *
 *   description - (string) Description of test.
 *
 *   expression - (string) Expression tuple to compile and evaluate.
 *
 *   expected - (any dataType) Expected result.
 *
 *   additionalUntrustedQualifiedVariables - (optional object
 *       _AdaptiveHybridPropertiesObjects_) This parameter supplies additional
 *       qualified variables that can be accessed during evaluation. These
 *       variables will not be used by anything that needs to ensure its
 *       qualified variables must come from a trusted source, such as
 *       authorization. This parameter is intended to be used for testing only
 *       and should not be used for anything running in production.
 *
 * Returns:
 *
 *   (object) Test results.
 */
const afw_value_t *
afw_function_execute_test_expression_tuple(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_object_t *result;
    const afw_value_string_t *id;
    const afw_value_string_t *description;
    const afw_value_string_t *expression;
    const afw_value_t *expected;
    const afw_value_t *compiled;
    const afw_value_t *evaluated;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(id, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(description, 2, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(expression, 3, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(expected, 4);

    result = afw_object_create_managed(x->p, xctx);
    afw_object_set_property(result, &afw_s_passed, afw_value_true, xctx);
    afw_object_set_property_as_string(result,
        &afw_s_id, &id->internal, xctx);
    afw_object_set_property_as_string(result,
        &afw_s_description, &description->internal, xctx);
    afw_object_set_property_as_string(result,
        &afw_s_expression, &expression->internal, xctx);
    afw_object_set_property_as_string(result,
        &afw_s_expected,
        afw_value_as_casted_utf8(expected, x->p, xctx),
        xctx);

    AFW_TRY{

        compiled = afw_compile_to_value(
            &expression->internal, AFW_FUNCTION_SOURCE_LOCATION,
            afw_compile_type_expression_tuple,
            NULL, NULL, x->p, xctx);

        if (AFW_FUNCTION_PARAMETER_IS_PRESENT(5)) {
            evaluated = afw_value_evaluate_with_additional_untrusted_qualified_variables(
                compiled, x->argv[5], x->p, xctx);
        }
        else {
            evaluated = afw_value_evaluate(compiled, x->p, xctx);
        }

        afw_object_set_property_as_string(result, &afw_s_result,
            afw_value_as_casted_utf8(evaluated, x->p, xctx),
            xctx);

        if (!afw_value_equal(evaluated, expected, xctx)) {
            afw_object_set_property(result, &afw_s_passed, afw_value_false,
                xctx);
        }
    }

        AFW_CATCH_UNHANDLED{

            /* If 'error' is not expected value, set passed false. */
            if (!afw_value_is_string(expected) ||
                !afw_utf8_equal(
                    &((const afw_value_string_t *)expected)->internal,
                    &afw_s_error))
            {
                afw_object_set_property(result, &afw_s_passed, afw_value_false,
                    xctx);
            }

            /* Set error property. */
            afw_object_set_property_as_object(result, &afw_s_error,
                afw_error_to_object(AFW_ERROR_THROWN, x->p, xctx), xctx);
    }

    AFW_ENDTRY;

    return afw_value_create_object(result, x->p, xctx);
}



/*
 * Adaptive function: test_hybrid
 *
 * afw_function_execute_test_hybrid
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile and evaluate a string containing adaptive hybrid syntax which can be
 * an adaptive template or adaptive expression tuple and then compare the
 * results to an expected value. Return object with the test's results.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function test_hybrid(
 *       id: string,
 *       description: string,
 *       hybrid: string,
 *       expected: any,
 *       additionalUntrustedQualifiedVariables?: (object _AdaptiveHybridPropertiesObjects_)
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   id - (string) Id of test.
 *
 *   description - (string) Description of test.
 *
 *   hybrid - (string) Hybrid to compile and evaluate.
 *
 *   expected - (any dataType) Expected evaluated result.
 *
 *   additionalUntrustedQualifiedVariables - (optional object
 *       _AdaptiveHybridPropertiesObjects_) This parameter supplies additional
 *       qualified variables that can be accessed during evaluation. These
 *       variables will not be used by anything that needs to ensure its
 *       qualified variables must come from a trusted source, such as
 *       authorization. This parameter is intended to be used for testing only
 *       and should not be used for anything running in production.
 *
 * Returns:
 *
 *   (object) Test results.
 */
const afw_value_t *
afw_function_execute_test_hybrid(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_object_t *result;
    const afw_value_string_t *id;
    const afw_value_string_t *description;
    const afw_value_string_t *hybrid;
    const afw_value_t *expected;
    const afw_value_t *compiled;
    const afw_value_t *evaluated;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(id, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(description, 2, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(hybrid, 3, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(expected, 4);

    result = afw_object_create_managed(x->p, xctx);
    afw_object_set_property(result, &afw_s_passed, afw_value_true, xctx);
    afw_object_set_property_as_string(result,
        &afw_s_id, &id->internal, xctx);
    afw_object_set_property_as_string(result,
        &afw_s_description, &description->internal, xctx);
    afw_object_set_property_as_string(result,
        &afw_s_hybrid, &hybrid->internal, xctx);
    afw_object_set_property_as_string(result,
        &afw_s_expected, 
        afw_value_as_casted_utf8(expected, x->p, xctx),
        xctx);

    AFW_TRY {

        compiled = afw_compile_to_value(
            &hybrid->internal, AFW_FUNCTION_SOURCE_LOCATION,
            afw_compile_type_hybrid,
            NULL, NULL, x->p, xctx);

        if (AFW_FUNCTION_PARAMETER_IS_PRESENT(5)) {
            evaluated = afw_value_evaluate_with_additional_untrusted_qualified_variables(
                compiled, x->argv[5], x->p, xctx);
        }
        else {
            evaluated = afw_value_evaluate(compiled, x->p, xctx);
        }

        afw_object_set_property_as_string(result, &afw_s_result,
            afw_value_as_casted_utf8(evaluated, x->p, x->xctx),
            xctx);
        
        if (!afw_value_equal(evaluated, expected, xctx)) {
            afw_object_set_property(result, &afw_s_passed, afw_value_false,
                xctx);
        }
    }

    AFW_CATCH_UNHANDLED{

        /* If 'error' is not expected value, set passed false. */
        if (!afw_value_is_string(expected) ||
            !afw_utf8_equal(
                &((const afw_value_string_t *)expected)->internal,
                &afw_s_error))
        {
            afw_object_set_property(result, &afw_s_passed, afw_value_false,
                xctx);
        }

        /* Set error property. */
        afw_object_set_property_as_object(result, &afw_s_error,
            afw_error_to_object(AFW_ERROR_THROWN, x->p, xctx), xctx);
    }

    AFW_ENDTRY;

    return afw_value_create_object(result, x->p, xctx);
}



/*
 * Adaptive function: test_script
 *
 * afw_function_execute_test_script
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile and evaluate an adaptive script and compare the results to an
 * expected value. Return object with the test's results.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function test_script(
 *       id: string,
 *       description: string,
 *       script: string,
 *       expected: any,
 *       additionalUntrustedQualifiedVariables?: (object _AdaptiveHybridPropertiesObjects_)
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   id - (string) Id of test.
 *
 *   description - (string) Description of test.
 *
 *   script - (string) Script to compile and evaluate.
 *
 *   expected - (any dataType) Expected result.
 *
 *   additionalUntrustedQualifiedVariables - (optional object
 *       _AdaptiveHybridPropertiesObjects_) This parameter supplies additional
 *       qualified variables that can be accessed during evaluation. These
 *       variables will not be used by anything that needs to ensure its
 *       qualified variables must come from a trusted source, such as
 *       authorization. This parameter is intended to be used for testing only
 *       and should not be used for anything running in production.
 *
 * Returns:
 *
 *   (object) Test results.
 */
const afw_value_t *
afw_function_execute_test_script(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_object_t *result;
    const afw_value_string_t *id;
    const afw_value_string_t *description;
    const afw_value_string_t *expression;
    const afw_value_t *expected;
    const afw_value_t *compiled;
    const afw_value_t *evaluated;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(id, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(description, 2, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(expression, 3, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(expected, 4);

    result = afw_object_create_managed(x->p, xctx);
    afw_object_set_property(result, &afw_s_passed, afw_value_true, xctx);
    afw_object_set_property_as_string(result,
        &afw_s_id, &id->internal, xctx);
    afw_object_set_property_as_string(result,
        &afw_s_description, &description->internal, xctx);
    afw_object_set_property_as_string(result,
        &afw_s_expression, &expression->internal, xctx);
    afw_object_set_property_as_string(result,
        &afw_s_expected,
        afw_value_as_casted_utf8(expected, x->p, xctx),
        xctx);

    AFW_TRY{

        compiled = afw_compile_to_value(
            &expression->internal, AFW_FUNCTION_SOURCE_LOCATION,
            afw_compile_type_script,
            NULL, NULL, x->p, xctx);

        if (AFW_FUNCTION_PARAMETER_IS_PRESENT(5)) {
            evaluated = afw_value_evaluate_with_additional_untrusted_qualified_variables(
                compiled, x->argv[5], x->p, xctx);
        }
        else {
            evaluated = afw_value_evaluate(compiled, x->p, xctx);
        }

        afw_object_set_property_as_string(result, &afw_s_result,
            afw_value_as_casted_utf8(evaluated, x->p, xctx),
            xctx);

        if (!afw_value_equal(evaluated, expected, xctx)) {
            afw_object_set_property(result, &afw_s_passed, afw_value_false,
                xctx);
        }
    }

        AFW_CATCH_UNHANDLED{

            /* If 'error' is not expected value, set passed false. */
            if (!afw_value_is_string(expected) ||
                !afw_utf8_equal(
                    &((const afw_value_string_t *)expected)->internal,
                    &afw_s_error))
            {
                afw_object_set_property(result, &afw_s_passed, afw_value_false,
                    xctx);
            }

            /* Set error property. */
            afw_object_set_property_as_object(result, &afw_s_error,
                afw_error_to_object(AFW_ERROR_THROWN, x->p, xctx), xctx);
    }

    AFW_ENDTRY;

    return afw_value_create_object(result, x->p, xctx);
}



/*
 * Adaptive function: test_template
 *
 * afw_function_execute_test_template
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile and evaluate an adaptive template and compare the results to an
 * expected value. Return object with the test's results.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function test_template(
 *       id: string,
 *       description: string,
 *       template: string,
 *       expected: any,
 *       additionalUntrustedQualifiedVariables?: (object _AdaptiveHybridPropertiesObjects_)
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   id - (string) Id of test.
 *
 *   description - (string) Description of test.
 *
 *   template - (string) Template to compile and evaluate.
 *
 *   expected - (any dataType) Expected evaluated result.
 *
 *   additionalUntrustedQualifiedVariables - (optional object
 *       _AdaptiveHybridPropertiesObjects_) This parameter supplies additional
 *       qualified variables that can be accessed during evaluation. These
 *       variables will not be used by anything that needs to ensure its
 *       qualified variables must come from a trusted source, such as
 *       authorization. This parameter is intended to be used for testing only
 *       and should not be used for anything running in production.
 *
 * Returns:
 *
 *   (object) Test results.
 */
const afw_value_t *
afw_function_execute_test_template(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_object_t *result;
    const afw_value_string_t *id;
    const afw_value_string_t *description;
    const afw_value_string_t *template;
    const afw_value_t *expected;
    const afw_value_t *compiled;
    const afw_value_t *evaluated;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(id, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(description, 2, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(template, 3, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(expected, 4);

    result = afw_object_create_managed(x->p, xctx);
    afw_object_set_property(result, &afw_s_passed, afw_value_true, xctx);
    afw_object_set_property_as_string(result,
        &afw_s_id, &id->internal, xctx);
    afw_object_set_property_as_string(result,
        &afw_s_description, &description->internal, xctx);
    afw_object_set_property_as_string(result,
        &afw_s_template, &template->internal, xctx);

    afw_object_set_property_as_string(result,
        &afw_s_expected, 
        afw_value_as_casted_utf8(expected, x->p, xctx),
        xctx);

    AFW_TRY {
        compiled = afw_compile_to_value(
            &template->internal, AFW_FUNCTION_SOURCE_LOCATION,
            afw_compile_type_template,
            NULL, NULL, x->p, xctx);

        if (AFW_FUNCTION_PARAMETER_IS_PRESENT(5)) {
            evaluated = afw_value_evaluate_with_additional_untrusted_qualified_variables(
                compiled, x->argv[5], x->p, xctx);
        }
        else {
            evaluated = afw_value_evaluate(compiled, x->p, xctx);
        }

        afw_object_set_property_as_string(result, &afw_s_result,
            afw_value_as_casted_utf8(evaluated, x->p, xctx),
            xctx);

        if (!afw_value_equal(evaluated, expected, xctx)) {
            afw_object_set_property(result, &afw_s_passed, afw_value_false,
                xctx);
        }
    }

    AFW_CATCH_UNHANDLED{

        /* If 'error' is not expected value, set passed false. */
        if (!afw_value_is_string(expected) ||
            !afw_utf8_equal(
                &((const afw_value_string_t *)expected)->internal,
                &afw_s_error))
        {
            afw_object_set_property(result, &afw_s_passed, afw_value_false,
                xctx);
        }

        /* Set error property. */
        afw_object_set_property_as_object(result, &afw_s_error,
            afw_error_to_object(AFW_ERROR_THROWN, x->p, xctx), xctx);
    }

    AFW_ENDTRY;

    return afw_value_create_object(result, x->p, xctx);
}



/*
 * Adaptive function: test_value
 *
 * afw_function_execute_test_value
 *
 * See afw_function_bindings.h for more information.
 *
 * Evaluate an adaptive value and compare it to an expected value. Return
 * object with the test's results.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function test_value(
 *       id: string,
 *       description: string,
 *       value: string,
 *       expected: any,
 *       additionalUntrustedQualifiedVariables?: (object _AdaptiveHybridPropertiesObjects_)
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   id - (string) Id of test.
 *
 *   description - (string) Description of test.
 *
 *   value - (string) Value to evaluate.
 *
 *   expected - (any dataType) Expected result.
 *
 *   additionalUntrustedQualifiedVariables - (optional object
 *       _AdaptiveHybridPropertiesObjects_) This parameter supplies additional
 *       qualified variables that can be accessed during evaluation. These
 *       variables will not be used by anything that needs to ensure its
 *       qualified variables must come from a trusted source, such as
 *       authorization. This parameter is intended to be used for testing only
 *       and should not be used for anything running in production.
 *
 * Returns:
 *
 *   (object) Test results.
 */
const afw_value_t *
afw_function_execute_test_value(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_object_t *result;
    const afw_value_string_t *id;
    const afw_value_string_t *description;
    const afw_value_t *value;
    const afw_value_t *expected;
    afw_utf8_t value_source;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(id, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(description, 2, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(value, 3);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(expected, 4);

    result = afw_object_create_managed(x->p, xctx);
    afw_object_set_property(result, &afw_s_passed, afw_value_true, xctx);
    afw_object_set_property_as_string(result,
        &afw_s_id, &id->internal, xctx);
    afw_object_set_property_as_string(result,
        &afw_s_description, &description->internal, xctx);

    if (afw_value_is_any_call(AFW_FUNCTION_ARGV(3))) {
        afw_value_contextual_resolve_value_source(&value_source,
            ((const afw_value_call_t *)x->argv[3])->args.contextual);
        afw_object_set_property_as_string(result,
            &afw_s_value, &value_source, xctx);
    }

    afw_object_set_property_as_string(result,
        &afw_s_expected, 
        afw_value_as_casted_utf8(expected, x->p, xctx),
        xctx);

    AFW_TRY {

        /* If additional context, evaluate with context ahead of time. */
        if (AFW_FUNCTION_PARAMETER_IS_PRESENT(5)) {
            value = afw_value_evaluate_with_additional_untrusted_qualified_variables(
                value, x->argv[5], x->p, xctx);
        }

        afw_object_set_property_as_string(result, &afw_s_result,
            afw_value_as_casted_utf8(value, x->p, xctx),
            xctx);
        
        if (!afw_value_equal(value, expected, xctx)) {
            afw_object_set_property(result, &afw_s_passed, afw_value_false,
                xctx);
        }

    }

    AFW_CATCH_UNHANDLED{

        /* If 'error' is not expected value, set passed false. */
        if (!afw_value_is_string(expected) ||
            !afw_utf8_equal(
                &((const afw_value_string_t *)expected)->internal,
                &afw_s_error))
        {
            afw_object_set_property(result, &afw_s_passed, afw_value_false,
                xctx);
        }

        /* Set error property. */
        afw_object_set_property_as_object(result, &afw_s_error,
            afw_error_to_object(AFW_ERROR_THROWN, x->p, xctx), xctx);
    }

    AFW_ENDTRY;

    return afw_value_create_object(result, x->p, xctx);
}



/*
 * Adaptive function: qualifier
 *
 * afw_function_execute_qualifier
 *
 * See afw_function_bindings.h for more information.
 *
 * This function allows the active variables for a qualifier to be accessed as
 * the properties of an object.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function qualifier(
 *       qualifier: string,
 *       forTesting?: boolean
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   qualifier - (string) This is the qualifier whose variables are to be
 *       accessed as properties of the returned object.
 *
 *   forTesting - (optional boolean) If specified and true, the object returned
 *       will be suitable to pass as the additionalUntrustedQualifiedVariables
 *       parameter of evaluate*() functions. This is intended for testing
 *       purposes and should not be used in production.
 *
 * Returns:
 *
 *   (object) Each property is the name of a variable with the value influenced
 *       by the forTesting property.
 */
const afw_value_t *
afw_function_execute_qualifier(
    afw_function_execute_t *x)
{
    const afw_value_string_t *qualifier;
    const afw_value_boolean_t *forTesting;
    const afw_object_t *object;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(qualifier,
        1, string);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(forTesting,
        2, boolean);

    object = afw_xctx_qualifier_object_create(
        &qualifier->internal,
        (forTesting && forTesting->internal),
        x->p, x->xctx);

    return afw_value_create_object(object, x->p, x->xctx);
}



/*
 * Adaptive function: qualifiers
 *
 * afw_function_execute_qualifiers
 *
 * See afw_function_bindings.h for more information.
 *
 * This function allows the active qualifiers to be accessed as properties of
 * an object. The value of each of these properties is an object whose
 * properties are the variables for the corresponding qualifier.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function qualifiers(
 *       forTesting?: boolean
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   forTesting - (optional boolean) If specified and true, the object returned
 *       will be suitable to pass as the additionalUntrustedQualifiedVariables
 *       parameter of evaluate*() functions. This is intended for testing
 *       purposes and should not be used in production.
 *
 * Returns:
 *
 *   (object) Each property is the name of a qualifier with a value that is an
 *       object whose properties are the variables of that qualifier. The value
 *       of the variable properties is influenced by the forTesting property.
 */
const afw_value_t *
afw_function_execute_qualifiers(
    afw_function_execute_t *x)
{
    const afw_value_boolean_t *forTesting;
    const afw_object_t *object;

    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(forTesting,
        1, boolean);

    object = afw_xctx_qualifiers_object_create(
        (forTesting && forTesting->internal),
        x->p, x->xctx);

    return afw_value_create_object(object, x->p, x->xctx);
}
