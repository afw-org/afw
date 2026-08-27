// See the 'COPYING' file in the project root for licensing information.
/*
 * Heap / heap-tracker pool internals.
 *
 * Copyright (c) 2010-2026 Clemson University
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

typedef struct afw_pool_heap_chunk_s afw_pool_heap_chunk_t;

/**
 * @brief Heap/tracker allocation chunk
 *
 * One header for heap and tracker. size is the whole chunk. prev/next
 * are the tracker allocated list while live, or the heap free list
 * when freed. Callers pass the pool to free_memory(); there is no
 * pool pointer in the chunk.
 */
struct afw_pool_heap_chunk_s {
    afw_size_t size;
    afw_pool_heap_chunk_t *prev;
    afw_pool_heap_chunk_t *next;
    /* Payload starts here. */
};

#ifdef AFW_DEBUG_POOL
/**
 * Immediately before the user pointer on heap and tracker. Same
 * place so free uses one check.
 */
typedef struct afw_pool_debug_prefix_s {
    const afw_pool_t *pool;
    afw_size_t size;
} afw_pool_debug_prefix_t;
#define AFW_POOL_DEBUG_PREFIX_BYTES sizeof(afw_pool_debug_prefix_t)
#else
#define AFW_POOL_DEBUG_PREFIX_BYTES ((afw_size_t)0)
#endif

#define AFW_POOL_HEAP_CHUNK(address) \
    ((afw_pool_heap_chunk_t *)((char *)(address) - \
        AFW_POOL_DEBUG_PREFIX_BYTES - sizeof(afw_pool_heap_chunk_t)))

#define AFW_POOL_CHUNK_TO_USER(block) \
    ((void *)((char *)(block) + sizeof(afw_pool_heap_chunk_t) + \
        AFW_POOL_DEBUG_PREFIX_BYTES))


typedef struct afw_pool_internal_free_memory_head_s
afw_pool_internal_free_memory_head_t;

/** @brief Head of the heap free list. Trackers share the heap's. */
struct afw_pool_internal_free_memory_head_s {
    afw_pool_heap_chunk_t *first;
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
     * This is how the heap holds memory today (free list, else
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
     * @brief First allocated chunk (tracker only).
     *
     * NULL on the heap and when a tracker has no live mallocs.
     */
    afw_pool_heap_chunk_t *first_allocated_memory;

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
