// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for call_test_script
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_value_call_test_script.c
 * @brief Implementation of afw_value interface for call_test_script
 */

#include "afw_internal.h"


#define impl_afw_value_optional_release NULL
#define impl_afw_value_get_reference NULL
#define impl_afw_value_get_assignable_value NULL

#define impl_afw_value_get_evaluated_meta \
    afw_value_internal_get_evaluated_meta_default

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_default
    
#define impl_afw_value_create_iterator NULL

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "call_test_script"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_call_test_script_inf
#define AFW_VALUE_SELF_T afw_value_call_test_script_t
#include "afw_value_impl_declares.h"



/* Create function for call value. */
AFW_DEFINE(const afw_value_t *)
afw_value_call_test_script_create(
    const afw_compile_value_contextual_t *contextual,
    const afw_object_t *test_script,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_call_test_script_t *self;

    self = afw_pool_calloc_type(p, afw_value_call_test_script_t, xctx);
    self->inf = &afw_value_call_test_script_inf;
    self->contextual = contextual;
    self->test_script_object_value = 
        (const afw_value_object_t *)
        afw_value_create_unmanaged_object(test_script, p, xctx);

    return &self->pub;
}



/*
 * Install a utf-8 capture stream on a standard stream slot so print/write to
 * that streamId does not touch process FDs (and thus does not pollute the afw
 * CLI result channel). Caller must release after harvesting.
 */
static void
impl_test_script_install_utf8_capture(
    afw_stream_number_t n,
    const afw_utf8_t *streamId,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_stream_t *stream;

    if (xctx->stream_anchor &&
        n < xctx->stream_anchor->maximum_number_of_streams &&
        xctx->stream_anchor->streams[n])
    {
        afw_stream_release(xctx->stream_anchor->streams[n], xctx);
        *(const afw_stream_t **)&xctx->stream_anchor->streams[n] = NULL;
    }

    stream = afw_utf8_stream_create(streamId, p, xctx);
    *(const afw_stream_t **)&xctx->stream_anchor->streams[n] = stream;
}



/*
 * Harvest captured utf-8 for a standard stream, store actual under streamId
 * (stdout/stderr), release the slot, and fail the case if expect does not
 * match. expect NULL means do not assert (caller still may have left a
 * previous FD stream; we only harvest when expect is set).
 */
static void
impl_test_script_check_stream_expect(
    const afw_object_t *test,
    afw_stream_number_t n,
    const afw_utf8_t *streamId,
    const afw_utf8_t *expect,
    const afw_utf8_t *mismatch_reason,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_utf8_t actual;
    const afw_utf8_t *actual_owned;

    if (!expect) {
        return;
    }

    actual.s = NULL;
    actual.len = 0;
    if (xctx->stream_anchor &&
        n < xctx->stream_anchor->maximum_number_of_streams &&
        xctx->stream_anchor->streams[n])
    {
        afw_utf8_stream_get_current_cached_string(
            xctx->stream_anchor->streams[n], &actual, xctx);
        /* Clone before release — capture buffer lives in the stream pool. */
        actual_owned = afw_utf8_clone(&actual, p, xctx);
        afw_stream_release(xctx->stream_anchor->streams[n], xctx);
        *(const afw_stream_t **)&xctx->stream_anchor->streams[n] = NULL;
    }
    else {
        actual_owned = afw_utf8_create(
            (const afw_utf8_octet_t *)"", 0, p, xctx);
    }

    /* Actual capture for harness / failure display. */
    afw_object_set_property_as_string_internal(test,
        afw_value_create_unmanaged_string(streamId, p, xctx),
        actual_owned, xctx);

    if (!afw_utf8_equal(actual_owned, expect)) {
        afw_object_set_property(test, afw_v_passed,
            afw_boolean_v_false, xctx);
        /* Keep compile/eval errorReason if already set. */
        if (!afw_object_has_property(test, afw_v_errorReason, xctx)) {
            afw_object_set_property_as_string_internal(test,
                afw_v_errorReason, mismatch_reason, xctx);
        }
    }
}



/*
 * Implementation of method optional_evaluate for interface afw_value.
 */
const afw_value_t *
impl_afw_value_optional_evaluate(
    AFW_VALUE_SELF_T *self,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    const afw_iterator_old_t *iterator;
    const afw_array_t *tests;
    const afw_object_t *test;
    const afw_value_t *value;
    const afw_utf8_t *default_source_type;
    const afw_utf8_t *source_type;
    const afw_utf8_t *source;
    const afw_utf8_t *expect;
    const afw_utf8_t *expect_stdout;
    const afw_utf8_t *expect_stderr;
    const afw_value_t *expected_value;
    const afw_value_t *compiled_value;
    const afw_value_t *evaluated_value;
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

    tests = afw_object_get_property_as_array_internal(
        self->test_script_object_value->internal, afw_v_tests, p, xctx);
    default_source_type = afw_object_get_property_as_string_internal(
        self->test_script_object_value->internal, afw_v_sourceType, p, xctx);
    if (!default_source_type) {
        default_source_type = afw_s_script;
    }
    for (iterator = NULL;;) {
        value = afw_array_get_next_value(tests, &iterator, p, xctx);
        if (!value) {
            break;
        }
        test = afw_value_as_object_internal(value, xctx);
        /*
         * Array GET may have promoted a face. Result flags belong on the
         * test definition object so a later parent wrap/JSON look-through
         * still sees them.
         */
        test = afw_object_memory_wrapper_base(test);
        source_type = afw_object_get_property_as_string_internal(test,
            afw_v_sourceType, p, xctx);
        if (!source_type) {
            source_type = default_source_type;
        }
        info = afw_compile_type_get_info_by_pneumonic(source_type, xctx);

        expect = afw_object_get_property_as_string_internal(test,
            afw_v_expect, p, xctx);
        if (!expect) {
            AFW_THROW_ERROR_Z(general, "expect required", xctx);
        }

        /*
         * Optional side-channel expects (string equality on captured utf-8).
         * Keys are "expect-stdout" / "expect-stderr" (hyphen; not Adaptive).
         * Absent key → do not assert that stream.
         */
        expect_stdout = afw_object_get_property_as_string_internal(test,
            afw_v_expect_stdout, p, xctx);
        expect_stderr = afw_object_get_property_as_string_internal(test,
            afw_v_expect_stderr, p, xctx);

        expectUTF8OctetLengthInTestScript = afw_object_get_property_as_integer_internal(
            test, afw_v_expectUTF8OctetLengthInTestScript, &found, p, xctx);
        if (!found) {
            AFW_THROW_ERROR_Z(coding_error, "Internal error", xctx);
        }
        expectUTF8OctetOffsetInTestScript = afw_object_get_property_as_integer_internal(
            test, afw_v_expectUTF8OctetOffsetInTestScript, &found, p, xctx);
        if (!found) {
            AFW_THROW_ERROR_Z(coding_error, "Internal error", xctx);
        }

        source = afw_object_get_property_as_string_internal(test,
            afw_v_source, p, xctx);
        if (!source) {
            AFW_THROW_ERROR_Z(general, "source required", xctx);
        }       
        sourceUTF8OctetOffsetInTestScript = afw_object_get_property_as_integer_internal(
            test, afw_v_sourceUTF8OctetOffsetInTestScript, &found, p, xctx);
        if (!found) {
            AFW_THROW_ERROR_Z(coding_error, "Internal error", xctx);
        }
        sourceUTF8OctetLengthInTestScript = afw_object_get_property_as_integer_internal(
            test, afw_v_sourceUTF8OctetLengthInTestScript, &found, p, xctx);
        if (!found) {
            AFW_THROW_ERROR_Z(coding_error, "Internal error", xctx);
        }

        /* Skip processing test is requested. */
        if (afw_object_get_property_as_boolean_internal(test,
            afw_v_skip, &found, p, xctx))
        {
            continue;
        }

        if (info->compile_type == afw_compile_type_error) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "source_type=" AFW_UTF8_FMT_Q " is invalid",
                AFW_UTF8_FMT_ARG(source_type));
        }

        /*
         * Per-case capture: replace standard stdout/stderr with utf-8 buffers
         * when those expects are present so print/println do not hit process
         * FDs (afw CLI keeps Adaptive stdout on stderr for result JSON).
         */
        if (expect_stdout) {
            impl_test_script_install_utf8_capture(
                afw_stream_number_stdout, afw_s_stdout, p, xctx);
        }
        if (expect_stderr) {
            impl_test_script_install_utf8_capture(
                afw_stream_number_stderr, afw_s_stderr, p, xctx);
        }

        AFW_TRY{
            error_in = error_in_other;
            (void)error_in; /* In catch. Avoid "not used" error. */
            if (afw_utf8_starts_with(expect, afw_s_error)) {
                expected_value = NULL;
            }
            else if (afw_utf8_equal(expect, afw_s_success)) {
                /* Completed without throw; ignore the result value. */
                expected_value = NULL;
            }
            else if (afw_utf8_equal(expect, afw_s_undefined)) {
                expected_value = afw_value_undefined;
            }
            else {
                error_in = error_in_compile_expect;
                (void)error_in; /* In catch. Avoid "not used" error. */
                contextual = afw_pool_calloc_type(p,
                    afw_compile_value_contextual_t, xctx);
                afw_memory_copy(contextual, self->contextual);
                contextual->value_offset = afw_safe_cast_integer_to_size(
                    expectUTF8OctetOffsetInTestScript, xctx);
                contextual->value_size = afw_safe_cast_integer_to_size(
                    expectUTF8OctetLengthInTestScript, xctx);
                value = afw_value_create_unmanaged_string(expect, p, xctx);
                argv[0] = (const afw_value_t *)info->compile_function;
                argv[1] = afw_value_convert(value, info->data_type, true,
                    p, xctx);
                compiled_value = afw_value_call_built_in_function(
                    contextual, info->compile_function, 1, argv, p, xctx);
                expected_value = afw_value_evaluate(compiled_value, p, xctx);
                expected_value = afw_value_function_return_value_consume(
                    expected_value, p, xctx);
            }

            error_in = error_in_compile_source;
            (void)error_in; /* In catch. Avoid "not used" error. */
            contextual = afw_pool_calloc_type(
                p, afw_compile_value_contextual_t, xctx);
            afw_memory_copy(contextual, self->contextual);
            contextual->value_offset = afw_safe_cast_integer_to_size(
                sourceUTF8OctetOffsetInTestScript, xctx);
            contextual->value_size = afw_safe_cast_integer_to_size(
                sourceUTF8OctetLengthInTestScript, xctx);
            value = afw_value_create_unmanaged_string(source, p, xctx);
            argv[0] = (const afw_value_t *)info->compile_function;
            argv[1] = afw_value_convert(value, info->data_type, true, p, xctx);
            compiled_value = afw_value_call_built_in_function(
                contextual, info->compile_function, 1, argv, p, xctx);
            error_in = error_in_evaluate_source;
            (void)error_in; /* In catch. Avoid "not used" error. */
            evaluated_value = afw_value_evaluate(compiled_value, p, xctx);
            evaluated_value = afw_value_function_return_value_consume(
                evaluated_value, p, xctx);
            error_in = error_in_other;
            (void)error_in; /* In catch. Avoid "not used" error. */

            /*
             * success: ignore the value (do not put a raw Adaptive value
             * on the test object — JSON cannot write script_function).
             */
            if (afw_utf8_equal(expect, afw_s_success)) {
                afw_object_set_property(test, afw_v_passed,
                    afw_boolean_v_true, xctx);
            }
            else {
                /*
                 * Test objects live in the compile unit. Eval temps
                 * now land on the frame tracker; store the isolated
                 * occupant so last_return clone does not walk a
                 * freed string (0x0BADF00D).
                 */
                afw_object_set_property(test, afw_v_result,
                    afw_value_as_assignable(evaluated_value, xctx),
                    xctx);

                passed_value =
                    !afw_utf8_starts_with(expect, afw_s_error) &&
                    (
                        (!expected_value && !evaluated_value) ||
                        (
                            expected_value &&
                            afw_value_equal(evaluated_value, expected_value,
                                xctx)
                        )
                    )
                    ? afw_boolean_v_true
                    : afw_boolean_v_false;
                afw_object_set_property(test, afw_v_passed, passed_value,
                    xctx);
            }
        }

        AFW_CATCH_UNHANDLED{

            afw_boolean_t expected_error;

            /* If 'error' is not expected value, set passed false. */
            expected_error = false;
            if (afw_utf8_starts_with(expect, afw_s_error)) {
                if (expect->len == afw_self_s_error.len) {
                    expected_error = true;
                }
                else {
                    expect_message.s = expect->s + afw_self_s_error.len + 1;
                    expect_message.len = expect->len - afw_self_s_error.len - 1;
                    if (strlen(AFW_ERROR_THROWN->message_z) ==
                        expect_message.len
                        &&
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
                ? afw_boolean_v_true
                : afw_boolean_v_false;
            afw_object_set_property(test, afw_v_passed, passed_value, xctx);

            /* errorReason */
            switch (error_in) {
            case error_in_compile_expect:
                errorReason = afw_s_a_compile_expect_error;
                break;

            case error_in_evaluate_expect:
                errorReason = afw_s_a_evaluate_expect_error;
                break;

            case error_in_compile_source:
                errorReason = afw_s_a_compile_source_error;
                break;

            case error_in_evaluate_source:
                errorReason = afw_s_a_evaluate_source_error;
                break;

            default:
                errorReason = afw_s_a_other_error;
                break;
            };
            afw_object_set_property_as_string_internal(test,
                afw_v_errorReason, errorReason, xctx);

            /* Set error property. Isolate: object is compile-unit. */
            afw_object_set_property(test, afw_v_error,
                afw_value_as_assignable(
                    afw_value_create_unmanaged_object(
                        afw_error_to_object(AFW_ERROR_THROWN, p, xctx),
                        p, xctx),
                    xctx),
                xctx);
        }

        AFW_ENDTRY;

        /* Side-channel expects after return/error check; always release. */
        impl_test_script_check_stream_expect(test,
            afw_stream_number_stdout, afw_s_stdout,
            expect_stdout, afw_s_a_expect_stdout_mismatch, p, xctx);
        impl_test_script_check_stream_expect(test,
            afw_stream_number_stderr, afw_s_stderr,
            expect_stderr, afw_s_a_expect_stderr_mismatch, p, xctx);
    }

    return afw_value_create_unmanaged_object(
        self->test_script_object_value->internal, p, xctx);
}


/*
 * Implementation of method get_data_type for interface afw_value.
 */
const afw_data_type_t *
impl_afw_value_get_data_type(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    return NULL;
}


/*
 * Implementation of method decompile for interface afw_value.
 */
void
impl_afw_value_produce_compiler_listing(
    AFW_VALUE_SELF_T *self,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p;
    const afw_array_t *tests;
    const afw_object_t *test;
    const afw_object_t *test_script_object;
    const afw_value_t *test_object_value;
    const afw_utf8_t *source;
    const afw_utf8_t *default_source_type;
    const afw_utf8_t *source_type;
    const afw_utf8_t *source_location;
    const afw_iterator_old_t *iterator;
    const afw_compile_type_info_t *info;
    const afw_value_t *compiled_value;
    const afw_utf8_t *test_name;
    afw_size_t line_number;
    afw_size_t column_number;
    afw_compile_value_contextual_t contextual;
    afw_integer_t sourceUTF8OctetOffsetInTestScript;
    afw_integer_t sourceUTF8OctetLengthInTestScript;
    afw_integer_t upToTestsUTF8OctetOffsetInTestScript;
    afw_boolean_t found;
    afw_boolean_t test_begin;

    p = writer->p; // Use writer's pool.
    test_script_object = self->test_script_object_value->internal;
    tests = afw_object_get_property_as_array_internal(
        test_script_object, afw_v_tests, p, xctx);

    /* Make copy of contextual and prime offset up to first test. */
    afw_memory_copy(&contextual, self->contextual);
    upToTestsUTF8OctetOffsetInTestScript =
        afw_object_get_property_as_integer_internal(
            test_script_object, afw_v_upToTestsUTF8OctetOffsetInTestScript,
            &found, p, xctx);
    if (found) {
        contextual.value_offset = afw_safe_cast_integer_to_size(
            upToTestsUTF8OctetOffsetInTestScript, xctx);
    }

    /* Begin listing for test script. */
    afw_value_compiler_listing_begin_value(writer, &self->pub, &contextual, xctx);
    afw_writer_write_z(writer, ": [", xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_increment_indent(writer, xctx);

    afw_writer_write_eol(writer, xctx);
    afw_writer_write_z(writer,
        "// 'test-script-object:' is the test script object without "
        "the 'tests'.",
        xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_write_z(writer,
        "// There is one 'test-object:' for each test.",
        xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_write_z(writer, "test-script-", xctx);
    afw_data_type_object_value_compiler_listing(
        writer, (const afw_value_t *)self->test_script_object_value,
        true, xctx);

    afw_writer_write_eol(writer, xctx);

    default_source_type = afw_object_get_property_as_string_internal(
        self->test_script_object_value->internal, afw_v_sourceType, p, xctx);
    if (!default_source_type) {
        default_source_type = afw_s_script;
    }

    /* Process each test. */
    if (tests) for (iterator = NULL;;) {
        test_object_value = afw_array_get_next_value(tests, &iterator, p, xctx);
        if (!test_object_value) {
            break;
        }
        test_name = NULL;
        test_begin = false;
        AFW_TRY{

            test = afw_value_as_object_internal(test_object_value, xctx);

            test_name = afw_object_get_property_as_string_internal(
                test, afw_v_test, p, xctx);
            if (!test_name) {
                AFW_THROW_ERROR_Z(general, "test required", xctx);
            }

            sourceUTF8OctetOffsetInTestScript =
                afw_object_get_property_as_integer_internal(
                    test, afw_v_sourceUTF8OctetOffsetInTestScript,
                    &found, p, xctx);
            if (!found) {
                AFW_THROW_ERROR_Z(coding_error,
                    "sourceUTF8OctetOffsetInTestScript missing",
                    xctx);
            }
            contextual.value_offset = afw_safe_cast_integer_to_size(
                sourceUTF8OctetOffsetInTestScript, xctx);

            sourceUTF8OctetLengthInTestScript =
                afw_object_get_property_as_integer_internal(
                    test, afw_v_sourceUTF8OctetLengthInTestScript,
                    &found, p, xctx);
            if (!found) {
                AFW_THROW_ERROR_Z(coding_error,
                    "sourceUTF8OctetLengthInTestScript missing",
                    xctx);
            }
            contextual.value_size = afw_safe_cast_integer_to_size(
                sourceUTF8OctetLengthInTestScript, xctx);

            afw_writer_write_eol(writer, xctx);
            afw_writer_write_z(writer, "test-", xctx);
            afw_value_compiler_listing_begin_value(
                writer, test_object_value, &contextual, xctx);
            test_begin = true;
            afw_writer_write_z(writer, ": [", xctx);
            afw_writer_write_eol(writer, xctx);
            afw_writer_increment_indent(writer, xctx);
            afw_value_produce_compiler_listing(
                test_object_value, writer, xctx);

            /* Skip processing test is requested. */
            if (afw_object_get_property_as_boolean_internal(test,
                afw_v_skip, &found, p, xctx))
            {
                afw_writer_write_eol(writer, xctx);
                afw_writer_write_z(writer, "// Test ", xctx);
                afw_writer_write_utf8(writer, test_name, xctx);
                afw_writer_write_z(writer, " skipped", xctx);
                afw_writer_write_eol(writer, xctx);
                break;
            }

            source_type = afw_object_get_property_as_string_internal(test,
                afw_v_sourceType, p, xctx);
            if (!source_type) {
                source_type = default_source_type;
            }
            info = afw_compile_type_get_info_by_pneumonic(
                source_type, xctx);

            source = afw_object_get_property_as_string_internal(
                test, afw_v_source, p, xctx);
            if (!source) {
                AFW_THROW_ERROR_Z(general, "source required", xctx);
            }

            if (info->compile_type == afw_compile_type_error) {
                AFW_THROW_ERROR_FZ(general, xctx,
                    "source_type=" AFW_UTF8_FMT_Q " is invalid",
                    AFW_UTF8_FMT_ARG(source_type));
            }

            afw_utf8_line_column_of_offset(
                &line_number, &column_number,
                self->contextual->compiled_value->full_source,
                contextual.value_offset, 4, xctx);
            source_location = afw_utf8_printf(p, xctx,
                AFW_UTF8_FMT
                "+" AFW_SIZE_T_FMT
                "(" AFW_SIZE_T_FMT
                ":" AFW_SIZE_T_FMT ")"
                " test: " AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(self->contextual->source_location),
                contextual.value_offset, line_number, column_number,
                AFW_UTF8_FMT_ARG(test_name));
            compiled_value = afw_compile_to_value(
                source, source_location, info->compile_type, NULL, NULL,
                xctx->p, xctx);
            source_location = source_location;
            compiled_value = compiled_value;
            afw_value_produce_compiler_listing(
                compiled_value, writer, xctx);
        }

        AFW_CATCH_UNHANDLED{
            if (test_name) {
                afw_writer_write_z(writer, "\n\n---Test ", xctx);
                afw_writer_write_utf8(writer, test_name, xctx);
                afw_writer_write_z(writer, " has error:\n\n", xctx);
            }
            else {
                afw_writer_write_z(
                    writer, "\n\n--Test name missing:\n\n", xctx);
            }
            afw_writer_write_utf8(
                writer, afw_error_to_utf8(AFW_ERROR_THROWN, p, xctx), xctx);
            afw_writer_write_z(
                    writer, "\n\n---\n", xctx);
            afw_writer_write_eol(writer, xctx);
        }

        AFW_FINALLY {
            if (test_begin) {
                afw_writer_decrement_indent(writer, xctx);
                afw_writer_write_z(writer, "]", xctx);
                afw_writer_write_eol(writer, xctx);
            }
        }

        AFW_ENDTRY;
    }

    afw_writer_decrement_indent(writer, xctx);
    afw_writer_write_z(writer, "]", xctx);
    afw_writer_write_eol(writer, xctx);
}



/*
 * Implementation of method decompile for interface afw_value.
 *
 * Synthetic call #call_test_script(test_script_object).
 */
void
impl_afw_value_decompile(
    AFW_VALUE_SELF_T *self,
    const afw_writer_t * writer,
    afw_xctx_t *xctx)
{
    const afw_value_t *argv[1];

    afw_value_decompile_write_synthetic_function_name(&self->pub, writer, xctx);
    argv[0] = self->test_script_object_value
        ? (const afw_value_t *)self->test_script_object_value
        : NULL;
    afw_value_decompile_value_list(writer, 1, argv, xctx);
}


/*
 * Implementation of method get_info for interface afw_value.
 */
void
impl_afw_value_get_info(
    AFW_VALUE_SELF_T *self,
    afw_value_info_t *info,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{

    afw_memory_clear(info);
    info->detail = afw_s_test_script;
    info->value_inf_id = &self->pub.inf->rti.implementation_id;
    info->contextual = self->contextual;
    info->evaluated_data_type = afw_data_type_object;
    info->optimized_value = &self->pub;

    /* Note: Maybe something can be done for optimized_value_data_type. */
}
