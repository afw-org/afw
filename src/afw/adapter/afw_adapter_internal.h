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

/**
 * @defgroup afw_adapter_internal Adapter
 * @ingroup afw_c_api_internal
 *
 * Adapter internal.
 * @{
 */

/**
 * @file afw_adapter_internal.h
 * @brief Header for afw_adapter internal functions.
 */

AFW_BEGIN_DECLARES

/* @brief Internal struct used by adapters for this xctx. */
struct afw_adapter_xctx_internal_s {
    afw_boolean_t loading_object_types;
};

#define AFW_ADAPTER_SCOPE_INTERNAL(xctx) \
    ((xctx)->adapter_xctx_internal \
    ? (xctx)->adapter_xctx_internal \
    : ((xctx)->adapter_xctx_internal = \
        afw_xctx_calloc_type(afw_adapter_xctx_internal_t, xctx)))


struct afw_adapter_internal_session_cache_s {
    const afw_adapter_session_t *session;
    apr_hash_t *object_types_ht;
};


struct afw_adapter_internal_cache_s {
    apr_hash_t *session_cache;
    apr_array_header_t *transactions;
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
typedef struct afw_adapter_internal_transaction_s {
    const afw_utf8_t *adapter_id;
    const afw_adapter_transaction_t *transaction;
} afw_adapter_internal_transaction_t;



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


AFW_END_DECLARES

/** @} */

#endif /* __AFW_ADAPTER_INTERNAL_H__ */
