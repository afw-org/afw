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

typedef struct afw_pool_internal_allocated_scope_memory_s
afw_pool_internal_allocated_scope_memory_t;

/* Allocated memory block in scope pool. */
struct afw_pool_internal_allocated_memory_s {
    afw_pool_internal_allocated_scope_memory_t *next;
    afw_pool_internal_allocated_scope_memory_t *prev;
    afw_size_t size;
    /* Allocated memory starts here. */
};


typedef struct afw_pool_internal_free_memory_s
afw_pool_internal_free_memory_t;

/* Free memory block. */
struct afw_pool_internal_free_memory_s {
    afw_pool_internal_free_memory_t *next_free_memory;
    afw_size_t size;
    /* Free memory starts here. */
};


typedef struct afw_pool_internal_memory_block_s
afw_pool_internal_memory_block_t;

/* Free memory block. */
struct afw_pool_internal_memory_block_s {
    afw_size_t size;
    void *end_of_memory_block; /* For quick bounds check. */
    afw_pool_internal_memory_block_t *next_memory_block;
    afw_pool_internal_free_memory_t *first_free_memory;
    /* Memory for block starts here. */
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

    /*
     * This is first allocated memory for scope pool or the first free memory
     * block for others. Scope pool's memory is maintained in the parent. When a
     * scope pool is released, the parent's free method is called for everything
     * in the first_allocated_memory list.
     */
    union {
        afw_pool_internal_allocated_scope_memory_t *first_allocated_memory;
        afw_pool_internal_memory_block_t *first_memory_block;
    };

    /**
     * @brief Pools reference count.
     *
     * This starts at 1 on create and is incremented and decremented
     * by afw_pool_get_reference() and afw_pool_release().
     */
    afw_integer_t reference_count;

    /** @brief Bytes allocated via afw_pool_malloc()/afw_pool_calloc(). */
    afw_size_t bytes_allocated;

    /**
     * @brief Thread associated with a thread specific pool.
     *
     * If this is not NULL, this pool is thread specific and can only be
     * accessed by this thread.
     */
    const afw_thread_t *thread;
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


void afw_pool_print_debug_info(
    int indent,
    const afw_pool_t *pool,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_POOL_INTERNAL_H__ */
