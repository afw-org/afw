// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework OS thread/mutex/rwlock stubs for Windows
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_os_thread.c
 * @brief Windows stubs for afw_os mutex/rwlock/thread.
 */

#include "afw.h"


struct afw_os_mutex_s {
    int unused;
};

struct afw_os_rwlock_s {
    int unused;
};

struct afw_os_thread_s {
    int unused;
};


static void
impl_not_implemented(afw_xctx_t *xctx)
{
    AFW_THROW_ERROR_Z(general,
        "Windows thread/mutex is not implemented", xctx);
}


AFW_DEFINE(afw_os_mutex_t *)
afw_os_mutex_create(
    unsigned int flags,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    (void)flags;
    (void)p;
    impl_not_implemented(xctx);
    return NULL;
}


AFW_DEFINE(void)
afw_os_mutex_lock(afw_os_mutex_t *mutex, afw_xctx_t *xctx)
{
    (void)mutex;
    impl_not_implemented(xctx);
}


AFW_DEFINE(afw_boolean_t)
afw_os_mutex_trylock(afw_os_mutex_t *mutex, afw_xctx_t *xctx)
{
    (void)mutex;
    impl_not_implemented(xctx);
    return false;
}


AFW_DEFINE(void)
afw_os_mutex_unlock(afw_os_mutex_t *mutex, afw_xctx_t *xctx)
{
    (void)mutex;
    impl_not_implemented(xctx);
}


AFW_DEFINE(void)
afw_os_mutex_destroy(afw_os_mutex_t *mutex)
{
    (void)mutex;
}


AFW_DEFINE(afw_os_rwlock_t *)
afw_os_rwlock_create(
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    (void)p;
    impl_not_implemented(xctx);
    return NULL;
}


AFW_DEFINE(void)
afw_os_rwlock_rdlock(afw_os_rwlock_t *rwlock, afw_xctx_t *xctx)
{
    (void)rwlock;
    impl_not_implemented(xctx);
}


AFW_DEFINE(void)
afw_os_rwlock_wrlock(afw_os_rwlock_t *rwlock, afw_xctx_t *xctx)
{
    (void)rwlock;
    impl_not_implemented(xctx);
}


AFW_DEFINE(void)
afw_os_rwlock_unlock(afw_os_rwlock_t *rwlock, afw_xctx_t *xctx)
{
    (void)rwlock;
    impl_not_implemented(xctx);
}


AFW_DEFINE(void)
afw_os_rwlock_destroy(afw_os_rwlock_t *rwlock)
{
    (void)rwlock;
}


AFW_DEFINE(afw_os_thread_t *)
afw_os_thread_create(
    afw_os_thread_start_t start,
    void *arg,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    (void)start;
    (void)arg;
    (void)p;
    impl_not_implemented(xctx);
    return NULL;
}


AFW_DEFINE(void)
afw_os_thread_join(afw_os_thread_t *thread, afw_xctx_t *xctx)
{
    (void)thread;
    impl_not_implemented(xctx);
}


AFW_DEFINE(void)
afw_os_thread_kill(const afw_os_thread_t *thread, int signo)
{
    (void)thread;
    (void)signo;
}
