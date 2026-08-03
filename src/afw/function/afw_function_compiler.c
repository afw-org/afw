// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for compiler
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_compiler.c
 * @brief Adaptive function execute implementations for category `compiler`.
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
 *   ): void;
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
 *   (void)
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

    /* Return undefined for void. */
    return afw_value_undefined;
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
                result = afw_s_a_tab;
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
 * Adaptive function: decompile
 *
 * afw_function_execute_decompile
 *
 * See afw_function_bindings.h for more information.
 *
 * Decompile an adaptive value to Adaptive text that represents the compiled
 * form (functional forms and #implementation_id(...) pragmas such as
 * #script_function, #block, #assignment_target). This is not original source
 * recovery and is not pure JSON — use stringify() for JSON of evaluated data,
 * and compile(..., listing) for a human compiler listing with symbol tables.
 * Many decompile forms recompile to the same compiled value; #closure_binding
 * and #function_thunk are known rejects (runtime-only / C-side). Optional
 * whitespace matches stringify/listing style (integer 0-10 or indent string).
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
 *   value - (any) Value to decompile (may be unevaluated, such as a compiled
 *       script root).
 *
 *   whitespace - (optional any) Add whitespace for readability if present and
 *       not 0. This parameter can be an integer between 0 and 10 or a string
 *       that is used for indentation. If 0 is specified, no whitespace is added
 *       to the resulting string. If 1 through 10 is specified, that number of
 *       spaces is used.
 *
 * Returns:
 *
 *   (string) Adaptive text for the compiled form of the value.
 */
const afw_value_t *
afw_function_execute_decompile(
    afw_function_execute_t *x)
{
    const afw_utf8_t *whitespace;
    const afw_utf8_t *s;
    const afw_value_t *value;

    /*
     * Resolve the argument (symbol refs, compile<script>(...), etc.) but do
     * not evaluate a compiled_value — that would run the script. Standard
     * AFW_FUNCTION_EVALUATE_PARAMETER re-evaluates compiled_value.
     */
    value = (x->argc >= 1) ? x->argv[1] : NULL;
    if (!afw_value_is_defined_and_evaluated(value)) {
        value = afw_value_evaluate(value, x->p, x->xctx);
    }

    whitespace = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        whitespace = afw_function_evaluate_whitespace_parameter(x, 2);
    }

    s = afw_value_decompile_to_string(value, whitespace, x->p, x->xctx);

    return afw_value_create_unmanaged_string(s, x->p, x->xctx);
}



/*
 * Adaptive function: evaluate
 *
 * afw_function_execute_evaluate
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
 *   function evaluate(
 *       value: any,
 *       additionalUntrustedQualifiedVariables?: object // _AdaptiveTemplatePropertiesObjects_
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   value - (any)
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
 *   (any) Evaluated adaptive value.
 */
const afw_value_t *
afw_function_execute_evaluate(
    afw_function_execute_t *x)
{
    const afw_value_t *value;

    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        /*
         * Fully evaluate with additional frames still pushed (including any
         * nested compiled_value). Do not re-evaluate after untrusted frames
         * are popped — that would lose multi-frame qualifier resolution.
         */
        value =
            afw_value_evaluate_with_additional_untrusted_qualified_variables(
                x->argv[1], x->argv[2], x->p, x->xctx);
    }
    else {
        value = afw_value_evaluate(x->argv[1], x->p, x->xctx);
        if (afw_value_is_compiled_value(value)) {
            value = afw_value_evaluate(value, x->p, x->xctx);
        }
    }

    afw_xctx_statement_flow_reset_all_except_rethrow(x->xctx);
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
 *   value - (any) Value to evaluated.
 *
 *   limit - (integer) Maximum number to retry if an exception occurs.
 *
 * Returns:
 *
 *   (any) Evaluated value.
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

    afw_xctx_statement_flow_reset_all_except_rethrow(xctx);
    return value;
}



/*
 * Adaptive function: safe_evaluate
 *
 * afw_function_execute_safe_evaluate
 *
 * See afw_function_bindings.h for more information.
 *
 * Return the evaluated adaptive value. If an exception occurs, return evaluated
 * error instead.
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
 *   value - (any) Value to evaluated.
 *
 *   error - (any) Value to evaluate and return if exception occurs. If an error
 *       occurs evaluating this value, the exception will continue.
 *
 * Returns:
 *
 *   (any) Evaluated adaptive value or error value.
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
      
    afw_xctx_statement_flow_reset_all_except_rethrow(xctx);
    return value;
}



/*
 * stringify() helpers — pure JSON after optional replacer transform.
 *
 * First parameter is evaluated (AFW_FUNCTION_EVALUATE_PARAMETER). Replacer
 * function calls use the same pattern as higher_order_array functors:
 * afw_function_evaluate_function_parameter + afw_value_call_create + evaluate.
 */

typedef struct {
    const afw_value_t *call;          /* skeleton call; argv slots below */
    const afw_value_t **f_argv;       /* f_argv[0]=fn, [1]=key, [2]=value */
    const afw_array_t *allow_names;   /* optional property-name allow list */
    const afw_pool_t *p;
    afw_xctx_t *xctx;
} impl_stringify_ctx_t;

static afw_boolean_t
impl_stringify_name_allowed(
    const impl_stringify_ctx_t *ctx,
    const afw_utf8_t *name)
{
    const afw_iterator_t *iterator;
    const afw_value_t *entry;
    const afw_utf8_t *s;

    if (!ctx->allow_names) {
        return true;
    }
    for (iterator = NULL;;) {
        entry = afw_array_get_next_value(ctx->allow_names, &iterator,
            ctx->p, ctx->xctx);
        if (!entry) {
            return false;
        }
        if (afw_value_is_string(entry)) {
            s = &((const afw_value_string_t *)entry)->internal;
            if (afw_utf8_equal(s, name)) {
                return true;
            }
        }
    }
}

static const afw_value_t *
impl_stringify_call_replacer(
    impl_stringify_ctx_t *ctx,
    const afw_utf8_t *key,
    const afw_value_t *value)
{
    if (!ctx->call) {
        return value;
    }
    ctx->f_argv[1] = afw_value_create_unmanaged_string(key, ctx->p, ctx->xctx);
    ctx->f_argv[2] = value ? value : afw_value_null;
    return afw_value_evaluate(ctx->call, ctx->p, ctx->xctx);
}

static const afw_value_t *
impl_stringify_prepare(
    impl_stringify_ctx_t *ctx,
    const afw_utf8_t *key,
    const afw_value_t *value);

static const afw_value_t *
impl_stringify_prepare_object(
    impl_stringify_ctx_t *ctx,
    const afw_object_t *obj)
{
    const afw_object_t *out;
    const afw_iterator_t *iterator;
    const afw_utf8_t *property_name;
    const afw_value_t *next;
    const afw_value_t *child;

    out = afw_object_create_unmanaged(ctx->p, ctx->xctx);
    iterator = NULL;
    for (;;) {
        next = afw_object_get_next_property(obj, &iterator, &property_name,
            ctx->xctx);
        if (!next) {
            break;
        }
        if (!impl_stringify_name_allowed(ctx, property_name)) {
            continue;
        }
        child = impl_stringify_prepare(ctx, property_name, next);
        if (afw_value_is_undefined(child) || !child) {
            /* Omit property when replacer returns undefined. */
            continue;
        }
        afw_object_set_property(out, property_name, child, ctx->xctx);
    }
    return afw_value_create_unmanaged_object(out, ctx->p, ctx->xctx);
}

static const afw_value_t *
impl_stringify_prepare_array(
    impl_stringify_ctx_t *ctx,
    const afw_array_t *list)
{
    const afw_array_t *out;
    const afw_iterator_t *iterator;
    const afw_value_t *next;
    const afw_value_t *child;
    const afw_utf8_t *index_s;
    afw_integer_t index;

    out = afw_array_create_generic(ctx->p, ctx->xctx);
    iterator = NULL;
    index = 0;
    for (;;) {
        next = afw_array_get_next_value(list, &iterator, ctx->p, ctx->xctx);
        if (!next) {
            break;
        }
        index_s = afw_number_integer_to_utf8(index, ctx->p, ctx->xctx);
        child = impl_stringify_prepare(ctx, index_s, next);
        if (afw_value_is_undefined(child) || !child) {
            /* Array holes / undefined become JSON null. */
            child = afw_value_null;
        }
        afw_array_push_value(out, child, ctx->xctx);
        index++;
    }
    return afw_value_create_unmanaged_array(out, ctx->p, ctx->xctx);
}

/*
 * Apply replacer for this key/value, then recursively prepare objects/arrays.
 * Returns undefined to mean "omit" (object property) / "null" (array element).
 */
static const afw_value_t *
impl_stringify_prepare(
    impl_stringify_ctx_t *ctx,
    const afw_utf8_t *key,
    const afw_value_t *value)
{
    value = impl_stringify_call_replacer(ctx, key, value);
    if (afw_value_is_undefined(value) || !value) {
        return afw_value_undefined;
    }
    if (afw_value_is_object(value)) {
        return impl_stringify_prepare_object(ctx,
            ((const afw_value_object_t *)value)->internal);
    }
    if (afw_value_is_array(value)) {
        return impl_stringify_prepare_array(ctx,
            ((const afw_value_array_t *)value)->internal);
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
 * Evaluate value and serialize it as pure JSON text. Adaptive data types use
 * their jsonPrimitive (for example base64Binary and date become JSON strings).
 * The value is fully evaluated before serialization (not Adaptive compiled
 * form). For Adaptive compiled form as text use decompile(). For binary octets
 * as UTF-8 text use decode_to_string(); string(binary) is base64 printable
 * text, not UTF-8. Optional replacer is a function (key, value) that returns
 * the value to serialize, or an array of property names to include when
 * serializing objects. Optional whitespace matches decompile/listing style.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function stringify(
 *       value: any,
 *       replacer?: (key: string, value: any) => any,
 *       whitespace?: any
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   value - (any) Evaluated value to serialize as JSON.
 *
 *   replacer - (optional (key: string, value: any) => any) Optional replacer: a
 *       function (key: string, value: any): any called for the root (key is
 *       empty string) and each object property or array element; return
 *       undefined to omit an object property (array elements become null). Or
 *       an array of string property names to keep when serializing objects.
 *       Omit or null for no replacer.
 *
 *   whitespace - (optional any) Add whitespace for readability if present and
 *       not 0. This parameter can be an integer between 0 and 10 or a string
 *       that is used for indentation. If 0 is specified, no whitespace is added
 *       to the resulting string. If 1 through 10 is specified, that number of
 *       spaces is used.
 *
 * Returns:
 *
 *   (string) JSON text for the value.
 */
const afw_value_t *
afw_function_execute_stringify(
    afw_function_execute_t *x)
{

    const afw_utf8_t *whitespace;
    const afw_utf8_t *s;
    const afw_value_t *value;
    const afw_value_t *replacer;
    const afw_value_t *prepared;
    afw_object_options_t options;
    impl_stringify_ctx_t ctx;
    const afw_value_t **f_argv;
    afw_xctx_t *xctx = x->xctx;

    /* Evaluate first parameter fully (not decompile of unevaluated form). */
    AFW_FUNCTION_EVALUATE_PARAMETER(value, 1);

    if (!value || afw_value_is_undefined(value)) {
        return afw_value_create_unmanaged_string(
            afw_s_null, x->p, xctx);
    }

    afw_memory_clear(&ctx);
    ctx.p = x->p;
    ctx.xctx = xctx;

    AFW_FUNCTION_EVALUATE_PARAMETER(replacer, 2);
    if (!afw_value_is_nullish(replacer)) {
        if (afw_value_is_array(replacer)) {
            ctx.allow_names =
                ((const afw_value_array_t *)replacer)->internal;
        }
        else {
            f_argv = afw_pool_calloc(x->p, sizeof(afw_value_t *) * 3, xctx);
            f_argv[0] = afw_function_evaluate_function_parameter(
                replacer, x->p, xctx);
            if (!f_argv[0]) {
                AFW_THROW_ERROR_Z(arg_error,
                    "stringify replacer function is required when replacer "
                    "is not an array of property names",
                    xctx);
            }
            ctx.f_argv = f_argv;
            ctx.call = afw_value_call_create(AFW_FUNCTION_CONTEXTUAL,
                2, f_argv, false, x->p, xctx);
        }
    }

    if (!ctx.call && !ctx.allow_names) {
        /* No replacer: serialize the evaluated value as-is. */
        prepared = value;
    }
    else {
        prepared = impl_stringify_prepare(&ctx, afw_s_a_empty_string, value);
        if (afw_value_is_undefined(prepared) || !prepared) {
            return afw_value_create_unmanaged_string(
                afw_s_null, x->p, xctx);
        }
    }

    whitespace = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        whitespace = afw_function_evaluate_whitespace_parameter(x, 3);
    }

    afw_memory_clear(&options);
    if (whitespace && whitespace->len > 0) {
        AFW_OBJECT_OPTION_SET_ON(&options, whitespace);
    }

    s = afw_json_from_value_with_indent(prepared, &options, whitespace,
        x->p, xctx);

    return afw_value_create_unmanaged_string(s, x->p, xctx);
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
 *       expected?: any,
 *       additionalUntrustedQualifiedVariables?: object // _AdaptiveTemplatePropertiesObjects_
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
 *   expected - (optional any) Expected result.
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
    AFW_FUNCTION_EVALUATE_PARAMETER(expected, 4);

    result = afw_object_create(x->p, xctx);
    afw_object_set_property(result, afw_s_passed, afw_boolean_v_true, xctx);
    afw_object_set_property_as_string(result,
        afw_s_id, &id->internal, xctx);
    afw_object_set_property_as_string(result,
        afw_s_description, &description->internal, xctx);
    afw_object_set_property_as_string(result,
        afw_s_expression, &expression->internal, xctx);
    afw_object_set_property_as_string(result,
        afw_s_expected,
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

        afw_object_set_property_as_string(result, afw_s_result,
            afw_value_as_casted_utf8(evaluated, x->p, xctx),
            xctx);

        if (!afw_value_equal(evaluated, expected, xctx)) {
            afw_object_set_property(result, afw_s_passed, afw_boolean_v_false,
                xctx);
        }
    }

        AFW_CATCH_UNHANDLED{

            /* If 'error' is not expected value, set passed false. */
            if (!afw_value_is_string(expected) ||
                !afw_utf8_equal(
                    &((const afw_value_string_t *)expected)->internal,
                    afw_s_error))
            {
                afw_object_set_property(result, afw_s_passed, afw_boolean_v_false,
                    xctx);
            }

            /* Set error property. */
            afw_object_set_property_as_object(result, afw_s_error,
                afw_error_to_object(AFW_ERROR_THROWN, x->p, xctx), xctx);
    }

    AFW_ENDTRY;

    afw_xctx_statement_flow_reset_all_except_rethrow(xctx);
    return afw_value_create_unmanaged_object(result, x->p, xctx);
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
 *       expected?: any,
 *       additionalUntrustedQualifiedVariables?: object // _AdaptiveTemplatePropertiesObjects_
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
 *   expected - (optional any) Expected evaluated result.
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
    AFW_FUNCTION_EVALUATE_PARAMETER(expected, 4);

    result = afw_object_create(x->p, xctx);
    afw_object_set_property(result, afw_s_passed, afw_boolean_v_true, xctx);
    afw_object_set_property_as_string(result,
        afw_s_id, &id->internal, xctx);
    afw_object_set_property_as_string(result,
        afw_s_description, &description->internal, xctx);
    afw_object_set_property_as_string(result,
        afw_s_template, &template->internal, xctx);

    afw_object_set_property_as_string(result,
        afw_s_expected, 
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

        afw_object_set_property_as_string(result, afw_s_result,
            afw_value_as_casted_utf8(evaluated, x->p, xctx),
            xctx);

        if (!afw_value_equal(evaluated, expected, xctx)) {
            afw_object_set_property(result, afw_s_passed, afw_boolean_v_false,
                xctx);
        }
    }

    AFW_CATCH_UNHANDLED{

        /* If 'error' is not expected value, set passed false. */
        if (!afw_value_is_string(expected) ||
            !afw_utf8_equal(
                &((const afw_value_string_t *)expected)->internal,
                afw_s_error))
        {
            afw_object_set_property(result, afw_s_passed, afw_boolean_v_false,
                xctx);
        }

        /* Set error property. */
        afw_object_set_property_as_object(result, afw_s_error,
            afw_error_to_object(AFW_ERROR_THROWN, x->p, xctx), xctx);
    }

    AFW_ENDTRY;
  
    afw_xctx_statement_flow_reset_all_except_rethrow(xctx);
    return afw_value_create_unmanaged_object(result, x->p, xctx);
}



/*
 * Adaptive function: test_value
 *
 * afw_function_execute_test_value
 *
 * See afw_function_bindings.h for more information.
 *
 * Evaluate an adaptive value and compare it to an expected value. Return object
 * with the test's results.
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
 *       expected?: any,
 *       additionalUntrustedQualifiedVariables?: object // _AdaptiveTemplatePropertiesObjects_
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
 *   expected - (optional any) Expected result.
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
    AFW_FUNCTION_EVALUATE_PARAMETER(expected, 4);

    result = afw_object_create(x->p, xctx);
    afw_object_set_property(result, afw_s_passed, afw_boolean_v_true, xctx);
    afw_object_set_property_as_string(result,
        afw_s_id, &id->internal, xctx);
    afw_object_set_property_as_string(result,
        afw_s_description, &description->internal, xctx);

    if (afw_value_is_any_call(AFW_FUNCTION_ARGV(3))) {
        afw_value_contextual_resolve_value_source(&value_source,
            ((const afw_value_call_t *)x->argv[3])->args.contextual);
        afw_object_set_property_as_string(result,
            afw_s_value, &value_source, xctx);
    }

    afw_object_set_property_as_string(result,
        afw_s_expected, 
        afw_value_as_casted_utf8(expected, x->p, xctx),
        xctx);

    AFW_TRY {

        /* If additional context, evaluate with context ahead of time. */
        if (AFW_FUNCTION_PARAMETER_IS_PRESENT(5)) {
            value =
            afw_value_evaluate_with_additional_untrusted_qualified_variables(
                value, x->argv[5], x->p, xctx);
        }

        afw_object_set_property_as_string(result, afw_s_result,
            afw_value_as_casted_utf8(value, x->p, xctx),
            xctx);
        
        if (!afw_value_equal(value, expected, xctx)) {
            afw_object_set_property(result, afw_s_passed, afw_boolean_v_false,
                xctx);
        }

    }

    AFW_CATCH_UNHANDLED{

        /* If 'error' is not expected value, set passed false. */
        if (!afw_value_is_string(expected) ||
            !afw_utf8_equal(
                &((const afw_value_string_t *)expected)->internal,
                afw_s_error))
        {
            afw_object_set_property(result, afw_s_passed, afw_boolean_v_false,
                xctx);
        }

        /* Set error property. */
        afw_object_set_property_as_object(result, afw_s_error,
            afw_error_to_object(AFW_ERROR_THROWN, x->p, xctx), xctx);
    }

    AFW_ENDTRY;
  
    afw_xctx_statement_flow_reset_all_except_rethrow(xctx);
    return afw_value_create_unmanaged_object(result, x->p, xctx);
}



/*
 * Adaptive function: qualifier
 *
 * afw_function_execute_qualifier
 *
 * See afw_function_bindings.h for more information.
 *
 * Returns a new memory object whose properties are the active variables for the
 * given qualifier (issue #9). Built from the current xctx qualifier stack via
 * contribute callbacks; not a live view. Each call creates a fresh object.
 * Intended for debugging, tooling, and tests — not for hot production paths
 * that only need qualifier::name access.
 * 
 * Warning: snapshots can be large. Qualifiers such as environment:: or
 * request:: may contribute many properties (and some values can themselves be
 * large objects). qualifiers() nests a full snapshot per active qualifier name
 * and multiplies that cost. Prefer qualifier::name for normal work; use these
 * functions sparingly and avoid holding or repeatedly rebuilding large
 * snapshots in long-running scripts.
 * 
 * All matching visible stack entries for the qualifier name contribute into one
 * object (most recent first; later entries only fill property names not already
 * set). Get (qualifier::name) uses the same first-defining-frame rule per name
 * (newest → older; first non-null get_cb wins, including present undefined/null
 * values). Default visibility matches normal qualifier::name access right now.
 * Optional includeUntrusted is only meaningful while the xctx is secure: set
 * true so the snapshot includes the same frames you would see with :: if you
 * were less secure (trusted and untrusted). When already not secure, the flag
 * changes nothing.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function qualifier(
 *       qualifier: string,
 *       includeUntrusted?: boolean
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   qualifier - (string) This is the qualifier whose variables are to be
 *       accessed as properties of the returned object.
 *
 *   includeUntrusted - (optional boolean) Default false: snapshot matches what
 *       qualifier::name can access in the current xctx (while secure, untrusted
 *       stack frames with secure=false are omitted). Set true while secure to
 *       use the same visibility as running less secure — trusted and untrusted
 *       frames (not untrusted-only). When the xctx is not secure, true and
 *       false are the same because :: already sees untrusted frames. Does not
 *       change hot-path get; only this snapshot. Useful for debugging secure
 *       evaluation and for building objects to re-inject as evaluate()'s
 *       additionalUntrustedQualifiedVariables.
 *
 * Returns:
 *
 *   (any) When the qualifier has at least one matching visible stack entry,
 *       each property is a variable name for that qualifier (values from
 *       contribute, most recent entry wins per name). Fresh object on every
 *       call (may be empty if nothing was contributed). When no matching
 *       visible entry exists for that qualifier name, the result is undefined
 *       (nullish), not an empty object.
 */
const afw_value_t *
afw_function_execute_qualifier(
    afw_function_execute_t *x)
{
    const afw_value_string_t *qualifier;
    const afw_value_boolean_t *includeUntrusted;
    const afw_object_t *object;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(qualifier,
        1, string);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(includeUntrusted,
        2, boolean);

    object = afw_xctx_qualifier_object_create(
        &qualifier->internal,
        (includeUntrusted && includeUntrusted->internal),
        x->p, x->xctx);

    if (!object) {
        return afw_value_undefined;
    }

    return afw_value_create_unmanaged_object(object, x->p, x->xctx);
}



/*
 * Adaptive function: qualifiers
 *
 * afw_function_execute_qualifiers
 *
 * See afw_function_bindings.h for more information.
 *
 * Returns a new memory object whose properties are active qualifier names; each
 * value is an object of that qualifier's variables (issue #9). Built from the
 * current xctx qualifier stack; each call creates a fresh object. Intended for
 * debugging, tooling, and tests — not for hot production paths that only need
 * qualifier::name access.
 * 
 * Warning: the result can be very large. Each property is a full snapshot of
 * that qualifier (see qualifier()), so environment, request, application,
 * current, and others can all appear as nested objects with many properties.
 * Prefer qualifier::name or qualifier(name) when you need one bag; avoid
 * repeated qualifiers() calls or retaining the result in long-running work.
 * 
 * Each nested variables object is the multi-entry snapshot for that name (all
 * matching visible stack entries contribute; most recent wins per property). A
 * qualifier name is omitted if it is not active (same as qualifier(name) being
 * nullish); never invent an empty nested object for an inactive name. Default
 * visibility matches normal qualifier::name access right now. Optional
 * includeUntrusted is only meaningful while the xctx is secure: set true so
 * each nested snapshot uses the same frame visibility as running less secure
 * (trusted and untrusted). When already not secure, the flag changes nothing.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function qualifiers(
 *       includeUntrusted?: boolean
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   includeUntrusted - (optional boolean) Default false: only qualifiers/frames
 *       visible to qualifier::name in the current xctx. Set true while secure
 *       to match less-secure :: visibility (include untrusted frames). When not
 *       secure, true and false are the same. Does not change hot-path get. The
 *       result shape (qualifier → variables object) is suitable to pass as
 *       evaluate()'s additionalUntrustedQualifiedVariables when that is the
 *       intent.
 *
 * Returns:
 *
 *   (object) Each property is an active qualifier name with a value that is a
 *       variables snapshot object for that qualifier. Inactive names are
 *       omitted. Fresh object on every call.
 */
const afw_value_t *
afw_function_execute_qualifiers(
    afw_function_execute_t *x)
{
    const afw_value_boolean_t *includeUntrusted;
    const afw_object_t *object;

    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(includeUntrusted,
        1, boolean);

    object = afw_xctx_qualifiers_object_create(
        (includeUntrusted && includeUntrusted->internal),
        x->p, x->xctx);

    return afw_value_create_unmanaged_object(object, x->p, x->xctx);
}


/* holds context data for the callback routine */
typedef struct {
    const char *file_z;
    apr_file_t *f;
    apr_finfo_t finfo;
} afw_include_self_t;

/* callback routine for the parser to read the file octet by octet */
int impl_octet_get_cb(afw_utf8_octet_t *octet, void *data, afw_xctx_t *xctx)
{
    afw_include_self_t *self = (afw_include_self_t *)data;
    char c;
    apr_size_t len = 1;
    int rv;

    /* Get an octet. */
    rv = apr_file_read(self->f, &c, &len);
    
    /* if error return -1 */
    if (rv != APR_SUCCESS) {
        *octet = 0;
        return -1;
    }

    *octet = (afw_utf8_octet_t)c;
    return 0;
}

/*
 * Adaptive function: compile_from_file
 *
 * afw_function_execute_compile_from_file
 *
 * See afw_function_bindings.h for more information.
 *
 * This function is deprecated.
 *
 * Load an external adaptive script, json, or template to be compiled and
 * returned.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function compile_from_file(
 *       file: string,
 *       compileType?: string
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   file - (string) The path of the file to include, resolved using
 *       rootFilePaths (longest matching prefix; host path must remain under
 *       that root).
 *
 *   compileType - (optional string) The compile type, used by the parser to
 *       determine how to compile the data.
 *       For example, 'json', 'relaxed_json', 'script', 'template'.
 *
 * Returns:
 *
 *   (any)
 */
const afw_value_t *
afw_function_execute_compile_from_file(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_pool_t *p = x->p;
    const afw_value_t *result = NULL;
    const afw_value_string_t *file_value;
    const afw_value_string_t *compile_type_value;
    afw_compile_type_t compile_type = afw_compile_type_script;
    const afw_utf8_t *file;
    const afw_utf8_t *compile_type_string;
    afw_include_self_t *self;
    apr_pool_t *apr_p = afw_pool_get_apr_pool(xctx->p);
    apr_status_t rv;
    const afw_utf8_t *resolved_path;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(file_value, 1, string);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(compile_type_value, 2, string);

    self = afw_xctx_calloc_type(afw_include_self_t, xctx);    

    if (compile_type_value) {
        compile_type_string = &compile_type_value->internal;
        if (afw_utf8_compare(compile_type_string, afw_s_json) == 0)
            compile_type = afw_compile_type_json;
        else if (afw_utf8_compare(compile_type_string, afw_s_relaxed_json) == 0)
            compile_type = afw_compile_type_relaxed_json;
        else if (afw_utf8_compare(compile_type_string, afw_s_script) == 0)
            compile_type = afw_compile_type_script;
        else if (afw_utf8_compare(compile_type_string, afw_s_template) == 0)
            compile_type = afw_compile_type_template;
        else {
            AFW_THROW_ERROR_FZ(bad_request, xctx,
                "Invalid compile type '%s'.", compile_type_string);
        }
    }

    file = &file_value->internal;

    /* Resolve logical path via shared rootFilePaths helper. */
    resolved_path = afw_file_path_resolve_rootFilePaths(file, p, xctx);
    self->file_z = afw_utf8_to_utf8_z(resolved_path, p, xctx);

    /* now open the file */
    rv = apr_file_open(&self->f, self->file_z, 
        APR_FOPEN_READ | APR_BUFFERED, APR_OS_DEFAULT, apr_p);
    if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_FZ(not_found, apr, rv, xctx,
            "Failed to open file '%s'.", self->file_z);
    }

    /* read it using a callback and let it convert to an adaptive value */
    AFW_TRY {
        result = afw_compile_to_value_with_callback(NULL,
            impl_octet_get_cb, self, file, compile_type, 
            afw_compile_residual_check_to_full,
            NULL, NULL, x->p, xctx
        );
    }
    AFW_FINALLY {
        apr_file_close(self->f);
    }
    AFW_ENDTRY;

    if (!result) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "Failed to compile file '%s'.", self->file_z);
    }

    return result;
}



/*
 * Adaptive function: eval_from_file
 *
 * afw_function_execute_eval_from_file
 *
 * See afw_function_bindings.h for more information.
 *
 * Load an external adaptive script, json, or template to be compiled and
 * evaluate.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function eval_from_file(
 *       file: string,
 *       compileType?: string
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   file - (string) The path of the file to include, resolved using
 *       rootFilePaths (longest matching prefix; host path must remain under
 *       that root).
 *
 *   compileType - (optional string) The compile type, used by the parser to
 *       determine how to compile the data.
 *       For example, 'json', 'relaxed_json', 'script', 'template'.
 *
 * Returns:
 *
 *   (any)
 */
const afw_value_t *
afw_function_execute_eval_from_file(
    afw_function_execute_t *x)
{
    const afw_value_t *result;

    /* This is the same as compile except it also calls evalaute. */
    result = afw_function_execute_compile_from_file(x);
    return afw_value_evaluate(result, x->p, x->xctx);
}
