// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_adaptor_session interface for LMDB
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_lmdb_adaptor_session.c
 * @brief Implementation of afw_adaptor_session interface for LMDB
 */

#include "afw.h"
#include "afw_adaptor_impl.h"
#include "afw_adaptor_impl_index.h"
#include "generated/afw_lmdb_generated.h"
#include "afw_lmdb_internal.h"
#include "afw_lmdb_metadata.h"
#include "afw_lmdb_index.h"

/* Declares and rti/inf defines for interface afw_adaptor_session */
#define AFW_IMPLEMENTATION_ID "lmdb"
#include "afw_adaptor_session_impl_declares.h"


void afw_lmdb_adaptor_session_dump_objects(
    afw_lmdb_adaptor_session_t * session,
    MDB_txn * txn,
    const afw_utf8_t *object_type_id,
    const afw_query_criteria_t * criteria,
    afw_object_cb_t callback,
    void * context, 
    const afw_pool_t *p,
    afw_xctx_t *xctx);



afw_lmdb_adaptor_session_t *afw_lmdb_adaptor_session_create(
    afw_lmdb_adaptor_t *adaptor,
    afw_xctx_t *xctx)
{
    afw_lmdb_adaptor_session_t *self;

    self = afw_xctx_calloc_type(afw_lmdb_adaptor_session_t, xctx);
    self->pub.inf = &impl_afw_adaptor_session_inf;
    self->pub.adaptor = (afw_adaptor_t *)adaptor;
    self->pub.p = xctx->p;
    self->adaptor = adaptor;
 
    /* create our index implementation */
    self->indexer = afw_lmdb_adaptor_impl_index_create(self, 
        adaptor, NULL, xctx);

    /* create our journal */
    self->journal = afw_lmdb_journal_create(self, xctx); 
    self->key_value = afw_lmdb_key_value_create(self, xctx); 

    /* Return session. */
    return self;
}


/*
 * Implementation of method destroy of interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_destroy(
    const afw_adaptor_session_t * instance,
    afw_xctx_t *xctx)
{
}


/*
 * Implementation of method retrieve_objects for interface
 * afw_adaptor_session.
 */
void
impl_afw_adaptor_session_retrieve_objects(
    const afw_adaptor_session_t *instance,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_query_criteria_t *criteria,
    void *context,
    afw_object_cb_t callback,
    const afw_object_t *adaptor_type_specific,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_lmdb_adaptor_session_t * self = (afw_lmdb_adaptor_session_t *)instance;
    afw_lmdb_adaptor_t *adaptor = (afw_lmdb_adaptor_t *)self->adaptor;
    MDB_txn * txn;

    if (afw_lmdb_metadata_handles(object_type_id)) {
        afw_lmdb_metadata_retrieve_objects(
            self, object_type_id, criteria, xctx);
    }

    /* open the transaction */
    AFW_LMDB_BEGIN_TRANSACTION(adaptor, self, MDB_RDONLY, false, xctx) {

        txn = AFW_LMDB_GET_TRANSACTION();

        /* if we don't have an object_type_id, we need to do a dump */
        if (object_type_id == NULL) {
            /* no object_type_id, or id is a meta properties, then we need to dump */
            afw_lmdb_adaptor_session_dump_objects(self, txn, NULL, criteria,
                callback, context, p, xctx);
        }

        /* determine if the filter criteria contains predicates that allow us to index */
        else if (afw_adaptor_impl_index_sargable(self->indexer, 
            object_type_id, criteria, xctx)) 
        {
            afw_adaptor_impl_index_query(self->indexer, object_type_id, 
                criteria, callback, context, p, xctx);
        } 

        else {
            /* the search criteria cannot help us, so we need to scan the entire
                database and filter, if necessary.  We use the object_type_id to
                index over entries for this type */
            afw_lmdb_adaptor_session_dump_objects(self, txn, object_type_id,
                criteria, callback, context, p, xctx);
        }
    }
    AFW_LMDB_END_TRANSACTION();
}


/*
 * Implementation of method get_object for interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_get_object(
    const afw_adaptor_session_t *instance,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    void *context,
    afw_object_cb_t callback,
    const afw_object_t *adaptor_type_specific,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_lmdb_adaptor_session_t * self = (afw_lmdb_adaptor_session_t *)instance;
    afw_lmdb_adaptor_t *adaptor = (afw_lmdb_adaptor_t *)self->adaptor;
    MDB_dbi dbi;
    MDB_txn *txn;
    const afw_object_t *object;

    /*
     * If object_type_id is _AdaptiveObjectType_, return a generic object type for
     * now.  This may change if the lmdb adaptor starts storing object types.
     */
    if (afw_utf8_equal(object_type_id, &afw_s__AdaptiveObjectType_)) {
        object = afw_adaptor_impl_generic_object_type_object_get(
            instance->adaptor, object_id, p, xctx);
        callback(object, context, xctx);
        return;
    }

    /* begin the transaction */
    AFW_LMDB_BEGIN_TRANSACTION(adaptor, self, MDB_RDONLY, false, xctx) {

        txn = AFW_LMDB_GET_TRANSACTION();

        /* open our database */
        dbi = afw_lmdb_internal_open_database(adaptor, 
            txn, &afw_lmdb_s_Primary, 0, p, xctx);

        /* Callback with object. */
        callback(
            afw_lmdb_internal_create_object_from_entry(self,
                object_type_id, object_id, dbi, xctx),
            context, xctx);
    }
    AFW_LMDB_END_TRANSACTION();
}



/*
 * Implementation of method add_object for interface afw_adaptor_session.
 */
const afw_utf8_t *
impl_afw_adaptor_session_add_object(
    const afw_adaptor_session_t *instance,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *suggested_object_id,
    const afw_object_t *object,
    const afw_object_t *adaptor_type_specific,
    afw_xctx_t *xctx)
{
    afw_lmdb_adaptor_session_t *self = (afw_lmdb_adaptor_session_t *)instance;
    afw_lmdb_adaptor_t *adaptor = (afw_lmdb_adaptor_t *)self->adaptor;
    MDB_dbi dbi;
    MDB_txn *txn;
    const afw_uuid_t *uuid;
    const afw_utf8_t *uuid_string;    

    if (suggested_object_id) {
        /* we were given an object_id, so use it */
        uuid_string = suggested_object_id;
    } else {
        /* generate a UUID for this new object */
        uuid = afw_uuid_create(xctx->p, xctx);
        uuid_string = afw_uuid_to_utf8(uuid, xctx->p, xctx);
    }

    AFW_LMDB_BEGIN_TRANSACTION(adaptor, self, 0, false, xctx) {

        txn = AFW_LMDB_GET_TRANSACTION();

        /* open our primary database */
        dbi = afw_lmdb_internal_open_database(adaptor, 
            txn, &afw_lmdb_s_Primary, MDB_CREATE, xctx->p, xctx);

        /* add the object to our primary database */
        afw_lmdb_internal_create_entry_from_object(self,
            object_type_id, uuid_string, object, dbi, xctx);

        /* create our indexes for this object */
        afw_adaptor_impl_index_object(self->indexer, object_type_id,
            object, uuid_string, xctx);

        AFW_LMDB_COMMIT_TRANSACTION();
    }
    AFW_LMDB_END_TRANSACTION();

    return uuid_string;
}



/*
 * Implementation of method modify_object for interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_modify_object(
    const afw_adaptor_session_t *instance,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_adaptor_modify_entry_t *const *entry,
    const afw_object_t *adaptor_type_specific,
    afw_xctx_t *xctx)
{
    afw_lmdb_adaptor_session_t *self = (afw_lmdb_adaptor_session_t *)instance;
    afw_lmdb_adaptor_t *adaptor = (afw_lmdb_adaptor_t *)self->adaptor;    
    const afw_object_t *object, *new_object;
    MDB_dbi dbi;    
    MDB_txn *txn;

    if (!object_id || !object_type_id) {
        AFW_THROW_ERROR_Z(bad_request,
            "Missing id or object_type.", xctx);
    }

    AFW_LMDB_BEGIN_TRANSACTION(adaptor, self, 0, false, xctx) {

        txn = AFW_LMDB_GET_TRANSACTION(),

        /* open our primary database */
        dbi = afw_lmdb_internal_open_database(adaptor, 
            txn, &afw_lmdb_s_Primary, 0, xctx->p, xctx);

        /* load our object into memory and apply the modifications */
        object = afw_lmdb_internal_create_object_from_entry(self,
            object_type_id, object_id, dbi, xctx);

        /* call common routine to modify the object */
        new_object = afw_object_create_clone(object, xctx->p, xctx);
        afw_adaptor_modify_entries_apply_to_unnormalized_object(
            entry, new_object, xctx);

        /* Write updated object */
        afw_lmdb_internal_replace_entry_from_object(self, object_type_id, 
            object_id, new_object, dbi, xctx);

        /* Re-index object, if necessary */
        afw_adaptor_impl_index_reindex_object(self->indexer, object_type_id,
            object, new_object, object_id, xctx);

        AFW_LMDB_COMMIT_TRANSACTION();
    }
    AFW_LMDB_END_TRANSACTION();
}


/*
 * Implementation of method replace_object for interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_replace_object(
    const afw_adaptor_session_t *instance,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_object_t *replacement_object,
    const afw_object_t *adaptor_type_specific,
    afw_xctx_t *xctx)
{
    afw_lmdb_adaptor_session_t *self = (afw_lmdb_adaptor_session_t *)instance;
    afw_lmdb_adaptor_t *adaptor = (afw_lmdb_adaptor_t *)self->adaptor;    
    const afw_object_t *old_object;
    const afw_value_t *value;
    MDB_dbi dbi;    
    MDB_txn *txn;

    if (!object_id || !object_type_id) {
        AFW_THROW_ERROR_Z(bad_request,
            "Missing id or object_type.", xctx);
    }

    AFW_LMDB_BEGIN_TRANSACTION(adaptor, self, 0, false, xctx) {

        txn = AFW_LMDB_GET_TRANSACTION();

        /* open our primary database */
        dbi = afw_lmdb_internal_open_database(adaptor,
            txn, &afw_lmdb_s_Primary, 0, xctx->p, xctx);

        /* get the old object */
        value = afw_lmdb_internal_create_value_from_entry(self, 
                object_type_id, object_id, dbi, xctx);

        if (value && !afw_value_is_object(value)) {
            AFW_THROW_ERROR_Z(bad_request,
                "Not a valid object.", xctx);
        }

        old_object = afw_value_as_object(value, xctx);

        /* Write updated object */
        afw_lmdb_internal_replace_entry_from_object(self,
            object_type_id, object_id, replacement_object, dbi, xctx);

        /* Re-index object, if necessary */
        afw_adaptor_impl_index_reindex_object(self->indexer, object_type_id, 
            old_object, replacement_object, object_id, xctx);
    
        AFW_LMDB_COMMIT_TRANSACTION();
    }
    AFW_LMDB_END_TRANSACTION();
}


/*
 * Implementation of method delete_object for interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_delete_object(
    const afw_adaptor_session_t *instance,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_object_t *adaptor_type_specific,
    afw_xctx_t *xctx)
{
    afw_lmdb_adaptor_session_t *self = (afw_lmdb_adaptor_session_t *)instance;
    afw_lmdb_adaptor_t *adaptor = (afw_lmdb_adaptor_t *)self->adaptor;
    const afw_uuid_t *uuid;
    const afw_object_t *object;
    afw_memory_t raw_key;
    MDB_dbi dbi;
    MDB_txn *txn;
    int rc;

    uuid = afw_uuid_from_utf8(object_id, xctx->p, xctx);

    /* open the transaction */
    AFW_LMDB_BEGIN_TRANSACTION(adaptor, self, 0, false, xctx) {

        txn = AFW_LMDB_GET_TRANSACTION();

        dbi = afw_lmdb_internal_open_database(adaptor,
            txn, &afw_lmdb_s_Primary, 0, xctx->p, xctx);

        object = afw_lmdb_internal_create_object_from_entry(self, 
            object_type_id, object_id, dbi, xctx);
        if (!object) {
            AFW_THROW_ERROR_FZ(not_found, xctx,
                AFW_UTF8_FMT_Q " cannot be found.",
                AFW_UTF8_FMT_ARG(object_id));
        }

        /* remove the actual entry from the primary database */
        afw_lmdb_internal_set_key(&raw_key, object_type_id, uuid, xctx->p, xctx);

        rc = afw_lmdb_internal_delete_entry(
            AFW_LMDB_GET_TRANSACTION(),
            dbi, &raw_key, NULL, xctx);
        if (rc) {
            AFW_THROW_ERROR_RV_Z(general, lmdb, rc,
                "Error deleting value from primary database.", xctx);
        }

        AFW_TRY {
            /* remove any indexes that still exist */
            afw_adaptor_impl_index_unindex_object(self->indexer,
                object_type_id, object, object_id, xctx);
        }
        AFW_CATCH(not_found) {
            /** @fixme This is ok, but we may want to log an internal message. */
            break;
        }
        AFW_CATCH_UNHANDLED {
            /** @fixme we should not let the original error make it back to the client. */
            AFW_ERROR_RETHROW;
        }
        AFW_ENDTRY;

        AFW_LMDB_COMMIT_TRANSACTION();
    }
    AFW_LMDB_END_TRANSACTION();
}



/*
 * Implementation of method begin_transaction of interface afw_adaptor_session.
 */
const afw_adaptor_transaction_t *
impl_afw_adaptor_session_begin_transaction(
    const afw_adaptor_session_t * instance,
    afw_xctx_t *xctx)
{
    afw_lmdb_adaptor_session_t *self = (afw_lmdb_adaptor_session_t *)instance;
    afw_lmdb_transaction_t *transaction;

    /* create the session transaction at the time it's requested */
    transaction = afw_lmdb_transaction_create(self, xctx);

    return (afw_adaptor_transaction_t *)transaction;
}


/*
 * This routine does a sequential scan of the database, for
 * any and all object types, if specified.  When filter
 * criteria matches, the object is returned.
 */
void afw_lmdb_adaptor_session_dump_objects(
    afw_lmdb_adaptor_session_t * session,
    MDB_txn * txn,
    const afw_utf8_t *object_type_id,
    const afw_query_criteria_t * criteria,
    afw_object_cb_t callback,
    void * context, 
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *object;
    afw_utf8_t object_type;
    const afw_pool_t *obj_p;
    const afw_utf8_t *object_id;
    afw_memory_t raw, rawKey;
    afw_uuid_t uuid;
    MDB_cursor *cursor;
    MDB_val key, data;
    MDB_dbi dbi;
    int rc;
    afw_boolean_t abandon = false;

    /* Open the database where our primary objects are stored */
    dbi = afw_lmdb_internal_open_database(session->adaptor, txn, 
        &afw_lmdb_s_Primary, 0, p, xctx);

    /* Open a cursor against the object_type index database */
    cursor = afw_lmdb_internal_open_cursor(session, dbi, xctx);
    if (cursor == NULL) {
        AFW_THROW_ERROR_Z(general,
            "Error opening primary database cursor.", xctx);
    }

    /* 
     * If we have an object_type_id, then we can take advantage
     *  of LMDB's b-tree to start at.  Otherwise, start from the
     *  very beginning. 
     */
    memset(&data, 0, sizeof(MDB_val));
    if (object_type_id) {        
        key.mv_data = (void*)object_type_id->s;        
        key.mv_size = object_type_id->len;        
        rc = mdb_cursor_get(cursor, &key, &data, MDB_SET_RANGE);
    } else {
        /* else start at the very beginning */
        memset(&key, 0, sizeof(MDB_val));
        rc = mdb_cursor_get(cursor, &key, &data, MDB_FIRST);
    }
    
    if (rc == MDB_NOTFOUND) {
        /* no entries */
        mdb_cursor_close(cursor);
        callback(NULL, context, xctx);
        return;
    } else if (rc) {
        mdb_cursor_close(cursor);
        AFW_THROW_ERROR_RV_Z(general, lmdb_internal, rc,
            "Error getting cursor for primary database.", xctx);
    }

    /* loop until we hit the end of our cursor */
    do {
        raw.ptr = data.mv_data;
        raw.size = data.mv_size;
        rawKey.ptr = key.mv_data;
        rawKey.size = key.mv_size;


        /* create temporary pool for object memory */
        obj_p = afw_pool_create(p, xctx);

        afw_lmdb_internal_get_key(&rawKey, &object_type, &uuid);
        if (object_type.len == 0) {
            /* skip objects that have no type */
            afw_pool_release(obj_p, xctx);
            continue;
        }

        /* convert our uuid back into a utf8 string */
        object_id = afw_uuid_to_utf8(&uuid, obj_p, xctx);

        object = afw_content_type_raw_to_object(
            session->adaptor->ubjson, &raw, NULL, &session->adaptor->pub.adaptor_id,
            &object_type, object_id, true, obj_p, xctx);

        /* if object_type_id was specified, check it */
        if (object_type_id) {
            if (!afw_utf8_equal(object_type_id, &object_type)) {
                /* Now we're onto the next object_type, so just quit */
                mdb_cursor_close(cursor);
                afw_object_release(object, xctx);
                callback(NULL, context, xctx);
                return;
            }
        }

        abandon = false;
        if (afw_query_criteria_test_object(object, criteria,
            p, xctx)) 
        {
            afw_object_meta_set_ids(object, &session->adaptor->pub.adaptor_id,
                &object_type, object_id, xctx);

            /* we have an entry, so callback to process it */
            abandon = callback(object, context, xctx);
        }

    } while (!abandon && mdb_cursor_get(cursor, &key, &data, MDB_NEXT) == 0);

    mdb_cursor_close(cursor);
    callback(NULL, context, xctx);
}


/*
 * Implementation of method get_journal of interface afw_adaptor_session.
 */
const afw_adaptor_journal_t *
impl_afw_adaptor_session_get_journal_interface(
    const afw_adaptor_session_t * instance,
    afw_xctx_t *xctx)
{
    afw_lmdb_adaptor_session_t *self = (afw_lmdb_adaptor_session_t *)instance;

    return (afw_adaptor_journal_t *)self->journal;
}



/*
 * Implementation of method get_key_value_interface of interface
 * afw_adaptor_session.
 */
const afw_adaptor_key_value_t *
impl_afw_adaptor_session_get_key_value_interface (
    const afw_adaptor_session_t * instance,
    afw_xctx_t *xctx)
{
    afw_lmdb_adaptor_session_t *self = (afw_lmdb_adaptor_session_t *)instance;

    return (afw_adaptor_key_value_t *)self->key_value;
}


/*
 * Implementation of method get_index_interface of interface afw_adaptor_session.
 */
const afw_adaptor_impl_index_t *
impl_afw_adaptor_session_get_index_interface (
    const afw_adaptor_session_t * instance,
    afw_xctx_t *xctx)
{
    afw_lmdb_adaptor_session_t *self = (afw_lmdb_adaptor_session_t *)instance;

    return self->indexer;
}



/*
 * Implementation of method get_object_type_cache_interface for interface
 * afw_adaptor_session.
 */
const afw_adaptor_object_type_cache_t *
impl_afw_adaptor_session_get_object_type_cache_interface(
    const afw_adaptor_session_t * instance,
    afw_xctx_t *xctx)
{
    /* There is on adaptor cache. */
    return NULL;
}
