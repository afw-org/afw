// See the 'COPYING' file in the project root for licensing information.
/*
 * Indexing routines for LMDB Adaptor
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_lmdb_index.c
 * @brief Indexing routines for the LMDB Adaptor
 */

#include "afw.h"
#include "afw_uuid.h"
#include "afw_adaptor_impl.h"
#include "afw_adaptor_impl_index.h"

#include "afw_lmdb_index.h"
#include "afw_lmdb_internal.h"
#include "generated/afw_lmdb_generated.h"

#define AFW_IMPLEMENTATION_ID "lmdb_index"
#include "afw_adaptor_impl_index_impl_declares.h"



afw_adaptor_impl_index_t * afw_lmdb_adaptor_impl_index_create(
    const afw_lmdb_adaptor_session_t * session,
    const afw_lmdb_adaptor_t         * adaptor,
    MDB_txn                          * txn,
    afw_xctx_t                      * xctx)
{
    afw_lmdb_adaptor_impl_index_t *self;

    /*
     * You may want to create a new pool for instance, but will just use
     * xctx's pool in this example.
     */
    self = afw_xctx_calloc_type(afw_lmdb_adaptor_impl_index_t, xctx);

    self->pub.inf = &impl_afw_adaptor_impl_index_inf;
    self->session = session;
    self->adaptor = adaptor;
    self->txn = txn;
    self->pub.indexDefinitions = impl_afw_adaptor_impl_index_get_index_definitions(
        (afw_adaptor_impl_index_t*)self, xctx);

    /* Return new instance. */
    return (afw_adaptor_impl_index_t*)self;
}


/*
 * const afw_utf8_t * afw_lmdb_index_database()
 *
 * This routine constructs the database name for locating 
 * a particular index.  In general, for LMDB, we use the
 * format:
 * 
 *   Index#object_type_id#property_name
 *
 * For the object_type_id index, we use the special case:
 *
 *   Index#_meta_#objectType
 *
 */
const afw_utf8_t * afw_lmdb_index_database(
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *key,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *database;
    const afw_utf8_t separator = AFW_UTF8_LITERAL("#");

    /* Use the form: Index#object_type_id#key for our index database */
    if (object_type_id) {
        database = afw_utf8_concat(p, xctx, afw_lmdb_s_Index, 
            &separator, object_type_id, &separator, key, NULL);
    } else {
        database = afw_utf8_concat(p, xctx, afw_lmdb_s_Index, 
            &separator, key, NULL);
    }

    return database; 
}


/*
 * Implementation of method release of interface afw_adaptor_impl_index.
 */
void impl_afw_adaptor_impl_index_release(
    const afw_adaptor_impl_index_t * instance,
    afw_xctx_t *xctx)
{

}

/*
 * Implementation of method get_index_definitions of interface afw_adaptor_impl_index.
 */
const afw_object_t *
impl_afw_adaptor_impl_index_get_index_definitions (
    const afw_adaptor_impl_index_t * instance,
    afw_xctx_t *xctx)
{
    afw_lmdb_adaptor_impl_index_t *self = 
        (afw_lmdb_adaptor_impl_index_t *) instance;
    const afw_adaptor_t *adaptor = (afw_adaptor_t *)self->adaptor;
    const afw_object_t *indexes;

    /* lock the adaptor pool to fetch the indexes */
    AFW_ADAPTOR_IMPL_LOCK_READ_BEGIN(adaptor) {
        indexes = afw_object_old_get_property_as_object(
            self->adaptor->internalConfig, afw_lmdb_s_indexDefinitions, xctx);
        if (indexes) {
            indexes = afw_object_create_clone(indexes,
                xctx->p, xctx);
        } else {
            /* if we don't have one, just create one in our own pool */
            indexes = afw_object_create_managed(xctx->p, xctx);
        }
    }
    /* unlock, we have our own copy now */
    AFW_ADAPTOR_IMPL_LOCK_READ_END;

    return indexes;
}

/*
 * Implementation of method update_index_definitions of interface afw_adaptor_impl_index.
 */
void
impl_afw_adaptor_impl_index_update_index_definitions (
    const afw_adaptor_impl_index_t * instance,
    const afw_object_t * indexDefinitions,
    afw_xctx_t *xctx)
{
    afw_lmdb_adaptor_impl_index_t *self = 
        (afw_lmdb_adaptor_impl_index_t *) instance;
    const afw_lmdb_adaptor_session_t *session = self->session;
    const afw_adaptor_t *adaptor = (afw_adaptor_t *)self->session->adaptor;    
    const afw_pool_t *pool;
    MDB_txn *txn = self->txn;

    /* clone this object with our adaptor's memory pool */
    pool = adaptor->p;

    /* lock the adaptor */
    AFW_ADAPTOR_IMPL_LOCK_WRITE_BEGIN(adaptor) {
        afw_object_set_property_as_object(
            session->adaptor->internalConfig, afw_lmdb_s_indexDefinitions, 
            afw_object_create_clone(
                indexDefinitions, pool, xctx),
            xctx);

        /* now write out our new config */
        afw_lmdb_internal_save_config(session->adaptor, 
            session->adaptor->internalConfig, txn, xctx);
    }
   
    AFW_ADAPTOR_IMPL_LOCK_WRITE_END;
}

/*
 * Implementation of method create of interface afw_adaptor_impl_index.
 */
void
impl_afw_adaptor_impl_index_open(
    const afw_adaptor_impl_index_t * instance,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * key,
    afw_boolean_t integer,
    afw_boolean_t unique,
    afw_boolean_t reverse,
    const afw_pool_t * pool,
    afw_xctx_t *xctx)
{
    afw_lmdb_adaptor_impl_index_t *self = 
        (afw_lmdb_adaptor_impl_index_t *) instance;
    const afw_lmdb_adaptor_t *adaptor = self->adaptor;   
    const afw_lmdb_adaptor_session_t * session = NULL;     
    const afw_utf8_t *database;
    unsigned int flags;

    database = afw_lmdb_index_database(object_type_id, key, pool, xctx);

    /* we want to create the database, if it doesn't exist */
    flags = MDB_CREATE;

    /* now, select individual properties for our index */
    if (!unique) flags |= MDB_DUPSORT;

    if (reverse) flags |= MDB_REVERSEKEY | MDB_REVERSEDUP;
  
    if (integer) flags |= MDB_INTEGERKEY;

    /* open up our exclusive transaction, if we haven't already */
    if (self->txn == NULL) {
        AFW_LMDB_BEGIN_TRANSACTION(adaptor, session, 0, true, xctx) {        

            /* now open up the new index database */
            afw_lmdb_internal_open_database(adaptor, 
                AFW_LMDB_GET_TRANSACTION(), 
                database, flags, pool, xctx);

            /* and commit our change */
            AFW_LMDB_COMMIT_TRANSACTION();
        }        
        AFW_LMDB_END_TRANSACTION();
    } else {
        afw_lmdb_internal_open_database(adaptor, 
            self->txn, database, flags, pool, xctx);
    }
}


/*
 * Implementation of method add of interface afw_adaptor_impl_index.
 */
afw_rc_t impl_afw_adaptor_impl_index_add(
    const afw_adaptor_impl_index_t * instance,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    const afw_utf8_t * indexKey,
    const afw_utf8_t * value,
    afw_boolean_t      unique,
    const afw_pool_t * pool,
    afw_xctx_t *xctx)
{
    afw_lmdb_adaptor_impl_index_t *self = 
        (afw_lmdb_adaptor_impl_index_t *) instance;
    const afw_lmdb_adaptor_session_t *session = self->session;
    afw_rc_t rc;
    MDB_txn * txn;
    MDB_dbi dbi;
    MDB_val key, data;
    const afw_uuid_t *uuid;
    const afw_utf8_t *database;
    afw_memory_t raw;

    /* we will get an error if we try to add a key of length 0 */
    /** @fixme this basically avoids indexing an empty string, so
    we should determine if this is an error condition or not 
     */
    if (value->len == 0) {
        return 0;
    }

    database = afw_lmdb_index_database(
        object_type_id, indexKey, pool, xctx);

    /* use the transaction on our session interface */
    txn = self->txn;

    dbi = afw_lmdb_internal_open_database(session->adaptor, 
        txn, database, MDB_DUPSORT|MDB_CREATE, pool, xctx);

    uuid = afw_uuid_from_utf8(object_id, pool, xctx);

    afw_lmdb_internal_set_key(&raw, 
        object_type_id, uuid, pool, xctx);
    
    key.mv_data = (void*)value->s;
    key.mv_size = value->len;

    data.mv_data = (void*)raw.ptr;
    data.mv_size = raw.size;

    if (unique) {
        rc = mdb_put(txn, dbi, &key, &data, MDB_NOOVERWRITE);
        if (rc != 0) {
            AFW_THROW_ERROR_RV_Z(general, lmdb, rc, 
                "Unable to add unique index value.", xctx);
        }
    } else {
        rc = mdb_put(txn, dbi, &key, &data, MDB_NODUPDATA);
        if (rc != 0 && rc != MDB_KEYEXIST) {
            AFW_THROW_ERROR_RV_Z(general, lmdb, rc, 
                "Unable to add index value.", xctx);
        }
    }
 
    return rc;
}

/*
 * Implementation of method delete of interface afw_adaptor_impl_index.
 */
afw_rc_t impl_afw_adaptor_impl_index_delete(
    const afw_adaptor_impl_index_t * instance,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_utf8_t *indexKey,
    const afw_utf8_t *value,
    const afw_pool_t *pool,
    afw_xctx_t *xctx)
{
    afw_lmdb_adaptor_impl_index_t *self = 
        (afw_lmdb_adaptor_impl_index_t *) instance;
    const afw_lmdb_adaptor_session_t *session = self->session;
    MDB_val key, data;
    MDB_dbi dbi;
    MDB_txn *txn = self->txn;
    const afw_utf8_t *database;
    const afw_uuid_t *uuid;
    afw_rc_t rc;
    afw_memory_t raw;

    /* we will get an error if we try to delete a key of length 0 */
    /** @fixme this basically avoids indexing an empty string, so
    we should determine if this is an error condition or not 
     */
    if (value->len == 0) {
        return 0;
    }

    database = afw_lmdb_index_database(
        object_type_id, indexKey, pool, xctx);
   
    dbi = afw_lmdb_internal_open_database(session->adaptor, 
        txn, database, MDB_DUPSORT, pool, xctx);

    uuid = afw_uuid_from_utf8(object_id, pool, xctx);

    afw_lmdb_internal_set_key(&raw, object_type_id, uuid, pool, xctx);

    memset(&key, 0, sizeof(MDB_val));
    key.mv_data = (void *)value->s;
    key.mv_size = value->len;

    memset(&data, 0, sizeof(MDB_val));
    data.mv_data = (void*)raw.ptr;
    data.mv_size = raw.size;

    rc = mdb_del(txn, dbi, &key, &data);
    if (rc != 0 && rc != MDB_NOTFOUND) {
        AFW_THROW_ERROR_RV_Z(general, lmdb, rc, 
            "Unable to delete index value.", xctx);
    }

    return rc;
}

/*
 * Implementation of method replace of interface afw_adaptor_impl_index.
 */
afw_rc_t impl_afw_adaptor_impl_index_replace(
    const afw_adaptor_impl_index_t * instance,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_utf8_t *indexKey,
    const afw_utf8_t *old_value,
    const afw_utf8_t *new_value,
    const afw_pool_t *pool,
    afw_xctx_t *xctx)
{
    afw_lmdb_adaptor_impl_index_t *self = 
        (afw_lmdb_adaptor_impl_index_t *) instance;
    const afw_lmdb_adaptor_session_t *session = self->session;
    MDB_val key, data;
    MDB_dbi dbi;
    MDB_txn *txn = self->txn;
    const afw_utf8_t *database;
    const afw_uuid_t *uuid;
    afw_memory_t raw;
    afw_rc_t rc;

    database = afw_lmdb_index_database(
        object_type_id, indexKey, pool, xctx);
    
    dbi = afw_lmdb_internal_open_database(session->adaptor, 
        txn, database, MDB_DUPSORT, pool, xctx);

    memset(&key, 0, sizeof(MDB_val));
    key.mv_data = (void *)old_value->s;
    key.mv_size = old_value->len;

    memset(&data, 0, sizeof(MDB_val));
    uuid = afw_uuid_from_utf8(object_id, pool, xctx);

    afw_lmdb_internal_set_key(&raw, object_type_id, uuid, pool, xctx);

    data.mv_data = (void*)raw.ptr;
    data.mv_size = raw.size;

    /* remove the old value */
    rc = mdb_del(txn, dbi, &key, &data);
    if (rc != 0) {
        AFW_THROW_ERROR_RV_Z(general, lmdb, rc, 
            "Unable to remove old index value.", xctx);
    }

    memset(&key, 0, sizeof(MDB_val));
    key.mv_data = (void *)new_value->s;
    key.mv_size = new_value->len;

    memset(&data, 0, sizeof(MDB_val));
    uuid = afw_uuid_from_utf8(object_id, pool, xctx);

    afw_lmdb_internal_set_key(&raw, object_type_id, uuid, pool, xctx);

    data.mv_data = (void*)raw.ptr;
    data.mv_size = raw.size;

    /* now write the new value */
    rc = mdb_put(txn, dbi, &key, &data, MDB_NODUPDATA);

    return rc;
}

/*
 * Implementation of method drop of interface afw_adaptor_impl_index.
 */
afw_rc_t
impl_afw_adaptor_impl_index_drop (
    const afw_adaptor_impl_index_t * instance,
    const afw_utf8_t  * object_type_id,
    const afw_utf8_t  * key,
    const afw_pool_t  * pool,
    afw_xctx_t       * xctx)
{
    afw_lmdb_adaptor_impl_index_t *self = 
        (afw_lmdb_adaptor_impl_index_t *) instance;
    const afw_lmdb_adaptor_session_t *session = self->session;
    const afw_utf8_t *database;
    MDB_dbi dbi;
    MDB_txn *txn = self->txn;
    afw_rc_t rc;

    database = afw_lmdb_index_database(
        object_type_id, key, pool, xctx);

    dbi = afw_lmdb_internal_open_database(session->adaptor, 
        txn, database, MDB_DUPSORT|MDB_CREATE, pool, xctx);

    /* (1) means delete it from the environment and close the DB handle */
    rc = mdb_drop(txn, dbi, 1);

    return rc;
}

/*
 * Implementation of method open_cursor of interface afw_adaptor_impl_index.
 */
afw_adaptor_impl_index_cursor_t *
impl_afw_adaptor_impl_index_open_cursor (
    const afw_adaptor_impl_index_t * instance,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * key,
    int                operator,
    const afw_utf8_t * value,
    afw_boolean_t      unique,
    const afw_pool_t * pool,
    afw_xctx_t      * xctx)
{
    afw_lmdb_adaptor_impl_index_t *self = 
        (afw_lmdb_adaptor_impl_index_t *) instance;
    afw_adaptor_impl_index_cursor_t *cursor;
    const afw_utf8_t * database;

    database = afw_lmdb_index_database(
        object_type_id, key, pool, xctx);

    cursor = afw_lmdb_internal_cursor_create(self->session, 
        database, object_type_id, value, operator, unique, xctx);

    return cursor;
}

/*
 * Implementation of method get_session of interface afw_adaptor_impl_index.
 */
const afw_adaptor_session_t *
impl_afw_adaptor_impl_index_get_session (
    const afw_adaptor_impl_index_t * instance,
    afw_xctx_t *xctx)
{
    /* Assign instance pointer to self. */
    afw_lmdb_adaptor_impl_index_t * self =
        (afw_lmdb_adaptor_impl_index_t*)instance;

    return (const afw_adaptor_session_t *) self->session;
}
