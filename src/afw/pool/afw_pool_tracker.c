// See the 'COPYING' file in the project root for licensing information.
/*
 * Tracker pool implementation.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_pool_tracker.c
 * @brief Tracker allocated list, mark/collect, tracker infs.
 *
 * A tracker gets blocks from the ancestor heap store, tracks live
 * USER blocks, and returns them on destroy or garbage_collect.
 * Shared lifetime is `afw_pool.c`. Heap store is `afw_pool_heap.c`.
 * The multithreaded inf is `afw_pool_tracker_multithreaded.c`.
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

#define AFW_IMPLEMENTATION_ID "tracker"
#define AFW_IMPLEMENTATION_INF_LABEL impl_afw_pool_tracker_inf
#define impl_afw_pool_release afw_pool_internal_tracker_release
#define impl_afw_pool_run_cleanups afw_pool_internal_tracker_run_cleanups
#define impl_afw_pool_destroy afw_pool_internal_tracker_destroy
#define impl_afw_pool_calloc afw_pool_internal_tracker_calloc
#define impl_afw_pool_malloc afw_pool_internal_tracker_malloc
#define impl_afw_pool_free_memory afw_pool_internal_tracker_free_memory
#define impl_afw_pool_garbage_collect afw_pool_internal_tracker_garbage_collect
#define impl_afw_pool_calloc_no_throw afw_pool_internal_tracker_calloc_no_throw
#define impl_afw_pool_malloc_no_throw afw_pool_internal_tracker_malloc_no_throw
#define impl_afw_pool_free_memory_no_throw \
    afw_pool_internal_tracker_free_memory_no_throw

static const afw_pool_internal_inf_implementation_specific_t
impl_tracker_implementation_specific =
    {
        /* multithreaded */ false,
        /* tracker */ true
    };

#define AFW_IMPLEMENTATION_SPECIFIC &impl_tracker_implementation_specific

#include "afw_pool_impl_declares.h"
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef AFW_IMPLEMENTATION_SPECIFIC
#undef impl_afw_pool_release
#undef impl_afw_pool_run_cleanups
#undef impl_afw_pool_destroy
#undef impl_afw_pool_calloc
#undef impl_afw_pool_malloc
#undef impl_afw_pool_free_memory
#undef impl_afw_pool_garbage_collect
#undef impl_afw_pool_calloc_no_throw
#undef impl_afw_pool_malloc_no_throw
#undef impl_afw_pool_free_memory_no_throw


/* Create skeleton pool struct for tracker. */
afw_pool_internal_self_t *
afw_pool_internal_tracker_create(
    afw_pool_internal_self_t *parent,
    const afw_pool_inf_t *inf,
    afw_size_t self_bytes,
    afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *self;

    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required for tracker", xctx);
    }

    /*
     * Header is a parent-pool user block so destroy can free_memory
     * it. Not on this tracker’s allocated list.
     */
    self = afw_pool_calloc(&parent->pub, self_bytes, xctx);
    self->pub.inf = inf;
    self->pub.managed_p = parent->pub.managed_p
        ? parent->pub.managed_p
        : &parent->pub;
    self->thread = parent->thread;
    afw_pool_internal_assign_pool_number(self);
    self->reference_count = 1;
    afw_pool_internal_link_as_child(parent, self, xctx);

    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_Z(minimal, "create");

    return self;
}

static void
impl_tracker_return_leftovers(
    afw_pool_tracker_internal_self_t *tracker, afw_xctx_t *xctx)
{
    afw_pool_heap_internal_self_t *heap;
    afw_pool_tracker_internal_node_t *curr;
    afw_pool_tracker_internal_node_t *next;

    heap = afw_pool_heap_internal_reservoir_heap(&tracker->common);
    curr = tracker->first_allocated_memory;
    tracker->first_allocated_memory = NULL;
    while (curr) {
        next = AFW_POOL_TRACKER_INTERNAL_NEXT(curr);
        afw_pool_internal_debug_poison_user(AFW_POOL_TRACKER_INTERNAL_TO_USER(curr),
            AFW_POOL_TRACKER_INTERNAL_USER_SIZE(curr));
        afw_pool_heap_internal_add_to_free_list(heap, curr,
            afw_pool_heap_internal_block_bytes(AFW_POOL_TRACKER_INTERNAL_PREFIX_BYTES,
                AFW_POOL_TRACKER_INTERNAL_USER_SIZE(curr), xctx, false),
            xctx);
        curr = next;
    }
}


static void
impl_tracker_teardown_store(
    AFW_POOL_SELF_T *self,
    afw_size_t self_bytes,
    afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *parent;
    afw_boolean_t parent_destroying;

    afw_integer_t parent_pins;

    parent = self->parent;
    parent_destroying = parent && parent->destroying;
    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Tracker has no parent", xctx);
    }
    parent_pins = self->parent_pins;
    self->parent_pins = 0;
    afw_pool_internal_unlink_from_parent(self, xctx);
    impl_tracker_return_leftovers(afw_pool_tracker_internal_as_tracker(self), xctx);
    afw_pool_internal_account_destroy(self, xctx);
    afw_pool_free_memory(&parent->pub, self, self_bytes, xctx);
    /* self is back in the parent. A release at parent ref 1 frees it. */
    if (!parent_destroying) {
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
}


static void
impl_tracker_teardown(AFW_POOL_SELF_T *self, afw_xctx_t *xctx)
{
    impl_tracker_teardown_store(self,
        sizeof(afw_pool_tracker_internal_self_t), xctx);
}

/* --------------------------- tracker implementations ---------------------- */

const afw_pool_t *
afw_pool_internal_tracker_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_Z(minimal, "release");
    return afw_pool_internal_release_common(self, xctx, impl_tracker_teardown);
}

void
afw_pool_internal_tracker_run_cleanups(
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

void
afw_pool_internal_tracker_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_Z(minimal, "destroy");
    if (!self->destroying) {
        afw_pool_internal_mark_destroying(self);
    }
    afw_pool_internal_destroy_children(self, xctx);
    impl_tracker_teardown(self, xctx);
}

static void *
impl_tracker_malloc_internal(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx,
    afw_boolean_t unhandled)
{
    afw_pool_tracker_internal_self_t *tracker;
    void *start;
    void *user;
    afw_pool_tracker_internal_node_t *node;
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

    total = afw_pool_heap_internal_block_bytes(AFW_POOL_TRACKER_INTERNAL_PREFIX_BYTES, size,
        xctx, unhandled);
    if (unhandled && total == 0) {
        return NULL;
    }

    start = afw_pool_heap_internal_take_from_free_list_or_chunk(
        afw_pool_heap_internal_reservoir_heap(self), total, &reused, xctx, unhandled);
    if (!start) {
        return NULL;
    }
    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_FZ(detail, "alloc %s " AFW_SIZE_T_FMT,
        reused ? "reuse" : "chunk", size);
    tracker = afw_pool_tracker_internal_as_tracker(self);
    node = (afw_pool_tracker_internal_node_t *)start;
    node->next = tracker->first_allocated_memory;
    tracker->first_allocated_memory = node;
    user = AFW_POOL_TRACKER_INTERNAL_TO_USER(node);
#ifdef AFW_DEBUG_POOL
    node->debug.size = size;
    node->debug.pool = &self->pub;
#else
    node->size = size;
#endif
    if (xctx) {
        afw_pool_internal_account_alloc(self, total, xctx);
    }
    return user;
}


void *
afw_pool_internal_tracker_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    result = impl_tracker_malloc_internal(self, size, xctx, false);
    memset(result, 0, size);
    return result;
}


void *
afw_pool_internal_tracker_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    return impl_tracker_malloc_internal(self, size, xctx, false);
}


void *
afw_pool_internal_tracker_malloc_no_throw(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    return impl_tracker_malloc_internal(self, size, xctx, true);
}


void *
afw_pool_internal_tracker_calloc_no_throw(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    result = impl_tracker_malloc_internal(self, size, xctx, true);
    if (result) {
        memset(result, 0, size);
    }
    return result;
}


static void
impl_tracker_free_internal(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx,
    afw_boolean_t no_throw)
{
    afw_pool_tracker_internal_node_t *node;
    afw_size_t total;

    if (!address) {
        AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_Z(detail, "free");
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
    node = AFW_POOL_TRACKER_INTERNAL_NODE(address);
    if (AFW_POOL_TRACKER_INTERNAL_IS_FREED(node)) {
        if (no_throw) {
            return;
        }
        AFW_THROW_ERROR_Z(general,
            "afw_pool_free_memory: already freed",
            xctx);
    }
    afw_pool_internal_debug_poison_user(address, size);
    total = afw_pool_heap_internal_block_bytes(AFW_POOL_TRACKER_INTERNAL_PREFIX_BYTES, size,
        xctx, no_throw);
    if (no_throw && total == 0) {
        return;
    }
    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_FZ(
        detail, "free %p " AFW_SIZE_T_FMT,
        address, total);
    afw_pool_internal_account_free(self, total, xctx);
    AFW_POOL_TRACKER_INTERNAL_MARK_FREED(node);
}

void
afw_pool_internal_tracker_free_memory(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    impl_tracker_free_internal(self, address, size, xctx, false);
}

void
afw_pool_internal_tracker_free_memory_no_throw(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    impl_tracker_free_internal(self, address, size, xctx, true);
}


void
afw_pool_internal_tracker_garbage_collect(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    afw_pool_tracker_internal_self_t *tracker;
    afw_pool_heap_internal_self_t *heap;
    afw_pool_tracker_internal_node_t *prev;
    afw_pool_tracker_internal_node_t *curr;
    afw_pool_tracker_internal_node_t *next;

    AFW_POOL_INTERNAL_PRINT_DEBUG_INFO_Z(minimal, "garbage_collect");
    tracker = afw_pool_tracker_internal_as_tracker(self);
    heap = afw_pool_heap_internal_reservoir_heap(self);
    prev = NULL;
    curr = tracker->first_allocated_memory;
    while (curr) {
        next = AFW_POOL_TRACKER_INTERNAL_NEXT(curr);
        if (AFW_POOL_TRACKER_INTERNAL_IS_FREED(curr)) {
            if (prev) {
                prev->next = next;
            }
            else {
                tracker->first_allocated_memory = next;
            }
            afw_pool_heap_internal_add_to_free_list(heap, curr,
                afw_pool_heap_internal_block_bytes(AFW_POOL_TRACKER_INTERNAL_PREFIX_BYTES,
                    AFW_POOL_TRACKER_INTERNAL_USER_SIZE(curr), xctx, false),
                xctx);
        }
        else {
            prev = curr;
        }
        curr = next;
    }
}

AFW_DEFINE(const afw_pool_t *)
afw_pool_tracker_create(
    const afw_pool_t *parent, afw_xctx_t *xctx)
{
    AFW_POOL_SELF_T *self;
    AFW_POOL_SELF_T *parent_self;

    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required", xctx);
    }
    if (!afw_pool_internal_is_heap(parent) &&
        !afw_pool_internal_is_tracker(parent))
    {
        AFW_THROW_ERROR_Z(general,
            "afw_pool_tracker_create() parent must be a heap or tracker",
            xctx);
    }

    parent_self = (AFW_POOL_SELF_T *)parent;
    if (afw_pool_internal_is_multithreaded(parent)) {
        self = afw_pool_internal_tracker_multithreaded_create_self(parent_self,
            sizeof(afw_pool_tracker_internal_self_t), xctx);
    }
    else {
        self = afw_pool_internal_tracker_create(parent_self,
            &impl_afw_pool_tracker_inf,
            sizeof(afw_pool_tracker_internal_self_t), xctx);
    }
    return &self->pub;
}
