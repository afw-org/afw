// See the 'COPYING' file in the project root for licensing information.
/*
 * Heap / heap-tracker pool internals.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_POOL_HEAP_INTERNAL_H__
#define __AFW_POOL_HEAP_INTERNAL_H__

#include "afw_interface.h"

/**
 * @file afw_pool_heap_internal.h
 * @brief Internals for afw_pool_heap.c only.
 *
 * Heap and heap tracker are single-thread only. Create, use, and
 * release on the same thread (normally one compiled_value evaluate).
 */

AFW_BEGIN_DECLARES

typedef struct afw_pool_internal_inf_implementation_specific_s {
    afw_boolean_t is_multithreaded;
    afw_boolean_t is_subpool;
} afw_pool_internal_inf_implementation_specific_t;

typedef struct afw_pool_internal_memory_prefix_s
afw_pool_internal_memory_prefix_t;

/**
 * @brief Memory prefix
 *
 * Prefix before each address returned by calloc/malloc for heap
 * implementations that do not keep a chain of allocated memory.
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

/**
 * @brief Memory prefix with links
 *
 * Prefix for tracker allocations that keep a chain so destroy can
 * return remaining blocks to the heap.
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
    (((char *)address) - \
        sizeof(afw_pool_internal_memory_prefix_with_links_t))


typedef struct afw_pool_internal_free_memory_s
afw_pool_internal_free_memory_t;

struct afw_pool_internal_free_memory_s {
    afw_pool_internal_free_memory_t *next;
    afw_size_t size;
    /* Free memory starts here. */
};


typedef struct afw_pool_internal_free_memory_head_s
afw_pool_internal_free_memory_head_t;

/** @brief Head of each free-memory list. */
struct afw_pool_internal_free_memory_head_s {
    afw_pool_internal_free_memory_t *first; /* This will go away. */
};

typedef struct afw_pool_internal_self_s
afw_pool_internal_self_t;

struct afw_pool_internal_self_s {

    afw_pool_t pub;

    /** @brief Unique number for pool. */
    afw_integer_t pool_number;

    /**
     * @brief Heap reservoir APR pool (current impl). Trackers share it.
     *
     * This is how the heap holds memory today (first-fit, else
     * apr_palloc). It is not the afw_pool_get_apr_pool() door. A
     * future heap might not be APR-backed.
     */
    apr_pool_t *apr_p;

    /**
     * @brief APR pool for afw_pool_get_apr_pool() callers, or NULL.
     *
     * Door for leftover APR function calls. NULL until first
     * get_apr_pool(). Heap: for now aliases apr_p. Tracker: first call
     * creates a child of the heap reservoir (not get_apr_pool(heap));
     * tracker destroy releases it. Never created if nobody calls.
     */
    apr_pool_t *public_apr_p;

    /** @brief Optional pool name. */
    const afw_utf8_t *name;

    /** @brief Parent heap when this is a tracker. */
    afw_pool_internal_self_t *parent;

    /**
     * @brief AFW parent when this is a heap (usually a general pool).
     */
    const afw_pool_t *external_parent;

    /** @brief First tracker child of this heap. */
    afw_pool_internal_self_t *first_child;

    /** @brief Next sibling tracker. */
    afw_pool_internal_self_t *next_sibling;

    /**
     * @brief Creating thread, or NULL on the process main thread (afw).
     *
     * Heap and tracker are still single-thread: do not use from another
     * thread even when this is NULL.
     */
    const afw_thread_t *thread;

    /** @brief First cleanup function. */
    afw_pool_cleanup_t *first_cleanup;

    /**
     * @brief Reference count.
     *
     * Starts at 1 on create; get_reference / release.
     */
    afw_integer_t reference_count;

    /** @brief Outstanding malloc/calloc (minus free/destroy). */
    afw_size_t bytes_allocated;

    /**
     * @brief First allocated memory.
     *
     * NULL if this implementation does not keep a chain.
     */
    afw_pool_internal_memory_prefix_with_links_t *first_allocated_memory;

    /**
     * @brief Free memory head.
     *
     * For trackers this is the same pointer as the parent heap.
     */
    afw_pool_internal_free_memory_head_t *free_memory_head;
};


typedef struct afw_pool_internal_self_with_free_memory_head_s
afw_pool_internal_self_with_free_memory_head_t;
struct afw_pool_internal_self_with_free_memory_head_s {

    afw_pool_internal_self_t common;

    /* Don't access this directly. Use free_memory_head pointer instead. */
    afw_pool_internal_free_memory_head_t memory_for_free_memory_head;
};

AFW_END_DECLARES

#endif /* __AFW_POOL_HEAP_INTERNAL_H__ */
