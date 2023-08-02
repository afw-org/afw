// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Service Support
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_service.c
 * @brief Adaptive Framework service support
 */

#include "afw_internal.h"



typedef struct {
    const afw_pool_t * p;
    afw_object_cb_t original_callback;
    void *original_context;
    const afw_query_criteria_t * criteria;
    apr_hash_t *service_ids;
    const afw_object_t *last_object;
} impl_AdaptiveService_context_t;


typedef struct {
    /* Returned object for get. */
    const afw_object_t *object;

    /* This is a manual start. */
    afw_boolean_t manual_start;
} impl_start_context_t;


static const afw_value_string_t
impl_startup_values[] =
{

#define XX(id, _) \
    { \
        &afw_value_evaluated_string_inf, \
        AFW_UTF8_LITERAL(# id) \
    },
    AFW_SERVICE_STARTUP_MAP(XX)
#undef XX
    {
        &afw_value_evaluated_string_inf,
        AFW_UTF8_LITERAL("invalid"),
    }

};



#define impl_startup_value(id) \
(const afw_value_t *)&impl_startup_values[afw_service_startup_ ## id]



static const afw_value_string_t impl_startup_descriptions[] =
{
#define XX(id, desc) \
    { \
        &afw_value_evaluated_string_inf, \
        AFW_UTF8_LITERAL(desc) \
    },
    AFW_SERVICE_STARTUP_MAP(XX)
#undef XX
    {
        &afw_value_evaluated_string_inf,
        AFW_UTF8_LITERAL("invalid"),
    }
};



static const afw_value_string_t
impl_status_values[] =
{
#define XX(id, _) \
    { \
        &afw_value_evaluated_string_inf, \
        AFW_UTF8_LITERAL(# id) \
    },
    AFW_SERVICE_STATUS_MAP(XX)
#undef XX
    {
        &afw_value_evaluated_string_inf,
        AFW_UTF8_LITERAL("invalid"),
    }
};



#define impl_status_value(id) \
(const afw_value_t *)&impl_status_values[afw_service_status_ ## id]


static const afw_value_string_t
impl_status_descriptions[] =
{

#define XX(id, desc) \
    { \
        &afw_value_evaluated_string_inf, \
        AFW_UTF8_LITERAL(desc) \
    },
    AFW_SERVICE_STATUS_MAP(XX)
#undef XX
    {
        &afw_value_evaluated_string_inf,
        AFW_UTF8_LITERAL("invalid"),
    }
};


/* Internal start a service. */
static void
impl_start_service(
    afw_service_t *service,
    afw_xctx_t *xctx);


/* Internal restart a service. */
static void
impl_restart_service(
    afw_service_t *service,
    afw_xctx_t *xctx);


/* Convert afw_service_startup_t enum to corresponding utf8. */
AFW_DEFINE(const afw_utf8_t *)
afw_service_startup_as_utf8(afw_service_startup_t startup)
{
    afw_service_startup_t i;

    i = startup;
    if (i < 0 || i >= afw_service_startup_invalid) {
        i = afw_service_startup_invalid;
    }

    return &impl_startup_values[i].internal;
}


/* Convert afw_service_startup_t enum to corresponding adaptive value. */
AFW_DEFINE(const afw_value_t *)
afw_service_startup_as_value(afw_service_startup_t startup)
{
    afw_service_startup_t i;

    i = startup;
    if (i < 0 || i >= afw_service_startup_invalid) {
        i = afw_service_startup_invalid;
    }

    return (const afw_value_t *)&impl_startup_values[i];
}


/* Get description for a afw_service_startup_t enum.  */
AFW_DEFINE(const afw_utf8_t *)
afw_service_startup_description(afw_service_startup_t startup)
{
    afw_service_startup_t i;

    i = startup;
    if (i < 0 || i >= afw_service_startup_invalid) {
        i = afw_service_startup_invalid;
    }

    return &impl_startup_descriptions[i].internal;
}


/* Get description as value for a afw_service_startup_t enum.  */
AFW_DEFINE(const afw_value_t *)
afw_service_startup_description_as_value(afw_service_startup_t startup)
{
    afw_service_startup_t i;

    i = startup;
    if (i < 0 || i >= afw_service_startup_invalid) {
        i = afw_service_startup_invalid;
    }

    return (const afw_value_t*)&impl_startup_descriptions[i];
}


/* Convert utf8 to corresponding afw_service_startup_t enum. */
AFW_DECLARE(afw_service_startup_t)
afw_service_startup_as_enum(const afw_utf8_t *s)
{
    afw_service_startup_t result;

    for (result = 0;
        result < afw_service_startup_invalid &&
        !afw_utf8_equal(&impl_startup_values[result].internal, s);
        result++);

    return result;
}


/* Convert afw_service_status_t enum to corresponding utf8. */
AFW_DEFINE(const afw_utf8_t *)
afw_service_status_as_utf8(afw_service_status_t status)
{
    afw_service_status_t i;

    i = status;
    if (i < 0 || i >= afw_service_status_invalid) {
        i = afw_service_status_invalid;
    }

    return &impl_status_values[i].internal;
}


/* Convert afw_service_status_t enum to corresponding adaptive value. */
AFW_DEFINE(const afw_value_t *)
afw_service_status_as_value(afw_service_status_t status)
{
    afw_service_status_t i;

    i = status;
    if (i < 0 || i >= afw_service_status_invalid) {
        i = afw_service_status_invalid;
    }

    return (const afw_value_t *)&impl_status_values[i];
}


/* Get description of a afw_service_status_t enum. */
AFW_DECLARE(const afw_utf8_t *)
afw_service_status_description(afw_service_status_t status)
{
    afw_service_status_t i;

    i = status;
    if (i < 0 || i >= afw_service_status_invalid) {
        i = afw_service_status_invalid;
    }

    return &impl_status_descriptions[i].internal;
}


/* Get description as value of a afw_service_status_t enum. */
AFW_DECLARE(const afw_value_t *)
afw_service_status_description_as_value(afw_service_status_t status)
{
    afw_service_status_t i;

    i = status;
    if (i < 0 || i >= afw_service_status_invalid) {
        i = afw_service_status_invalid;
    }

    return (const afw_value_t *)&impl_status_descriptions[i];
}


/* Convert utf8 to corresponding afw_service_status_t enum. */
AFW_DEFINE(afw_service_status_t)
afw_service_status_as_enum(const afw_utf8_t *s)
{
    afw_service_status_t result;

    for (result = 0;
        result < afw_service_status_invalid &&
        !afw_utf8_equal(&impl_status_values[result].internal, s);
        result++);

    return result;
}



static void
impl_initialize_and_start_service_using_conf(
    afw_service_t *service,
    const afw_utf8_t *source_location,
    afw_boolean_t restart,
    const afw_object_t *conf,
    const afw_utf8_t *conf_source_location,
    afw_xctx_t *xctx)
{
    apr_status_t rv;
    const afw_utf8_t *s;
    const afw_pool_t *p = service->p;
    const afw_utf8_t *object_id;

    /* properties and source_location */
    service->properties = afw_object_create_clone(conf, p, xctx);;
    service->source_location = source_location;
    service->conf_source_location = conf_source_location;

    /* Set sourceLocation in properties. */
    afw_object_set_property_as_string(service->properties,
        &afw_s_sourceLocation, conf_source_location, xctx);
    /* type */
    service->type = afw_object_old_get_property_as_string(
        service->properties,
        &afw_s_type, xctx);
    if (!service->type || service->type->len == 0) {
        AFW_THROW_ERROR_FZ(general, xctx,
            AFW_UTF8_CONTEXTUAL_LABEL_FMT
            "missing conf/type property",
            AFW_UTF8_FMT_ARG(conf_source_location));
    }

    /* service type */
    service->service_type = afw_environment_get_service_type(
        service->type, xctx);
    if (!service->service_type) {
        AFW_THROW_ERROR_FZ(general, xctx,
            AFW_UTF8_CONTEXTUAL_LABEL_FMT
            "invalid service type " AFW_UTF8_FMT_Q,
            AFW_UTF8_FMT_ARG(source_location),
            AFW_UTF8_FMT_ARG(service->type));
    }

    /* conf subtype */
    service->conf_subtype = NULL;
    if (service->service_type->conf_type->subtype_property_name) {
        service->conf_subtype = afw_object_old_get_property_as_string(
            service->properties,
            service->service_type->conf_type->subtype_property_name, xctx);
        if (!service->conf_subtype || service->conf_subtype->len == 0)
        {
            AFW_THROW_ERROR_FZ(general, xctx,
                AFW_UTF8_CONTEXTUAL_LABEL_FMT
                "missing 'conf/" AFW_UTF8_FMT "' property",
                AFW_UTF8_FMT_ARG(source_location),
                AFW_UTF8_FMT_ARG(service->service_type->conf_type->
                    subtype_property_name));
        }
    }

    /* conf id */
    service->conf_id = NULL;
    if (service->service_type->conf_type->id_property_name)
    {
        service->conf_id = afw_object_old_get_property_as_string(
            service->properties,
            service->service_type->conf_type->id_property_name, xctx);
        if (!service->conf_id || service->conf_id->len == 0)
        {
            AFW_THROW_ERROR_FZ(general, xctx,
                AFW_UTF8_CONTEXTUAL_LABEL_FMT
                "missing " AFW_UTF8_FMT_Q " property",
                AFW_UTF8_FMT_ARG(conf_source_location),
                AFW_UTF8_FMT_ARG(service->service_type->conf_type->id_property_name)
            );
        }
    }

    /* service id */
    object_id = (service->conf_id) ? service->conf_id : &afw_s_current;
    s = afw_utf8_printf(p, xctx,
        AFW_UTF8_FMT
        "-" AFW_UTF8_FMT,
        AFW_UTF8_FMT_ARG(service->type),
        AFW_UTF8_FMT_ARG(object_id));
    if (service->service_id.len > 0) {
        if (!afw_utf8_equal(&service->service_id, s)) {
            AFW_THROW_ERROR_FZ(general, xctx,
                AFW_UTF8_CONTEXTUAL_LABEL_FMT
                "serviceId " AFW_UTF8_FMT_Q
                " is not appropriate for this conf",
                AFW_UTF8_FMT_ARG(source_location),
                AFW_UTF8_FMT_ARG(s));
        }
    }
    service->service_id.len = s->len;
    service->service_id.s = s->s;

    /* conf objectType */
    s = afw_utf8_printf(p, xctx,
        "_AdaptiveConf_" AFW_UTF8_FMT
        "%s" AFW_UTF8_FMT,
        AFW_UTF8_FMT_ARG(service->type),
        (service->conf_subtype) ? "_" : "",
        (service->conf_subtype) ? (int)service->conf_subtype->len : 0,
        (service->conf_subtype) ? service->conf_subtype->s : NULL);
    afw_object_meta_set_object_type_id(service->properties, s, xctx);

    rv = apr_thread_mutex_create(
            &service->mutex,
            APR_THREAD_MUTEX_UNNESTED, afw_pool_get_apr_pool(p));
    if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_Z(general, apr, rv,
            "apr_thread_mutex_create() failed", xctx);
    }

    service->status = afw_service_status_ready_to_start;
    if (restart) {
        impl_restart_service(service, xctx);
    }
    else {
        impl_start_service(service, xctx);
    }
}



/* Retrieve _AdaptiveServiceConf_ objects start initial callback. */
static afw_boolean_t
impl_start_cb(
    const afw_object_t *object,
    void *context,
    afw_xctx_t *xctx)
{
    impl_start_context_t *ctx = context;
    const afw_pool_t *p;
    const afw_utf8_t *service_id;
    const afw_utf8_t *startup;
    afw_service_t *service;
    afw_service_t *old_service;
    const afw_object_t *conf;
    const afw_utf8_t *source_location;

    /* If no object, return. */
    if (!object) return false;
    p = NULL;
    source_location = afw_object_meta_get_path(object, xctx);

    service = NULL;
    service_id = &afw_s_unknown;
    AFW_TRY {

        /*
         * Return if startup is not immediate or permanent, except when this
         * is a manual start and startup is manual.
         */
        startup = afw_object_old_get_property_as_string(object,
            &afw_s_startup, xctx);
        if (
            !startup ||
            (!afw_utf8_equal(startup, &afw_s_immediate) &&
                !afw_utf8_equal(startup, &afw_s_permanent) &&
                !(ctx->manual_start && afw_utf8_equal(startup, &afw_s_manual)))
            )
        {
            break; /* Return. */
        }

        /* Get serviceId.  Default to object id. */
        service_id = afw_object_old_get_property_as_string(object,
            &afw_s_serviceId, xctx);
        if (!service_id) {
            service_id = afw_object_meta_get_object_id(object, xctx);
        }
        if (!service_id) {
            AFW_THROW_ERROR_FZ(general, xctx,
                AFW_UTF8_CONTEXTUAL_LABEL_FMT
                "serviceId can not be determined",
                AFW_UTF8_FMT_ARG(source_location));
        }

        /*
         * See if service already registered.  Return if anything except error
         * or a manual start and service stopped.
         */
        service = afw_environment_get_service(service_id, xctx);
        if (service &&
            service->status != afw_service_status_error &&
            !(ctx->manual_start &&
                service->status == afw_service_status_stopped)
            )
        {
            break; /* Return. */
        }

        /* Allocate and initialize new service instance. */
        old_service = service;
        p = afw_pool_create_multithreaded(xctx->env->p, xctx);
        service = afw_pool_calloc_type(p, afw_service_t, xctx);
        service->p = p;
        service->source_location = afw_utf8_clone(source_location,
            p, xctx);
        source_location = service->source_location;
        service->service_id.len = service_id->len;
        service->service_id.s = afw_memory_dup(
            service_id->s, service_id->len, p, xctx);
        service->has_service_conf = true;
        if (!old_service) {
            afw_environment_register_service(&service->service_id, service,
                xctx);
        }

        conf = afw_object_old_get_property_as_object(object,
            &afw_s_conf, xctx);
        if (!conf) {
            AFW_THROW_ERROR_FZ(general, xctx,
                AFW_UTF8_CONTEXTUAL_LABEL_FMT
                "missing conf property",
                AFW_UTF8_FMT_ARG(source_location));
        }
        conf = afw_object_create_clone(conf, p, xctx);
        service->conf_source_location = afw_utf8_printf(
            p, xctx, AFW_UTF8_FMT "/conf",
            AFW_UTF8_FMT_ARG(source_location));

        /* Start service and release existing service if there is one. */
        impl_initialize_and_start_service_using_conf(service,
            source_location, false, conf, service->conf_source_location, xctx);
    }

    AFW_CATCH_UNHANDLED {
        /** @fixme This still needs work.  This error can be lost since this service
         * may not be registered.
         */
        if (service) {
            service->status = afw_service_status_error;
            service->status_message = afw_utf8_create_copy(
                AFW_ERROR_THROWN->message_z, AFW_UTF8_Z_LEN,
                p, xctx);
            service->status_debug = afw_error_to_utf8(
                    AFW_ERROR_THROWN, p, xctx);
        }
        afw_error_write_log(afw_log_priority_err, AFW_ERROR_THROWN, xctx);
        AFW_LOG_FZ(err, xctx, "Service " AFW_UTF8_FMT_Q " failed to start.",
                AFW_UTF8_FMT_ARG(service_id));

        /** @fixme Deal with error_service not NULL. */
    }

    AFW_FINALLY{
        /** @fixme Deal with error_service not NULL. */
    }

    AFW_ENDTRY;

    /* Return indicating not to short circuit */
    return false;
}



/* Start initial services */
void
afw_service_internal_start_initial_services(
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_adaptor_session_t *session;
    impl_start_context_t ctx;

    /* Clear ctx and set p. */
    afw_memory_clear(&ctx);

    /* If there is not a conf adaptor, nothing to start, so return. */
    if (!xctx->env->conf_adaptor ||
        xctx->env->conf_adaptor->adaptor_id.len == 0)
    {
        return;
    }

    /* Start immediate and permanent services. */
    session = afw_adaptor_session_create(
        &xctx->env->conf_adaptor->adaptor_id, xctx);
    AFW_TRY {
        afw_adaptor_session_retrieve_objects(session, NULL,
            &afw_s__AdaptiveServiceConf_, NULL,
            &ctx, impl_start_cb, NULL, p, xctx);
    }

    AFW_FINALLY{
        afw_adaptor_session_release(session, xctx);
    }

    AFW_ENDTRY;
}



static void
impl_add_runtime_service_info_to_object(
    const afw_object_t *object,
    const afw_service_t *service,
    const afw_object_t *service_conf,
    const afw_object_t *conf_property,
    const afw_utf8_t *service_id,
    const afw_utf8_t *type,
    const afw_utf8_t *id,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p = object->p;
    const afw_service_type_t *service_type;
    const afw_utf8_t *subtype;
    const afw_utf8_t *conf_object_type_id;
    const afw_value_t *value;
    const afw_value_t *value_description;
    const afw_value_t *startup_value;
    afw_service_startup_t startup;
    const afw_utf8_t *s;
    const afw_utf8_octet_t *c;
    afw_size_t len;
    afw_utf8_t w_type;
    afw_utf8_t w_id;
    afw_boolean_t can_start = false;
    afw_boolean_t can_stop = false;
    afw_boolean_t can_restart = false;

    afw_object_meta_set_ids(object,
        &afw_s_afw, &afw_s__AdaptiveService_, service_id, xctx);

    /* If type or id is NULL, determine from service_id. */
    if (!type || !id) {
        for (
            w_type.s = c = service_id->s,
            w_type.len = len = service_id->len;
            len > 0 && *c != '-';
            c++, len--
            );
        if (len > 0) {
            w_type.len = c - w_type.s;
            w_id.s = c + 1;
            w_id.len = len - 1;
        }
        if (len <= 0 || w_id.len < 1) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Invalid serviceId " AFW_UTF8_FMT_Q,
                AFW_UTF8_FMT_ARG(service_id));
        }
        type = &w_type;
        id = &w_id;
    }

    service_type = afw_environment_get_service_type(type, xctx);
    if (!service_type) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "Invalid type in serviceId " AFW_UTF8_FMT_Q,
            AFW_UTF8_FMT_ARG(service_id));
    }

    afw_object_set_property_as_string(object,
        &afw_s_serviceId, service_id, xctx);
    afw_object_set_property_as_string(object,
        &afw_s_serviceType, type, xctx);
    afw_object_set_property_as_string(object,
        &afw_s_confId, id, xctx);

    subtype = NULL;
    if (service_type->conf_type->subtype_property_name) {
        subtype = afw_object_old_get_property_as_string(conf_property,
            service_type->conf_type->subtype_property_name, xctx);
        if (!subtype) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "missing 'conf/" AFW_UTF8_FMT
                "' property in serviceId " AFW_UTF8_FMT_Q,
                AFW_UTF8_FMT_ARG(
                    service_type->conf_type->subtype_property_name),
                AFW_UTF8_FMT_ARG(service_id));
        }
        afw_object_set_property_as_string(object,
            &afw_s_confSubtype, subtype, xctx);
    }

    if (service_type->conf_type->id_runtime_object_type_id) {
        s = afw_utf8_printf(p, xctx,
            "/afw/" AFW_UTF8_FMT
            "/" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(service_type->conf_type->
                id_runtime_object_type_id),
            AFW_UTF8_FMT_ARG(id));
        afw_object_set_property_as_string(object,
            &afw_s_uriRelated, s, xctx);
    }

    if (subtype) {
        conf_object_type_id = afw_utf8_printf(p, xctx,
            "_AdaptiveConf_" AFW_UTF8_FMT
            "." AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(type), AFW_UTF8_FMT_ARG(subtype));

    } else {
        conf_object_type_id = afw_utf8_printf(p, xctx,
            "_AdaptiveConf_" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(type));
    }

    afw_object_set_property_as_string(object,
        &afw_s_confPropertyObjectType, conf_object_type_id, xctx);

    startup = afw_service_startup_invalid;
    if (service_conf) {
        startup_value = afw_object_get_property(service_conf, &afw_s_startup,
            xctx);
        if (afw_value_is_string(startup_value)) {
            startup = afw_service_startup_as_enum(
                &((const afw_value_string_t *)startup_value)->internal);
        }
        /* permanent not allowed in conf. */
        if (startup == afw_service_startup_permanent) {
            startup = afw_service_startup_invalid;
        }
        afw_object_set_property(object, &afw_s_startup,
            (const afw_value_t *)&impl_startup_values[startup], xctx);
        afw_object_set_property(object, &afw_s_startupDescription,
            (const afw_value_t *)&impl_startup_descriptions[startup], xctx);

        if (startup == afw_service_startup_manual ||
            startup == afw_service_startup_immediate)
        {
            if (service) {
                switch (service->status) {

                case afw_service_status_error:
                case afw_service_status_ready_to_start:
                case afw_service_status_stopped:
                    can_start = true;
                    break;

                case afw_service_status_running:
                    can_restart = true;
                    can_stop = true;

                case afw_service_status_disabled:
                case afw_service_status_starting:
                case afw_service_status_stopping:
                case afw_service_status_restarting:
                    /** @fixme These cases do nothing but maybe they should. */
                    break;

                case afw_service_status_invalid:
                default:
                    AFW_THROW_ERROR_Z(code, "Invalid case", xctx);
                }
            }
            else {
                can_start = true;
            }
        }
    }
    else  {
        afw_object_set_property(object, &afw_s_startup,
            (const afw_value_t *)
            &impl_startup_values[afw_service_startup_permanent], xctx);
        afw_object_set_property(object, &afw_s_startupDescription,
            (const afw_value_t *)
            &impl_startup_descriptions[afw_service_startup_permanent], xctx);
    }

    if (service) {

        /*
         * If there is a service entry that use to have a service conf and it
         * is running, allow it to be stopped.
         */
        if (service->has_service_conf && !service_conf &&
            service->status == afw_service_status_running)
        {
            can_stop = true;
        }

        afw_object_set_property(object,
            &afw_s_status,
            afw_service_status_as_value(service->status),
            xctx);

        afw_object_set_property(object,
            &afw_s_statusDescription,
            afw_service_status_description_as_value(service->status),
            xctx);

        if (service->source_location && service->source_location->len > 0) {
            afw_object_set_property_as_string(object,
                &afw_s_sourceLocation, service->source_location, xctx);
        }

        afw_object_set_property_as_dateTime(object,
            &afw_s_startTime, &service->start_time, xctx);


        if (service->status_message && service->status_message->len > 0) {
            afw_object_set_property_as_string(object,
                &afw_s_statusMessage, service->status_message, xctx);
        }

        if (service->status_debug && service->status_debug->len > 0) {
            afw_object_set_property_as_string(object,
                &afw_s_statusDebug, service->status_debug, xctx);
        }

    }

    else {
        if (startup == afw_service_startup_disabled)
        {
            value = (const afw_value_t *)
                &impl_status_values[afw_service_status_disabled];
            value_description =  (const afw_value_t *)
                &impl_status_descriptions[afw_service_status_disabled];
        } else {
            value = (const afw_value_t *)
                &impl_status_values[afw_service_status_ready_to_start];
            value_description =  (const afw_value_t *)
                &impl_status_descriptions[afw_service_status_ready_to_start];
        }
        afw_object_set_property(object, &afw_s_status, value, xctx);
        afw_object_set_property(object, &afw_s_statusDescription,
            value_description, xctx);
    }

    afw_object_set_property_as_boolean(object,
        &afw_s_canStart, can_start, xctx);
    afw_object_set_property_as_boolean(object,
        &afw_s_canStop, can_stop, xctx);
    afw_object_set_property_as_boolean(object,
        &afw_s_canRestart, can_restart, xctx);
}



/*
 * _AdaptiveService_ get/retrieve cb.  Augments _AdaptiveServiceConf_ object
 * with runtime info then calls original callback.
 */
static afw_boolean_t
impl_AdaptiveService_cb(
    const afw_object_t *original_object,
    void *context,
    afw_xctx_t *xctx)
{
    impl_AdaptiveService_context_t *ctx = context;
    const afw_utf8_t *service_id;
    const afw_service_t *service;
    afw_boolean_t meetsCriteria;
    const afw_pool_t *p;
    const afw_object_t *conf_property;
    const afw_object_t *object;
    const afw_utf8_t *s;
    const afw_value_t *value;
    const afw_utf8_t *error_message;
    afw_boolean_t is_complete;

    is_complete = false;
    p = xctx->p;
    object = original_object;
    if (object) {
        service_id = &afw_s_unknown; /* Get rid of compiler warning. */
        AFW_TRY {

            p = original_object->p;

            object = afw_object_create(p, xctx);

            service_id = afw_object_old_get_property_as_string(
                original_object,
                &afw_s_serviceId, xctx);
            if (!service_id) {
                AFW_THROW_ERROR_Z(general, "missing serviceId", xctx);
            }

            s = afw_object_meta_get_path(original_object, xctx);
            afw_object_set_property_as_anyURI(object,
                &afw_s_uriServiceConf, s, xctx);
            afw_object_set_property_as_string(object,
                &afw_s_sourceLocation, s, xctx);

            value = afw_object_get_property(original_object,
                &afw_s_annotation, xctx);
            if (value) {
                afw_object_set_property(object, &afw_s_annotation, value,
                    xctx);
            }

            conf_property = afw_object_old_get_property_as_object(
                original_object, &afw_s_conf, xctx);
            if (!conf_property) {
                AFW_THROW_ERROR_FZ(general, xctx,
                    "missing conf property in serviceId " AFW_UTF8_FMT_Q,
                    AFW_UTF8_FMT_ARG(service_id));
            }

            service = afw_environment_get_service(service_id, xctx);
            impl_add_runtime_service_info_to_object(object, service,
                original_object, conf_property, service_id,
                NULL, NULL, xctx);

        }

        AFW_CATCH_UNHANDLED{
            error_message = afw_utf8_create_copy(
                AFW_ERROR_THROWN->message_z, AFW_UTF8_Z_LEN,
                p, xctx);
            afw_object_set_property_as_string(object,
                &afw_s_statusMessage,
                error_message, xctx);
            error_message = afw_error_to_utf8(AFW_ERROR_THROWN,
                p, xctx);
            afw_object_set_property_as_string(object,
                &afw_s_statusDebug,
                error_message, xctx);
            afw_object_set_property(object, &afw_s_startup,
                impl_startup_value(disabled), xctx);
            afw_object_set_property(object, &afw_s_status,
                impl_status_value(error), xctx);
        }

        AFW_ENDTRY;

        meetsCriteria = true;
        if (ctx->criteria) {
            meetsCriteria = afw_query_criteria_test_object(object,
                ctx->criteria, ctx->p, xctx);
        }

        if (ctx->service_ids) {
            apr_hash_set(ctx->service_ids, service_id->s, service_id->len,
                service_id);
        }

        if (meetsCriteria && ctx->original_callback) {
            is_complete = ctx->original_callback(object,
                ctx->original_context, xctx);
        }
    }

    ctx->last_object = object;

    /* Return indicating not to short circuit */
    return is_complete;
}


static afw_boolean_t
impl_retrieve_from_registry_cb(
    int type_number,
    void *data,
    const afw_utf8_octet_t *key_s,
    afw_size_t key_len,
    void *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_service_t *service = value;
    impl_AdaptiveService_context_t *ctx = data;
    const afw_object_t *object;
    afw_boolean_t meetsCriteria;
    afw_boolean_t is_complete;

    is_complete = false;
    if (!ctx->service_ids || !apr_hash_get(ctx->service_ids, key_s, key_len)) {
        object = afw_object_create_managed(p, xctx);

        impl_add_runtime_service_info_to_object(object, service,
            NULL, service->properties, &service->service_id,
            service->type, service->conf_id, xctx);

        meetsCriteria = true;
        if (ctx->criteria) {
            meetsCriteria = afw_query_criteria_test_object(object,
                ctx->criteria, p, xctx);
        }

        if (meetsCriteria && ctx->original_callback) {
            is_complete = ctx->original_callback(object, ctx->original_context,
                xctx);
        }
    }

    return is_complete;
}



/* Special routine for _AdaptiveService_ session retrieve_objects(). */
void
afw_service_internal_AdaptiveService_retrieve_objects (
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
    impl_AdaptiveService_context_t ctx;
    const afw_adaptor_session_t *session;

    memset(&ctx, 0, sizeof(ctx));
    ctx.p = p;
    ctx.original_callback = callback;
    ctx.original_context = context;
    ctx.criteria = criteria;

    if (xctx->env->conf_adaptor) {
        session = afw_adaptor_session_create(
            &xctx->env->conf_adaptor->adaptor_id, xctx);
        AFW_TRY {
            ctx.service_ids = apr_hash_make(afw_pool_get_apr_pool(p));
            afw_adaptor_session_retrieve_objects(
                session, NULL, &afw_s__AdaptiveServiceConf_,
                NULL /* Callback checks criteria. */,
                &ctx, impl_AdaptiveService_cb,
                NULL, p, xctx);
        }

        AFW_FINALLY {
            afw_adaptor_session_release(session, xctx);
        }

        AFW_ENDTRY;

    }

    /* Add any that are registered but not in service conf. */
    afw_environment_foreach(
        afw_environemnt_registry_type_service,
        impl_retrieve_from_registry_cb,
        &ctx,
        p, xctx);


    /* Call with a NULL object. */
    callback(NULL, context, xctx); 
}



/* Special routine for _AdaptiveService_ session get_object(). */
void
afw_service_internal_AdaptiveService_get_object (
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

    const afw_object_t *result;

    /* Get result. */
    result = afw_service_get_object(object_id, p, xctx);
 
    /* Callback with result. */
    callback(result, context, xctx);
}


/** @brief Get a service object by service id. */
AFW_DECLARE(const afw_object_t *)
afw_service_get_object(
    const afw_utf8_t *service_id,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    impl_AdaptiveService_context_t ctx;
    const afw_adaptor_session_t *session;
    const afw_object_t *result = NULL;
    const afw_service_t *service;
    const afw_utf8_t *error_message;

    result = NULL;
    memset(&ctx, 0, sizeof(ctx));
    ctx.p = p;

    /* Get service. */
    service = afw_environment_get_service(service_id, xctx);

    /*
     * If no service yet or is existing service that has a service conf, try
     * getting object via services conf adaptor.
     */
    if ((!service || service->has_service_conf) && xctx->env->conf_adaptor)
    {
        session = afw_adaptor_session_create(
            &xctx->env->conf_adaptor->adaptor_id, xctx);
        AFW_TRY {
            afw_adaptor_session_get_object(
                session, NULL, &afw_s__AdaptiveServiceConf_, service_id,
                &ctx, impl_AdaptiveService_cb,
                NULL, p, xctx);
            result = ctx.last_object;
        }

        AFW_CATCH(not_found) {
            /* Okay, result is NULL. */
        }

        AFW_CATCH_UNHANDLED{
            result = ctx.last_object;
            if (!result) {
                result = afw_object_create(p, xctx);
            }
            afw_object_set_property_as_string(result,
                &afw_s_serviceId, service_id, xctx);

            error_message = afw_utf8_create_copy(
                AFW_ERROR_THROWN->message_z, AFW_UTF8_Z_LEN,
                p, xctx);
            afw_object_set_property_as_string(result,
                &afw_s_statusMessage, error_message, xctx);

            error_message = afw_error_to_utf8(AFW_ERROR_THROWN,
                p, xctx);
            afw_object_set_property_as_string(result,
                &afw_s_statusDebug, error_message, xctx);

            afw_object_set_property(result, &afw_s_startup,
                impl_startup_value(disabled), xctx);
            afw_object_set_property(result, &afw_s_status,
                impl_status_value(error), xctx);
        }

        AFW_FINALLY {
            afw_adaptor_session_release(session, xctx);
        }

        AFW_ENDTRY;
    }


    /* If not available in services conf, return result based on registered service. */
    if (!result) {
        if (service) {
            result = afw_object_create_managed(p, xctx);
            impl_add_runtime_service_info_to_object(result, 
                service, NULL, service->properties,
                service_id, service->type, service->conf_id,
                xctx);
        }
    }

    return result;
}



/* Start a service. */
AFW_DEFINE(void)
afw_service_start_using_AdaptiveConf_cede_p(
    const afw_object_t *conf,
    const afw_utf8_t *source_location,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_service_t *service;

    /* Allocate service struct. */
    service = afw_pool_calloc_type(p, afw_service_t, xctx);
    service->p = p;
    service->source_location = &afw_s_conf;
    service->conf_source_location = source_location;

    impl_initialize_and_start_service_using_conf(service,
        &afw_s_conf, false, conf, source_location, xctx);
}



/* Start a service. */
static void
impl_start_service(
    afw_service_t *service,
    afw_xctx_t *xctx)
{
    const afw_service_t *existing_service;

    AFW_THREAD_MUTEX_LOCK(service->mutex, xctx)
    {
        afw_dateTime_set_now(&service->start_time, NULL, xctx);
        service->status = afw_service_status_starting;
        existing_service = afw_environment_get_service(
            &service->service_id, xctx);
        afw_environment_register_service(&service->service_id, service,
            xctx);
        AFW_LOG_FZ(debug, xctx, "Service " AFW_UTF8_FMT_Q " starting.",
            AFW_UTF8_FMT_ARG(&service->service_id));
        afw_service_type_start_cede_p(service->service_type,
            service->properties, service->p, xctx);
        service->status = afw_service_status_running;
        if (existing_service) {
            /** @fixme Release pool. */
        }
        AFW_LOG_FZ(info, xctx, "Service " AFW_UTF8_FMT_Q
            " successfully started.",
            AFW_UTF8_FMT_ARG(&service->service_id));
    }
    AFW_THREAD_MUTEX_UNLOCK();
}



/* Insure service is started. */
AFW_DEFINE(void)
afw_service_start(
    const afw_utf8_t *service_id,
    afw_boolean_t manual_start,
    afw_xctx_t *xctx)
{
    const afw_service_t *service;
    const afw_utf8_t *description;
    const afw_adaptor_session_t *session;
    const afw_pool_t *p;
    impl_start_context_t ctx;

    /* Clear ctx and set p. */
    afw_memory_clear(&ctx);
    ctx.manual_start = manual_start;

    service = afw_environment_get_service(service_id, xctx);

    /* If service already registered, allow error status to retry. */
    if (service &&
        service->status != afw_service_status_ready_to_start &&
        service->status != afw_service_status_stopped &&
        service->status != afw_service_status_error)
    {
        description = afw_service_status_description(service->status);
        AFW_THROW_ERROR_FZ(general, xctx,
            "Service " AFW_UTF8_FMT_Q
            " can not be started.  " AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(service_id),
            AFW_UTF8_FMT_ARG(description));
    }

    /* Should not get this condition, but fuss anyways. */
    if (!xctx->env->conf_adaptor) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "Can not start service " AFW_UTF8_FMT_Q,
            AFW_UTF8_FMT_ARG(service_id));
    }

    /* If there is a conf adaptor, try to start. */
    session = afw_adaptor_session_create(
        &xctx->env->conf_adaptor->adaptor_id, xctx);
    AFW_TRY {
        p = afw_pool_create(xctx->env->p, xctx);
        afw_adaptor_session_get_object(session, NULL,
            &afw_s__AdaptiveServiceConf_, service_id,
            &ctx, impl_start_cb, NULL, p, xctx);
    }

    AFW_CATCH_UNHANDLED{
        AFW_LOG_Z(err, AFW_ERROR_THROWN->message_z, xctx);
        AFW_ERROR_RETHROW;
    }

    AFW_FINALLY {
        afw_adaptor_session_release(session, xctx);
    }
    AFW_ENDTRY;

}




/* Stop a service. */
AFW_DEFINE(void)
afw_service_stop(
    const afw_utf8_t *service_id,
    afw_xctx_t *xctx)
{
    afw_service_t *service;
    const afw_utf8_t *description;

    service = afw_environment_get_service(service_id, xctx);
    if (!service) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "Service " AFW_UTF8_FMT_Q " is not running",
            AFW_UTF8_FMT_ARG(service_id));
    }

    AFW_THREAD_MUTEX_LOCK(service->mutex, xctx)
    {
        if (service->status == afw_service_status_running) {

            AFW_TRY {
                    service->status = afw_service_status_stopping;
                    AFW_LOG_FZ(debug, xctx, "Service " AFW_UTF8_FMT_Q
                        " stopping.",
                        AFW_UTF8_FMT_ARG(&service->service_id));
                    service->status = afw_service_status_stopping;
                    afw_service_type_stop(service->service_type,
                        service->conf_id, xctx);
                    AFW_LOG_FZ(info, xctx, "Service " AFW_UTF8_FMT_Q
                        " successfully stopped.",
                        AFW_UTF8_FMT_ARG(&service->service_id));
                    service->status = afw_service_status_stopped;
            }

            AFW_CATCH_UNHANDLED{
                AFW_LOG_Z(err, AFW_ERROR_THROWN->message_z, xctx);
                service->status =
                    (afw_service_type_related_instance_count(
                        service->service_type, service->conf_id, xctx) > 0)
                    ? afw_service_status_running
                    : afw_service_status_stopped;
                AFW_ERROR_RETHROW;
            }

            AFW_ENDTRY;
        }
        else {
            description = afw_service_status_description(service->status);
            AFW_THROW_ERROR_FZ(general, xctx,
                "Service " AFW_UTF8_FMT_Q
                " can not be stopped.  " AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(&service->service_id),
                AFW_UTF8_FMT_ARG(description));

        }
    }
    AFW_THREAD_MUTEX_UNLOCK();

}



/* Restart a service. */
static void
impl_restart_service(
    afw_service_t *service,
    afw_xctx_t *xctx)
{
    const afw_service_t *existing_service;

    AFW_THREAD_MUTEX_LOCK(service->mutex, xctx)
    {
        afw_dateTime_set_now(&service->start_time, NULL, xctx);
        service->status = afw_service_status_restarting;
        existing_service = afw_environment_get_service(
            &service->service_id, xctx);
        AFW_LOG_FZ(debug, xctx, "Service " AFW_UTF8_FMT_Q " restarting.",
            AFW_UTF8_FMT_ARG(&service->service_id));
        afw_service_type_restart_cede_p(service->service_type,
            service->properties, service->p, xctx);
        service->status = afw_service_status_running;
        if (existing_service) {
            /** @fixme Release pool. */
        }
        AFW_LOG_FZ(info, xctx, "Service " AFW_UTF8_FMT_Q
            " successfully restarted.",
            AFW_UTF8_FMT_ARG(&service->service_id));
    }
    AFW_THREAD_MUTEX_UNLOCK();
}


/* Get _AdaptiveServiceConf_ objects start initial callback. */
static afw_boolean_t
impl_restart_get_cb(
    const afw_object_t *object,
    void *context,
    afw_xctx_t *xctx)
{
    impl_start_context_t *ctx = context;
    const afw_pool_t *p;
    const afw_utf8_t *service_id;
    const afw_utf8_t *startup;
    afw_service_t *service;
    const afw_object_t *conf;
    const afw_utf8_t *source_location;

    /* If no object, return. */
    ctx->object = object;
    if (!object) return false;
    p = NULL;
    source_location = afw_object_meta_get_path(object, xctx);

    service = NULL;
    service_id = &afw_s_unknown;
    AFW_TRY {

        /*
         * Return if startup is not immediate or permanent, except when this
         * is a manual start and startup is manual.
         */
        startup = afw_object_old_get_property_as_string(object,
            &afw_s_startup, xctx);
        if (!startup ||
            (!afw_utf8_equal(startup, &afw_s_immediate) &&
                !afw_utf8_equal(startup, &afw_s_manual)))
        {
            break; /* Return. */
        }

        /* Get serviceId.  Default to object id. */
        service_id = afw_object_old_get_property_as_string(object,
            &afw_s_serviceId, xctx);
        if (!service_id) {
            service_id = afw_object_meta_get_object_id(object, xctx);
        }
        if (!service_id) {
            AFW_THROW_ERROR_FZ(general, xctx,
                AFW_UTF8_CONTEXTUAL_LABEL_FMT
                "serviceId can not be determined",
                AFW_UTF8_FMT_ARG(source_location));
        }

        /* Allocate and initialize new service instance. */
        p = afw_pool_create_multithreaded(xctx->env->p, xctx);
        service = afw_pool_calloc_type(p, afw_service_t, xctx);
        service->p = p;
        service->source_location = afw_utf8_clone(source_location,
            p, xctx);
        source_location = service->source_location;
        service->service_id.len = service_id->len;
        service->service_id.s = afw_memory_dup(
            service_id->s, service_id->len, p, xctx);
        service->has_service_conf = true;

        service->conf_source_location = afw_utf8_printf(
            p, xctx, AFW_UTF8_FMT "/conf",
            AFW_UTF8_FMT_ARG(source_location));
        conf = afw_object_old_get_property_as_object(object,
            &afw_s_conf, xctx);
        if (!conf) {
            AFW_THROW_ERROR_FZ(general, xctx,
                AFW_UTF8_CONTEXTUAL_LABEL_FMT
                "missing conf property",
                AFW_UTF8_FMT_ARG(source_location));
        }

        /* Start service and release existing service if there is one. */
        impl_initialize_and_start_service_using_conf(service,
            source_location, true, conf, service->conf_source_location, xctx);
    }

    AFW_CATCH_UNHANDLED {
        if (service) {
            service->status = afw_service_status_error;
            service->status_message = afw_utf8_create_copy(
                AFW_ERROR_THROWN->message_z, AFW_UTF8_Z_LEN,
                p, xctx);
            service->status_debug = afw_error_to_utf8(
                    AFW_ERROR_THROWN, p, xctx);
        }
        afw_error_write_log(afw_log_priority_err, AFW_ERROR_THROWN, xctx);
        AFW_LOG_FZ(err, xctx, "Service " AFW_UTF8_FMT_Q " failed to start.",
                AFW_UTF8_FMT_ARG(service_id));

        /** @fixme Deal with error_service not NULL. */
    }

    AFW_FINALLY{
        /** @fixme Deal with error_service not NULL. */
    }

    AFW_ENDTRY;

    /* Return indicating not to short circuit */
    return false;
}


/* Restart a service. */
AFW_DEFINE(void)
afw_service_restart(
    const afw_utf8_t *service_id,
    afw_xctx_t *xctx)
{
    const afw_service_t *service;
    const afw_adaptor_session_t *session;
    afw_boolean_t error;
    const afw_pool_t *p;
    impl_start_context_t ctx;

    /* Clear ctx and set p. */
    afw_memory_clear(&ctx);

    service = afw_environment_get_service(service_id, xctx);

    /* If service already registered, allow error status to retry. */
    if (!service || service->status != afw_service_status_running)
    {
        AFW_THROW_ERROR_FZ(general, xctx,
            "Service " AFW_UTF8_FMT_Q
            " cannot be restarted.  It is not running",
            AFW_UTF8_FMT_ARG(service_id));
    }

    /* If there is a conf adaptor, try to start. */
    if (!xctx->env->conf_adaptor) {
        goto error;
    }

    error = false;
    session = afw_adaptor_session_create(
        &xctx->env->conf_adaptor->adaptor_id, xctx);
    AFW_TRY {
        p = afw_pool_create(xctx->env->p, xctx);
        afw_adaptor_session_get_object(session, NULL,
            &afw_s__AdaptiveServiceConf_, service_id,
            &ctx, impl_restart_get_cb, NULL, p, xctx);
    }

    AFW_CATCH_UNHANDLED{
        error = true;
        AFW_LOG_Z(err, AFW_ERROR_THROWN->message_z, xctx);
    }

    AFW_FINALLY {
        afw_adaptor_session_release(session, xctx);
    }
    AFW_ENDTRY;
    if (error) {
        goto error;
    }
    return;

error:
    AFW_THROW_ERROR_FZ(general, xctx,
        "Error starting service " AFW_UTF8_FMT_Q,
        AFW_UTF8_FMT_ARG(service_id));
}
