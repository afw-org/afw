// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Adapter* Interface Support and Internal Functions
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_adapter.c
 * @brief Interface support and internal functions for afw_adapter*
 */

#include "afw_internal.h"


/* Declares and rti/inf defines for interface afw_service_type */
#define AFW_IMPLEMENTATION_ID "adapter"
#include "afw_service_type_impl_declares.h"


afw_adapter_internal_cache_t *
afw_adapter_internal_get_cache(afw_xctx_t *xctx)
{
    afw_adapter_internal_cache_t *cache;

    if (!xctx->cache) {
        cache = afw_xctx_calloc_type(afw_adapter_internal_cache_t, xctx);
        cache->session_cache = apr_hash_make(afw_pool_get_apr_pool(xctx->p));
        cache->transactions = apr_array_make(afw_pool_get_apr_pool(xctx->p), 5,
            sizeof(afw_adapter_internal_transaction_t *));
        xctx->cache = cache;
    }

    return xctx->cache;
}


/*
 * Set instance to the active one for its id.  Instance can be NULL to stop
 * new access to this id.
 */
static void
impl_set_instance_active(
    const afw_utf8_t *adapter_id,
    const afw_adapter_t *adapter,
    afw_xctx_t *xctx)
{
    afw_adapter_id_anchor_t *anchor;
    afw_adapter_id_anchor_t *stopping;

    stopping = NULL;
    AFW_LOCK_BEGIN(xctx->env->adapter_id_anchor_lock) {

        /*
         * Get anchor.  If not already registered, make a new one
         * in env->p and register it.
         */
        anchor = (afw_adapter_id_anchor_t *)
            afw_environment_get_adapter_id(adapter_id, xctx);
        if (!anchor) {
            if (!adapter) {
                AFW_THROW_ERROR_Z(general,
                    "adapter can not be NULL for a new anchor",
                    xctx);
            }
            anchor = afw_pool_calloc_type(
                xctx->env->p, afw_adapter_id_anchor_t, xctx);
            anchor->adapter_id = afw_utf8_clone(
                adapter_id, xctx->env->p, xctx);
            anchor->adapter_type_id = afw_utf8_clone(
                adapter->adapter_type_id, xctx->env->p, xctx);
            anchor->service_id = afw_utf8_clone(
                adapter->service_id, xctx->env->p, xctx);
            afw_environment_register_adapter_id(
                anchor->adapter_id, anchor, xctx);
        }

        /*
         * If there is already an active instance, make a copy of active
         * anchor in the active instance's pool so it will go away when
         * instance is released put it in stopping chain.
         */
        else if (anchor->adapter) {
            stopping = afw_pool_calloc_type(anchor->adapter->p,
                afw_adapter_id_anchor_t, xctx);
            afw_memory_copy(stopping, anchor);
            anchor->stopping = stopping;
        }

        /* Claim anchor.  Set/Remove _AdaptiveAdapterMetrics_ as appropriate.  */
        anchor->adapter = adapter;
        if (adapter) {
            anchor->properties = adapter->properties;
            afw_runtime_env_set_object(adapter->impl->metrics_object, true, xctx);
            anchor->reference_count = 1;
        }
        else {
            afw_runtime_remove_object(
                afw_s__AdaptiveAdapterMetrics_, adapter_id,
                xctx);
            anchor->properties = NULL;
            anchor->reference_count = 0;
        }

    }

    AFW_LOCK_END;

    /* If there was a previously active adapter, release it. */
    if (stopping) {
        afw_adapter_release(stopping->adapter, xctx);
    }
}



static const afw_adapter_t *
impl_get_reference(
    const afw_utf8_t *adapter_id,
    afw_xctx_t *xctx)
{
    afw_adapter_id_anchor_t *anchor;
    const afw_adapter_t *instance;

    AFW_LOCK_BEGIN(xctx->env->adapter_id_anchor_lock) {

        instance = NULL;
        anchor = (afw_adapter_id_anchor_t *)
            afw_environment_get_adapter_id(adapter_id, xctx);
        if (anchor) {
            instance = anchor->adapter;
            if (instance) {
                anchor->reference_count++;
            }
        }
    }

    AFW_LOCK_END;

    return instance;
}



/* Get an adapter and make sure it is started. */
AFW_DEFINE(const afw_adapter_t *)
afw_adapter_get_reference(
    const afw_utf8_t *adapter_id, afw_xctx_t *xctx)
{
    const afw_adapter_t *instance;
    const afw_utf8_t *service_id;

    instance = impl_get_reference(adapter_id, xctx);

    /* If adapter is not registered, try starting it. */
    if (!instance) {
        service_id = afw_utf8_concat(xctx->p, xctx,
            afw_s_adapter, afw_s_a_dash,
            adapter_id, NULL);
        afw_service_start(service_id, false, xctx);
        instance = impl_get_reference(adapter_id, xctx);
        if (!instance) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Adapter " AFW_UTF8_FMT_Q
                " is not available",
                AFW_UTF8_FMT_ARG(adapter_id));
        }
    }

    return instance;
}



/* Parse query criteria object appropriate for an adapter. */
AFW_DEFINE(const afw_query_criteria_t *)
afw_adapter_query_criteria_parse_object(
    const afw_object_t *query_criteria,
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *object_type_id,
    const afw_object_t *journal_entry,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_type_t *object_type;
    const afw_query_criteria_t *result;

    object_type = afw_adapter_get_object_type(
        adapter_id, object_type_id, journal_entry, xctx);

    result = afw_query_criteria_parse_AdaptiveQueryCriteria_object(
        query_criteria, object_type, p, xctx);

    return result;
}



/* Parse URL encoded RQL query string appropriate for an adapter. */
AFW_DEFINE(const afw_query_criteria_t *)
afw_adapter_query_criteria_parse_url_encoded_rql_string(
    const afw_utf8_t *url_encoded_rql_string,
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *object_type_id,
    const afw_object_t *journal_entry,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_type_t *object_type;
    const afw_query_criteria_t *result;

    object_type = afw_adapter_get_object_type(
        adapter_id, object_type_id, journal_entry, xctx);

    result = afw_query_criteria_parse_url_encoded_rql_string(
        url_encoded_rql_string, object_type, p, xctx);
    
    return result;
}



/* Release an adapter accessed by afw_adapter_get_reference(). */
AFW_DEFINE(void)
afw_adapter_release(const afw_adapter_t *instance, afw_xctx_t *xctx)
{
    afw_adapter_id_anchor_t *anchor;
    afw_adapter_id_anchor_t *previous_anchor;
    afw_boolean_t destroy;

    destroy = false;
    AFW_LOCK_BEGIN(xctx->env->adapter_id_anchor_lock) {

        /* Find anchor for this adapter instance. */
        for (
            anchor = (afw_adapter_id_anchor_t *)
                afw_environment_get_adapter_id(
                    &instance->adapter_id, xctx),
            previous_anchor = NULL;
            anchor && anchor->adapter != instance;
            previous_anchor = anchor,
            anchor = anchor->stopping);

        if (anchor) {
            anchor->reference_count--;
            if (anchor->reference_count <= 0) {
                destroy = true;
                anchor->reference_count = 0;
                if (!previous_anchor) {
                    anchor->adapter = NULL;
                }
                else {
                    previous_anchor->stopping = anchor->stopping;
                }
            }
        }
    }

    AFW_LOCK_END;

    if (destroy) {
        afw_adapter_destroy(instance, xctx);
    }
}


/* Create an adapter session. */
AFW_DEFINE(const afw_adapter_session_t *)
afw_adapter_session_create(
    const afw_utf8_t *adapter_id, afw_xctx_t *xctx)
{
    const afw_adapter_t *adapter;
    const afw_adapter_session_t *session;

    adapter = afw_adapter_get_reference(adapter_id, xctx);

    session = afw_adapter_create_adapter_session(
        adapter, xctx);

    return session;
}


/* Release an adapter session created by afw_adapter_session_create(). */
AFW_DEFINE(void)
afw_adapter_session_release(
    const afw_adapter_session_t *session,
    afw_xctx_t *xctx)
{
    const afw_adapter_t *adapter;
    const afw_adapter_internal_cache_t *cache;
    afw_adapter_internal_session_cache_t *session_cache;

    adapter = session->adapter;

    /* If session in cache, remove it. */
    cache = afw_adapter_internal_get_cache(xctx);
    if (cache) {
        session_cache = apr_hash_get(cache->session_cache,
            adapter->adapter_id.s, adapter->adapter_id.len);
        if (session_cache && session == session_cache->session) {
            apr_hash_set(cache->session_cache,
                adapter->adapter_id.s, adapter->adapter_id.len, NULL);
        }
    }

    /* Destroy session and release adapter. */
    afw_adapter_session_destroy(session, xctx);
    afw_adapter_release(adapter, xctx);
}


/* Get an active cached session for adapter_id. */
static afw_adapter_internal_session_cache_t *
impl_get_adapter_session_cache(const afw_utf8_t *adapter_id,
    afw_boolean_t begin_transaction, afw_xctx_t *xctx)
{
    afw_adapter_internal_session_cache_t *session_cache;
    const afw_adapter_internal_cache_t *cache;
    afw_adapter_internal_transaction_t *transaction;
    const afw_adapter_transaction_t *new_transaction;
    int i;

    /* Get cached session. */
    cache = afw_adapter_internal_get_cache(xctx);
    session_cache = apr_hash_get(cache->session_cache,
        adapter_id->s, adapter_id->len);

    /* If there is not already one, create one. */
    if (!session_cache) {
        session_cache = afw_xctx_calloc_type(
            afw_adapter_internal_session_cache_t, xctx);
        session_cache->session = afw_adapter_session_create(adapter_id, xctx);
        if (session_cache->session) {
            apr_hash_set(cache->session_cache, adapter_id->s, adapter_id->len,
                session_cache);
        }
    }

    /* If session and begin_transaction, make sure transactions is started. */
    if (session_cache->session && begin_transaction) {
        for (i = 0; i < cache->transactions->nelts; i++) {
            transaction = APR_ARRAY_IDX(cache->transactions, i,
                afw_adapter_internal_transaction_t *);
            if (afw_utf8_equal(transaction->adapter_id, adapter_id))
            {
                break;
            }
        }
        if (i >= cache->transactions->nelts) {
            new_transaction = afw_adapter_session_begin_transaction(
                session_cache->session,
                xctx);
            if (new_transaction) {
                transaction = afw_xctx_calloc_type(
                    afw_adapter_internal_transaction_t, xctx);
                transaction->adapter_id =
                    &session_cache->session->adapter->adapter_id;
                transaction->transaction = new_transaction;
                APR_ARRAY_PUSH(cache->transactions,
                    const afw_adapter_internal_transaction_t *) = transaction;
            }
        }
    }

    /* Error if no valid adapter session. */
    if (!session_cache->session) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "Unable to start session for adapter id " AFW_UTF8_FMT_Q,
            AFW_UTF8_FMT_ARG(adapter_id));
    }

    /* Return result. */
    return session_cache;
}

/* Get an active cached session for adapter_id. */
AFW_DEFINE(const afw_adapter_session_t *)
afw_adapter_session_get_cached(const afw_utf8_t *adapter_id,
    afw_boolean_t begin_transaction, afw_xctx_t *xctx)
{
    afw_adapter_internal_session_cache_t *session_cache;

    session_cache = impl_get_adapter_session_cache(adapter_id,
        begin_transaction, xctx);

    /* Return result. */
    return (session_cache) ? session_cache->session : NULL;
}



/* Commit changes and release cached sessions and objects. */
AFW_DEFINE(void)
afw_adapter_session_commit_and_release_cache(afw_boolean_t abort,
    afw_xctx_t *xctx)
{
    const afw_adapter_internal_cache_t *cache;
    const afw_adapter_internal_transaction_t **t0;
    const afw_adapter_internal_transaction_t **t;
    afw_adapter_internal_session_cache_t *session_cache;
    apr_hash_index_t *hi;
    const void * key;
    apr_ssize_t klen;

    cache = xctx->cache;
    if (!cache) return;

    /* Call commit for all active transaction in reverse order of begin. */
    for (t0 = (const afw_adapter_internal_transaction_t **)
        cache->transactions->elts - 1,
        t = t0 + cache->transactions->nelts; t > t0; t--)
    {
        if (!abort) {
            afw_adapter_transaction_commit((*t)->transaction, xctx);
        }
        afw_adapter_transaction_release((*t)->transaction, xctx);
    }

    /* Release all active sessions in no particular order. */
    for (hi = apr_hash_first(afw_pool_get_apr_pool(xctx->p), cache->session_cache);
        hi; hi = apr_hash_next(hi))
    {
        apr_hash_this(hi, &key, &klen, (void **)&session_cache);
        afw_adapter_session_release(session_cache->session, xctx);
    }

    /* If there is a runtime adapter session, release it. */
    if (cache->runtime_adapter_session) {
        afw_adapter_session_release(cache->runtime_adapter_session, xctx);
    }

    /* Clear pointer in xctx to cache struct. */
    xctx->cache = NULL;
}



/* Get and cache _AdaptiveObjectType_ object. */
AFW_DEFINE(const afw_object_type_t *)
afw_adapter_get_object_type(
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *object_type_id,
    const afw_object_t *journal_entry,
    afw_xctx_t *xctx)
{
    const afw_object_type_t *result;
    const afw_object_t *object;
    const afw_utf8_t *embedded_object_type_id;
    const afw_adapter_object_type_cache_t *adapter_cache;
    afw_adapter_internal_session_cache_t *session_cache;
    afw_object_type_property_type_t *property_type;
    const afw_adapter_t *adapter;
    const afw_pool_t *p;
    afw_boolean_t final_result;

    /** @fixme This may change so that object types are reused unless changed. */

    /* Pool starts out as xctx's p, but may change to adapter's p. */
    p = xctx->p;

    /* Get adapter_id's session cache, adapter, and impl. */
    session_cache = impl_get_adapter_session_cache(adapter_id, false, xctx);
    if (!session_cache) {
        return NULL;
    }
    adapter = session_cache->session->adapter;

    result = NULL;
    final_result = false;

    /* Return object type if already cached for life of adapter. */
    adapter_cache = afw_adapter_session_get_object_type_cache_interface(
        session_cache->session, xctx);
    if (adapter_cache) {
        result = afw_adapter_object_type_cache_get(
            adapter_cache, object_type_id, &final_result, xctx);
        if (result) {
            return result;
        }
        if (final_result) {
            return NULL;
        }
        if (adapter_cache->all_object_types_immutable) {
            p = adapter->p;
        }
    }

    /* Return object type if already cached for session. */
    else if (session_cache->object_types_ht) {
        result = apr_hash_get(session_cache->object_types_ht,
            object_type_id->s, object_type_id->len);
        if (result) {
            return result;
        }
    }

    /* Get special runtime object types to prevent loop. */
    result = NULL;
    if (afw_utf8_equal(object_type_id, afw_s__AdaptiveObjectType_) ||
        afw_utf8_equal(object_type_id, afw_s__AdaptiveValueMeta_))
    {
        object = afw_runtime_get_object(
            afw_s__AdaptiveObjectType_, object_type_id, xctx);
        result = afw_object_type_internal_create(
            adapter, object, p, xctx);
    }

    /* For other object types, get object type object via adapter. */
    else {
        object = NULL;
        AFW_TRY {
            object = afw_adapter_get_object(adapter_id,
                afw_s__AdaptiveObjectType_,
                object_type_id, &afw_object_options_composite_and_defaults,
                NULL, journal_entry, NULL, p, xctx);
        }
        AFW_CATCH(not_found) {
            /* Ok, object returned will be NULL. */
        }
        AFW_ENDTRY;

        if (object) {
            result = afw_object_type_internal_create(
                adapter, object, p, xctx);
        }
    }

    /*
     * If result, cache in adapter or session. Then, resolve any embedded object
     * types base on data_type_parameter (default _AdaptiveObject_).
     */
    if (result) {
        if (result && adapter_cache && adapter_cache->all_object_types_immutable)
        {
            afw_adapter_object_type_cache_set(adapter_cache, result, xctx);
        }
        else {
            if (!session_cache->object_types_ht) {
                session_cache->object_types_ht =
                    apr_hash_make(afw_pool_get_apr_pool(p));
            }
            apr_hash_set(session_cache->object_types_ht,
                object_type_id->s, object_type_id->len, result);
        }

        for (
            property_type =
                (afw_object_type_property_type_t *)result->first_property_type;
            property_type;
            property_type =
                (afw_object_type_property_type_t *)property_type->next
            )
        {
            if (property_type->data_type == afw_data_type_object &&
                !property_type->object_type)
            {
                embedded_object_type_id = (property_type->data_type_parameter)
                    ? property_type->data_type_parameter
                    : afw_s__AdaptiveObject_;
                property_type->object_type = afw_adapter_get_object_type(
                    adapter_id, embedded_object_type_id, journal_entry, xctx);
            }
        }

        property_type =
            (afw_object_type_property_type_t *)result->other_properties;
        if (property_type &&
            property_type->data_type == afw_data_type_object &&
            !property_type->object_type)
        {
            embedded_object_type_id = (property_type->data_type_parameter)
                ? property_type->data_type_parameter
                : afw_s__AdaptiveObject_;
            property_type->object_type = afw_adapter_get_object_type(
                adapter_id, embedded_object_type_id, journal_entry, xctx);
        }
    }

    /* Return result. */
    return result;
}



/*
 * {
 *    type           : "adapter",
 *    adapterType    : "afw_runtime",
 *    adapterId      : "afw"
 * }
 */
void
afw_adapter_internal_register_afw_adapter(afw_xctx_t *xctx)
{
    const afw_object_t *conf;
    const afw_pool_t *p;

    p = afw_pool_create(xctx->env->p, xctx);
    conf = afw_object_create_unmanaged(p, xctx);
    afw_object_set_property_as_string(conf,
        afw_s_type, afw_s_adapter, xctx);
    afw_object_set_property_as_string(conf,
        afw_s_adapterType, afw_s_afw_runtime, xctx);
    afw_object_set_property_as_string(conf,
        afw_s_adapterId, afw_s_afw, xctx);
    afw_object_set_property_as_string(conf,
        afw_s_sourceLocation, afw_s_a_Core_afw_adapter, xctx);
    afw_adapter_internal_conf_type_create_cede_p(afw_s_adapter,
        conf, afw_s_a_Core_afw_adapter, p, xctx);
}


/* Configuration handler for entry type "adapter". */
void
afw_adapter_internal_conf_type_create_cede_p(
    const afw_utf8_t *type,
    const afw_object_t *conf,
    const afw_utf8_t *source_location,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_adapter_id_anchor_t *anchor;
    const afw_utf8_t *adapter_id;

    /* Get adapter_id. */
    adapter_id = afw_object_old_get_property_as_string(conf,
        afw_s_adapterId, xctx);
    if (!adapter_id) {
        AFW_THROW_ERROR_FZ(general, xctx,
            AFW_UTF8_CONTEXTUAL_LABEL_FMT
            "adapterId properties is required",
            AFW_UTF8_FMT_ARG(source_location));
    }

    /* See if adapter id already used. */
    anchor = afw_environment_get_adapter_id(adapter_id, xctx);
    if (anchor) {

        /* If adapter id is afw, handle special. */
        if (afw_utf8_equal(adapter_id, afw_s_afw)) {
            AFW_LOG_FZ(warning, xctx,
                AFW_UTF8_CONTEXTUAL_LABEL_FMT
                "adapter id afw is automatically defined.  Entry ignored.",
                AFW_UTF8_FMT_ARG(source_location));
            return;
        }

        /* Any other adapter id already registered is an error. */
        else {
            AFW_THROW_ERROR_FZ(general, xctx,
                AFW_UTF8_FMT " adapter " AFW_UTF8_FMT_Q " is already running",
                source_location->len, source_location->s,
                adapter_id->len, adapter_id->s);
        }
    }

    /* Start adapter. */
    afw_service_start_using_AdaptiveConf_cede_p(
        conf, source_location, p, xctx);
}



/* Adapt and apply view if requested and object is not NULL. */
void
afw_adapter_internal_process_object_from_adapter(
    const afw_object_t * *adapted_object,
    const afw_object_t * *view,
    afw_adapter_internal_object_cb_context_t *ctx,
    const afw_object_t *object,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{ 
    const afw_utf8_t *entity_path;

    *adapted_object = object;
    afw_object_get_reference(object, xctx);
    entity_path = afw_object_path_make(
        ctx->adapter_id,
        ctx->object_type_id,
        (ctx->object_id)
            ? ctx->object_id
            : afw_object_meta_get_object_id(object, xctx),
        p,
        xctx);

    /** @fixme Do assess control here that might add meta and remove properties.
    Might be in view??? */

    /* Make view based on options. */
    *view = afw_object_view_create(*adapted_object, entity_path,
        ctx->options, p, xctx);

    /** @fixme Need to add releases at correct place by caller. */
}



void
afw_adapter_internal_register_service_type(afw_xctx_t *xctx)
{
    afw_service_type_t *self;

    self = afw_xctx_calloc_type(afw_service_type_t, xctx);
    self->inf = &impl_afw_service_type_inf;
    afw_memory_copy(&self->service_type_id, afw_s_adapter);
    self->conf_type = afw_environment_get_conf_type(afw_s_adapter, xctx);
    if (!self->conf_type) {
        AFW_THROW_ERROR_Z(general, "conf_type must already be registered",
            xctx);
    }
    self->title = afw_s_a_service_type_adapter_title;
    self->conf_type_object = afw_runtime_get_object(afw_s__AdaptiveConfType_,
        afw_s_adapter, xctx);
    afw_environment_register_service_type(afw_s_adapter, self, xctx);
}



/*
 * Implementation of method related_instance_count of interface afw_service_type.
 */
afw_integer_t
impl_afw_service_type_related_instance_count (
    const afw_service_type_t * instance,
    const afw_utf8_t * id,
    afw_xctx_t *xctx)
{
    afw_adapter_id_anchor_t *anchor;
    afw_adapter_id_anchor_t *stopping;
    afw_integer_t result = 0;

    AFW_LOCK_BEGIN(xctx->env->adapter_id_anchor_lock) {

        anchor = (afw_adapter_id_anchor_t *)
            afw_environment_get_adapter_id(id, xctx);
        if (anchor) {
            for (stopping = anchor->stopping;
                stopping;
                stopping = stopping->stopping)
            {
                result++;
            }
            if (anchor->adapter) {
                result++;
            }
            else {
                result = -result;
            }
        }
    }
    AFW_LOCK_END;

    return result;
}



/*
 * Implementation of method start_cede_p of interface afw_service_type.
 */
void
impl_afw_service_type_start_cede_p (
    const afw_service_type_t * instance,
    const afw_object_t * properties,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    const afw_adapter_factory_t *factory;
    const afw_adapter_t *adapter;
    const afw_utf8_t *adapter_type;

    adapter_type = afw_object_old_get_property_as_utf8(properties,
        afw_s_adapterType, p, xctx);
    if (!adapter_type) {
        AFW_THROW_ERROR_Z(general,
            "parameter adapterType missing",
            xctx);
    }

    factory = afw_environment_get_adapter_type(adapter_type, xctx);
    if (!factory) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "adapterType " AFW_UTF8_FMT_Q
            " is not a registered adapter type",
            AFW_UTF8_FMT_ARG(adapter_type));
    }
   
    /* Create adapter. */
    adapter = afw_adapter_factory_create_adapter_cede_p(factory,
        properties, p, xctx);

    /* Make this adapter the active one. */
    impl_set_instance_active(&adapter->adapter_id, adapter, xctx);
}



/*
 * Implementation of method stop of interface afw_service_type.
 */
void
impl_afw_service_type_stop (
    const afw_service_type_t * instance,
    const afw_utf8_t * id,
    afw_xctx_t *xctx)
{
    impl_set_instance_active(id, NULL, xctx);
}



/*
 * Implementation of method restart_cede_p of interface afw_service_type.
 */
void
impl_afw_service_type_restart_cede_p (
    const afw_service_type_t * instance,
    const afw_object_t * properties,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    /* Count on already running. Start will restart if necessary. */
    impl_afw_service_type_start_cede_p(instance, properties, p, xctx);
}
