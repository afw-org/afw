// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_adapter_journal interface for LMDB
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_lmdb_journal.c
 * @brief Implementation of afw_adapter_journal interface for LMDB.
 */

#include "afw.h"
#include "afw_lmdb.h"
#include "afw_lmdb_internal.h"
#include "generated/afw_lmdb_generated.h"
#include "afw_adapter_impl.h"
#include "lmdb.h"

/* Declares and rti/inf defines for interface afw_adapter_journal */
#define AFW_IMPLEMENTATION_ID "lmdb"
#include "afw_adapter_journal_impl_declares.h"

afw_lmdb_journal_t *
afw_lmdb_journal_create(
    afw_lmdb_adapter_session_t * session,
    afw_xctx_t *xctx)
{
    afw_lmdb_journal_t *self;

    self = afw_xctx_calloc_type(afw_lmdb_journal_t, xctx);

    self->pub.inf = &impl_afw_adapter_journal_inf;
    self->session = (afw_adapter_session_t *)session;

    return self;
}


/*
 * Implementation of method add_entry of interface afw_adapter_journal.
 */
const afw_utf8_t *
impl_afw_adapter_journal_add_entry(
    const afw_adapter_journal_t * instance,
    const afw_adapter_impl_request_t * impl_request,
    const afw_object_t * entry,
    afw_xctx_t *xctx)
{
    /* Assign instance pointer to self. */
    afw_lmdb_journal_t * self =
        (afw_lmdb_journal_t *)instance;
    afw_lmdb_adapter_session_t *session = 
        (afw_lmdb_adapter_session_t *)self->session;
    afw_lmdb_adapter_t *adapter = 
        (afw_lmdb_adapter_t *)session->adapter;
    const afw_utf8_t *token;
    const afw_memory_t *entry_string;
    MDB_dbi dbi;
    MDB_val key, data;
    MDB_cursor *cursor;
    apr_uint64_t t = 0;
    int rc;

    AFW_LMDB_BEGIN_TRANSACTION(adapter, session, 0, false, xctx) {

        /* open the journal database */
        dbi = afw_lmdb_internal_open_database(adapter, 
            AFW_LMDB_GET_TRANSACTION(),
            afw_lmdb_s_Journal, MDB_CREATE, xctx->p, xctx);

        /* first get the last entry in the database */
        cursor = afw_lmdb_internal_open_cursor(session, dbi, xctx);
        if (cursor == NULL) {
            /* unable to get a cursor over the journal database */
            AFW_THROW_ERROR_Z(general,
                "Error getting cursor for journal database.", xctx);
        }

        rc = mdb_cursor_get(cursor, &key, &data, MDB_LAST);
        if (rc == MDB_NOTFOUND) {
            /* no entries here */
            t = 0;
            key.mv_data = (void *) &t;
            key.mv_size = sizeof(t);
        } else if (rc == 0) {
            t = *((apr_uint64_t *)(key.mv_data));
            AFW_ENDIAN_BIG_TO_NATIVE_64(&t);
        }

        /* increment the key for the next entry */
        t++;
        AFW_ENDIAN_NATIVE_TO_BIG_64(&t);
        key.mv_data = &t;

        /* store the entry into the next data pointer */
        entry_string = afw_content_type_object_to_raw(
            adapter->ubjson, entry, NULL, xctx->p, xctx);
        data.mv_data = (void *)entry_string->ptr;
        data.mv_size = entry_string->size;

        rc = mdb_cursor_put(cursor, &key, &data, MDB_APPEND);
        if (rc) {
            /* error appending key/data to our journal database */
            AFW_THROW_ERROR_RV_Z(general, lmdb_journal, rc,
                "Error appending journal entry database.", xctx);
        }

        mdb_cursor_close(cursor);

        AFW_LMDB_COMMIT_TRANSACTION();
    }
    AFW_LMDB_END_TRANSACTION();

    /* return the new integer id */
    AFW_ENDIAN_BIG_TO_NATIVE_64(&t);

    token = afw_utf8_printf(xctx->p, xctx, "%lu", t);

    return token;
}

const afw_object_t *
afw_lmdb_adapter_journal_get_peer_object(
    afw_lmdb_journal_t * self,
    afw_lmdb_adapter_session_t * session,
    afw_lmdb_adapter_t * adapter,
    MDB_dbi dbi,
    MDB_txn * txn,
    const afw_uuid_t *uuid,
    afw_xctx_t *xctx)
{
    const afw_object_t *object = NULL;
    const afw_value_t *value;
    afw_memory_t raw;
    MDB_val key, data;

    memset(&data, 0, sizeof(MDB_val));
    key.mv_data = (void*)uuid;
    key.mv_size = sizeof(afw_uuid_t);

    if (mdb_get(txn, dbi, &key, &data) == 0) {
        raw.size = data.mv_size;
        raw.ptr = data.mv_data;

        value = afw_content_type_raw_to_value(adapter->ubjson, &raw, NULL,
            xctx->p, xctx);

        if (value && afw_value_is_object(value)) {
            object = afw_value_as_object(value, xctx);
        }
    } else {
        /* no entry found */
    }

    /* return a copy of the object that we may need to modify */
    return object;
}

void
afw_lmdb_journal_update_peer(
    afw_lmdb_journal_t * self,
    afw_lmdb_adapter_session_t * session,
    afw_lmdb_adapter_t *adapter,
    MDB_dbi dbi,
    const afw_uuid_t *uuid,
    const afw_object_t *updated_object,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *object_id;

    object_id = afw_uuid_to_utf8(uuid, xctx->p, xctx);

    afw_lmdb_internal_replace_entry_from_object(session,
        afw_s__AdaptiveJournalEntry_, object_id, updated_object,
        dbi, xctx);
}

const afw_object_t *
afw_lmdb_adapter_journal_get_entry_object(
    afw_lmdb_journal_t * self,
    afw_lmdb_adapter_session_t * session,
    afw_lmdb_adapter_t * adapter,
    MDB_dbi dbi,
    MDB_txn * txn,
    apr_uint64_t cursor,
    afw_xctx_t *xctx)
{
    const afw_object_t *object = NULL;
    const afw_value_t *value;
    afw_memory_t raw;
    MDB_val key, data;

    /* The journal keys are in Big Endian for portability */
    AFW_ENDIAN_NATIVE_TO_BIG_64(&cursor);

    memset(&data, 0, sizeof(MDB_val));
    key.mv_data = (void *)&cursor;
    key.mv_size = sizeof(cursor);

    if (mdb_get(txn, dbi, &key, &data) == 0) {
        raw.size = data.mv_size;
        raw.ptr = data.mv_data;

        value = afw_content_type_raw_to_value(adapter->ubjson, &raw, NULL,
            xctx->p, xctx);

        if (value && afw_value_is_object(value)) {
            object = afw_value_as_object(value, xctx);
        }
    } else {
        /* no entry found */
    }

    /* return a copy of the object that we may need to modify */
    return object;
}

void
afw_lmdb_journal_get_first(
    afw_lmdb_journal_t * self,
    afw_lmdb_adapter_session_t *session,
    afw_lmdb_adapter_t * adapter,
    MDB_dbi dbiJournal,
    MDB_txn * txn,
    const afw_object_t * response,
    afw_xctx_t *xctx)
{
    const afw_object_t *entry;
    apr_uint64_t cursor;

    /* each entry is represented numerically */
    cursor = 1;

    entry = afw_lmdb_adapter_journal_get_entry_object(
        self, session, adapter, dbiJournal, txn, cursor, xctx);
    if (entry) {
        afw_object_set_property_as_string(response, afw_s_entryCursor,
                afw_utf8_printf(xctx->p, xctx, "%lu", cursor), xctx);

        /* Set entry property in response. */
        afw_object_set_property_as_object(response, afw_s_entry, entry,
            xctx);
    }
}

void
afw_lmdb_journal_get_by_cursor(
    afw_lmdb_journal_t * self,
    afw_lmdb_adapter_session_t *session,
    afw_lmdb_adapter_t * adapter,
    MDB_dbi dbiJournal,
    MDB_txn * txn,
    const afw_utf8_t * entry_cursor,
    const afw_object_t * response,
    afw_xctx_t *xctx)
{
    const afw_object_t *entry;
    apr_uint64_t cursor;

    cursor = apr_strtoi64(
        afw_utf8_to_utf8_z(entry_cursor, xctx->p, xctx), NULL, 10);

    entry = afw_lmdb_adapter_journal_get_entry_object(
        self, session, adapter, dbiJournal, txn, cursor, xctx);
    if (entry) {
        afw_object_set_property_as_string(response, afw_s_entryCursor,
                afw_utf8_printf(xctx->p, xctx, "%lu", cursor), xctx);

        /* Set entry property in response. */
        afw_object_set_property_as_object(response, afw_s_entry, entry,
            xctx);
    }
}

void
afw_lmdb_journal_get_next_after_cursor(
    afw_lmdb_journal_t * self,
    afw_lmdb_adapter_session_t *session,
    afw_lmdb_adapter_t * adapter,
    MDB_dbi dbiJournal,
    MDB_txn * txn,
    const afw_utf8_t * entry_cursor,
    afw_size_t limit,
    const afw_object_t * response,
    afw_xctx_t *xctx)
{
    const afw_object_t *entry;
    apr_uint64_t cursor;

    /* set our cursor to one after the entry_cursor */
    cursor = apr_strtoi64(
        afw_utf8_to_utf8_z(entry_cursor, xctx->p, xctx), NULL, 10) + 1;

    entry = afw_lmdb_adapter_journal_get_entry_object(
        self, session, adapter, dbiJournal, txn, cursor, xctx);
    if (entry) {
        afw_object_set_property_as_string(response, afw_s_entryCursor,
                afw_utf8_printf(xctx->p, xctx, "%lu", cursor), xctx);

        /* Set entry property in response. */
        afw_object_set_property_as_object(response, afw_s_entry, entry,
            xctx);
    }
}

void
afw_lmdb_journal_get_next_for_consumer_after_cursor(
    afw_lmdb_journal_t * self,
    afw_lmdb_adapter_session_t *session,
    afw_lmdb_adapter_t * adapter,
    MDB_dbi dbiJournal,
    MDB_txn * txn,
    const afw_utf8_t * consumer_id,
    const afw_utf8_t * entry_cursor,
    afw_size_t limit,
    const afw_object_t * response,
    afw_xctx_t *xctx)
{
    const afw_object_t *entry = NULL;
    MDB_dbi dbiConsumers;
    apr_uint64_t cursor;
    const afw_uuid_t *uuid;
    const afw_object_t *peer;
    const afw_utf8_t *advance_cursor;
    const afw_utf8_t *cursor_str;
    const afw_value_t *consumer_filter;
    afw_boolean_t found = AFW_FALSE;
    const afw_dateTime_t *now;
    afw_size_t i;
    
    /* In this routine, we can simply start with entry_cursor 
        and work our way up until we find the next entry */
    cursor = apr_strtoi64(
        afw_utf8_to_utf8_z(entry_cursor, xctx->p, xctx), NULL, 10);

    dbiConsumers = afw_lmdb_internal_open_database(session->adapter, 
        txn, afw_lmdb_s_Primary, 0, xctx->p, xctx);

    /* lookup the cursor from the consumer database */
    uuid = afw_uuid_from_utf8(consumer_id, xctx->p, xctx);

    peer = afw_lmdb_adapter_journal_get_peer_object(
        self, session, adapter, dbiConsumers, txn, uuid, xctx);
    if (peer == NULL) {
        AFW_THROW_ERROR_Z(general,
            "Error, provisioning peer not found.", xctx);
    }

    advance_cursor = afw_object_old_get_property_as_utf8(
        peer, afw_s_advanceCursor, xctx->p, xctx);
    consumer_filter = afw_object_get_property(
        peer, afw_s_consumeFilter, xctx);

    /* Scan our journal until we find an applicable entry, or hit our limit */
    for (i = 0; (i < limit) || !found;  i++) {
        entry = afw_lmdb_adapter_journal_get_entry_object(self,
            session, adapter, dbiJournal, txn, cursor, xctx);

        if (entry) {
            /* Now, determine if it's applicable */
            if (afw_adapter_impl_is_journal_entry_applicable(
                    (const afw_adapter_journal_t *)self,
                    entry, peer, &consumer_filter, xctx)) {
                found = AFW_TRUE;
            } else cursor++;

        } else {
            /* No more entries left, so we're done */
            break;
        }
    }

    /* make a string out of the cursor */
    cursor_str = afw_utf8_printf(xctx->p, xctx, "%lu", cursor);

    /* update our last contact time */
    now = afw_dateTime_now_utc(xctx->p, xctx);

    if (found) {
        /* set our consumption properties */
        afw_object_set_property_as_dateTime(peer,
            afw_s_consumeStartTime, now, xctx);
        afw_object_set_property_as_string(peer,
            afw_s_consumeCursor, cursor_str, xctx);
        afw_object_set_property_as_string(peer,
            afw_s_currentCursor, cursor_str, xctx);
        afw_object_remove_property(peer,
            afw_s_advanceCursor, xctx);

        /* set our entry cursor */
        afw_object_set_property_as_string(response,
            afw_s_entryCursor, cursor_str, xctx);

        /* set the entry to be returned */
        afw_object_set_property_as_object(response,
            afw_s_entry, entry, xctx);
    } else {
        /* we may need to increase the advanceCursor */
        if (advance_cursor) {
            /** @fixme: is the cursor further along than our existing advanceCursor? */

        } else {
            afw_object_set_property_as_string(peer,
                afw_s_advanceCursor, cursor_str, xctx);
        }
    }

    afw_object_set_property_as_dateTime(peer,
        afw_s_lastContactTime, now, xctx);

    /* update the peer object */
    afw_lmdb_journal_update_peer(self, session, adapter,
        dbiConsumers, uuid, peer, xctx);
}

void
impl_afw_adapter_journal_get_next_for_consumer(
    afw_lmdb_journal_t * self,
    afw_lmdb_adapter_session_t *session,
    afw_lmdb_adapter_t *adapter,
    MDB_dbi dbiJournal,
    MDB_txn * txn,
    const afw_utf8_t * consumer_id,
    afw_size_t limit,
    const afw_object_t * response,
    afw_xctx_t *xctx)
{
    const afw_object_t *entry = NULL;
    apr_uint64_t cursor;
    MDB_dbi dbiConsumers;
    const afw_uuid_t *uuid;
    const afw_object_t *peer;
    const afw_utf8_t *current_cursor;
    const afw_utf8_t *advance_cursor;
    const afw_utf8_t *consume_cursor;
    const afw_utf8_t *cursor_str;
    const afw_value_t *consumer_filter;
    afw_boolean_t found = AFW_FALSE;
    const afw_dateTime_t *now;
    afw_size_t i;

    dbiConsumers = afw_lmdb_internal_open_database(session->adapter,
        txn, afw_lmdb_s_Primary, 0, xctx->p, xctx);

    /* lookup the cursor from the consumer database */
    uuid = afw_uuid_from_utf8(consumer_id, xctx->p, xctx);

    peer = afw_lmdb_adapter_journal_get_peer_object(
        self, session, adapter, dbiConsumers, txn, uuid, xctx);
    if (peer == NULL) {
        AFW_THROW_ERROR_Z(general,
            "Error, provisioning peer not found.", xctx);
    }

    current_cursor = afw_object_old_get_property_as_utf8(
        peer, afw_s_currentCursor, xctx->p, xctx);
    advance_cursor = afw_object_old_get_property_as_utf8(
        peer, afw_s_advanceCursor, xctx->p, xctx);
    consumer_filter = afw_object_get_property(
        peer, afw_s_consumeFilter, xctx);
    consume_cursor = afw_object_old_get_property_as_string(
        peer, afw_s_consumeCursor, xctx);

    /** @fixme: we'll have to consider the scenario where we get
        a consume_cursor (re-issue) and now it's removed from the
        journal */

    if (consume_cursor)
        cursor = apr_strtoi64(
            afw_utf8_to_utf8_z(consume_cursor, xctx->p, xctx), NULL, 10);
    else if (advance_cursor)
        cursor = apr_strtoi64(
            afw_utf8_to_utf8_z(advance_cursor, xctx->p, xctx), NULL, 10);
    else if (current_cursor)
        cursor = apr_strtoi64(
            afw_utf8_to_utf8_z(current_cursor, xctx->p, xctx), NULL, 10) + 1;
    else /* start at the beginning */
        cursor = 1;

    /* Scan our journal until we find an applicable entry, or hit our limit */
    for (i = 0; (i < limit) || !found;  i++) {
        entry = afw_lmdb_adapter_journal_get_entry_object(self,
            session, adapter, dbiJournal, txn, cursor, xctx);

        if (entry) {
            /* Now, determine if it's applicable */
            if (afw_adapter_impl_is_journal_entry_applicable(
                    (const afw_adapter_journal_t *)self, 
                    entry, peer, &consumer_filter, xctx)) {
                found = AFW_TRUE;
            } else cursor++;
             
        } else {
            /* No more entries left, so we're done */
            break;
        }
    }

    /* make a string out of the cursor */
    cursor_str = afw_utf8_printf(xctx->p, xctx, "%lu", cursor);

    /* update our last contact time */
    now = afw_dateTime_now_utc(xctx->p, xctx);
    
    if (found) {
        /* check to see if this is a re-issue */
        if (consume_cursor) {
            afw_object_set_property_as_boolean(response, afw_s_reissue,
                true, xctx);
        } else {
            /* not a re-issue, so set our consumption properties */
            afw_object_set_property_as_dateTime(peer, 
                afw_s_consumeStartTime, now, xctx);
            afw_object_set_property_as_string(peer, 
                afw_s_consumeCursor, cursor_str, xctx);
            afw_object_set_property_as_string(peer, 
                afw_s_currentCursor, cursor_str, xctx);
            afw_object_remove_property(peer, 
                afw_s_advanceCursor, xctx);
        }

        /* set our entry cursor */
        afw_object_set_property_as_string(response, 
            afw_s_entryCursor, cursor_str, xctx);

        /* set the entry to be returned */
        afw_object_set_property_as_object(response, 
            afw_s_entry, entry, xctx);
    } else {
        /* we may need to increase the advanceCursor */
        if (advance_cursor) {
            /** @fixme: is the cursor further along than our existing advanceCursor? */

        } else {
            afw_object_set_property_as_string(peer, 
                afw_s_advanceCursor, cursor_str, xctx); 
        }
    }

    afw_object_set_property_as_dateTime(peer,
        afw_s_lastContactTime, now, xctx);   

    /* update the peer object */ 
    afw_lmdb_journal_update_peer(self, session, adapter,
        dbiConsumers, uuid, peer, xctx); 
}

void
afw_lmdb_journal_advance_cursor_for_consumer(
    afw_lmdb_journal_t * self,
    afw_lmdb_adapter_session_t * session,
    afw_lmdb_adapter_t * adapter,
    MDB_dbi dbiJournal,
    MDB_txn * txn,
    const afw_utf8_t * consumer_id,
    afw_size_t limit,
    const afw_object_t * response,
    afw_xctx_t *xctx)
{
    const afw_object_t *entry;
    apr_uint64_t cursor;
    MDB_dbi dbiConsumers;
    const afw_uuid_t *uuid;
    const afw_object_t *peer;
    const afw_utf8_t *current_cursor;
    const afw_utf8_t *advance_cursor;
    const afw_utf8_t *consume_cursor;
    const afw_utf8_t *cursor_str;
    const afw_value_t *consumer_filter;
    afw_boolean_t found = AFW_FALSE;
    const afw_dateTime_t *now;
    afw_size_t i;

    dbiConsumers = afw_lmdb_internal_open_database(session->adapter,
        txn, afw_lmdb_s_Primary, 0, xctx->p, xctx);

    /* lookup the cursor from the consumer database */
    uuid = afw_uuid_from_utf8(consumer_id, xctx->p, xctx);

    peer = afw_lmdb_adapter_journal_get_peer_object(
        self, session, adapter, dbiConsumers, txn, uuid, xctx);
    if (peer == NULL) {
        AFW_THROW_ERROR_Z(general,
            "Error, provisioning peer not found.", xctx);
    }

    current_cursor = afw_object_old_get_property_as_utf8(
        peer, afw_s_currentCursor, xctx->p, xctx);
    advance_cursor = afw_object_old_get_property_as_utf8(
        peer, afw_s_advanceCursor, xctx->p, xctx);
    consumer_filter = afw_object_get_property(
        peer, afw_s_consumeFilter, xctx);
    consume_cursor = afw_object_old_get_property_as_string(
        peer, afw_s_consumeCursor, xctx);

    /** @fixme: we'll have to consider the scenario where we get
        a consume_cursor (re-issue) and now it's removed from the
        journal */

    if (consume_cursor)
        cursor = apr_strtoi64(
            afw_utf8_to_utf8_z(consume_cursor, xctx->p, xctx), NULL, 10);
    else if (advance_cursor)
        cursor = apr_strtoi64(
            afw_utf8_to_utf8_z(advance_cursor, xctx->p, xctx), NULL, 10);
    else if (current_cursor)
        cursor = apr_strtoi64(
            afw_utf8_to_utf8_z(current_cursor, xctx->p, xctx), NULL, 10) + 1;
    else /* start at the beginning */
        cursor = 1;

    /* Scan our journal until we find an applicable entry, or hit our limit */
    for (i = 0; (i < limit) || !found;  i++) {
        entry = afw_lmdb_adapter_journal_get_entry_object(self,
            session, adapter, dbiJournal, txn, cursor, xctx);

        if (entry) {
            /* Now, determine if it's applicable */
            if (afw_adapter_impl_is_journal_entry_applicable(
                    (const afw_adapter_journal_t *)self,
                    entry, peer, &consumer_filter, xctx)) {
                found = AFW_TRUE;
            } else cursor++;

        } else {
            /* No more entries left, so we're done */
            break;
        }
    }

    /* make a string out of the cursor */
    cursor_str = afw_utf8_printf(xctx->p, xctx, "%lu", cursor);

    /* update our last contact time */
    now = afw_dateTime_now_utc(xctx->p, xctx);

    if (found) {
        /* check to see if this is a re-issue */
        if (consume_cursor) {
            afw_object_set_property_as_boolean(response, afw_s_reissue,
                true, xctx);
        } else {
            /* not a re-issue, so set our consumption properties */
            afw_object_set_property_as_dateTime(peer,
                afw_s_consumeStartTime, now, xctx);
            afw_object_set_property_as_string(peer,
                afw_s_consumeCursor, cursor_str, xctx);
            afw_object_set_property_as_string(peer,
                afw_s_currentCursor, cursor_str, xctx);
            afw_object_remove_property(peer,
                afw_s_advanceCursor, xctx);
        }

        /* set our entry cursor */
        afw_object_set_property_as_string(response,
            afw_s_entryCursor, cursor_str, xctx);
    } else {
        /* we may need to increase the advanceCursor */
        if (advance_cursor) {
            /** @fixme: is the cursor further along than our existing advanceCursor? */

        } else {
            afw_object_set_property_as_string(peer,
                afw_s_advanceCursor, cursor_str, xctx);
        }
    }

    afw_object_set_property_as_dateTime(peer,
        afw_s_lastContactTime, now, xctx);

    /* update the peer object */
    afw_lmdb_journal_update_peer(self, session, adapter,
        dbiConsumers, uuid, peer, xctx);
}


/*
 * Implementation of method get_entry of interface afw_adapter_journal.
 */
void
impl_afw_adapter_journal_get_entry(
    const afw_adapter_journal_t * instance,
    const afw_adapter_impl_request_t * impl_request,
    afw_adapter_journal_option_t option,
    const afw_utf8_t * consumer_id,
    const afw_utf8_t * entry_cursor,
    afw_size_t limit,
    const afw_object_t * response,
    afw_xctx_t *xctx)
{
    afw_lmdb_journal_t * self =
        (afw_lmdb_journal_t *)instance;
    afw_lmdb_adapter_session_t *session = 
        (afw_lmdb_adapter_session_t *)self->session;
    afw_lmdb_adapter_t *adapter = 
        (afw_lmdb_adapter_t *)session->adapter;
    MDB_dbi dbiJournal;    
    MDB_txn *txn;

    AFW_LMDB_BEGIN_TRANSACTION(adapter, session, 0, false, xctx) {

        txn = AFW_LMDB_GET_TRANSACTION();

        /* Open up our Journal database, for every call option */
        dbiJournal = afw_lmdb_internal_open_database(adapter, 
        txn, afw_lmdb_s_Journal, 0, xctx->p, xctx);
    
        switch (option) {
            case afw_adapter_journal_option_get_first:
                afw_lmdb_journal_get_first(self, 
                    session, adapter, dbiJournal, txn, response, xctx);
                break;
            case afw_adapter_journal_option_get_by_cursor:
                afw_lmdb_journal_get_by_cursor(self, session,
                    adapter, dbiJournal, txn, entry_cursor, response, xctx);
                break;
            case afw_adapter_journal_option_get_next_after_cursor:
                afw_lmdb_journal_get_next_after_cursor(self, session,
                    adapter, dbiJournal, txn, entry_cursor, limit, response, xctx);
                break;
            case afw_adapter_journal_option_get_next_for_consumer:
                impl_afw_adapter_journal_get_next_for_consumer(
                    self, session, adapter, dbiJournal, txn, consumer_id,
                    limit, response, xctx);
                break;
            case afw_adapter_journal_option_get_next_for_consumer_after_cursor:
                afw_lmdb_journal_get_next_for_consumer_after_cursor(
                    self, session, adapter, dbiJournal, txn, consumer_id,
                    entry_cursor, limit, response, xctx);
                break;
            case afw_adapter_journal_option_advance_cursor_for_consumer:
                afw_lmdb_journal_advance_cursor_for_consumer(
                    self, session, adapter, dbiJournal, txn, consumer_id,
                    limit, response, xctx);
                break;
        }
        
        /* commit our transaction */
        AFW_LMDB_COMMIT_TRANSACTION();
    }
    AFW_LMDB_END_TRANSACTION();
}



/*
 * Implementation of method mark_entry_consumed of interface afw_adapter_journal.
 */
void
impl_afw_adapter_journal_mark_entry_consumed(
    const afw_adapter_journal_t * instance,
    const afw_adapter_impl_request_t * impl_request,
    const afw_utf8_t * consumer_id,
    const afw_utf8_t * entry_cursor,
    afw_xctx_t *xctx)
{
    afw_lmdb_journal_t * self =
        (afw_lmdb_journal_t *)instance;
    afw_lmdb_adapter_session_t *session = 
        (afw_lmdb_adapter_session_t *)self->session;
    afw_lmdb_adapter_t *adapter = 
        (afw_lmdb_adapter_t *)session->adapter;
    const afw_object_t *peer;
    const afw_utf8_t *consume_cursor;
    const afw_uuid_t *uuid;
    MDB_dbi dbiConsumers;
    MDB_txn * txn;
    const afw_dateTime_t *now;    

    AFW_LMDB_BEGIN_TRANSACTION(adapter, session, 0, false, xctx) {

        txn = AFW_LMDB_GET_TRANSACTION();

        dbiConsumers = afw_lmdb_internal_open_database(adapter,
            txn, afw_lmdb_s_Primary, 0, xctx->p, xctx);

        /* lookup the cursor from the database */
        uuid = afw_uuid_from_utf8(consumer_id, xctx->p, xctx);

        peer = afw_lmdb_adapter_journal_get_peer_object(
            self, session, adapter, dbiConsumers, txn, uuid, xctx);

        consume_cursor = afw_object_old_get_property_as_string(peer,
            afw_s_consumeCursor, xctx);
        if (!consume_cursor || !afw_utf8_equal(entry_cursor, consume_cursor)) {
            AFW_THROW_ERROR_Z(general,
                "Object id supplied is not currently being consumed", xctx);
        }

        /* now, clear consumerStart/consumeCursor */
        afw_object_remove_property(peer, afw_s_consumeStartTime, xctx);
        afw_object_remove_property(peer, afw_s_consumeCursor, xctx);

        /* Update lastContactTime property */
        now = afw_dateTime_now_utc(xctx->p, xctx);
        afw_object_set_property_as_dateTime(peer, afw_s_lastContactTime,
            now, xctx);

        /* write out the object and commit */
        afw_lmdb_journal_update_peer(self, session, adapter,
            dbiConsumers, uuid, peer, xctx); 

        AFW_LMDB_COMMIT_TRANSACTION();
    }
    AFW_LMDB_END_TRANSACTION();
}
