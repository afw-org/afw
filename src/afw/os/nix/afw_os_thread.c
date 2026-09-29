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

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include "afw_internal.h"
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <unistd.h>


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


static int
impl_mutex_init(afw_os_mutex_t *self, unsigned int flags)
{
    pthread_mutexattr_t attr;
    int err;
    int kind;

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
        return err;
    }
    err = pthread_mutexattr_settype(&attr, kind);
    if (err != 0) {
        (void)pthread_mutexattr_destroy(&attr);
        return err;
    }
    err = pthread_mutex_init(&self->mutex, &attr);
    (void)pthread_mutexattr_destroy(&attr);
    return err;
}


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
    int err;

    self = afw_pool_calloc_type(p, afw_os_mutex_t, xctx);

    err = impl_mutex_init(self, flags);
    if (err != 0) {
        AFW_THROW_ERROR_RV_Z(general, errno, err,
            "pthread_mutex_init() failed", xctx);
    }
    self->initialized = true;
    afw_pool_register_cleanup(p, self, NULL,
        impl_mutex_cleanup, xctx);
    return self;
}


AFW_DEFINE(afw_os_mutex_t *)
afw_os_mutex_create_unhandled(unsigned int flags)
{
    afw_os_mutex_t *self;

    self = (afw_os_mutex_t *)calloc(1, sizeof(afw_os_mutex_t));
    if (!self) {
        return NULL;
    }
    if (impl_mutex_init(self, flags) != 0) {
        free(self);
        return NULL;
    }
    self->initialized = true;
    return self;
}


AFW_DEFINE(void)
afw_os_mutex_free_unhandled(afw_os_mutex_t *mutex)
{
    if (!mutex) {
        return;
    }
    afw_os_mutex_destroy(mutex);
    free(mutex);
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
    afw_pool_register_cleanup(p, self, NULL,
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
    pthread_attr_t attr;
    size_t stack_size;
    size_t want;
    afw_size_t headroom;
    struct rlimit rl;
    int err;

    self = afw_pool_calloc_type(p, afw_os_thread_t, xctx);

    /*
     * Size thread stacks to max(2MiB, 4 x limitCStackHeadroomBytes,
     * RLIMIT_STACK). glibc already defaults to RLIMIT_STACK (2MiB on
     * x86_64 when unlimited); musl defaults to ~128KiB, which the C
     * stack headroom check trips on right away.
     *
     * The floor only applies when ulimit -s is lower or unlimited.
     * A full default evaluation stack (500) used ~500KiB of C stack
     * unoptimized, so with the 256KiB default headroom 2MiB leaves
     * ~3x margin for sanitizers and deeper non-eval recursion. The
     * headroom term keeps a raised limitCStackHeadroomBytes from
     * tripping on every check. Raise ulimit -s for more.
     */
    err = pthread_attr_init(&attr);
    if (err != 0) {
        AFW_THROW_ERROR_RV_Z(general, errno, err,
            "pthread_attr_init() failed", xctx);
    }
    want = 2 * 1024 * 1024;
    headroom = (xctx && xctx->env)
        ? xctx->env->limit_c_stack_headroom_bytes : 0;
    if (headroom <= AFW_SIZE_T_MAX / 4 && headroom * 4 > want) {
        want = (size_t)(headroom * 4);
    }
    if (getrlimit(RLIMIT_STACK, &rl) == 0 &&
        rl.rlim_cur != RLIM_INFINITY &&
        (size_t)rl.rlim_cur > want)
    {
        want = (size_t)rl.rlim_cur;
    }
    if (pthread_attr_getstacksize(&attr, &stack_size) != 0 ||
        stack_size < want)
    {
        err = pthread_attr_setstacksize(&attr, want);
        if (err != 0) {
            pthread_attr_destroy(&attr);
            AFW_THROW_ERROR_RV_Z(general, errno, err,
                "pthread_attr_setstacksize() failed", xctx);
        }
    }

    err = pthread_create(&self->tid, &attr, start, arg);
    pthread_attr_destroy(&attr);
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


AFW_DEFINE(void)
afw_os_c_stack_bounds(void **base, afw_size_t *size)
{
    pthread_attr_t attr;
    void *addr;
    size_t nbytes;
    struct rlimit rl;
    int err;

    if (base) {
        *base = NULL;
    }
    if (size) {
        *size = 0;
    }
    err = pthread_getattr_np(pthread_self(), &attr);
    if (err != 0) {
        return;
    }
    err = pthread_attr_getstack(&attr, &addr, &nbytes);
    pthread_attr_destroy(&attr);
    if (err != 0 || !addr || nbytes == 0) {
        return;
    }

    /*
     * musl reports an unreliable, much-too-small main-thread stack
     * size (observed ~130KiB against an 8MiB RLIMIT_STACK), while
     * its high end (addr + nbytes) tracks actual usage closely. When
     * the process stack limit is bigger, keep that high end but
     * rebase the low end on RLIMIT_STACK instead of musl's size.
     *
     * Main thread only. RLIMIT_STACK sizes the main thread's stack;
     * other threads have their real pthread size (musl's default is
     * ~128KiB), and rebasing those would put the low bound below the
     * actual stack so the headroom check could never trip.
     */
    if (getpid() == (pid_t)syscall(SYS_gettid) &&
        getrlimit(RLIMIT_STACK, &rl) == 0 &&
        rl.rlim_cur != RLIM_INFINITY &&
        (afw_size_t)rl.rlim_cur > (afw_size_t)nbytes)
    {
        char *high = (char *)addr + nbytes;
        addr = high - rl.rlim_cur;
        nbytes = (size_t)rl.rlim_cur;
    }

    if (base) {
        *base = addr;
    }
    if (size) {
        *size = (afw_size_t)nbytes;
    }
}
