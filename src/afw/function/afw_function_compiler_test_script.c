// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for test script runtime
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_runtime_test_script.c
 * @brief afw_function_execute_* functions for test script runtime.
 */

#include "afw_internal.h"


/*
 * Adaptive function: test_script_runtime_support
 *
 * afw_function_execute_test_script_runtime_support
 *
 * See afw_function_bindings.h for more information.
 *
 * This is a function called internally as the result of a test_script compile.
 * This function is not intended to be called directly.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function test_script_runtime_support(
 *       testScriptObject: (object _AdaptiveTestScriptResult_)
 *   ): (object _AdaptiveTestScriptResult_);
 * ```
 *
 * Parameters:
 *
 *   testScriptObject - (object _AdaptiveTestScriptResult_) A test script
 *       results object with the required evaluation result properties missing.
 *       The sources will be evaluated and the corresponding test result
 *       properties will be set.
 *
 * Returns:
 *
 *   (object _AdaptiveTestScriptResult_) The testScriptObject object with test
 *       result properties set.
 */
const afw_value_t *
afw_function_execute_test_script_runtime_support(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_value_object_t *testScriptObject;
    const afw_iterator_t *iterator;
    const afw_array_t *tests;
    const afw_object_t *test;
    const afw_value_t *value;
    const afw_utf8_t *default_source_type;
    const afw_utf8_t *source_type;
    const afw_utf8_t *source;
    const afw_utf8_t *expect;
    const afw_value_t *expected_value;
    const afw_value_t *compiled_value;
    const afw_value_t *evaluated_value;
    const afw_object_t *test_script;
    const afw_value_t *passed_value;
    const afw_utf8_t *errorReason;
    const afw_compile_type_info_t *info;
    afw_compile_value_contextual_t *contextual;
    const afw_value_t *argv[2];
    afw_integer_t expectUTF8OctetLengthInTestScript;
    afw_integer_t expectUTF8OctetOffsetInTestScript;
    afw_integer_t sourceUTF8OctetOffsetInTestScript;
    afw_integer_t sourceUTF8OctetLengthInTestScript;
    afw_utf8_t expect_message;

    enum {
        error_in_other,
        error_in_compile_expect,
        error_in_evaluate_expect,
        error_in_compile_source,
        error_in_evaluate_source
    } error_in;

    afw_boolean_t found;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(testScriptObject,
        1, object);

    test_script = testScriptObject->internal;
    tests = afw_object_old_get_property_as_array(test_script,
        &afw_s_tests, xctx);
    default_source_type = afw_object_old_get_property_as_string(
        test_script, &afw_s_sourceType, xctx);
    if (!default_source_type) {
        default_source_type = &afw_s_script;
    }
    for (iterator = NULL;;) {
        value = afw_array_get_next_value(tests, &iterator, x->p, xctx);
        if (!value) {
            break;
        }
        test = afw_value_as_object(value, xctx);
        source_type = afw_object_old_get_property_as_string(test,
            &afw_s_sourceType, xctx);
        if (!source_type) {
            source_type = default_source_type;
        }
        info = afw_compile_type_get_info_by_pneumonic(source_type, xctx);

        expect = afw_object_old_get_property_as_string(test,
            &afw_s_expect, xctx);
        if (!expect) {
            AFW_THROW_ERROR_Z(general, "expect required", xctx);
        }

        expectUTF8OctetLengthInTestScript = afw_object_old_get_property_as_integer(
            test, &afw_s_expectUTF8OctetLengthInTestScript, &found, xctx);
        if (!found) {
            AFW_THROW_ERROR_Z(code, "Internal error", xctx);
        }
        expectUTF8OctetOffsetInTestScript = afw_object_old_get_property_as_integer(
            test, &afw_s_expectUTF8OctetOffsetInTestScript, &found, xctx);
        if (!found) {
            AFW_THROW_ERROR_Z(code, "Internal error", xctx);
        }

        source = afw_object_old_get_property_as_string(test,
            &afw_s_source, xctx);
        if (!source) {
            AFW_THROW_ERROR_Z(general, "source required", xctx);
        }       
        sourceUTF8OctetOffsetInTestScript = afw_object_old_get_property_as_integer(
            test, &afw_s_sourceUTF8OctetOffsetInTestScript, &found, xctx);
        if (!found) {
            AFW_THROW_ERROR_Z(code, "Internal error", xctx);
        }
        sourceUTF8OctetLengthInTestScript = afw_object_old_get_property_as_integer(
            test, &afw_s_sourceUTF8OctetLengthInTestScript, &found, xctx);
        if (!found) {
            AFW_THROW_ERROR_Z(code, "Internal error", xctx);
        }

        /* Skip processing test is requested. */
        if (afw_object_old_get_property_as_boolean(test,
            &afw_s_skip, &found, xctx))
        {
            continue;
        }

        if (info->compile_type == afw_compile_type_error) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "source_type=" AFW_UTF8_FMT_Q " is invalid",
                AFW_UTF8_FMT_ARG(source_type));
        }

        AFW_TRY{
            error_in = error_in_other;
            (void)error_in; /* In catch. Avoid "not used" error. */
            if (afw_utf8_starts_with(expect, &afw_s_error)) {
                expected_value = NULL;
            }
            else if (afw_utf8_equal(expect, &afw_s_undefined)) {
                expected_value = afw_value_undefined;
            }
            else {
                error_in = error_in_compile_expect;
                (void)error_in; /* In catch. Avoid "not used" error. */
                contextual = afw_pool_calloc_type(x->p,
                    afw_compile_value_contextual_t, xctx);
                afw_memory_copy(contextual, x->self->args.contextual);
                contextual->value_offset = afw_safe_cast_integer_to_size(
                    expectUTF8OctetOffsetInTestScript, xctx);
                contextual->value_size = afw_safe_cast_integer_to_size(
                    expectUTF8OctetLengthInTestScript, xctx);
                value = afw_value_create_string(expect, x->p, xctx);
                argv[0] = (const afw_value_t *)info->compile_function;
                argv[1] = afw_value_convert(value, info->data_type, true,
                    x->p, xctx);
                compiled_value = afw_value_call_built_in_function(
                    contextual, info->compile_function, 1, argv, x->p, xctx);
                expected_value = afw_value_evaluate(compiled_value,
                    x->p, xctx);
            }

            error_in = error_in_compile_source;
            (void)error_in; /* In catch. Avoid "not used" error. */
            contextual = afw_pool_calloc_type(x->p,
                afw_compile_value_contextual_t, xctx);
            afw_memory_copy(contextual, x->self->args.contextual);
            contextual->value_offset = afw_safe_cast_integer_to_size(
                sourceUTF8OctetOffsetInTestScript, xctx);
            contextual->value_size = afw_safe_cast_integer_to_size(
                sourceUTF8OctetLengthInTestScript, xctx);
            value = afw_value_create_string(source, x->p, xctx);
            argv[0] = (const afw_value_t *)info->compile_function;
            argv[1] = afw_value_convert(value, info->data_type, true,
                x->p, xctx);
            compiled_value = afw_value_call_built_in_function(
                contextual, info->compile_function, 1, argv, x->p, xctx);
            error_in = error_in_evaluate_source;
            (void)error_in; /* In catch. Avoid "not used" error. */
            evaluated_value = afw_value_evaluate(compiled_value,
                x->p, xctx);
            error_in = error_in_other;
            (void)error_in; /* In catch. Avoid "not used" error. */

            afw_object_set_property(test, &afw_s_result,
                evaluated_value, xctx);

            afw_object_set_property(test, &afw_s_passed,
                afw_value_true,
                xctx);

            passed_value =
                !afw_utf8_starts_with(expect, &afw_s_error) &&
                (
                    (!expected_value && !evaluated_value) ||
                    (
                        expected_value &&
                        afw_value_equal(evaluated_value, expected_value, xctx)
                    )
                )
                ? afw_value_true
                : afw_value_false;
            afw_object_set_property(test, &afw_s_passed, passed_value,
                xctx);
        }

        AFW_CATCH_UNHANDLED{

            afw_boolean_t expected_error;

            /* If 'error' is not expected value, set passed false. */
            expected_error = false;
            if (afw_utf8_starts_with(expect, &afw_s_error)) {
                if (expect->len == afw_s_error.len) {
                    expected_error = true;
                }
                else {
                    expect_message.s = expect->s + afw_s_error.len + 1;
                    expect_message.len = expect->len - afw_s_error.len - 1;
                    if (strlen(AFW_ERROR_THROWN->message_z) == expect_message.len &&
                        memcmp(expect_message.s, AFW_ERROR_THROWN->message_z,
                            expect_message.len) == 0)
                    {
                        expected_error = true;
                    }
                }
            }

            /* Set passed only if this is an expected error. */
            passed_value =
                expected_error
                ? afw_value_true
                : afw_value_false;
            afw_object_set_property(test, &afw_s_passed, passed_value, xctx);

            /* errorReason */
            switch (error_in) {
            case error_in_compile_expect:
                errorReason = &afw_s_a_compile_expect_error;
                break;

            case error_in_evaluate_expect:
                errorReason = &afw_s_a_evaluate_expect_error;
                break;

            case error_in_compile_source:
                errorReason = &afw_s_a_compile_source_error;
                break;

            case error_in_evaluate_source:
                errorReason = &afw_s_a_evaluate_source_error;
                break;

            default:
                errorReason = &afw_s_a_other_error;
                break;
            };
            afw_object_set_property_as_string(test,
                &afw_s_errorReason, errorReason, xctx);

            /* Set error property. */
            afw_object_set_property_as_object(test, &afw_s_error,
                afw_error_to_object(AFW_ERROR_THROWN, x->p, xctx), xctx);
        }

        AFW_ENDTRY;
    }

    return (const afw_value_t *)testScriptObject;
}
