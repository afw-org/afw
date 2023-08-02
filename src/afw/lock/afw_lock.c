// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework lock support
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_lock.c
 * @brief Lock support
 */

#include "afw_internal.h"

#ifdef AFW_LOCK_DEBUG
#undef afw_lock_obtain
#undef afw_lock_release
#undef afw_lock_read_obtain
#undef afw_lock_read_release
#undef afw_lock_write_obtain
#undef afw_lock_write_release
#endif


static void
impl_lock_destroy(
    void *data, void *data2, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_lock_t *self = (afw_lock_t *)data;

    /* Ignore errors. */
    apr_thread_mutex_destroy(self->mutex);
}


/* Create a lock. */
AFW_DEFINE(const afw_lock_t *)
afw_lock_create_environment_lock(
    const afw_utf8_t *lock_id,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_lock_t *self;
    apr_status_t rv;

    /*
     * Create environment lock instance.  Use apr_pcalloc since
     * afw_pool is not ready.
     */
    self = apr_pcalloc(afw_pool_get_apr_pool(p), sizeof(afw_lock_t));
    self->lock_id = (lock_id)
        ? lock_id
        : &afw_s_a_empty_string;
    rv = apr_thread_mutex_create(&self->mutex,
        APR_THREAD_MUTEX_UNNESTED, afw_pool_get_apr_pool(p));
    if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
            AFW_UTF8_FMT
            " apr_thread_mutex_create() failed",
            AFW_UTF8_FMT_ARG(self->lock_id));
    }

    /* Return new instance. */
    return self;
}



/* Create a lock and register in environment. */
AFW_DEFINE(const afw_lock_t *)
afw_lock_create_and_register(
    const afw_utf8_t *lock_id,
    const afw_utf8_t *brief,
    const afw_utf8_t *description,
    afw_boolean_t insure_recursive_lock,
    afw_xctx_t *xctx)
{
    const afw_lock_t *lock;

    lock = afw_lock_create(lock_id, brief, description,
        insure_recursive_lock, xctx->env->p, xctx);
    afw_environment_register_lock(lock->lock_id, lock, xctx);
    return lock;
}



/* Create a lock. */
AFW_DEFINE(const afw_lock_t *)
afw_lock_create(
    const afw_utf8_t *lock_id,
    const afw_utf8_t *brief,
    const afw_utf8_t *description,
    afw_boolean_t insure_recursive_lock,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_lock_t *self;
    apr_status_t rv;

#ifdef AFW_LOCK_DEBUG
    const afw_flag_t *flag;
#endif

    /*
     * Create new instance.  Use apr_pcalloc if env->environment_lock
     * is NULL.  This only happens during environment create.
     */
    self = afw_pool_calloc_type(p, afw_lock_t, xctx);
    self->lock_id = (lock_id)
        ? lock_id
        : afw_uuid_create_utf8(p, xctx);
    self->lock_type = (insure_recursive_lock)
        ? afw_lock_type_thread_recursive_mutex
        : afw_lock_type_thread_mutex;
    self->brief = brief;
    self->description = description;

#ifdef AFW_LOCK_DEBUG
    self->flag_id_debug = afw_utf8_printf(p, xctx,
        "debug:lock:" AFW_UTF8_FMT,
        AFW_UTF8_FMT_ARG(lock_id));
    flag = afw_environment_get_flag(self->flag_id_debug, xctx);
    if (!flag) {
        brief = afw_utf8_printf(p, xctx,
            "Debug lock " AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(lock_id));
        description = afw_utf8_printf(p, xctx,
            "Debug lock " AFW_UTF8_FMT ".",
            AFW_UTF8_FMT_ARG(lock_id));
        afw_environment_register_flag(
            self->flag_id_debug, brief, description,
            &afw_s_a_flag_debug_lock, xctx);
        flag = afw_environment_get_flag(self->flag_id_debug, xctx);
    }
    self->flag_index_debug = flag->flag_index;
#endif

    rv = apr_thread_mutex_create(&self->mutex,
        ((insure_recursive_lock)
            ? APR_THREAD_MUTEX_NESTED
            : APR_THREAD_MUTEX_DEFAULT),
        afw_pool_get_apr_pool(p));
    if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
            AFW_UTF8_FMT
            " apr_thread_mutex_create() failed",
            AFW_UTF8_FMT_ARG(self->lock_id));
    }

    /* Destroy instance before pool is destroyed. */
    afw_pool_register_cleanup_before(p,
        self, NULL, impl_lock_destroy, xctx);

    /* Return new instance. */
    return self;
}


static void
impl_lock_destroy_rw(
    void *data, void *data2, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_lock_rw_t *self = (afw_lock_rw_t *)data;

    /* Ignore errors. */
    apr_thread_rwlock_destroy(self->lock.rwlock);
}


/* Create a read/write lock and register in environment. */
AFW_DEFINE(const afw_lock_rw_t *)
afw_lock_create_rw_and_register(
    const afw_utf8_t *lock_id,
    const afw_utf8_t *brief,
    const afw_utf8_t *description,
    afw_xctx_t *xctx)
{
    const afw_lock_rw_t *rw;

    rw = afw_lock_create_rw(lock_id, brief, description,
        xctx->env->p, xctx);
    afw_environment_register_lock(rw->lock.lock_id, &rw->lock, xctx);
    return rw;
}


/* Create a read/write lock. */
AFW_DEFINE(const afw_lock_rw_t *)
afw_lock_create_rw(
    const afw_utf8_t *lock_id,
    const afw_utf8_t *brief,
    const afw_utf8_t *description,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_lock_rw_t *self;
    apr_status_t rv;

#ifdef AFW_LOCK_DEBUG
    const afw_flag_t *flag;
#endif

    /* Create new instance. */
    self = afw_pool_calloc_type(p, afw_lock_rw_t, xctx);
    self->lock.lock_id = (lock_id)
        ? lock_id
        : afw_uuid_create_utf8(p, xctx);
    self->lock.brief = brief;
    self->lock.description = description;

#ifdef AFW_LOCK_DEBUG
    self->lock.flag_id_debug = afw_utf8_printf(p, xctx,
        "debug:lock:" AFW_UTF8_FMT,
        AFW_UTF8_FMT_ARG(lock_id));
    flag = afw_environment_get_flag(self->lock.flag_id_debug, xctx);
    if (!flag) {
        brief = afw_utf8_printf(p, xctx,
            "Debug lock " AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(lock_id));
        description = afw_utf8_printf(p, xctx,
            "Debug lock " AFW_UTF8_FMT ".",
            AFW_UTF8_FMT_ARG(lock_id));
        afw_environment_register_flag(
            self->lock.flag_id_debug, brief, description,
            &afw_s_a_flag_debug_lock, xctx);
        flag = afw_environment_get_flag(self->lock.flag_id_debug, xctx);
    }
    self->lock.flag_index_debug = flag->flag_index;
#endif

    rv = apr_thread_rwlock_create(&self->lock.rwlock, afw_pool_get_apr_pool(p));
    if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
            AFW_UTF8_FMT
            " apr_thread_rwlock_create() failed",
            AFW_UTF8_FMT_ARG(self->lock.lock_id));
    }

    /* Destroy instance before pool is destroyed. */
    afw_pool_register_cleanup_before(p,
        self, NULL, impl_lock_destroy_rw, xctx);

    /* Return new instance. */
    return self;
}



/* Obtain lock. */
AFW_DEFINE(void)
afw_lock_obtain(const afw_lock_t *instance, afw_xctx_t *xctx)
{
    afw_lock_t *self = (afw_lock_t *)instance;
    apr_status_t rv;

    if (!instance) return;

    rv = apr_thread_mutex_lock(self->mutex);
    if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
            AFW_UTF8_FMT
            " apr_thread_mutex_lock() failed",
            AFW_UTF8_FMT_ARG(self->lock_id));
    }
}


/* Debug version of obtain lock. */
AFW_DEFINE(void)
afw_lock_obtain_debug(const afw_lock_t *instance,
    afw_xctx_t *xctx, const afw_utf8_z_t *source_z)
{
    if (!instance) return;

    if (afw_flag_is_active(instance->flag_index_debug, xctx)) {
        afw_debug_write_fz(NULL, source_z, xctx,
            "lock " AFW_UTF8_FMT ": afw_lock_obtain",
            AFW_UTF8_FMT_ARG(instance->lock_id));
    }

    /* Call non-debug function. */
    afw_lock_obtain(instance, xctx);
}


/* Release lock. */
AFW_DEFINE(void)
afw_lock_release(const afw_lock_t *instance, afw_xctx_t *xctx)
{
    afw_lock_t *self = (afw_lock_t *)instance;
    apr_status_t rv;

    if (!instance) return;

    rv = apr_thread_mutex_unlock(self->mutex);
    if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
            AFW_UTF8_FMT
            " apr_thread_mutex_unlock() failed",
            AFW_UTF8_FMT_ARG(self->lock_id));
    }
}


/* Debug version of release lock. */
AFW_DEFINE(void)
afw_lock_release_debug(const afw_lock_t *instance,
    afw_xctx_t *xctx, const afw_utf8_z_t *source_z)
{
    if (!instance) return;

    if (afw_flag_is_active(instance->flag_index_debug, xctx)) {
        afw_debug_write_fz(NULL, source_z, xctx,
            "lock " AFW_UTF8_FMT ": afw_lock_release",
            AFW_UTF8_FMT_ARG(instance->lock_id));
    }

    /* Call non-debug function. */
    afw_lock_release(instance, xctx);
}


/* Obtain read lock. */
AFW_DEFINE(void)
afw_lock_read_obtain(const afw_lock_rw_t *instance, afw_xctx_t *xctx)
{
    afw_lock_rw_t *self = (afw_lock_rw_t *)instance;
    apr_status_t rv;

    if (!instance) return;

    rv = apr_thread_rwlock_rdlock(self->lock.rwlock);
    if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
            AFW_UTF8_FMT
            " apr_thread_rwlock_rdlock() failed",
            AFW_UTF8_FMT_ARG(self->lock.lock_id));
    }
}


/* Debug version of obtain read lock. */
AFW_DEFINE(void)
afw_lock_read_obtain_debug(const afw_lock_rw_t *instance,
    afw_xctx_t *xctx, const afw_utf8_z_t *source_z)
{
    if (!instance) return;

    if (afw_flag_is_active(instance->lock.flag_index_debug, xctx))
    {
        afw_debug_write_fz(NULL, source_z, xctx,
            "lock " AFW_UTF8_FMT ": afw_lock_read_obtain",
            AFW_UTF8_FMT_ARG(instance->lock.lock_id));
    }

    /* Call non-debug function. */
    afw_lock_read_obtain(instance, xctx);
}


/* Release read lock. */
AFW_DEFINE(void)
afw_lock_read_release(const afw_lock_rw_t *instance, afw_xctx_t *xctx)
{
    afw_lock_rw_t *self = (afw_lock_rw_t *)instance;
    apr_status_t rv;

    if (!instance) return;

    rv = apr_thread_rwlock_unlock(self->lock.rwlock);
    if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
            AFW_UTF8_FMT
            " apr_thread_rwlock_unlock() failed",
            AFW_UTF8_FMT_ARG(self->lock.lock_id));
    }
}


/* Debug version of release read lock. */
AFW_DEFINE(void)
afw_lock_read_release_debug(const afw_lock_rw_t *instance,
    afw_xctx_t *xctx, const afw_utf8_z_t *source_z)
{
    if (!instance) return;

    if (afw_flag_is_active(instance->lock.flag_index_debug, xctx)) {
        afw_debug_write_fz(NULL, source_z, xctx,
            "lock " AFW_UTF8_FMT ": afw_lock_read_release",
            AFW_UTF8_FMT_ARG(instance->lock.lock_id));
    }

    /* Call non-debug function. */
    afw_lock_read_release(instance, xctx);
}


/* Obtain write lock. */
AFW_DEFINE(void)
afw_lock_write_obtain(const afw_lock_rw_t *instance, afw_xctx_t *xctx)
{
    afw_lock_rw_t *self = (afw_lock_rw_t *)instance;
    apr_status_t rv;

    if (!instance) return;

    rv = apr_thread_rwlock_wrlock(self->lock.rwlock);
    if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
            AFW_UTF8_FMT
            " apr_thread_rwlock_wrlock() failed",
            AFW_UTF8_FMT_ARG(self->lock.lock_id));
    }
}


/* Debug version of obtain write lock. */
AFW_DEFINE(void)
afw_lock_write_obtain_debug(const afw_lock_rw_t *instance,
    afw_xctx_t *xctx, const afw_utf8_z_t *source_z)
{
    if (!instance) return;

    if (afw_flag_is_active(instance->lock.flag_index_debug, xctx)) {
        afw_debug_write_fz(NULL, source_z, xctx,
            "lock " AFW_UTF8_FMT ": afw_lock_write_obtain",
            AFW_UTF8_FMT_ARG(instance->lock.lock_id));
    }

    afw_lock_write_obtain(instance, xctx);
}

/* Release write lock. */
AFW_DEFINE(void)
afw_lock_write_release(const afw_lock_rw_t *instance, afw_xctx_t *xctx)
{
    afw_lock_rw_t *self = (afw_lock_rw_t *)instance;
    apr_status_t rv;

    if (!instance) return;

    rv = apr_thread_rwlock_unlock(self->lock.rwlock);
    if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
            AFW_UTF8_FMT
            " apr_thread_rwlock_unlock() failed",
            AFW_UTF8_FMT_ARG(self->lock.lock_id));
    }
}


/* Debug version of release write lock. */
AFW_DEFINE(void)
afw_lock_write_release_debug(const afw_lock_rw_t *instance,
    afw_xctx_t *xctx, const afw_utf8_z_t *source_z)
{
    if (!instance) return;

    if (afw_flag_is_active(instance->lock.flag_index_debug, xctx)) {
        afw_debug_write_fz(NULL, source_z, xctx,
            "lock " AFW_UTF8_FMT ": afw_lock_write_release",
            AFW_UTF8_FMT_ARG(instance->lock.lock_id));
    }

    afw_lock_write_release(instance, xctx);
}
