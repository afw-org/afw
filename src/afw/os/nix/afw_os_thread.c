// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework OS thread/mutex/rwlock for *nix (pthreads)
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_os_thread.c
 * @brief pthreads mutex, rwlock, and thread behind afw_os_*.
 *
 * Lifetime is the afw_pool passed at create (cleanup destroys the
 * pthread object). Nested is PTHREAD_MUTEX_RECURSIVE; unnested is
 * ERRORCHECK. Threads are joinable; join is the caller's job.
 *
 * pthread_* return the errno value, not -1 with errno set.
 */

#include "afw.h"
#include <pthread.h>
#include <errno.h>
#include <signal.h>


struct afw_os_mutex_s {
    pthread_mutex_t mutex;
    afw_boolean_t initialized;
};

struct afw_os_rwlock_s {
    pthread_rwlock_t rwlock;
    afw_boolean_t initialized;
};

struct afw_os_thread_s {
    pthread_t tid;
    afw_boolean_t created;
};


static void
impl_mutex_cleanup(
    void *data, void *data2, const afw_pool_t *p, afw_xctx_t *xctx)
{
    (void)data2;
    (void)p;
    (void)xctx;
    afw_os_mutex_destroy((afw_os_mutex_t *)data);
}


static void
impl_rwlock_cleanup(
    void *data, void *data2, const afw_pool_t *p, afw_xctx_t *xctx)
{
    (void)data2;
    (void)p;
    (void)xctx;
    afw_os_rwlock_destroy((afw_os_rwlock_t *)data);
}


AFW_DEFINE(afw_os_mutex_t *)
afw_os_mutex_create(
    unsigned int flags,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_os_mutex_t *self;
    pthread_mutexattr_t attr;
    int err;
    int kind;

    self = afw_pool_calloc_type(p, afw_os_mutex_t, xctx);

    if (flags == AFW_OS_MUTEX_NESTED) {
        kind = PTHREAD_MUTEX_RECURSIVE;
    }
    else if (flags == AFW_OS_MUTEX_UNNESTED) {
        kind = PTHREAD_MUTEX_ERRORCHECK;
    }
    else {
        kind = PTHREAD_MUTEX_DEFAULT;
    }

    err = pthread_mutexattr_init(&attr);
    if (err != 0) {
        AFW_THROW_ERROR_RV_Z(general, errno, err,
            "pthread_mutexattr_init() failed", xctx);
    }
    err = pthread_mutexattr_settype(&attr, kind);
    if (err != 0) {
        (void)pthread_mutexattr_destroy(&attr);
        AFW_THROW_ERROR_RV_Z(general, errno, err,
            "pthread_mutexattr_settype() failed", xctx);
    }
    err = pthread_mutex_init(&self->mutex, &attr);
    (void)pthread_mutexattr_destroy(&attr);
    if (err != 0) {
        AFW_THROW_ERROR_RV_Z(general, errno, err,
            "pthread_mutex_init() failed", xctx);
    }
    self->initialized = true;
    afw_pool_register_cleanup_before(p, self, NULL,
        impl_mutex_cleanup, xctx);
    return self;
}


AFW_DEFINE(void)
afw_os_mutex_lock(afw_os_mutex_t *mutex, afw_xctx_t *xctx)
{
    int err;

    if (!mutex || !mutex->initialized) {
        return;
    }
    err = pthread_mutex_lock(&mutex->mutex);
    if (err != 0) {
        AFW_THROW_ERROR_RV_Z(general, errno, err,
            "pthread_mutex_lock() failed", xctx);
    }
}


AFW_DEFINE(afw_boolean_t)
afw_os_mutex_trylock(afw_os_mutex_t *mutex, afw_xctx_t *xctx)
{
    int err;

    if (!mutex || !mutex->initialized) {
        return false;
    }
    err = pthread_mutex_trylock(&mutex->mutex);
    if (err == 0) {
        return true;
    }
    if (err == EBUSY) {
        return false;
    }
    AFW_THROW_ERROR_RV_Z(general, errno, err,
        "pthread_mutex_trylock() failed", xctx);
    return false;
}


AFW_DEFINE(void)
afw_os_mutex_unlock(afw_os_mutex_t *mutex, afw_xctx_t *xctx)
{
    int err;

    if (!mutex || !mutex->initialized) {
        return;
    }
    err = pthread_mutex_unlock(&mutex->mutex);
    if (err != 0) {
        AFW_THROW_ERROR_RV_Z(general, errno, err,
            "pthread_mutex_unlock() failed", xctx);
    }
}


AFW_DEFINE(void)
afw_os_mutex_destroy(afw_os_mutex_t *mutex)
{
    if (!mutex || !mutex->initialized) {
        return;
    }
    (void)pthread_mutex_destroy(&mutex->mutex);
    mutex->initialized = false;
}


AFW_DEFINE(afw_os_rwlock_t *)
afw_os_rwlock_create(
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_os_rwlock_t *self;
    int err;

    self = afw_pool_calloc_type(p, afw_os_rwlock_t, xctx);
    err = pthread_rwlock_init(&self->rwlock, NULL);
    if (err != 0) {
        AFW_THROW_ERROR_RV_Z(general, errno, err,
            "pthread_rwlock_init() failed", xctx);
    }
    self->initialized = true;
    afw_pool_register_cleanup_before(p, self, NULL,
        impl_rwlock_cleanup, xctx);
    return self;
}


AFW_DEFINE(void)
afw_os_rwlock_rdlock(afw_os_rwlock_t *rwlock, afw_xctx_t *xctx)
{
    int err;

    if (!rwlock || !rwlock->initialized) {
        return;
    }
    err = pthread_rwlock_rdlock(&rwlock->rwlock);
    if (err != 0) {
        AFW_THROW_ERROR_RV_Z(general, errno, err,
            "pthread_rwlock_rdlock() failed", xctx);
    }
}


AFW_DEFINE(void)
afw_os_rwlock_wrlock(afw_os_rwlock_t *rwlock, afw_xctx_t *xctx)
{
    int err;

    if (!rwlock || !rwlock->initialized) {
        return;
    }
    err = pthread_rwlock_wrlock(&rwlock->rwlock);
    if (err != 0) {
        AFW_THROW_ERROR_RV_Z(general, errno, err,
            "pthread_rwlock_wrlock() failed", xctx);
    }
}


AFW_DEFINE(void)
afw_os_rwlock_unlock(afw_os_rwlock_t *rwlock, afw_xctx_t *xctx)
{
    int err;

    if (!rwlock || !rwlock->initialized) {
        return;
    }
    err = pthread_rwlock_unlock(&rwlock->rwlock);
    if (err != 0) {
        AFW_THROW_ERROR_RV_Z(general, errno, err,
            "pthread_rwlock_unlock() failed", xctx);
    }
}


AFW_DEFINE(void)
afw_os_rwlock_destroy(afw_os_rwlock_t *rwlock)
{
    if (!rwlock || !rwlock->initialized) {
        return;
    }
    (void)pthread_rwlock_destroy(&rwlock->rwlock);
    rwlock->initialized = false;
}


AFW_DEFINE(afw_os_thread_t *)
afw_os_thread_create(
    afw_os_thread_start_t start,
    void *arg,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_os_thread_t *self;
    int err;

    self = afw_pool_calloc_type(p, afw_os_thread_t, xctx);
    err = pthread_create(&self->tid, NULL, start, arg);
    if (err != 0) {
        AFW_THROW_ERROR_RV_Z(general, errno, err,
            "pthread_create() failed", xctx);
    }
    self->created = true;
    return self;
}


AFW_DEFINE(void)
afw_os_thread_join(afw_os_thread_t *thread, afw_xctx_t *xctx)
{
    int err;
    void *retval;

    if (!thread || !thread->created) {
        return; /* already joined or never started */
    }
    err = pthread_join(thread->tid, &retval);
    if (err != 0) {
        AFW_THROW_ERROR_RV_Z(general, errno, err,
            "pthread_join() failed", xctx);
    }
    thread->created = false;
    (void)retval;
}


AFW_DEFINE(void)
afw_os_thread_kill(const afw_os_thread_t *thread, int signo)
{
    if (!thread || !thread->created) {
        return;
    }
    (void)pthread_kill(thread->tid, signo);
}
