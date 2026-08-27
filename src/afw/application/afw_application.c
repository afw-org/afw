// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Application
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_application.c
 * @brief Application conf, qualified variables, and startup.
 */

#include "afw_internal.h"


static afw_utf8_t
impl_s_a_onApplicationStartupComplete_source_location =
    AFW_UTF8_LITERAL("Application/onApplicationStartupComplete");


/* Conf object callback. */
static afw_boolean_t
impl_conf_object_cb(
    const afw_object_t *object,
    void *context,
    afw_xctx_t *xctx)
{
    if (object) {
        object = afw_object_view_create(object, NULL,
            &afw_object_options_composite_normalize_defaults_required,
            object->p, xctx);
        object = afw_object_create_clone(object, object->p, xctx);
    }

    *((const afw_object_t * *)context) = object;

    /* Return indicating not to short circuit */
    return false;
}


/*
 * Note: Make sure to update
 * afw_application_internal_register_basic_application_context_type()
 * with parallel changes to this function.
 *
 * get_cb: C NULL = not this frame; non-NULL = defined here (use
 * afw_value_undefined for present undefined — permanent singleton).
 */
static const afw_value_t *
impl_current_get_variable_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    const afw_value_t *result;
    afw_integer_t pid;

    (void)entry;

    result = NULL;
    if (afw_utf8_equal(name, afw_s_mode)) {
        /* Always defined on app current; mode is set at env create. */
        result = xctx->mode ? xctx->mode : afw_value_undefined;
    }
    else if (afw_utf8_equal(name, afw_s_pid)) {
        pid = afw_os_get_pid();
        result = afw_value_create_unmanaged_integer(pid, xctx->p, xctx);
    }
    else if (afw_utf8_equal(name, afw_s_xctxUUID)) {
        result = afw_value_create_unmanaged_string(xctx->uuid, xctx->p, xctx);
    }
    else if (afw_utf8_equal(name, afw_s_programName)) {
        result = afw_value_create_unmanaged_string(
            &xctx->env->program_name, xctx->p, xctx);
    }

    return result;
}


static void
impl_current_contribute_variables_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_object_t *object,
    afw_boolean_t include_untrusted,
    afw_xctx_t *xctx)
{
    static const afw_utf8_t * const names[] = {
        afw_s_mode,
        afw_s_pid,
        afw_s_xctxUUID,
        afw_s_programName,
        NULL
    };
    const afw_utf8_t * const *np;
    const afw_value_t *value;

    (void)include_untrusted;

    for (np = names; *np; np++) {
        if (afw_object_has_property(object,
            afw_value_create_unmanaged_string(*np, object->p, xctx), xctx))
        {
            continue;
        }
        value = impl_current_get_variable_cb(entry, *np, xctx);
        if (value) {
            afw_object_set_property(object,
                afw_value_create_unmanaged_string(*np, object->p, xctx),
                value, xctx);
        }
        else {
            afw_object_set_property(object,
                afw_value_create_unmanaged_string(*np, object->p, xctx),
                afw_value_undefined, xctx);
        }
    }
}



void
afw_application_internal_push_qualifiers(afw_xctx_t *xctx)
{
    const afw_environment_t *env = xctx->env;

    /* Push current:: qualifier. */
    afw_xctx_qualifier_stack_qualifier_push(afw_s_current, NULL, true,
        impl_current_get_variable_cb, impl_current_contribute_variables_cb,
        NULL, xctx->p, xctx);

    /*
     * Process ambient qualifiers (environment::, process::). Objects are
     * created once at environment create; push only (issue #71).
     */
    if (env->environment_variables_object) {
        afw_xctx_qualifier_stack_qualifier_object_push(afw_s_environment,
            env->environment_variables_object, true, xctx->p, xctx);
    }
    if (env->process_object) {
        afw_xctx_qualifier_stack_qualifier_object_push(afw_s_process,
            env->process_object, true, xctx->p, xctx);
    }

    /* If there is an application qualifier, push application qualifier. */
    if (env->application_object) {
        afw_xctx_qualifier_stack_qualifier_object_push(afw_s_application,
            env->application_object, true, xctx->p, xctx);
    }

    /* If there are qualified application variables, push qualifiers. */
    if (env->application_qualified_variables) {
        afw_xctx_qualifier_stack_qualifiers_object_push(env->application_qualified_variables,
            true, xctx->p, xctx);
    }
}



/*
 * Set basic application context type
 *
 * Note: Make sure to update impl_current_get_variable_cb()
 * with parallel changes to this function.
 */
void
afw_application_internal_register_basic_application_context_type(
    afw_xctx_t *xctx)
{
    const afw_object_t *process_context_type_object;
    const afw_object_t *context_type_object;
    const afw_object_t *qualifier_definitions;
    const afw_object_t *variable_definitions;

    /*
     * Context type "process": ambient process-level qualifiers available
     * after environment create (environment:: and process::).
     */
    process_context_type_object = afw_context_type_create(
        afw_s_process, xctx->env->p, xctx);
    afw_object_set_property_as_string_from_utf8_z(
        process_context_type_object, afw_v_description,
        "Process-level qualifiers available after environment create: "
        "environment:: (process environment variables) and process:: "
        "(invocation: args, programName). Distinct from request::.",
        xctx);

    /* environment:: — open-ended process env names. */
    variable_definitions =
        afw_context_type_insure_variable_definitions_object_exists(
            process_context_type_object, afw_s_environment, xctx);
    afw_context_variable_definition_add_z(variable_definitions,
        afw_s_HOME, afw_s_runtime,
        &afw_value_unmanaged_string_inf,
        "HOME",
        "Example process environment variable. Any process env name is "
        "available as environment::<name>.",
        NULL, NULL,
        xctx);

    /* process:: — invocation and process identity. */
    variable_definitions =
        afw_context_type_insure_variable_definitions_object_exists(
            process_context_type_object, afw_s_process, xctx);
    afw_context_variable_definition_add_z(variable_definitions,
        afw_s_args, afw_s_runtime,
        &afw_value_unmanaged_array_inf,
        "Arguments",
        "Command-line arguments (array of string) from process start. "
        "Use length(process::args) for the count.",
        "string", NULL,
        xctx);
    afw_context_variable_definition_add_z(variable_definitions,
        afw_s_programName, afw_s_runtime,
        &afw_value_unmanaged_string_inf,
        "Program Name",
        "Base name of args[0] (e.g. afw, afwfcgi).",
        NULL, NULL,
        xctx);
    afw_context_variable_definition_add_z(variable_definitions,
        afw_s_pid, afw_s_runtime,
        &afw_value_unmanaged_integer_inf,
        "Pid",
        "Operating system process id at environment create.",
        NULL, NULL,
        xctx);
    afw_context_variable_definition_add_z(variable_definitions,
        afw_s_cwd, afw_s_runtime,
        &afw_value_unmanaged_string_inf,
        "Working Directory",
        "Working directory snapshot at environment create.",
        NULL, NULL,
        xctx);
    afw_context_variable_definition_add_z(variable_definitions,
        afw_s_afwVersion, afw_s_runtime,
        &afw_value_unmanaged_string_inf,
        "AFW Version",
        "Active libafw version string.",
        NULL, NULL,
        xctx);
    afw_context_variable_definition_add_z(variable_definitions,
        afw_s_startTime, afw_s_runtime,
        &afw_value_unmanaged_dateTime_inf,
        "Start Time",
        "Local dateTime when the Adaptive environment was created.",
        NULL, NULL,
        xctx);

    afw_environment_register_context_type(afw_s_process,
        process_context_type_object, xctx);

    /* Context type "application" — includes process ambient via parentPaths. */
    context_type_object = afw_context_type_create(
        afw_s_application, xctx->env->p, xctx);

    qualifier_definitions =
        afw_context_type_insure_qualifier_definitions_object_exists(
            context_type_object, xctx);
    afw_object_meta_add_parent_path(qualifier_definitions,
        afw_s_a_context_type_process_qualifier_definitions_path,
        xctx);

    variable_definitions =
        afw_context_type_insure_variable_definitions_object_exists(
            context_type_object, afw_s_current, xctx);

    afw_context_variable_definition_add_z(variable_definitions,
        afw_s_mode, afw_s_runtime,
        &afw_value_unmanaged_string_inf,
        "Authorization Mode",
        "The current authorization mode.",
        NULL, NULL,
        xctx);

    afw_context_variable_definition_add_z(variable_definitions,
        afw_s_pid, afw_s_runtime,
        &afw_value_unmanaged_integer_inf,
        "Pid",
        "Deprecated: use process::pid. Process id (still provided on current:: for compatibility).",
        NULL, NULL,
        xctx);

    afw_context_variable_definition_add_z(variable_definitions,
        afw_s_programName, afw_s_runtime,
        &afw_value_unmanaged_string_inf,
        "Program Name",
        "Deprecated: use process::programName. Program base name (still provided on current:: for compatibility).",
        NULL, NULL,
        xctx);

    afw_context_variable_definition_add_z(variable_definitions,
        afw_s_xctxUUID, afw_s_runtime,
        &afw_value_unmanaged_string_inf,
        "XCTX UUID",
        "The execution context (xctx) UUID which can normally be considered the UUID of the current request.",
        NULL, NULL,
        xctx);

    afw_environment_register_context_type(afw_s_application,
        context_type_object, xctx);
}



void
afw_application_internal_application_conf_type_create_cede_p(
    const afw_utf8_t *type,
    const afw_object_t *entry,
    const afw_utf8_t *source_location,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_environment_t *env = (afw_environment_t *)xctx->env;
    const afw_utf8_t *application_id;
    const afw_utf8_t *conf_adapter_id;
    const afw_adapter_session_t *session;
    const afw_iterator_old_t *iterator;
    const afw_value_t *value;
    const afw_value_t *entry_value;
    const afw_value_t *property_name;
    const afw_object_t *properties;
    const afw_adapter_t *layout_adapter;
    const afw_object_t *context_type_object;
    const afw_object_t *variable_definitions_object;
    const afw_array_t *default_flags;
    const afw_utf8_t * const *extension_id;
    const afw_value_t * const *module_path_values;
    const afw_value_t *evaluated;
    const afw_utf8_t *module_path;
    const afw_utf8_t *full_path;
    const afw_object_t *root_file_paths;
    const afw_object_t *normalized_root_file_paths;
    const afw_utf8_t *detail_source_location;
    const afw_object_t *object;
    const afw_utf8_t *s;
    afw_boolean_t error;

    /* Only one application conf allowed. */
    if (env->application_object) {
        AFW_THROW_ERROR_FZ(general, xctx,
            AFW_UTF8_CONTEXTUAL_LABEL_FMT
            "type=application conf entry already specified.",
            AFW_UTF8_FMT_ARG(source_location));
    }

    /* Log application starting. */
    AFW_LOG_Z(info, "Application starting.", xctx);

    /* Get optional confAdapterId. */
    conf_adapter_id = afw_object_old_get_property_as_utf8(
        entry, afw_v_confAdapterId, p, xctx);

    /* Get conf adapter.  It will not ever be released. */
    if (conf_adapter_id) {
        AFW_LOG_FZ(debug, xctx,
            "Application specified confAdapterId " AFW_UTF8_FMT
            ".",
            AFW_UTF8_FMT_ARG(conf_adapter_id));
        env->conf_adapter = afw_adapter_get_reference(conf_adapter_id, xctx);
        afw_adapter_impl_set_supported_core_object_type(env->conf_adapter,
            afw_s__AdaptiveServiceConf_, true, true, xctx);
        afw_adapter_impl_set_supported_core_object_type(env->conf_adapter,
            afw_s__AdaptiveConf_application, true, true, xctx);
        afw_adapter_impl_set_supported_core_object_type(env->conf_adapter,
            afw_s__AdaptiveTemplateProperties_, false, true, xctx);
    }

    /* Get optional applicationId and default to "application". */
    application_id = afw_object_old_get_property_as_utf8(entry,
        afw_v_applicationId, p, xctx);
    if (!application_id) {
        application_id = afw_s_Adaptive;
        afw_object_set_property_as_string(entry,
            afw_v_applicationId, application_id, xctx);
    }
    ((afw_environment_t *)env)->application_id.len = application_id->len;
    ((afw_environment_t *)env)->application_id.s = application_id->s;

    /*
     * If conf adapter, merge in the _AdaptiveConf_application application_id
     * object to entry.
     */
    if (conf_adapter_id) {
        object = NULL;
        error = false;
        session = afw_adapter_session_create(conf_adapter_id, xctx);
        AFW_TRY {
            afw_adapter_session_get_object(
                session, NULL,
                afw_s__AdaptiveConf_application, application_id,
                (void *)&object, impl_conf_object_cb, NULL, p, xctx);
        }

        AFW_CATCH(not_found) {
            /* Okay, object is NULL. */
        }

        AFW_CATCH_UNHANDLED{
            error = true;
            AFW_LOG_FZ(err, xctx,
                AFW_UTF8_CONTEXTUAL_LABEL_FMT
                "%s",
                AFW_UTF8_FMT_ARG(source_location),
                AFW_ERROR_THROWN->message_z);
        }

        AFW_FINALLY{
            afw_adapter_session_release(session, xctx);
        }

        AFW_ENDTRY;

        /* If there was an unexpected error, throw error. */
        if (error) {
            AFW_THROW_ERROR_FZ(general, xctx,
                AFW_UTF8_CONTEXTUAL_LABEL_FMT
                "error logged _AdaptiveService_",
                AFW_UTF8_FMT_ARG(source_location));
        }

        if (object) {
            for (iterator = NULL;;) {
                value = afw_object_get_next_property(object, &iterator,
                    &property_name, xctx);
                if (!value) {
                    break;
                }
                entry_value = afw_object_get_property(entry,
                    property_name, xctx);
                if (entry_value ||
                    afw_value_equal(property_name, afw_v_confAdapterId, xctx) ||
                    afw_value_equal(property_name, afw_v_applicationId, xctx))
                {
                    if (!afw_value_equal(value, entry_value, xctx)) {
                        AFW_LOG_FZ(warning, xctx,
                            AFW_UTF8_CONTEXTUAL_LABEL_FMT
                            "configuration type \"application\" ignored "
                            "/" AFW_UTF8_FMT
                            "/_AdaptiveConf_application/" AFW_UTF8_FMT
                            "." AFW_UTF8_FMT
                            " because it is allowWrite=false or specified "
                            " in the conf file.",
                            AFW_UTF8_FMT_ARG(source_location),
                            AFW_UTF8_FMT_ARG(conf_adapter_id),
                            AFW_UTF8_FMT_ARG(application_id),
                            AFW_UTF8_FMT_ARG(
                                afw_object_property_name_display_utf8(
                                    property_name, xctx)));
                    }
                }
                else {
                    afw_object_set_property(entry, property_name, value, xctx);
                }
            }
        }
    }

    /* Prepare/validate application conf. */
    properties = afw_environment_prepare_conf_type_properties(entry, xctx);
    env->application_object = properties;
    afw_object_meta_set_ids(env->application_object, afw_s_afw,
        afw_s__AdaptiveApplication_, afw_s_current, xctx);
    afw_runtime_env_set_object(env->application_object, false, xctx);

    /* If extensions specified, load them. */
    value = afw_object_get_property(env->application_object,
        afw_v_extensions, xctx);
    if (value) {
        for (extension_id = afw_value_as_array_of_utf8(value, p, xctx);
            *extension_id;
            extension_id++)
        {
            afw_environment_load_extension(*extension_id, NULL, NULL, xctx);
        }
    }

    /* If extensionModulePaths specified, load them. */
    value = afw_object_get_property(env->application_object,
        afw_v_extensionModulePaths, xctx);
    if (value) {
        detail_source_location = afw_utf8_printf(p, xctx,
            AFW_UTF8_FMT "/" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(source_location),
            AFW_UTF8_FMT_ARG(afw_s_extensionModulePaths));
        for (module_path_values = afw_value_as_array_of_values(value, p, xctx);
            *module_path_values;
            module_path_values++)
        {
            evaluated = afw_value_compile_and_evaluate_as(
                *module_path_values, detail_source_location,
                afw_compile_type_template, p, xctx);
            if (!afw_value_is_string(evaluated)) {
                AFW_THROW_ERROR_FZ(general, xctx,
                    AFW_UTF8_CONTEXTUAL_LABEL_FMT
                    "extensionModulePaths entries must evaluate to string",
                    AFW_UTF8_FMT_ARG(detail_source_location));
            }
            module_path =
                &((const afw_value_string_t *)evaluated)->internal;
            afw_environment_load_extension(NULL, module_path, NULL, xctx);
        }
    }

    /* Make application context object. */
    context_type_object = afw_environment_get_context_type(afw_s_application,
        xctx);

    /*
     * rootFilePaths: evaluate each host directory as a template and resolve
     * to a full path at application create (issue #15).
     */
    root_file_paths = afw_object_old_get_property_as_object(
        env->application_object, afw_v_rootFilePaths, xctx);
    env->root_file_paths = NULL;
    if (root_file_paths) {
        detail_source_location = afw_utf8_printf(p, xctx,
            AFW_UTF8_FMT "/" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(source_location),
            AFW_UTF8_FMT_ARG(afw_s_rootFilePaths));
        normalized_root_file_paths = afw_object_and_pool_create(p, xctx);
        for (iterator = NULL;;) {
            value = afw_object_get_next_property(root_file_paths, &iterator,
                &property_name, xctx);
            if (!value) {
                break;
            }
            evaluated = afw_value_compile_and_evaluate_as(
                value, detail_source_location,
                afw_compile_type_template, p, xctx);
            if (!afw_value_is_string(evaluated)) {
                AFW_THROW_ERROR_FZ(general, xctx,
                    AFW_UTF8_CONTEXTUAL_LABEL_FMT
                    "rootFilePaths." AFW_UTF8_FMT
                    " must evaluate to string",
                    AFW_UTF8_FMT_ARG(detail_source_location),
                    AFW_UTF8_FMT_ARG(
                        afw_object_property_name_display_utf8(
                            property_name, xctx)));
            }
            full_path = afw_file_insure_full_path(
                &((const afw_value_string_t *)evaluated)->internal,
                p, xctx);
            afw_object_set_property_as_string(normalized_root_file_paths,
                property_name, full_path, xctx);
        }
        env->root_file_paths = normalized_root_file_paths;
    }

    /* defaultFlags */
    default_flags = afw_object_old_get_property_as_array(env->application_object,
        afw_v_defaultFlags, xctx);
    if (default_flags) {
        afw_flag_set_default_flag_ids(default_flags, xctx);
    }

    /* application:: variable definitions. */
    variable_definitions_object =
        afw_context_type_insure_variable_definitions_object_exists(
        context_type_object, afw_s_application, xctx);
    afw_context_variable_definitions_add_based_on_object(
        variable_definitions_object, env->application_object, xctx);

    /* qualifiedVariables definitions. */
    env->application_qualified_variables = afw_object_old_get_property_as_object(
        env->application_object, afw_v_qualifiedVariables, xctx);
    if (env->application_qualified_variables) {
        detail_source_location = afw_utf8_printf(
            env->application_qualified_variables->p, xctx,
            AFW_UTF8_FMT "/" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(source_location),
            AFW_UTF8_FMT_ARG(afw_s_qualifiedVariables));
        env->application_qualified_variables = afw_object_create_clone(
            env->application_qualified_variables, p, xctx);
        afw_context_variable_definitions_compile_and_add_based_on_qualifiers_object(
            context_type_object, env->application_qualified_variables,
            detail_source_location, xctx);
    }

    /* Get optional layoutAdapterId. */
    env->layout_adapter_id = afw_object_old_get_property_as_utf8(
        properties, afw_v_layoutsAdapterId, p, xctx);

    /* Set supported core object type in adapter. */
    if (env->layout_adapter_id) {
        AFW_LOG_FZ(debug, xctx,
            "Application specified layoutAdapterId " AFW_UTF8_FMT_Q
            ".",
            AFW_UTF8_FMT_ARG(env->layout_adapter_id));
        layout_adapter = afw_adapter_get_reference(env->layout_adapter_id, xctx);
        afw_adapter_impl_set_supported_core_object_type(layout_adapter,
            afw_s__AdaptiveLayoutComponentType_, true, true, xctx);
        afw_adapter_impl_set_supported_core_object_type(layout_adapter,
            afw_s__AdaptiveLayoutComponent_, false, true, xctx);
        afw_adapter_release(layout_adapter, xctx);
    }

    /* Process authorizationControl*/
    object = afw_object_old_get_property_as_object(properties,
        afw_v_authorizationControl, xctx);
    afw_authorization_internal_set_control(object, xctx);

    /* If conf adapter, start any services that are ready. */
    if (conf_adapter_id) {
        afw_service_internal_start_initial_services(p, xctx);
    }

    /* Push qualifiers on env xctx. */
    afw_application_internal_push_qualifiers(xctx);

    /*
     * If onApplicationStartupComplete specified, run its script and terminate
     * if it returns a non-nullish value of other than integer 0.
     */
    value = afw_object_get_property(properties,
        afw_v_onApplicationStartupComplete, xctx);
    if (value) {       
        AFW_LOG_Z(info,
            "Application onApplicationStartupComplete script being called.",
            xctx);
        value = afw_value_compile_and_evaluate(value,
            &impl_s_a_onApplicationStartupComplete_source_location,
            p, xctx);
        if (!afw_value_is_nullish(value) &&
            (!afw_value_is_integer(value) ||
            ((afw_value_integer_t *)value)->internal != 0))
        {
            s = afw_value_as_casted_utf8(value, p, xctx);
            AFW_THROW_ERROR_FZ(general, xctx,
                "Application onApplicationStartupComplete script returned value "
                "other than 0 - " AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(s));
        }
        else {
            AFW_LOG_Z(info,
                "Application onApplicationStartupComplete script successfully completed.",
                xctx);

        }
    }
  
    /* Log application startup complete. */
    AFW_LOG_FZ(info, xctx,
        AFW_UTF8_FMT_Q " application startup complete.",
        AFW_UTF8_FMT_ARG(application_id));

}
