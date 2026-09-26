// See the 'COPYING' file in the project root for licensing information.
/*
 * Heap and scope pool implementation.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_pool_heap.c
 * @brief Heap store, scope last-release delay, heap/scope infs.
 *
 * Chunks, bump, free list, and heap malloc/free. Scope is a heap
 * with compile-sized chunks plus throw last-release delay.
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

const afw_pool_t *
impl_heap_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);
#define impl_afw_pool_release impl_heap_afw_pool_release

void
impl_heap_afw_pool_run_cleanups(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);
#define impl_afw_pool_run_cleanups impl_heap_afw_pool_run_cleanups

void
impl_heap_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);
#define impl_afw_pool_destroy impl_heap_afw_pool_destroy

void
impl_heap_afw_pool_garbage_collect(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);
#define impl_afw_pool_garbage_collect impl_heap_afw_pool_garbage_collect

#define impl_afw_pool_calloc afw_pool_heap_calloc
#define impl_afw_pool_malloc afw_pool_heap_malloc
#define impl_afw_pool_free_memory afw_pool_heap_free_memory
#define impl_afw_pool_free_memory_no_throw \
    afw_pool_heap_free_memory_no_throw
#define impl_afw_pool_calloc_no_throw afw_pool_heap_calloc_no_throw
#define impl_afw_pool_malloc_no_throw afw_pool_heap_malloc_no_throw

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
    afw_pool_internal_heap_self_t *heap,
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
    afw_pool_internal_heap_self_t *heap, afw_size_t size, afw_xctx_t *xctx)
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

afw_pool_internal_heap_self_t *
afw_pool_heap_internal_reservoir_heap(afw_pool_internal_self_t *self)
{
    /* A heap is its own store even when it has an AFW parent. */
    while (self->parent && !afw_pool_internal_is_heap(&self->pub)) {
        self = self->parent;
    }
    return afw_pool_heap_internal_as_heap(self);
}


const afw_memory_region_t *
afw_pool_internal_memory_region(const afw_pool_internal_self_t *self)
{
    afw_pool_internal_heap_self_t *heap;

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
    ((char *)(_chunk) + AFW_POOL_ALIGN_UP(sizeof(afw_pool_chunk_t)))

#define impl_chunk_end(_chunk) \
    ((char *)(_chunk) + (_chunk)->size)

#define AFW_POOL_BLOCK_FREE_BIT ((uintptr_t)1)

#define impl_block_chunk(_start) \
    ((afw_pool_chunk_t *)(((uintptr_t) \
        ((afw_pool_free_node_t *)(_start))->chunk) & \
        ~AFW_POOL_BLOCK_FREE_BIT))

#define impl_block_set_chunk(_start, _chunk) \
    (((afw_pool_free_node_t *)(_start))->chunk = (_chunk))

#define impl_block_mark_free(_start) \
    (((afw_pool_free_node_t *)(_start))->chunk = \
        (afw_pool_chunk_t *)(((uintptr_t)impl_block_chunk(_start)) | \
            AFW_POOL_BLOCK_FREE_BIT))

#define impl_block_is_free(_start) \
    ((((uintptr_t)((afw_pool_free_node_t *)(_start))->chunk) & \
        AFW_POOL_BLOCK_FREE_BIT) != 0)

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
    if (size < AFW_POOL_CHUNK_ALIGN) {
        return AFW_POOL_CHUNK_ALIGN;
    }
    rem = size & (AFW_POOL_CHUNK_ALIGN - 1);
    if (rem == 0) {
        return size;
    }
    add = AFW_POOL_CHUNK_ALIGN - rem;
    if (size > AFW_SIZE_T_MAX - add) {
        return AFW_SIZE_T_MAX & ~(AFW_POOL_CHUNK_ALIGN - 1);
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

    header = AFW_POOL_ALIGN_UP(sizeof(afw_pool_chunk_t));
    if (min_payload > AFW_SIZE_T_MAX - header) {
        return 0;
    }
    need = header + min_payload;
    if (need < chunk_min) {
        need = chunk_min;
    }
    return afw_pool_round_up_chunk_size(need);
}


static afw_pool_chunk_t *
impl_chunk_malloc(
    afw_size_t min_payload,
    afw_size_t chunk_min,
    const afw_memory_region_t *region,
    afw_xctx_t *xctx)
{
    afw_size_t need;
    void *mem;
    afw_pool_chunk_t *chunk;

    need = impl_chunk_need(min_payload, chunk_min);
    if (need == 0 || !region) {
        return NULL;
    }
    mem = NULL;
    afw_memory_region_get(region, &mem, &need, xctx);
    if (!mem) {
        return NULL;
    }
    chunk = (afw_pool_chunk_t *)mem;
    chunk->next = NULL;
    chunk->size = need;
    return chunk;
}


static afw_pool_internal_self_t *
impl_heap_allocate_self(
    const afw_pool_inf_t *inf,
    afw_size_t chunk_min,
    afw_size_t self_bytes,
    const afw_memory_region_t *region,
    afw_xctx_t *xctx)
{
    afw_size_t min_self;
    afw_pool_chunk_t *chunk;
    afw_pool_internal_heap_self_t *heap;
    afw_pool_internal_self_t *self;
    char *usable;
    char *after_self;
    const afw_environment_t *env;

    env = (xctx && xctx->env) ? xctx->env : NULL;
    chunk_min = impl_normalize_chunk_min(chunk_min, env);
    min_self = sizeof(afw_pool_internal_self_with_free_memory_head_t);
    if (self_bytes < min_self) {
        self_bytes = min_self;
    }
    self_bytes = AFW_POOL_ALIGN_UP(self_bytes);
    chunk = impl_chunk_malloc(self_bytes, chunk_min, region, xctx);
    if (!chunk) {
        return NULL;
    }
    usable = impl_chunk_usable(chunk);
    memset(usable, 0, self_bytes);
    heap = (afw_pool_internal_heap_self_t *)(void *)usable;
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
    /* Same overlay as afw_pool_internal_self_with_free_memory_head_t. */
    heap->free_memory_head = (afw_pool_internal_free_memory_head_t *)
        (usable + offsetof(afw_pool_internal_self_with_free_memory_head_t,
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
static afw_pool_internal_heap_self_t *impl_base_pool_self;

/* Create skeleton heap struct. Parent is any AFW pool. */
afw_pool_internal_self_t *
afw_pool_heap_create_self(
    const afw_pool_t *afw_parent,
    const afw_pool_inf_t *inf,
    afw_boolean_t as_managed_p,
    afw_size_t chunk_min,
    afw_size_t self_bytes,
    const afw_thread_t *thread,
    afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *self;
    afw_pool_internal_heap_self_t *heap;
    afw_pool_internal_self_t *parent_self;
    const afw_pool_internal_inf_implementation_specific_t *spec;
    const afw_memory_region_t *region;
    afw_boolean_t multithreaded;

    if (!thread && xctx) {
        thread = xctx->thread;
    }
    if (!thread) {
        if (!xctx) {
            return NULL;
        }
        AFW_THROW_ERROR_Z(general, "Heap requires a thread", xctx);
    }
    spec = inf ? inf->rti.implementation_specific : NULL;
    multithreaded = spec && spec->is_multithreaded;
    if (multithreaded) {
        if (!xctx || !xctx->env) {
            if (!xctx) {
                return NULL;
            }
            AFW_THROW_ERROR_Z(general,
                "Multithreaded heap requires the environment region",
                xctx);
        }
        region = ((const afw_environment_t *)xctx->env)
            ->multithreaded_memory_region;
        if (!region) {
            AFW_THROW_ERROR_Z(general,
                "Multithreaded heap requires the environment region",
                xctx);
        }
        afw_memory_region_lock(region, xctx);
    }
    else {
        region = thread->memory_region;
        if (!region) {
            if (!xctx) {
                return NULL;
            }
            AFW_THROW_ERROR_Z(general,
                "Heap requires thread->memory_region", xctx);
        }
    }
    self = impl_heap_allocate_self(inf, chunk_min, self_bytes,
        region, xctx);
    if (!self) {
        if (multithreaded) {
            afw_memory_region_unlock(region, xctx);
        }
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

    if (multithreaded) {
        afw_memory_region_unlock(region, xctx);
    }

    IMPL_PRINT_DEBUG_INFO_Z(minimal, "create");

    return self;
}

/*
 * First-fit on a LIFO free list. Overlay lives only on freed
 * blocks. Remainder too small to hold a free node is left on the
 * list so total is always recoverable as prefix + USER size.
 *
 * largest is an upper bound. A take of a max-sized node sets it to
 * AFW_SIZE_T_MAX until a full miss walk records the new maximum.
 * Allocations larger than a known maximum skip the walk.
 */
static void
impl_largest_before_unlink(
    afw_pool_internal_free_memory_head_t *head,
    const afw_pool_free_node_t *node)
{
    if (head->largest != AFW_SIZE_T_MAX &&
        node->total >= head->largest)
    {
        head->largest = AFW_SIZE_T_MAX;
    }
}

static void
impl_largest_note(
    afw_pool_internal_free_memory_head_t *head,
    afw_size_t total)
{
    if (head->largest != AFW_SIZE_T_MAX && total > head->largest) {
        head->largest = total;
    }
}

static void
impl_heap_free_unlink(
    afw_pool_free_node_t **head,
    afw_pool_free_node_t *node)
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
    if (need < sizeof(afw_pool_free_node_t)) {
        need = sizeof(afw_pool_free_node_t);
    }
    aligned = AFW_POOL_ALIGN_UP(need);
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
    afw_boolean_t unhandled)
{
    afw_pool_internal_free_memory_head_t *head;
    afw_pool_free_node_t *curr;
    afw_pool_free_node_t *prev;
    afw_pool_free_node_t *next;
    afw_pool_free_node_t *rest;
    afw_pool_free_node_t *slow;
    afw_pool_free_node_t *fast;
    afw_pool_chunk_t *chunk;
    const afw_memory_region_t *region;
    char *end;
    void *start;
    afw_size_t seen_max;

    head = heap->free_memory_head;
    curr = NULL;
    /*
     * largest < total means no free node can satisfy this request.
     * AFW_SIZE_T_MAX is unknown, so that still walks.
     */
    if (head && head->first && head->largest >= total) {
        seen_max = 0;
        slow = head->first;
        fast = slow;
        for (curr = slow; curr; curr = curr->next) {
            if (curr->total > seen_max) {
                seen_max = curr->total;
            }
            if (curr->total >= total &&
                (curr->total == total ||
                    curr->total - total >= sizeof(afw_pool_free_node_t)))
            {
                break;
            }
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
        if (curr->total - total >= sizeof(afw_pool_free_node_t)) {
            rest = (afw_pool_free_node_t *)(((char *)curr) + total);
            rest->total = curr->total - total;
            rest->chunk = impl_block_chunk(curr);
            impl_block_mark_free(rest);
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
        else if (!head->first) {
            head->largest = 0;
        }
        impl_block_set_chunk(curr, impl_block_chunk(curr));
        *reused = true;
        return curr;
    }

    *reused = false;
    if (heap->remaining >= total) {
        start = heap->bump;
        heap->bump += total;
        heap->remaining -= total;
        impl_block_set_chunk(start, heap->current_chunk);
        return start;
    }

    if (heap->current_chunk &&
        heap->remaining >= sizeof(afw_pool_free_node_t))
    {
        impl_block_set_chunk(heap->bump, heap->current_chunk);
        afw_pool_heap_internal_add_to_free_list(heap, heap->bump,
            heap->remaining, xctx);
    }
    heap->bump = NULL;
    heap->remaining = 0;

    if (!unhandled && xctx->error_processing_count == 0) {
        afw_xctx_check_resource_limits(xctx, 0);
        if (heap->common.thread &&
            (heap->common.thread->type == afw_thread_type_request ||
                (xctx->env->limit_request_pool_apply_to_base &&
                    xctx != ((const afw_environment_internal_t *)
                        xctx->env)->base_xctx)))
        {
            afw_size_t limit;
            afw_size_t asked;

            limit = xctx->env->limit_request_pool_bytes;
            asked = heap->common.thread->pool_bytes_in_use;
            if (limit != 0 &&
                (asked >= limit || total > limit - asked))
            {
                AFW_THROW_ERROR_Z(payload_too_large,
                    "Request pool limit exceeded.", xctx);
            }
        }
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
    impl_block_set_chunk(start, chunk);
    return start;
}


void
afw_pool_heap_internal_add_to_free_list(
    afw_pool_internal_heap_self_t *heap,
    void *start,
    afw_size_t total,
    afw_xctx_t *xctx)
{
    afw_pool_free_node_t *freeing;
    afw_pool_internal_free_memory_head_t *head;

    (void)xctx;
    head = heap->free_memory_head;
    if (!head) {
        return;
    }

    freeing = (afw_pool_free_node_t *)start;
    freeing->total = total;
    impl_block_mark_free(freeing);

    {
        char *nstart;
        afw_pool_chunk_t *chunk;
        afw_pool_free_node_t *nxt;

        chunk = impl_block_chunk(freeing);
        nstart = ((char *)freeing) + freeing->total;
        /*
         * Unused bump in the current chunk is not a block header.
         * Peeking it is an uninit read (valgrind).
         */
        if (chunk &&
            !(chunk == heap->current_chunk &&
                nstart == heap->bump) &&
            impl_addr_in_chunk(nstart, chunk,
                sizeof(afw_pool_free_node_t)) &&
            impl_block_is_free(nstart) &&
            impl_block_chunk(nstart) == chunk)
        {
            nxt = (afw_pool_free_node_t *)nstart;
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
impl_heap_free_chunks(afw_pool_internal_heap_self_t *heap, afw_xctx_t *xctx)
{
    afw_pool_chunk_t *chunk;
    afw_pool_chunk_t *next;
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
        if (region) {
            afw_memory_region_free(region, chunk, size, xctx);
        }
        chunk = next;
    }
}

void
afw_pool_heap_teardown_store(AFW_POOL_SELF_T *self, afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *parent;
    afw_boolean_t parent_destroying;
    afw_integer_t parent_pins;

    parent = self->parent;
    parent_destroying = parent && parent->destroying;
    parent_pins = self->parent_pins;
    self->parent_pins = 0;
    afw_pool_internal_unlink_from_parent(self, xctx);
    afw_pool_internal_account_destroy(self, xctx);
    /*
     * Drop pins before free_chunks. This struct and an xctx pool's
     * xctx live in these chunks; a later parent release reads xctx.
     * A release that finds the parent at 1 destroys it.
     */
    if (parent && !parent_destroying) {
        while (parent_pins > 0) {
            afw_boolean_t parent_dies;

            parent_pins--;
            parent_dies = (parent->reference_count == 1);
            afw_pool_release(&parent->pub, xctx);
            if (parent_dies) {
                break;
            }
        }
    }
    impl_heap_free_chunks(afw_pool_heap_internal_as_heap(self), xctx);
}

/*
 * Implementation of method release for interface afw_pool.
 */
const afw_pool_t *
impl_heap_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "release");
    return afw_pool_internal_release_common(self, xctx, afw_pool_heap_teardown_store);
}


/*
 * Implementation of method run_cleanups for interface afw_pool.
 */
void
impl_heap_afw_pool_run_cleanups(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "run_cleanups");
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
impl_heap_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "destroy");
    if (!self->destroying) {
        afw_pool_internal_mark_destroying(self);
    }
    afw_pool_internal_destroy_children(self, xctx);
    afw_pool_heap_teardown_store(self, xctx);
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

    total = afw_pool_heap_internal_block_bytes(AFW_POOL_HEAP_PREFIX_BYTES, size,
        xctx, unhandled);
    if (unhandled && total == 0) {
        return NULL;
    }

    start = afw_pool_heap_internal_take_from_free_list_or_chunk(afw_pool_heap_internal_as_heap(self),
        total, &reused, xctx, unhandled);
    if (!start) {
        return NULL;
    }
    IMPL_PRINT_DEBUG_INFO_FZ(detail, "alloc %s " AFW_SIZE_T_FMT,
        reused ? "reuse" : "chunk", size);
    if (xctx) {
        afw_pool_internal_account_alloc(self, total, xctx);
    }
    user = AFW_POOL_HEAP_USER_FROM_START(start);
    afw_pool_internal_debug_prefix_set(self, user, size);
    return user;
}

/*
 * Implementation of method calloc for interface afw_pool.
 */
void *
afw_pool_heap_calloc(
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
afw_pool_heap_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    return impl_heap_malloc_internal(self, size, xctx, false);
}

void *
afw_pool_heap_malloc_no_throw(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    return impl_heap_malloc_internal(self, size, xctx, true);
}

void *
afw_pool_heap_calloc_no_throw(
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
        IMPL_PRINT_DEBUG_INFO_Z(detail, "free");
        return;
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
    total = afw_pool_heap_internal_block_bytes(AFW_POOL_HEAP_PREFIX_BYTES, size,
        xctx, no_throw);
    if (no_throw && total == 0) {
        return;
    }
    start = AFW_POOL_HEAP_ALLOC_START(address);
    IMPL_PRINT_DEBUG_INFO_FZ(
        detail, "free %p " AFW_SIZE_T_FMT,
        address, total);
    afw_pool_internal_account_free(self, total, xctx);
    afw_pool_heap_internal_add_to_free_list(afw_pool_heap_internal_as_heap(self), start, total, xctx);
}

void
afw_pool_heap_free_memory(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    impl_heap_free_internal(self, address, size, xctx, false);
}

void
afw_pool_heap_free_memory_no_throw(
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
impl_heap_afw_pool_garbage_collect(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "garbage_collect");
    (void)self;
    (void)xctx;
}

/*
 * Thread pool. One parent hold from create until teardown.
 * get_reference does not pin the parent, so a worker does not
 * bump the base job pool.
 */
static void
impl_thread_pool_teardown(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    self->parent_pins = 1;
    afw_pool_heap_teardown_store(self, xctx);
}

static void
impl_thread_pool_get_reference(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "get_reference");
    self->reference_count++;
}

static const afw_pool_t *
impl_thread_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "release");
    return afw_pool_internal_release_common(
        self, xctx, impl_thread_pool_teardown);
}

static void
impl_thread_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "destroy");
    if (!self->destroying) {
        afw_pool_internal_mark_destroying(self);
    }
    afw_pool_internal_destroy_children(self, xctx);
    impl_thread_pool_teardown(self, xctx);
}

#undef impl_afw_pool_get_reference
#undef impl_afw_pool_register_cleanup
#undef impl_afw_pool_deregister_cleanup

#define AFW_POOL_INF_ONLY 1
#define AFW_IMPLEMENTATION_ID "thread"
#define AFW_IMPLEMENTATION_INF_LABEL impl_afw_pool_thread_inf
#define AFW_IMPLEMENTATION_SPECIFIC &impl_pool_implementation_specific
#define impl_afw_pool_release impl_thread_pool_release
#define impl_afw_pool_get_reference impl_thread_pool_get_reference
#define impl_afw_pool_run_cleanups impl_heap_afw_pool_run_cleanups
#define impl_afw_pool_destroy impl_thread_pool_destroy
#define impl_afw_pool_calloc afw_pool_heap_calloc
#define impl_afw_pool_malloc afw_pool_heap_malloc
#define impl_afw_pool_free_memory afw_pool_heap_free_memory
#define impl_afw_pool_free_memory_no_throw \
    afw_pool_heap_free_memory_no_throw
#define impl_afw_pool_calloc_no_throw afw_pool_heap_calloc_no_throw
#define impl_afw_pool_malloc_no_throw afw_pool_heap_malloc_no_throw
#define impl_afw_pool_register_cleanup afw_pool_internal_register_cleanup
#define impl_afw_pool_deregister_cleanup \
    afw_pool_internal_deregister_cleanup
#define impl_afw_pool_garbage_collect impl_heap_afw_pool_garbage_collect

#include "afw_pool_impl_declares.h"
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef AFW_IMPLEMENTATION_SPECIFIC
#undef AFW_POOL_INF_ONLY
#undef impl_afw_pool_release
#undef impl_afw_pool_get_reference
#undef impl_afw_pool_run_cleanups
#undef impl_afw_pool_destroy
#undef impl_afw_pool_calloc
#undef impl_afw_pool_malloc
#undef impl_afw_pool_free_memory
#undef impl_afw_pool_free_memory_no_throw
#undef impl_afw_pool_calloc_no_throw
#undef impl_afw_pool_malloc_no_throw
#undef impl_afw_pool_register_cleanup
#undef impl_afw_pool_deregister_cleanup
#undef impl_afw_pool_garbage_collect

afw_boolean_t
afw_pool_heap_internal_is_multithreaded(const afw_pool_t *p)
{
    return p && p->inf == &impl_afw_pool_heap_multithreaded_inf;
}

const afw_pool_t *
afw_pool_heap_internal_create(
    const afw_pool_t *parent,
    afw_boolean_t multithreaded,
    afw_boolean_t as_managed_p,
    afw_size_t chunk_min,
    afw_xctx_t *xctx)
{
    AFW_POOL_SELF_T *self;
    const afw_pool_inf_t *inf;

    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required", xctx);
    }

    inf = multithreaded
        ? &impl_afw_pool_heap_multithreaded_inf
        : &impl_afw_pool_inf;
    self = afw_pool_heap_create_self(parent, inf, as_managed_p, chunk_min,
        sizeof(afw_pool_internal_self_with_free_memory_head_t),
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
    self = afw_pool_heap_create_self(parent, &impl_afw_pool_inf, as_managed_p,
        chunk_min,
        sizeof(afw_pool_internal_self_with_free_memory_head_t),
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
    return afw_pool_heap_internal_create(parent,
        afw_pool_internal_is_multithreaded(parent), false,
        chunk_min, xctx);
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
    return afw_pool_heap_internal_create(parent,
        afw_pool_internal_is_multithreaded(parent), true,
        chunk_min, xctx);
}

const afw_pool_t *
afw_pool_heap_internal_create_base_pool(
    const afw_thread_t *thread,
    const afw_memory_region_t *mt_region)
{
    afw_pool_internal_self_t *self;
    afw_pool_internal_heap_self_t *heap;

    if (!thread || !mt_region) {
        return NULL;
    }
    /*
     * One thread exists. Take the lock anyway so this get matches
     * every later multithreaded create.
     */
    afw_memory_region_lock(mt_region, NULL);
    self = impl_heap_allocate_self(
        &impl_afw_pool_heap_multithreaded_inf,
        AFW_ENVIRONMENT_DEFAULT_CHUNK_MIN,
        sizeof(afw_pool_internal_self_with_free_memory_head_t),
        mt_region, NULL);
    afw_memory_region_unlock(mt_region, NULL);
    if (!self) {
        return NULL;
    }
    self->pool_number = 1;
    self->thread = thread;
    heap = afw_pool_heap_internal_as_heap(self);
    heap->memory_region = mt_region;
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
    thread->memory_region = region;
    AFW_TRY {
        self = afw_pool_heap_create_self(xctx->p,
            &impl_afw_pool_thread_inf, true,
            xctx->env->xctx_chunk_min,
            sizeof(afw_pool_internal_self_with_free_memory_head_t),
            thread, xctx);
        impl_pool_set_owning_thread(
            afw_pool_heap_internal_as_heap(self), thread);
        thread->p = &self->pub;
        /*
         * One hold on the base job pool, taken on this thread.
         * The thread inf does not pin on later get_reference.
         * Teardown releases this hold.
         */
        if (self->parent) {
            afw_pool_get_reference(&self->parent->pub, xctx);
        }
    }
    AFW_CATCH_UNHANDLED {
        afw_memory_region_release(region, xctx);
        free(thread);
        AFW_ERROR_RETHROW;
    }
    AFW_ENDTRY;

    IMPL_PRINT_DEBUG_INFO_FZ(minimal,
        "thread_create " AFW_SIZE_T_FMT,
        size);

    return thread;
}

/* ---------------------------- create() -------------------------------- */

AFW_DEFINE(const afw_pool_t *)
afw_pool_create(
    const afw_pool_t *parent,
    afw_xctx_t *xctx)
{
    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required", xctx);
    }

    /*
     * Heap like the parent: ST or MT (lock wrappers). Inherits
     * managed_p. Tracker is afw_pool_tracker_create(). Job heaps
     * use *_as_managed_p.
     */
    return afw_pool_heap_internal_create(parent,
        afw_pool_internal_is_multithreaded(parent), false, 0, xctx);
}


AFW_DEFINE(const afw_pool_t *)
afw_pool_multithread_create(
    const afw_pool_t *parent,
    afw_size_t chunk_min,
    afw_xctx_t *xctx)
{
    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required", xctx);
    }
    if (!afw_pool_heap_internal_is_multithreaded(parent)) {
        AFW_THROW_ERROR_Z(general,
            "afw_pool_multithread_create() parent must be a "
            "multithreaded heap",
            xctx);
    }
    return afw_pool_heap_internal_create(parent, true, false,
        chunk_min, xctx);
}


AFW_DEFINE(const afw_pool_t *)
afw_pool_multithread_create_as_managed_p(
    const afw_pool_t *parent,
    afw_size_t chunk_min,
    afw_xctx_t *xctx)
{
    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required", xctx);
    }
    if (!afw_pool_heap_internal_is_multithreaded(parent)) {
        AFW_THROW_ERROR_Z(general,
            "afw_pool_multithread_create_as_managed_p() parent must "
            "be a multithreaded heap",
            xctx);
    }
    return afw_pool_heap_internal_create(parent, true, true,
        chunk_min, xctx);
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
