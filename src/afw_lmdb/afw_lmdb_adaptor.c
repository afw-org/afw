// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_adaptor interface for LMDB
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_lmdb_adaptor.c
 * @brief Implementation of afw_adaptor interface for LMDB.
 */

#include "afw.h"
#include "generated/afw_lmdb_generated.h"
#include "afw_lmdb.h"
#include "afw_lmdb_internal.h"
#include "afw_lmdb_metadata.h"
#include "afw_lmdb_index.h"
#include "afw_adaptor_impl.h"
#include "afw_adaptor_impl_index.h"

/* Declares and rti/inf defines for interface afw_adaptor */
#define AFW_IMPLEMENTATION_ID "lmdb"
#include "afw_adaptor_impl_declares.h"

/*
 * This routine loads the LMDB Adaptor's internal configuration object.
 */
void afw_lmdb_adaptor_load_configuration(
    afw_lmdb_adaptor_t * self,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    MDB_txn *txn;
    const afw_object_t *config;
    int rc;

    rc = mdb_txn_begin(self->dbEnv, NULL, 0, &txn);
    if (rc) {
        AFW_THROW_ERROR_RV_Z(general, lmdb, rc,
            "Unable to begin initial transaction.", xctx);
    }

    config = afw_lmdb_internal_get_config(self, txn, p, xctx);

    self->internalConfig = afw_object_create_clone(
        config, p, xctx);

    afw_lmdb_internal_save_config(self, self->internalConfig, txn, xctx);

    rc = mdb_txn_commit(txn);
    if (rc) {
        AFW_THROW_ERROR_RV_Z(general, lmdb, rc,
            "Unable to commit initial transaction.", xctx);
    }
}

/*
 *
 */
const afw_lmdb_env_t * afw_lmdb_adaptor_parse_env(
    const afw_object_t *envObject, 
    const afw_pool_t *p, 
    afw_xctx_t *xctx)
{
    afw_lmdb_env_t *env;
    const afw_value_t *value;

    env = afw_xctx_calloc_type(afw_lmdb_env_t, xctx);

    value = afw_object_get_property(envObject, afw_lmdb_s_path, xctx);
    if (!value) {
        AFW_THROW_ERROR_Z(general,
            "Property env.path required by LMDB adaptor.", xctx);
    }
    env->path_z = afw_value_as_utf8_z(value, p, xctx);

    value = afw_object_get_property(envObject, afw_lmdb_s_mode, xctx);
    if (!value) {
        AFW_THROW_ERROR_Z(general,
            "Property env.mode required by LMDB adaptor.", xctx);
    }
    env->mode = afw_safe_cast_integer_to_int(afw_value_as_integer(value, xctx), xctx);

    value = afw_object_get_property(envObject, afw_lmdb_s_maxreaders, xctx);
    if (value) {
        env->maxreaders = afw_safe_cast_integer_to_int(afw_value_as_integer(value, xctx), xctx);
    } else {
        env->maxreaders = 0;
    }

    value = afw_object_get_property(envObject, afw_lmdb_s_maxdbs, xctx);
    if (value) {
        env->maxdbs = afw_safe_cast_integer_to_int(afw_value_as_integer(value, xctx), xctx);
    } else {
        env->maxdbs = 0;
    }

    value = afw_object_get_property(envObject, afw_lmdb_s_mapsize, xctx);
    if (value) {
        env->mapsize = (size_t)afw_value_as_integer(value, xctx);
    } else {
        env->mapsize = 0;
    }

    return env;
}

const afw_lmdb_limits_t * afw_lmdb_adaptor_parse_limits(
    const afw_object_t *lim, afw_xctx_t *xctx) 
{
    afw_lmdb_limits_t *limits;
    const afw_object_t *obj;
    const afw_value_t *value;

    limits = afw_xctx_calloc_type(afw_lmdb_limits_t, xctx);

    value = afw_object_get_property(lim, afw_lmdb_s_size, xctx);
    if (value) {
        obj = afw_value_as_object(value, xctx);

        value = afw_object_get_property(obj, afw_lmdb_s_soft, xctx);
        if (value)
            limits->size_soft = afw_safe_cast_integer_to_int(afw_value_as_integer(value, xctx), xctx);
        else
            limits->size_soft = 500;

        value = afw_object_get_property(obj, afw_lmdb_s_hard, xctx);
        if (value)
            limits->size_hard = afw_safe_cast_integer_to_int(afw_value_as_integer(value, xctx), xctx);
        else
            limits->size_hard = 1000;
    }

    value = afw_object_get_property(lim, afw_lmdb_s_time, xctx);
    if (value) {
        obj = afw_value_as_object(value, xctx);

        value = afw_object_get_property(obj, afw_lmdb_s_soft, xctx);
        if (value)
            limits->time_soft = afw_safe_cast_integer_to_int(afw_value_as_integer(value, xctx), xctx);
        else
            limits->time_soft = 3600;

        value = afw_object_get_property(obj, afw_lmdb_s_hard, xctx);
        if (value)
            limits->time_hard = afw_safe_cast_integer_to_int(afw_value_as_integer(value, xctx), xctx);
        else
            limits->time_hard = 14400;
    }

    return limits;
}

/*
 * The LMDB API to open a database is mdb_dbi_open().
 *
 * According to the documentation, this function must not be
 * called from multiple concurrent transactions in the same 
 * process.  One way to make this easy is to go ahead and
 * open all the databases we think we will need ahead of time.
 *
 */
void afw_lmdb_adaptor_open_databases(
    afw_lmdb_adaptor_t * self, 
    const afw_pool_t   * pool,
    afw_xctx_t        * xctx)
{
    const afw_adaptor_impl_index_t *indexer;
    const afw_object_t *indexDefinitions;
    MDB_txn *txn;
    int rc;

    rc = mdb_txn_begin(self->dbEnv, NULL, 0, &txn);
    if (rc) {
        AFW_THROW_ERROR_RV_Z(general, lmdb, rc,
            "Unable to begin initial transaction.", xctx);
    }

    /* first, open our "root" database, which is really only used for metrics */
    //! @fixme
    //afw_lmdb_internal_open_database(self, txn, 
    //    NULL, MDB_CREATE, pool, xctx);

    /* open the Primary database */
    afw_lmdb_internal_open_database(self, txn, 
        afw_lmdb_s_Primary, MDB_CREATE, pool, xctx);

    /* open the Journal database */
    afw_lmdb_internal_open_database(self, txn, 
        afw_lmdb_s_Journal, MDB_CREATE, pool, xctx);

    indexer = afw_lmdb_adaptor_impl_index_create(NULL, self, txn, xctx);

    /* now open up any index databases we may have */
    indexDefinitions = afw_object_old_get_property_as_object(
        self->internalConfig, afw_lmdb_s_indexDefinitions, xctx);
    if (indexDefinitions) {
        afw_adaptor_impl_index_open_definitions(indexer, 
            indexDefinitions, pool, xctx);
    }

    /** @fixme: set compare routines? */

    rc = mdb_txn_commit(txn);
    if (rc) {
        AFW_THROW_ERROR_RV_Z(general, lmdb, rc,
            "Unable to commit initial transaction.", xctx);
    }
}

const afw_adaptor_t * afw_lmdb_adaptor_create_cede_p(
    const afw_object_t *properties,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_lmdb_adaptor_t *self;
    afw_adaptor_t *adaptor;
    const afw_value_t *value;
    const afw_object_t *env;
    const afw_object_t *limits;
    int rc;
    int deadReaders;

    /* Create adaptor and process common properties */
    adaptor = afw_adaptor_impl_create_cede_p(
        &impl_afw_adaptor_inf,
        sizeof(afw_lmdb_adaptor_t),
        properties, p, xctx);
    self = (afw_lmdb_adaptor_t *)adaptor;

    /* This adaptor will always use the UBJSON content type */
    self->ubjson = afw_environment_get_content_type(
        afw_s_ubjson, xctx);
    if (!self->ubjson)
    {
        AFW_THROW_ERROR_Z(general,
            "Unable to load UBJSON content-type", xctx);
    }

    /* Get our env parameters */
    value = afw_object_get_property(properties, afw_lmdb_s_env, xctx);
    if (!value) {
        AFW_THROW_ERROR_Z(general,
            "Property env required by LMDB adaptor.", xctx);
    }
    env = afw_value_as_object(value, xctx);

    self->env = afw_lmdb_adaptor_parse_env(env, p, xctx);

    /* create our LMDB environment */
    rc = mdb_env_create(&self->dbEnv);
    if (rc) {
        AFW_THROW_ERROR_RV_Z(general, lmdb, rc,
            "Unable to initialize LMDB environment.", xctx);
    }

    /* set our maxdb's */
    if (self->env->maxdbs)
        mdb_env_set_maxdbs(self->dbEnv, self->env->maxdbs);
    else
        mdb_env_set_maxdbs(self->dbEnv, 128);

    /* set our maxreaders */
    if (self->env->maxreaders)
        mdb_env_set_maxreaders(self->dbEnv, self->env->maxreaders);

    /* set our mapsize */
    if (self->env->mapsize)
        mdb_env_set_mapsize(self->dbEnv, self->env->mapsize);

    /*
        See:  https://github.com/BVLC/caffe/issues/2404

        To make valgrind happy, our mapsize must fit inside physical
        memory.  Otherwise, the previous call will fail.

        If using valgrind, compile with -DVALGRIND
     */
#ifdef VALGRIND
    mdb_env_set_mapsize(self->dbEnv, 
        (self->env->mapsize && (self->env->mapsize < 7438953472) ? 
            self->env->mapsize : 7438953472));
#endif

    /* Now, open our LMDB environment */
    rc = mdb_env_open(self->dbEnv, self->env->path_z, 0, self->env->mode);
    if (rc) {
        AFW_THROW_ERROR_RV_Z(general, lmdb, rc,
            "Unable to open LMDB environment.  Check path and permissions.",
            xctx);
    }

    /* 
        If multiple processes have the same LMDB environment open, then readers 
        can accumulate.  This routine checks for stale readers and cleans them
        up, if they are no longer in use.
     */
    // FIXME check return code here and decide what to do/throw
    mdb_reader_check(self->dbEnv, &deadReaders);

    value = afw_object_get_property(properties, afw_lmdb_s_limits, xctx);
    if (value) {
        limits = afw_value_as_object(value, xctx);
        self->limits = afw_lmdb_adaptor_parse_limits(limits, xctx);
    }

    /* Load metadata. */
    afw_lmdb_metadata_refresh(self, xctx);

    /* 
        Create a database reader/writer lock, so transactions do not interfere
        if we need to open a database in the future, that we haven't already
        opened.
     */
    apr_thread_rwlock_create(&self->dbLock, afw_pool_get_apr_pool(p));

    /* create our dbi_handles */
    self->dbi_handles = apr_hash_make(afw_pool_get_apr_pool(p));

    /* load our internal configuration object */
    afw_lmdb_adaptor_load_configuration(self, p, xctx);

    /* Pre-open any databases we will need */
    afw_lmdb_adaptor_open_databases(self, p, xctx);

    /* Return adaptor. */
    return (const afw_adaptor_t *)self;
}



/*
 * Implementation of method destroy of interface afw_adaptor.
 */
void
impl_afw_adaptor_destroy(
    const afw_adaptor_t * instance,
    afw_xctx_t *xctx)
{  
    afw_lmdb_adaptor_t *self = (afw_lmdb_adaptor_t *) instance;
    apr_hash_index_t *hi;

    /* close any open databases */
    for (hi = apr_hash_first(NULL, self->dbi_handles); 
        hi; hi = apr_hash_next(hi)) {
        const char *k;
        afw_lmdb_dbi_t *v;

        apr_hash_this(hi, (const void **)&k, NULL, (void**)&v);
        mdb_dbi_close(self->dbEnv, v->dbi);
    }

    /* release our database reader/writer lock */
    apr_thread_rwlock_destroy(self->dbLock);

    /* close our LMDB environment */
    mdb_env_close(self->dbEnv);

    /* Release pool. */
    afw_pool_release(instance->p, xctx);
}



/*
 * Implementation of method create_adaptor_session of interface afw_adaptor.
 */
const afw_adaptor_session_t *
impl_afw_adaptor_create_adaptor_session (
    const afw_adaptor_t * instance,
    afw_xctx_t *xctx)
{
    afw_lmdb_adaptor_t *self = (afw_lmdb_adaptor_t *)instance;

    return (const afw_adaptor_session_t *)
        afw_lmdb_adaptor_session_create(self, xctx);
}


/*
 * Implementation of method get_additional_metrics of interface afw_adaptor.
 */
const afw_object_t *
impl_afw_adaptor_get_additional_metrics (
    const afw_adaptor_t * instance,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    const afw_lmdb_adaptor_t *self = 
        (afw_lmdb_adaptor_t *) instance;
    int major, minor, patch;
    char *version_str;
    const afw_object_t *metrics;
    const afw_object_t *version;
    const afw_object_t *statistics;
    const afw_object_t *information;
    const afw_object_t *environment;
    const afw_object_t *database;
    MDB_stat stat;
    MDB_envinfo info;
    MDB_dbi dbi;
    MDB_txn *txn;
    int rc;

    /* Create additional metrics object and set object type. */
    metrics = afw_object_create_managed(p, xctx);
    afw_object_meta_set_object_type_id(metrics, 
        afw_lmdb_s__AdaptiveAdaptorMetrics_adaptor_lmdb,
        xctx);

    /** @fixme Jeremy

    You need to call this here with the name of an object type defined
    in afw_lmdb/object/_AdaptiveObjectType_/

    afw_object_meta_set_object_type_id(metrics,
        afw_lmdb_s_????,
        xctx);

    This object type can have the object type for all of the embedded
    objects -- version, statistics, and information.  These should also
    be defined in afw_lmdb/object/_AdaptiveObjectType_/.  You should
    also use afw_object_meta_set_object_type_id() to set the object type
    of these embedded objects.

    Use afw_object_create_embedded() instead of
    afw_object_create_managed() for the embedded ones. This creates the
    object and sets the property in the embedding object. So the
    afw_object_set_property_as_object() for each at the end
    can be removed.

    When this is done and you access /afw/_AdaptiveAdaptor_/... the
    "metrics.additional" property should have meta.
    
     */

    /* create our version object, which reports LMDB version information */
    version = afw_object_create_embedded(metrics, 
        afw_lmdb_s_version, xctx);

    afw_object_meta_set_object_type_id(version,
        afw_lmdb_s__AdaptiveAdaptorMetrics_adaptor_lmdb_version,
        xctx);

    version_str = mdb_version(&major, &minor, &patch);
    if (version_str) {
        afw_object_set_property_as_string_from_utf8_z(version, 
            afw_lmdb_s_version_string, version_str, xctx);
        afw_object_set_property(version, afw_lmdb_s_major,
            afw_value_create_unmanaged_integer(major, p, xctx), xctx);
        afw_object_set_property(version, afw_lmdb_s_minor,
            afw_value_create_unmanaged_integer(minor, p, xctx), xctx);
        afw_object_set_property(version, afw_lmdb_s_patch,
            afw_value_create_unmanaged_integer(patch, p, xctx), xctx);
    }

    /* gather statistics for each database */
    statistics = afw_object_create_embedded(metrics, 
        afw_lmdb_s_statistics, xctx);

    afw_object_meta_set_object_type_id(statistics,
        afw_lmdb_s__AdaptiveAdaptorMetrics_adaptor_lmdb_statistics,
        xctx);

    /* start with the environment stats */
    environment = afw_object_create_embedded(statistics,
        afw_lmdb_s_environment, xctx);

    afw_object_meta_set_object_type_id(environment,
        afw_lmdb_s__AdaptiveAdaptorMetrics_adaptor_lmdb_statistic,
        xctx);

    rc = mdb_env_stat(self->dbEnv, &stat);
    if (rc == 0) {
        afw_object_set_property(environment, afw_lmdb_s_psize, 
            afw_value_create_unmanaged_integer(
                stat.ms_psize, p, xctx), xctx);
        afw_object_set_property(environment, afw_lmdb_s_depth, 
            afw_value_create_unmanaged_integer(
                stat.ms_depth, p, xctx), xctx);
        afw_object_set_property(environment, afw_lmdb_s_branch_pages, 
            afw_value_create_unmanaged_integer(
                stat.ms_branch_pages, p, xctx), xctx);
        afw_object_set_property(environment, afw_lmdb_s_leaf_pages, 
            afw_value_create_unmanaged_integer(
                stat.ms_leaf_pages, p, xctx), xctx);
        afw_object_set_property(environment, afw_lmdb_s_overflow_pages, 
            afw_value_create_unmanaged_integer(
                stat.ms_overflow_pages, p, xctx), xctx);
        afw_object_set_property(environment, afw_lmdb_s_entries, 
            afw_value_create_unmanaged_integer(
                stat.ms_entries, p, xctx), xctx);
    }
   
    /* gather the env_info */
    information = afw_object_create_embedded(metrics, 
        afw_lmdb_s_information, xctx);

    afw_object_meta_set_object_type_id(information,
        afw_lmdb_s__AdaptiveAdaptorMetrics_adaptor_lmdb_information,
        xctx);

    rc = mdb_env_info(self->dbEnv, &info);
    if (rc == 0) {
        afw_object_set_property(information, afw_lmdb_s_mapaddr,
            afw_value_create_unmanaged_integer((size_t)info.me_mapaddr, p, xctx), xctx);
        afw_object_set_property(information, afw_lmdb_s_mapsize,
            afw_value_create_unmanaged_integer(
                info.me_mapsize, p, xctx), xctx);
        afw_object_set_property(information, afw_lmdb_s_last_pgno,
            afw_value_create_unmanaged_integer(
                info.me_last_pgno, p, xctx), xctx);
        afw_object_set_property(information, afw_lmdb_s_last_txnid,
            afw_value_create_unmanaged_integer(
                info.me_last_txnid, p, xctx), xctx);
        afw_object_set_property(information, afw_lmdb_s_maxreaders,
            afw_value_create_unmanaged_integer(
                info.me_maxreaders, p, xctx), xctx);
        afw_object_set_property(information, afw_lmdb_s_numreaders,
            afw_value_create_unmanaged_integer(
                info.me_numreaders, p, xctx), xctx);
    }

    apr_thread_rwlock_rdlock(self->dbLock);

    rc = mdb_txn_begin(self->dbEnv, NULL, 0, &txn);
    if (rc) {
        apr_thread_rwlock_unlock(self->dbLock);

        AFW_THROW_ERROR_RV_Z(general, lmdb, rc,
            "Unable to begin initial transaction.", xctx);
    }

    rc = mdb_dbi_open(txn, NULL, 0, &dbi);
    if (rc == 0) {
        MDB_cursor *cursor;
        MDB_val key;
        const afw_utf8_t *database_str;

        rc = mdb_cursor_open(txn, dbi, &cursor);  
        if (rc == 0) {
            while (mdb_cursor_get(cursor, &key, NULL, MDB_NEXT_NODUP) == 0) 
            {
                char *str;
                MDB_dbi db2;
                if (memchr(key.mv_data, '\0', key.mv_size))
                    continue;
                str = afw_pool_malloc(p, key.mv_size+1, xctx);
                memcpy(str, key.mv_data, key.mv_size);
                str[key.mv_size] = '\0';

                database_str = afw_utf8_create(str, key.mv_size, p, xctx);

                db2 = afw_lmdb_internal_open_database(self, 
                    txn, database_str, 0, p, xctx);
                if (db2 == 0) continue;

                rc = mdb_stat(txn, db2, &stat);
                if (rc == 0) {
                    database = afw_object_create_embedded(statistics,
                        database_str, xctx); 

                    afw_object_meta_set_object_type_id(database,
                        afw_lmdb_s__AdaptiveAdaptorMetrics_adaptor_lmdb_statistic,
                        xctx);
                  
                    afw_object_set_property(database, afw_lmdb_s_psize,
                        afw_value_create_unmanaged_integer(
                            stat.ms_psize, p, xctx), xctx);
                    afw_object_set_property(database, afw_lmdb_s_depth,
                        afw_value_create_unmanaged_integer(
                            stat.ms_depth, p, xctx), xctx);
                    afw_object_set_property(database, afw_lmdb_s_branch_pages,
                        afw_value_create_unmanaged_integer(
                            stat.ms_branch_pages, p, xctx), xctx);
                    afw_object_set_property(database, afw_lmdb_s_leaf_pages,
                        afw_value_create_unmanaged_integer(
                            stat.ms_leaf_pages, p, xctx), xctx);
                    afw_object_set_property(database, afw_lmdb_s_overflow_pages,
                        afw_value_create_unmanaged_integer(
                            stat.ms_overflow_pages, p, xctx), xctx);
                    afw_object_set_property(database, afw_lmdb_s_entries,
                        afw_value_create_unmanaged_integer(
                            stat.ms_entries, p, xctx), xctx);
                }
            }

            mdb_cursor_close(cursor);
        }

        mdb_close(self->dbEnv, dbi);
    }

    /* FIXME check return code here and decide what to do/throw */
    mdb_txn_commit(txn);

    apr_thread_rwlock_unlock(self->dbLock);

    return metrics;
}
