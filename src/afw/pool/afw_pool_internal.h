// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework memory pool internal header.
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_POOL_INTERNAL_H__
#define __AFW_POOL_INTERNAL_H__

#include "afw_interface.h"

/**
 * @defgroup afw_pool_internal Pool
 * @ingroup afw_c_api_internal
 *
 * Pool internal API
 *
 * @{
 */

/**
 * @file afw_pool.h
 * @brief Adaptive Framework memory pool internal header.
 */

AFW_BEGIN_DECLARES

typedef struct afw_pool_internal_multithreaded_self_s
afw_pool_internal_multithreaded_self_t;


struct afw_pool_internal_multithreaded_self_s {

    afw_pool_t pub;

    /** @brief Associated apr pool or NULL if it has not been created. */
    apr_pool_t *apr_p;

    /** @brief Optional pool name. */
    const afw_utf8_t *name;

    /** @brief Parent pool of this pool. */
    afw_pool_internal_multithreaded_self_t *parent;

    /* @brief First subpool of this pool. */
    afw_pool_internal_multithreaded_self_t * AFW_ATOMIC first_child;

    /* @brief Next sibling of this pool. */
    afw_pool_internal_multithreaded_self_t * AFW_ATOMIC next_sibling;

    /**
     * @brief Pools reference count.
     *
     * This starts at 1 on create and is incremented and decremented
     * by afw_pool_add_reference() and afw_pool_release().
     */
    AFW_ATOMIC afw_integer_t reference_count;

    /** @brief First cleanup function. */
    afw_pool_cleanup_t *first_cleanup;

    /** @brief Bytes allocated via afw_pool_malloc()/afw_pool_calloc(). */
    AFW_ATOMIC afw_size_t bytes_allocated;

    /** @brief Unique number for pool. */
    afw_integer_t pool_number;

};


typedef struct afw_pool_internal_singlethreaded_self_s
afw_pool_internal_singlethreaded_self_t;


struct afw_pool_internal_singlethreaded_self_s {

    afw_pool_t pub;

    /** @brief Associated apr pool or NULL if it has not been created. */
    apr_pool_t *apr_p;

    /** @brief Optional pool name. */
    const afw_utf8_t *name;

    /** @brief Parent pool of this pool. */
    afw_pool_internal_singlethreaded_self_t *parent;

    /* @brief First subpool of this pool. */
    afw_pool_internal_singlethreaded_self_t *first_child;

    /* @brief Next sibling of this pool. */
    afw_pool_internal_singlethreaded_self_t *next_sibling;

    /**
     * @brief Pools reference count.
     *
     * This starts at 1 on create and is incremented and decremented
     * by afw_pool_add_reference() and afw_pool_release().
     */
    afw_integer_t reference_count;

    /**
     * @brief Thread associated with a thread specific pool.
     *
     * If this is not NULL, this pool is thread specific and can only be
     * accessed by this thread.
     */
    const afw_thread_t *thread;

    /** @brief First cleanup function. */
    afw_pool_cleanup_t *first_cleanup;

    /** @brief Bytes allocated via afw_pool_malloc()/afw_pool_calloc(). */
    afw_size_t bytes_allocated;

    /** @brief Unique number for pool. */
    afw_integer_t pool_number;

};


/**
 * @internal
 * @brief Allocate base pool.
 * @return Pointer to base pool or NULL if there is an error.
 *
 * This should only be used in afw_environment_create().
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_internal_create_base_pool();


/**
 * @internal
 * @brief Create thread struct in new thread specific pool with p  set.
 * @param size of thread struct or -1 if sizeof(afw_thread_t) should be used.
 * @param xctx of caller.
 * @return new thread struct with p set.
 *
 * This should only be called from afw_thread_create().
 */
AFW_DECLARE(afw_thread_t *)
afw_pool_internal_create_thread(
    afw_size_t size,
    afw_xctx_t *xctx);


/**
 * @internal
 * @brief Debug version of create thread struct in new thread specific pool
 *    with p set.
 * @param size of thread struct or -1 if sizeof(afw_thread_t) should be used.
 * @param xctx of caller.
 * @param source_z file:line where function called.
 * @return new thread struct with p set.
 *
 * This should only be called from afw_thread_create().
 */
AFW_DECLARE(afw_thread_t *)
afw_pool_internal_create_thread_debug(
    afw_size_t size,
    afw_xctx_t *xctx,
    const afw_utf8_z_t *source_z);


#ifdef AFW_POOL_DEBUG
#define afw_pool_internal_create_thread(size,xctx) \
    afw_pool_internal_create_thread_debug(size, xctx, AFW__FILE_LINE__)
#endif

AFW_END_DECLARES

/** @} */

#endif /* __AFW_POOL_INTERNAL_H__ */
