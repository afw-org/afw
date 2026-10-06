// See the 'COPYING' file in the project root for licensing information.
/*
 * Heap and scope pool implementation.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_pool_heap.c
 * @brief Heap store, heap/scope infs.
 *
 * Chunks, bump, free list, and heap malloc/free. Scope is a heap
 * with compile-sized chunks (see afw_pool_scope.c).
 * The multithreaded inf is `afw_pool_heap_multithreaded.c`.
 * Shared lifetime is `afw_pool.c`. Tracker is `afw_pool_tracker.c`.
 */

#include "afw_internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define AFW_POOL_SELF_T afw_pool_internal_self_t

/*
 * Shared methods live in afw_pool.c. These #defines skip the
 * generated static prototypes so the infs take the common
 * implementations.
 */
#define impl_afw_pool_get_reference afw_pool_internal_get_reference
#define impl_afw_pool_register_cleanup afw_pool_internal_register_cleanup
#define impl_afw_pool_deregister_cleanup afw_pool_internal_deregister_cleanup

#define AFW_IMPLEMENTATION_ID "heap"

static const afw_pool_internal_inf_implementation_specific_t
impl_pool_implementation_specific =
    {
        /* multithreaded */ false,
        /* tracker */ false
    };

#define AFW_IMPLEMENTATION_SPECIFIC &impl_pool_implementation_specific

#define impl_afw_pool_release afw_pool_heap_internal_release
#define impl_afw_pool_run_cleanups afw_pool_heap_internal_run_cleanups
#define impl_afw_pool_destroy afw_pool_heap_internal_destroy
#define impl_afw_pool_garbage_collect afw_pool_heap_internal_garbage_collect
#define impl_afw_pool_calloc afw_pool_heap_internal_calloc
#define impl_afw_pool_malloc afw_pool_heap_internal_malloc
#define impl_afw_pool_free_memory afw_pool_heap_internal_free_memory
#define impl_afw_pool_free_memory_no_throw \
    afw_pool_heap_internal_free_memory_no_throw
#define impl_afw_pool_calloc_no_throw afw_pool_heap_internal_calloc_no_throw
#define impl_afw_pool_malloc_no_throw afw_pool_heap_internal_malloc_no_throw

AFW_POOL_INTERNAL_REFERENCE_WRAPPERS(impl_pool_ref_1, afw_pool_heap_internal_release, afw_pool_internal_get_reference)
#undef impl_afw_pool_release
#define impl_afw_pool_release impl_pool_ref_1_release
#undef impl_afw_pool_get_reference
#define impl_afw_pool_get_reference impl_pool_ref_1_get_reference
#undef impl_afw_pool_get_reference_count
#define impl_afw_pool_get_reference_count afw_pool_internal_get_reference_count
#undef impl_afw_pool_for_each_reference
#define impl_afw_pool_for_each_reference afw_pool_internal_no_references_for_each
#undef impl_afw_pool_release_references
#define impl_afw_pool_release_references afw_pool_internal_no_references_release_references
#include "afw_pool_impl_declares.h"
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_SPECIFIC
#undef impl_afw_pool_release
#undef impl_afw_pool_run_cleanups
#undef impl_afw_pool_destroy
#undef impl_afw_pool_garbage_collect
#undef impl_afw_pool_calloc
#undef impl_afw_pool_malloc
#undef impl_afw_pool_free_memory
#undef impl_afw_pool_calloc_no_throw
#undef impl_afw_pool_malloc_no_throw
#undef impl_afw_pool_free_memory_no_throw

static void
impl_pool_set_owning_thread(
    afw_pool_heap_internal_self_t *heap,
    const afw_thread_t *thread)
{
    afw_pool_internal_self_t *self;

    self = &heap->common;
    if (self->thread == thread) {
        return;
    }
    if (self->thread) {
        afw_pool_internal_thread_sub_bytes(self->thread, self->bytes_allocated);
        afw_pool_internal_thread_sub_chunks(self->thread, heap->chunk_bytes);
    }
    self->thread = thread;
    if (thread) {
        afw_pool_internal_thread_add_bytes(thread, self->bytes_allocated);
        afw_pool_internal_thread_add_chunks(thread, heap->chunk_bytes);
    }
}

static void
impl_account_chunk_add(
    afw_pool_heap_internal_self_t *heap, afw_size_t size, afw_xctx_t *xctx)
{
    heap->chunk_count++;
    heap->chunk_bytes += size;
    if (xctx && xctx->env) {
        afw_pool_internal_env_add_chunks((afw_environment_t *)xctx->env, size);
    }
    if (afw_pool_internal_counts_on_thread(&heap->common)) {
        afw_pool_internal_thread_add_chunks(heap->common.thread, size);
    }
}

afw_pool_heap_internal_self_t *
afw_pool_heap_internal_reservoir_heap(afw_pool_internal_self_t *self)
{
    /* A heap is its own store even when it has an AFW parent. */
    while (self->parent && !afw_pool_internal_is_heap(&self->pub)) {
        self = self->parent;
    }
    return afw_pool_heap_internal_as_heap(self);
}


void
afw_pool_heap_internal_init_bins(afw_pool_heap_internal_self_t *heap)
{
    afw_size_t size;

    if (heap->common.pub.managed_p != &heap->common.pub ||
        !heap->free_memory_head ||
        heap->current_chunk != heap->first_chunk)
    {
        return;
    }
    size = AFW_POOL_HEAP_INTERNAL_ALIGN_UP(
        sizeof(afw_pool_heap_internal_free_node_t *) *
        AFW_POOL_HEAP_INTERNAL_BIN_COUNT);
    if (heap->remaining < size) {
        return;
    }
    AFW_MEMORY_ANNOTATE_ACCESS(heap->bump, size);
    memset(heap->bump, 0, size);
    heap->free_memory_head->bins =
        (afw_pool_heap_internal_free_node_t **)(void *)heap->bump;
    heap->free_memory_head->bin_map = 0;
    heap->bump += size;
    heap->remaining -= size;
}


const afw_memory_region_t *
afw_pool_internal_memory_region(const afw_pool_internal_self_t *self)
{
    afw_pool_heap_internal_self_t *heap;

    if (!self) {
        return NULL;
    }
    if (afw_pool_internal_is_heap(&self->pub)) {
        heap = afw_pool_heap_internal_as_heap(
            (afw_pool_internal_self_t *)self);
    }
    else {
        heap = afw_pool_heap_internal_reservoir_heap(
            (afw_pool_internal_self_t *)self);
    }
    return heap ? heap->memory_region : NULL;
}


#define impl_chunk_usable(_chunk) \
    ((char *)(_chunk) + AFW_POOL_HEAP_INTERNAL_ALIGN_UP(sizeof(afw_pool_heap_internal_chunk_t)))

#define impl_chunk_end(_chunk) \
    ((char *)(_chunk) + (_chunk)->size)

/*
 * Low bits of a block's chunk pointer (chunks are page aligned).
 * FREE: on a free list or in a bin. BINNED: in a bin, so forward
 * coalescing leaves it alone.
 */
#define AFW_POOL_BLOCK_FREE_BIT ((uintptr_t)1)
#define AFW_POOL_BLOCK_BINNED_BIT ((uintptr_t)2)
#define AFW_POOL_BLOCK_BITS \
    (AFW_POOL_BLOCK_FREE_BIT | AFW_POOL_BLOCK_BINNED_BIT)

#define impl_block_chunk(_start) \
    ((afw_pool_heap_internal_chunk_t *)(((uintptr_t) \
        ((afw_pool_heap_internal_free_node_t *)(_start))->chunk) & \
        ~AFW_POOL_BLOCK_BITS))

#define impl_block_set_chunk(_start, _chunk) \
    (((afw_pool_heap_internal_free_node_t *)(_start))->chunk = (_chunk))

#define impl_block_mark_free(_start) \
    (((afw_pool_heap_internal_free_node_t *)(_start))->chunk = \
        (afw_pool_heap_internal_chunk_t *)(((uintptr_t)impl_block_chunk(_start)) | \
            AFW_POOL_BLOCK_FREE_BIT))

#define impl_block_is_free(_start) \
    ((((uintptr_t)((afw_pool_heap_internal_free_node_t *)(_start))->chunk) & \
        AFW_POOL_BLOCK_FREE_BIT) != 0)

#define impl_block_mark_binned(_start) \
    (((afw_pool_heap_internal_free_node_t *)(_start))->chunk = \
        (afw_pool_heap_internal_chunk_t *)(((uintptr_t)impl_block_chunk(_start)) | \
            AFW_POOL_BLOCK_BITS))

#define impl_block_is_binned(_start) \
    ((((uintptr_t)((afw_pool_heap_internal_free_node_t *)(_start))->chunk) & \
        AFW_POOL_BLOCK_BINNED_BIT) != 0)

#define impl_same_chunk(_a, _b) \
    (impl_block_chunk(_a) == impl_block_chunk(_b))

#define impl_addr_in_chunk(_addr, _chunk, _need) \
    ((_addr) && (_chunk) && (_need) != 0 && \
        (char *)(_addr) >= impl_chunk_usable(_chunk) && \
        (char *)(_addr) <= impl_chunk_end(_chunk) && \
        (afw_size_t)(impl_chunk_end(_chunk) - (char *)(_addr)) >= \
            (_need))


AFW_DEFINE(afw_size_t)
afw_pool_round_up_chunk_size(afw_size_t size)
{
    afw_size_t rem;
    afw_size_t add;

    if (size == 0) {
        return 0;
    }
    if (size < AFW_POOL_HEAP_INTERNAL_CHUNK_ALIGN) {
        return AFW_POOL_HEAP_INTERNAL_CHUNK_ALIGN;
    }
    rem = size & (AFW_POOL_HEAP_INTERNAL_CHUNK_ALIGN - 1);
    if (rem == 0) {
        return size;
    }
    add = AFW_POOL_HEAP_INTERNAL_CHUNK_ALIGN - rem;
    if (size > AFW_SIZE_T_MAX - add) {
        return AFW_SIZE_T_MAX & ~(AFW_POOL_HEAP_INTERNAL_CHUNK_ALIGN - 1);
    }
    return size + add;
}


static afw_size_t
impl_normalize_chunk_min(afw_size_t chunk_min, const afw_environment_t *env)
{
    if (chunk_min == 0) {
        if (env) {
            return env->default_chunk_min;
        }
        chunk_min = AFW_ENVIRONMENT_DEFAULT_CHUNK_MIN;
    }
    return afw_pool_round_up_chunk_size(chunk_min);
}


static afw_size_t
impl_chunk_need(afw_size_t min_payload, afw_size_t chunk_min)
{
    afw_size_t header;
    afw_size_t need;

    header = AFW_POOL_HEAP_INTERNAL_ALIGN_UP(sizeof(afw_pool_heap_internal_chunk_t));
    if (min_payload > AFW_SIZE_T_MAX - header) {
        return 0;
    }
    need = header + min_payload;
    if (need < chunk_min) {
        need = chunk_min;
    }
    return afw_pool_round_up_chunk_size(need);
}


static afw_pool_heap_internal_chunk_t *
impl_chunk_malloc(
    afw_size_t min_payload,
    afw_size_t chunk_min,
    const afw_memory_region_t *region,
    afw_xctx_t *xctx)
{
    afw_size_t need;
    void *mem;
    afw_pool_heap_internal_chunk_t *chunk;

    need = impl_chunk_need(min_payload, chunk_min);
    if (need == 0 || !region) {
        return NULL;
    }
    mem = NULL;
    afw_memory_region_get(region, &mem, &need, xctx);
    if (!mem) {
        return NULL;
    }
    chunk = (afw_pool_heap_internal_chunk_t *)mem;
    chunk->next = NULL;
    chunk->size = need;
    AFW_MEMORY_ANNOTATE_NOACCESS(impl_chunk_usable(chunk),
        impl_chunk_end(chunk) - impl_chunk_usable(chunk));
    AFW_MEMORY_ANNOTATE_ROOT(chunk, need);
    return chunk;
}


afw_pool_internal_self_t *
afw_pool_heap_internal_allocate_self(
    const afw_pool_inf_t *inf,
    afw_size_t chunk_min,
    afw_size_t self_bytes,
    const afw_memory_region_t *region,
    afw_xctx_t *xctx)
{
    afw_size_t min_self;
    afw_pool_heap_internal_chunk_t *chunk;
    afw_pool_heap_internal_self_t *heap;
    afw_pool_internal_self_t *self;
    char *usable;
    char *after_self;
    const afw_environment_t *env;

    env = (xctx && xctx->env) ? xctx->env : NULL;
    chunk_min = impl_normalize_chunk_min(chunk_min, env);
    min_self = sizeof(afw_pool_heap_internal_self_with_free_memory_head_t);
    if (self_bytes < min_self) {
        self_bytes = min_self;
    }
    self_bytes = AFW_POOL_HEAP_INTERNAL_ALIGN_UP(self_bytes);
    chunk = impl_chunk_malloc(self_bytes, chunk_min, region, xctx);
    if (!chunk) {
        return NULL;
    }
    usable = impl_chunk_usable(chunk);
    AFW_MEMORY_ANNOTATE_ACCESS(usable, self_bytes);
    memset(usable, 0, self_bytes);
    heap = (afw_pool_heap_internal_self_t *)(void *)usable;
    self = &heap->common;
    self->pub.inf = inf;
    self->pub.managed_p = &self->pub;
    heap->first_chunk = chunk;
    heap->current_chunk = chunk;
    heap->chunk_count = 1;
    heap->chunk_bytes = chunk->size;
    heap->chunk_min = chunk_min;
    after_self = usable + self_bytes;
    heap->bump = after_self;
    heap->remaining = (afw_size_t)(impl_chunk_end(chunk) - after_self);
    /* Same overlay as afw_pool_heap_internal_self_with_free_memory_head_t. */
    heap->free_memory_head = (afw_pool_heap_internal_free_memory_head_t *)
        (usable + offsetof(afw_pool_heap_internal_self_with_free_memory_head_t,
            memory_for_free_memory_head));
    self->reference_count = 1;
    return self;
}


/*
 * Process base pool. environment_release does not destroy it
 * (intended: MT lock lives in this pool; process lifetime). Keep
 * this pointer so valgrind sees the chunks as still-reachable, not
 * definitely lost.
 */
static afw_pool_heap_internal_self_t *impl_base_pool_self;

/* Single-threaded heap or scope. Chunks come from the thread region. */
afw_pool_internal_self_t *
afw_pool_heap_internal_create_self(
    const afw_pool_t *afw_parent,
    const afw_pool_inf_t *inf,
    afw_boolean_t as_managed_p,
    afw_size_t chunk_min,
    afw_size_t self_bytes,
    const afw_thread_t *thread,
    afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *self;
    afw_pool_heap_internal_self_t *heap;
    afw_pool_internal_self_t *parent_self;
    const afw_memory_region_t *region;

    if (!thread && xctx) {
        thread = xctx->thread;
    }
    if (!thread) {
        if (!xctx) {
            return NULL;
        }
        AFW_THROW_ERROR_Z(general, "Heap requires a thread", xctx);
    }
    region = thread->memory_region;
    if (!region) {
        if (!xctx) {
            return NULL;
        }
        AFW_THROW_ERROR_Z(general,
            "Heap requires thread->memory_region", xctx);
    }
    self = afw_pool_heap_internal_allocate_self(inf, chunk_min, self_bytes,
        region, xctx);
    if (!self) {
        if (!xctx) {
            return NULL;
        }
        AFW_THROW_ERROR_Z(memory, "Unable to allocate pool", xctx);
    }
    heap = afw_pool_heap_internal_as_heap(self);
    heap->memory_region = region;
    if (!as_managed_p && afw_parent) {
        self->pub.managed_p = afw_parent->managed_p
            ? afw_parent->managed_p
            : afw_parent;
    }
    afw_pool_heap_internal_init_bins(heap);
    self->thread = thread;
    afw_pool_internal_assign_pool_number(self);

    if (afw_parent) {
        parent_self = (afw_pool_internal_self_t *)afw_parent;
        afw_pool_internal_link_as_child(parent_self, self, xctx);
    }

    if (xctx && xctx->env && heap->chunk_bytes) {
        afw_pool_internal_env_add_chunks((afw_environment_t *)xctx->env,
            heap->chunk_bytes);
        if (afw_pool_internal_counts_on_thread(self)) {
            afw_pool_internal_thread_add_chunks(self->thread, heap->chunk_bytes);
        }
    }

    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_Z(minimal, "create");

    return self;
}

/*
 * Free blocks: small bins (afw_pool_heap_internal.h) on a heap that
 * has them, else first-fit on the LIFO list `first`. Overlay lives
 * only on freed blocks. Remainder too small to hold a free node is
 * left with the block so total is always recoverable as prefix +
 * USER size.
 *
 * largest is an upper bound. A take of a max-sized node sets it to
 * AFW_SIZE_T_MAX until a full miss walk records the new maximum.
 * Allocations larger than a known maximum skip the walk.
 */
static void
impl_largest_before_unlink(
    afw_pool_heap_internal_free_memory_head_t *head,
    const afw_pool_heap_internal_free_node_t *node)
{
    if (head->largest != AFW_SIZE_T_MAX &&
        node->total >= head->largest)
    {
        head->largest = AFW_SIZE_T_MAX;
    }
}

static void
impl_largest_note(
    afw_pool_heap_internal_free_memory_head_t *head,
    afw_size_t total)
{
    if (head->largest != AFW_SIZE_T_MAX && total > head->largest) {
        head->largest = total;
    }
}

static void
impl_heap_free_unlink(
    afw_pool_heap_internal_free_node_t **head,
    afw_pool_heap_internal_free_node_t *node)
{
    if (node->prev) {
        node->prev->next = node->next;
    }
    else {
        *head = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    }
    node->prev = NULL;
    node->next = NULL;
}


afw_size_t
afw_pool_heap_internal_block_bytes(
    afw_size_t prefix_bytes,
    afw_size_t user_size,
    afw_xctx_t *xctx,
    afw_boolean_t unhandled)
{
    afw_size_t need;
    afw_size_t aligned;

    if (prefix_bytes > AFW_SIZE_T_MAX - user_size) {
        if (unhandled) {
            return 0;
        }
        AFW_THROW_ERROR_Z(memory,
            "Requested allocation size is too large",
            xctx);
    }
    need = prefix_bytes + user_size;
    if (need < sizeof(afw_pool_heap_internal_free_node_t)) {
        need = sizeof(afw_pool_heap_internal_free_node_t);
    }
    aligned = AFW_POOL_HEAP_INTERNAL_ALIGN_UP(need);
    if (aligned < need) {
        if (unhandled) {
            return 0;
        }
        AFW_THROW_ERROR_Z(memory,
            "Requested allocation size is too large",
            xctx);
    }
    return aligned;
}


void
afw_pool_heap_internal_add_to_free_list(
    afw_pool_heap_internal_self_t *heap,
    void *start,
    afw_size_t total,
    afw_pool_heap_internal_chunk_t *chunk,
    afw_xctx_t *xctx);

/* Push a free block (FREE already set) on its bin. */
static void
impl_bin_push(
    afw_pool_heap_internal_free_memory_head_t *head,
    afw_pool_heap_internal_free_node_t *node)
{
    afw_size_t i;

    i = AFW_POOL_HEAP_INTERNAL_BIN_INDEX(node->total);
    impl_block_mark_binned(node);
    node->prev = NULL;
    node->next = head->bins[i];
    head->bins[i] = node;
    head->bin_map |= ((afw_uint32_t)1) << i;
}


static afw_pool_heap_internal_free_node_t *
impl_bin_pop(
    afw_pool_heap_internal_free_memory_head_t *head,
    afw_size_t i)
{
    afw_pool_heap_internal_free_node_t *node;

    node = head->bins[i];
    head->bins[i] = node->next;
    if (!node->next) {
        head->bin_map &= ~(((afw_uint32_t)1) << i);
    }
    return node;
}


/* Lowest set bit. map is not 0. */
static afw_size_t
impl_lowest_bit(afw_uint32_t map)
{
#if defined(__GNUC__) || defined(__clang__)
    return (afw_size_t)__builtin_ctz(map);
#else
    afw_size_t i;

    for (i = 0; !(map & 1); i++) {
        map >>= 1;
    }
    return i;
#endif
}


/*
 * Small request on a heap with bins: the exact bin, else the
 * smallest bin at least a free node bigger, split, with the rest
 * in its bin. NULL if no bin can serve it.
 */
static afw_pool_heap_internal_free_node_t *
impl_take_from_bins(
    afw_pool_heap_internal_free_memory_head_t *head,
    afw_size_t total)
{
    afw_pool_heap_internal_free_node_t *node;
    afw_pool_heap_internal_free_node_t *rest;
    afw_size_t i;
    afw_size_t skip;
    afw_uint32_t map;

    i = AFW_POOL_HEAP_INTERNAL_BIN_INDEX(total);
    if (head->bins[i]) {
        return impl_bin_pop(head, i);
    }
    /* A split must leave at least a free node. */
    skip = i + AFW_POOL_HEAP_INTERNAL_BIN_MIN / AFW_POOL_HEAP_INTERNAL_ALIGN;
    if (skip >= AFW_POOL_HEAP_INTERNAL_BIN_COUNT) {
        return NULL;
    }
    map = head->bin_map & ~((((afw_uint32_t)1) << skip) - 1);
    if (!map) {
        return NULL;
    }
    node = impl_bin_pop(head, impl_lowest_bit(map));
    rest = (afw_pool_heap_internal_free_node_t *)(((char *)node) + total);
    AFW_MEMORY_ANNOTATE_ACCESS(rest, sizeof(*rest));
    rest->total = node->total - total;
    rest->chunk = impl_block_chunk(node);
    impl_block_mark_free(rest);
    impl_bin_push(head, rest);
    node->total = total;
    return node;
}


void *
afw_pool_heap_internal_take_from_free_list_or_chunk(
    afw_pool_heap_internal_self_t *heap,
    afw_size_t total,
    afw_boolean_t *reused,
    afw_xctx_t *xctx,
    afw_boolean_t unhandled)
{
    afw_pool_heap_internal_free_memory_head_t *head;
    afw_pool_heap_internal_free_node_t *curr;
    afw_pool_heap_internal_free_node_t *prev;
    afw_pool_heap_internal_free_node_t *next;
    afw_pool_heap_internal_free_node_t *rest;
#ifdef AFW_DEBUG_POOL
    afw_pool_heap_internal_free_node_t *fast;
#endif
    afw_pool_heap_internal_chunk_t *chunk;
    const afw_memory_region_t *region;
    char *end;
    void *start;
    afw_size_t seen_max;

    head = heap->free_memory_head;
    curr = NULL;

    if (head && head->bins && total <= AFW_POOL_HEAP_INTERNAL_BIN_MAX) {
        curr = impl_take_from_bins(head, total);
        if (curr) {
            AFW_MEMORY_ANNOTATE_ACCESS(curr, total);
            impl_block_set_chunk(curr, impl_block_chunk(curr));
            *reused = true;
            return curr;
        }
    }

    /*
     * largest < total means no free node can satisfy this request.
     * AFW_SIZE_T_MAX is unknown, so that still walks.
     */
    if (head && head->first && head->largest >= total) {
        seen_max = 0;
#ifdef AFW_DEBUG_POOL
        /* A cycle would loop forever. Debug builds check for one. */
        fast = head->first;
#endif
        for (curr = head->first; curr; curr = curr->next) {
            if (curr->total > seen_max) {
                seen_max = curr->total;
            }
            if (curr->total >= total &&
                (curr->total == total ||
                    curr->total - total >= sizeof(afw_pool_heap_internal_free_node_t)))
            {
                break;
            }
#ifdef AFW_DEBUG_POOL
            if (fast) {
                fast = fast->next;
            }
            if (fast) {
                fast = fast->next;
            }
            if (fast && fast == curr) {
                if (unhandled) {
                    return NULL;
                }
                AFW_THROW_ERROR_Z(general,
                    "heap free-list cycle",
                    xctx);
            }
#endif
        }
        /* Full miss. seen_max is the exact maximum still on the list. */
        if (!curr) {
            head->largest = seen_max;
        }
    }

    if (curr) {
        prev = curr->prev;
        next = curr->next;
        impl_largest_before_unlink(head, curr);
        impl_heap_free_unlink(&head->first, curr);
        if (curr->total - total >= sizeof(afw_pool_heap_internal_free_node_t)) {
            rest = (afw_pool_heap_internal_free_node_t *)(((char *)curr) + total);
            AFW_MEMORY_ANNOTATE_ACCESS(rest, sizeof(*rest));
            rest->total = curr->total - total;
            rest->chunk = impl_block_chunk(curr);
            impl_block_mark_free(rest);
            curr->total = total;
            if (head->bins &&
                rest->total <= AFW_POOL_HEAP_INTERNAL_BIN_MAX)
            {
                impl_bin_push(head, rest);
                if (!head->first) {
                    head->largest = 0;
                }
            }
            else {
                rest->prev = prev;
                rest->next = next;
                if (prev) {
                    prev->next = rest;
                }
                else {
                    head->first = rest;
                }
                if (next) {
                    next->prev = rest;
                }
                if (next &&
                    ((char *)rest) + rest->total == (char *)next &&
                    impl_block_is_free(next) &&
                    impl_same_chunk(rest, next))
                {
                    impl_largest_before_unlink(head, next);
                    rest->total += next->total;
                    rest->next = next->next;
                    if (next->next) {
                        next->next->prev = rest;
                    }
                }
                if (head->first == rest && rest->next == NULL) {
                    head->largest = rest->total;
                }
                else {
                    impl_largest_note(head, rest->total);
                }
            }
        }
        else if (!head->first) {
            head->largest = 0;
        }
        AFW_MEMORY_ANNOTATE_ACCESS(curr, total);
        impl_block_set_chunk(curr, impl_block_chunk(curr));
        *reused = true;
        return curr;
    }

    *reused = false;
    if (heap->remaining >= total) {
        start = heap->bump;
        heap->bump += total;
        heap->remaining -= total;
        AFW_MEMORY_ANNOTATE_ACCESS(start, total);
        impl_block_set_chunk(start, heap->current_chunk);
        return start;
    }

    if (heap->current_chunk &&
        heap->remaining >= sizeof(afw_pool_heap_internal_free_node_t))
    {
        afw_pool_heap_internal_add_to_free_list(heap, heap->bump,
            heap->remaining, heap->current_chunk, xctx);
    }
    heap->bump = NULL;
    heap->remaining = 0;

    /* Resource limits are checked once per chunk, not per malloc. */
    if (!unhandled && xctx->error_processing_count == 0) {
        afw_xctx_check_resource_limits(xctx, 0);
        afw_xctx_internal_check_request_pool_bytes(xctx,
            heap->common.thread, total);
    }

    region = heap->memory_region;
    if (!region) {
        if (unhandled || !xctx) {
            return NULL;
        }
        AFW_THROW_ERROR_Z(general,
            "Heap requires a memory_region", xctx);
    }
    chunk = impl_chunk_malloc(total, heap->chunk_min, region, xctx);
    if (!chunk) {
        if (unhandled) {
            return NULL;
        }
        AFW_THROW_ERROR_Z(memory, "Allocate memory error", xctx);
    }
    impl_account_chunk_add(heap, chunk->size, xctx);
    chunk->next = heap->first_chunk;
    heap->first_chunk = chunk;
    heap->current_chunk = chunk;
    heap->bump = impl_chunk_usable(chunk);
    end = impl_chunk_end(chunk);
    heap->remaining = (afw_size_t)(end - heap->bump);
    if (heap->remaining < total) {
        if (unhandled) {
            return NULL;
        }
        AFW_THROW_ERROR_Z(memory, "Allocate memory error", xctx);
    }
    start = heap->bump;
    heap->bump += total;
    heap->remaining -= total;
    AFW_MEMORY_ANNOTATE_ACCESS(start, total);
    impl_block_set_chunk(start, chunk);
    return start;
}


void
afw_pool_heap_internal_add_to_free_list(
    afw_pool_heap_internal_self_t *heap,
    void *start,
    afw_size_t total,
    afw_pool_heap_internal_chunk_t *chunk,
    afw_xctx_t *xctx)
{
    afw_pool_heap_internal_free_node_t *freeing;
    afw_pool_heap_internal_free_memory_head_t *head;

    (void)xctx;
    head = heap->free_memory_head;
    if (!head) {
        return;
    }

    freeing = (afw_pool_heap_internal_free_node_t *)start;
    /*
     * Header stays accessible for the list; the rest of this block is
     * no-access. A header this block absorbs below stays accessible
     * too: a second free of that block reads its free bit.
     */
    AFW_MEMORY_ANNOTATE_ACCESS(freeing, sizeof(*freeing));
    AFW_MEMORY_ANNOTATE_NOACCESS((char *)freeing + sizeof(*freeing),
        total - sizeof(*freeing));
    freeing->total = total;
    impl_block_set_chunk(freeing, chunk);
    impl_block_mark_free(freeing);

    if (head->bins && total <= AFW_POOL_HEAP_INTERNAL_BIN_MAX) {
        impl_bin_push(head, freeing);
        return;
    }

    {
        char *nstart;
        afw_pool_heap_internal_free_node_t *nxt;

        nstart = ((char *)freeing) + freeing->total;
        /*
         * Unused bump in the current chunk is not a block header.
         * Peeking it is an uninit read (valgrind).
         */
        if (chunk &&
            !(chunk == heap->current_chunk &&
                nstart == heap->bump) &&
            impl_addr_in_chunk(nstart, chunk,
                sizeof(afw_pool_heap_internal_free_node_t)) &&
            impl_block_is_free(nstart) &&
            !impl_block_is_binned(nstart) &&
            impl_block_chunk(nstart) == chunk)
        {
            nxt = (afw_pool_heap_internal_free_node_t *)nstart;
            if (impl_addr_in_chunk(nstart, chunk, nxt->total)) {
                impl_largest_before_unlink(head, nxt);
                impl_heap_free_unlink(&head->first, nxt);
                freeing->total += nxt->total;
            }
        }
    }

    freeing->prev = NULL;
    freeing->next = head->first;
    if (head->first) {
        head->first->prev = freeing;
    }
    head->first = freeing;
    if (freeing->next == NULL) {
        head->largest = freeing->total;
    }
    else {
        impl_largest_note(head, freeing->total);
    }
}

static void
impl_heap_free_chunks(afw_pool_heap_internal_self_t *heap, afw_xctx_t *xctx)
{
    afw_pool_heap_internal_chunk_t *chunk;
    afw_pool_heap_internal_chunk_t *next;
    const afw_memory_region_t *region;
    afw_size_t size;

    if (xctx && xctx->env && heap->chunk_bytes) {
        ((afw_environment_t *)xctx->env)->pool_chunk_bytes -=
            heap->chunk_bytes;
    }
    if (afw_pool_internal_counts_on_thread(&heap->common)) {
        afw_pool_internal_thread_sub_chunks(heap->common.thread, heap->chunk_bytes);
    }
    heap->chunk_bytes = 0;
    heap->chunk_count = 0;
    chunk = heap->first_chunk;
    heap->first_chunk = NULL;
    heap->current_chunk = NULL;
    heap->bump = NULL;
    heap->remaining = 0;
    region = heap->memory_region;
    while (chunk) {
        next = chunk->next;
        size = chunk->size;
        AFW_MEMORY_ANNOTATE_UNROOT(chunk, size);
        if (region) {
            afw_memory_region_free(region, chunk, size, xctx);
        }
        chunk = next;
    }
}

void
afw_pool_heap_internal_teardown_store(AFW_POOL_SELF_T *self, afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *parent;
    afw_boolean_t parent_destroying;
    afw_boolean_t holds_parent;

    parent = self->parent;
    parent_destroying = parent && parent->destroying;
    holds_parent = self->holds_parent;
    self->holds_parent = false;
    afw_pool_internal_unlink_from_parent(self, xctx);
    afw_pool_internal_account_destroy(self, xctx);
    /*
     * Give back the parent reference before free_chunks: this struct
     * and an xctx pool's xctx live in these chunks, and a parent
     * release reads xctx. Only a pool destroyed while referenced still
     * holds one here.
     */
    if (holds_parent && parent && !parent_destroying) {
        afw_pool_release(&parent->pub, xctx);
    }
    impl_heap_free_chunks(afw_pool_heap_internal_as_heap(self), xctx);
}

/*
 * Implementation of method release for interface afw_pool.
 */
const afw_pool_t *
afw_pool_heap_internal_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_Z(minimal, "release");
    return afw_pool_internal_release_common(self, xctx, afw_pool_heap_internal_teardown_store);
}


/*
 * Implementation of method run_cleanups for interface afw_pool.
 */
void
afw_pool_heap_internal_run_cleanups(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_Z(minimal, "run_cleanups");
    if (!self->destroying) {
        afw_pool_internal_mark_destroying(self);
    }
    afw_pool_internal_run_child_cleanups(self, xctx);
    afw_pool_internal_run_cleanups(self, xctx);
}

/*
 * Implementation of method destroy for interface afw_pool.
 */
void
afw_pool_heap_internal_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_Z(minimal, "destroy");
    if (!self->destroying) {
        afw_pool_internal_mark_destroying(self);
    }
    afw_pool_internal_destroy_children(self, xctx);
    afw_pool_heap_internal_teardown_store(self, xctx);
}

static void *
impl_heap_malloc_internal(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx,
    afw_boolean_t unhandled)
{
    void *start;
    void *user;
    afw_size_t total;
    afw_boolean_t reused;

    if (size == 0) {
        if (unhandled) {
            return NULL;
        }
        AFW_THROW_ERROR_Z(general,
            "Attempt to allocate memory for a size of 0",
            xctx);
    }

    total = afw_pool_heap_internal_block_bytes(AFW_POOL_HEAP_INTERNAL_PREFIX_BYTES, size,
        xctx, unhandled);
    if (unhandled && total == 0) {
        return NULL;
    }

    start = afw_pool_heap_internal_take_from_free_list_or_chunk(afw_pool_heap_internal_as_heap(self),
        total, &reused, xctx, unhandled);
    if (!start) {
        return NULL;
    }
    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_FZ(detail, "alloc %s " AFW_SIZE_T_FMT,
        reused ? "reuse" : "chunk", size);
    if (xctx) {
        afw_pool_internal_account_alloc(self, total, xctx);
    }
    user = AFW_POOL_HEAP_INTERNAL_USER_FROM_START(start);
    AFW_MEMORY_ANNOTATE_NOACCESS((char *)user + size,
        ((char *)start + total) - ((char *)user + size));
    afw_pool_internal_debug_prefix_set(self, user, size);
    return user;
}

/*
 * Implementation of method calloc for interface afw_pool.
 */
void *
afw_pool_heap_internal_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    result = impl_heap_malloc_internal(self, size, xctx, false);
    memset(result, 0, size);
    return result;
}

/*
 * Implementation of method malloc for interface afw_pool.
 */
void *
afw_pool_heap_internal_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    return impl_heap_malloc_internal(self, size, xctx, false);
}

void *
afw_pool_heap_internal_malloc_no_throw(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    return impl_heap_malloc_internal(self, size, xctx, true);
}

void *
afw_pool_heap_internal_calloc_no_throw(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    result = impl_heap_malloc_internal(self, size, xctx, true);
    if (result) {
        memset(result, 0, size);
    }
    return result;
}

/*
 * Implementation of method free_memory for interface afw_pool.
 */
static void
impl_heap_free_internal(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx,
    afw_boolean_t no_throw)
{
    void *start;
    afw_size_t total;

    if (!address) {
        AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_Z(detail, "free");
        return;
    }
    start = AFW_POOL_HEAP_INTERNAL_ALLOC_START(address);
    /*
     * Before the debug prefix check: the free overlay covers
     * [size][pool], so a second free would report the wrong pool.
     */
    if (impl_block_is_free(start)) {
        if (no_throw) {
            return;
        }
        AFW_THROW_ERROR_Z(general,
            "afw_pool_free_memory: already freed",
            xctx);
    }
    if (no_throw) {
        if (!afw_pool_internal_debug_prefix_ok(self, address, size)) {
            return;
        }
    }
    else {
        afw_pool_internal_debug_check_prefix(self, address, size, xctx);
    }
    afw_pool_internal_debug_poison_user(address, size);
    total = afw_pool_heap_internal_block_bytes(AFW_POOL_HEAP_INTERNAL_PREFIX_BYTES, size,
        xctx, no_throw);
    if (no_throw && total == 0) {
        return;
    }
    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_FZ(
        detail, "free %p " AFW_SIZE_T_FMT,
        address, total);
    afw_pool_internal_account_free(self, total, xctx);
    afw_pool_heap_internal_add_to_free_list(afw_pool_heap_internal_as_heap(self),
        start, total, impl_block_chunk(start), xctx);
}

void
afw_pool_heap_internal_free_memory(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    impl_heap_free_internal(self, address, size, xctx, false);
}

void
afw_pool_heap_internal_free_memory_no_throw(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    impl_heap_free_internal(self, address, size, xctx, true);
}

/*
 * Implementation of method garbage_collect for interface afw_pool.
 * Heap: free_memory already returned blocks.
 */
void
afw_pool_heap_internal_garbage_collect(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_Z(minimal, "garbage_collect");
    (void)self;
    (void)xctx;
}

#undef impl_afw_pool_get_reference
#undef impl_afw_pool_get_reference_count
#undef impl_afw_pool_for_each_reference
#undef impl_afw_pool_release_references
#undef impl_afw_pool_calloc
#undef impl_afw_pool_malloc
#undef impl_afw_pool_free_memory
#undef impl_afw_pool_free_memory_no_throw
#undef impl_afw_pool_calloc_no_throw
#undef impl_afw_pool_malloc_no_throw
#undef impl_afw_pool_register_cleanup
#undef impl_afw_pool_garbage_collect

afw_boolean_t
afw_pool_heap_internal_is_multithreaded(const afw_pool_t *p)
{
    return p && p->inf == &afw_pool_heap_internal_multithreaded_inf;
}

const afw_pool_t *
afw_pool_heap_internal_create_st(
    const afw_pool_t *parent,
    afw_boolean_t as_managed_p,
    afw_size_t chunk_min,
    afw_xctx_t *xctx)
{
    AFW_POOL_SELF_T *self;

    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required", xctx);
    }
    self = afw_pool_heap_internal_create_self(parent, &impl_afw_pool_inf,
        as_managed_p, chunk_min,
        sizeof(afw_pool_heap_internal_self_with_free_memory_head_t),
        NULL, xctx);
    return &self->pub;
}


const afw_pool_t *
afw_pool_heap_internal_create_st_for_thread(
    const afw_pool_t *parent,
    afw_boolean_t as_managed_p,
    afw_size_t chunk_min,
    const afw_thread_t *thread,
    afw_xctx_t *xctx)
{
    AFW_POOL_SELF_T *self;

    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required", xctx);
    }
    if (!thread || !thread->memory_region) {
        AFW_THROW_ERROR_Z(general,
            "Heap requires thread->memory_region", xctx);
    }
    self = afw_pool_heap_internal_create_self(parent, &impl_afw_pool_inf, as_managed_p,
        chunk_min,
        sizeof(afw_pool_heap_internal_self_with_free_memory_head_t),
        thread, xctx);
    return &self->pub;
}


AFW_DEFINE(const afw_pool_t *)
afw_pool_heap_create(
    const afw_pool_t *parent,
    afw_size_t chunk_min,
    afw_xctx_t *xctx)
{
    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required", xctx);
    }
    if (afw_pool_internal_is_multithreaded(parent)) {
        return afw_pool_heap_internal_multithreaded_create(
            parent, false, chunk_min, xctx);
    }
    return afw_pool_heap_internal_create_st(
        parent, false, chunk_min, xctx);
}


AFW_DEFINE(const afw_pool_t *)
afw_pool_heap_create_as_managed_p(
    const afw_pool_t *parent,
    afw_size_t chunk_min,
    afw_xctx_t *xctx)
{
    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required", xctx);
    }
    if (afw_pool_internal_is_multithreaded(parent)) {
        return afw_pool_heap_internal_multithreaded_create(
            parent, true, chunk_min, xctx);
    }
    return afw_pool_heap_internal_create_st(
        parent, true, chunk_min, xctx);
}

const afw_pool_t *
afw_pool_heap_internal_create_base_pool(
    const afw_thread_t *thread,
    const afw_memory_region_t *mt_region)
{
    afw_pool_internal_self_t *self;
    afw_pool_heap_internal_self_t *heap;

    if (!thread || !mt_region) {
        return NULL;
    }
    /*
     * One thread exists. Take the lock anyway so this get matches
     * every later multithreaded create.
     */
    afw_memory_region_lock(mt_region, NULL);
    self = afw_pool_heap_internal_allocate_self(
        &afw_pool_heap_internal_multithreaded_inf,
        AFW_ENVIRONMENT_DEFAULT_CHUNK_MIN,
        sizeof(afw_pool_heap_internal_self_with_free_memory_head_t),
        mt_region, NULL);
    afw_memory_region_unlock(mt_region, NULL);
    if (!self) {
        return NULL;
    }
    self->pool_number = 1;
    self->thread = thread;
    heap = afw_pool_heap_internal_as_heap(self);
    heap->memory_region = mt_region;
    afw_pool_heap_internal_init_bins(heap);
    impl_base_pool_self = heap;
    return &self->pub;
}


AFW_DEFINE(afw_thread_t *)
afw_pool_thread_create(
    afw_size_t size,
    afw_xctx_t *xctx)
{
    AFW_POOL_SELF_T *self;
    afw_thread_t *thread;
    const afw_memory_region_t *region;

    if (size == (afw_size_t)-1 || size < sizeof(afw_thread_t)) {
        size = sizeof(afw_thread_t);
    }

    /*
     * Same order as the base thread: region and thread exist
     * before the ST heap so the first chunk is get(). Thread
     * struct is C calloc (not in the heap). CATCH releases both
     * if heap create throws.
     */
    thread = (afw_thread_t *)calloc(1, size);
    if (!thread) {
        AFW_THROW_ERROR_Z(memory,
            "Unable to allocate thread", xctx);
    }
    region = afw_memory_region_create(
        xctx->env->memory_region_free_list_max_bytes, xctx);
    if (!region) {
        free(thread);
        AFW_THROW_ERROR_Z(memory,
            "Unable to allocate memory_region", xctx);
    }
    afw_memory_region_configure(region, xctx->env, xctx);
    thread->memory_region = region;
    AFW_TRY {
        self = afw_pool_heap_internal_create_self(xctx->p,
            &impl_afw_pool_inf, true,
            xctx->env->xctx_chunk_min,
            sizeof(afw_pool_heap_internal_self_with_free_memory_head_t),
            thread, xctx);
        impl_pool_set_owning_thread(
            afw_pool_heap_internal_as_heap(self), thread);
        thread->p = &self->pub;
    }
    AFW_CATCH_UNHANDLED {
        afw_memory_region_release(region, xctx);
        free(thread);
        AFW_ERROR_RETHROW;
    }
    AFW_ENDTRY;

    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_FZ(minimal,
        "thread_create " AFW_SIZE_T_FMT,
        size);

    return thread;
}

AFW_DEFINE(afw_size_t)
afw_pool_chunk_bytes(const afw_pool_t *instance)
{
    if (!instance || !afw_pool_internal_is_heap(instance)) {
        return 0;
    }
    return afw_pool_heap_internal_as_heap(instance)->chunk_bytes;
}


AFW_DEFINE(afw_size_t)
afw_pool_chunk_count(const afw_pool_t *instance)
{
    if (!instance || !afw_pool_internal_is_heap(instance)) {
        return 0;
    }
    return afw_pool_heap_internal_as_heap(instance)->chunk_count;
}
