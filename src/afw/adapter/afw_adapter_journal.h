// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Adapter Journal
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_ADAPTER_JOURNAL_H__
#define __AFW_ADAPTER_JOURNAL_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_adapter
 *
 * @{
 */

/**
 * @file afw_adapter_journal.h
 * @brief Adaptive Framework adapter journal
 */

AFW_BEGIN_DECLARES


/**
 * @brief Journal - advance cursor for consumer.
 * @param adapter_id of journal.
 * @param consumer_id of associated peer object.
 * @param limit is number of entries to search to find applicable entry.
 * @param p is pool for result.
 * @param xctx of caller
 */
AFW_DECLARE(const afw_object_t *)
afw_adapter_journal_advance_cursor_for_consumer(
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *consumer_id,
    afw_size_t limit,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Journal - get first entry.
 * @param adapter_id of journal.
 * @param p is pool for result.
 * @param xctx of caller.
 */
AFW_DECLARE(const afw_object_t *)
afw_adapter_journal_get_first(
    const afw_utf8_t *adapter_id,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Journal - get entry at cursor.
 * @param adapter_id of journal.
 * @param cursor of entry to retrieve.
 * @param p is pool for result.
 * @param xctx of caller
 */
AFW_DECLARE(const afw_object_t *)
afw_adapter_journal_get_by_cursor(
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *cursor,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Journal - get next entry after cursor.
 * @param adapter_id of journal.
 * @param cursor of entry to get next entry after.
 * @param p is pool for result.
 * @param xctx of caller
 */
AFW_DECLARE(const afw_object_t *)
afw_adapter_journal_get_next_after_cursor(
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *cursor,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Journal - get next entry for consumer.
 * @param adapter_id of journal.
 * @param consumer_id of associated peer object.
 * @param limit is number of entries to search to find applicable entry.
 * @param p is pool for result.
 * @param xctx of caller
 */
AFW_DECLARE(const afw_object_t *)
afw_adapter_journal_get_next_for_consumer(
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *consumer_id,
    afw_size_t limit,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Journal - get next entry after cursor for consumer.
 * @param adapter_id of journal.
 * @param consumer_id of associated peer object.
 * @param cursor of entry to get next entry after.
 * @param limit is number of entries to search to find applicable entry.
 * @param p is pool for result.
 * @param xctx of caller
 */
AFW_DECLARE(const afw_object_t *)
afw_adapter_journal_get_next_for_consumer_after_cursor(
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *consumer_id,
    const afw_utf8_t *cursor,
    afw_size_t limit,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Journal - mark entry consumed by consumer.
 * @param adapter_id of journal.
 * @param consumer_id of associated peer object.
 * @param cursor of entry to get next entry after.
 * @param p is pool for result.
 * @param xctx of caller
 */
AFW_DECLARE(void)
afw_adapter_journal_mark_consumed(
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *consumer_id,
    const afw_utf8_t *cursor,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/* _AdaptiveJournalEntry update_object(). */
AFW_DECLARE(void)
afw_adapter_journal_entry_consume(
    const afw_adapter_session_t *session,
    const afw_utf8_t *object_id,
    const afw_object_t *update_object,
    afw_xctx_t *xctx);



AFW_END_DECLARES

/** @} */

#endif /* __AFW_ADAPTER_JOURNAL_H__ */
