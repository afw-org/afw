// See the 'COPYING' file in the project root for licensing information.
/*
 * Helpers for afw_adaptor implementation development
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_ADAPTOR_IMPL_H__
#define __AFW_ADAPTOR_IMPL_H__

#include "afw_interface.h"

/**
 * @defgroup afw_adaptor_impl afw_adaptor
 * @ingroup afw_c_api_impl
 * 
 * Helpers for afw_adapor implementations.
 * 
 * @{
 */

/**
 * @file afw_adaptor_impl.h
 * @brief  Helpers for adaptor implementation development.
 */

AFW_BEGIN_DECLARES

/**
 * @brief Macro to begin an adaptor read lock section.
 * @param adaptor
 * @param xctx of caller.
 *
 * Usage:
 *
 * AFW_ADAPTOR_IMPL_LOCK_READ_BEGIN(adaptor, xctx) {
 *    ... a very small amount of code that doesn't call anything
 *        that might lock this again.
 * }
 * AFW_ADAPTOR_IMPL_LOCK_READ_END;
 */
#define AFW_ADAPTOR_IMPL_LOCK_READ_BEGIN(adaptor) \
AFW_LOCK_READ_BEGIN((adaptor)->impl->adaptor_lock_rw)


/**
 * @brief Macro to end an adaptor read lock section.
 *
 * See AFW_ADAPTOR_IMPL_LOCK_READ_BEGIN for usage.
 */
#define AFW_ADAPTOR_IMPL_LOCK_READ_END \
AFW_LOCK_READ_END


/**
 * @brief Macro to begin an adaptor write lock section.
 * @param adaptor
 * @param xctx of caller.
 *
 * Usage:
 *
 * AFW_ADAPTOR_IMPL_LOCK_WRITE_BEGIN(adaptor, xctx) {
 *    ... a very small amount of code that doesn't call anything
 *        that might lock this again.
 * }
 * AFW_ADAPTOR_IMPL_LOCK_WRITE_END;
 */
#define AFW_ADAPTOR_IMPL_LOCK_WRITE_BEGIN(adaptor) \
AFW_LOCK_WRITE_BEGIN((adaptor)->impl->adaptor_lock_rw)


/**
 * @brief Macro to end an adaptor write lock section.
 *
 * See AFW_ADAPTOR_IMPL_LOCK_WRITE_BEGIN for usage.
 */
#define AFW_ADAPTOR_IMPL_LOCK_WRITE_END \
AFW_LOCK_WRITE_END


/** @brief afw_adaptor_impl_t supported_core_object_types entry. */
typedef struct afw_adaptor_impl_core_object_type_s {
    const afw_utf8_t *object_type_id;
    const afw_object_t *object;
    afw_boolean_t allow_entity;
    afw_boolean_t allow_write;
} afw_adaptor_impl_core_object_type_t;


/**
 * @brief Internal struct used by common adaptor code for all adaptors.
 *
 * Reads and updates to this struct MUST be done using read and write locks
 * of adaptor->p of the appropriate adaptor.  For instance, object_types
 * is accessed by model_adaptor->impl->object_types, so the lock should be
 * on model_adaptor->p.
 */
struct afw_adaptor_impl_s {

    /** @brief Wrapped inf. */
    const afw_adaptor_inf_t *wrapped_inf;

    /** @brief Associated adaptor instance. */
    const afw_adaptor_t *adaptor;

    /** @brief Pointer to adaptor id. */
    const afw_utf8_t *adaptor_id;

    /** @brief Source location associated with this adaptor. */
    const afw_utf8_t *source_location;

    /** @brief Metrics object. */
    const afw_object_t *metrics_object;

    /** @brief Read/write lock used for changing adaptor related structs. */
    const afw_lock_rw_t *adaptor_lock_rw;

    /**
     * @brief Authorization handler id.
     *
     * This is the id of the authorization handler that controls access to
     * this adaptor.  If NULL, there is no access control.
     */
    const afw_utf8_t *authorization_handler_id;

    /**
     * @brief Journal adaptor id (FIXME Going away)
     *
     * Adaptor that will be used to record journal entries for every
     * modification to this store.  This can be the this adaptor or any other
     * adaptor that supports a journal.  See supports_journal below.  If NULL,
     * no journaling is performed
     */
    const afw_utf8_t *journal_adaptor_id;

    /**
     * @brief Model location struct if this adaptor is a model location.
     */
    const afw_model_location_t *model_location;

    /**
     * @brief Model location struct if this adaptor is a model location.
     */
    const afw_authorization_policy_location_t *policy_location;

    /**
     * @brief See afw_adaptor_impl_core_object_type_t.
     */
    apr_hash_t *supported_core_object_types;

    /**
     * @brief Adaptor level authorization
     *
     * Evaluated when an authorization decision is needed at the adaptor level.
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
     * @brief If not NULL, object types that last for the life of the adaptor.
     */
    apr_hash_t *object_types_ht;

    /**
     * @brief Object types last for lifetime of adaptor.
     *
     * The object_types member is fully loaded with object types.
     */
    afw_boolean_t all_object_types_immutable;

    /**
     * @brief Object types fully loaded.
     *
     * If true, object_types_ht holds all the object types for life of adaptor.
     */
    afw_boolean_t object_types_fully_loaded;
};


/** @brief Internal session info used by afw_adaptor_impl*() functions. */
struct afw_adaptor_impl_session_s {

    /** @brief Public part of session. */
    afw_adaptor_session_t pub;

    /** @brief Session this common impl wraps. */
    const afw_adaptor_session_t *wrapped_session;

    /** @brief Public part of journal. */
    afw_adaptor_journal_t journal_pub;

    /** @brief Journal this common impl wraps. */
    const afw_adaptor_journal_t *wrapped_journal;
};


/** @brief Internal request info used by afw_adaptor_impl*() functions. */
struct afw_adaptor_impl_request_s {

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
 * @brief Developers should call this in all create functions for afw_adaptor.
 * @param inf afw_adaptor_inf_t pointer for implementation.
 * @param instance_size 0 or size greater than sizeof(afw_adaptor_t).
 * @param properties config object.
 * @param start is function to call to start adaptor.
 * @param stop is function to call to stop adaptor.
 * @param p to use as parent when creating adaptor pool.
 * @param xctx of caller.
 * @return instance of afw_adaptor_t that optionally with extra memory based
 *    on instance_size parameter.
 *
 * This function creates and initializes an afw_adaptor instance.  After
 * calling this function, the caller should do additional adaptor type
 * specific processing.  The instance_size parameter can be specified to
 * get a larger instance size than sizeof(afw_adaptor_t) for adaptor type
 * specific use.
 *
 * A new pool is created in the callings xctx's pool for use by the adaptor.
 *
 * This function validates and normalizes the adaptor properties based on
 * /afw/_AdaptiveObjectType_/_AdaptiveConf_adaptor_<adaptorType>, where
 * <adaptorType> is the value of the adaptorType property.
 *
 * All properties from /afw/_AdaptiveObjectType_/_AdaptiveConf_adaptor are
 * processed.
 *
 * Some additional properties are processed if they pass validation.
 * Properties isModelLocation and isPolicyLocation cause the appropriate core
 * object types to be supported.
 *
 * If this adaptorId is the layout adaptorId, the appropriate core object
 * types are supported.
 *
 * See afw_adaptor_impl_s for more information on how on how common adaptor
 * code uses this information.
 */
AFW_DECLARE(afw_adaptor_t *)
afw_adaptor_impl_create_cede_p(
    const afw_adaptor_inf_t *inf,
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
 * afw_adaptor_session_get_index_interface to initialize
 * afw_adaptor_object_type_cache_t to return.  Parameter
 * should probably be defined in the sessions self.
 */
AFW_DECLARE(void)
afw_adaptor_impl_object_type_cache_initialize(
    afw_adaptor_object_type_cache_t *object_type_cache,
    const afw_adaptor_object_type_cache_inf_t *inf,
    const afw_adaptor_session_t *session,
    afw_boolean_t all_object_types_immutable,
    afw_xctx_t *xctx);



/**
 * @brief inf for an implementation of afw_adaptor_object_type_cache.
 *
 * This is the inf for an implementation of afw_adaptor_object_type_cache
 * that provided life of the adaptor cache support for
 * object types.  The address of this can be passed as the inf parameter
 * of afw_adaptor_impl_object_type_cache_initialize() if this
 * meets the needs of the adaptor implementation.
 */
AFW_DECLARE_CONST_DATA(afw_adaptor_object_type_cache_inf_t)
afw_adaptor_impl_object_type_cache_inf;



/**
 * @brief Set object types fully loaded.
 *
 * This can be called by adaptor implementation to indicate that all object
 * types that last for life of adaptor have been accessed at least once.
 * This allows gets for unknown object types to fail quicker.
 */
AFW_DECLARE(void)
afw_adaptor_impl_set_object_types_fully_loaded(
    const afw_adaptor_t *adaptor,
    afw_xctx_t *xctx);


/**
 * @brief Create a generic object type object.
 * @param adaptor_id.
 * @param object_type_id.
 * @param p to use for result.
 * @param xctx of caller.
 * @return generic object type object (like _AdaptiveObject_).
 *
 * This can be used by adaptors that don't have specific object
 * types to create generic object types to return on get_object.
 */
AFW_DECLARE(const afw_object_t *)
afw_adaptor_impl_generic_object_type_object_get(
    const afw_adaptor_t *adaptor,
    const afw_utf8_t *object_type_id,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Push adaptor qualifiers to xctx.
 * @param adaptor.
 * @param xctx of caller.
 *
 * Call this to add adaptor related qualifiers to xctx.  Make sure
 * to save and restore xctx->qualifier_stack->nelts.
 */
AFW_DECLARE(void)
afw_adaptor_impl_push_qualifiers(
    const afw_adaptor_t *adaptor,
    afw_xctx_t *xctx);



/**
 * @brief Developers should call this for configuration property errors.
 * @param adaptor.
 * @param property_name this is in error.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_adaptor_impl_throw_property_invalid(
    const afw_adaptor_t *adaptor,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx);


/**
 * @brief Developers should call this for missing required configuration
 *    property.
 * @param adaptor.
 * @param property_name this is in error.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_adaptor_impl_throw_property_required(
    const afw_adaptor_t *adaptor,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx);


/**
 * @brief Call object cb from list.
 * @param list
 * @param context
 * @param callback
 * @param xctx of caller.
 * @return afw_adaptor_session_retrieve_objects_result instance.
 */
AFW_DECLARE(void)
afw_adaptor_impl_call_object_cb_from_list(
    const afw_list_t *list,
    void * context,
    afw_object_cb_t callback,
    afw_xctx_t *xctx);


/**
 * @brief Determine whether a journal entry is applicable to a consumer.
 * @param afw_adaptor_journal instance.
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
afw_adaptor_impl_is_journal_entry_applicable(
    const afw_adaptor_journal_t *instance,
    const afw_object_t * entry,
    const afw_object_t * consumer,
    const afw_value_t * const * filter,
    afw_xctx_t *xctx);

#define AFW_ADAPTOR_IMPL_ERROR_ADAPTOR_IMMUTABLE \
AFW_THROW_ERROR_Z(read_only, "Adaptor is read-only.", xctx)


/**
 * @brief Indicates support of a core object type.
 * @param adaptor.
 * @param object_type_id.
 * @param allow_entity indicates adaptor allows entities of this type.
 * @param allow_write indicates adaptor can store instances of this type.
 * @param xctx of caller.
 *
 * afw_adaptor_impl_create_cede_p() calls this for _AdaptiveObjectType_
 * with allow_write false.  Other common conf parameters can cause allow_write
 * to change to true and for other object types to be added.
 *
 * After return from afw_adaptor_impl_create_cede_p(), an implementation
 * can call this to add additional ids.
 *
 * The common session impl code will use this information before calling
 * a custom adaptor type implementation.  This includes returning
 * these object types for get/retrieve requests and insuring the objects
 * are not modified if allow_write is false.
 */
AFW_DECLARE(void)
afw_adaptor_impl_set_supported_core_object_type(
    const afw_adaptor_t *adaptor,
    const afw_utf8_t *object_type_id,
    afw_boolean_t allow_entity,
    afw_boolean_t allow_write,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_ADAPTOR_IMPL_H__ */
