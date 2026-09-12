// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework thread support header.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_THREAD_H__
#define __AFW_THREAD_H__

#include "afw_interface.h"
#include "afw_os.h"

/**
 * @addtogroup afw_thread
 * @{
 */

/**
 * @file afw_thread.h
 * @brief Thread create/join helpers and attributes.
 *
 * See @ref afw_thread. Follow pool and xctx rules when sharing data across
 * threads; many AFW APIs are single-xctx unless documented otherwise.
 */

AFW_BEGIN_DECLARES

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

    /** @brief The thread specific pool for the thread. */
    const afw_pool_t *p;

    /** @brief The base xctx for the thread. */
    afw_xctx_t *xctx;

    /** @brief Native OS thread (joinable). */
    afw_os_thread_t *os_thread;
};


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
