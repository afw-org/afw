// See the 'COPYING' file in the project root for licensing information.
/*
 * Helpers for interfaces afw_adaptor*
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_ADAPTOR_INTERNAL_H__
#define __AFW_ADAPTOR_INTERNAL_H__

/**
 * @defgroup afw_adaptor_internal Adaptor
 * @ingroup afw_c_internal
 *
 * Adaptor internal.
 * @{
 */


/**
 * @file afw_adaptor_internal.h
 * @brief Header for afw_adaptor internal functions.
 */

#include "afw_interface.h"

AFW_BEGIN_DECLARES

/* @brief Internal struct used by adaptors for this xctx. */
struct afw_adaptor_xctx_internal_s {
    afw_boolean_t loading_object_types;
};

#define AFW_ADAPTOR_SCOPE_INTERNAL(xctx) \
    ((xctx)->adaptor_xctx_internal \
    ? (xctx)->adaptor_xctx_internal \
    : ((xctx)->adaptor_xctx_internal = \
        afw_xctx_calloc_type(afw_adaptor_xctx_internal_t, xctx)))


struct afw_adaptor_internal_session_cache_s {
    const afw_adaptor_session_t *session;
    apr_hash_t *object_types_ht;
};


struct afw_adaptor_internal_cache_s {
    apr_hash_t *session_cache;
    apr_array_header_t *transactions;
    const afw_adaptor_session_t *runtime_adaptor_session;
};


/* Object callback context. */
typedef struct {
    const afw_pool_t *p;
    const afw_adaptor_session_t *session;
    afw_adaptor_impl_t *impl;
    const afw_utf8_t *adaptor_id;
    const afw_utf8_t *object_type_id;
    const afw_utf8_t *object_id;
    void * original_context;
    afw_object_cb_t original_callback;
    const afw_object_t *journal_entry;
    const afw_object_options_t *options;
    afw_boolean_t first_call;
    afw_boolean_t outboundNormalization;
} afw_adaptor_internal_object_cb_context_t;



/**
 * @internal
 * @brief Get adaptor cache.
 *
 * Definition in afw_adaptor.c.
 */
afw_adaptor_internal_cache_t *
afw_adaptor_internal_get_cache(afw_xctx_t *xctx);



/**
 * @internal
 * @brief Get journal entry.
 *
 * Definition in afw_adaptor.c.
 */
const afw_object_t *
afw_adaptor_internal_journal_get_entry(
    const afw_adaptor_session_t *session,
    const afw_utf8_t *object_id,
    const afw_object_t *journal_entry,
    afw_xctx_t *xctx);


/**
 * @internal
 * @brief Called internally for action journal prologue.
 *
 * Definition in afw_adaptor.c.
 */
void
afw_adaptor_internal_journal_prologue(
    const afw_adaptor_session_t *session,
    const afw_object_t *journal_entry,
    afw_xctx_t *xctx);

/**
 * @internal
 * @brief Called internally for action journal epilogue.
 *
 * Definition in afw_adaptor.c.
 */
void
afw_adaptor_internal_journal_epilogue(
    const afw_adaptor_session_t *session,
    const afw_object_t *journal_entry,
    afw_boolean_t modification,
    afw_xctx_t *xctx);

    
/** @brief Adaptor id and associated transaction. */
typedef struct afw_adaptor_internal_transaction_s {
    const afw_utf8_t *adaptor_id;
    const afw_adaptor_transaction_t *transaction;
} afw_adaptor_internal_transaction_t;



/**
 * @internal
 * @brief Adapt and apply view if requested and object is not NULL.
 */
void
afw_adaptor_internal_process_object_from_adaptor(
    const afw_object_t * *adapted_object,
    const afw_object_t * *view,
    afw_adaptor_internal_object_cb_context_t *ctx,
    const afw_object_t *object,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @internal
 * @brief Register adaptor service type.
 *
 * Definition in afw_adaptor_service_type.c.
 */
void
afw_adaptor_internal_register_service_type(afw_xctx_t *xctx);


/**
 * @internal
 * @brief Register afw adaptor.
 *
 * Definition in afw_adaptor.c.
 */
void
afw_adaptor_internal_register_afw_adaptor(afw_xctx_t *xctx);


/**
 * @internal
 * @brief Configuration handler for entry type "adaptor".
 *
 * Definition in afw_adaptor.c.
 */
void
afw_adaptor_internal_conf_type_create_cede_p(
    const afw_utf8_t *type,
    const afw_object_t *entry,
    const afw_utf8_t *source_location,
    const afw_pool_t *p, afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_ADAPTOR_INTERNAL_H__ */
