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

typedef struct afw_pool_internal_memory_prefix_s
afw_pool_internal_memory_prefix_t;

/**
 * @brief Memory prefix
 * 
 * This is the prefix before each address returned by afw_pool_calloc()
 * and afw_pool_malloc() for implementations that do not keep a chain of
 * allocated memory.
 */
struct afw_pool_internal_memory_prefix_s {
    afw_size_t size;
    const afw_pool_t *p;
    /* Allocated/free memory starts here. */
};

#define AFW_POOL_INTERNAL_MEMORY_PREFIX(address) \
    (afw_pool_internal_memory_prefix_t *) \
    (((char *)address) - sizeof(afw_pool_internal_memory_prefix_t))

typedef struct afw_pool_internal_memory_prefix_with_links_s
afw_pool_internal_memory_prefix_with_links_t;

/*
 * @brief Memory prefix with links
 *
 * This is the prefix before each address returned by afw_pool_calloc()
 * and afw_pool_malloc() for implementations that keep up with a chain of
 * allocated memory like the subpool* implementations.
 */
struct afw_pool_internal_memory_prefix_with_links_s {
    afw_pool_internal_memory_prefix_with_links_t *prev;
    afw_pool_internal_memory_prefix_with_links_t *next;
    /* Common prefix must always be at end. */
    afw_pool_internal_memory_prefix_t common;
    /* Allocated/free memory starts here. */
};

#define AFW_POOL_INTERNAL_MEMORY_PREFIX_WITH_LINKS(address) \
    (afw_pool_internal_memory_prefix_with_links_t *) \
    (((char *)address) - sizeof(afw_pool_internal_memory_prefix_with_links_t))


typedef struct afw_pool_internal_free_memory_s
afw_pool_internal_free_memory_t;

struct afw_pool_internal_free_memory_s {
    afw_pool_internal_free_memory_t *next;
    afw_size_t size;
    /* Free memory starts here. */
};


typedef struct afw_pool_internal_free_memory_head_s
afw_pool_internal_free_memory_head_t;
/* @brief The head of each free memory free_block. */
struct afw_pool_internal_free_memory_head_s {
    afw_pool_internal_free_memory_t *first; /* This will go away. */
};

typedef struct afw_pool_internal_self_s
afw_pool_internal_self_t;

struct afw_pool_internal_self_s {

    afw_pool_t pub;

    /** @brief Unique number for pool. */
    afw_integer_t pool_number;

    /** @brief Associated apr pool. This might be the same a parent's. */
    apr_pool_t *apr_p;

    /**
     * @brief Public apr pool or NULL if not exposed yet.
     * 
     * This is set the first time when afw_pool_get_apr_pool() is called.
     * If this pool allocates from parent, a new pool is created if needed. If
     * not, public_apr_p is set to apr_p.
     */
    apr_pool_t *public_apr_p;

    /** @brief Optional pool name. */
    const afw_utf8_t *name;

    /** @brief Parent pool of this pool. */
    afw_pool_internal_self_t *parent;

    /* @brief First subpool of this pool. */
    afw_pool_internal_self_t *first_child;

    /* @brief Next sibling of this pool. */
    afw_pool_internal_self_t *next_sibling;

    /**
     * @brief Thread associated with a thread specific pool.
     *
     * If this is not NULL, this pool is thread specific and can only be
     * accessed by this thread.
     */
    const afw_thread_t *thread;

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

    /**
     * @brief First allocated memory.
     * 
     * This will be NULL if implementation does not keep up with allocated
     * memory.
     */
    afw_pool_internal_memory_prefix_with_links_t *first_allocated_memory;

    /**
     * @brief Free memory head.
     * 
     * For subpool implementations, this is the same as the parent's.
     */
    afw_pool_internal_free_memory_head_t *free_memory_head;
};


typedef struct afw_pool_internal_self_with_free_memory_head_s
afw_pool_internal_self_with_free_memory_head_t;
struct afw_pool_internal_self_with_free_memory_head_s {

    afw_pool_internal_self_t common;

    /* Don't assess this directly. Use free_memory_head pointer instead. */
    afw_pool_internal_free_memory_head_t memory_for_free_memory_head;
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


AFW_DECLARE_INTERNAL(void)
afw_pool_print_debug_info(
    int indent,
    const afw_pool_t *pool,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_POOL_INTERNAL_H__ */
