// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Register Core
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_environment_register_core.c
 * @brief Register core environment.
 */

#include "afw_internal.h"
#include <unicode/utypes.h>


AFW_DECLARE_INTERNAL(void)
afw_function_internal_prepare_environment(afw_xctx_t *xctx);

static const afw_utf8_t impl_s_apr =
AFW_UTF8_LITERAL("apr");

static const afw_utf8_t impl_s_icu = 
AFW_UTF8_LITERAL("icu");

static const afw_utf8_t impl_s_description_initialEnvironmentVariables =
    AFW_UTF8_LITERAL("Environment variables when environment was created.");

/* APR RV decoder. */
static const afw_utf8_z_t * impl_rv_decoder_z_apr(int rv,
    afw_utf8_z_t *wa, afw_size_t wa_size)
{
    return apr_strerror(rv, wa, wa_size);
}


/* ICU RV decoder. */
static const afw_utf8_z_t * impl_rv_decoder_z_icu(int rv,
    afw_utf8_z_t *wa, afw_size_t wa_size)
{
    return u_errorName(rv);
}

typedef struct impl_AdaptiveLayoutComponentType_context_s {
    void *original_context;
    afw_object_cb_t original_callback;
    afw_boolean_t skip_runtime;
} impl_AdaptiveLayoutComponentType_context_t;


static afw_boolean_t
impl_AdaptiveLayoutComponentType_retrieve_cb(
    const afw_object_t *object,
    void *context,
    afw_xctx_t *xctx)
{
    impl_AdaptiveLayoutComponentType_context_t *ctx =
        (impl_AdaptiveLayoutComponentType_context_t *)context;

    if (object) {
        if (ctx->skip_runtime) {
            if (afw_runtime_get_object(
                afw_object_meta_get_object_type_id(object, xctx),
                object->meta.id, xctx))
            {
                AFW_LOG_FZ(info, xctx,
                    "/" AFW_UTF8_FMT
                    "/_AdaptiveLayoutComponentType_/" AFW_UTF8_FMT
                    " ignored because AFW core or an extension supplies it",
                    AFW_UTF8_FMT_ARG(xctx->env->layout_adaptor_id),
                    AFW_UTF8_FMT_ARG(object->meta.id));
                return false;
            }
        }
        ctx->original_callback(object, ctx->original_context, xctx);
    }

    return false;
}


/* /afw/_AdaptiveLayoutComponentType_ retrieve_objects(). */
static void
impl_AdaptiveLayoutComponentType_retrieve_objects(
    const afw_adaptor_session_t * instance,
    const afw_adaptor_impl_request_t * impl_request,
    const afw_utf8_t * object_type_id,
    const afw_query_criteria_t * criteria,
    void * context,
    afw_object_cb_t callback,
    const afw_object_t *adaptor_type_specific,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    impl_AdaptiveLayoutComponentType_context_t ctx;
    const afw_adaptor_session_t *session;

    ctx.original_callback = callback;
    ctx.original_context = context;
    ctx.skip_runtime = false;

    /* Return core component types. */
    afw_runtime_foreach(object_type_id, (void *)&ctx,
        impl_AdaptiveLayoutComponentType_retrieve_cb, xctx);

    /* If there is a layout adaptor, check it too. */
    if (xctx->env->layout_adaptor_id) {
        ctx.skip_runtime = true;
        session = afw_adaptor_session_get_cached(xctx->env->layout_adaptor_id,
            false, xctx);
        afw_adaptor_session_retrieve_objects(session, impl_request,
            object_type_id, criteria, (void *)&ctx,
            impl_AdaptiveLayoutComponentType_retrieve_cb,
            NULL, p, xctx);
    }

    /* Call callback one more time with NULL object pointer. */
    callback(NULL, context, xctx);
}



/* /afw/_AdaptiveLayoutComponentType_ get_object(). */
static void
impl_AdaptiveLayoutComponentType_get_object(
    const afw_adaptor_session_t * instance,
    const afw_adaptor_impl_request_t * impl_request,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    void * context,
    afw_object_cb_t callback,
    const afw_object_t *adaptor_type_specific,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    const afw_object_t *object;
    const afw_adaptor_session_t *session;

    /* Check for runtime object first. */
    object = afw_runtime_get_object(object_type_id, object_id, xctx);
    if (object) {
        callback(object, context, xctx);
    }

    /* Check in layout adaptor next. */
    else if (xctx->env->layout_adaptor_id) {
        session = afw_adaptor_session_get_cached(xctx->env->layout_adaptor_id,
            false, xctx);
        afw_adaptor_session_get_object(session, impl_request,
            object_type_id, object_id,
            context, callback,
            adaptor_type_specific,
            p, xctx);
    }

    /* Otherwise, not found. */
    else {
        callback(NULL, context, xctx);
    }
}


static const afw_object_t *
impl_create_environment_variables_object(
    afw_xctx_t *xctx)
{
    extern char **environ;
    const afw_object_t *result;
    char **v;
    char *s;
    char *c;
    const afw_utf8_t *name;
    const afw_utf8_t *value;

    result = afw_object_create(xctx->p, xctx);
    afw_object_meta_set_ids(result,
        afw_s_afw,
        afw_s__AdaptiveSystemInfo_,
        afw_s_initialEnvironmentVariables,
        xctx);
    afw_object_meta_set_property_as(result, afw_s_description, string,
        &impl_s_description_initialEnvironmentVariables, xctx);
    afw_object_meta_set_read_only(result, xctx);

    for (v = environ; *v; v++)
    {
        for (s = c = *v; *c != '=' && *c != 0; c++);
        name = afw_utf8_create_copy(s, c - s, result->p, xctx);
        if (*c == '=') c++;
        value = afw_utf8_create_copy(c, AFW_UTF8_Z_LEN, result->p, xctx);
        afw_object_set_property_as_string(result,
            name, value, xctx);
    }

    return result;
}


/*
 * Register anything that is part of libafw.  This is a bootstrap function
 * so order of many of the contained calls is important.
 */
void afw_environment_internal_register_core(afw_xctx_t *xctx)
{
    afw_request_handler_head_t *head;
    const afw_adaptor_factory_t *adaptor_factory;
    const afw_log_factory_t *log_factory;
    afw_environment_internal_t *env;
    afw_runtime_custom_t *runtime_custom;
    afw_components_t *afw_components;
    afw_double_t d0;

    env = (afw_environment_internal_t *)xctx->env;

    /* Set infinity, minus infinity, and NaN */
    d0 = 0;
    env->pub.infinity = 1 / d0;
    env->pub.infinity_value =
        afw_value_create_double(env->pub.infinity,
            xctx->p, xctx);
    env->pub.minus_infinity = -1 / d0;
    env->pub.minus_infinity_value =
        afw_value_create_double(env->pub.minus_infinity,
            xctx->p, xctx);
    env->pub.NaN = sqrt(-1);
    env->pub.NaN_value =
        afw_value_create_double(env->pub.NaN,
            xctx->p, xctx);

    /* Register core generated data_types, functions, etc. */
    afw_generated_register(xctx);
    env->core_data_types_registered = true;
    env->core_functions_registered = true;
    env->core_object_type_maps_registered = true;

    /* Register APR RV decoder. */
    afw_environment_register_error_rv_decoder(&impl_s_apr,
        impl_rv_decoder_z_apr, xctx);

    /* Register ICU RV decoder. */
    afw_environment_register_error_rv_decoder(&impl_s_icu,
        impl_rv_decoder_z_icu, xctx);

    /* Register flag that are needed early in register core. */
    afw_flag_internal_early_register_core(xctx);

    /* Register core value infs. */
    afw_value_register_core_value_infs(xctx);

    /* Register core runtime value accessors. */
    afw_runtime_register_core_value_accessors(xctx);

    /* Prepare function environment. */
    afw_function_internal_prepare_environment(xctx);

    /* Create and register environment variable object. */
    env->pub.initial_environment_variables =
        impl_create_environment_variables_object(xctx);
    afw_runtime_env_set_object(
        env->pub.initial_environment_variables, false, xctx);

    /* Register basic application context type. */
    afw_application_internal_register_basic_application_context_type(
        xctx);

    /* Register conf type "adaptor" configuration entry handler. */
    afw_environment_create_and_register_conf_type(
        afw_s_adaptor,
        afw_adaptor_internal_conf_type_create_cede_p,
        afw_s_a_adaptor_title,
        afw_s_a_adaptor_description,
        afw_s_adaptorId,
        afw_s_adaptor_id,
        afw_s__AdaptiveAdaptor_,
        afw_s_adaptorType,
        afw_s_adaptor_type,
        afw_s__AdaptiveAdaptorType_,
        true,
        xctx);

    /* Register /afw/_AdaptiveEnvironmentRegistry_/current */
    afw_runtime_env_set_object(
        afw_environment_registry_object_get_current(xctx),
        false, xctx);

    /* Register service type "adaptor". */
    afw_adaptor_internal_register_service_type(xctx);

    /* Register type "application" configuration entry handler. */
    afw_environment_create_and_register_conf_type(
        afw_s_application,
        afw_application_internal_application_conf_type_create_cede_p,
        afw_s_a_application_title,
        afw_s_a_application_description,
        NULL,
        NULL,
        afw_s__AdaptiveApplication_,
        NULL,
        NULL,
        NULL,
        false,
        xctx);

    /* Register type "extension" configuration entry handler. */
    afw_environment_create_and_register_conf_type(
        afw_s_extension,
        afw_environment_internal_extension_conf_type_create_cede_p,
        afw_s_a_extension_title,
        afw_s_a_extension_description,
        afw_s_extensionId,
        afw_s_extension,
        afw_s__AdaptiveExtension_,
        NULL,
        NULL,
        NULL,
        false,
        xctx);

    /* Register type "log" configuration entry handler. */
    afw_environment_create_and_register_conf_type(
        afw_s_log,
        afw_log_internal_conf_type_create_cede_p,
        afw_s_a_log_title,
        afw_s_a_log_description,
        afw_s_logId,
        afw_s_log_id,
        afw_s__AdaptiveLog_,
        afw_s_logType,
        afw_s_log_type,
        afw_s__AdaptiveLogType_,
        true,
        xctx);

    /* Register service type "log" */
    afw_log_internal_register_service_type(xctx);

    /* Register type "requestHandler" configuration entry handler. */
    afw_environment_create_and_register_conf_type(
        afw_s_requestHandler,
        afw_request_handler_internal_conf_type_create_cede_p,
        afw_s_a_requestHandler_title,
        afw_s_a_requestHandler_description,
        NULL,
        NULL,
        afw_s__AdaptiveRequestHandler_,
        afw_s_requestHandlerType,
        NULL,
        afw_s__AdaptiveRequestHandlerType_,
        false,
        xctx);

    /* Register runtime adaptor. */
    adaptor_factory = afw_runtime_get_adaptor_factory();
    afw_environment_register_adaptor_type(
        &adaptor_factory->adaptor_type,
        adaptor_factory, xctx);

    /* Register afw adaptor. */
    afw_adaptor_internal_register_afw_adaptor(xctx);

    /* Register factory for log_type=standard. */
    log_factory = afw_log_standard_factory_get();
    afw_environment_register_log_type(
        &log_factory->log_type,
        log_factory, xctx);

    /* Register special handler for _AdaptiveService_ objects. */
    runtime_custom = afw_xctx_calloc_type(afw_runtime_custom_t, xctx);
    runtime_custom->retrieve_objects =
        afw_service_internal_AdaptiveService_retrieve_objects;
    runtime_custom->get_object =
        afw_service_internal_AdaptiveService_get_object;
    afw_environment_register_runtime_custom(
        afw_s__AdaptiveService_,
        runtime_custom, xctx);

    /* Register special handler for _AdaptiveLayoutComponentType_ objects. */
    runtime_custom = afw_xctx_calloc_type(afw_runtime_custom_t, xctx);
    runtime_custom->retrieve_objects =
        impl_AdaptiveLayoutComponentType_retrieve_objects;
    runtime_custom->get_object =
        impl_AdaptiveLayoutComponentType_get_object;
    afw_environment_register_runtime_custom(
        afw_s__AdaptiveLayoutComponentType_,
        runtime_custom, xctx);

    /* Register factory for adaptor_type=file. */
    adaptor_factory = afw_file_adaptor_factory_get();
    afw_environment_register_adaptor_type(
        &adaptor_factory->adaptor_type,
        adaptor_factory, xctx);

    /* Register model adaptor factory. */
    afw_environment_register_adaptor_type(
        &afw_adaptor_factory_model.adaptor_type,
        &afw_adaptor_factory_model, xctx);

    /* Register context_type for model. */
    afw_model_internal_register_context_type_model(xctx);

    /* Register request handler factory for handler type "adaptor". */
    afw_environment_register_request_handler_type(
        &afw_request_handler_factory_adaptor.request_handler_type,
        &afw_request_handler_factory_adaptor, xctx);

    /* Register service type authorizationHandler and default handler. */
    afw_authorization_internal_register(xctx);

    /* Allocate request handler head and register its singleton. */
    head = afw_pool_calloc_type(xctx->env->p, afw_request_handler_head_t, xctx);
    afw_environment_register_singleton(
        &AFW_REQUEST_HANDLER_S_SINGLETON_KEY_HEAD,
        head, xctx);

    /* Allocate object for accessing and loading afw_components extension. */
    afw_components = afw_pool_calloc_type(xctx->env->p, afw_components_t, xctx);
    afw_runtime_env_create_and_set_indirect_object(
        afw_s__AdaptiveApplicationComponents_,
        afw_s_current, afw_components, true, xctx);

    /** @fixme Not implemented yet
        Register factory for log_type=file.
    log_factory = afw_log_file_factory_get();
    afw_environment_register_log_type(
        &log_factory->log_type,
        log_factory, xctx);
     */

    /* Register JSON support. */
    afw_json_register(xctx);

    /* Register application/x-afw content handler. */
    AFW_DECLARE_INTERNAL(void)
        afw_content_type_application_afw_register(afw_xctx_t *xctx);
    afw_content_type_application_afw_register(xctx);

    /* Call OS environment initialize. */
    afw_os_environment_initialize(xctx);
}
