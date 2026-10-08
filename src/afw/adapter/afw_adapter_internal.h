// See the 'COPYING' file in the project root for licensing information.
/*
 * Helpers for interfaces afw_adapter*
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_ADAPTER_INTERNAL_H__
#define __AFW_ADAPTER_INTERNAL_H__

#include "afw_interface.h"
#include "afw_vector.h"
#include "afw_hash_table.h"

/**
 * @addtogroup afw_adapter_internal
 * @{
 */

/**
 * @file afw_adapter_internal.h
 * @brief Header for afw_adapter internal functions.
 */

AFW_BEGIN_DECLARES

/**
 * Adapter id registry entry. Protect with env->adapter_id_anchor_lock.
 */
struct afw_adapter_id_anchor_s {
    const afw_utf8_t *adapter_id;
    const afw_utf8_t *adapter_type_id;
    const afw_adapter_t *adapter;
    const afw_object_t *properties;
    const afw_utf8_t *service_id;
    afw_integer_t reference_count;
    afw_adapter_id_anchor_t *stopping;
};

/** @brief Internal struct used by adapters for this xctx. */
struct afw_adapter_xctx_internal_s {
    afw_boolean_t loading_object_types;
};

#define AFW_ADAPTER_SCOPE_INTERNAL(_xctx) \
    ((_xctx)->adapter_xctx_internal \
    ? (_xctx)->adapter_xctx_internal \
    : ((_xctx)->adapter_xctx_internal = \
        afw_pool_calloc_type((_xctx)->p, afw_adapter_xctx_internal_t, _xctx)))


struct afw_adapter_internal_session_cache_s {
    const afw_adapter_session_t *session;
    afw_void_hash_table_t *object_types_ht;
};


typedef struct afw_adapter_internal_transaction_s
    afw_adapter_internal_transaction_t;

AFW_VECTOR_STRUCT(afw_adapter_internal_transaction_p_vector_s,
    afw_adapter_internal_transaction_t *);
typedef struct afw_adapter_internal_transaction_p_vector_s
    afw_adapter_internal_transaction_p_vector_t;


struct afw_adapter_internal_cache_s {
    afw_void_hash_table_t *session_cache;
    afw_adapter_internal_transaction_p_vector_t *transactions;
    const afw_adapter_session_t *runtime_adapter_session;
};


/* Object callback context. */
typedef struct {
    const afw_pool_t *p;
    const afw_adapter_session_t *session;
    afw_adapter_impl_t *impl;
    const afw_utf8_t *adapter_id;
    const afw_utf8_t *object_type_id;
    const afw_utf8_t *object_id;
    void * original_context;
    afw_object_cb_t original_callback;
    const afw_object_t *journal_entry;
    const afw_object_options_t *options;
    afw_boolean_t first_call;
    afw_boolean_t outboundNormalization;
} afw_adapter_internal_object_cb_context_t;



/**
 * @internal
 * @brief Get adapter cache.
 *
 * Definition in afw_adapter.c.
 */
afw_adapter_internal_cache_t *
afw_adapter_internal_get_cache(afw_xctx_t *xctx);


/**
 * @internal
 * Remember one afw_adapter_release() for adapter when p is destroyed.
 * The same adapter may be registered more than once.
 * afw_adapter_release() removes one matching entry on xctx->p only.
 * A cleanup registered on another pool stays until that pool dies.
 */
void
afw_adapter_internal_reference_cleanup(
    const afw_adapter_t *adapter,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @internal
 * @brief Get journal entry.
 *
 * Definition in afw_adapter.c.
 */
const afw_object_t *
afw_adapter_internal_journal_get_entry(
    const afw_adapter_session_t *session,
    const afw_utf8_t *object_id,
    const afw_object_t *journal_entry,
    afw_xctx_t *xctx);


/**
 * @internal
 * @brief Called internally for action journal prologue.
 *
 * Definition in afw_adapter.c.
 */
void
afw_adapter_internal_journal_prologue(
    const afw_adapter_session_t *session,
    const afw_object_t *journal_entry,
    afw_xctx_t *xctx);

/**
 * @internal
 * @brief Called internally for action journal epilogue.
 *
 * Definition in afw_adapter.c.
 */
void
afw_adapter_internal_journal_epilogue(
    const afw_adapter_session_t *session,
    const afw_object_t *journal_entry,
    afw_boolean_t modification,
    afw_xctx_t *xctx);

    
/** @brief Adapter id and associated transaction. */
struct afw_adapter_internal_transaction_s {
    const afw_utf8_t *adapter_id;
    const afw_adapter_transaction_t *transaction;
};



/**
 * @internal
 * @brief Throw if value (or an object or array in it) has a property named
 *        _meta_.
 *
 * Object meta is not a property. An adapter that stores a property named
 * _meta_ writes an object it can not read back (#497).
 */
void
afw_adapter_internal_refuse_meta_property(
    const afw_value_t *value,
    afw_xctx_t *xctx);


/**
 * @internal
 * @brief Throw if object (or an object or array in it) has a property named
 *        _meta_. See afw_adapter_internal_refuse_meta_property().
 */
void
afw_adapter_internal_refuse_meta_property_in_object(
    const afw_object_t *object,
    afw_xctx_t *xctx);


/**
 * @internal
 * @brief Adapt and apply view if requested and object is not NULL.
 */
void
afw_adapter_internal_process_object_from_adapter(
    const afw_object_t * *adapted_object,
    const afw_object_t * *view,
    afw_adapter_internal_object_cb_context_t *ctx,
    const afw_object_t *object,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @internal
 * @brief Register adapter service type.
 *
 * Definition in afw_adapter_service_type.c.
 */
void
afw_adapter_internal_register_service_type(afw_xctx_t *xctx);


/**
 * @internal
 * @brief Register afw adapter.
 *
 * Definition in afw_adapter.c.
 */
void
afw_adapter_internal_register_afw_adapter(afw_xctx_t *xctx);


/**
 * @internal
 * @brief Configuration handler for entry type "adapter".
 *
 * Definition in afw_adapter.c.
 */
void
afw_adapter_internal_conf_type_create_cede_p(
    const afw_utf8_t *type,
    const afw_object_t *entry,
    const afw_utf8_t *source_location,
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @internal
 * @brief Pin an adapter instance; the caller holds adapter_id_anchor_lock
 *    and later releases the pin with afw_adapter_release().
 * @param instance to pin. Active or draining.
 * @param p of the caller. No pin when p is instance->p.
 * @param xctx of caller.
 * @return instance when pinned, otherwise NULL.
 *
 * Caller holds adapter_id_anchor_lock. Increments that instance's
 * anchor reference count so stop drains instead of destroying it.
 * The caller releases the pin with afw_adapter_release(). The pin is
 * not tied to the lifetime of p.
 */
const afw_adapter_t *
afw_adapter_internal_pin_for_pool_lock_held(
    const afw_adapter_t *instance,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */  // end of @addtogroup @addtogroup

#endif /* __AFW_ADAPTER_INTERNAL_H__ */
