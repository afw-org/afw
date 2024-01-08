// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework LMDB Adaptor Internal
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#include "afw.h"
#include "afw_uuid.h"
#include "generated/afw_lmdb_generated.h"
#include "afw_lmdb_internal.h"

#define AFW_IMPLEMENTATION_ID "lmdb"
#include "afw_adaptor_impl_index_cursor_impl_declares.h"
#include "afw_adaptor_key_value_impl_declares.h"
#include "afw_adaptor_transaction_impl_declares.h"

/*
 * Functions for maintaining a LMDB Database handle.  
 * Both the environment and dbi pointers are required
 * to close it and release its resources, so we
 * group them together.  These routines allow us to 
 * register the handle on the xctx for release.
 */
afw_lmdb_dbi_t * afw_lmdb_internal_dbi_handle(
    MDB_env           * env, 
    MDB_dbi             dbi, 
    const afw_pool_t  * pool,
    afw_xctx_t       * xctx)
{
    afw_lmdb_dbi_t *dbi_p;

    dbi_p = afw_pool_calloc_type(pool, afw_lmdb_dbi_t, xctx);
    dbi_p->env = env;
    dbi_p->dbi = dbi;

    return dbi_p;
}

afw_rc_t afw_lmdb_internal_close_database(void *val)
{
    afw_lmdb_dbi_t *dbi_p = (afw_lmdb_dbi_t *)val;

    mdb_dbi_close(dbi_p->env, dbi_p->dbi);

    return 0;
}

/*
 * MDB_dbi afw_lmdb_internal_open_database()
 *
 * This routine opens a database by first trying the pre-loaded
 * handles that were created at adaptor create.  If it's not found,
 * then it creates one, on-the-fly, and registers a cleanup to close
 * it.
 *
 * Note:  only one transaction at a time may open a new database.  
 * Therefore, the AFW_LMDB_BEGIN_TRANSACTION() macro requires an exclusive
 * writer lock, ahead of time, to achieve this.
 */
MDB_dbi afw_lmdb_internal_open_database(
    const afw_lmdb_adaptor_t * adaptor,
    MDB_txn                  * txn,
    const afw_utf8_t         * database,
    unsigned int               flags,
    const afw_pool_t         * p,
    afw_xctx_t              * xctx)
{
    const afw_pool_t *adaptor_p;
    MDB_dbi dbi = 0;
    afw_lmdb_dbi_t *dbi_p;
    int rc;

    adaptor_p = ((afw_adaptor_t *)adaptor)->p;

    /* first check our adaptor's dbi_handles */
    dbi_p = apr_hash_get(adaptor->dbi_handles, database->s, database->len);

    /* if we got a database handle, use it */
    if (dbi_p) {
        return dbi_p->dbi;
    }

    /* if it's not found from our pre-loaded databases, then try to open it */
    rc = mdb_dbi_open(txn, afw_utf8_to_utf8_z(database, p, xctx), flags, &dbi);
    if (rc == 0) {
        dbi_p = afw_lmdb_internal_dbi_handle(
            adaptor->dbEnv, dbi, adaptor_p, xctx);

        /* add it to our handle list, so we can access it later */
        apr_hash_set(adaptor->dbi_handles, database->s,
            database->len, dbi_p);
    } else if (rc == MDB_NOTFOUND) {
        AFW_THROW_ERROR_RV_FZ(not_found, lmdb, rc, xctx,
            "Unable to open database: " AFW_UTF8_FMT_Q ".", 
            AFW_UTF8_FMT_ARG(database));
    } else {
        AFW_THROW_ERROR_RV_FZ(general, lmdb, rc, xctx,
            "Unable to open database: " AFW_UTF8_FMT_Q ".", 
            AFW_UTF8_FMT_ARG(database));
    }

    return dbi;
}

/*
 * Functions for opening and automatically releasing
 * LMDB cursors by an xctx cleanup registration.
 */
afw_rc_t afw_lmdb_internal_close_cursor(void *cursor)
{
    mdb_cursor_close((MDB_cursor*)cursor);

    return 0;
}

MDB_cursor * afw_lmdb_internal_open_cursor(
    const afw_lmdb_adaptor_session_t *session,
    MDB_dbi dbi,
    afw_xctx_t *xctx)
{
    int rc;
    MDB_cursor *cursor = NULL;    
    MDB_txn *txn = session->currTxn;

    rc = mdb_cursor_open(txn, dbi, &cursor);
    if (rc == MDB_NOTFOUND) {
        AFW_THROW_ERROR_RV_Z(not_found, lmdb, rc,
            "Cursor not found.", xctx);
    } else if (rc) {
        AFW_THROW_ERROR_RV_Z(general, lmdb, rc,
            "Unable to open cursor.", xctx);
    }

    return cursor;
}

/*
 * Sets the raw key in the format of:
 *
 * {object_type_id}{uuid}
 */
void afw_lmdb_internal_set_key(
    afw_memory_t * key, 
    const afw_utf8_t * object_type_id,
    const afw_uuid_t * uuid,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    key->ptr = afw_pool_malloc(p, object_type_id->len + sizeof(afw_uuid_t), xctx);
    if (object_type_id->len)
        memcpy((char*)(key->ptr), object_type_id->s, object_type_id->len);
    memcpy((char*)(key->ptr) + object_type_id->len, uuid, sizeof(afw_uuid_t));
    key->size = object_type_id->len + sizeof(afw_uuid_t);
}

/*
 * Parses the raw key in the format of:
 *
 * {ObjectType}{uuid}
 *
 * And returns the object_type_id and uuid.
 */
void afw_lmdb_internal_get_key(
    afw_memory_t  * key,
    afw_utf8_t * object_type_id,
    afw_uuid_t * uuid)
{
    if (key->size > sizeof(afw_uuid_t)) {
        object_type_id->s = (const afw_utf8_octet_t *)key->ptr;
        object_type_id->len = key->size - sizeof(afw_uuid_t);
        memcpy(uuid, key->ptr + object_type_id->len, sizeof(afw_uuid_t));
    } else {
        object_type_id->len = 0;
        memcpy(uuid, key->ptr, sizeof(afw_uuid_t));
    }
}

/*
 * afw_rc_t afw_lmdb_internal_create_entry()
 *
 * The foundational routine for storing a key/value entry
 * into LMDB.  All other methods/interfaces should handle 
 * which database and transaction to use, along with how 
 * to serialize the data into a raw key and value.
 *
 * Unlike replace, this routine will not overwrite an existing
 * key/value.
 */
afw_rc_t afw_lmdb_internal_create_entry(
    MDB_txn *txn,
    MDB_dbi dbi,
    const afw_memory_t * raw_key,
    const afw_memory_t * raw_value,
    afw_xctx_t *xctx)
{
    MDB_val key, value;
    int rc;

    key.mv_data = (void*)raw_key->ptr;
    key.mv_size = raw_key->size;

    value.mv_data = (void*)raw_value->ptr;
    value.mv_size = raw_value->size;

    rc = mdb_put(txn, dbi, &key, &value, MDB_NOOVERWRITE);

    return rc;
}

/*
 * afw_rc_t afw_lmdb_internal_replace_entry()
 *
 * The foundational routine for replacing a key/value entry
 * into LMDB.  All other methods/interfaces should handle
 * which database and transaction to use, along with how
 * to serialize the data into a raw key and value.
 */
afw_rc_t afw_lmdb_internal_replace_entry(
    MDB_txn *txn,
    MDB_dbi dbi,
    const afw_memory_t * raw_key,
    const afw_memory_t * raw_value,
    afw_xctx_t *xctx)
{
    MDB_val key, value;
    int rc;

    key.mv_data = (void*)raw_key->ptr;
    key.mv_size = raw_key->size;

    value.mv_data = (void*)raw_value->ptr;
    value.mv_size = raw_value->size;

    rc = mdb_put(txn, dbi, &key, &value, 0);

    return rc;
}

/*
 * afw_rc_t afw_lmdb_internal_delete_entry()
 *
 * The foundational routine for deleting a key/value entry
 * into LMDB.  All other methods/interfaces should handle
 * which database and transaction to use, along with how
 * to serialize the data into a raw key and value.
 */
afw_rc_t afw_lmdb_internal_delete_entry(
    MDB_txn *txn,
    MDB_dbi dbi,
    const afw_memory_t * raw_key,
    const afw_memory_t * raw_value,
    afw_xctx_t *xctx)
{
    MDB_val key, value;
    int rc;

    key.mv_data = (void*)raw_key->ptr;
    key.mv_size = raw_key->size;

    if (raw_value) {
        value.mv_data = (void*)raw_value->ptr;
        value.mv_size = raw_value->size;
    } else {
        value.mv_data = NULL;
        value.mv_size = 0;
    }

    rc = mdb_del(txn, dbi, &key, &value);

    return rc;
}

/*
 * afw_rc_t afw_lmdb_internal_get_entry()
 *
 * The foundational routine for getting a key/value entry
 * from LMDB.  All other methods/interfaces should handle
 * which database and transaction to use, along with how
 * to serialize the data into a raw key.
 */
afw_rc_t afw_lmdb_internal_get_entry(
    MDB_txn *txn,
    MDB_dbi dbi,
    const afw_memory_t * raw_key,
    afw_memory_t * raw_value,
    afw_xctx_t *xctx)
{
    MDB_val key, value;
    int rc;

    key.mv_data = (void*)raw_key->ptr;
    key.mv_size = raw_key->size;

    memset(&value, 0, sizeof(MDB_val));

    rc = mdb_get(txn, dbi, &key, &value);
    if (rc == 0) {
        raw_value->ptr = value.mv_data;
        raw_value->size = value.mv_size;
    }

    return rc;
}


/*
 * void afw_lmdb_internal_create_entry_from_object()
 *
 * This routine takes an Adaptive Object and adds it to
 * the database specified with dbi.
 *
 * The object_id becomes the key, stored as a binary UUID.
 * The object is converted into a string format, specified
 * by the configured adaptor content-type.
 *
 */
void afw_lmdb_internal_create_entry_from_object(
    const afw_lmdb_adaptor_session_t *self,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_object_t *object,
    MDB_dbi dbi,
    afw_xctx_t *xctx)
{
    afw_memory_t key, value;
    const afw_uuid_t *uuid;
    const afw_memory_t *object_string;
    int rc;
    MDB_txn *txn = self->currTxn;

    uuid = afw_uuid_from_utf8(object_id, xctx->p, xctx);
    if (uuid == NULL)
        AFW_THROW_ERROR_Z(bad_request,
            "Invalid object_id format (UUID required).", xctx);

    object_string = afw_content_type_object_to_raw(
        self->adaptor->ubjson, object, &afw_object_options_essential,
        xctx->p, xctx);

    afw_lmdb_internal_set_key(&key, 
        object_type_id, uuid, xctx->p, xctx);

    value.ptr = object_string->ptr;
    value.size = object_string->size;

    rc = afw_lmdb_internal_create_entry(
        txn, dbi, &key, &value, xctx);
    if (rc)
        AFW_THROW_ERROR_RV_Z(general, lmdb, rc,
            "Error writing object to database.", xctx);
}

/*
 * void afw_lmdb_internal_replace_entry_from_object()
 *
 * This routine takes an Adaptive Object and replaces it in 
 * the database specified with dbi.
 *
 * The object_id becomes the key, stored as a binary UUID.
 * The object is converted into a string format, specified
 * by the configured adaptor content-type.
 *
 * Note:  if one does not already exist, this will create a 
 * new entry.
 */
void afw_lmdb_internal_replace_entry_from_object(
    const afw_lmdb_adaptor_session_t *self,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_object_t *object,
    MDB_dbi dbi,
    afw_xctx_t *xctx)
{
    afw_memory_t key, value;
    const afw_uuid_t *uuid;
    const afw_memory_t *object_string;
    MDB_txn *txn = self->currTxn;
    int rc;

    uuid = afw_uuid_from_utf8(object_id, xctx->p, xctx);
    if (uuid == NULL)
        AFW_THROW_ERROR_Z(bad_request,
            "Invalid object_id format (UUID required).", xctx);

    object_string = afw_content_type_object_to_raw(
        self->adaptor->ubjson, object, &afw_object_options_essential,
        xctx->p, xctx);

    afw_lmdb_internal_set_key(&key, 
        object_type_id, uuid, xctx->p, xctx);

    value.ptr = object_string->ptr;
    value.size = object_string->size;

    rc = afw_lmdb_internal_replace_entry(
        txn, dbi, &key, &value, xctx);
    if (rc)
        AFW_THROW_ERROR_RV_Z(general, lmdb, rc,
            "Error writing object to database.", xctx);
}

/*
 * const afw_object_t * afw_lmdb_internal_create_object_from_entry()
 *
 * This routine looks fetches the object from the specified
 * database and returns it as an Adaptive object.
 *
 */
const afw_object_t * afw_lmdb_internal_create_object_from_entry(
    const afw_lmdb_adaptor_session_t *self,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    MDB_dbi dbi,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;

    value = afw_lmdb_internal_create_value_from_entry(self, 
        object_type_id, object_id, dbi, xctx);

    return afw_value_as_object(value, xctx);
}

/*
 * const afw_value_t * afw_lmdb_internal_create_value_from_entry()
 *
 * This routine looks fetches the object from the specified
 * database and returns it as an Adaptive Value.
 *
 */
const afw_value_t * afw_lmdb_internal_create_value_from_entry(
    const afw_lmdb_adaptor_session_t *self,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    MDB_dbi dbi,
    afw_xctx_t *xctx)
{
    afw_lmdb_adaptor_t *adaptor = (afw_lmdb_adaptor_t *)self->adaptor;
    const afw_object_t *object;
    const afw_value_t *v;
    afw_memory_t raw;
    afw_memory_t key;
    afw_memory_t value;
    const afw_uuid_t *uuid;
    MDB_txn *txn = self->currTxn;

    uuid = afw_uuid_from_utf8(object_id, xctx->p, xctx);

    afw_lmdb_internal_set_key(&key, 
        object_type_id, uuid, xctx->p, xctx);
    if (afw_lmdb_internal_get_entry(
            txn, dbi, &key, &value, xctx) != 0) 
    {
        /* no entry for this key */
        AFW_THROW_ERROR_Z(general,
            "Unable to locate entry in database.", xctx);
    }

    /* convert the data into a value */
    raw.size = value.size;
    raw.ptr = value.ptr;

    v = afw_content_type_raw_to_value(
        adaptor->ubjson, &raw, NULL, xctx->p, xctx);

    object = afw_value_as_object(v, xctx);

    afw_object_meta_set_ids(object, &adaptor->pub.adaptor_id, 
        object_type_id, object_id, xctx);

    return v;
}

/*
 * void afw_lmdb_internal_save_config()
 *
 * This routine writes the config object out to
 * the internal configuration location.  We let
 * the LMDB transaction mechanisms provide our
 * locking for us.
 *
 */
void afw_lmdb_internal_save_config(
    afw_lmdb_adaptor_t *self,
    const afw_object_t *config,
    MDB_txn *txn,
    afw_xctx_t *xctx)
{
    MDB_dbi dbi;
    MDB_val key, data;
    afw_uuid_t uuid;
    const afw_memory_t *raw;
    const afw_value_t *now;
    int rc;

    /* the key for configuration is found at UUID(0) */
    memset(&uuid, 0, sizeof(afw_uuid_t));

    dbi = afw_lmdb_internal_open_database(self, 
        txn, afw_lmdb_s_Primary, MDB_CREATE, xctx->p, xctx);

    /* set our last updateTimeStamp property */
    now = afw_value_create_dateTime_now_utc(config->p, xctx);
    afw_object_set_property(config, afw_lmdb_s_updateTimeStamp, now, xctx);

    /* write out our object to raw string */
    raw = afw_content_type_object_to_raw(self->ubjson, config,
        &afw_object_options_essential,
        config->p, xctx);

    key.mv_data = (void *)&uuid;
    key.mv_size = sizeof(afw_uuid_t);
    data.mv_data = (void *)raw->ptr;
    data.mv_size = raw->size;

    rc = mdb_put(txn, dbi, &key, &data, 0);
    if (rc) {
        AFW_THROW_ERROR_RV_Z(general, lmdb, rc,
            "Unable to write internal configuration.", xctx);
    }
}

/*
 * const afw_object_t * afw_lmdb_internal_get_config()
 *
 * This routine reads the internal configuration object.
 * The config object is stored at object_id (0) in our
 * primary database.
 *
 */
const afw_object_t * afw_lmdb_internal_get_config(
    afw_lmdb_adaptor_t *self,
    MDB_txn *txn,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *config = NULL;
    const afw_utf8_t *object_type_id, *object_id;
    const afw_value_t *value;
    const afw_value_t *now;
    afw_uuid_t uuid;
    MDB_dbi dbi;
    MDB_val key, data;
    afw_memory_t raw;
    const afw_memory_t *raw_out;
    int rc;

    /* the key for configuration is found at UUID(0) */
    memset(&uuid, 0, sizeof(afw_uuid_t));
    object_id = afw_uuid_to_utf8(&uuid, p, xctx);

    /* set our object type to an internal type */
    /* (FIXME: inherit from config object types?) */
    object_type_id = afw_lmdb_s_internalConfig;

    dbi = afw_lmdb_internal_open_database(self, txn, 
        afw_lmdb_s_Primary, MDB_CREATE, p, xctx);

    key.mv_data = (void *)&uuid;
    key.mv_size = sizeof(afw_uuid_t);
    memset(&data, 0, sizeof(MDB_val));

    rc = mdb_get(txn, dbi, &key, &data);
    if (rc == MDB_NOTFOUND) {
        /* first time running?  create a new internal config */
        config = afw_object_create_managed(p, xctx);

        /* get the current time for timestamps */
        now = afw_value_create_dateTime_now_utc(p, xctx);

        afw_object_set_property(config, afw_lmdb_s_createTimeStamp, now, xctx);
        afw_object_set_property(config, afw_lmdb_s_updateTimeStamp, now, xctx);

        /* go ahead and write it back out */
        raw_out = afw_content_type_object_to_raw(
            self->ubjson, config, &afw_object_options_essential,
            p, xctx);

        key.mv_data = (void *)&uuid;
        key.mv_size = sizeof(afw_uuid_t);
        data.mv_data = (void *)raw_out->ptr;
        data.mv_size = raw_out->size;

        rc = mdb_put(txn, dbi, &key, &data, 0);
        if (rc) {
            AFW_THROW_ERROR_RV_Z(general, lmdb, rc,
                "Unable to write internal configuration.", xctx);
        }
    } else if (rc == 0) {
        /* found it, now parse and load it */
        raw.ptr = data.mv_data;
        raw.size = data.mv_size;

        value = afw_content_type_raw_to_value(
            self->ubjson, &raw, NULL, p, xctx);

        config = afw_value_as_object(value, xctx);
        afw_object_meta_set_ids(config, &self->pub.adaptor_id, object_type_id,
            object_id, xctx);
    } else {
        /* some other problem! */
        AFW_THROW_ERROR_RV_Z(general, lmdb, rc,
            "Unable to find internal configuration in the primary database.", xctx);
    }

    return config;
}

/*
 * Implementation of Key Value interface
 *
 * The following routines implement the interface for
 * managing key/values in LMDB.
 */
afw_lmdb_key_value_t * afw_lmdb_key_value_create(
    afw_lmdb_adaptor_session_t *session,
    afw_xctx_t *xctx)
{
    afw_lmdb_key_value_t *self;

    self = afw_xctx_calloc_type(afw_lmdb_key_value_t, xctx);
    self->pub.inf = &impl_afw_adaptor_key_value_inf;

    self->session = (afw_adaptor_session_t *)session;

    return self;
}

/*
 * Implementation of method add of interface afw_adaptor_key_value.
 */
void
impl_afw_adaptor_key_value_add (
    const afw_adaptor_key_value_t * instance,
    const afw_utf8_t * namespace,
    const afw_memory_t * key,
    const afw_memory_t * value,
    afw_xctx_t *xctx)
{
    /* Assign instance pointer to self. */
    afw_lmdb_key_value_t * self = 
        (afw_lmdb_key_value_t *)instance;
    afw_lmdb_adaptor_session_t * session =
        (afw_lmdb_adaptor_session_t *)self->session;
    const afw_utf8_t separator = AFW_UTF8_LITERAL("#");
    const afw_utf8_t *database;
    MDB_dbi dbi;
    MDB_txn *txn;
    afw_rc_t rc;
    
    AFW_LMDB_BEGIN_TRANSACTION(session->adaptor, session, 0, false, xctx) {

        txn = AFW_LMDB_GET_TRANSACTION();

        database = afw_utf8_concat(xctx->p, xctx, afw_lmdb_s_Extern, 
            &separator, namespace, NULL);

        /** @fixme determine if we need to keep these databases open or not */
        dbi = afw_lmdb_internal_open_database(session->adaptor, 
            txn, database, MDB_CREATE, xctx->p, xctx);

        rc = afw_lmdb_internal_create_entry(
            txn, dbi, key, value, xctx);
        if (rc) {
            AFW_THROW_ERROR_RV_Z(general, lmdb_internal, rc,
                "Unable to write key/value to database.", xctx);
        }

        AFW_LMDB_COMMIT_TRANSACTION();
    }
    AFW_LMDB_END_TRANSACTION();
}

/*
 * Implementation of method delete of interface afw_adaptor_key_value.
 */
void
impl_afw_adaptor_key_value_delete (
    const afw_adaptor_key_value_t * instance,
    const afw_utf8_t * namespace,
    const afw_memory_t * key,
    const afw_memory_t * value,
    afw_boolean_t must_exist,
    afw_xctx_t *xctx)
{
    /* Assign instance pointer to self. */
    afw_lmdb_key_value_t * self =
        (afw_lmdb_key_value_t *)instance;
    afw_lmdb_adaptor_session_t * session =
        (afw_lmdb_adaptor_session_t *)self->session;
    const afw_utf8_t separator = AFW_UTF8_LITERAL("#");
    const afw_utf8_t *database;
    MDB_dbi dbi;
    MDB_txn *txn;
    afw_rc_t rc;
    
    AFW_LMDB_BEGIN_TRANSACTION(session->adaptor, session, 0, false, xctx) {

        txn = AFW_LMDB_GET_TRANSACTION();

        database = afw_utf8_concat(xctx->p, xctx, afw_lmdb_s_Extern,
            &separator, namespace, NULL);

        dbi = afw_lmdb_internal_open_database(session->adaptor, 
            txn, database, 0, xctx->p, xctx);

        rc = afw_lmdb_internal_delete_entry(
            txn, dbi, key, value, xctx);
        if (rc == MDB_NOTFOUND) {
            if (must_exist)
                AFW_THROW_ERROR_RV_Z(general, lmdb_internal, dbi,
                    "Unable to delete key/value.", xctx);
            else 
                return;
        } else if (rc) {
            AFW_THROW_ERROR_RV_Z(general, lmdb_internal, rc,
                "Unable to delete key/value to database.", xctx);
        }

        AFW_LMDB_COMMIT_TRANSACTION();
    }
    AFW_LMDB_END_TRANSACTION();
}

/*
 * Implementation of method replace of interface afw_adaptor_key_value.
 */
void
impl_afw_adaptor_key_value_replace (
    const afw_adaptor_key_value_t * instance,
    const afw_utf8_t * namespace,
    const afw_memory_t * key,
    const afw_memory_t * value,
    afw_boolean_t must_exist,
    afw_xctx_t *xctx)
{
    /* Assign instance pointer to self. */
    afw_lmdb_key_value_t * self =
        (afw_lmdb_key_value_t *)instance;
    afw_lmdb_adaptor_session_t * session =
        (afw_lmdb_adaptor_session_t *)self->session;
    const afw_utf8_t separator = AFW_UTF8_LITERAL("#");
    const afw_utf8_t *database;
    MDB_dbi dbi;
    MDB_txn *txn;
    afw_rc_t rc;
    afw_memory_t existing;

    AFW_LMDB_BEGIN_TRANSACTION(session->adaptor, session, 0, false, xctx) {

        txn = AFW_LMDB_GET_TRANSACTION();

        database = afw_utf8_concat(xctx->p, xctx, afw_lmdb_s_Extern,
            &separator, namespace, NULL);

        if (must_exist)
            dbi = afw_lmdb_internal_open_database(session->adaptor, 
                txn, database, 0, xctx->p, xctx);
        else
            dbi = afw_lmdb_internal_open_database(session->adaptor, 
                txn, database, MDB_CREATE, xctx->p, xctx);

        if (must_exist) {
            rc = afw_lmdb_internal_get_entry(
                txn, dbi, key, &existing, xctx);
            if (rc != 0 ||
                (value->size != existing.size) ||
                (memcmp(value->ptr, existing.ptr, value->size) != 0)) 
            {
                AFW_THROW_ERROR_RV_Z(general, lmdb_internal, dbi,
                    "Existing key/value does not exist for replacement.", xctx);
            }
        }

        rc = afw_lmdb_internal_replace_entry(
            txn, dbi, key, value, xctx);
        if (rc == MDB_NOTFOUND) {
            AFW_THROW_ERROR_RV_Z(not_found, lmdb, rc,
                "Entry not found.", xctx);
        } else if (rc) {
            AFW_THROW_ERROR_RV_Z(general, lmdb_internal, rc,
                "Unable to write key/value to database.", xctx);
        }

        AFW_LMDB_COMMIT_TRANSACTION();
    }
    AFW_LMDB_END_TRANSACTION();
}


/*
 * Implementation of method get of interface afw_adaptor_key_value.
 */
const afw_memory_t *
impl_afw_adaptor_key_value_get (
    const afw_adaptor_key_value_t * instance,
    const afw_utf8_t * namespace,
    const afw_memory_t * key,
    afw_xctx_t *xctx)
{
    afw_lmdb_key_value_t * self =
        (afw_lmdb_key_value_t *)instance;
    afw_lmdb_adaptor_session_t * session =
        (afw_lmdb_adaptor_session_t *)self->session;
    const afw_utf8_t separator = AFW_UTF8_LITERAL("#");
    const afw_utf8_t *database;
    MDB_dbi dbi;    
    MDB_txn *txn;
    afw_memory_t existing;
    const afw_memory_t *value;

    AFW_LMDB_BEGIN_TRANSACTION(session->adaptor, session, 0, false, xctx) {

        txn = AFW_LMDB_GET_TRANSACTION();

        database = afw_utf8_concat(xctx->p, xctx, afw_lmdb_s_Extern,
            &separator, namespace, NULL);

        dbi = afw_lmdb_internal_open_database(session->adaptor, 
            txn, database, 0, xctx->p, xctx);

        afw_lmdb_internal_get_entry(
            txn, dbi, key, &existing, xctx);

        AFW_LMDB_COMMIT_TRANSACTION();
    }
    AFW_LMDB_END_TRANSACTION();

    /* allocate a new afw_memory_t * to return */
    //value->size = existing.size;
    //value->ptr = afw_xctx_calloc(existing.size, xctx);
    //memcpy(value->ptr, existing.ptr, value->size);
    /** @fixme*/return NULL;
    return value;
}

#define AFW_LMDB_CURSOR_CHECK_RV(rv) \
    if (rv && rv != MDB_NOTFOUND) { \
        AFW_THROW_ERROR_RV_Z(general, lmdb, rv, \
            "Cursor operation failed.", xctx); \
    }

/*
 * Implementation of Index Cursor interface
 *
 * The following routines implement the interface for
 * opening a cursor against an indexed property.
 */
void afw_lmdb_internal_cursor_reset(
    impl_afw_adaptor_impl_index_cursor_self_t * self,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *key_string;
    int rv;

    memset(&self->data, 0, sizeof(MDB_val));
    if (self->key_string) { 
        key_string = afw_utf8_create_copy(
            self->key_string->s, self->key_string->len,
            xctx->p, xctx);

        self->key.mv_data = (void *)key_string->s;
        self->key.mv_size = key_string->len;
    } else {
        self->key.mv_data = NULL;
        self->key.mv_size = 0;
    }

    /* the filter entry operator helps us determine our start position */
    switch (self->operator) {
        case afw_query_criteria_filter_op_id_ne:
            rv = mdb_cursor_get(self->cursor, &self->key, 
                &self->data, MDB_FIRST);
            AFW_LMDB_CURSOR_CHECK_RV(rv);
            break;

        case afw_query_criteria_filter_op_id_eq:
            rv = mdb_cursor_get(self->cursor, &self->key, 
                &self->data, MDB_SET);
            AFW_LMDB_CURSOR_CHECK_RV(rv);
            break;

        case afw_query_criteria_filter_op_id_ge:
            rv = mdb_cursor_get(self->cursor, &self->key, 
                &self->data, MDB_SET_RANGE);
            AFW_LMDB_CURSOR_CHECK_RV(rv);
            break;

        case afw_query_criteria_filter_op_id_le:
            rv = mdb_cursor_get(self->cursor, &self->key, 
                &self->data, MDB_SET_RANGE);
            AFW_LMDB_CURSOR_CHECK_RV(rv);

            /* MDB_SET_RANGE may have gone past our key */
            if (memcmp(self->key.mv_data, self->key_string->s, 
                        self->key_string->len) > 0) {
                /* So set it to our previous data item */
                rv = mdb_cursor_get(self->cursor, &self->key,
                    &self->data, MDB_PREV);
                AFW_LMDB_CURSOR_CHECK_RV(rv);
            }

            break;

        /* For lt, we start right before the key */
        case afw_query_criteria_filter_op_id_lt:
            rv = mdb_cursor_get(self->cursor, &self->key, 
                &self->data, MDB_SET_RANGE);
            AFW_LMDB_CURSOR_CHECK_RV(rv);

            /* MDB_SET_RANGE will always set at or just past
                our key.  So, we go one item backwards */
            rv = mdb_cursor_get(self->cursor, &self->key, 
                &self->data, MDB_PREV);
            AFW_LMDB_CURSOR_CHECK_RV(rv);
            
            break;

        /* For gt, we start right after this key */ 
        case afw_query_criteria_filter_op_id_gt:
            rv = mdb_cursor_get(self->cursor, &self->key,
                &self->data, MDB_SET_RANGE);
            AFW_LMDB_CURSOR_CHECK_RV(rv);
            if (self->key_string) {
                if (memcmp(self->key.mv_data, self->key_string->s, 
                            self->key_string->len) == 0) {
                    /* if we've landed at our key, go past it */
                    rv = mdb_cursor_get(self->cursor, &self->key,
                        &self->data, MDB_NEXT_NODUP);
                    AFW_LMDB_CURSOR_CHECK_RV(rv);
                }
            }

            break;

        default:
            AFW_THROW_ERROR_Z(general,
                "Unable to create cursor for this operator", xctx);
            break;
    }
}

afw_adaptor_impl_index_cursor_t * afw_lmdb_internal_cursor_create(
    const afw_lmdb_adaptor_session_t *session,
    const afw_utf8_t * database,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * value,
    const afw_query_criteria_filter_op_id_t operator,
    afw_boolean_t unique,
    afw_xctx_t *xctx)
{
    impl_afw_adaptor_impl_index_cursor_self_t *self;
    MDB_dbi dbi;
    MDB_txn *txn = session->currTxn;

    self = afw_xctx_calloc_type(impl_afw_adaptor_impl_index_cursor_self_t, xctx);
    self->pub.inf = &impl_afw_adaptor_impl_index_cursor_inf;

    self->session = session;
    self->object_type_id = object_type_id;
    self->unique = unique;
    self->operator = operator;

    dbi = afw_lmdb_internal_open_database(self->session->adaptor, 
        txn, database, 0, xctx->p, xctx);

    self->cursor = afw_lmdb_internal_open_cursor(session, dbi, xctx);
    if (self->cursor == NULL) {
        AFW_THROW_ERROR_Z(general,
            "Error opening index cursor.", xctx);
    }

    if (value) {
        self->key_string = afw_utf8_create_copy(
            value->s, value->len, xctx->p, xctx);
    } else
        self->key_string = NULL;

    afw_lmdb_internal_cursor_reset(self, xctx);

    /* also open the primary database to resolve references */
    self->dbPri = afw_lmdb_internal_open_database(self->session->adaptor,
        txn, afw_lmdb_s_Primary, 0, xctx->p, xctx);

    /* Return new instance. */
    return (afw_adaptor_impl_index_cursor_t *)self;
}

/*
 * Implementation of method release of interface afw_adaptor_impl_index_cursor.
 */
void
impl_afw_adaptor_impl_index_cursor_release (
    const afw_adaptor_impl_index_cursor_t * instance,
    afw_xctx_t *xctx)
{
    /* Assign instance pointer to self. */
    impl_afw_adaptor_impl_index_cursor_self_t * self = 
        (impl_afw_adaptor_impl_index_cursor_self_t *)instance;

    mdb_cursor_close(self->cursor);
}

/*
 * This routine uses the operator to decide how to move the
 * cursor to the next object.
 */
int afw_lmdb_internal_cursor_next(
    const afw_adaptor_impl_index_cursor_t * instance,
    afw_xctx_t *xctx)
{
    impl_afw_adaptor_impl_index_cursor_self_t * self = 
        (impl_afw_adaptor_impl_index_cursor_self_t *)instance;
    int rc = -1;

    /** @fixme check all cases for unique constraint */
    switch (self->operator) {
        case afw_query_criteria_filter_op_id_ne:
            rc = mdb_cursor_get(self->cursor, &self->key,
                &self->data, MDB_NEXT);
            if (self->key_string) {
                if (memcmp(self->key.mv_data, self->key_string->s, 
                            self->key_string->len) == 0) {
                    /* if we've landed at our key, go past it */
                    rc = mdb_cursor_get(self->cursor, &self->key,
                        &self->data, MDB_NEXT_NODUP);
                }
            }
            break;

        case afw_query_criteria_filter_op_id_eq:
            /* If this is a unique constraint index, MDB_NEXT_DUP will
                not do the right thing */
            if (self->unique)
                return -1;

            rc = mdb_cursor_get(self->cursor, &self->key, 
                &self->data, MDB_NEXT_DUP);
            break;

        case afw_query_criteria_filter_op_id_gt:
            /** @fixme check on this */

        case afw_query_criteria_filter_op_id_ge:

            rc = mdb_cursor_get(self->cursor, &self->key,
                &self->data, MDB_NEXT);
            break;

        case afw_query_criteria_filter_op_id_lt:
            /** @fixme check on this */

        case afw_query_criteria_filter_op_id_le:

            rc = mdb_cursor_get(self->cursor, &self->key,
                &self->data, MDB_PREV);
            break;

        default:
            break;
    }

    return rc;
}

/*
 * Implementation of method get_next_object of interface afw_adaptor_impl_index_cursor.
 */
const afw_object_t *
impl_afw_adaptor_impl_index_cursor_get_next_object (
    const afw_adaptor_impl_index_cursor_t * instance,
    const afw_pool_t *pool,
    afw_xctx_t *xctx)
{
    /* Assign instance pointer to self. */
    impl_afw_adaptor_impl_index_cursor_self_t * self = 
        (impl_afw_adaptor_impl_index_cursor_self_t *)instance;
    const afw_lmdb_adaptor_t *adaptor = self->session->adaptor;
    const afw_object_t *object = NULL;
    const afw_value_t *value;
    const afw_utf8_t *object_id;
    afw_utf8_t object_type;
    afw_memory_t from_raw;
    afw_memory_t rawKey;
    afw_uuid_t uuid;
    MDB_val key, data;
    MDB_txn *txn;
    int rc;

    do {
        /* the cursor may be empty or finished */
        if (self->data.mv_data == NULL)
            return NULL;

        memset(&data, 0, sizeof(MDB_val));
        key.mv_data = (void *)self->data.mv_data;
        key.mv_size = self->data.mv_size;
        rawKey.ptr = key.mv_data;
        rawKey.size = key.mv_size;

        txn = self->session->currTxn;

        /* now, fetch the actual object from the primary database */
        rc = mdb_get(txn, self->dbPri, &key, &data);
        if (rc == MDB_NOTFOUND) {
            /* indicates an index was not cleaned up */
            rc = afw_lmdb_internal_cursor_next(instance, xctx);
            if (rc) {
                return NULL;
            }
            continue;
        } else if (rc) {
            AFW_THROW_ERROR_RV_Z(general, lmdb, rc, 
                "Error in mdb_get()!", xctx);
        }

        afw_lmdb_internal_get_key(&rawKey, &object_type, &uuid);
        object_id = afw_uuid_to_utf8(&uuid, pool, xctx);

        from_raw.ptr = data.mv_data;
        from_raw.size = data.mv_size; 

        value = afw_content_type_raw_to_value(
            adaptor->ubjson, &from_raw, NULL, pool, xctx);

        object = afw_value_as_object(value, xctx);

        afw_object_meta_set_ids(object, &adaptor->pub.adaptor_id,
            self->object_type_id, object_id, xctx);

        /* advance the cursor for the next object */
        rc = afw_lmdb_internal_cursor_next(instance, xctx);
        if (rc) {
            /* end of data */
            self->data.mv_data = NULL;
        }

    } while (object == NULL && rc == MDB_NOTFOUND);

    return object;
}


/*
 * Implementation of method contains_object of interface afw_adaptor_impl_index_cursor.
 *
 */
afw_boolean_t
impl_afw_adaptor_impl_index_cursor_contains_object (
    const afw_adaptor_impl_index_cursor_t * instance,
    const afw_object_t * object,
    afw_xctx_t *xctx)
{
    /* Assign instance pointer to self. */
    impl_afw_adaptor_impl_index_cursor_self_t * self = 
        (impl_afw_adaptor_impl_index_cursor_self_t *)instance;
    const afw_utf8_t *object_id, *key_string;
    afw_memory_t index;
    MDB_val key, data;
    afw_boolean_t contains = false;
    const afw_uuid_t *uuid;
    int rc;

    /** @fixme:  This routine only works for afw_query_criteria_filter_op_id_eq. In order 
     *    to support other operators, we need to scan sequentially instead of letting LMDB
     *    try to find a specific key/data.
     */
    if (self->operator != afw_query_criteria_filter_op_id_eq) {
        AFW_THROW_ERROR_Z(general,
            "The contains_object method for LMDB cursors only implements eq", xctx);
    }

    object_id = afw_object_meta_get_object_id(object, xctx);
    uuid = afw_uuid_from_utf8(object_id, xctx->p, xctx);

    afw_lmdb_internal_set_key(&index,
        self->object_type_id, uuid, xctx->p, xctx);

    key_string = afw_utf8_create_copy(
        self->key_string->s, self->key_string->len, xctx->p, xctx);

    key.mv_data = (void *)key_string->s;
    key.mv_size = key_string->len;
    data.mv_data = (void *)index.ptr;
    data.mv_size = index.size;

    rc = mdb_cursor_get(self->cursor, &key, &data, MDB_GET_BOTH);
    if (rc == 0) {
        /* 
            I don't feel like I should have to do this, but MDB_GET_BOTH
            likes to return random records from time to time, which do not
            match the key/data I've supplied.  The documentation on this 
            operand is slim to say the least. Some google searches indicate
            bugginess with this call/operand, even recently.  
         */
        if (memcmp(data.mv_data, index.ptr, index.size) == 0) {
            contains = true;
        }
    }

    /* reset the cursor to point to our previous entry */
    afw_lmdb_internal_cursor_reset(self, xctx);

    return contains;
}



/*
 * Implementation of method inner_join of interface afw_adaptor_impl_index_cursor.
 *
 */
const afw_adaptor_impl_index_cursor_t *
impl_afw_adaptor_impl_index_cursor_inner_join (
    const afw_adaptor_impl_index_cursor_t * instance,
    const afw_adaptor_impl_index_cursor_t * cursor,
    afw_xctx_t *xctx)
{
    /* Assign instance pointer to self. */
    impl_afw_adaptor_impl_index_cursor_self_t * self = 
        (impl_afw_adaptor_impl_index_cursor_self_t *)instance;
    impl_afw_adaptor_impl_index_cursor_self_t * that = 
        (impl_afw_adaptor_impl_index_cursor_self_t *)cursor;
    size_t count_this, count_that;
    int rv;
    
    rv = mdb_cursor_count(self->cursor, &count_this);
    if (rv) {
        AFW_THROW_ERROR_RV_Z(general, lmdb, rv, 
                "Cursor count failed.", xctx);
    }

    rv = mdb_cursor_count(that->cursor, &count_that);
    if (rv) {
        AFW_THROW_ERROR_RV_Z(general, lmdb, rv, 
                "Cursor count failed.", xctx);
    }

    /* use the cursor count and return the smaller set */
    if (count_this < count_that)
        return instance;
    else
        return cursor;
}

/*
 * Implementation of method get_count of interface afw_adaptor_impl_index_cursor.
 */
afw_boolean_t
impl_afw_adaptor_impl_index_cursor_get_count(
    const afw_adaptor_impl_index_cursor_t * instance,
    size_t * count,
    afw_xctx_t *xctx)
{
    impl_afw_adaptor_impl_index_cursor_self_t * self =
        (impl_afw_adaptor_impl_index_cursor_self_t *)instance;
    int rc;

    /* we can only calculate the count for duplicate data on a single key,
        so this means only eq operators are supported. */
    if (self->operator != afw_query_criteria_filter_op_id_eq) {
        return false;
    }

    rc = mdb_cursor_count(self->cursor, count);

    return (rc == 0) ? true : false;
}



/*
 * Implementation of Transaction interface.
 *
 * The following routines implement the functions necessary to 
 * implement a transaction.
 */
afw_lmdb_transaction_t * afw_lmdb_transaction_create(
    afw_lmdb_adaptor_session_t *session,
    afw_xctx_t *xctx)
{
    afw_lmdb_transaction_t *self;
    int rc;

    self = afw_xctx_calloc_type(afw_lmdb_transaction_t, xctx);
    self->pub.inf = &impl_afw_adaptor_transaction_inf;

    self->session = (afw_adaptor_session_t *)session;

    /* 
        Anytime we need to begin a transaction, we must first obtain
        a database lock to prevent another transactions from opening
        databases. 
     */
    apr_thread_rwlock_rdlock(session->adaptor->dbLock);

    rc = mdb_txn_begin(session->adaptor->dbEnv, NULL, 0, &self->txn);
    if (rc) {
        apr_thread_rwlock_unlock(session->adaptor->dbLock);

        AFW_THROW_ERROR_RV_Z(general, lmdb, rc,
            "Unable to begin transaction.", xctx);
    }

    session->transaction = self;
    session->currTxn = self->txn;

    return self;
}




/*
 * Implementation of method release of interface afw_adaptor_transaction.
 */
void
impl_afw_adaptor_transaction_release (
    const afw_adaptor_transaction_t * instance,
    afw_xctx_t *xctx)
{
    /* Assign instance pointer to self. */
    afw_lmdb_transaction_t * self = 
        (afw_lmdb_transaction_t *)instance;
    afw_lmdb_adaptor_session_t * session =
        (afw_lmdb_adaptor_session_t *)self->session;

    /* if our session still has an active transaction going, abort it */
    if (session->transaction) {
        mdb_txn_abort(self->txn);
        apr_thread_rwlock_unlock(session->adaptor->dbLock);
    }

    self->txn = NULL;
    session->transaction = NULL;
}



/*
 * Implementation of method commit of interface afw_adaptor_transaction.
 */
void
impl_afw_adaptor_transaction_commit (
    const afw_adaptor_transaction_t * instance,
    afw_xctx_t *xctx)
{
    /* Assign instance pointer to self. */
    afw_lmdb_transaction_t * self = 
        (afw_lmdb_transaction_t *)instance;
    afw_lmdb_adaptor_session_t * session =
        (afw_lmdb_adaptor_session_t *)self->session;
    int rc;

    rc = mdb_txn_commit(self->txn);
    if (rc) {
        apr_thread_rwlock_unlock(session->adaptor->dbLock);

        AFW_THROW_ERROR_RV_Z(general, lmdb, rc,
            "Unable to commit transaction.", xctx);
    }

    apr_thread_rwlock_unlock(session->adaptor->dbLock);

    /* clear our session transaction to prevent further commits */
    self->txn = NULL;
    session->transaction = NULL;
}

/*
 * Used by adaptive function to perform a reader_check to clear any
 * stale readers of the database.
 */
int
afw_lmdb_internal_reader_check(
    const afw_adaptor_t * instance,
    int * deadReaders,
    afw_xctx_t *xctx)
{
    int rc;
    afw_lmdb_adaptor_t * self = 
        (afw_lmdb_adaptor_t *)instance;

    rc = mdb_reader_check(self->dbEnv, deadReaders);

    return rc;
}

typedef struct {
    const afw_lmdb_adaptor_t    * adaptor;
    const afw_pool_t            * pool;
    afw_xctx_t                  * xctx;
    const afw_utf8_t            ** list;
} afw_lmdb_internal_reader_list_cb_ctx;

int afw_lmdb_internal_reader_list_cb(
    const char *msg,
    void *ctx)
{
    afw_lmdb_internal_reader_list_cb_ctx * context = (afw_lmdb_internal_reader_list_cb_ctx *)ctx;
    const afw_utf8_t *message;

    message = afw_utf8_from_utf8_z(msg, context->pool, context->xctx);
    *(context->list) = afw_utf8_create_copy(
    message->s, message->len, context->pool, context->xctx);

    return (int)strlen(msg);
}

/*
 * Used by adaptive function to perform a reader_list to print a list of 
 * current readers of the database, including their pid and thread id's.
 */
int afw_lmdb_internal_reader_list(
    const afw_adaptor_t * instance,
    const afw_utf8_t ** list,
    const afw_pool_t * pool,
    afw_xctx_t *xctx)
{
    int rc;
    afw_lmdb_adaptor_t * self =
        (afw_lmdb_adaptor_t *)instance;
    afw_lmdb_internal_reader_list_cb_ctx ctx;

    ctx.adaptor = self;
    ctx.pool = pool;
    ctx.xctx = xctx;
    ctx.list = list;

    rc = mdb_reader_list(self->dbEnv, afw_lmdb_internal_reader_list_cb, &ctx);

    return rc;
}
