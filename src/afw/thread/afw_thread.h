// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework thread support header.
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_THREAD_H__
#define __AFW_THREAD_H__


/** @addtogroup afw_c_api
* @{
 */

/** @addtogroup afw_pool Pool
*
 * Pool support
*
* @{
 */

/**
 * @file afw_pool.h
 * @brief Adaptive Framework memory pool support header.
 */

#include "afw_interface.h"

AFW_BEGIN_DECLARES

/** @brief Uses apr_thread_mutex_t asis. */
#define afw_thread_mutex_create apr_thread_mutex_create

/** @brief Uses apr_thread_mutex_t asis. */
#define afw_thread_mutex_lock apr_thread_mutex_lock

/** @brief Uses apr_thread_mutex_trylock asis. */
#define afw_thread_mutex_trylock apr_thread_mutex_trylock

/** @brief Uses apr_thread_mutex_unlock asis. */
#define afw_thread_mutex_unlock apr_thread_mutex_unlock

/** @brief Uses apr_thread_mutex_destroy asis. */
#define afw_thread_mutex_destroy apr_thread_mutex_destroy

/** @brief Uses APR_THREAD_FUNC as AFW_THREAD_FUNCTION. */
#define AFW_THREAD_FUNCTION APR_THREAD_FUNC

/** @brief Typedef for thread start function. */
typedef void *(AFW_THREAD_FUNCTION *afw_thread_function_t)
    (const afw_thread_t *thread, void *arg);

/** @brief Typedef for afw_thread_attr. */
typedef struct afw_thread_attr_s afw_thread_attr_t;

/** @brief Struct for public part of afw_pool_t. */
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

    /** @brief The associated apr thread. */
    apr_thread_t *apr_thread;
};


/**
 * @brief Create a thread attr
 * @param p to use.
 * @param xctx of caller.
 * @return new thread attr.
 */
AFW_DECLARE(afw_thread_attr_t *)
afw_thread_attr_create(
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Create a thread
 * @param thread_attr to use.
 * @param start_function to call when thread starts.
 * @param start_function_arg to pass to start function.
 * @param name to be associated with thread.
 * @param thread_number to be associated with thread.
 * @param p to use.
 * @param xctx of caller.
 * @return new threadr.
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
 * @brief Join a thread
 * @param thread to join.
 * @param xctx of caller.
 * @return new threadr.
 */
AFW_DECLARE(void)
afw_thread_join(
    const afw_thread_t *thread,
    afw_xctx_t *xctx);


/**
 * @brief Macro to begin a mutex lock section.
 * @param xctx of caller.
 *
 * Usage:
 *
 * AFW_THREAD_MUTEX_LOCK(xctx) {
 *    ... a very small amount of code that doesn't call anything
 * }
 * AFW_THREAD_MUTEX_UNLOCK();
 */
#define AFW_THREAD_MUTEX_LOCK(mutex, xctx) \
do { \
    afw_thread_mutex_t *this_mutex; \
    this_mutex = mutex; \
    afw_thread_mutex_lock(mutex); \
    AFW_TRY



/**
 * @brief Macro to end a mutex lock.
 *
 * See AFW_THREAD_MUTEX_LOCK for usage.
 */
#define AFW_THREAD_MUTEX_UNLOCK() \
AFW_FINALLY { \
    afw_thread_mutex_unlock(this_mutex); \
} \
AFW_ENDTRY; \
} while(0)


AFW_END_DECLARES

/** @} */
/** @} */

#endif /* __AFW_THREAD_H__ */
