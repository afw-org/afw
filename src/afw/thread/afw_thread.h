// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework thread support header.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_THREAD_H__
#define __AFW_THREAD_H__

#include "afw_minimal.h"

/**
 * @addtogroup afw_thread
 * @{
 */

/**
 * @file afw_thread.h
 * @brief Thread create/join helpers and attributes.
 *
 * See @ref afw_thread. Follow pool and xctx rules when sharing data
 * across threads; many AFW APIs are single-xctx unless documented
 * otherwise.
 */

AFW_BEGIN_DECLARES

/**
 * @brief Kind of AFW thread.
 *
 * `base` is the process main xctx (no pthread). `request` is
 * `afw_thread_create()` (joinable pthread). Worker is later.
 */
typedef enum afw_thread_type_e {
    afw_thread_type_base,
    afw_thread_type_request
} afw_thread_type_t;

/** @brief Struct for public part of afw_thread_t. */
struct afw_thread_s {
    afw_thread_attr_t *thread_attr;

    /** @brief The function called when the thread starts. */
    afw_thread_function_t start_function;

    /** @brief The arg passed to the start function. */
    void *start_function_arg;

    /** @brief The name passed on afw_thread_create(). */
    const afw_utf8_t *name;

    /** @brief The thread number within the afw environment. */
    afw_integer_t thread_number;

    /**
     * @brief Last pool id assigned on this thread.
     *
     * Debug identity is thread_number plus this count. Not atomic;
     * only this thread creates its pools.
     */
    afw_integer_t pool_number;

    /** @brief `base` or `request`. Not a NULL `xctx->thread`. */
    afw_thread_type_t type;

    /** @brief The thread specific pool for the thread. */
    const afw_pool_t *p;

    /** @brief The base xctx for the thread. */
    afw_xctx_t *xctx;

    /** @brief Native OS thread (joinable). NULL on base (no pthread). */
    afw_os_thread_t *os_thread;

    /**
     * @brief ST asked-for bytes on pools whose `pool->thread` is this.
     *
     * Updated from afw_pool when the pool is thread-owned. MT heaps
     * keep `pool->thread` NULL and do not count here.
     */
    afw_size_t pool_bytes_in_use;

    /** @brief High-water of pool_bytes_in_use. */
    afw_size_t peak_pool_bytes_in_use;

    /** @brief ST posix_memalign chunk bytes on thread-owned heaps. */
    afw_size_t pool_chunk_bytes;

    /** @brief High-water of pool_chunk_bytes. */
    afw_size_t peak_pool_chunk_bytes;

    /**
     * @brief Lowest C-stack address for this OS thread.
     *
     * Sampled at thread start. NULL if unknown; skip headroom checks.
     */
    void *c_stack_base;

    /** @brief C-stack size in bytes. 0 if unknown. */
    afw_size_t c_stack_size;
};

/**
 * @brief This xctx's ST asked-for bytes (thread current minus snap).
 *
 * Not a second running counter. Thread peak is the worker's life;
 * do not subtract snap from peak. Needs the complete `afw_thread_t`.
 */
#define afw_xctx_pool_bytes_in_use(xctx) \
    (((xctx)->thread && \
        (xctx)->thread->pool_bytes_in_use > \
            (xctx)->snap_pool_bytes_in_use) \
        ? ((xctx)->thread->pool_bytes_in_use - \
            (xctx)->snap_pool_bytes_in_use) \
        : (afw_size_t)0)

/**
 * @brief This xctx's ST chunk bytes (thread current minus snap).
 */
#define afw_xctx_pool_chunk_bytes(xctx) \
    (((xctx)->thread && \
        (xctx)->thread->pool_chunk_bytes > \
            (xctx)->snap_pool_chunk_bytes) \
        ? ((xctx)->thread->pool_chunk_bytes - \
            (xctx)->snap_pool_chunk_bytes) \
        : (afw_size_t)0)

/** @brief Platform default mutex (non-recursive on nix). */
#define AFW_THREAD_MUTEX_DEFAULT  AFW_OS_MUTEX_DEFAULT

/** @brief Recursive mutex (was APR_THREAD_MUTEX_NESTED). */
#define AFW_THREAD_MUTEX_NESTED   AFW_OS_MUTEX_NESTED

/** @brief Non-recursive mutex (was APR_THREAD_MUTEX_UNNESTED). */
#define AFW_THREAD_MUTEX_UNNESTED AFW_OS_MUTEX_UNNESTED

/** @brief Mutex/rwlock are `afw_os_*` on `afw_pool` (see afw_os.h). */
#define afw_thread_mutex_create  afw_os_mutex_create
#define afw_thread_mutex_lock    afw_os_mutex_lock
#define afw_thread_mutex_trylock afw_os_mutex_trylock
#define afw_thread_mutex_unlock  afw_os_mutex_unlock
#define afw_thread_mutex_destroy afw_os_mutex_destroy

#define afw_thread_rwlock_create  afw_os_rwlock_create
#define afw_thread_rwlock_rdlock  afw_os_rwlock_rdlock
#define afw_thread_rwlock_wrlock  afw_os_rwlock_wrlock
#define afw_thread_rwlock_unlock  afw_os_rwlock_unlock
#define afw_thread_rwlock_destroy afw_os_rwlock_destroy

/**
 * @brief Create a thread attr.
 * @param p to use.
 * @param xctx of caller.
 * @return new thread attr.
 *
 * Currently unused by create (joinable POSIX default). Kept so
 * callers can pass one; do not assume detach.
 */
AFW_DECLARE(afw_thread_attr_t *)
afw_thread_attr_create(
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Create a joinable thread.
 * @param thread_attr currently unused (joinable default).
 * @param start_function to call when the thread starts.
 * @param start_function_arg to pass to start function.
 * @param name to be associated with thread.
 * @param thread_number to be associated with thread.
 * @param xctx of caller.
 * @return new thread.
 */
AFW_DECLARE(const afw_thread_t *)
afw_thread_create(
    afw_thread_attr_t *thread_attr,
    afw_thread_function_t start_function,
    void *start_function_arg, 
    const afw_utf8_t *name,
    afw_integer_t thread_number,
    afw_xctx_t *xctx);


/**
 * @brief Join a thread.
 * @param thread to join.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_thread_join(
    const afw_thread_t *thread,
    afw_xctx_t *xctx);


/**
 * @brief Macro to begin a mutex lock section.
 * @param mutex to lock.
 * @param xctx of caller (must be named xctx).
 *
 * Usage:
 *
 * AFW_THREAD_MUTEX_LOCK(mutex, xctx) {
 *    ... a very small amount of code that doesn't call anything
 * }
 * AFW_THREAD_MUTEX_UNLOCK();
 */
#define AFW_THREAD_MUTEX_LOCK(mutex, xctx) \
do { \
    afw_thread_mutex_t *this_mutex; \
    this_mutex = mutex; \
    afw_thread_mutex_lock(this_mutex, xctx); \
    AFW_TRY



/**
 * @brief Macro to end a mutex lock.
 *
 * See AFW_THREAD_MUTEX_LOCK for usage.
 */
#define AFW_THREAD_MUTEX_UNLOCK() \
AFW_FINALLY { \
    afw_thread_mutex_unlock(this_mutex, xctx); \
} \
AFW_ENDTRY; \
} while(0)


AFW_END_DECLARES

/** @} */  // end of @addtogroup @addtogroup

#endif /* __AFW_THREAD_H__ */
