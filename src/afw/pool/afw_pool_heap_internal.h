// See the 'COPYING' file in the project root for licensing information.
/*
 * Heap and scope pool internals.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_POOL_HEAP_INTERNAL_H__
#define __AFW_POOL_HEAP_INTERNAL_H__

#include "afw_pool_internal.h"

/**
 * @file afw_pool_heap_internal.h
 * @brief Heap store internals (`afw_pool_heap.c`).
 *
 * The heap owns the free list and posix_memalign chunks (4k-aligned,
 * 64k minimum). Destroy returns chunks to the thread memory_region.
 * Scope is a heap with compile-sized chunks plus throw last-release
 * delay.
 *
 * Heap live: [chunk][USER] or, if AFW_DEBUG_POOL,
 * [chunk…][size][pool][USER]. `chunk` is the posix_memalign
 * chunk so free coalescing does not walk first_chunk.
 * Freed heap blocks overlay afw_pool_free_node_t at the block start.
 * Tracker gets blocks from this store (`afw_pool_heap_internal_reservoir_heap`).
 */

AFW_BEGIN_DECLARES

#define AFW_POOL_HEAP_ALLOC_START(user) \
    ((void *)((char *)(user) - AFW_POOL_HEAP_PREFIX_BYTES))

#define AFW_POOL_HEAP_USER_FROM_START(start) \
    ((void *)((char *)(start) + AFW_POOL_HEAP_PREFIX_BYTES))

/** Heap chunk. Destroy walks first_chunk and free()s each. */
typedef struct afw_pool_chunk_s afw_pool_chunk_t;
struct afw_pool_chunk_s {
    afw_pool_chunk_t *next;
    afw_size_t size;
};

/** Free-list overlay at the start of a freed block. `total` is the whole block. */
typedef struct afw_pool_free_node_s afw_pool_free_node_t;
struct afw_pool_free_node_s {
    afw_pool_chunk_t *chunk;
    afw_size_t total;
    afw_pool_free_node_t *prev;
    afw_pool_free_node_t *next;
};

#define AFW_POOL_ALIGN ((afw_size_t)16)
#define AFW_POOL_ALIGN_UP(n) \
    (((n) + (AFW_POOL_ALIGN - 1)) & ~(AFW_POOL_ALIGN - 1))
/** posix_memalign alignment (page). Not an env knob. */
#define AFW_POOL_CHUNK_ALIGN ((afw_size_t)4096)

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
#define AFW_POOL_HEAP_PREFIX_BYTES sizeof(afw_pool_chunk_t *)
#endif

typedef struct afw_pool_internal_free_memory_head_s
afw_pool_internal_free_memory_head_t;

struct afw_pool_internal_free_memory_head_s {
    afw_pool_free_node_t *first;
};


typedef struct afw_pool_internal_heap_self_s
afw_pool_internal_heap_self_t;

struct afw_pool_internal_heap_self_s {

    afw_pool_internal_self_t common;

    /**
     * @brief First malloc chunk.
     *
     * Destroy walks this list and free()s every chunk. The heap
     * self lives in the first allocated chunk.
     */
    afw_pool_chunk_t *first_chunk;

    /** @brief Chunk currently used for bump allocation. */
    afw_pool_chunk_t *current_chunk;

    /** @brief Next unused byte in current_chunk. */
    char *bump;

    /** @brief Bytes left at bump in current_chunk. */
    afw_size_t remaining;

    /**
     * @brief posix_memalign bytes still held.
     *
     * Not asked-for malloc.
     */
    afw_size_t chunk_bytes;

    /** @brief Number of chunks on first_chunk. */
    afw_size_t chunk_count;

    /** @brief Minimum posix_memalign size (0 = default). */
    afw_size_t chunk_min;

    /** @brief Heap-owned free list head. */
    afw_pool_internal_free_memory_head_t *free_memory_head;
};


/**
 * Scope pool. ST job heap (4k chunks) plus last-release delay while a
 * script throw is handled.
 */
typedef struct afw_pool_internal_scope_self_s
afw_pool_internal_scope_self_t;

struct afw_pool_internal_scope_self_s {

    afw_pool_internal_heap_self_t heap;

    /* Don't access this directly. Use heap.free_memory_head. */
    afw_pool_internal_free_memory_head_t memory_for_free_memory_head;

    /**
     * Next pool delaying last release while
     * error_processing_count > 0.
     */
    const afw_pool_t *error_delaying_release_next;

    /** Already on xctx->error_delaying_release_first. */
    afw_boolean_t error_delaying_release;
};


typedef struct afw_pool_internal_self_with_free_memory_head_s
afw_pool_internal_self_with_free_memory_head_t;
struct afw_pool_internal_self_with_free_memory_head_s {

    afw_pool_internal_heap_self_t heap;

    /* Don't access this directly. Use free_memory_head pointer instead. */
    afw_pool_internal_free_memory_head_t memory_for_free_memory_head;
};


#define afw_pool_heap_internal_as_heap(self) \
    ((afw_pool_internal_heap_self_t *)(self))
#define afw_pool_heap_internal_as_scope(self) \
    ((afw_pool_internal_scope_self_t *)(self))


afw_pool_internal_heap_self_t *
afw_pool_heap_internal_reservoir_heap(afw_pool_internal_self_t *self);

afw_size_t
afw_pool_heap_internal_block_bytes(
    afw_size_t prefix_bytes,
    afw_size_t user_size,
    afw_xctx_t *xctx,
    afw_boolean_t unhandled);

void
afw_pool_heap_internal_add_to_free_list(
    afw_pool_internal_heap_self_t *heap,
    void *start,
    afw_size_t total,
    afw_xctx_t *xctx);

void *
afw_pool_heap_internal_take_from_free_list_or_chunk(
    afw_pool_internal_heap_self_t *heap,
    afw_size_t total,
    afw_boolean_t *reused,
    afw_xctx_t *xctx,
    afw_boolean_t unhandled);

/**
 * Create the process base MT pool. thread is the base thread already
 * created; may be NULL only if create failed earlier. xctx does not
 * exist yet.
 */
const afw_pool_t *
afw_pool_heap_internal_create_base_pool(const afw_thread_t *thread);

afw_boolean_t
afw_pool_heap_internal_is_multithreaded(const afw_pool_t *p);

/**
 * ST job heap for this thread. Parent may be MT (env->p). This is the
 * thread-handoff door; heap_create() follows parent ST/MT.
 */
const afw_pool_t *
afw_pool_heap_internal_create_st_for_thread(
    const afw_pool_t *parent,
    afw_boolean_t as_managed_p,
    afw_size_t chunk_min,
    const afw_thread_t *thread,
    afw_xctx_t *xctx);

const afw_pool_t *
afw_pool_heap_internal_create(
    const afw_pool_t *parent,
    afw_boolean_t multithreaded,
    afw_boolean_t as_managed_p,
    afw_size_t chunk_min,
    afw_xctx_t *xctx);

/**
 * Last-release pools delayed during error processing (ENDTRY).
 */
void
afw_pool_heap_internal_release_delayed(
    const afw_pool_t *instance,
    afw_xctx_t *xctx);

AFW_END_DECLARES

#endif /* __AFW_POOL_HEAP_INTERNAL_H__ */
