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

typedef struct afw_pool_internal_memory_s
afw_pool_internal_memory_t;

/* Allocated memory block in scope pool. */
struct afw_pool_internal_memory_s {
    afw_pool_internal_memory_t *next;
    afw_pool_internal_memory_t *prev;
    afw_size_t size;
    /* Allocated memory starts here. */
};


typedef struct afw_pool_internal_self_s
afw_pool_internal_self_t;

struct afw_pool_internal_self_s {

    afw_pool_t pub;

    /*NOTE: Keep pool_number just after pub for debugging. */
    /** @brief Unique number for pool. */
    afw_integer_t pool_number;

    /** @brief Associated apr pool or NULL if it has not been created. */
    apr_pool_t *apr_p;

    /** @brief Optional pool name. */
    const afw_utf8_t *name;

    /** @brief Parent pool of this pool. */
    afw_pool_internal_self_t *parent;

    /* @brief First subpool of this pool. */
    afw_pool_internal_self_t *first_child;

    /* @brief Next sibling of this pool. */
    afw_pool_internal_self_t *next_sibling;

    /** @brief First cleanup function. */
    afw_pool_cleanup_t *first_cleanup;

    /**
     * @brief Pools reference count.
     *
     * This starts at 1 on create and is incremented and decremented
     * by afw_pool_get_reference() and afw_pool_release().
     */
    afw_integer_t reference_count;

    /** @brief Bytes allocated via afw_pool_malloc()/afw_pool_calloc(). */
    afw_size_t bytes_allocated;

    /*
     * This is the head of the allocated memory in the pool except for the
     * unmanaged pool implementation.
     *
     * When the next pointer is NULL, the
     * AFW_POOL_INTERNAL_POOL_SELF_WITH_HEAD_ALLOCATED_MEMORY() macro can be
     * used to get the address of the pool's self.
     */
    afw_pool_internal_memory_t head_allocated_memory;

    /*
     * This is the head of the free memory in the pool for the managed and the
     * multithreaded pool implementations. Pool type unmanaged ignores the
     * free() method. Pool type subpool and multithread_subpool return freed
     * memory to their parent pool.
     *
     * When the next pointer is NULL, the
     * AFW_POOL_INTERNAL_POOL_SELF_WITH_HEAD_FREE_MEMORY() macro can be used to
     * get the address of the pool's self.
     */
    afw_pool_internal_memory_t head_free_memory;

    /**
     * @brief Thread associated with a thread specific pool.
     *
     * If this is not NULL, this pool is thread specific and can only be
     * accessed by this thread.
     */
    const afw_thread_t *thread;
};


#define AFW_POOL_INTERNAL_POOL_SELF_WITH_HEAD_ALLOCATED_MEMORY(head) \
    (afw_pool_internal_self_t *) \
    ((char *)head - offsetof(afw_pool_internal_self_t, head_allocated_memory))


#define AFW_POOL_INTERNAL_POOL_SELF_WITH_HEAD_FREE_MEMORY(head) \
    (afw_pool_internal_self_t *) \
    ((char *)head - offsetof(afw_pool_internal_self_t, head_free_memory))


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


void afw_pool_print_debug_info(
    int indent,
    const afw_pool_t *pool,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_POOL_INTERNAL_H__ */
