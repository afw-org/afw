// See the 'COPYING' file in the project root for licensing information.
/*
 * Helpers for afw_adaptor implementation development
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_adaptor_impl.c
 * @brief Helpers for afw_adaptor implementation development
 */

#include "afw_internal.h"
#include "afw_model_location.h"

/* Declares and rti/inf defines for interface afw_adaptor */
#define AFW_IMPLEMENTATION_ID "adaptor_impl"
#include "afw_adaptor_impl_declares.h"
#define AFW_ADAPTOR_SESSION_SELF_T afw_adaptor_impl_session_t
#include "afw_adaptor_session_impl_declares.h"
#include "afw_adaptor_journal_impl_declares.h"

#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_adaptor_impl_object_type_cache_inf
#include "afw_adaptor_object_type_cache_impl_declares.h"
#undef AFW_IMPLEMENTATION_INF_SPECIFIER
#undef AFW_IMPLEMENTATION_INF_LABEL


typedef struct {
    apr_hash_t *ht;
    void *original_context;
    afw_object_cb_t original_callback;
    void *impl_callback_context;
    afw_object_cb_t impl_callback;
    afw_integer_t retrieved_object_count;

    const afw_pool_t *p;
    afw_adaptor_impl_session_t *session;
    const afw_adaptor_impl_request_t *impl_request;
    const afw_utf8_t *resource_id;
    const afw_value_t *action_id_value;

} impl_request_context_t;



static void
impl_set_trace_flag_fields(
    afw_adaptor_t *adaptor,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p = adaptor->p;
    const afw_pool_t *env_p = xctx->env->p;
    const afw_utf8_t *flag_id;
    const afw_utf8_t *brief;
    const afw_utf8_t *description;
    const afw_utf8_t *included_by_flag_id;
    const afw_flag_t *flag;

    /* adaptor_id_trace_flag_id */
    adaptor->trace_flag_id =
        afw_utf8_printf(p, xctx,
            "trace:adaptorId:" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(&adaptor->adaptor_id));

    /* adaptor_id_detail_flag_id */
    adaptor->detail_flag_id =
        afw_utf8_printf(p, xctx,
            "trace:adaptorId:" AFW_UTF8_FMT ":detail",
            AFW_UTF8_FMT_ARG(&adaptor->adaptor_id));

    /* adaptor_id_detail_flag_index */
    flag = afw_environment_get_flag(
        adaptor->detail_flag_id, xctx);
    if (!flag) {
        flag_id = afw_utf8_clone(
            adaptor->detail_flag_id,
            env_p, xctx);

        brief = afw_utf8_printf(env_p, xctx,
            "Detail trace of adaptor id " AFW_UTF8_FMT_Q,
            AFW_UTF8_FMT_ARG(&adaptor->adaptor_id));

        description = afw_utf8_printf(env_p, xctx,
            "This produces a basic plus detail trace "
            "of adaptor id " AFW_UTF8_FMT_Q ".",
            AFW_UTF8_FMT_ARG(&adaptor->adaptor_id));

        included_by_flag_id = afw_utf8_printf(env_p, xctx,
            "trace:adaptorType:" AFW_UTF8_FMT ":detail",
            AFW_UTF8_FMT_ARG(adaptor->adaptor_type_id));

        afw_environment_register_flag(flag_id, brief, description,
            included_by_flag_id, xctx);

        flag = afw_environment_get_flag(
            adaptor->detail_flag_id, xctx);
    }
    adaptor->detail_flag_index = flag->flag_index;


    /* adaptor_id_trace_flag_index */
    flag = afw_environment_get_flag(
        adaptor->trace_flag_id, xctx);
    if (!flag) {
        flag_id = afw_utf8_clone(
            adaptor->trace_flag_id,
            env_p, xctx);

        brief = afw_utf8_printf(env_p, xctx,
            "Trace adaptor id " AFW_UTF8_FMT_Q,
            AFW_UTF8_FMT_ARG(&adaptor->adaptor_id));

        description = afw_utf8_printf(env_p, xctx,
            "This produces a basic trace of adaptor id " AFW_UTF8_FMT_Q ".",
            AFW_UTF8_FMT_ARG(&adaptor->adaptor_id));

        included_by_flag_id = afw_utf8_printf(env_p, xctx,
            "trace:adaptorType:" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(adaptor->adaptor_type_id));

        afw_environment_register_flag(flag_id, brief, description,
            included_by_flag_id, xctx);

        afw_flag_add_included_by(adaptor->trace_flag_id,
            adaptor->detail_flag_id, xctx);

        flag = afw_environment_get_flag(
            adaptor->trace_flag_id, xctx);
    }
    adaptor->trace_flag_index = flag->flag_index;

}



AFW_DEFINE(void)
afw_adaptor_impl_throw_property_invalid(
    const afw_adaptor_t *adaptor,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx)
{
    AFW_THROW_ERROR_FZ(general, xctx,
        AFW_UTF8_FMT
        "Configuration type='adaptor', adaptorType="
        AFW_UTF8_FMT_Q
        " property name "
        AFW_UTF8_FMT_Q
        " is not valid.",
        AFW_UTF8_FMT_ARG(adaptor->impl->source_location),
        AFW_UTF8_FMT_ARG(&adaptor->adaptor_id),
        AFW_UTF8_FMT_ARG(property_name));
}



AFW_DEFINE(void)
afw_adaptor_impl_throw_property_required(
    const afw_adaptor_t *adaptor,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx)
{
    AFW_THROW_ERROR_FZ(general, xctx,
        AFW_UTF8_FMT
        " Configuration type='adaptor', adaptorType=" AFW_UTF8_FMT_Q
        " property name " AFW_UTF8_FMT_Q
        " is required.",
        AFW_UTF8_FMT_ARG(adaptor->impl->source_location),
        AFW_UTF8_FMT_ARG(&adaptor->adaptor_id),
        AFW_UTF8_FMT_ARG(property_name));
}



/* Get common variable callback. */
static const afw_value_t *
impl_adaptor_common_variable_get_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    const afw_value_t *result;

    result = NULL;
    /** @fixme Add code. */

    return result;
}



/* Push adaptor qualifiers to xctx. */
AFW_DEFINE(void)
afw_adaptor_impl_push_qualifiers(
    const afw_adaptor_t *adaptor,
    afw_xctx_t *xctx)
{
    afw_xctx_qualifier_stack_qualifier_object_push(afw_s_adaptor,
        adaptor->properties, true, xctx->p, xctx);
    afw_xctx_qualifier_stack_qualifier_push(afw_s_adaptor, NULL, true,
        impl_adaptor_common_variable_get_cb, (void *)adaptor,
        xctx->p, xctx);
    if (adaptor->impl->custom_variables) {
        afw_xctx_qualifier_stack_qualifier_object_push(afw_s_custom,
            adaptor->impl->custom_variables, true,
            xctx->p, xctx);
    }
}


AFW_DEFINE(afw_adaptor_t *)
afw_adaptor_impl_create_cede_p(
    const afw_adaptor_inf_t *inf,
    afw_size_t instance_size,
    const afw_object_t *properties,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_adaptor_impl_t *impl;
    afw_adaptor_t *adaptor;
    const afw_adaptor_session_t *temp_session;
    const afw_utf8_t *s;
    const afw_authorization_handler_t *authorization_handler;
    afw_boolean_t b;

    /* Allocate new adaptor instance. */
    adaptor = (afw_adaptor_t *)afw_pool_calloc(p,
        (instance_size != 0) ? instance_size : sizeof(afw_adaptor_t),
        xctx);
    adaptor->inf = &impl_afw_adaptor_inf;
    adaptor->p = p;
    adaptor->adaptor_type_id = afw_object_old_get_property_as_string(
        properties, afw_s_adaptorType, xctx);
    impl = afw_pool_calloc_type(p, afw_adaptor_impl_t, xctx);
    adaptor->impl = impl;
    impl->adaptor = adaptor;
    impl->wrapped_inf = inf;

    /* Prepare properties. */
    adaptor->properties = afw_environment_prepare_conf_type_properties(
        properties, xctx);

    /* Get adaptorType from properties. */
    adaptor->adaptor_type_id = afw_object_old_get_property_as_string(
        properties, afw_s_adaptorType, xctx);

    /* Get source location.  Default it to adaptor. */
    impl->source_location = afw_object_old_get_property_as_string(
        properties, afw_s_sourceLocation, xctx);
    if (!impl->source_location) {
        impl->source_location = afw_s_adaptor;
    }

    /* Get adaptor_id from parameters. */
    s = afw_object_old_get_property_as_utf8(properties,
        afw_s_adaptorId, p, xctx);

    if (!s) {
        AFW_THROW_ERROR_FZ(general, xctx,
            AFW_UTF8_FMT " requires 'id' property.",
            AFW_UTF8_FMT_ARG(impl->source_location));
    }
    afw_memory_copy(&adaptor->adaptor_id, s);
    impl->adaptor_id = &adaptor->adaptor_id;

    /* Create lock. */
    s = afw_utf8_printf(p, xctx,
        "adaptor_id:" AFW_UTF8_FMT_Q,
        AFW_UTF8_FMT_ARG(impl->adaptor_id));
    if (!afw_environment_get_lock(s, xctx)) {
        impl->adaptor_lock_rw = afw_lock_create_rw_and_register(
            afw_utf8_clone(s, xctx->env->p, xctx),
            afw_utf8_printf(xctx->env->p, xctx,
                "Adaptor id " AFW_UTF8_FMT_Q " read/write lock",
                AFW_UTF8_FMT_ARG(impl->adaptor_id)),
            afw_utf8_printf(xctx->env->p, xctx,
                "Read/write lock used internally by adaptor id "
                    AFW_UTF8_FMT_Q " implementation",
                AFW_UTF8_FMT_ARG(impl->adaptor_id)),
            xctx);
    }

    /* Service id. */
    adaptor->service_id = afw_utf8_printf(p, xctx,
        "adaptor-" AFW_UTF8_FMT,
        AFW_UTF8_FMT_ARG(impl->adaptor_id));

    /* Set trace fields */
    impl_set_trace_flag_fields(adaptor, xctx);

    /* Trace create */
    afw_trace_fz(1, adaptor->trace_flag_index, adaptor, xctx,
        "adaptorId " AFW_UTF8_FMT_Q " is being created",
        AFW_UTF8_FMT_ARG(&adaptor->adaptor_id));

    /* As default, allow read _AdaptiveObjectType_/_AdaptiveObjectType_. */
    impl->supported_core_object_types = apr_hash_make(
        afw_pool_get_apr_pool(p));
    afw_adaptor_impl_set_supported_core_object_type(adaptor,
        afw_s__AdaptiveObjectType_, true, false, xctx);

    /* Get optional authorizationHandlerId from parameters. */
    impl->authorization_handler_id =
        afw_object_old_get_property_as_string(adaptor->properties,
            afw_s_authorizationHandlerId, xctx);
    if (impl->authorization_handler_id)
    {
        authorization_handler = NULL;
        AFW_TRY{
            authorization_handler = afw_authorization_handler_get_reference(
                impl->authorization_handler_id, xctx);
        }
        AFW_FINALLY {
            if (authorization_handler) {
                afw_authorization_handler_destroy(authorization_handler, xctx);
            }
        }
        AFW_ENDTRY;
        if (!authorization_handler) {
            AFW_THROW_ERROR_FZ(general, xctx,
                AFW_UTF8_FMT
                " authorizationHandlerId  " AFW_UTF8_FMT_Q
                " is not startable",
                AFW_UTF8_FMT_ARG(impl->source_location),
                AFW_UTF8_FMT_ARG(impl->authorization_handler_id));
        }
    }

    /* Get optional journalAdaptorId from parameters. */
    impl->journal_adaptor_id = afw_object_old_get_property_as_string(
        adaptor->properties, afw_s_journalAdaptorId, xctx);
    if (impl->journal_adaptor_id) {
        AFW_LOG_FZ(debug, xctx,
            "Adaptor " AFW_UTF8_FMT_Q
            " specified journalAdaptorId " AFW_UTF8_FMT_Q
            ".",
            AFW_UTF8_FMT_ARG(&adaptor->adaptor_id),
            AFW_UTF8_FMT_ARG(impl->journal_adaptor_id));
    }

    if (impl->journal_adaptor_id &&
        !afw_utf8_equal(s, &adaptor->adaptor_id))
    {
        temp_session = NULL;
        AFW_TRY{
            temp_session = afw_adaptor_session_create(
                impl->journal_adaptor_id, xctx);
            if (!temp_session ||
                !afw_adaptor_session_get_journal_interface(temp_session, xctx))
            {
                AFW_THROW_ERROR_FZ(general, xctx,
                    AFW_UTF8_FMT
                    " Adaptor id " AFW_UTF8_FMT_Q
                    " specified in journalAdaptorId property is not startable"
                    " or does not support journaling.",
                    AFW_UTF8_FMT_ARG(impl->source_location),
                    AFW_UTF8_FMT_ARG(impl->journal_adaptor_id));
            }
        }
        AFW_FINALLY{
            if (temp_session) {
                afw_adaptor_session_release(temp_session, xctx);
            }
        }
        AFW_ENDTRY;
    }

    /* Authorization */
    impl->authorization = afw_object_get_property(
        properties, afw_s_authorization, xctx);

    /** @fixme Reuse if already exists or reuse correct pool. */
    /* Create runtime metrics object and set in properties. */
    impl->metrics_object = afw_runtime_object_create_indirect(
        afw_s__AdaptiveAdaptorMetrics_,
        &adaptor->adaptor_id, impl, xctx->env->p, xctx);

    /* If this is layout adaptor id, allow layout object type. */
    if (xctx->env->layout_adaptor_id &&
        afw_utf8_equal(xctx->env->layout_adaptor_id, &adaptor->adaptor_id))
    {
        afw_adaptor_impl_set_supported_core_object_type(adaptor,
            afw_s__AdaptiveLayoutComponentType_, true, true, xctx);
        afw_adaptor_impl_set_supported_core_object_type(adaptor,
            afw_s__AdaptiveLayoutComponent_, false, true, xctx);
    }

    /* If isModelLocation is true, provide appropriate object types. */
    b = afw_object_old_get_property_as_boolean_deprecated(properties,
        afw_s_isModelLocation, xctx);
    if (b) {
        impl->model_location = afw_model_location_create(adaptor, p, xctx);
        afw_adaptor_impl_set_supported_core_object_type(adaptor,
            afw_s__AdaptiveTemplateProperties_, false, true, xctx);
        afw_adaptor_impl_set_supported_core_object_type(adaptor,
            afw_s__AdaptiveModel_, true, true, xctx);
        afw_adaptor_impl_set_supported_core_object_type(adaptor,
            afw_s__AdaptiveModelObjectTypes_, false, true, xctx);
        afw_adaptor_impl_set_supported_core_object_type(adaptor,
            afw_s__AdaptiveModelObjectType_, false, true, xctx);
        afw_adaptor_impl_set_supported_core_object_type(adaptor,
            afw_s__AdaptiveModelPropertyTypes_, false, true, xctx);
        afw_adaptor_impl_set_supported_core_object_type(adaptor,
            afw_s__AdaptiveModelPropertyType_, false, true, xctx);
    }

    /** @fixme have way for extension to add these.
    If isPolicyLocation is true, provide appropriate object types.
    b = afw_object_old_get_property_as_boolean_deprecated(properties,
        afw_s_isPolicyLocation, xctx);
    if (b) {
        impl->policy_location = afw_authorization_policy_internal_location_create(
            adaptor, p, xctx);
        afw_adaptor_impl_set_supported_core_object_type(adaptor,
            afw_s__AdaptiveAuthorizationAdviceExpression_, false, true,
            xctx);
        afw_adaptor_impl_set_supported_core_object_type(adaptor,
            afw_s__AdaptiveAuthorizationCombinerParameter_, false, true,
            xctx);
        afw_adaptor_impl_set_supported_core_object_type(adaptor,
            afw_s__AdaptiveAuthorizationObligationExpression_, false, true,
            xctx);
        afw_adaptor_impl_set_supported_core_object_type(adaptor,
            afw_s__AdaptiveAuthorizationPolicy_, true, true,
            xctx);
        afw_adaptor_impl_set_supported_core_object_type(adaptor,
            afw_s__AdaptiveAuthorizationPolicyIssuer_, false, true,
            xctx);
        afw_adaptor_impl_set_supported_core_object_type(adaptor,
            afw_s__AdaptiveAuthorizationPolicySet_, true, true,
            xctx);
        afw_adaptor_impl_set_supported_core_object_type(adaptor,
            afw_s__AdaptiveAuthorizationRule_, false, true,
            xctx);
        afw_adaptor_impl_set_supported_core_object_type(adaptor,
            afw_s__AdaptiveTemplateProperties_, false, true, xctx);
    }
     */

    /* Return new adaptor. */
    return adaptor;
}


/* Call object cb for list. */
AFW_DEFINE(void)
afw_adaptor_impl_call_object_cb_from_list(
    const afw_array_t *list,
    void *context,
    afw_object_cb_t callback,
    afw_xctx_t *xctx)
{
    const afw_object_t *obj;
    const afw_iterator_t *iterator;

    /* Call callback for each object in list. */
    for (iterator = NULL;;) {
        obj = afw_array_of_object_get_next(
            list, &iterator, xctx);
        if (!obj) break;
        callback(obj, context, xctx);
    }

    /* Call one move time with NULL object.*/
    callback(NULL, context, xctx);
}


/* Determine whether a journal entry is applicable to a consumer. */
AFW_DEFINE(afw_boolean_t)
afw_adaptor_impl_is_journal_entry_applicable(
    const afw_adaptor_journal_t *instance,
    const afw_object_t *entry,
    const afw_object_t *consumer,
    const afw_value_t *const *filter,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *peerIdInEntry;
    const afw_utf8_t *peerIdInConsumer;
    afw_boolean_t is_applicable;

    is_applicable = true;

    peerIdInEntry = afw_object_old_get_property_as_string(entry,
        afw_s_peerId, xctx);
    if (peerIdInEntry) {
        peerIdInConsumer = afw_object_old_get_property_as_string(consumer,
            afw_s_peerId, xctx);
        if (!peerIdInConsumer) {
            AFW_THROW_ERROR_Z(general,
                "Missing peerId property in "
                AFW_OBJECT_Q_OBJECT_TYPE_ID_PROVISIONING_PEER
                " object",
                xctx);
        }
        /* Not applicable if loopback. */
        is_applicable = !afw_utf8_equal(peerIdInEntry, peerIdInConsumer);
    }

    return is_applicable;
}



static void
impl_update_allow(
    const afw_object_t *object,
    afw_boolean_t allow_entity,
    afw_boolean_t allow_write,
    afw_xctx_t *xctx)
{
    afw_object_set_property(object, afw_s_allowEntity,
        (allow_entity) ? afw_boolean_v_true : afw_boolean_v_false, xctx);
    afw_object_set_property(object, afw_s_allowAdd,
        (allow_write) ? afw_boolean_v_true : afw_boolean_v_false, xctx);
    afw_object_set_property(object, afw_s_allowChange,
        (allow_write) ? afw_boolean_v_true : afw_boolean_v_false, xctx);
    afw_object_set_property(object, afw_s_allowDelete,
        (allow_write) ? afw_boolean_v_true : afw_boolean_v_false, xctx);
}



/* Indicates support of a core object type. */
AFW_DEFINE(void)
afw_adaptor_impl_set_supported_core_object_type(
    const afw_adaptor_t *adaptor,
    const afw_utf8_t *object_type_id,
    afw_boolean_t allow_entity,
    afw_boolean_t allow_write,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p = adaptor->p;
    apr_hash_t *ht;
    afw_adaptor_impl_core_object_type_t *e;
    const afw_utf8_t *path;
    afw_value_array_t *parent_paths;

    /* Ignore call for afw adaptor.  Will cause parentPaths loops. */
    if (afw_utf8_equal(&adaptor->adaptor_id, afw_s_afw)) {
        return;
    }

    ht = adaptor->impl->supported_core_object_types;

    e = apr_hash_get(ht, object_type_id->s, object_type_id->len);
    if (e) {
        e->allow_entity = allow_entity;
        e->allow_write = allow_write;
        impl_update_allow(e->object, allow_entity, allow_write, xctx);
    }
    else {
        e = afw_pool_calloc_type(p,
            afw_adaptor_impl_core_object_type_t, xctx);
        e->object_type_id = object_type_id;
        e->object = afw_runtime_get_object(afw_s__AdaptiveObjectType_,
            object_type_id, xctx);
        if (!e->object) {
            AFW_THROW_ERROR_Z(general, "Invalid object type id", xctx);
        }
        /** @fixme This is where object wrapper would be good to just hold deltas. */
        e->object = afw_object_create_clone(e->object, p, xctx);
        path = afw_object_meta_get_path(e->object, xctx);
        afw_object_meta_set_ids(e->object, &adaptor->adaptor_id,
            afw_s__AdaptiveObjectType_, object_type_id, xctx);
        parent_paths = afw_value_allocate_unmanaged_array(p, xctx);
        parent_paths->internal = afw_array_create_wrapper_for_array(
            (const void *)path, false, afw_data_type_anyURI, 1, p, xctx);
        afw_object_meta_set_parent_paths(e->object, parent_paths, xctx);

        impl_update_allow(e->object, allow_entity, allow_write, xctx);

        apr_hash_set(ht, e->object_type_id->s, e->object_type_id->len, e);
    }
}



/* Create a generic object type object. */
AFW_DEFINE(const afw_object_t *)
afw_adaptor_impl_generic_object_type_object_get(
    const afw_adaptor_t *adaptor,
    const afw_utf8_t *object_type_id,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *result;
    const afw_object_t *other_properties;
    const afw_utf8_t *objectType;

    /** @fixme These can be cached off of impl. */

    result = afw_object_create_managed(p, xctx);
    objectType = afw_utf8_clone(object_type_id, result->p, xctx);
    afw_object_meta_set_ids(result, &adaptor->adaptor_id,
        afw_s__AdaptiveObjectType_, objectType, xctx);
    afw_object_set_property_as_string(result, afw_s_objectType,
        objectType, xctx);
    afw_object_set_property_as_boolean(result,
        afw_s_allowAdd, afw_boolean_v_true, xctx);
    afw_object_set_property_as_boolean(result,
        afw_s_allowChange, afw_boolean_v_true, xctx);
    afw_object_set_property_as_boolean(result,
        afw_s_allowDelete, afw_boolean_v_true, xctx);
    other_properties = afw_object_create_embedded(result,
        afw_s_otherProperties, xctx);
    afw_object_set_property_as_boolean(other_properties,
        afw_s_allowQuery, afw_boolean_v_true, xctx);
    afw_object_set_property_as_boolean(other_properties,
        afw_s_allowWrite, afw_boolean_v_true, xctx);

    return result;
}



/* Initialize object type cache instance. */
AFW_DEFINE(void)
afw_adaptor_impl_object_type_cache_initialize(
    afw_adaptor_object_type_cache_t *object_type_cache,
    const afw_adaptor_object_type_cache_inf_t *inf,
    const afw_adaptor_session_t *session,
    afw_boolean_t all_object_types_immutable,
    afw_xctx_t *xctx)
{
    object_type_cache->inf = inf;
    object_type_cache->session = session;
    object_type_cache->all_object_types_immutable =
        all_object_types_immutable;
}


/* Set object types fully loaded. */
AFW_DEFINE(void)
afw_adaptor_impl_set_object_types_fully_loaded(
    const afw_adaptor_t *adaptor,
    afw_xctx_t *xctx)
{
    afw_adaptor_impl_t *impl;

    impl = (afw_adaptor_impl_t *)adaptor->impl;
    impl->object_types_fully_loaded = true;
}


/*
 * Implementation of method get for interface afw_adaptor_object_type_cache.
 */
const afw_object_type_t *
impl_afw_adaptor_object_type_cache_get(
    const afw_adaptor_object_type_cache_t *instance,
    const afw_utf8_t *object_type_id,
    afw_boolean_t *final_result,
    afw_xctx_t *xctx)
{
    const afw_object_type_t *result;
    const afw_adaptor_t *adaptor;
    afw_adaptor_impl_t *impl;

    adaptor = instance->session->adaptor;
    impl = (afw_adaptor_impl_t *)adaptor->impl;
    *final_result = impl->object_types_fully_loaded;

    if (!impl->object_types_ht) {
        return NULL;
    }

    AFW_ADAPTOR_IMPL_LOCK_READ_BEGIN(adaptor) {
        result = apr_hash_get(adaptor->impl->object_types_ht,
            object_type_id->s, object_type_id->len);
    }
    AFW_ADAPTOR_IMPL_LOCK_READ_END;

    return result;
}



/*
 * Implementation of method set for interface afw_adaptor_object_type_cache.
 */
void
impl_afw_adaptor_object_type_cache_set(
    const afw_adaptor_object_type_cache_t *instance,
    const afw_object_type_t *object_type,
    afw_xctx_t *xctx)
{
    const afw_adaptor_t *adaptor;
    afw_adaptor_impl_t *impl;

    adaptor = instance->session->adaptor;
    impl = (afw_adaptor_impl_t *)adaptor->impl;

    if (!impl->object_types_ht) {
        impl->object_types_ht = apr_hash_make(
            afw_pool_get_apr_pool(adaptor->p));
    }

    AFW_ADAPTOR_IMPL_LOCK_WRITE_BEGIN(adaptor) {
        apr_hash_set(impl->object_types_ht,
            object_type->object_type_id->s,
            object_type->object_type_id->len,
            object_type);
    }
    AFW_ADAPTOR_IMPL_LOCK_WRITE_END;
}



/*
 * Implementation of method destroy of interface afw_adaptor.
 */
void
impl_afw_adaptor_destroy(
    const afw_adaptor_t *instance,
    afw_xctx_t *xctx)
{
    afw_adaptor_impl_t *impl = (afw_adaptor_impl_t *)instance->impl;

    /** @fixme Add common prologue code. */

    /* Call wrapped instance method. */
    impl->wrapped_inf->destroy(instance, xctx);

    /** @fixme Add common epilogue code. */
}



/*
 * Implementation of method create_adaptor_session of interface afw_adaptor.
 */
const afw_adaptor_session_t *
impl_afw_adaptor_create_adaptor_session(
    const afw_adaptor_t *instance,
    afw_xctx_t *xctx)
{
    afw_adaptor_impl_t *impl = (afw_adaptor_impl_t *)instance->impl;
    AFW_ADAPTOR_SESSION_SELF_T *self;

    /* Create session self. */
    self = afw_xctx_calloc_type(AFW_ADAPTOR_SESSION_SELF_T, xctx);
    self->pub.adaptor = instance;
    self->pub.inf = &impl_afw_adaptor_session_inf;
    self->pub.p = xctx->p;

    /** @fixme Add common prologue code. */

    /* Call wrapped instance method. */
    self->wrapped_session =
        impl->wrapped_inf->create_adaptor_session(instance, xctx);

    /** @fixme Add common epilogue code. */

    /* Return result. */
    return &self->pub;
}



/*
 * Implementation of method get_additional_metrics of interface afw_adaptor.
 */
const afw_object_t *
impl_afw_adaptor_get_additional_metrics(
    const afw_adaptor_t *instance,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_adaptor_impl_t *impl = (afw_adaptor_impl_t *)instance->impl;
    const afw_object_t *result;

    /** @fixme Add common prologue code. */

    /* Call wrapped instance method. */
    result = impl->wrapped_inf->get_additional_metrics(instance,
        p, xctx);

    /** @fixme Add common epilogue code. */

    /* Return result. */
    return result;
}


/*
 * Implementation of method destroy of interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_destroy(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    afw_xctx_t *xctx)
{
    /** @fixme Add common prologue code. */

    /* Call wrapped instance method. */
    afw_adaptor_session_destroy(self->wrapped_session, xctx);

    /** @fixme Add common epilogue code. */
}



static void
impl_check_authorization(
    impl_request_context_t *ctx,
    afw_xctx_t *xctx)
{
    const afw_value_t *resource_id_value;
    const afw_value_t *object_value;

    resource_id_value = afw_value_create_unmanaged_string(
        ctx->resource_id,
        ctx->p, xctx);

    object_value = NULL;
    if (ctx->impl_request && ctx->impl_request->request) {
        object_value = afw_value_create_unmanaged_object(
            ctx->impl_request->request,
            ctx->p, xctx);
    }

    afw_authorization_check(
        true, NULL,
        resource_id_value,
        object_value,
        ctx->action_id_value,
        ctx->p, xctx);
}



static afw_boolean_t
impl_authorization_cb(
    const afw_object_t *object,
    void *context,
    afw_xctx_t *xctx)
{
    impl_request_context_t *ctx = context;
    const afw_utf8_t *resource_id;
    const afw_value_t *resource_id_value;
    const afw_value_t *object_value;

    if (!object) {
        return ctx->impl_callback(object, ctx->impl_callback_context, xctx);
    }

    resource_id = afw_object_meta_get_path(object, xctx);
    if (!resource_id) {
        AFW_THROW_ERROR_Z(general, "Missing path", xctx);
    }
    resource_id_value = afw_value_create_unmanaged_string(resource_id, ctx->p, xctx);
    object_value = afw_value_create_unmanaged_object(object, ctx->p, xctx);
    afw_authorization_check(true, NULL,
        resource_id_value, object_value,
        afw_authorization_action_id_read, ctx->p, xctx);

    return ctx->impl_callback(object, ctx->impl_callback_context, xctx);
}



static afw_boolean_t
impl_special_object_handling_cb(
    const afw_object_t *object,
    void *context,
    afw_xctx_t *xctx)
{
    impl_request_context_t *ctx = context;
    const afw_utf8_t *type;
    const afw_utf8_t *subtype;
    const afw_object_t *conf;
    const afw_service_type_t *service_type;
    const afw_utf8_t *object_type_id;

    if (object) {

        /* _AdaptiveServiceConf_ special processing for conf object type. */
        if (afw_utf8_equal(
            afw_object_meta_get_object_type_id(object, xctx),
            afw_s__AdaptiveServiceConf_))
        {
            conf = afw_object_old_get_property_as_object(object,
                afw_s_conf, xctx);
            if (conf) {
                type = afw_object_old_get_property_as_string(conf,
                    afw_s_type, xctx);
                if (type) {
                    service_type = afw_environment_get_service_type(
                        type, xctx);
                    if (service_type) {
                        subtype = afw_object_old_get_property_as_string(
                            conf,
                            service_type->conf_type->subtype_property_name,
                            xctx);
                        if (subtype) {
                            object_type_id = afw_utf8_printf(
                                object->p, xctx,
                                "_AdaptiveConf_" AFW_UTF8_FMT
                                "_" AFW_UTF8_FMT,
                                AFW_UTF8_FMT_ARG(type),
                                AFW_UTF8_FMT_ARG(subtype));
                            afw_object_meta_set_object_type_id(conf,
                                object_type_id, xctx);
                        }
                    }
                }
            }

        }

    }

    return ctx->original_callback(object, ctx->original_context, xctx);
}


static afw_boolean_t
impl_no_duplicate_object_type_cb(
    const afw_object_t *object,
    void *context,
    afw_xctx_t *xctx)
{
    impl_request_context_t *ctx = context;
    const afw_utf8_t *object_id;
    afw_adaptor_impl_core_object_type_t *e;

    /* Skip object types that have already been provided. */
    if (object) {
        object_id = afw_object_meta_get_object_id(object, xctx);
        e = apr_hash_get(ctx->ht, object_id->s, object_id->len);
        if (e) {
            return false;
        }
    }

    /* Call original callback and return result. */
    return ctx->original_callback(object, ctx->original_context,
        xctx);
}


static afw_boolean_t
impl_limit_returned_objects_cb(
    const afw_object_t *object,
    void *context,
    afw_xctx_t *xctx)
{
    impl_request_context_t *ctx = context;

    /** @fixme This is a temporary way to limit # of objects returned. */
    ctx->retrieved_object_count++;
    if (ctx->retrieved_object_count > 100) {
        return true;
        //AFW_THROW_ERROR_Z(payload_too_large,
        //    "Object retrieve limit exceeded.",
        //    xctx);
    }

    /** @fixme Check authorization */


    /* Call original callback and return result. */
    return ctx->original_callback(object, ctx->original_context,
        xctx);
}


/*
 * Implementation of method retrieve_objects for interface
 * afw_adaptor_session.
 */
void
impl_afw_adaptor_session_retrieve_objects(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_query_criteria_t *criteria,
    void *context,
    afw_object_cb_t callback,
    const afw_object_t *adaptor_type_specific,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_adaptor_t *adaptor = self->pub.adaptor;
    afw_adaptor_impl_t *impl = (afw_adaptor_impl_t *)adaptor->impl;
    apr_hash_t *ht;
    apr_hash_index_t *hi;
    afw_adaptor_impl_core_object_type_t *e;
    afw_utf8_t object_id;
    impl_request_context_t ctx;

    /* Initialize ctx. */
    afw_memory_clear(&ctx);
    ctx.p = p;
    ctx.original_context = context;
    ctx.original_callback = callback;
    ctx.impl_request = impl_request;
    ctx.resource_id = NULL;
    if (impl_request) {
        ctx.resource_id = impl_request->resource_id;
    }
    if (!ctx.resource_id) {
        ctx.resource_id = afw_utf8_printf(p, xctx,
            "/" AFW_UTF8_FMT "/" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(&adaptor->adaptor_id),
            AFW_UTF8_FMT_ARG(object_type_id));
    }
    ctx.action_id_value = afw_authorization_action_id_query;

    /* Trace begin */
    afw_trace_fz(1, adaptor->trace_flag_index, self->wrapped_session, xctx,
        "begin retrieve_objects "
        AFW_UTF8_FMT_Q,
        AFW_UTF8_FMT_ARG(ctx.resource_id));

    /** @fixme Add common prologue code. */
    afw_atomic_integer_increment(&impl->retrieve_objects_count);

    /* Authorize */
    impl_check_authorization(&ctx, xctx);

    /*
     * Handle _AdaptiveObjectType_.
     *
     * Allow only specific object types to be returned for retrieve.
     *
     * Note: get allows all to be returned.
     */
    if (afw_utf8_equal(object_type_id, afw_s__AdaptiveObjectType_))
    {
        ht = self->pub.adaptor->impl->supported_core_object_types;
        for (hi = apr_hash_first(afw_pool_get_apr_pool(p), ht);
            hi;
            hi = apr_hash_next(hi))
        {
            apr_hash_this(hi,
                (const void **)& object_id.s, (apr_ssize_t *)& object_id.len,
                (void **)& e);
            if (afw_query_criteria_test_object(e->object, criteria, p, xctx))
            {
                callback(e->object, context, xctx);
            }
        }
        ctx.ht = ht;
        ctx.impl_callback_context = &ctx;
        ctx.impl_callback = impl_no_duplicate_object_type_cb;
        afw_adaptor_session_retrieve_objects(
            self->wrapped_session, impl_request,
            object_type_id, criteria,
            &ctx, impl_authorization_cb,
            adaptor_type_specific,
            p, xctx);
    }

    /** @fixme discuss this defensive check, it's null when indexing */
    else if (object_type_id && afw_utf8_starts_with(object_type_id, afw_s__Adaptive)) {
        ctx.impl_callback_context = &ctx;
        ctx.impl_callback = impl_special_object_handling_cb;
        afw_adaptor_session_retrieve_objects(
            self->wrapped_session, impl_request,
            object_type_id, criteria,
            &ctx, impl_authorization_cb,
            adaptor_type_specific,
            p, xctx);
    }

    else {
        ctx.impl_callback_context = &ctx;
        ctx.impl_callback = impl_limit_returned_objects_cb;
        afw_adaptor_session_retrieve_objects(
            self->wrapped_session, impl_request,
            object_type_id, criteria,
            &ctx, impl_authorization_cb,
            adaptor_type_specific,
            p, xctx);
    }


    /** @fixme Add common epilogue code. */

    /* Trace end */
    afw_trace_fz(1, adaptor->trace_flag_index, self->wrapped_session, xctx,
        "end retrieve_objects "
        AFW_UTF8_FMT_Q,
        AFW_UTF8_FMT_ARG(ctx.resource_id));
}



/*
 * Implementation of method get_object for interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_get_object(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    void *context,
    afw_object_cb_t callback,
    const afw_object_t *adaptor_type_specific,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_adaptor_t *adaptor = self->pub.adaptor;
    afw_adaptor_impl_t *impl = (afw_adaptor_impl_t *)adaptor->impl;
    const afw_object_t *object_type_object;
    afw_adaptor_impl_core_object_type_t *e;
    impl_request_context_t ctx;

    /* Initialize ctx. */
    afw_memory_clear(&ctx);
    ctx.p = p;
    ctx.original_context = context;
    ctx.original_callback = callback;
    ctx.impl_request = impl_request;
    ctx.resource_id = NULL;
    if (impl_request) {
        ctx.resource_id = impl_request->resource_id;
    }
    if (!ctx.resource_id) {
        ctx.resource_id = afw_utf8_printf(p, xctx,
            "/" AFW_UTF8_FMT "/" AFW_UTF8_FMT "/" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(&adaptor->adaptor_id),
            AFW_UTF8_FMT_ARG(object_type_id),
            AFW_UTF8_FMT_ARG(object_id));
    }
    ctx.action_id_value = afw_authorization_action_id_query;

    /* Trace begin */
    afw_trace_fz(1, adaptor->trace_flag_index, self->wrapped_session, xctx,
        "begin get_object "
        AFW_UTF8_FMT_Q,
        AFW_UTF8_FMT_ARG(ctx.resource_id));

    /** @fixme Add common prologue code. */
    afw_atomic_integer_increment(&impl->get_object_count);

    /* Authorize */
    impl_check_authorization(&ctx, xctx);

    /* Get any core object types from runtime environment. */
    if (afw_utf8_equal(object_type_id, afw_s__AdaptiveObjectType_) &&
        afw_utf8_starts_with(object_id, afw_s__Adaptive))
    {
        /* If object type might have different allows, return it. */
        e = apr_hash_get(self->pub.adaptor->impl->supported_core_object_types,
            object_id->s, object_id->len);
        if (e) {
            ctx.impl_callback_context = context;
            ctx.impl_callback = callback;
            impl_authorization_cb(e->object, &ctx, xctx);
            goto end_trace;
        }

        /* Return /afw/_AdaptiveObjectType_/<objectId> asis. */
        object_type_object = afw_runtime_get_object(
            object_type_id, object_id, xctx);
        if (object_type_object) {
            ctx.impl_callback_context = context;
            ctx.impl_callback = callback;
            impl_authorization_cb(object_type_object, &ctx, xctx);
        }

        /* Other special pattern object type ids that begin with _Adaptive. */
        else {
            ctx.impl_callback_context = context;
            ctx.impl_callback = callback;
            afw_adaptor_session_get_object(
                self->wrapped_session, impl_request,
                object_type_id, object_id,
                &ctx, impl_authorization_cb,
                adaptor_type_specific,
                p, xctx);
        }
        goto end_trace;
    }

    if (afw_utf8_starts_with(object_type_id, afw_s__Adaptive)) {
        ctx.impl_callback_context = &ctx;
        ctx.impl_callback = impl_special_object_handling_cb;
        afw_adaptor_session_get_object(
            self->wrapped_session, impl_request,
            object_type_id, object_id,
            &ctx, impl_authorization_cb,
            adaptor_type_specific,
            p, xctx);
    }

    else {
        /* Call wrapped instance method. */
        ctx.impl_callback_context = context;
        ctx.impl_callback = callback;
        afw_adaptor_session_get_object(
            self->wrapped_session, impl_request,
            object_type_id, object_id,
            &ctx, impl_authorization_cb,
            adaptor_type_specific,
            p, xctx);
    }

    /** @fixme Add common epilogue code. */

end_trace:
    /* Trace end */
    afw_trace_fz(1, adaptor->trace_flag_index, self->wrapped_session, xctx,
        "end get_object "
        AFW_UTF8_FMT_Q,
        AFW_UTF8_FMT_ARG(ctx.resource_id));
}



/*
 * Implementation of method add_object for interface afw_adaptor_session.
 */
const afw_utf8_t *
impl_afw_adaptor_session_add_object(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *suggested_object_id,
    const afw_object_t *object,
    const afw_object_t *adaptor_type_specific,
    afw_xctx_t *xctx)
{
    const afw_adaptor_t *adaptor = self->pub.adaptor;
    afw_adaptor_impl_t *impl = (afw_adaptor_impl_t *)adaptor->impl;
    const afw_utf8_t *result;
    impl_request_context_t ctx;

    /* Initialize ctx. */
    afw_memory_clear(&ctx);
    ctx.p = self->pub.p;
    ctx.impl_request = impl_request;
    ctx.resource_id = NULL;
    if (impl_request) {
        ctx.resource_id = impl_request->resource_id;
    }
    if (!ctx.resource_id) {
        ctx.resource_id = afw_utf8_printf(xctx->p, xctx,
            "/" AFW_UTF8_FMT "/" AFW_UTF8_FMT "/" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(&adaptor->adaptor_id),
            AFW_UTF8_FMT_ARG(object_type_id),
            AFW_UTF8_FMT_OPTIONAL_ARG(suggested_object_id));
    }
    ctx.action_id_value = afw_authorization_action_id_create;

    /* Trace begin */
    afw_trace_fz(1, adaptor->trace_flag_index, self->wrapped_session, xctx,
        "begin add_object "
        AFW_UTF8_FMT_Q,
        AFW_UTF8_FMT_ARG(ctx.resource_id));

    /** @fixme Add common prologue code. */
    afw_atomic_integer_increment(&impl->add_object_count);

    /* Authorize */
    impl_check_authorization(&ctx, xctx);

    /* Call wrapped instance method. */
    result = afw_adaptor_session_add_object(
        self->wrapped_session, impl_request,
        object_type_id, suggested_object_id, object,
        adaptor_type_specific, xctx);

    /** @fixme Add common epilogue code. */

    /* Trace end */
    afw_trace_fz(1, adaptor->trace_flag_index, self->wrapped_session, xctx,
        "end add_object "
        AFW_UTF8_FMT_Q,
        AFW_UTF8_FMT_ARG(ctx.resource_id));

    /* Return result. */
    return result;
}



/*
 * Implementation of method modify_object for interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_modify_object(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_adaptor_modify_entry_t *const *entry,
    const afw_object_t *adaptor_type_specific,
    afw_xctx_t *xctx)
{
    const afw_adaptor_t *adaptor = self->pub.adaptor;
    afw_adaptor_impl_t *impl = (afw_adaptor_impl_t *)adaptor->impl;
    impl_request_context_t ctx;

    /* Initialize ctx. */
    afw_memory_clear(&ctx);
    ctx.p = self->pub.p;
    ctx.impl_request = impl_request;
    ctx.resource_id = NULL;
    if (impl_request) {
        ctx.resource_id = impl_request->resource_id;
    }
    if (!ctx.resource_id) {
        ctx.resource_id = afw_utf8_printf(xctx->p, xctx,
            "/" AFW_UTF8_FMT "/" AFW_UTF8_FMT "/" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(&adaptor->adaptor_id),
            AFW_UTF8_FMT_ARG(object_type_id),
            AFW_UTF8_FMT_ARG(object_id));
    }
    ctx.action_id_value = afw_authorization_action_id_modify;

    /* Trace begin */
    afw_trace_fz(1, adaptor->trace_flag_index, self->wrapped_session, xctx,
        "begin modify_object "
        AFW_UTF8_FMT_Q,
        AFW_UTF8_FMT_ARG(ctx.resource_id));

    /** @fixme Add common prologue code. */
    afw_atomic_integer_increment(&impl->modify_object_count);

    /* Authorize */
    impl_check_authorization(&ctx, xctx);

    /* Call wrapped instance method. */
    afw_adaptor_session_modify_object(
        self->wrapped_session, impl_request,
        object_type_id, object_id, entry, adaptor_type_specific,
        xctx);

    /** @fixme Add common epilogue code. */

    /* Trace end */
    afw_trace_fz(1, adaptor->trace_flag_index, self->wrapped_session, xctx,
        "end modify_object "
        AFW_UTF8_FMT_Q,
        AFW_UTF8_FMT_ARG(ctx.resource_id));
}



/*
 * Implementation of method replace_object for interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_replace_object(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_object_t *replacement_object,
    const afw_object_t *adaptor_type_specific,
    afw_xctx_t *xctx)
{
    const afw_adaptor_t *adaptor = self->pub.adaptor;
    afw_adaptor_impl_t *impl = (afw_adaptor_impl_t *)adaptor->impl;
    impl_request_context_t ctx;

    /* Initialize ctx. */
    afw_memory_clear(&ctx);
    ctx.p = self->pub.p;
    ctx.impl_request = impl_request;
    ctx.resource_id = NULL;
    if (impl_request) {
        ctx.resource_id = impl_request->resource_id;
    }
    if (!ctx.resource_id) {
        ctx.resource_id = afw_utf8_printf(xctx->p, xctx,
            "/" AFW_UTF8_FMT "/" AFW_UTF8_FMT "/" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(&adaptor->adaptor_id),
            AFW_UTF8_FMT_ARG(object_type_id),
            AFW_UTF8_FMT_ARG(object_id));
    }
    ctx.action_id_value = afw_authorization_action_id_modify;

    /* Trace begin */
    afw_trace_fz(1, adaptor->trace_flag_index, self->wrapped_session, xctx,
        "begin replace_object "
        AFW_UTF8_FMT_Q,
        AFW_UTF8_FMT_ARG(ctx.resource_id));

    /** @fixme Add common prologue code. */
    afw_atomic_integer_increment(&impl->replace_object_count);

    /* Authorize */
    impl_check_authorization(&ctx, xctx);

    /* Call wrapped instance method. */
    afw_adaptor_session_replace_object(
        self->wrapped_session, impl_request,
        object_type_id, object_id, replacement_object,
        adaptor_type_specific, xctx);

    /** @fixme Add common epilogue code. */

    /* Trace end */
    afw_trace_fz(1, adaptor->trace_flag_index, self->wrapped_session, xctx,
        "end replace_object "
        AFW_UTF8_FMT_Q,
        AFW_UTF8_FMT_ARG(ctx.resource_id));
}



/*
 * Implementation of method delete_object for interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_delete_object(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_object_t *adaptor_type_specific,
    afw_xctx_t *xctx)
{
    const afw_adaptor_t *adaptor = self->pub.adaptor;
    afw_adaptor_impl_t *impl = (afw_adaptor_impl_t *)adaptor->impl;
    impl_request_context_t ctx;

    /* Initialize ctx. */
    afw_memory_clear(&ctx);
    ctx.p = self->pub.p;
    ctx.impl_request = impl_request;
    ctx.resource_id = NULL;
    if (impl_request) {
        ctx.resource_id = impl_request->resource_id;
    }
    if (!ctx.resource_id) {
        ctx.resource_id = afw_utf8_printf(xctx->p, xctx,
            "/" AFW_UTF8_FMT "/" AFW_UTF8_FMT "/" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(&adaptor->adaptor_id),
            AFW_UTF8_FMT_ARG(object_type_id),
            AFW_UTF8_FMT_ARG(object_id));
    }
    ctx.action_id_value = afw_authorization_action_id_delete;

    /* Trace begin */
    afw_trace_fz(1, adaptor->trace_flag_index, self->wrapped_session, xctx,
        "begin delete_object "
        AFW_UTF8_FMT_Q,
        AFW_UTF8_FMT_ARG(ctx.resource_id));

    /** @fixme Add common prologue code. */
    afw_atomic_integer_increment(&impl->delete_object_count);

    /* Authorize */
    impl_check_authorization(&ctx, xctx);

    /* Call wrapped instance method. */
    afw_adaptor_session_delete_object(
        self->wrapped_session, impl_request,
        object_type_id, object_id, adaptor_type_specific, xctx);

    /** @fixme Add common epilogue code. */

    /* Trace end */
    afw_trace_fz(1, adaptor->trace_flag_index, self->wrapped_session, xctx,
        "end delete_object "
        AFW_UTF8_FMT_Q,
        AFW_UTF8_FMT_ARG(ctx.resource_id));
}



/*
 * Implementation of method begin_transaction of interface afw_adaptor_session.
 */
const afw_adaptor_transaction_t *
impl_afw_adaptor_session_begin_transaction(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    afw_xctx_t *xctx)
{
    const afw_adaptor_transaction_t *result;

    /** @fixme Add common prologue code. */

    /* Call wrapped instance method. */
    result = afw_adaptor_session_begin_transaction(
        self->wrapped_session,
        xctx);

    /** @fixme Add common epilogue code. */

    /* Return result. */
    return result;
}



/*
 * Implementation of method get_journal_interface of interface afw_adaptor_session.
 */
const afw_adaptor_journal_t *
impl_afw_adaptor_session_get_journal_interface(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    afw_xctx_t *xctx)
{
    /* Call wrapped instance method. */
    if (!self->wrapped_journal) {
        self->wrapped_journal = afw_adaptor_session_get_journal_interface(
            self->wrapped_session, xctx);
        if (self->wrapped_journal) {
            self->journal_pub.inf = &impl_afw_adaptor_journal_inf;
            self->journal_pub.session = &self->pub;
        }
    }

    /* Return result. */
    return (self->wrapped_journal)
        ? &self->journal_pub
        : NULL;
}



/*
 * Implementation of method get_key_value_interface of interface afw_adaptor_session.
 */
const afw_adaptor_key_value_t *
impl_afw_adaptor_session_get_key_value_interface(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    afw_xctx_t *xctx)
{
    const afw_adaptor_key_value_t *result;

    /** @fixme Add common prologue code. */

    /* Call wrapped instance method. */
    result = afw_adaptor_session_get_key_value_interface(self->wrapped_session,
        xctx);

    /** @fixme Add common epilogue code. */

    /* Return result. */
    return result;
}



/*
 * Implementation of method get_index_interface of interface afw_adaptor_session.
 */
const afw_adaptor_impl_index_t *
impl_afw_adaptor_session_get_index_interface(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    afw_xctx_t *xctx)
{
    const afw_adaptor_impl_index_t *result;

    /** @fixme Add common prologue code. */

    /* Call wrapped instance method. */
    result = afw_adaptor_session_get_index_interface(self->wrapped_session,
        xctx);

    /** @fixme Add common epilogue code. */

    /* Return result. */
    return result;
}


/*
 * Implementation of method get_object_type_cache_interface for interface
 * afw_adaptor.
 */
const afw_adaptor_object_type_cache_t *
impl_afw_adaptor_session_get_object_type_cache_interface(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    afw_xctx_t *xctx)
{
    const afw_adaptor_object_type_cache_t *result;

    result = afw_adaptor_session_get_object_type_cache_interface(
        self->wrapped_session, xctx);

    return result;
}


/*
 * Implementation of method add_entry for interface afw_adaptor_journal.
 */
const afw_utf8_t *
impl_afw_adaptor_journal_add_entry(
    const afw_adaptor_journal_t * instance,
    const afw_adaptor_impl_request_t * impl_request,
    const afw_object_t * entry,
    afw_xctx_t *xctx)
{
    afw_adaptor_impl_session_t *self =
        (afw_adaptor_impl_session_t *)(instance->session);
    const afw_utf8_t *result;

    /** @fixme authorization check */

    result = afw_adaptor_journal_add_entry(self->wrapped_journal,
        impl_request, entry, xctx);
    return result;
}

/*
 * Implementation of method get_entry for interface afw_adaptor_journal.
 */
void
impl_afw_adaptor_journal_get_entry(
    const afw_adaptor_journal_t * instance,
    const afw_adaptor_impl_request_t * impl_request,
    afw_adaptor_journal_option_t option,
    const afw_utf8_t * consumer_id,
    const afw_utf8_t * entry_cursor,
    afw_size_t limit,
    const afw_object_t * response,
    afw_xctx_t *xctx)
{
    afw_adaptor_impl_session_t *self =
        (afw_adaptor_impl_session_t *)(instance->session);

    /** @fixme authorization check */

    afw_adaptor_journal_get_entry(self->wrapped_journal,
        impl_request, option, consumer_id, entry_cursor,
        limit, response, xctx);
}

/*
 * Implementation of method mark_entry_consumed for interface
 * afw_adaptor_journal.
 */
void
impl_afw_adaptor_journal_mark_entry_consumed(
    const afw_adaptor_journal_t * instance,
    const afw_adaptor_impl_request_t * impl_request,
    const afw_utf8_t * consumer_id,
    const afw_utf8_t * entry_cursor,
    afw_xctx_t *xctx)
{
    afw_adaptor_impl_session_t *self =
        (afw_adaptor_impl_session_t *)(instance->session);

    /** @fixme authorization check */

    afw_adaptor_journal_mark_entry_consumed(self->wrapped_journal,
        impl_request, consumer_id, entry_cursor, xctx);
}
