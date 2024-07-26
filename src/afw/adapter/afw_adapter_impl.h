// See the 'COPYING' file in the project root for licensing information.
/*
 * Helpers for afw_adapter implementation development
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_ADAPTER_IMPL_H__
#define __AFW_ADAPTER_IMPL_H__

#include "afw_interface.h"

/**
 * @defgroup afw_adapter_impl afw_adapter
 * @ingroup afw_c_api_impl
 * 
 * Helpers for afw_adapor implementations.
 * 
 * @{
 */

/**
 * @file afw_adapter_impl.h
 * @brief  Helpers for adapter implementation development.
 */

AFW_BEGIN_DECLARES

/**
 * @brief Macro to begin an adapter read lock section.
 * @param adapter
 * @param xctx of caller.
 *
 * Usage:
 *
 * AFW_ADAPTER_IMPL_LOCK_READ_BEGIN(adapter, xctx) {
 *    ... a very small amount of code that doesn't call anything
 *        that might lock this again.
 * }
 * AFW_ADAPTER_IMPL_LOCK_READ_END;
 */
#define AFW_ADAPTER_IMPL_LOCK_READ_BEGIN(adapter) \
AFW_LOCK_READ_BEGIN((adapter)->impl->adapter_lock_rw)


/**
 * @brief Macro to end an adapter read lock section.
 *
 * See AFW_ADAPTER_IMPL_LOCK_READ_BEGIN for usage.
 */
#define AFW_ADAPTER_IMPL_LOCK_READ_END \
AFW_LOCK_READ_END


/**
 * @brief Macro to begin an adapter write lock section.
 * @param adapter
 * @param xctx of caller.
 *
 * Usage:
 *
 * AFW_ADAPTER_IMPL_LOCK_WRITE_BEGIN(adapter, xctx) {
 *    ... a very small amount of code that doesn't call anything
 *        that might lock this again.
 * }
 * AFW_ADAPTER_IMPL_LOCK_WRITE_END;
 */
#define AFW_ADAPTER_IMPL_LOCK_WRITE_BEGIN(adapter) \
AFW_LOCK_WRITE_BEGIN((adapter)->impl->adapter_lock_rw)


/**
 * @brief Macro to end an adapter write lock section.
 *
 * See AFW_ADAPTER_IMPL_LOCK_WRITE_BEGIN for usage.
 */
#define AFW_ADAPTER_IMPL_LOCK_WRITE_END \
AFW_LOCK_WRITE_END


/** @brief afw_adapter_impl_t supported_core_object_types entry. */
typedef struct afw_adapter_impl_core_object_type_s {
    const afw_utf8_t *object_type_id;
    const afw_object_t *object;
    afw_boolean_t allow_entity;
    afw_boolean_t allow_write;
} afw_adapter_impl_core_object_type_t;


/**
 * @brief Internal struct used by common adapter code for all adapters.
 *
 * Reads and updates to this struct MUST be done using read and write locks
 * of adapter->p of the appropriate adapter.  For instance, object_types
 * is accessed by model_adapter->impl->object_types, so the lock should be
 * on model_adapter->p.
 */
struct afw_adapter_impl_s {

    /** @brief Wrapped inf. */
    const afw_adapter_inf_t *wrapped_inf;

    /** @brief Associated adapter instance. */
    const afw_adapter_t *adapter;

    /** @brief Pointer to adapter id. */
    const afw_utf8_t *adapter_id;

    /** @brief Source location associated with this adapter. */
    const afw_utf8_t *source_location;

    /** @brief Metrics object. */
    const afw_object_t *metrics_object;

    /** @brief Read/write lock used for changing adapter related structs. */
    const afw_lock_rw_t *adapter_lock_rw;

    /**
     * @brief Authorization handler id.
     *
     * This is the id of the authorization handler that controls access to
     * this adapter.  If NULL, there is no access control.
     */
    const afw_utf8_t *authorization_handler_id;

    /**
     * @brief Journal adapter id (FIXME Going away)
     *
     * Adapter that will be used to record journal entries for every
     * modification to this store.  This can be the this adapter or any other
     * adapter that supports a journal.  See supports_journal below.  If NULL,
     * no journaling is performed
     */
    const afw_utf8_t *journal_adapter_id;

    /**
     * @brief Model location struct if this adapter is a model location.
     */
    const afw_model_location_t *model_location;

    /**
     * @brief Model location struct if this adapter is a model location.
     */
    const afw_authorization_policy_location_t *policy_location;

    /**
     * @brief See afw_adapter_impl_core_object_type_t.
     */
    apr_hash_t *supported_core_object_types;

    /**
     * @brief Adapter level authorization
     *
     * Evaluated when an authorization decision is needed at the adapter level.
     */
    const afw_value_t *authorization;

    /** @brief Custom variables. */
    const afw_object_t *custom_variables;

    /**
     * @brief retrieve_objects() count.
     */
    AFW_ATOMIC afw_integer_t retrieve_objects_count;

    /**
     * @brief get_object() count.
     */
    AFW_ATOMIC afw_integer_t get_object_count;

    /**
     * @brief add_object() count.
     */
    AFW_ATOMIC afw_integer_t add_object_count;

    /**
     * @brief modify_object() count.
     */
    AFW_ATOMIC afw_integer_t modify_object_count;

    /**
     * @brief replace_object() count.
     */
    AFW_ATOMIC afw_integer_t replace_object_count;

    /**
     * @brief update_object() count.
     */
    AFW_ATOMIC afw_integer_t update_object_count;

    /**
     * @brief delete_object() count.
     */
    AFW_ATOMIC afw_integer_t delete_object_count;

    /**
     * @brief If not NULL, object types that last for the life of the adapter.
     */
    apr_hash_t *object_types_ht;

    /**
     * @brief Check individual object read access.
     *
     * This is the value of the 'checkIndividualObjectReadAccess' property for
     * this adapter. If true, an additional 'read' check is performed for each
     * object retrieved from the adapter.
     */
    afw_boolean_t check_individual_object_read_access;

    /**
     * @brief Object types last for lifetime of adapter.
     *
     * The object_types member is fully loaded with object types.
     */
    afw_boolean_t all_object_types_immutable;

    /**
     * @brief Object types fully loaded.
     *
     * If true, object_types_ht holds all the object types for life of adapter.
     */
    afw_boolean_t object_types_fully_loaded;
};


/** @brief Internal session info used by afw_adapter_impl*() functions. */
struct afw_adapter_impl_session_s {

    /** @brief Public part of session. */
    afw_adapter_session_t pub;

    /** @brief Session this common impl wraps. */
    const afw_adapter_session_t *wrapped_session;

    /** @brief Public part of journal. */
    afw_adapter_journal_t journal_pub;

    /** @brief Journal this common impl wraps. */
    const afw_adapter_journal_t *wrapped_journal;
};


/** @brief Internal request info used by afw_adapter_impl*() functions. */
struct afw_adapter_impl_request_s {

    /** @brief Pool used. */
    const afw_pool_t *p;

    /** @brief Journal entry. */
    const afw_object_t *journal_entry;

    /** @brief Request object. */
    const afw_object_t *request;

    /** @brief Object options. */
    const afw_object_options_t *options;

    /** @brief resource id */
    const afw_utf8_t *resource_id;
};


/**
 * @brief Developers should call this in all create functions for afw_adapter.
 * @param inf afw_adapter_inf_t pointer for implementation.
 * @param instance_size 0 or size greater than sizeof(afw_adapter_t).
 * @param properties config object.
 * @param start is function to call to start adapter.
 * @param stop is function to call to stop adapter.
 * @param p to use as parent when creating adapter pool.
 * @param xctx of caller.
 * @return instance of afw_adapter_t that optionally with extra memory based
 *    on instance_size parameter.
 *
 * This function creates and initializes an afw_adapter instance.  After
 * calling this function, the caller should do additional adapter type
 * specific processing.  The instance_size parameter can be specified to
 * get a larger instance size than sizeof(afw_adapter_t) for adapter type
 * specific use.
 *
 * A new pool is created in the callings xctx's pool for use by the adapter.
 *
 * This function validates and normalizes the adapter properties based on
 * `/afw/_AdaptiveObjectType_/_AdaptiveConf_adapter_<adapterType>`, where
 * `<adapterType>` is the value of the adapterType property.
 *
 * All properties from /afw/_AdaptiveObjectType_/_AdaptiveConf_adapter are
 * processed.
 *
 * Some additional properties are processed if they pass validation.
 * Properties isModelLocation and isPolicyLocation cause the appropriate core
 * object types to be supported.
 *
 * If this adapterId is the layout adapterId, the appropriate core object
 * types are supported.
 *
 * See afw_adapter_impl_s for more information on how on how common adapter
 * code uses this information.
 */
AFW_DECLARE(afw_adapter_t *)
afw_adapter_impl_create_cede_p(
    const afw_adapter_inf_t *inf,
    afw_size_t instance_size,
    const afw_object_t *properties,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Initialize object type cache instance.
 * @param object_type_cache to initialize.
 * @param inf for instance.
 * @param session for instance.
 * @param all_object_types_immutable for instance.
 * @param xctx of caller.
 *
 * This can be used in implementation of method
 * afw_adapter_session_get_index_interface to initialize
 * afw_adapter_object_type_cache_t to return.  Parameter
 * should probably be defined in the sessions self.
 */
AFW_DECLARE(void)
afw_adapter_impl_object_type_cache_initialize(
    afw_adapter_object_type_cache_t *object_type_cache,
    const afw_adapter_object_type_cache_inf_t *inf,
    const afw_adapter_session_t *session,
    afw_boolean_t all_object_types_immutable,
    afw_xctx_t *xctx);



/**
 * @brief inf for an implementation of afw_adapter_object_type_cache.
 *
 * This is the inf for an implementation of afw_adapter_object_type_cache
 * that provided life of the adapter cache support for
 * object types.  The address of this can be passed as the inf parameter
 * of afw_adapter_impl_object_type_cache_initialize() if this
 * meets the needs of the adapter implementation.
 */
AFW_DECLARE_CONST_DATA(afw_adapter_object_type_cache_inf_t)
afw_adapter_impl_object_type_cache_inf;



/**
 * @brief Set object types fully loaded.
 *
 * This can be called by adapter implementation to indicate that all object
 * types that last for life of adapter have been accessed at least once.
 * This allows gets for unknown object types to fail quicker.
 */
AFW_DECLARE(void)
afw_adapter_impl_set_object_types_fully_loaded(
    const afw_adapter_t *adapter,
    afw_xctx_t *xctx);


/**
 * @brief Create a generic object type object.
 * @param adapter_id.
 * @param object_type_id.
 * @param p to use for result.
 * @param xctx of caller.
 * @return generic object type object (like _AdaptiveObject_).
 *
 * This can be used by adapters that don't have specific object
 * types to create generic object types to return on get_object.
 */
AFW_DECLARE(const afw_object_t *)
afw_adapter_impl_generic_object_type_object_get(
    const afw_adapter_t *adapter,
    const afw_utf8_t *object_type_id,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Push adapter qualifiers to xctx.
 * @param adapter.
 * @param xctx of caller.
 *
 * Call this to add adapter related qualifiers to xctx.  Make sure
 * to save and restore xctx->qualifier_stack->nelts.
 */
AFW_DECLARE(void)
afw_adapter_impl_push_qualifiers(
    const afw_adapter_t *adapter,
    afw_xctx_t *xctx);



/**
 * @brief Developers should call this for configuration property errors.
 * @param adapter.
 * @param property_name this is in error.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_adapter_impl_throw_property_invalid(
    const afw_adapter_t *adapter,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx);


/**
 * @brief Developers should call this for missing required configuration
 *    property.
 * @param adapter.
 * @param property_name this is in error.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_adapter_impl_throw_property_required(
    const afw_adapter_t *adapter,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx);


/**
 * @brief Call object cb from list.
 * @param list
 * @param context
 * @param callback
 * @param xctx of caller.
 * @return afw_adapter_session_retrieve_objects_result instance.
 */
AFW_DECLARE(void)
afw_adapter_impl_call_object_cb_from_list(
    const afw_array_t *list,
    void * context,
    afw_object_cb_t callback,
    afw_xctx_t *xctx);


/**
 * @brief Determine whether a journal entry is applicable to a consumer.
 * @param afw_adapter_journal instance.
 * @param entry from journal.
 * @param consumer object.
 * @param filter should be initialized to NULL on first call and anytime
 *   the filter has changed.  The value returned should be passed to
 *   subsequent calls.  This holds the evaluated expression from the
 *   consumer filter property.
 * @param xctx of caller.
 * @return true if journal entry applies to consumer.
 */
AFW_DECLARE(afw_boolean_t)
afw_adapter_impl_is_journal_entry_applicable(
    const afw_adapter_journal_t *instance,
    const afw_object_t * entry,
    const afw_object_t * consumer,
    const afw_value_t * const * filter,
    afw_xctx_t *xctx);

#define AFW_ADAPTER_IMPL_ERROR_ADAPTER_IMMUTABLE \
AFW_THROW_ERROR_Z(read_only, "Adapter is read-only.", xctx)


/**
 * @brief Indicates support of a core object type.
 * @param adapter.
 * @param object_type_id.
 * @param allow_entity indicates adapter allows entities of this type.
 * @param allow_write indicates adapter can store instances of this type.
 * @param xctx of caller.
 *
 * afw_adapter_impl_create_cede_p() calls this for _AdaptiveObjectType_
 * with allow_write false.  Other common conf parameters can cause allow_write
 * to change to true and for other object types to be added.
 *
 * After return from afw_adapter_impl_create_cede_p(), an implementation
 * can call this to add additional ids.
 *
 * The common session impl code will use this information before calling
 * a custom adapter type implementation.  This includes returning
 * these object types for get/retrieve requests and insuring the objects
 * are not modified if allow_write is false.
 */
AFW_DECLARE(void)
afw_adapter_impl_set_supported_core_object_type(
    const afw_adapter_t *adapter,
    const afw_utf8_t *object_type_id,
    afw_boolean_t allow_entity,
    afw_boolean_t allow_write,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_ADAPTER_IMPL_H__ */
