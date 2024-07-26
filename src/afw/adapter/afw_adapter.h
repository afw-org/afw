// See the 'COPYING' file in the project root for licensing information.
/*
 * Helpers for interfaces afw_adapter*
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_ADAPTER_H__
#define __AFW_ADAPTER_H__

#include "afw_interface.h"

/* Headers that are part of the adapter api. */
#include "afw_adapter_add.h"
#include "afw_adapter_delete.h"
#include "afw_adapter_get.h"
#include "afw_adapter_journal.h"
#include "afw_adapter_modify.h"
#include "afw_adapter_replace.h"
#include "afw_adapter_retrieve.h"

/**
 * @defgroup afw_adapter Adapter
 * @ingroup afw_c_api_public
 *
 * Provide afw_adapter interface common support.
 * @{
 */

/**
 * @file afw_adapter.h
 * @brief Header for interface afw_adapter* helpers.
 *
 * These headers are included and are part of the adapter api:
 *
 *  afw_adapter_add.h
 *  afw_adapter_delete.h
 *  afw_adapter_get.h
 *  afw_adapter_journal.h
 *  afw_adapter_modify.h
 *  afw_adapter_replace.h
 *  afw_adapter_retrieve.h
 *  afw_adapter_update.h
 */

AFW_BEGIN_DECLARES


/**
 * @internal
 * @brief Adapter id struct used for afw_register_adapter_id.
 *
 * Reading and modifying this struct must be protected by
 * env->adapter_id_anchor_lock.
 */
struct afw_adapter_id_anchor_s {
    
    /** @brief Adapter id. */
    const afw_utf8_t *adapter_id;
    
    /** @brief Adapter type id. */
    const afw_utf8_t *adapter_type_id;
    
    /** @brief Adapter instance or NULL. */
    const afw_adapter_t *adapter;

    /** @brief Associated properties. */
    const afw_object_t *properties;

    /** @brief Associated service id or NULL. */
    const afw_utf8_t *service_id;

    /** @brief Reference count for this instance of adapter. */
    afw_integer_t reference_count;

    /** @brief First/next stopping adapter or NULL. */
    afw_adapter_id_anchor_t *stopping;
};



/**
 * @brief Prepare expression evaluation context for adapter::.
 * @param adapter for which the request is being made.
 * @param p used for result.
 * @param xctx of caller.
 * @return _AdaptiveAuthorizationResult_ object.
 *
 * This is called internally by afw_adapter_* functions, so it is not
 * usually necessary to call this externally.
 */
AFW_DECLARE(void)
afw_adapter_context_prepare(
    const afw_adapter_t *adapter,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Get an adapter and make sure it is started.
 * @param adapter_id Adapter id.
 * @param xctx of caller.
 * @return adapter.
 *
 * Get a registered adapter by adapter id and increment it's reference count.
 * If the adapter is not already registered, start it if possible and return
 * the adapter with a reference count of 2.
 *
 * It is the responsibility of the caller to call afw_adapter_release()
 * for each call to afw_adapter_get_reference().  When an adapter reference count
 * goes to 0, it is unregistered and it's resources released.
 */
AFW_DECLARE(const afw_adapter_t *)
afw_adapter_get_reference(const afw_utf8_t *adapter_id, afw_xctx_t *xctx);


/**
 * @brief Parse query criteria object appropriate for an adapter.
 * @param query_criteria is an _AdaptiveQueryCriteria_ object.
 * @param adapter_id of adapter.
 * @param object_type_id of object.
 * @param journal_entry is a memory object where journal entry properties will
 *    be added.
 * @param p to use for result.
 * @param xctx of caller.
 * @return parsed query criteria structure.
 *
 * See afw_query_criteria_parse_AdaptiveQueryCriteria_object().
 */
AFW_DECLARE(const afw_query_criteria_t *)
afw_adapter_query_criteria_parse_object(
    const afw_object_t *query_criteria,
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *object_type_id,
    const afw_object_t *journal_entry,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Parse URL encoded RQL query string appropriate for an adapter.
 * @param url_encoded_rql_string URL encoded query string.
 * @param adapter_id of adapter.
 * @param object_type_id of object.
 * @param journal_entry is a memory object where journal entry properties will
 *    be added.
 * @param p to use for result.
 * @param xctx of caller.
 * @return parsed query criteria structure.
 *
 * See afw_query_criteria_parse_url_encoded_rql_string().
 */
AFW_DECLARE(const afw_query_criteria_t *)
afw_adapter_query_criteria_parse_url_encoded_rql_string(
    const afw_utf8_t *url_encoded_rql_string,
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *object_type_id,
    const afw_object_t *journal_entry,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Release an adapter accessed by afw_adapter_get_reference().
 * @param adapter to release.
 * @param xctx of caller.
 * @return adapter.
 *
 * Get adapter.  Start it if necessary and possible.
 */
AFW_DECLARE(void)
afw_adapter_release(const afw_adapter_t *adapter, afw_xctx_t *xctx);



/**
 * @brief Commit/Abort changes and release cached sessions and objects.
 * @param abort changes if true.
 * @param xctx of caller.
 *
 * See afw_adapter_session_get_cached().
 *
 * This function will call the commit() method of all active transactions in
 * the reverse order they began, then close all sessions in no particular
 * order.
 */
AFW_DECLARE(void)
afw_adapter_session_commit_and_release_cache(afw_boolean_t abort,
    afw_xctx_t *xctx);



/**
 * @brief Create an adapter session.
 * @param adapter_id Adapter id.
 * @param xctx of caller.
 * @return Adapter session.
 *
 * This creates an adapter session.  It is the responsibility of the caller to
 * call afw_adapter_session_release().  It is also the responsibility of the
 * caller to call afw_adapter_transaction_commit() and
 * afw_adapter_transaction_release().
 *
 * See afw_adapter_session_get_cached() for using cached session.
 */
AFW_DECLARE(const afw_adapter_session_t *)
afw_adapter_session_create(
    const afw_utf8_t *adapter_id, afw_xctx_t *xctx);



/**
 * @brief Get/create an active cached session for adapter_id
 * @param adapter_id Adapter id.
 * @param begin_transaction if one is not already started.
 * @param xctx of caller.
 * @return Adapter session.
 *
 * If an active session is cached, return it.  If not, create a new session,
 * cache it, begin a transaction (if supported), and return it.  If
 * unsuccessful, error thrown.
 *
 * This session will be released by afw_adapter_session_commit_and_release_cache().
 */
AFW_DECLARE(const afw_adapter_session_t *)
afw_adapter_session_get_cached(
    const afw_utf8_t *adapter_id,
    afw_boolean_t begin_transaction,
    afw_xctx_t *xctx);


/**
 * @brief Release an adapter session created by afw_adapter_session_create().
 * @param session to release.
 * @param xctx of caller.
 * @return Adapter session.
 *
 * Any uncommitted changes are lost.
 *
 * See afw_adapter_session_get_cached() for using cached session.
 */
AFW_DECLARE(void)
afw_adapter_session_release(
    const afw_adapter_session_t *session,
    afw_xctx_t *xctx);

AFW_END_DECLARES

/** @} */

#endif /* __AFW_ADAPTER_H__ */
