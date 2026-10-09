// See the 'COPYING' file in the project root for licensing information.
/*
 * Indexing routines for LMDB Adapter
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_lmdb_index.c
 * @brief LMDB adapter index create/update and cursor routines.
 */

#include "afw.h"
#include "afw_uuid.h"
#include "afw_adapter_impl.h"
#include "afw_adapter_impl_index.h"

#include "afw_lmdb_index.h"
#include "afw_lmdb_internal.h"
#include "generated/afw_lmdb_generated_internal.h"

#define AFW_IMPLEMENTATION_ID "lmdb_index"
#define AFW_ADAPTER_IMPL_INDEX_SELF_T afw_lmdb_adapter_impl_index_t
#include "afw_adapter_impl_index_impl_declares.h"



afw_adapter_impl_index_t * afw_lmdb_adapter_impl_index_create(
    const afw_lmdb_adapter_session_t * session,
    const afw_lmdb_adapter_t         * adapter,
    MDB_txn                          * txn,
    afw_xctx_t                      * xctx)
{
    afw_lmdb_adapter_impl_index_t *self;

    /*
     * You may want to create a new pool for instance, but will just use
     * xctx's pool in this example.
     */
    self = afw_pool_calloc_type(xctx->p, afw_lmdb_adapter_impl_index_t, xctx);

    self->pub.inf = &impl_afw_adapter_impl_index_inf;
    self->session = session;
    self->adapter = adapter;
    self->txn = txn;
    self->pub.indexDefinitions = impl_afw_adapter_impl_index_get_index_definitions(
        (AFW_ADAPTER_IMPL_INDEX_SELF_T *)self, xctx);

    /* Return new instance. */
    return (afw_adapter_impl_index_t*)self;
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


afw_boolean_t afw_lmdb_index_database_is_for_key(
    const afw_utf8_t *database,
    const afw_utf8_t *key,
    afw_utf8_t *object_type_id)
{
    const afw_utf8_t *prefix = afw_lmdb_s_Index;
    afw_size_t fixed;

    /* "Index" "#" type "#" key, with a type of at least one octet */
    fixed = prefix->len + 1 + 1 + key->len;
    if (database->len <= fixed ||
        memcmp(database->s, prefix->s, prefix->len) != 0 ||
        database->s[prefix->len] != '#' ||
        database->s[database->len - key->len - 1] != '#' ||
        memcmp(database->s + database->len - key->len, key->s,
            key->len) != 0)
    {
        return false;
    }

    object_type_id->s = database->s + prefix->len + 1;
    object_type_id->len = database->len - fixed;

    return true;
}


/*
 * Implementation of method release of interface afw_adapter_impl_index.
 */
void impl_afw_adapter_impl_index_release(
    AFW_ADAPTER_IMPL_INDEX_SELF_T *self,
    afw_xctx_t *xctx)
{

}

/*
 * Implementation of method get_index_definitions of interface afw_adapter_impl_index.
 */
const afw_object_t *
impl_afw_adapter_impl_index_get_index_definitions (
    AFW_ADAPTER_IMPL_INDEX_SELF_T *self,
    afw_xctx_t *xctx)
{
    const afw_adapter_t *adapter = (afw_adapter_t *)self->adapter;
    const afw_object_t *indexes;
    const afw_pool_t *pool;
    afw_uint32_t generation;

    /*
     * Long-lived home for the cached copy: the session's own pool when
     * session-backed, or the adapter's pool for the one-off adapter-level
     * indexer (afw_lmdb_adapter_impl_index_create() called with an
     * explicit txn and no session). Must NOT be the calling xctx's pool --
     * this getter can now be called again, later, from a different and
     * possibly shorter-lived xctx than the one that created this instance
     * (issue #252 item 3), and the cached copy has to outlive that call.
     */
    pool = (self->session) ? self->session->pub.p : self->adapter->pub.p;

    /* lock the adapter to check/fetch the indexes */
    AFW_ADAPTER_IMPL_LOCK_READ_BEGIN(adapter) {
        generation = self->adapter->indexDefinitionsGeneration;

        /*
         * Cheap common case (issue #252 item 3): nothing has changed
         * adapter-wide (via update_index_definitions(), called by
         * index_create/index_remove on any session) since we last cached
         * indexDefinitions on this instance -- reuse it instead of
         * cloning again on every call.
         */
        if (self->pub.indexDefinitions &&
            generation == self->definitionsGeneration)
        {
            indexes = self->pub.indexDefinitions;
        }
        else {
            indexes = afw_object_get_property_as_object_internal(
                self->adapter->internalConfig, afw_lmdb_v_indexDefinitions,
                xctx);
            if (indexes) {
                indexes = afw_object_create_pooled_copy(indexes, pool, xctx);
            } else {
                /* if we don't have one, just create one in our own pool */
                indexes = afw_object_create_unmanaged_new_p(pool, xctx);
            }
            self->definitionsGeneration = generation;
        }
    }
    /* unlock, we have our own copy now */
    AFW_ADAPTER_IMPL_LOCK_READ_END;

    self->pub.indexDefinitions = indexes;

    return indexes;
}

/*
 * Implementation of method update_index_definitions of interface afw_adapter_impl_index.
 */
void
impl_afw_adapter_impl_index_update_index_definitions (
    AFW_ADAPTER_IMPL_INDEX_SELF_T *self,
    const afw_object_t * indexDefinitions,
    afw_xctx_t *xctx)
{
    const afw_lmdb_adapter_t *adapter = self->adapter;
    const afw_lmdb_adapter_session_t *session = self->session;
    const afw_pool_t *pool;
    MDB_txn *txn;

    /* clone this object with our adapter's memory pool */
    pool = adapter->pub.p;

    /*
     * self->txn is only set for the one-off adapter-level indexer
     * (afw_lmdb_adapter_impl_index_create() called with an explicit txn).
     * A session-backed indexer's self->txn is always NULL -- the live
     * transaction for this session lives on the session itself (either an
     * explicit begin_transaction(), or the implicit per-request txn), so
     * fall back to it via the same reentrant-safe macro impl_index_open
     * uses, rather than passing a stale NULL txn to save_config.
     */
    if (self->txn == NULL) {
        AFW_LMDB_BEGIN_TRANSACTION(adapter, session, 0, xctx) {
            txn = AFW_LMDB_GET_TRANSACTION();

            /* lock the adapter */
            AFW_ADAPTER_IMPL_LOCK_WRITE_BEGIN(((afw_adapter_t *)adapter)) {
                afw_object_set_property_as_object_internal(
                    session->adapter->internalConfig, afw_lmdb_v_indexDefinitions,
                    afw_object_create_pooled_copy(
                        indexDefinitions, pool, xctx),
                    xctx);

                /* now write out our new config */
                afw_lmdb_internal_save_config(session->adapter,
                    session->adapter->internalConfig, txn, xctx);

                /*
                 * Bump so any other indexer instance's cached
                 * indexDefinitions (this one's own field is already
                 * current -- the caller mutated it in place before
                 * calling us) is recognized as stale on its next check
                 * (issue #252 item 3).
                 */
                self->definitionsGeneration =
                    ++((afw_lmdb_adapter_t *)adapter)->indexDefinitionsGeneration;
            }
            AFW_ADAPTER_IMPL_LOCK_WRITE_END;

            AFW_LMDB_COMMIT_TRANSACTION();
        }
        AFW_LMDB_END_TRANSACTION();
    } else {
        txn = self->txn;

        /* lock the adapter */
        AFW_ADAPTER_IMPL_LOCK_WRITE_BEGIN(((afw_adapter_t *)adapter)) {
            afw_object_set_property_as_object_internal(
                session->adapter->internalConfig, afw_lmdb_v_indexDefinitions,
                afw_object_create_pooled_copy(
                    indexDefinitions, pool, xctx),
                xctx);

            /* now write out our new config */
            afw_lmdb_internal_save_config(session->adapter,
                session->adapter->internalConfig, txn, xctx);

            self->definitionsGeneration =
                ++((afw_lmdb_adapter_t *)adapter)->indexDefinitionsGeneration;
        }
        AFW_ADAPTER_IMPL_LOCK_WRITE_END;
    }
}

/* Flags LMDB stores with a database. */
#define IMPL_PERSISTENT_FLAGS (MDB_REVERSEKEY | MDB_DUPSORT | \
    MDB_INTEGERKEY | MDB_DUPFIXED | MDB_INTEGERDUP | MDB_REVERSEDUP)

/*
 * A database already there may be one a removed index cleared, which
 * keeps that index's flags until the environment is next opened in a new
 * process (#511). LMDB keeps a database's flags whatever mdb_dbi_open()
 * is given, so this index would store its entries the wrong way.
 */
static void
impl_index_flags_check(
    MDB_txn *txn,
    MDB_dbi dbi,
    const afw_utf8_t *database,
    unsigned int flags,
    afw_xctx_t *xctx)
{
    unsigned int existing;
    int rc;

    rc = mdb_dbi_flags(txn, dbi, &existing);
    if (rc) {
        AFW_THROW_ERROR_RV_FZ(general, lmdb, rc, xctx,
            "Unable to read flags of database: '%.*s'.",
            (int)database->len, database->s);
    }

    if ((existing & IMPL_PERSISTENT_FLAGS) !=
        (flags & IMPL_PERSISTENT_FLAGS))
    {
        AFW_THROW_ERROR_FZ(general, xctx,
            "Index database '%.*s' is left from a removed index with "
            "different options. Restart AFW to create this index with "
            "these options.",
            (int)database->len, database->s);
    }
}


/*
 * Open or create the index database of key in txn. For object_type_id
 * NULL (an index on all object types), index add creates a database per
 * object type, always with MDB_DUPSORT; only those already there are
 * checked.
 */
static void
impl_index_open(
    const afw_lmdb_adapter_t *adapter,
    MDB_txn *txn,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *key,
    unsigned int flags,
    const afw_pool_t *pool,
    afw_xctx_t *xctx)
{
    const afw_utf8_t * const *names;
    const afw_utf8_t *database;
    afw_utf8_t type;
    MDB_dbi dbi;
    int rc;

    if (!object_type_id) {
        for (names = afw_lmdb_internal_database_names(txn, pool, xctx);
            *names; names++)
        {
            if (afw_lmdb_index_database_is_for_key(*names, key, &type)) {
                dbi = afw_lmdb_internal_open_database(adapter, txn,
                    *names, 0, pool, xctx);
                impl_index_flags_check(txn, dbi, *names, MDB_DUPSORT,
                    xctx);
            }
        }
        return;
    }

    database = afw_lmdb_index_database(object_type_id, key, pool, xctx);

    rc = afw_lmdb_internal_try_open_database(adapter, txn, database,
        flags & ~MDB_CREATE, &dbi, pool, xctx);
    if (rc == 0) {
        impl_index_flags_check(txn, dbi, database, flags, xctx);
    }
    else if (rc == MDB_NOTFOUND) {
        afw_lmdb_internal_open_database(adapter, txn, database, flags,
            pool, xctx);
    }
    else {
        AFW_THROW_ERROR_RV_FZ(general, lmdb, rc, xctx,
            "Unable to open database: '%.*s'.",
            (int)database->len, database->s);
    }
}


/*
 * Implementation of method create of interface afw_adapter_impl_index.
 */
void
impl_afw_adapter_impl_index_open(
    AFW_ADAPTER_IMPL_INDEX_SELF_T *self,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * key,
    afw_boolean_t unique,
    afw_boolean_t reverse,
    const afw_pool_t * pool,
    afw_xctx_t *xctx)
{
    const afw_lmdb_adapter_t *adapter = self->adapter;
    const afw_lmdb_adapter_session_t * session = self->session;
    unsigned int flags;

    /* we want to create the database, if it doesn't exist */
    flags = MDB_CREATE;

    /* now, select individual properties for our index */
    if (!unique) flags |= MDB_DUPSORT;

    if (reverse) flags |= MDB_REVERSEKEY | MDB_REVERSEDUP;

    /* open up our own write transaction, if we haven't one already */
    if (self->txn == NULL) {
        AFW_LMDB_BEGIN_TRANSACTION(adapter, session, 0, xctx) {        

            impl_index_open(adapter, AFW_LMDB_GET_TRANSACTION(),
                object_type_id, key, flags, pool, xctx);

            /* and commit our change */
            AFW_LMDB_COMMIT_TRANSACTION();
        }        
        AFW_LMDB_END_TRANSACTION();
    } else {
        impl_index_open(adapter, self->txn,
            object_type_id, key, flags, pool, xctx);
    }
}


/*
 * Index entry data: {object_type_id}{uuid}. object_id may be an alias
 * (a retroactive scan reports objects by alias), so it is resolved in
 * txn.
 */
static void
impl_index_data(
    const afw_lmdb_adapter_t *adapter,
    MDB_txn *txn,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    MDB_val *data,
    const afw_pool_t *pool,
    afw_xctx_t *xctx)
{
    const afw_uuid_t *uuid;
    afw_memory_t raw;

    uuid = afw_lmdb_internal_object_uuid(adapter, txn,
        object_type_id, object_id, pool, xctx);

    afw_lmdb_internal_set_key(&raw, object_type_id, uuid, pool, xctx);

    memset(data, 0, sizeof(MDB_val));
    data->mv_data = (void *)raw.ptr;
    data->mv_size = raw.size;
}


/*
 * Implementation of method add of interface afw_adapter_impl_index.
 */
void impl_afw_adapter_impl_index_add(
    AFW_ADAPTER_IMPL_INDEX_SELF_T *self,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    const afw_utf8_t * indexKey,
    const afw_utf8_t * value,
    afw_boolean_t      unique,
    const afw_pool_t * pool,
    afw_xctx_t *xctx)
{
    const afw_lmdb_adapter_t *adapter = self->adapter;
    const afw_lmdb_adapter_session_t *session = self->session;
    afw_rc_t rc;
    MDB_txn * txn;
    MDB_dbi dbi;
    MDB_val key, data;
    const afw_utf8_t *database;

    /* we will get an error if we try to add a key of length 0 */
    /** @fixme this basically avoids indexing an empty string, so
    we should determine if this is an error condition or not
     */
    if (value->len == 0) {
        return;
    }

    database = afw_lmdb_index_database(
        object_type_id, indexKey, pool, xctx);

    key.mv_data = (void*)value->s;
    key.mv_size = value->len;

    /*
     * self->txn is only set for the one-off adapter-level indexer; a
     * session-backed indexer's self->txn is always NULL, so fall back to
     * the session's active transaction via the same reentrant-safe macro
     * impl_index_open uses.
     */
    if (self->txn == NULL) {
        AFW_LMDB_BEGIN_TRANSACTION(adapter, session, 0, xctx) {
            txn = AFW_LMDB_GET_TRANSACTION();

            impl_index_data(adapter, txn, object_type_id, object_id,
                &data, pool, xctx);

            dbi = afw_lmdb_internal_open_database(session->adapter,
                txn, database, MDB_DUPSORT|MDB_CREATE, pool, xctx);

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

            AFW_LMDB_COMMIT_TRANSACTION();
        }
        AFW_LMDB_END_TRANSACTION();
    } else {
        txn = self->txn;

        impl_index_data(adapter, txn, object_type_id, object_id,
            &data, pool, xctx);

        dbi = afw_lmdb_internal_open_database(session->adapter,
            txn, database, MDB_DUPSORT|MDB_CREATE, pool, xctx);

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
    }
}

/*
 * Implementation of method delete of interface afw_adapter_impl_index.
 */
void impl_afw_adapter_impl_index_delete(
    AFW_ADAPTER_IMPL_INDEX_SELF_T *self,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_utf8_t *indexKey,
    const afw_utf8_t *value,
    const afw_pool_t *pool,
    afw_xctx_t *xctx)
{
    const afw_lmdb_adapter_t *adapter = self->adapter;
    const afw_lmdb_adapter_session_t *session = self->session;
    MDB_val key, data;
    MDB_dbi dbi;
    MDB_txn *txn;
    const afw_utf8_t *database;
    afw_rc_t rc;

    /* we will get an error if we try to delete a key of length 0 */
    /** @fixme this basically avoids indexing an empty string, so
    we should determine if this is an error condition or not
     */
    if (value->len == 0) {
        return;
    }

    database = afw_lmdb_index_database(
        object_type_id, indexKey, pool, xctx);

    memset(&key, 0, sizeof(MDB_val));
    key.mv_data = (void *)value->s;
    key.mv_size = value->len;

    /*
     * self->txn is only set for the one-off adapter-level indexer; a
     * session-backed indexer's self->txn is always NULL, so fall back to
     * the session's active transaction via the same reentrant-safe macro
     * impl_index_open uses.
     */
    if (self->txn == NULL) {
        AFW_LMDB_BEGIN_TRANSACTION(adapter, session, 0, xctx) {
            txn = AFW_LMDB_GET_TRANSACTION();

            impl_index_data(adapter, txn, object_type_id, object_id,
                &data, pool, xctx);

            dbi = afw_lmdb_internal_open_database(session->adapter,
                txn, database, MDB_DUPSORT, pool, xctx);

            rc = mdb_del(txn, dbi, &key, &data);
            if (rc != 0 && rc != MDB_NOTFOUND) {
                AFW_THROW_ERROR_RV_Z(general, lmdb, rc,
                    "Unable to delete index value.", xctx);
            }

            AFW_LMDB_COMMIT_TRANSACTION();
        }
        AFW_LMDB_END_TRANSACTION();
    } else {
        txn = self->txn;

        impl_index_data(adapter, txn, object_type_id, object_id,
            &data, pool, xctx);

        dbi = afw_lmdb_internal_open_database(session->adapter,
            txn, database, MDB_DUPSORT, pool, xctx);

        rc = mdb_del(txn, dbi, &key, &data);
        if (rc != 0 && rc != MDB_NOTFOUND) {
            AFW_THROW_ERROR_RV_Z(general, lmdb, rc,
                "Unable to delete index value.", xctx);
        }
    }
}

/*
 * Clear the index databases of key in txn: Index#<object_type_id>#<key>,
 * or for object_type_id NULL (an index on all object types) every
 * Index#<type>#<key> and the old unused Index#<key>.
 */
static int
impl_index_clear(
    const afw_lmdb_adapter_t *adapter,
    MDB_txn *txn,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *key,
    const afw_pool_t *pool,
    afw_xctx_t *xctx)
{
    const afw_utf8_t * const *names;
    afw_utf8_t type;
    int rc;

    rc = afw_lmdb_internal_clear_database(adapter, txn,
        afw_lmdb_index_database(object_type_id, key, pool, xctx),
        pool, xctx);

    if (rc == 0 && !object_type_id) {
        for (names = afw_lmdb_internal_database_names(txn, pool, xctx);
            rc == 0 && *names; names++)
        {
            if (afw_lmdb_index_database_is_for_key(*names, key, &type)) {
                rc = afw_lmdb_internal_clear_database(adapter, txn,
                    *names, pool, xctx);
            }
        }
    }

    return rc;
}


/*
 * Implementation of method drop of interface afw_adapter_impl_index.
 *
 * Clears the index databases rather than deleting them. Deleting closes
 * the handle for the whole process at once (mdb_drop() del 1), under any
 * transaction still reading it. A cleared database that no definition
 * covers is deleted when the adapter next opens the environment in a new
 * process (#511).
 */
afw_rc_t
impl_afw_adapter_impl_index_drop (
    AFW_ADAPTER_IMPL_INDEX_SELF_T *self,
    const afw_utf8_t  * object_type_id,
    const afw_utf8_t  * key,
    const afw_pool_t  * pool,
    afw_xctx_t       * xctx)
{
    const afw_lmdb_adapter_t *adapter = self->adapter;
    const afw_lmdb_adapter_session_t *session = self->session;
    afw_rc_t rc = 0;

    /*
     * self->txn is only set for the one-off adapter-level indexer; a
     * session-backed indexer's self->txn is always NULL, so fall back to
     * the session's active transaction via the same reentrant-safe macro
     * impl_index_open uses.
     */
    if (self->txn == NULL) {
        AFW_LMDB_BEGIN_TRANSACTION(adapter, session, 0, xctx) {
            rc = impl_index_clear(adapter, AFW_LMDB_GET_TRANSACTION(),
                object_type_id, key, pool, xctx);

            AFW_LMDB_COMMIT_TRANSACTION();
        }
        AFW_LMDB_END_TRANSACTION();
    } else {
        rc = impl_index_clear(adapter, self->txn,
            object_type_id, key, pool, xctx);
    }

    return rc;
}

/*
 * Implementation of method open_cursor of interface afw_adapter_impl_index.
 */
afw_adapter_impl_index_cursor_t *
impl_afw_adapter_impl_index_open_cursor (
    AFW_ADAPTER_IMPL_INDEX_SELF_T *self,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * key,
    int                operator,
    const afw_utf8_t * value,
    afw_boolean_t      unique,
    const afw_pool_t * pool,
    afw_xctx_t      * xctx)
{
    afw_adapter_impl_index_cursor_t *cursor;
    const afw_utf8_t * database;

    database = afw_lmdb_index_database(
        object_type_id, key, pool, xctx);

    cursor = afw_lmdb_internal_cursor_create(self->session, 
        database, object_type_id, value, operator, unique, xctx);

    return cursor;
}

/*
 * Implementation of method get_session of interface afw_adapter_impl_index.
 */
const afw_adapter_session_t *
impl_afw_adapter_impl_index_get_session (
    AFW_ADAPTER_IMPL_INDEX_SELF_T *self,
    afw_xctx_t *xctx)
{
    /* Assign &self->pub pointer to self. */

    return (const afw_adapter_session_t *) self->session;
}
