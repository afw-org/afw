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
#include "afw_memory_annotate_internal.h"

/**
 * @file afw_pool_heap_internal.h
 * @brief Heap store internals (`afw_pool_heap.c`).
 *
 * The heap owns the free list and mapped chunks (4k-aligned).
 * chunk_min 0 is the 64k floor. Destroy returns chunks to this
 * heap's memory_region. A single-threaded heap uses its thread's
 * region. A multithreaded heap uses the environment's region.
 * Scope is a heap with compile-sized chunks plus throw last-release
 * delay.
 *
 * Heap live: [chunk][USER] or, if AFW_DEBUG_POOL,
 * [chunk…][size][pool][USER]. An ASAN build without AFW_DEBUG_POOL
 * widens the prefix to a free node (afw_memory_annotate_internal.h).
 * `chunk` is the chunk that holds the
 * block so free coalescing does not walk first_chunk. Its low bits
 * mark a freed block and a block in a small bin.
 * Freed heap blocks overlay afw_pool_heap_internal_free_node_t at the block start.
 * Tracker gets blocks from this store (`afw_pool_heap_internal_reservoir_heap`).
 * A tracker block's first word is its list link, not `chunk`, so
 * blocks a tracker returns carry a NULL chunk.
 */

AFW_BEGIN_DECLARES

#define AFW_POOL_HEAP_INTERNAL_ALLOC_START(_user) \
    ((void *)((char *)(_user) - AFW_POOL_HEAP_INTERNAL_PREFIX_BYTES))

#define AFW_POOL_HEAP_INTERNAL_USER_FROM_START(_start) \
    ((void *)((char *)(_start) + AFW_POOL_HEAP_INTERNAL_PREFIX_BYTES))

/** Heap chunk. Destroy walks first_chunk and free()s each. */
typedef struct afw_pool_heap_internal_chunk_s afw_pool_heap_internal_chunk_t;
struct afw_pool_heap_internal_chunk_s {
    afw_pool_heap_internal_chunk_t *next;
    afw_size_t size;
};

/** Free-list overlay at the start of a freed block. `total` is the whole block. */
typedef struct afw_pool_heap_internal_free_node_s afw_pool_heap_internal_free_node_t;
struct afw_pool_heap_internal_free_node_s {
    afw_pool_heap_internal_chunk_t *chunk;
    afw_size_t total;
    afw_pool_heap_internal_free_node_t *prev;
    afw_pool_heap_internal_free_node_t *next;
};

#define AFW_POOL_HEAP_INTERNAL_ALIGN ((afw_size_t)16)
#define AFW_POOL_HEAP_INTERNAL_ALIGN_UP(_n) \
    (((_n) + (AFW_POOL_HEAP_INTERNAL_ALIGN - 1)) & ~(AFW_POOL_HEAP_INTERNAL_ALIGN - 1))
/** Page alignment for mapped chunks. Not an env knob. */
#define AFW_POOL_HEAP_INTERNAL_CHUNK_ALIGN ((afw_size_t)4096)

/*
 * Heap debug prefix is at least a free node so overlay on free does
 * not touch USER. [size][pool] stay immediately before USER while
 * the block is live. The free overlay covers them.
 */
#ifdef AFW_DEBUG_POOL
#define AFW_POOL_HEAP_INTERNAL_PREFIX_BYTES \
    ((sizeof(afw_pool_internal_debug_prefix_t) > sizeof(afw_pool_heap_internal_free_node_t)) \
        ? sizeof(afw_pool_internal_debug_prefix_t) \
        : sizeof(afw_pool_heap_internal_free_node_t))
#elif AFW_MEMORY_ANNOTATE_ACTIVE
/* Free overlay stays in the prefix so all of USER can be no-access. */
#define AFW_POOL_HEAP_INTERNAL_PREFIX_BYTES \
    sizeof(afw_pool_heap_internal_free_node_t)
#else
#define AFW_POOL_HEAP_INTERNAL_PREFIX_BYTES sizeof(afw_pool_heap_internal_chunk_t *)
#endif

/*
 * Small bins: one LIFO list per exact block total from
 * AFW_POOL_HEAP_INTERNAL_BIN_MIN to AFW_POOL_HEAP_INTERNAL_BIN_MAX in
 * AFW_POOL_HEAP_INTERNAL_ALIGN steps. Only a heap that is its own
 * managed_p (a job heap, env->p, a conf or adapter heap) has bins:
 * those see most frees and reallocations. Scopes and inherit heaps
 * are mostly thrown away whole and keep the one list. A binned block
 * does not coalesce.
 */
#define AFW_POOL_HEAP_INTERNAL_BIN_MIN \
    ((afw_size_t)sizeof(afw_pool_heap_internal_free_node_t))
#define AFW_POOL_HEAP_INTERNAL_BIN_MAX ((afw_size_t)512)
#define AFW_POOL_HEAP_INTERNAL_BIN_COUNT \
    ((AFW_POOL_HEAP_INTERNAL_BIN_MAX - AFW_POOL_HEAP_INTERNAL_BIN_MIN) / \
        AFW_POOL_HEAP_INTERNAL_ALIGN + 1)
#define AFW_POOL_HEAP_INTERNAL_BIN_INDEX(_total) \
    (((_total) - AFW_POOL_HEAP_INTERNAL_BIN_MIN) / AFW_POOL_HEAP_INTERNAL_ALIGN)

typedef struct afw_pool_heap_internal_free_memory_head_s
afw_pool_heap_internal_free_memory_head_t;

struct afw_pool_heap_internal_free_memory_head_s {
    /* Blocks that are not in a bin. First-fit; coalesce forward. */
    afw_pool_heap_internal_free_node_t *first;

    /*
     * Upper bound on totals on first. AFW_SIZE_T_MAX means the
     * bound is unknown and the list must be walked. 0 means empty.
     */
    afw_size_t largest;

    /*
     * AFW_POOL_HEAP_INTERNAL_BIN_COUNT LIFO lists linked by next, or
     * NULL if this heap has no bins. Carved from the first chunk.
     */
    afw_pool_heap_internal_free_node_t **bins;

    /* Bit i set: bins[i] is not empty. */
    afw_uint32_t bin_map;
};


typedef struct afw_pool_heap_internal_self_s
afw_pool_heap_internal_self_t;

struct afw_pool_heap_internal_self_s {

    afw_pool_internal_self_t common;

    /**
     * @brief Where this heap's chunks come from.
     *
     * Single-threaded: the owning thread's region. Multithreaded:
     * the environment's region. Not looked up from `thread` later.
     */
    const afw_memory_region_t *memory_region;

    /**
     * @brief First malloc chunk.
     *
     * Destroy walks this list and free()s every chunk. The heap
     * self lives in the first allocated chunk.
     */
    afw_pool_heap_internal_chunk_t *first_chunk;

    /** @brief Chunk currently used for bump allocation. */
    afw_pool_heap_internal_chunk_t *current_chunk;

    /** @brief Next unused byte in current_chunk. */
    char *bump;

    /** @brief Bytes left at bump in current_chunk. */
    afw_size_t remaining;

    /**
     * @brief Mapped chunk bytes still held.
     *
     * Not asked-for malloc.
     */
    afw_size_t chunk_bytes;

    /** @brief Number of chunks on first_chunk. */
    afw_size_t chunk_count;

    /** @brief Minimum mapped chunk size (0 = default). */
    afw_size_t chunk_min;

    /** @brief Heap-owned free list head. */
    afw_pool_heap_internal_free_memory_head_t *free_memory_head;
};


/**
 * Scope pool. ST job heap (4k chunks); its last release also releases
 * the frame slots and lexical parent.
 */
typedef struct afw_pool_heap_internal_scope_self_s
afw_pool_heap_internal_scope_self_t;

struct afw_pool_heap_internal_scope_self_s {

    afw_pool_heap_internal_self_t heap;

    /* Don't access this directly. Use heap.free_memory_head. */
    afw_pool_heap_internal_free_memory_head_t memory_for_free_memory_head;
};


typedef struct afw_pool_heap_internal_self_with_free_memory_head_s
afw_pool_heap_internal_self_with_free_memory_head_t;
struct afw_pool_heap_internal_self_with_free_memory_head_s {

    afw_pool_heap_internal_self_t heap;

    /* Don't access this directly. Use free_memory_head pointer instead. */
    afw_pool_heap_internal_free_memory_head_t memory_for_free_memory_head;
};


#define afw_pool_heap_internal_as_heap(_self) \
    ((afw_pool_heap_internal_self_t *)(_self))
#define afw_pool_heap_internal_as_scope(_self) \
    ((afw_pool_heap_internal_scope_self_t *)(_self))


afw_pool_heap_internal_self_t *
afw_pool_heap_internal_reservoir_heap(afw_pool_internal_self_t *self);

/*
 * Give heap small bins if it is its own managed_p. Call after
 * managed_p is set, before any malloc. Takes the bins from the bump
 * space in the first chunk; no bins if there is not room.
 */
void
afw_pool_heap_internal_init_bins(afw_pool_heap_internal_self_t *heap);

afw_size_t
afw_pool_heap_internal_block_bytes(
    afw_size_t prefix_bytes,
    afw_size_t user_size,
    afw_xctx_t *xctx,
    afw_boolean_t unhandled);

/*
 * chunk is the chunk that holds start, or NULL if the caller does
 * not know it. A NULL chunk block never coalesces.
 */
void
afw_pool_heap_internal_add_to_free_list(
    afw_pool_heap_internal_self_t *heap,
    void *start,
    afw_size_t total,
    afw_pool_heap_internal_chunk_t *chunk,
    afw_xctx_t *xctx);

void *
afw_pool_heap_internal_take_from_free_list_or_chunk(
    afw_pool_heap_internal_self_t *heap,
    afw_size_t total,
    afw_boolean_t *reused,
    afw_xctx_t *xctx,
    afw_boolean_t unhandled);

extern const afw_pool_inf_t afw_pool_heap_internal_multithreaded_inf;

const afw_pool_t *
afw_pool_heap_internal_release(
    afw_pool_internal_self_t *self,
    afw_xctx_t *xctx);

void
afw_pool_heap_internal_run_cleanups(
    afw_pool_internal_self_t *self,
    afw_xctx_t *xctx);

void
afw_pool_heap_internal_destroy(
    afw_pool_internal_self_t *self,
    afw_xctx_t *xctx);

void
afw_pool_heap_internal_garbage_collect(
    afw_pool_internal_self_t *self,
    afw_xctx_t *xctx);

/**
 * Create the process base MT pool. thread is the base thread already
 * created; may be NULL only if create failed earlier. Chunks come
 * from mt_region, not thread->memory_region. xctx does not exist yet.
 */
const afw_pool_t *
afw_pool_heap_internal_create_base_pool(
    const afw_thread_t *thread,
    const afw_memory_region_t *mt_region);

/*
 * True only for the multithreaded heap inf, not a multithreaded
 * scope or tracker. afw_pool_internal_is_multithreaded() is true for
 * all three.
 */
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
afw_pool_heap_internal_create_st(
    const afw_pool_t *parent,
    afw_boolean_t as_managed_p,
    afw_size_t chunk_min,
    afw_xctx_t *xctx);

const afw_pool_t *
afw_pool_heap_internal_multithreaded_create(
    const afw_pool_t *parent,
    afw_boolean_t as_managed_p,
    afw_size_t chunk_min,
    afw_xctx_t *xctx);

afw_pool_internal_self_t *
afw_pool_heap_internal_allocate_self(
    const afw_pool_inf_t *inf,
    afw_size_t chunk_min,
    afw_size_t self_bytes,
    const afw_memory_region_t *region,
    afw_xctx_t *xctx);

afw_pool_internal_self_t *
afw_pool_heap_internal_multithreaded_create_self(
    const afw_pool_t *afw_parent,
    const afw_pool_inf_t *inf,
    afw_boolean_t as_managed_p,
    afw_size_t chunk_min,
    afw_size_t self_bytes,
    const afw_thread_t *thread,
    afw_xctx_t *xctx);

afw_pool_internal_self_t *
afw_pool_heap_internal_create_self(
    const afw_pool_t *afw_parent,
    const afw_pool_inf_t *inf,
    afw_boolean_t as_managed_p,
    afw_size_t chunk_min,
    afw_size_t self_bytes,
    const afw_thread_t *thread,
    afw_xctx_t *xctx);

void
afw_pool_heap_internal_teardown_store(
    afw_pool_internal_self_t *self,
    afw_xctx_t *xctx);

void *
afw_pool_heap_internal_calloc(
    afw_pool_internal_self_t *self,
    afw_size_t size,
    afw_xctx_t *xctx);

void *
afw_pool_heap_internal_malloc(
    afw_pool_internal_self_t *self,
    afw_size_t size,
    afw_xctx_t *xctx);

void
afw_pool_heap_internal_free_memory(
    afw_pool_internal_self_t *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx);

void *
afw_pool_heap_internal_calloc_no_throw(
    afw_pool_internal_self_t *self,
    afw_size_t size,
    afw_xctx_t *xctx);

void *
afw_pool_heap_internal_malloc_no_throw(
    afw_pool_internal_self_t *self,
    afw_size_t size,
    afw_xctx_t *xctx);

void
afw_pool_heap_internal_free_memory_no_throw(
    afw_pool_internal_self_t *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx);

AFW_END_DECLARES

#endif /* __AFW_POOL_HEAP_INTERNAL_H__ */
