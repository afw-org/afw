// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Action
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_action.c
 * @brief Functions for working with _AdaptiveAction_ objects.
 */

#include "afw_internal.h"

static void
impl_call_function(
    const afw_compile_value_contextual_t *contextual,
    const afw_value_function_definition_t *function,
    const afw_object_t *parent_request,
    const afw_object_t *action_entry,
    const afw_content_type_t *response_content_type,
    const afw_object_t *action_response_entry,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_value_function_parameter_t *const *a;
    afw_size_t argc, i;
    const afw_data_type_t *data_type;
    const afw_value_t **argv;
    const afw_value_t *args;
    const afw_value_t *result;
    const afw_value_t *const *vargs;
    const afw_value_t *const *varg;
    const afw_value_t *value;
    const afw_utf8_t *const *flag_ids;
    afw_value_info_t info;
    afw_boolean_t minArgs_found;

    /* Determine argc. */
    for (argc = 0,
        a = function->parameters,
        varg = NULL,
        vargs = NULL,
        minArgs_found = false;
        *a;
        a++)
    {
        if (minArgs_found) {
            AFW_THROW_ERROR_Z(general,
                "minArgs must be on last parameter", xctx);
        }
        if ((*a)->minArgs == -1) {
            argc++;
        }
        else {
            minArgs_found = true;
            args = afw_object_get_property(action_entry,
                &(*a)->name, xctx);
            if (!args && parent_request) {
                args = afw_object_get_property(parent_request,
                    &(*a)->name, xctx);
            }
            if (args) {
                vargs = afw_value_as_array_of_values(args, p, xctx);
                for (varg = vargs; *varg; varg++) {
                    argc++;
                }
            }
        }
    }

    /* Build argv. */
    argv = afw_pool_calloc(p, sizeof(afw_value_t *) * (argc + 1), xctx);
    argv[0] = (const afw_value_t *)function;
    for (i = 1, a = function->parameters; i <= argc; i++, a++) {

        /* If this is not minArgs parameter ... */
        if (*a && (*a)->minArgs == -1) {
            argv[i] = afw_object_get_property(action_entry, &(*a)->name,
                xctx);
            if (argv[i]) {
                if (afw_value_is_object(argv[i]) && parent_request) {
                    value = afw_object_get_property(parent_request,
                        &(*a)->name, xctx);
                    if (afw_value_is_object(value)) {
                        argv[i] = afw_value_create_object(
                            afw_object_create_merged(
                            ((const afw_value_object_t *)value)->
                                internal,
                                ((const afw_value_object_t *)argv[i])->
                                internal,
                                p, xctx), p, xctx);
                    }
                }
            }
            else if (parent_request) {
                argv[i] = afw_object_get_property(parent_request,
                    &(*a)->name, xctx);
            }

            /* If arg's data type doesn't match declared value, convert. */
            if (argv[i]) {
                if ((*a)->data_type &&
                    (*a)->data_type != afw_value_quick_data_type(argv[i]))
                {
                    argv[i] = afw_value_convert(argv[i],
                        (*a)->data_type, true, p, xctx);
                }
            }

            /* If arg is NULL and not optional, throw error. */
            else if (!(*a)->optional) {
                AFW_THROW_ERROR_FZ(general, xctx,
                    "Missing parameter " AFW_UTF8_FMT,
                    AFW_UTF8_FMT_ARG(&(*a)->name));
            }
        }

        /* If this is minArgs parameter, move remaining to argv.  */
        else if (vargs) {
            for (varg = vargs; *varg; varg++, i++) {
                argv[i] = *varg;

                /* If arg's data type doesn't match declared, convert. */
                if (*a && (*a)->data_type &&
                    (*a)->data_type != afw_value_quick_data_type(argv[i]))
                {
                    argv[i] = afw_value_convert(argv[i],
                        (*a)->data_type, true, p, xctx);
                }
            }
        }
    }

    /* Set flags. */
    value = afw_object_get_property(action_entry, &afw_s__flags_, xctx);
    if (!value && parent_request) {
        value = afw_object_get_property(parent_request, &afw_s__flags_, xctx);
    }
    if (value) {
        flag_ids = afw_value_as_array_of_utf8(value, p, xctx);
        afw_flag_set_to_defaults_plus_array(flag_ids, xctx);
    }

    /* Call function. */
    result = afw_value_call_built_in_function(
        contextual, function, argc, argv, p, xctx);
    
    /* Set result if not undefined. */
    if (result) {
            
        /* Make sure result if evaluated. */
        if (!afw_value_is_fully_evaluated(result, xctx)) {
            afw_value_get_info(result, &info, p, xctx);
            AFW_THROW_ERROR_FZ(general, xctx,
                "Function " AFW_UTF8_FMT
                " returned a value that is not evaluated. ("
                AFW_UTF8_FMT " " AFW_UTF8_FMT ")",
                AFW_UTF8_FMT_ARG(&function->functionId),
                AFW_UTF8_FMT_ARG(info.value_inf_id),
                AFW_UTF8_FMT_OPTIONAL_ARG(info.detail)
            );
        }

        /* Set result property in result. */
        afw_object_set_property(action_response_entry, &afw_s_result, result,
            xctx);

        /* Set resultDataType property in result. */
        data_type = afw_value_quick_data_type(result);
        if (data_type) {
            afw_object_set_property_as_string(action_response_entry,
                &afw_s_resultDataType, &data_type->data_type_id, xctx);
        }
    }
}


static void
impl_add_stream_properties(
    const afw_object_t *response,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_utf8_t string;

    if (afw_flag_by_id_is_active(&afw_s_a_flag_response_console, xctx) &&
        !afw_flag_by_id_is_active(&afw_s_a_flag_response_console_stream, xctx) &&
        afw_stream_standard_is_set(console, xctx))
    {
        afw_utf8_stream_get_current_cached_string(
            afw_stream_standard(console, xctx),
            &string, xctx);
        afw_object_set_property_as_string(response, &afw_s_console,
            afw_utf8_clone(&string, p, xctx), xctx);
        afw_stream_standard_release(console, xctx);
    }

    if (afw_flag_by_id_is_active(&afw_s_a_flag_response_stderr, xctx) &&
        !afw_flag_by_id_is_active(&afw_s_a_flag_response_stderr_stream, xctx) &&
        afw_stream_standard_is_set(stderr, xctx))
    {
        afw_utf8_stream_get_current_cached_string(
            afw_stream_standard(stderr, xctx),
            &string, xctx);
        afw_object_set_property_as_string(response, &afw_s_stderr,
            afw_utf8_clone(&string, p, xctx), xctx);
        afw_stream_standard_release(stderr, xctx);
    }

    if (afw_flag_by_id_is_active(&afw_s_a_flag_response_stdout, xctx) &&
        !afw_flag_by_id_is_active(&afw_s_a_flag_response_stdout_stream, xctx) &&
        afw_stream_standard_is_set(stdout, xctx))
    {
        afw_utf8_stream_get_current_cached_string(
            afw_stream_standard(stdout, xctx),
            &string, xctx);
        afw_object_set_property_as_string(response, &afw_s_stdout,
            afw_utf8_clone(&string, p, xctx), xctx);
        afw_stream_standard_release(stdout, xctx);
    }
}



/* Perform actions(s) specified in _AdaptiveActions_ object. */
AFW_DEFINE(const afw_object_t *)
afw_action_perform(
    const afw_object_t *request,
    const afw_content_type_t *response_content_type,
    const afw_object_t *response,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_list_t *actions;
    const afw_object_t *action_entry;
    const afw_list_t *action_response_entries;
    const afw_object_t *action_response_entry;
    const afw_value_t *action_response_entry_value;
    const afw_utf8_t *functionId;
    const afw_value_function_definition_t *function;
    const afw_utf8_t *name;
    afw_integer_t action_number = 0;
    const afw_value_t *value;
    const afw_iterator_t *iterator;
    const afw_object_t *error;
    const afw_stream_t *stream;
    afw_compile_value_contextual_t *contextual;


    /* If existing response not passed, make a new one. */
    if (!response) {
        response = afw_object_create_managed(p, xctx);
    }

    contextual = afw_pool_calloc_type(response->p,
        afw_compile_value_contextual_t, xctx);
    action_response_entry = NULL;

    AFW_TRY{

        /* Get actions property.  If it doesn't exist, process single function. */
        name = &afw_s_actions;
        value = afw_object_get_property(request, name, xctx);
        if (!value) {
            name = &afw_s_function;
            value = afw_object_get_property(request, name, xctx);
            if (!value) {
                AFW_THROW_ERROR_Z(syntax,
                    "Either actions or function property must be specified",
                    xctx);
            }
            else {
                functionId = afw_value_as_utf8(value, response->p, xctx);

                /* Get function. */
                function = afw_environment_get_function(functionId, xctx);
                if (!function) {
                    AFW_THROW_ERROR_FZ(syntax, xctx,
                        "Unknown function " AFW_UTF8_FMT,
                        AFW_UTF8_FMT_ARG(functionId));
                }

                /* Call function. */
                contextual->source_location = &afw_s_action;
                impl_call_function(contextual, function, NULL,
                    request, response_content_type, response,
                    response->p, xctx);

                afw_object_set_property_as_string(response, &afw_s_status,
                    &afw_s_success, xctx);

                break;
            }
        }

        /* Make sure actions property is multiple actions list. */
        if (!afw_value_is_list(value)) {
            AFW_THROW_ERROR_FZ(syntax, xctx,
                "Property " AFW_UTF8_FMT " of actions is missing or invalid",
                AFW_UTF8_FMT_ARG(name));
        }
        actions = ((const afw_value_list_t *)value)->internal;

        /* Create list for action response entries if not application/x-afw. */
        action_response_entries = NULL;
        if (!afw_content_type_is_application_afw(response_content_type)) {
            action_response_entries = afw_list_of_create(
                afw_data_type_object, response->p, xctx);
            afw_object_set_property_as_list(response, &afw_s_actions,
                action_response_entries, xctx);
        }

        /* Process each action. */
        for (iterator = NULL, action_number = 1; ; action_number++) {

            /* Get next action.  If there are not more, break. */
            value = afw_list_get_next_value(actions, &iterator,
                response->p, xctx);
            if (!value) break;

            /* Action must be a single_object. */
            if (!afw_value_is_object(value)) {
                AFW_THROW_ERROR_Z(syntax,
                    "Entries in actions list must be objects",
                    xctx);
            }
            action_entry = ((const afw_value_object_t *)value)->internal;

            /* Create action result entry. */
            if (afw_content_type_is_application_afw(response_content_type)) {
                action_response_entry = afw_object_create_managed(
                    response->p, xctx);
            }
            else {
                action_response_entry = afw_object_create(
                    response->p, xctx);
            }
            action_response_entry_value = afw_value_create_object(
                action_response_entry, response->p, xctx);
            if (!afw_content_type_is_application_afw(response_content_type)) {
                afw_list_add_value(action_response_entries,
                    action_response_entry_value, xctx);
            }

            /* Get required function. */
            name = &afw_s_function;
            value = afw_object_get_property(action_entry, name, xctx);
            if (!value) {
                value = afw_object_get_property(request, name, xctx);
            }
            functionId = afw_value_as_utf8(value, action_response_entry->p, xctx);
            if (!functionId) {
                AFW_THROW_ERROR_FZ(syntax, xctx,
                    "Property " AFW_UTF8_FMT " of action " AFW_INTEGER_FMT
                    " is missing or invalid",
                    AFW_UTF8_FMT_ARG(name), action_number);
            }

            /* Get function. */
            function = afw_environment_get_function(functionId, xctx);
            if (!function) {
                AFW_THROW_ERROR_FZ(syntax, xctx,
                    "Unknown function " AFW_UTF8_FMT
                    " in action " AFW_INTEGER_FMT,
                    AFW_UTF8_FMT_ARG(functionId), action_number);
            }

            /* Call function. */
            contextual->source_location =
                afw_utf8_printf(response->p, xctx,
                "action%d", action_number);
            impl_call_function(contextual, function, request,
                action_entry, response_content_type, action_response_entry,
                response->p, xctx);

            /* if application/x-afw, write response. */
            if (afw_content_type_is_application_afw(response_content_type)) {
                impl_add_stream_properties(action_response_entry, p, xctx);
                stream = afw_stream_standard(response_body, xctx);
                afw_object_set_property(action_response_entry,
                    &afw_s_intermediate, afw_value_true, xctx);
                afw_content_type_write_value(
                    response_content_type,
                    action_response_entry_value,
                    NULL, (void *)stream, stream->write_cb,
                    p, xctx);
                afw_stream_flush(stream, xctx);
                afw_object_release(action_response_entry, xctx);
            }
        }

        /* Set status to success. */
        afw_object_set_property_as_string(response, &afw_s_status,
            &afw_s_success, xctx);
    }

    AFW_CATCH_UNHANDLED{

        /* Add error to response object. */
        afw_object_set_property_as_string(response,
            &afw_s_status, &afw_s_error, xctx);
        error = afw_object_create_embedded(response, &afw_s_error, xctx);
        afw_error_add_to_object(error, &this_THROWN_ERROR, xctx);

        /* If multiple actions, add actionNumber to error. */
        if (action_number > 0) {
            afw_object_set_property_as_integer(error,
                &afw_s_actionNumber, action_number, xctx);
        }
    }

    AFW_FINALLY{
        impl_add_stream_properties(response, p, xctx);
    }

    AFW_ENDTRY;

    return response;
}

