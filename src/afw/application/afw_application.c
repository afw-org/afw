// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Application
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_application.c
 * @brief Application functions.
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
 */
static const afw_value_t *
impl_current_get_variable_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    const afw_value_t *result;
    afw_integer_t pid;

    result = NULL;
    if (afw_utf8_equal(name, afw_s_mode)) {
        result = xctx->mode;
    }
    else if (afw_utf8_equal(name, afw_s_pid)) {
        pid = afw_os_get_pid();
        result = afw_value_create_integer_unmanaged(pid, xctx->p, xctx);
    }
    else if (afw_utf8_equal(name, afw_s_xctxUUID)) {
        result = afw_value_create_string_unmanaged(xctx->uuid, xctx->p, xctx);
    }
    else if (afw_utf8_equal(name, afw_s_programName)) {
        result = afw_value_create_string_unmanaged(
            &xctx->env->program_name, xctx->p, xctx);
    }

    return result;
}



void
afw_application_internal_push_qualifiers(afw_xctx_t *xctx)
{
    const afw_environment_t *env = xctx->env;

    /* Push current:: qualifier. */
    afw_xctx_qualifier_stack_qualifier_push(afw_s_current, NULL, true,
        impl_current_get_variable_cb, NULL, xctx->p, xctx);

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
    const afw_object_t *context_type_object;
    const afw_object_t *variable_definitions;

    context_type_object = afw_context_type_create(
        afw_s_application, xctx->env->p, xctx);

    variable_definitions =
        afw_context_type_insure_variable_definitions_object_exists(
            context_type_object, afw_s_current, xctx);

    afw_context_variable_definition_add_z(variable_definitions,
        afw_s_mode, afw_s_runtime,
        &afw_value_evaluated_string_inf,
        "Authorization Mode",
        "The current authorization mode.",
        NULL, NULL,
        xctx);

    afw_context_variable_definition_add_z(variable_definitions,
        afw_s_pid, afw_s_runtime,
        &afw_value_evaluated_integer_inf,
        "Pid",
        "The current processor id.",
        NULL, NULL,
        xctx);

    afw_context_variable_definition_add_z(variable_definitions,
        afw_s_programName, afw_s_runtime,
        &afw_value_evaluated_string_inf,
        "Program Name",
        "The current program name.",
        NULL, NULL,
        xctx);

    afw_context_variable_definition_add_z(variable_definitions,
        afw_s_xctxUUID, afw_s_runtime,
        &afw_value_evaluated_string_inf,
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
    const afw_utf8_t *conf_adaptor_id;
    const afw_adaptor_session_t *session;
    const afw_iterator_t *iterator;
    const afw_value_t *value;
    const afw_value_t *entry_value;
    const afw_utf8_t *property_name;
    const afw_object_t *properties;
    const afw_adaptor_t *layout_adaptor;
    const afw_object_t *context_type_object;
    const afw_object_t *variable_definitions_object;
    const afw_array_t *default_flags;
    const afw_utf8_t * const *extension_id;
    const afw_utf8_t * const *module_path;
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

    /* Get optional confAdaptorId. */
    conf_adaptor_id = afw_object_old_get_property_as_utf8(
        entry, afw_s_confAdaptorId, p, xctx);

    /* Get conf adaptor.  It will not ever be released. */
    if (conf_adaptor_id) {
        AFW_LOG_FZ(debug, xctx,
            "Application specified confAdaptorId " AFW_UTF8_FMT
            ".",
            AFW_UTF8_FMT_ARG(conf_adaptor_id));
        env->conf_adaptor = afw_adaptor_get_reference(conf_adaptor_id, xctx);
        afw_adaptor_impl_set_supported_core_object_type(env->conf_adaptor,
            afw_s__AdaptiveServiceConf_, true, true, xctx);
        afw_adaptor_impl_set_supported_core_object_type(env->conf_adaptor,
            afw_s__AdaptiveConf_application, true, true, xctx);
        afw_adaptor_impl_set_supported_core_object_type(env->conf_adaptor,
            afw_s__AdaptiveTemplateProperties_, false, true, xctx);
    }

    /* Get optional applicationId and default to "application". */
    application_id = afw_object_old_get_property_as_utf8(entry,
        afw_s_applicationId, p, xctx);
    if (!application_id) {
        application_id = afw_s_Adaptive;
        afw_object_set_property_as_string(entry,
            afw_s_applicationId, application_id, xctx);
    }
    ((afw_environment_t *)env)->application_id.len = application_id->len;
    ((afw_environment_t *)env)->application_id.s = application_id->s;

    /*
     * If conf adaptor, merge in the _AdaptiveConf_application application_id
     * object to entry.
     */
    if (conf_adaptor_id) {
        object = NULL;
        error = false;
        session = afw_adaptor_session_create(conf_adaptor_id, xctx);
        AFW_TRY {
            afw_adaptor_session_get_object(
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
            afw_adaptor_session_release(session, xctx);
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
                    afw_utf8_equal(property_name, afw_s_confAdaptorId) ||
                    afw_utf8_equal(property_name, afw_s_applicationId))
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
                            AFW_UTF8_FMT_ARG(conf_adaptor_id),
                            AFW_UTF8_FMT_ARG(application_id),
                            AFW_UTF8_FMT_ARG(property_name));
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
        afw_s_extensions, xctx);
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
        afw_s_extensionModulePaths, xctx);
    if (value) {
        for (module_path = afw_value_as_array_of_utf8(value, p, xctx);
            *module_path;
            module_path++)
        {
            afw_environment_load_extension(NULL, *module_path, NULL, xctx);
        }
    }

    /* Make application context object. */
    context_type_object = afw_environment_get_context_type(afw_s_application,
        xctx);

    /* application:: variable definitions. */
    variable_definitions_object =
        afw_context_type_insure_variable_definitions_object_exists(
        context_type_object, afw_s_application, xctx);
    afw_context_variable_definitions_add_based_on_object(
        variable_definitions_object, env->application_object, xctx);

    /* qualifiedVariables definitions. */
    env->application_qualified_variables = afw_object_old_get_property_as_object(
        env->application_object, afw_s_qualifiedVariables, xctx);
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

    /* rootFilePaths */
    env->root_file_paths = afw_object_old_get_property_as_object(
        env->application_object, afw_s_rootFilePaths, xctx);

    /* defaultFlags */
    default_flags = afw_object_old_get_property_as_array(env->application_object,
        afw_s_defaultFlags, xctx);
    if (default_flags) {
        afw_flag_set_default_flag_ids(default_flags, xctx);
    }

    /* Get optional layoutAdaptorId. */
    env->layout_adaptor_id = afw_object_old_get_property_as_utf8(
        properties, afw_s_layoutsAdaptorId, p, xctx);

    /* Set supported core object type in adaptor. */
    if (env->layout_adaptor_id) {
        AFW_LOG_FZ(debug, xctx,
            "Application specified layoutAdaptorId " AFW_UTF8_FMT_Q
            ".",
            AFW_UTF8_FMT_ARG(env->layout_adaptor_id));
        layout_adaptor = afw_adaptor_get_reference(env->layout_adaptor_id, xctx);
        afw_adaptor_impl_set_supported_core_object_type(layout_adaptor,
            afw_s__AdaptiveLayoutComponentType_, true, true, xctx);
        afw_adaptor_impl_set_supported_core_object_type(layout_adaptor,
            afw_s__AdaptiveLayoutComponent_, false, true, xctx);
        afw_adaptor_release(layout_adaptor, xctx);
    }

    /* Process authorizationControl*/
    object = afw_object_old_get_property_as_object(properties,
        afw_s_authorizationControl, xctx);
    afw_authorization_internal_set_control(object, xctx);

    /* If conf adaptor, start any services that are ready. */
    if (conf_adaptor_id) {
        afw_service_internal_start_initial_services(p, xctx);
    }

    /* Push qualifiers on env xctx. */
    afw_application_internal_push_qualifiers(xctx);

    /*
     * If onApplicationStartupComplete specified, run its script and terminate
     * if it returns a non-nullish value of other than integer 0.
     */
    value = afw_object_get_property(properties,
        afw_s_onApplicationStartupComplete, xctx);
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
