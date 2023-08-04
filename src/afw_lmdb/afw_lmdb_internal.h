// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework LMDB Adaptor Internal Header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_LMDB_INTERNAL_H__
#define __AFW_LMDB_INTERNAL_H__


/**
 * @file afw_lmdb_internal.h
 * @brief Adaptive Framework LMDB Adaptor Internal Header.
 */

#include "afw_minimal.h"
#include "afw_lmdb.h"
#include "afw_uuid.h"
#include "generated/afw_lmdb_generated.h"
#include <lmdb.h>

AFW_BEGIN_DECLARES

AFW_LMDB_DECLARE_INTERNAL_CONST_DATA(afw_adaptor_factory_t) afw_lmdb_adaptor_factory;

/* Defined in afw_lmdb_metadata.h */
typedef struct afw_lmdb_metadata_s afw_lmdb_metadata_t;

typedef struct afw_lmdb_env_s {
    const afw_utf8_z_t *path_z;
    int mode;
    int maxreaders;
    int maxdbs;
    size_t mapsize;
    unsigned int flags;
} afw_lmdb_env_t;

typedef struct afw_lmdb_limits_s {
    int size_soft;
    int size_hard;
    int time_soft;
    int time_hard;
} afw_lmdb_limits_t;

typedef struct afw_lmdb_adaptor_s {
    afw_adaptor_t pub;
    const afw_content_type_t *ubjson;
    const afw_lmdb_limits_t *limits;
    const afw_lmdb_env_t *env; 
    const afw_object_t *internalConfig;
    MDB_env *dbEnv;
    afw_lmdb_metadata_t *metadata;
    apr_hash_t *dbi_handles;
    apr_thread_rwlock_t *dbLock;
} afw_lmdb_adaptor_t;

typedef struct afw_lmdb_journal_s {
    afw_adaptor_journal_t pub;
    afw_adaptor_session_t *session;
} afw_lmdb_journal_t;

typedef struct afw_lmdb_key_value_s {
    afw_adaptor_key_value_t pub;
    afw_adaptor_session_t *session;
} afw_lmdb_key_value_t;

typedef struct afw_lmdb_transaction_s {
    afw_adaptor_transaction_t pub;
    afw_adaptor_session_t *session;
    MDB_txn *txn;
} afw_lmdb_transaction_t;

/* defines the data used to remember our open transaction handle */
typedef struct {
    const afw_lmdb_adaptor_t *adaptor;
    MDB_txn *txn;
    afw_boolean_t transCommitted;
} afw_lmdb_txn_t;

typedef struct afw_lmdb_adaptor_session_s {
    afw_adaptor_session_t pub;
    afw_lmdb_adaptor_t *adaptor;
    afw_adaptor_session_t *metadata_session;
    afw_adaptor_impl_index_t *indexer;      
    afw_lmdb_journal_t *journal;
    afw_lmdb_key_value_t *key_value;
    
    /* currently used, inner, transaction */
    MDB_txn *currTxn;
    
    /* storage for transaction interface */
    afw_lmdb_transaction_t *transaction;
    
} afw_lmdb_adaptor_session_t;

typedef struct afw_lmdb_adaptor_impl_index_s {
    afw_adaptor_impl_index_t pub;
    const afw_lmdb_adaptor_session_t * session;
    const afw_lmdb_adaptor_t * adaptor;
    MDB_txn *txn;
} afw_lmdb_adaptor_impl_index_t;

typedef struct impl_afw_adaptor_impl_index_cursor_self_s {
    afw_adaptor_impl_index_cursor_t pub;

    /* Private implementation variables */
    const afw_lmdb_adaptor_session_t *session;
    const afw_utf8_t *object_type_id;
    const afw_utf8_t *key_string;
    afw_boolean_t unique;
    afw_query_criteria_filter_op_id_t operator;
    MDB_dbi dbPri;
    MDB_cursor * cursor;
    MDB_val key;
    MDB_val data;

} impl_afw_adaptor_impl_index_cursor_self_t;

/* defines the data used to remember our open database handle */
typedef struct {
    MDB_env *env;
    MDB_dbi dbi;
} afw_lmdb_dbi_t;



/*
 *
 */
afw_lmdb_dbi_t * afw_lmdb_internal_dbi_handle(
    MDB_env           * env, 
    MDB_dbi             dbi, 
    const afw_pool_t  * pool,
    afw_xctx_t       * xctx);


/**
 * @brief Internal create an LMDB adaptor.
 * @param properties Parameters.
 * @param p to use for adaptor resources.
 * @param xctx of caller.
 * @return New instance of LMDB adaptor.
 */
const afw_adaptor_t * afw_lmdb_adaptor_create_cede_p(
    const afw_object_t *properties,
    const afw_pool_t *p, afw_xctx_t *xctx);

/**
 * @brief Internal create a LMDB adaptor session.
 * @param adaptor Internal instance of adaptor.
 * @param xctx of caller.
 * @return New instance of LMDB adaptor session.
 */
afw_lmdb_adaptor_session_t *
    afw_lmdb_adaptor_session_create(
    afw_lmdb_adaptor_t * adaptor,
    afw_xctx_t *xctx);

/**
 * @brief Internal create a LMDB adaptor journal.
 * @param session Internal instance of adaptor session.
 * @param xctx of caller.
 * @return New instance of LMDB adaptor journal.
 */
afw_lmdb_journal_t *
    afw_lmdb_journal_create(
    afw_lmdb_adaptor_session_t *session,
    afw_xctx_t *xctx);

/**
 * @brief Internal create a LMDB adaptor key/value.
 * @param session Internal instance of adaptor session.
 * @param xctx of caller.
 * @return New instance of LMDB key/value.
 */
afw_lmdb_key_value_t * 
    afw_lmdb_key_value_create(
    afw_lmdb_adaptor_session_t *session,
    afw_xctx_t *xctx);

/**
 * @brief Internal create a LMDB adaptor transaction.
 * @param session Internal instance of adaptor session.
 * @param xctx of caller.
 * @return New instance of LMDB adaptor transaction.
 */
afw_lmdb_transaction_t *
    afw_lmdb_transaction_create(
    afw_lmdb_adaptor_session_t *session,
    afw_xctx_t *xctx);


afw_rc_t afw_lmdb_internal_cleanup_free_result(void *data);

const afw_utf8_t * afw_lmdb_internal_get_object_id(
    const afw_lmdb_adaptor_session_t *self,
    afw_xctx_t *xctx);

afw_rc_t afw_lmdb_internal_close_database(void *val);
afw_rc_t afw_lmdb_internal_close_cursor(void *val);

void afw_lmdb_internal_close_transaction(
    void *data, void *data2,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

MDB_dbi afw_lmdb_internal_open_database(
    const afw_lmdb_adaptor_t * adaptor,
    MDB_txn                  * txn,
    const afw_utf8_t         * database,
    unsigned int               flags,
    const afw_pool_t         * p,
    afw_xctx_t              * xctx);

MDB_cursor * afw_lmdb_internal_open_cursor(
    const afw_lmdb_adaptor_session_t *session,
    MDB_dbi dbi,
    afw_xctx_t *xctx);

afw_lmdb_txn_t * afw_lmdb_internal_open_transaction(
    const afw_lmdb_adaptor_session_t *session,
    const afw_lmdb_adaptor_t *adaptor, 
    unsigned int flags,
    afw_boolean_t exclusive,
    afw_xctx_t *xctx);

int afw_lmdb_internal_commit_transaction(
    const afw_lmdb_adaptor_session_t *session,
    const afw_lmdb_adaptor_t *adaptor,
    afw_xctx_t *xctx);

void afw_lmdb_internal_abort_transaction(
    const afw_lmdb_adaptor_session_t *session,
    const afw_lmdb_adaptor_t *adaptor,
    afw_xctx_t *xctx);

int afw_lmdb_internal_index_object(
    const afw_lmdb_adaptor_session_t * session,
    const afw_lmdb_adaptor_t * adaptor,
    const afw_utf8_t * object_type_id,
    const afw_object_t * object,
    const afw_uuid_t *uuid,
    afw_xctx_t *xctx);

int afw_lmdb_internal_index_property(
    const afw_lmdb_adaptor_t * adaptor,
    MDB_txn *txn,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *property,
    const afw_utf8_t *value,
    const afw_uuid_t *uuid,
    afw_xctx_t *xctx);

int afw_lmdb_internal_delete_index(
    const afw_lmdb_adaptor_session_t * session,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_utf8_t *property,
    const afw_value_t *value,
    afw_xctx_t *xctx);

afw_boolean_t afw_lmdb_internal_is_property_indexed(
    const afw_lmdb_adaptor_t *adaptor,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx);

const afw_object_t * afw_lmdb_internal_create_object_from_entry(
    const afw_lmdb_adaptor_session_t *self,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    MDB_dbi dbi,
    afw_xctx_t *xctx);

const afw_value_t * afw_lmdb_internal_create_value_from_entry(
    const afw_lmdb_adaptor_session_t *self,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    MDB_dbi dbi,
    afw_xctx_t *xctx);

void afw_lmdb_internal_create_entry_from_object(
    const afw_lmdb_adaptor_session_t *self,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_object_t *object,
    MDB_dbi dbi,
    afw_xctx_t *xctx);

void afw_lmdb_internal_replace_entry_from_object(
    const afw_lmdb_adaptor_session_t *self,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_object_t *object,
    MDB_dbi dbi,
    afw_xctx_t *xctx);

afw_adaptor_impl_index_cursor_t * afw_lmdb_internal_cursor_create(
    const afw_lmdb_adaptor_session_t *self,
    const afw_utf8_t * database,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * value,
    const afw_query_criteria_filter_op_id_t operator,
    afw_boolean_t unique,
    afw_xctx_t *xctx);

const afw_object_t * afw_lmdb_internal_get_config(
    afw_lmdb_adaptor_t * self,  
    MDB_txn *txn,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

void afw_lmdb_internal_save_config(
    afw_lmdb_adaptor_t * self,
    const afw_object_t * config,
    MDB_txn *txn,
    afw_xctx_t *xctx);

void afw_lmdb_internal_set_key(
    afw_memory_t * key,
    const afw_utf8_t * object_type_id,
    const afw_uuid_t * uuid,
    const afw_pool_t * p,
    afw_xctx_t *xctx);

void afw_lmdb_internal_get_key(
    afw_memory_t  * key,
    afw_utf8_t * object_type_id,
    afw_uuid_t * uuid);

afw_rc_t afw_lmdb_internal_delete_entry(
    MDB_txn *txn,
    MDB_dbi dbi,
    const afw_memory_t * raw_key,
    const afw_memory_t * raw_value,
    afw_xctx_t *xctx);

int afw_lmdb_internal_reader_check(
    const afw_adaptor_t * instance,
    int * deadReaders,
    afw_xctx_t *xctx);

int afw_lmdb_internal_reader_list(
    const afw_adaptor_t * instance,
    const afw_utf8_t ** list,
    const afw_pool_t * pool,
    afw_xctx_t *xctx);

/**
 * @brief Begin an LMDB transaction.
 * 
 * All LMDB operations *must* be wrapped inside a LMDB 
 * transaction, even when the transaction interface is not 
 * requested explicitly by the user. 
 * 
 * \fixme LMDB transactions may also be nested (second param). 
 * However, I don't believe LMDB allows for write transactions 
 * to be nested under read transactions, so this macro needs 
 * to contain logic to handle both scenarios.
 */
#define AFW_LMDB_BEGIN_TRANSACTION( \
    adaptor,    \
    session,    \
    flags,      \
    exclusive,  \
    xctx)      \
do { \
    MDB_txn * this_txn = NULL; \
    bool this_txnHandled = false; \
    const afw_lmdb_adaptor_t * this_adaptor = adaptor; \
    const afw_lmdb_adaptor_session_t * this_session = session; \
    afw_xctx_t * this_xctx = xctx; \
    int this_rc; \
    AFW_TRY { \
        if (session && session->transaction) { \
            ((afw_lmdb_adaptor_session_t *)session)->currTxn = session->transaction->txn; \
        } else { \
            if (exclusive) { \
                apr_thread_rwlock_wrlock(adaptor->dbLock); \
            } else { \
                apr_thread_rwlock_rdlock(adaptor->dbLock); \
            } \
            this_rc = mdb_txn_begin(adaptor->dbEnv, NULL, flags, &this_txn); \
            if (this_rc) { \
                apr_thread_rwlock_unlock(adaptor->dbLock); \
                AFW_THROW_ERROR_RV_Z(general, lmdb, this_rc, \
                    "Unable to begin transaction.", this_xctx); \
            } \
            if (session) \
                ((afw_lmdb_adaptor_session_t *)session)->currTxn = this_txn; \
        } \
        do {

/**
 * 
 */
#define AFW_LMDB_GET_TRANSACTION() \
    (this_session && this_session->currTxn) ? this_session->currTxn : this_txn

/**
 * @brief Commit a transaction.
 */
#define AFW_LMDB_COMMIT_TRANSACTION() \
    if (!(this_session && this_session->transaction)) { \
        if (this_txn && !this_txnHandled) { \
            this_rc = mdb_txn_commit(this_txn); \
            this_txnHandled = true; \
            if (this_rc) { \
                AFW_THROW_ERROR_RV_Z(general, lmdb_internal, this_rc, \
                    "Unable to commit transaction.", this_xctx); \
            } \
        } \
    } 

/**
 * @brief Abort a transaction.
 */
#define AFW_LMDB_ABORT_TRANSACTION() \
    if (!(this_session && this_session->transaction)) { \
        if (this_txn && !this_txnHandled) { \
            mdb_txn_abort(this_txn); \
            txnHandled = true; \
            if (this_rc) { \
                AFW_THROW_ERROR_RV_Z(general, lmdb_internal, this_rc, \
                    "Unable to abort transaction.", this_xctx); \
            } \
        } \
    } 

/**
 * @brief End an LMDB transaction.
 */
#define AFW_LMDB_END_TRANSACTION() \
        } while (0); \
    } AFW_FINALLY { \
        if (!(this_session && this_session->transaction)) { \
            if (this_txn && !this_txnHandled) { \
                mdb_txn_abort(this_txn); \
                this_txnHandled = true; \
            } \
            apr_thread_rwlock_unlock(this_adaptor->dbLock); \
        } \
        AFW_ERROR_RETHROW; \
    } \
    AFW_ENDTRY; \
} while(0)

AFW_END_DECLARES

#endif /* __AFW_LMDB_INTERNAL_H__ */
