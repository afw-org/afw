// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Adaptor* Interface Support and Internal Functions
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_adaptor.c
 * @brief Interface support and internal functions for afw_adaptor*
 */

#include "afw_internal.h"


/* Declares and rti/inf defines for interface afw_service_type */
#define AFW_IMPLEMENTATION_ID "adaptor"
#include "afw_service_type_impl_declares.h"


afw_adaptor_internal_cache_t *
afw_adaptor_internal_get_cache(afw_xctx_t *xctx)
{
    afw_adaptor_internal_cache_t *cache;

    if (!xctx->cache) {
        cache = afw_xctx_calloc_type(afw_adaptor_internal_cache_t, xctx);
        cache->session_cache = apr_hash_make(afw_pool_get_apr_pool(xctx->p));
        cache->transactions = apr_array_make(afw_pool_get_apr_pool(xctx->p), 5,
            sizeof(afw_adaptor_internal_transaction_t *));
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
    const afw_utf8_t *adaptor_id,
    const afw_adaptor_t *adaptor,
    afw_xctx_t *xctx)
{
    afw_adaptor_id_anchor_t *anchor;
    afw_adaptor_id_anchor_t *stopping;

    stopping = NULL;
    AFW_LOCK_BEGIN(xctx->env->adaptor_id_anchor_lock) {

        /*
         * Get anchor.  If not already registered, make a new one
         * in env->p and register it.
         */
        anchor = (afw_adaptor_id_anchor_t *)
            afw_environment_get_adaptor_id(adaptor_id, xctx);
        if (!anchor) {
            if (!adaptor) {
                AFW_THROW_ERROR_Z(general,
                    "adaptor can not be NULL for a new anchor",
                    xctx);
            }
            anchor = afw_pool_calloc_type(
                xctx->env->p, afw_adaptor_id_anchor_t, xctx);
            anchor->adaptor_id = afw_utf8_clone(
                adaptor_id, xctx->env->p, xctx);
            anchor->adaptor_type_id = afw_utf8_clone(
                adaptor->adaptor_type_id, xctx->env->p, xctx);
            anchor->service_id = afw_utf8_clone(
                adaptor->service_id, xctx->env->p, xctx);
            afw_environment_register_adaptor_id(
                anchor->adaptor_id, anchor, xctx);
        }

        /*
         * If there is already an active instance, make a copy of active
         * anchor in the active instance's pool so it will go away when
         * instance is released put it in stopping chain.
         */
        else if (anchor->adaptor) {
            stopping = afw_pool_calloc_type(anchor->adaptor->p,
                afw_adaptor_id_anchor_t, xctx);
            afw_memory_copy(stopping, anchor);
            anchor->stopping = stopping;
        }

        /* Claim anchor.  Set/Remove _AdaptiveAdaptorMetrics_ as appropriate.  */
        anchor->adaptor = adaptor;
        if (adaptor) {
            anchor->properties = adaptor->properties;
            afw_runtime_env_set_object(adaptor->impl->metrics_object, true, xctx);
            anchor->reference_count = 1;
        }
        else {
            afw_runtime_remove_object(
                afw_s__AdaptiveAdaptorMetrics_, adaptor_id,
                xctx);
            anchor->properties = NULL;
            anchor->reference_count = 0;
        }

    }

    AFW_LOCK_END;

    /* If there was a previously active adaptor, release it. */
    if (stopping) {
        afw_adaptor_release(stopping->adaptor, xctx);
    }
}



static const afw_adaptor_t *
impl_get_reference(
    const afw_utf8_t *adaptor_id,
    afw_xctx_t *xctx)
{
    afw_adaptor_id_anchor_t *anchor;
    const afw_adaptor_t *instance;

    AFW_LOCK_BEGIN(xctx->env->adaptor_id_anchor_lock) {

        instance = NULL;
        anchor = (afw_adaptor_id_anchor_t *)
            afw_environment_get_adaptor_id(adaptor_id, xctx);
        if (anchor) {
            instance = anchor->adaptor;
            if (instance) {
                anchor->reference_count++;
            }
        }
    }

    AFW_LOCK_END;

    return instance;
}



/* Get an adaptor and make sure it is started. */
AFW_DEFINE(const afw_adaptor_t *)
afw_adaptor_get_reference(
    const afw_utf8_t *adaptor_id, afw_xctx_t *xctx)
{
    const afw_adaptor_t *instance;
    const afw_utf8_t *service_id;

    instance = impl_get_reference(adaptor_id, xctx);

    /* If adaptor is not registered, try starting it. */
    if (!instance) {
        service_id = afw_utf8_concat(xctx->p, xctx,
            afw_s_adaptor, afw_s_a_dash,
            adaptor_id, NULL);
        afw_service_start(service_id, false, xctx);
        instance = impl_get_reference(adaptor_id, xctx);
        if (!instance) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Adaptor " AFW_UTF8_FMT_Q
                " is not available",
                AFW_UTF8_FMT_ARG(adaptor_id));
        }
    }

    return instance;
}



/* Parse query criteria object appropriate for an adaptor. */
AFW_DEFINE(const afw_query_criteria_t *)
afw_adaptor_query_criteria_parse_object(
    const afw_object_t *query_criteria,
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *object_type_id,
    const afw_object_t *journal_entry,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_type_t *object_type;
    const afw_query_criteria_t *result;

    object_type = afw_adaptor_get_object_type(
        adaptor_id, object_type_id, journal_entry, xctx);

    result = afw_query_criteria_parse_AdaptiveQueryCriteria_object(
        query_criteria, object_type, p, xctx);

    return result;
}



/* Parse URL encoded RQL query string appropriate for an adaptor. */
AFW_DEFINE(const afw_query_criteria_t *)
afw_adaptor_query_criteria_parse_url_encoded_rql_string(
    const afw_utf8_t *url_encoded_rql_string,
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *object_type_id,
    const afw_object_t *journal_entry,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_type_t *object_type;
    const afw_query_criteria_t *result;

    object_type = afw_adaptor_get_object_type(
        adaptor_id, object_type_id, journal_entry, xctx);

    result = afw_query_criteria_parse_url_encoded_rql_string(
        url_encoded_rql_string, object_type, p, xctx);
    
    return result;
}



/* Release an adaptor accessed by afw_adaptor_get_reference(). */
AFW_DEFINE(void)
afw_adaptor_release(const afw_adaptor_t *instance, afw_xctx_t *xctx)
{
    afw_adaptor_id_anchor_t *anchor;
    afw_adaptor_id_anchor_t *previous_anchor;
    afw_boolean_t destroy;

    destroy = false;
    AFW_LOCK_BEGIN(xctx->env->adaptor_id_anchor_lock) {

        /* Find anchor for this adaptor instance. */
        for (
            anchor = (afw_adaptor_id_anchor_t *)
                afw_environment_get_adaptor_id(
                    &instance->adaptor_id, xctx),
            previous_anchor = NULL;
            anchor && anchor->adaptor != instance;
            previous_anchor = anchor,
            anchor = anchor->stopping);

        if (anchor) {
            anchor->reference_count--;
            if (anchor->reference_count <= 0) {
                destroy = true;
                anchor->reference_count = 0;
                if (!previous_anchor) {
                    anchor->adaptor = NULL;
                }
                else {
                    previous_anchor->stopping = anchor->stopping;
                }
            }
        }
    }

    AFW_LOCK_END;

    if (destroy) {
        afw_adaptor_destroy(instance, xctx);
    }
}


/* Create an adaptor session. */
AFW_DEFINE(const afw_adaptor_session_t *)
afw_adaptor_session_create(
    const afw_utf8_t *adaptor_id, afw_xctx_t *xctx)
{
    const afw_adaptor_t *adaptor;
    const afw_adaptor_session_t *session;

    adaptor = afw_adaptor_get_reference(adaptor_id, xctx);

    session = afw_adaptor_create_adaptor_session(
        adaptor, xctx);

    return session;
}


/* Release an adaptor session created by afw_adaptor_session_create(). */
AFW_DEFINE(void)
afw_adaptor_session_release(
    const afw_adaptor_session_t *session,
    afw_xctx_t *xctx)
{
    const afw_adaptor_t *adaptor;
    const afw_adaptor_internal_cache_t *cache;
    afw_adaptor_internal_session_cache_t *session_cache;

    adaptor = session->adaptor;

    /* If session in cache, remove it. */
    cache = afw_adaptor_internal_get_cache(xctx);
    if (cache) {
        session_cache = apr_hash_get(cache->session_cache,
            adaptor->adaptor_id.s, adaptor->adaptor_id.len);
        if (session_cache && session == session_cache->session) {
            apr_hash_set(cache->session_cache,
                adaptor->adaptor_id.s, adaptor->adaptor_id.len, NULL);
        }
    }

    /* Destroy session and release adaptor. */
    afw_adaptor_session_destroy(session, xctx);
    afw_adaptor_release(adaptor, xctx);
}


/* Get an active cached session for adaptor_id. */
static afw_adaptor_internal_session_cache_t *
impl_get_adaptor_session_cache(const afw_utf8_t *adaptor_id,
    afw_boolean_t begin_transaction, afw_xctx_t *xctx)
{
    afw_adaptor_internal_session_cache_t *session_cache;
    const afw_adaptor_internal_cache_t *cache;
    afw_adaptor_internal_transaction_t *transaction;
    const afw_adaptor_transaction_t *new_transaction;
    int i;

    /* Get cached session. */
    cache = afw_adaptor_internal_get_cache(xctx);
    session_cache = apr_hash_get(cache->session_cache,
        adaptor_id->s, adaptor_id->len);

    /* If there is not already one, create one. */
    if (!session_cache) {
        session_cache = afw_xctx_calloc_type(
            afw_adaptor_internal_session_cache_t, xctx);
        session_cache->session = afw_adaptor_session_create(adaptor_id, xctx);
        if (session_cache->session) {
            apr_hash_set(cache->session_cache, adaptor_id->s, adaptor_id->len,
                session_cache);
        }
    }

    /* If session and begin_transaction, make sure transactions is started. */
    if (session_cache->session && begin_transaction) {
        for (i = 0; i < cache->transactions->nelts; i++) {
            transaction = APR_ARRAY_IDX(cache->transactions, i,
                afw_adaptor_internal_transaction_t *);
            if (afw_utf8_equal(transaction->adaptor_id, adaptor_id))
            {
                break;
            }
        }
        if (i >= cache->transactions->nelts) {
            new_transaction = afw_adaptor_session_begin_transaction(
                session_cache->session,
                xctx);
            if (new_transaction) {
                transaction = afw_xctx_calloc_type(
                    afw_adaptor_internal_transaction_t, xctx);
                transaction->adaptor_id =
                    &session_cache->session->adaptor->adaptor_id;
                transaction->transaction = new_transaction;
                APR_ARRAY_PUSH(cache->transactions,
                    const afw_adaptor_internal_transaction_t *) = transaction;
            }
        }
    }

    /* Error if no valid adaptor session. */
    if (!session_cache->session) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "Unable to start session for adaptor id " AFW_UTF8_FMT_Q,
            AFW_UTF8_FMT_ARG(adaptor_id));
    }

    /* Return result. */
    return session_cache;
}

/* Get an active cached session for adaptor_id. */
AFW_DEFINE(const afw_adaptor_session_t *)
afw_adaptor_session_get_cached(const afw_utf8_t *adaptor_id,
    afw_boolean_t begin_transaction, afw_xctx_t *xctx)
{
    afw_adaptor_internal_session_cache_t *session_cache;

    session_cache = impl_get_adaptor_session_cache(adaptor_id,
        begin_transaction, xctx);

    /* Return result. */
    return (session_cache) ? session_cache->session : NULL;
}



/* Commit changes and release cached sessions and objects. */
AFW_DEFINE(void)
afw_adaptor_session_commit_and_release_cache(afw_boolean_t abort,
    afw_xctx_t *xctx)
{
    const afw_adaptor_internal_cache_t *cache;
    const afw_adaptor_internal_transaction_t **t0;
    const afw_adaptor_internal_transaction_t **t;
    afw_adaptor_internal_session_cache_t *session_cache;
    apr_hash_index_t *hi;
    const void * key;
    apr_ssize_t klen;

    cache = xctx->cache;
    if (!cache) return;

    /* Call commit for all active transaction in reverse order of begin. */
    for (t0 = (const afw_adaptor_internal_transaction_t **)
        cache->transactions->elts - 1,
        t = t0 + cache->transactions->nelts; t > t0; t--)
    {
        if (!abort) {
            afw_adaptor_transaction_commit((*t)->transaction, xctx);
        }
        afw_adaptor_transaction_release((*t)->transaction, xctx);
    }

    /* Release all active sessions in no particular order. */
    for (hi = apr_hash_first(afw_pool_get_apr_pool(xctx->p), cache->session_cache);
        hi; hi = apr_hash_next(hi))
    {
        apr_hash_this(hi, &key, &klen, (void **)&session_cache);
        afw_adaptor_session_release(session_cache->session, xctx);
    }

    /* If there is a runtime adaptor session, release it. */
    if (cache->runtime_adaptor_session) {
        afw_adaptor_session_release(cache->runtime_adaptor_session, xctx);
    }

    /* Clear pointer in xctx to cache struct. */
    xctx->cache = NULL;
}



/* Get and cache _AdaptiveObjectType_ object. */
AFW_DEFINE(const afw_object_type_t *)
afw_adaptor_get_object_type(
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *object_type_id,
    const afw_object_t *journal_entry,
    afw_xctx_t *xctx)
{
    const afw_object_type_t *result;
    const afw_object_t *object;
    const afw_utf8_t *embedded_object_type_id;
    const afw_adaptor_object_type_cache_t *adaptor_cache;
    afw_adaptor_internal_session_cache_t *session_cache;
    afw_object_type_property_type_t *property_type;
    const afw_adaptor_t *adaptor;
    const afw_pool_t *p;
    afw_boolean_t final_result;

    /** @fixme This may change so that object types are reused unless changed. */

    /* Pool starts out as xctx's p, but may change to adaptor's p. */
    p = xctx->p;

    /* Get adaptor_id's session cache, adaptor, and impl. */
    session_cache = impl_get_adaptor_session_cache(adaptor_id, false, xctx);
    if (!session_cache) {
        return NULL;
    }
    adaptor = session_cache->session->adaptor;

    result = NULL;
    final_result = false;

    /* Return object type if already cached for life of adaptor. */
    adaptor_cache = afw_adaptor_session_get_object_type_cache_interface(
        session_cache->session, xctx);
    if (adaptor_cache) {
        result = afw_adaptor_object_type_cache_get(
            adaptor_cache, object_type_id, &final_result, xctx);
        if (result) {
            return result;
        }
        if (final_result) {
            return NULL;
        }
        if (adaptor_cache->all_object_types_immutable) {
            p = adaptor->p;
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
            adaptor, object, p, xctx);
    }

    /* For other object types, get object type object via adaptor. */
    else {
        object = NULL;
        AFW_TRY {
            object = afw_adaptor_get_object(adaptor_id,
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
                adaptor, object, p, xctx);
        }
    }

    /*
     * If result, cache in adaptor or session. Then, resolve any embedded object
     * types base on data_type_parameter (default _AdaptiveObject_).
     */
    if (result) {
        if (result && adaptor_cache && adaptor_cache->all_object_types_immutable)
        {
            afw_adaptor_object_type_cache_set(adaptor_cache, result, xctx);
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
                property_type->object_type = afw_adaptor_get_object_type(
                    adaptor_id, embedded_object_type_id, journal_entry, xctx);
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
            property_type->object_type = afw_adaptor_get_object_type(
                adaptor_id, embedded_object_type_id, journal_entry, xctx);
        }
    }

    /* Return result. */
    return result;
}



/*
 * {
 *    type           : "adaptor",
 *    adaptorType    : "afw_runtime",
 *    adaptorId      : "afw"
 * }
 */
void
afw_adaptor_internal_register_afw_adaptor(afw_xctx_t *xctx)
{
    const afw_object_t *conf;
    const afw_pool_t *p;

    p = afw_pool_create(xctx->env->p, xctx);
    conf = afw_object_create_unmanaged(p, xctx);
    afw_object_set_property_as_string(conf,
        afw_s_type, afw_s_adaptor, xctx);
    afw_object_set_property_as_string(conf,
        afw_s_adaptorType, afw_s_afw_runtime, xctx);
    afw_object_set_property_as_string(conf,
        afw_s_adaptorId, afw_s_afw, xctx);
    afw_object_set_property_as_string(conf,
        afw_s_sourceLocation, afw_s_a_Core_afw_adaptor, xctx);
    afw_adaptor_internal_conf_type_create_cede_p(afw_s_adaptor,
        conf, afw_s_a_Core_afw_adaptor, p, xctx);
}


/* Configuration handler for entry type "adaptor". */
void
afw_adaptor_internal_conf_type_create_cede_p(
    const afw_utf8_t *type,
    const afw_object_t *conf,
    const afw_utf8_t *source_location,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_adaptor_id_anchor_t *anchor;
    const afw_utf8_t *adaptor_id;

    /* Get adaptor_id. */
    adaptor_id = afw_object_old_get_property_as_string(conf,
        afw_s_adaptorId, xctx);
    if (!adaptor_id) {
        AFW_THROW_ERROR_FZ(general, xctx,
            AFW_UTF8_CONTEXTUAL_LABEL_FMT
            "adaptorId properties is required",
            AFW_UTF8_FMT_ARG(source_location));
    }

    /* See if adaptor id already used. */
    anchor = afw_environment_get_adaptor_id(adaptor_id, xctx);
    if (anchor) {

        /* If adaptor id is afw, handle special. */
        if (afw_utf8_equal(adaptor_id, afw_s_afw)) {
            AFW_LOG_FZ(warning, xctx,
                AFW_UTF8_CONTEXTUAL_LABEL_FMT
                "adaptor id afw is automatically defined.  Entry ignored.",
                AFW_UTF8_FMT_ARG(source_location));
            return;
        }

        /* Any other adaptor id already registered is an error. */
        else {
            AFW_THROW_ERROR_FZ(general, xctx,
                AFW_UTF8_FMT " adaptor " AFW_UTF8_FMT_Q " is already running",
                source_location->len, source_location->s,
                adaptor_id->len, adaptor_id->s);
        }
    }

    /* Start adaptor. */
    afw_service_start_using_AdaptiveConf_cede_p(
        conf, source_location, p, xctx);
}



/* Adapt and apply view if requested and object is not NULL. */
void
afw_adaptor_internal_process_object_from_adaptor(
    const afw_object_t * *adapted_object,
    const afw_object_t * *view,
    afw_adaptor_internal_object_cb_context_t *ctx,
    const afw_object_t *object,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{ 
    const afw_utf8_t *entity_path;

    *adapted_object = object;
    afw_object_get_reference(object, xctx);
    entity_path = afw_object_path_make(
        ctx->adaptor_id,
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
afw_adaptor_internal_register_service_type(afw_xctx_t *xctx)
{
    afw_service_type_t *self;

    self = afw_xctx_calloc_type(afw_service_type_t, xctx);
    self->inf = &impl_afw_service_type_inf;
    afw_memory_copy(&self->service_type_id, afw_s_adaptor);
    self->conf_type = afw_environment_get_conf_type(afw_s_adaptor, xctx);
    if (!self->conf_type) {
        AFW_THROW_ERROR_Z(general, "conf_type must already be registered",
            xctx);
    }
    self->title = afw_s_a_service_type_adaptor_title;
    self->conf_type_object = afw_runtime_get_object(afw_s__AdaptiveConfType_,
        afw_s_adaptor, xctx);
    afw_environment_register_service_type(afw_s_adaptor, self, xctx);
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
    afw_adaptor_id_anchor_t *anchor;
    afw_adaptor_id_anchor_t *stopping;
    afw_integer_t result = 0;

    AFW_LOCK_BEGIN(xctx->env->adaptor_id_anchor_lock) {

        anchor = (afw_adaptor_id_anchor_t *)
            afw_environment_get_adaptor_id(id, xctx);
        if (anchor) {
            for (stopping = anchor->stopping;
                stopping;
                stopping = stopping->stopping)
            {
                result++;
            }
            if (anchor->adaptor) {
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
    const afw_adaptor_factory_t *factory;
    const afw_adaptor_t *adaptor;
    const afw_utf8_t *adaptor_type;

    adaptor_type = afw_object_old_get_property_as_utf8(properties,
        afw_s_adaptorType, p, xctx);
    if (!adaptor_type) {
        AFW_THROW_ERROR_Z(general,
            "parameter adaptorType missing",
            xctx);
    }

    factory = afw_environment_get_adaptor_type(adaptor_type, xctx);
    if (!factory) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "adaptorType " AFW_UTF8_FMT_Q
            " is not a registered adaptor type",
            AFW_UTF8_FMT_ARG(adaptor_type));
    }
   
    /* Create adaptor. */
    adaptor = afw_adaptor_factory_create_adaptor_cede_p(factory,
        properties, p, xctx);

    /* Make this adaptor the active one. */
    impl_set_instance_active(&adaptor->adaptor_id, adaptor, xctx);
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
