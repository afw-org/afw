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
 * heap on free or tracker destroy. The heap owns the free list and
 * a list of 4k-aligned posix_memalign chunks. Destroy free()s every
 * chunk.
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

/** Heap 4k-aligned region. Destroy walks first_chunk and free()s each. */
typedef struct afw_pool_chunk_s afw_pool_chunk_t;
struct afw_pool_chunk_s {
    afw_pool_chunk_t *next;
    afw_size_t size;
};

#define AFW_POOL_ALIGN ((afw_size_t)16)
#define AFW_POOL_ALIGN_UP(n) \
    (((n) + (AFW_POOL_ALIGN - 1)) & ~(AFW_POOL_ALIGN - 1))
#define AFW_POOL_CHUNK_MIN ((afw_size_t)4096)

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
     * @brief First malloc chunk (heap only). Trackers leave this NULL.
     *
     * Destroy walks this list and free()s every chunk. The heap self
     * lives in the first allocated chunk.
     */
    afw_pool_chunk_t *first_chunk;

    /**
     * @brief Chunk currently used for bump allocation (heap only).
     */
    afw_pool_chunk_t *current_chunk;

    /**
     * @brief Next unused byte in current_chunk (heap only).
     */
    char *bump;

    /**
     * @brief Bytes left at bump in current_chunk (heap only).
     */
    afw_size_t remaining;

    /**
     * @brief APR pool for afw_pool_get_apr_pool() callers, or NULL.
     *
     * Door for leftover APR function calls. NULL until first
     * get_apr_pool(). Independent of the chunk store. Tracker destroy
     * releases it. Never created if nobody calls.
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

    /**
     * Next pool delaying last release/destroy while
     * error_processing_count > 0.
     */
    afw_pool_internal_self_t *error_delaying_release_next;

    /** Already on xctx->error_delaying_release_first. */
    afw_boolean_t error_delaying_release;

    /** Flush should destroy, not release. */
    afw_boolean_t error_processing_destroy;

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

/**
 * Destroy the process base pool. Process teardown is one thread; do
 * not take the multithreaded lock (it lives in this pool).
 */
AFW_DECLARE(void)
afw_pool_internal_destroy_base_pool(afw_xctx_t *xctx);

AFW_DECLARE(afw_boolean_t)
afw_pool_internal_is_heap(const afw_pool_t *p);

AFW_DECLARE(afw_boolean_t)
afw_pool_internal_is_heap_multithreaded(const afw_pool_t *p);

AFW_DECLARE(afw_boolean_t)
afw_pool_internal_is_tracker(const afw_pool_t *p);

AFW_DECLARE(void)
afw_pool_error_processing_finish(afw_xctx_t *xctx);

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
