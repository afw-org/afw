// See the 'COPYING' file in the project root for licensing information.
/*
 * Helpers for interfaces afw_adaptor*
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_ADAPTOR_H__
#define __AFW_ADAPTOR_H__

#include "afw_interface.h"

/* Headers that are part of the adaptor api. */
#include "afw_adaptor_add.h"
#include "afw_adaptor_delete.h"
#include "afw_adaptor_get.h"
#include "afw_adaptor_journal.h"
#include "afw_adaptor_modify.h"
#include "afw_adaptor_replace.h"
#include "afw_adaptor_retrieve.h"

/**
 * @defgroup afw_adaptor Adaptor
 * @ingroup afw_c_api_public
 *
 * Provide afw_adaptor interface common support.
 * @{
 */

/**
 * @file afw_adaptor.h
 * @brief Header for interface afw_adaptor* helpers.
 *
 * These headers are included and are part of the adaptor api:
 *
 *  afw_adaptor_add.h
 *  afw_adaptor_delete.h
 *  afw_adaptor_get.h
 *  afw_adaptor_journal.h
 *  afw_adaptor_modify.h
 *  afw_adaptor_replace.h
 *  afw_adaptor_retrieve.h
 *  afw_adaptor_update.h
 */

AFW_BEGIN_DECLARES


/**
 * @internal
 * @brief Adaptor id struct used for afw_register_adaptor_id.
 *
 * Reading and modifying this struct must be protected by
 * env->adaptor_id_anchor_lock.
 */
struct afw_adaptor_id_anchor_s {
    
    /** @brief Adaptor id. */
    const afw_utf8_t *adaptor_id;
    
    /** @brief Adaptor type id. */
    const afw_utf8_t *adaptor_type_id;
    
    /** @brief Adaptor instance or NULL. */
    const afw_adaptor_t *adaptor;

    /** @brief Associated properties. */
    const afw_object_t *properties;

    /** @brief Associated service id or NULL. */
    const afw_utf8_t *service_id;

    /** @brief Reference count for this instance of adaptor. */
    afw_integer_t reference_count;

    /** @brief First/next stopping adaptor or NULL. */
    afw_adaptor_id_anchor_t *stopping;
};



/**
 * @brief Prepare expression evaluation context for adaptor::.
 * @param adaptor for which the request is being made.
 * @param p used for result.
 * @param xctx of caller.
 * @return _AdaptiveAuthorizationResult_ object.
 *
 * This is called internally by afw_adaptor_* functions, so it is not
 * usually necessary to call this externally.
 */
AFW_DECLARE(void)
afw_adaptor_context_prepare(
    const afw_adaptor_t *adaptor,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Get an adaptor and make sure it is started.
 * @param adaptor_id Adaptor id.
 * @param xctx of caller.
 * @return adaptor.
 *
 * Get a registered adaptor by adaptor id and increment it's reference count.
 * If the adaptor is not already registered, start it if possible and return
 * the adaptor with a reference count of 2.
 *
 * It is the responsibility of the caller to call afw_adaptor_release()
 * for each call to afw_adaptor_get_reference().  When an adaptor reference count
 * goes to 0, it is unregistered and it's resources released.
 */
AFW_DECLARE(const afw_adaptor_t *)
afw_adaptor_get_reference(const afw_utf8_t *adaptor_id, afw_xctx_t *xctx);


/**
 * @brief Parse query criteria object appropriate for an adaptor.
 * @param query_criteria is an _AdaptiveQueryCriteria_ object.
 * @param adaptor_id of adaptor.
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
afw_adaptor_query_criteria_parse_object(
    const afw_object_t *query_criteria,
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *object_type_id,
    const afw_object_t *journal_entry,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Parse URL encoded RQL query string appropriate for an adaptor.
 * @param url_encoded_rql_string URL encoded query string.
 * @param adaptor_id of adaptor.
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
afw_adaptor_query_criteria_parse_url_encoded_rql_string(
    const afw_utf8_t *url_encoded_rql_string,
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *object_type_id,
    const afw_object_t *journal_entry,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Release an adaptor accessed by afw_adaptor_get_reference().
 * @param adaptor to release.
 * @param xctx of caller.
 * @return adaptor.
 *
 * Get adaptor.  Start it if necessary and possible.
 */
AFW_DECLARE(void)
afw_adaptor_release(const afw_adaptor_t *adaptor, afw_xctx_t *xctx);



/**
 * @brief Commit/Abort changes and release cached sessions and objects.
 * @param abort changes if true.
 * @param xctx of caller.
 *
 * See afw_adaptor_session_get_cached().
 *
 * This function will call the commit() method of all active transactions in
 * the reverse order they began, then close all sessions in no particular
 * order.
 */
AFW_DECLARE(void)
afw_adaptor_session_commit_and_release_cache(afw_boolean_t abort,
    afw_xctx_t *xctx);



/**
 * @brief Create an adaptor session.
 * @param adaptor_id Adaptor id.
 * @param xctx of caller.
 * @return Adaptor session.
 *
 * This creates an adaptor session.  It is the responsibility of the caller to
 * call afw_adaptor_session_release().  It is also the responsibility of the
 * caller to call afw_adaptor_transaction_commit() and
 * afw_adaptor_transaction_release().
 *
 * See afw_adaptor_session_get_cached() for using cached session.
 */
AFW_DECLARE(const afw_adaptor_session_t *)
afw_adaptor_session_create(
    const afw_utf8_t *adaptor_id, afw_xctx_t *xctx);



/**
 * @brief Get/create an active cached session for adaptor_id
 * @param adaptor_id Adaptor id.
 * @param begin_transaction if one is not already started.
 * @param xctx of caller.
 * @return Adaptor session.
 *
 * If an active session is cached, return it.  If not, create a new session,
 * cache it, begin a transaction (if supported), and return it.  If
 * unsuccessful, error thrown.
 *
 * This session will be released by afw_adaptor_session_commit_and_release_cache().
 */
AFW_DECLARE(const afw_adaptor_session_t *)
afw_adaptor_session_get_cached(
    const afw_utf8_t *adaptor_id,
    afw_boolean_t begin_transaction,
    afw_xctx_t *xctx);


/**
 * @brief Release an adaptor session created by afw_adaptor_session_create().
 * @param session to release.
 * @param xctx of caller.
 * @return Adaptor session.
 *
 * Any uncommitted changes are lost.
 *
 * See afw_adaptor_session_get_cached() for using cached session.
 */
AFW_DECLARE(void)
afw_adaptor_session_release(
    const afw_adaptor_session_t *session,
    afw_xctx_t *xctx);

AFW_END_DECLARES

/** @} */

#endif /* __AFW_ADAPTOR_H__ */
