// See the 'COPYING' file in the project root for licensing information.
/*
 * Heap and tracker pool internals.
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

#ifndef __AFW_POOL_INTERNAL_H__
#define __AFW_POOL_INTERNAL_H__

#include "afw_interface.h"

/**
 * @file afw_pool_internal.h
 * @brief Pool internals (heap and tracker).
 *
 * A pool is a heap unless it is a tracker. A tracker gets memory
 * from a heap, tracks live USER blocks, and returns them to the
 * heap on free or tracker destroy. The heap owns the free list.
 *
 * USER `size` is always the malloc/free_memory argument.
 *
 * Heap live: [USER] or, if AFW_DEBUG_POOL, [size][pool][USER].
 * Tracker live: [prev][next][size][USER] or, if AFW_DEBUG_POOL,
 * [prev][next][size][pool][USER]. With debug, [size][pool] is
 * immediately before USER on both.
 *
 * Freed blocks overlay afw_pool_free_node_t at the block start.
 */

AFW_BEGIN_DECLARES

typedef struct afw_pool_internal_inf_implementation_specific_s {
    afw_boolean_t is_multithreaded;
    afw_boolean_t is_tracker;
} afw_pool_internal_inf_implementation_specific_t;

#ifdef AFW_DEBUG_POOL
/** Immediately before USER on heap and tracker. */
typedef struct afw_pool_debug_prefix_s {
    afw_size_t size;
    const afw_pool_t *pool;
} afw_pool_debug_prefix_t;
#define AFW_POOL_DEBUG_PREFIX_BYTES sizeof(afw_pool_debug_prefix_t)
/*
 * USER fill on free. First word is a value `inf` pointer: non-canonical
 * on x86-64 so any `value->inf->…` faults, not only optional_release.
 */
#ifdef __LP64__
#define AFW_POOL_DEBUG_POISON \
    ((afw_size_t)0x0BADF00D0BADF00DULL)
#else
#define AFW_POOL_DEBUG_POISON ((afw_size_t)0x0BADF00Du)
#endif
#else
#define AFW_POOL_DEBUG_PREFIX_BYTES ((afw_size_t)0)
#endif

typedef struct afw_pool_tracker_node_s afw_pool_tracker_node_t;
struct afw_pool_tracker_node_s {
    /* Doubly linked for now. Forward-only + a later GC walk is enough;
     * still tiny vs an APR pool per scope. Do not change now. */
    afw_pool_tracker_node_t *prev;
    afw_pool_tracker_node_t *next;
#ifdef AFW_DEBUG_POOL
    afw_pool_debug_prefix_t debug;
#else
    afw_size_t size;
#endif
};

#define AFW_POOL_TRACKER_PREFIX_BYTES sizeof(afw_pool_tracker_node_t)

#define AFW_POOL_TRACKER_NODE(user) \
    ((afw_pool_tracker_node_t *)((char *)(user) - \
        AFW_POOL_TRACKER_PREFIX_BYTES))

#define AFW_POOL_TRACKER_TO_USER(node) \
    ((void *)((char *)(node) + AFW_POOL_TRACKER_PREFIX_BYTES))

#ifdef AFW_DEBUG_POOL
#define AFW_POOL_TRACKER_USER_SIZE(node) ((node)->debug.size)
#else
#define AFW_POOL_TRACKER_USER_SIZE(node) ((node)->size)
#endif

#define AFW_POOL_HEAP_ALLOC_START(user) \
    ((void *)((char *)(user) - AFW_POOL_HEAP_PREFIX_BYTES))

#define AFW_POOL_HEAP_USER_FROM_START(start) \
    ((void *)((char *)(start) + AFW_POOL_HEAP_PREFIX_BYTES))

/** Free-list overlay at the start of a freed block. `total` is the whole block. */
typedef struct afw_pool_free_node_s afw_pool_free_node_t;
struct afw_pool_free_node_s {
    afw_size_t total;
    afw_pool_free_node_t *prev;
    afw_pool_free_node_t *next;
};

/*
 * Heap debug prefix is at least a free node so overlay on free does
 * not touch USER. [size][pool] stay immediately before USER.
 */
#ifdef AFW_DEBUG_POOL
#define AFW_POOL_HEAP_PREFIX_BYTES \
    ((sizeof(afw_pool_debug_prefix_t) > sizeof(afw_pool_free_node_t)) \
        ? sizeof(afw_pool_debug_prefix_t) \
        : sizeof(afw_pool_free_node_t))
#else
#define AFW_POOL_HEAP_PREFIX_BYTES ((afw_size_t)0)
#endif

typedef struct afw_pool_internal_free_memory_head_s
afw_pool_internal_free_memory_head_t;

struct afw_pool_internal_free_memory_head_s {
    afw_pool_free_node_t *first;
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
     * @brief First live tracker allocation (tracker only).
     */
    afw_pool_tracker_node_t *first_allocated_memory;

    /**
     * @brief Free memory head.
     *
     * Heap-owned free list. Trackers share the parent heap's head.
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


AFW_DECLARE(const afw_pool_t *)
afw_pool_internal_create_base_pool();

AFW_DECLARE(afw_boolean_t)
afw_pool_internal_is_heap(const afw_pool_t *p);

AFW_DECLARE(afw_boolean_t)
afw_pool_internal_is_heap_multithreaded(const afw_pool_t *p);

AFW_DECLARE(afw_boolean_t)
afw_pool_internal_is_tracker(const afw_pool_t *p);

AFW_DECLARE(const afw_pool_t *)
afw_pool_internal_heap_create(
    const afw_pool_t *parent,
    afw_boolean_t multithreaded,
    afw_xctx_t *xctx);

extern void
afw_pool_print_debug_info(
    int indent,
    const afw_pool_t *pool,
    afw_xctx_t *xctx);

AFW_END_DECLARES

#endif /* __AFW_POOL_INTERNAL_H__ */
