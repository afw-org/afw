// See the 'COPYING' file in the project root for licensing information.
/*
 * Heap and tracker pool implementation.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_pool.c
 * @brief Heap and tracker pool implementation.
 *
 * A pool is a heap unless it is a tracker. A tracker gets memory from
 * a heap, tracks live USER blocks, and returns them to the heap on
 * destroy (or garbage_collect for marked frees). The heap owns the
 * free list (overlay on freed blocks; else a bump from a 64k-min,
 * 4k-aligned chunk). Destroy returns chunks to the thread
 * memory_region.
 * Parent/child is lifetime only; store is the ancestor heap.
 * Last-release does not call destroy. Single-thread heaps:
 * create/use/release on one thread. Multithreaded heap is lock
 * wrappers.
 * Heap live: [USER] or [size][pool][USER] if AFW_DEBUG_POOL.
 * Tracker live: [next][size][USER], plus [pool] if debug. Low bit
 * of next marks freed. Debug free fills USER with
 * AFW_POOL_DEBUG_POISON (bad inf).
 */

#include "afw_internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/*
 * MT heap methods lock the pool's thread region (recursive so
 * get/free inside malloc are fine). ST get/free do not lock.
 */
#define IMPL_MULTITHREADED_LOCK_BEGIN(pool) \
const afw_memory_region_t *_this_region = \
    ((pool)->thread \
        ? (pool)->thread->memory_region : NULL); \
if (_this_region) { \
    afw_memory_region_lock(_this_region, xctx); \
} \
AFW_TRY

#define IMPL_MULTITHREADED_LOCK_END \
AFW_FINALLY { \
    if (_this_region) { \
        afw_memory_region_unlock(_this_region, xctx); \
    } \
} \
AFW_ENDTRY

static const afw_memory_region_t *
impl_pool_region(const afw_pool_internal_self_t *self)
{
    if (!self->thread) {
        return NULL;
    }
    return self->thread->memory_region;
}

#define impl_as_heap(self) \
    ((afw_pool_internal_heap_self_t *)(self))
#define impl_as_tracker(self) \
    ((afw_pool_internal_tracker_self_t *)(self))
#define impl_as_scope(self) \
    ((afw_pool_internal_scope_self_t *)(self))

/*
 * The pool methods begin with 'impl_afw_pool_' only.
 */
#define AFW_POOL_SELF_T afw_pool_internal_self_t

#define AFW_IMPLEMENTATION_ID "heap"

static const afw_pool_internal_inf_implementation_specific_t
impl_pool_implementation_specific =
    {
        /* multithreaded */ false,
        /* tracker */ false
    };

#define AFW_IMPLEMENTATION_SPECIFIC &impl_pool_implementation_specific

static const afw_pool_t *
impl_heap_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);
#define impl_afw_pool_release impl_heap_afw_pool_release

static void
impl_heap_afw_pool_run_cleanups(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);
#define impl_afw_pool_run_cleanups impl_heap_afw_pool_run_cleanups

static void
impl_heap_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);
#define impl_afw_pool_destroy impl_heap_afw_pool_destroy

static void *
impl_heap_afw_pool_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx);
#define impl_afw_pool_calloc impl_heap_afw_pool_calloc

static void *
impl_heap_afw_pool_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx);
#define impl_afw_pool_malloc impl_heap_afw_pool_malloc

static void
impl_heap_afw_pool_free_memory(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx);
#define impl_afw_pool_free_memory impl_heap_afw_pool_free_memory

static void
impl_heap_afw_pool_free_memory_no_throw(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx);
#define impl_afw_pool_free_memory_no_throw \
    impl_heap_afw_pool_free_memory_no_throw

static void *
impl_heap_afw_pool_calloc_no_throw(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx);
#define impl_afw_pool_calloc_no_throw impl_heap_afw_pool_calloc_no_throw

static void *
impl_heap_afw_pool_malloc_no_throw(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx);
#define impl_afw_pool_malloc_no_throw impl_heap_afw_pool_malloc_no_throw

#include "afw_pool_impl_declares.h"
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_SPECIFIC
#undef impl_afw_pool_release
#undef impl_afw_pool_run_cleanups
#undef impl_afw_pool_destroy
#undef impl_afw_pool_calloc
#undef impl_afw_pool_malloc
#undef impl_afw_pool_free_memory
#undef impl_afw_pool_calloc_no_throw
#undef impl_afw_pool_malloc_no_throw
#undef impl_afw_pool_free_memory_no_throw

#define AFW_POOL_INF_ONLY 1

/*
 * Some tracker methods begin with 'impl_tracker_afw_pool_'; the rest
 * match the heap methods.
 */
#define AFW_IMPLEMENTATION_ID "tracker"
#define AFW_IMPLEMENTATION_INF_LABEL impl_afw_pool_tracker_inf

static const afw_pool_t *
impl_tracker_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);

#define impl_afw_pool_release \
    impl_tracker_afw_pool_release

static void
impl_tracker_afw_pool_run_cleanups(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);

#define impl_afw_pool_run_cleanups \
    impl_tracker_afw_pool_run_cleanups

static void
impl_tracker_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);

#define impl_afw_pool_destroy \
    impl_tracker_afw_pool_destroy

static void *
impl_tracker_afw_pool_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx);

#define impl_afw_pool_calloc \
    impl_tracker_afw_pool_calloc

static void *
impl_tracker_afw_pool_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx);

#define impl_afw_pool_malloc \
    impl_tracker_afw_pool_malloc

static void
impl_tracker_afw_pool_free_memory(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx);

#define impl_afw_pool_free_memory \
    impl_tracker_afw_pool_free_memory

static void
impl_tracker_afw_pool_garbage_collect(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);

#define impl_afw_pool_garbage_collect \
    impl_tracker_afw_pool_garbage_collect

static void *
impl_tracker_afw_pool_calloc_no_throw(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx);

#define impl_afw_pool_calloc_no_throw \
    impl_tracker_afw_pool_calloc_no_throw

static void *
impl_tracker_afw_pool_malloc_no_throw(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx);

#define impl_afw_pool_malloc_no_throw \
    impl_tracker_afw_pool_malloc_no_throw

static void
impl_tracker_afw_pool_free_memory_no_throw(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx);

#define impl_afw_pool_free_memory_no_throw \
    impl_tracker_afw_pool_free_memory_no_throw

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

/* --- tracker multithreaded wrappers (lock, then ST tracker) -------- */

static const afw_pool_t *
impl_mt_tracker_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    const afw_pool_t *result;

    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        result = impl_tracker_afw_pool_release(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
    return result;
}

static void
impl_mt_tracker_afw_pool_get_reference(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        impl_afw_pool_get_reference(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_tracker_afw_pool_run_cleanups(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        impl_tracker_afw_pool_run_cleanups(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_tracker_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        impl_tracker_afw_pool_destroy(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void *
impl_mt_tracker_afw_pool_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        result = impl_tracker_afw_pool_calloc(self, size, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
    return result;
}

static void *
impl_mt_tracker_afw_pool_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        result = impl_tracker_afw_pool_malloc(self, size, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
    return result;
}

static void *
impl_mt_tracker_afw_pool_calloc_no_throw(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    const afw_memory_region_t *region;
    void *result;

    region = impl_pool_region(self);
    if (region) {
        afw_memory_region_lock(region, xctx);
    }
    result = impl_tracker_afw_pool_calloc_no_throw(self, size, xctx);
    if (region) {
        afw_memory_region_unlock(region, xctx);
    }
    return result;
}

static void *
impl_mt_tracker_afw_pool_malloc_no_throw(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    const afw_memory_region_t *region;
    void *result;

    region = impl_pool_region(self);
    if (region) {
        afw_memory_region_lock(region, xctx);
    }
    result = impl_tracker_afw_pool_malloc_no_throw(self, size, xctx);
    if (region) {
        afw_memory_region_unlock(region, xctx);
    }
    return result;
}

static void
impl_mt_tracker_afw_pool_free_memory(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        impl_tracker_afw_pool_free_memory(self, address, size, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_tracker_afw_pool_free_memory_no_throw(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    const afw_memory_region_t *region;

    region = impl_pool_region(self);
    if (region) {
        afw_memory_region_lock(region, xctx);
    }
    impl_tracker_afw_pool_free_memory_no_throw(
        self, address, size, xctx);
    if (region) {
        afw_memory_region_unlock(region, xctx);
    }
}

static void
impl_mt_tracker_afw_pool_garbage_collect(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        impl_tracker_afw_pool_garbage_collect(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_tracker_afw_pool_register_cleanup(
    AFW_POOL_SELF_T *self,
    void *data,
    void *data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        impl_afw_pool_register_cleanup(
            self, data, data2, cleanup, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_tracker_afw_pool_deregister_cleanup(
    AFW_POOL_SELF_T *self,
    void *data,
    void *data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        impl_afw_pool_deregister_cleanup(
            self, data, data2, cleanup, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

#define impl_afw_pool_release impl_mt_tracker_afw_pool_release
#define impl_afw_pool_get_reference \
    impl_mt_tracker_afw_pool_get_reference
#define impl_afw_pool_run_cleanups \
    impl_mt_tracker_afw_pool_run_cleanups
#define impl_afw_pool_destroy impl_mt_tracker_afw_pool_destroy
#define impl_afw_pool_calloc impl_mt_tracker_afw_pool_calloc
#define impl_afw_pool_malloc impl_mt_tracker_afw_pool_malloc
#define impl_afw_pool_calloc_no_throw \
    impl_mt_tracker_afw_pool_calloc_no_throw
#define impl_afw_pool_malloc_no_throw \
    impl_mt_tracker_afw_pool_malloc_no_throw
#define impl_afw_pool_free_memory \
    impl_mt_tracker_afw_pool_free_memory
#define impl_afw_pool_free_memory_no_throw \
    impl_mt_tracker_afw_pool_free_memory_no_throw
#define impl_afw_pool_garbage_collect \
    impl_mt_tracker_afw_pool_garbage_collect
#define impl_afw_pool_register_cleanup \
    impl_mt_tracker_afw_pool_register_cleanup
#define impl_afw_pool_deregister_cleanup \
    impl_mt_tracker_afw_pool_deregister_cleanup

#define AFW_IMPLEMENTATION_ID "tracker_multithreaded"
#define AFW_IMPLEMENTATION_INF_LABEL \
    impl_afw_pool_tracker_multithreaded_inf

static const afw_pool_internal_inf_implementation_specific_t
impl_tracker_mt_implementation_specific =
    {
        /* multithreaded */ true,
        /* tracker */ true
    };

#define AFW_IMPLEMENTATION_SPECIFIC \
    &impl_tracker_mt_implementation_specific

#include "afw_pool_impl_declares.h"
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef AFW_IMPLEMENTATION_SPECIFIC
#undef impl_afw_pool_release
#undef impl_afw_pool_get_reference
#undef impl_afw_pool_run_cleanups
#undef impl_afw_pool_destroy
#undef impl_afw_pool_calloc
#undef impl_afw_pool_malloc
#undef impl_afw_pool_calloc_no_throw
#undef impl_afw_pool_malloc_no_throw
#undef impl_afw_pool_free_memory
#undef impl_afw_pool_free_memory_no_throw
#undef impl_afw_pool_garbage_collect
#undef impl_afw_pool_register_cleanup
#undef impl_afw_pool_deregister_cleanup

/*
 * Scope pool (evaluation `{ }`). ST job heap, 4k chunks. Last-release
 * can delay while a script throw is handled.
 */
#define AFW_IMPLEMENTATION_ID "scope"
#define AFW_IMPLEMENTATION_INF_LABEL impl_afw_pool_scope_inf

static const afw_pool_t *
impl_scope_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);

#define impl_afw_pool_release \
    impl_scope_afw_pool_release

static void
impl_scope_afw_pool_run_cleanups(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);

#define impl_afw_pool_run_cleanups \
    impl_scope_afw_pool_run_cleanups

static void
impl_scope_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx);

#define impl_afw_pool_destroy \
    impl_scope_afw_pool_destroy

#define impl_afw_pool_calloc \
    impl_heap_afw_pool_calloc
#define impl_afw_pool_malloc \
    impl_heap_afw_pool_malloc
#define impl_afw_pool_free_memory \
    impl_heap_afw_pool_free_memory
#define impl_afw_pool_garbage_collect \
    impl_afw_pool_garbage_collect
#define impl_afw_pool_calloc_no_throw \
    impl_heap_afw_pool_calloc_no_throw
#define impl_afw_pool_malloc_no_throw \
    impl_heap_afw_pool_malloc_no_throw
#define impl_afw_pool_free_memory_no_throw \
    impl_heap_afw_pool_free_memory_no_throw

#define AFW_IMPLEMENTATION_SPECIFIC &impl_pool_implementation_specific

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

/* --- scope multithreaded wrappers (lock, then ST scope) ------------- */

static const afw_pool_t *
impl_mt_scope_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    const afw_pool_t *result;

    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        result = impl_scope_afw_pool_release(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
    return result;
}

static void
impl_mt_scope_afw_pool_get_reference(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        impl_afw_pool_get_reference(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_scope_afw_pool_run_cleanups(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        impl_scope_afw_pool_run_cleanups(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_scope_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        impl_scope_afw_pool_destroy(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void *
impl_mt_scope_afw_pool_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        result = impl_heap_afw_pool_calloc(self, size, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
    return result;
}

static void *
impl_mt_scope_afw_pool_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        result = impl_heap_afw_pool_malloc(self, size, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
    return result;
}

static void *
impl_mt_scope_afw_pool_calloc_no_throw(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    const afw_memory_region_t *region;
    void *result;

    region = impl_pool_region(self);
    if (region) {
        afw_memory_region_lock(region, xctx);
    }
    result = impl_heap_afw_pool_calloc_no_throw(self, size, xctx);
    if (region) {
        afw_memory_region_unlock(region, xctx);
    }
    return result;
}

static void *
impl_mt_scope_afw_pool_malloc_no_throw(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    const afw_memory_region_t *region;
    void *result;

    region = impl_pool_region(self);
    if (region) {
        afw_memory_region_lock(region, xctx);
    }
    result = impl_heap_afw_pool_malloc_no_throw(self, size, xctx);
    if (region) {
        afw_memory_region_unlock(region, xctx);
    }
    return result;
}

static void
impl_mt_scope_afw_pool_free_memory(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        impl_heap_afw_pool_free_memory(self, address, size, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_scope_afw_pool_free_memory_no_throw(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    const afw_memory_region_t *region;

    region = impl_pool_region(self);
    if (region) {
        afw_memory_region_lock(region, xctx);
    }
    impl_heap_afw_pool_free_memory_no_throw(self, address, size, xctx);
    if (region) {
        afw_memory_region_unlock(region, xctx);
    }
}

static void
impl_mt_scope_afw_pool_garbage_collect(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        impl_afw_pool_garbage_collect(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_scope_afw_pool_register_cleanup(
    AFW_POOL_SELF_T *self,
    void *data,
    void *data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        impl_afw_pool_register_cleanup(
            self, data, data2, cleanup, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_scope_afw_pool_deregister_cleanup(
    AFW_POOL_SELF_T *self,
    void *data,
    void *data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        impl_afw_pool_deregister_cleanup(
            self, data, data2, cleanup, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

#define impl_afw_pool_release impl_mt_scope_afw_pool_release
#define impl_afw_pool_get_reference \
    impl_mt_scope_afw_pool_get_reference
#define impl_afw_pool_run_cleanups \
    impl_mt_scope_afw_pool_run_cleanups
#define impl_afw_pool_destroy impl_mt_scope_afw_pool_destroy
#define impl_afw_pool_calloc impl_mt_scope_afw_pool_calloc
#define impl_afw_pool_malloc impl_mt_scope_afw_pool_malloc
#define impl_afw_pool_calloc_no_throw \
    impl_mt_scope_afw_pool_calloc_no_throw
#define impl_afw_pool_malloc_no_throw \
    impl_mt_scope_afw_pool_malloc_no_throw
#define impl_afw_pool_free_memory \
    impl_mt_scope_afw_pool_free_memory
#define impl_afw_pool_free_memory_no_throw \
    impl_mt_scope_afw_pool_free_memory_no_throw
#define impl_afw_pool_garbage_collect \
    impl_mt_scope_afw_pool_garbage_collect
#define impl_afw_pool_register_cleanup \
    impl_mt_scope_afw_pool_register_cleanup
#define impl_afw_pool_deregister_cleanup \
    impl_mt_scope_afw_pool_deregister_cleanup

#define AFW_IMPLEMENTATION_ID "scope_multithreaded"
#define AFW_IMPLEMENTATION_INF_LABEL \
    impl_afw_pool_scope_multithreaded_inf
static const afw_pool_internal_inf_implementation_specific_t
impl_scope_mt_implementation_specific =
    {
        /* multithreaded */ true,
        /* tracker */ false
    };

#define AFW_IMPLEMENTATION_SPECIFIC \
    &impl_scope_mt_implementation_specific

#include "afw_pool_impl_declares.h"
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef AFW_IMPLEMENTATION_SPECIFIC
#undef impl_afw_pool_release
#undef impl_afw_pool_get_reference
#undef impl_afw_pool_run_cleanups
#undef impl_afw_pool_destroy
#undef impl_afw_pool_calloc
#undef impl_afw_pool_malloc
#undef impl_afw_pool_calloc_no_throw
#undef impl_afw_pool_malloc_no_throw
#undef impl_afw_pool_free_memory
#undef impl_afw_pool_free_memory_no_throw
#undef impl_afw_pool_garbage_collect
#undef impl_afw_pool_register_cleanup
#undef impl_afw_pool_deregister_cleanup


/* --------------------------- internal macros ------------------------------ */

#ifdef AFW_DEBUG_POOL

#define AFW_POOL_INTERNAL_DEBUG_LEVEL_detail  flag_index_debug_pool_detail
#define AFW_POOL_INTERNAL_DEBUG_LEVEL_minimal flag_index_debug_pool

#define IMPL_PRINT_DEBUG_INFO_Z(level,info_z) \
do { \
    FILE *fd; \
    if (xctx && xctx->env && xctx->env->debug_fd && \
        afw_flag_is_active( \
            xctx->env->AFW_POOL_INTERNAL_DEBUG_LEVEL_##level, xctx)) \
    { \
        fd = xctx->env->debug_fd; \
        fprintf(fd, \
            ">debug pool %s thread " AFW_INTEGER_FMT \
            " pool " AFW_INTEGER_FMT \
            " in_use " AFW_SIZE_T_FMT \
            " total " AFW_SIZE_T_FMT "/" AFW_SIZE_T_FMT \
            " chunks " AFW_SIZE_T_FMT "/" AFW_SIZE_T_FMT \
            " rss " AFW_SIZE_T_FMT " KB" \
            " refs " AFW_INTEGER_FMT \
            " parent " AFW_INTEGER_FMT \
            " (%s)\n", \
            info_z, \
            self->thread ? self->thread->thread_number : \
                (afw_integer_t)0, \
            self->pool_number, \
            self->bytes_allocated, \
            (afw_size_t)xctx->env->pool_bytes_in_use, \
            (afw_size_t)xctx->env->peak_pool_bytes_in_use, \
            (afw_size_t)xctx->env->pool_chunk_bytes, \
            (afw_size_t)xctx->env->peak_pool_chunk_bytes, \
            afw_os_get_rss(), \
            self->reference_count, \
            (afw_integer_t)((self->parent) \
                ? self->parent->pool_number : 0), \
            afw_utf8_z_source_file(AFW__FILE_LINE__)); \
        fflush(fd); \
    } \
} while (0)

#define IMPL_PRINT_DEBUG_INFO_FZ(level,format_z,...) \
do { \
    FILE *fd; \
    if (xctx && xctx->env && xctx->env->debug_fd && \
        afw_flag_is_active( \
            xctx->env->AFW_POOL_INTERNAL_DEBUG_LEVEL_##level, xctx)) \
    { \
        fd = xctx->env->debug_fd; \
        fprintf(fd, \
            ">debug pool " format_z " thread " AFW_INTEGER_FMT \
            " pool " AFW_INTEGER_FMT \
            " in_use " AFW_SIZE_T_FMT \
            " total " AFW_SIZE_T_FMT "/" AFW_SIZE_T_FMT \
            " chunks " AFW_SIZE_T_FMT "/" AFW_SIZE_T_FMT \
            " rss " AFW_SIZE_T_FMT " KB" \
            " refs " AFW_INTEGER_FMT \
            " parent " AFW_INTEGER_FMT \
            " (%s)\n", \
            __VA_ARGS__, \
            self->thread ? self->thread->thread_number : \
                (afw_integer_t)0, \
            self->pool_number, \
            self->bytes_allocated, \
            (afw_size_t)xctx->env->pool_bytes_in_use, \
            (afw_size_t)xctx->env->peak_pool_bytes_in_use, \
            (afw_size_t)xctx->env->pool_chunk_bytes, \
            (afw_size_t)xctx->env->peak_pool_chunk_bytes, \
            afw_os_get_rss(), \
            self->reference_count, \
            (afw_integer_t)((self->parent) \
                ? self->parent->pool_number : 0), \
            afw_utf8_z_source_file(AFW__FILE_LINE__)); \
        fflush(fd); \
    } \
} while (0)

#else

#define IMPL_PRINT_DEBUG_INFO_Z(level,info_z)
#define IMPL_PRINT_DEBUG_INFO_FZ(level,format_z,...)

#endif


static void
impl_env_add_bytes(afw_environment_t *env, afw_size_t n)
{
    env->pool_bytes_in_use += n;
    if (env->pool_bytes_in_use > env->peak_pool_bytes_in_use) {
        env->peak_pool_bytes_in_use = env->pool_bytes_in_use;
    }
}


static void
impl_env_add_chunks(afw_environment_t *env, afw_size_t n)
{
    env->pool_chunk_bytes += n;
    if (env->pool_chunk_bytes > env->peak_pool_chunk_bytes) {
        env->peak_pool_chunk_bytes = env->pool_chunk_bytes;
    }
}


static void
impl_thread_add_bytes(const afw_thread_t *thread, afw_size_t n)
{
    afw_thread_t *t;

    if (!thread || n == 0) {
        return;
    }
    t = (afw_thread_t *)thread;
    t->pool_bytes_in_use += n;
    if (t->pool_bytes_in_use > t->peak_pool_bytes_in_use) {
        t->peak_pool_bytes_in_use = t->pool_bytes_in_use;
    }
}


static void
impl_thread_sub_bytes(const afw_thread_t *thread, afw_size_t n)
{
    if (!thread || n == 0) {
        return;
    }
    ((afw_thread_t *)thread)->pool_bytes_in_use -= n;
}


static void
impl_thread_add_chunks(const afw_thread_t *thread, afw_size_t n)
{
    afw_thread_t *t;

    if (!thread || n == 0) {
        return;
    }
    t = (afw_thread_t *)thread;
    t->pool_chunk_bytes += n;
    if (t->pool_chunk_bytes > t->peak_pool_chunk_bytes) {
        t->peak_pool_chunk_bytes = t->pool_chunk_bytes;
    }
}


static void
impl_thread_sub_chunks(const afw_thread_t *thread, afw_size_t n)
{
    if (!thread || n == 0) {
        return;
    }
    ((afw_thread_t *)thread)->pool_chunk_bytes -= n;
}


static afw_boolean_t
impl_pool_is_multithreaded(const afw_pool_t *p)
{
    const afw_pool_internal_inf_implementation_specific_t *spec;

    if (!p || !p->inf) {
        return false;
    }
    spec = p->inf->rti.implementation_specific;
    return spec && spec->is_multithreaded;
}


static afw_boolean_t
impl_counts_on_thread(const afw_pool_internal_self_t *self)
{
    if (!self->thread) {
        return false;
    }
    return !impl_pool_is_multithreaded(&self->pub);
}


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
        impl_thread_sub_bytes(self->thread, self->bytes_allocated);
        impl_thread_sub_chunks(self->thread, heap->chunk_bytes);
    }
    self->thread = thread;
    if (thread) {
        impl_thread_add_bytes(thread, self->bytes_allocated);
        impl_thread_add_chunks(thread, heap->chunk_bytes);
    }
}


static void
impl_assign_pool_number(afw_pool_internal_self_t *self)
{
    afw_thread_t *thread;

    if (!self->thread) {
        return;
    }
    thread = (afw_thread_t *)self->thread;
    self->pool_number = ++thread->pool_number;
}


static void
impl_account_alloc(
    afw_pool_internal_self_t *self, afw_size_t consumed, afw_xctx_t *xctx)
{
    self->bytes_allocated += consumed;
    if (xctx && xctx->env) {
        impl_env_add_bytes((afw_environment_t *)xctx->env, consumed);
    }
    if (impl_counts_on_thread(self)) {
        impl_thread_add_bytes(self->thread, consumed);
    }
}


static void
impl_account_chunk_add(
    afw_pool_internal_heap_self_t *heap, afw_size_t size, afw_xctx_t *xctx)
{
    heap->chunk_count++;
    heap->chunk_bytes += size;
    if (xctx && xctx->env) {
        impl_env_add_chunks((afw_environment_t *)xctx->env, size);
    }
    if (impl_counts_on_thread(&heap->common)) {
        impl_thread_add_chunks(heap->common.thread, size);
    }
}


static void
impl_account_free(
    afw_pool_internal_self_t *self, afw_size_t consumed, afw_xctx_t *xctx)
{
    self->bytes_allocated -= consumed;
    if (xctx && xctx->env) {
        ((afw_environment_t *)xctx->env)->pool_bytes_in_use -= consumed;
    }
    if (impl_counts_on_thread(self)) {
        impl_thread_sub_bytes(self->thread, consumed);
    }
}


static void
impl_account_destroy(afw_pool_internal_self_t *self, afw_xctx_t *xctx)
{
    if (xctx && xctx->env) {
        ((afw_environment_t *)xctx->env)->pool_bytes_in_use -=
            self->bytes_allocated;
    }
    if (impl_counts_on_thread(self)) {
        impl_thread_sub_bytes(self->thread, self->bytes_allocated);
    }
    self->bytes_allocated = 0;
}


static afw_pool_internal_heap_self_t *
impl_reservoir_heap(afw_pool_internal_self_t *self)
{
    /* A heap is its own store even when it has an AFW parent. */
    while (self->parent && !afw_pool_internal_is_heap(&self->pub)) {
        self = self->parent;
    }
    return impl_as_heap(self);
}


static char *
impl_chunk_usable(afw_pool_chunk_t *chunk)
{
    return (char *)chunk + AFW_POOL_ALIGN_UP(sizeof(afw_pool_chunk_t));
}


static char *
impl_chunk_end(afw_pool_chunk_t *chunk)
{
    return (char *)chunk + chunk->size;
}


static afw_pool_chunk_t *
impl_chunk_containing(afw_pool_internal_heap_self_t *heap, void *addr)
{
    afw_pool_chunk_t *chunk;
    char *p;

    p = (char *)addr;
    for (chunk = heap->first_chunk; chunk; chunk = chunk->next) {
        if (p >= (char *)chunk && p < impl_chunk_end(chunk)) {
            return chunk;
        }
    }
    return NULL;
}


static afw_boolean_t
impl_same_chunk(afw_pool_internal_heap_self_t *heap, void *a, void *b)
{
    afw_pool_chunk_t *ca;
    afw_pool_chunk_t *cb;

    ca = impl_chunk_containing(heap, a);
    cb = impl_chunk_containing(heap, b);
    return (ca && ca == cb);
}


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
            return env->chunk_min;
        }
        chunk_min = AFW_ENVIRONMENT_CHUNK_MIN;
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


/* --------------------------- internal functions --------------------------- */

static void
impl_add_child(
    afw_pool_internal_self_t *parent,
    afw_pool_internal_self_t *child, afw_xctx_t *xctx)
{
    afw_pool_get_reference(&parent->pub, xctx);

    child->parent = parent;
    child->next_sibling = parent->first_child;
    parent->first_child = child;
}


static void
impl_unlink_child(
    afw_pool_internal_self_t *parent,
    afw_pool_internal_self_t *child,
    afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *prev;
    afw_pool_internal_self_t *sibling;

    (void)xctx;
    for (prev = NULL, sibling = parent->first_child;
        sibling;
        prev = sibling, sibling = sibling->next_sibling)
    {
        if (sibling == child) {
            if (!prev) {
                parent->first_child = sibling->next_sibling;
            }
            else {
                prev->next_sibling = sibling->next_sibling;
            }
            child->next_sibling = NULL;
            return;
        }
    }
}


static void
impl_link_as_child(
    afw_pool_internal_self_t *parent,
    afw_pool_internal_self_t *child,
    afw_xctx_t *xctx)
{
    if (impl_pool_is_multithreaded(&parent->pub)) {
        IMPL_MULTITHREADED_LOCK_BEGIN(parent) {
            impl_add_child(parent, child, xctx);
        }
        IMPL_MULTITHREADED_LOCK_END;
    }
    else {
        impl_add_child(parent, child, xctx);
    }
}


/* Create skeleton heap struct. Parent is any AFW pool. */
static afw_pool_internal_self_t *
impl_heap_create(
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

    if (!thread && xctx) {
        thread = xctx->thread;
    }
    if (!thread || !thread->memory_region) {
        if (!xctx) {
            return NULL;
        }
        AFW_THROW_ERROR_Z(general,
            "Heap requires thread->memory_region", xctx);
    }
    self = impl_heap_allocate_self(inf, chunk_min, self_bytes,
        thread->memory_region, xctx);
    if (!self) {
        AFW_THROW_ERROR_Z(memory, "Unable to allocate pool", xctx);
    }
    heap = impl_as_heap(self);
    if (!as_managed_p && afw_parent) {
        self->pub.managed_p = afw_parent->managed_p
            ? afw_parent->managed_p
            : afw_parent;
    }
    self->thread = thread;
    impl_assign_pool_number(self);

    if (afw_parent) {
        parent_self = (afw_pool_internal_self_t *)afw_parent;
        impl_link_as_child(parent_self, self, xctx);
    }

    if (xctx && xctx->env && heap->chunk_bytes) {
        impl_env_add_chunks((afw_environment_t *)xctx->env,
            heap->chunk_bytes);
        if (impl_counts_on_thread(self)) {
            impl_thread_add_chunks(self->thread, heap->chunk_bytes);
        }
    }

    IMPL_PRINT_DEBUG_INFO_Z(minimal, "create");

    return self;
}

/* Create skeleton pool struct for tracker. */
static afw_pool_internal_self_t *
impl_create_for_tracker(
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
    impl_assign_pool_number(self);
    self->reference_count = 1;
    impl_link_as_child(parent, self, xctx);

    IMPL_PRINT_DEBUG_INFO_Z(minimal, "create");

    return self;
}

/*
 * First-fit on an address-ordered free list. Overlay lives only on
 * freed blocks. Remainder too small to hold a free node is left on
 * the list so total is always recoverable as prefix + USER size.
 */
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


static afw_size_t
impl_block_bytes(
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


static void
impl_heap_add_to_free_list(
    afw_pool_internal_heap_self_t *heap,
    void *start,
    afw_size_t total,
    afw_xctx_t *xctx);

static void *
impl_heap_take_from_free_list_or_chunk(
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

    head = heap->free_memory_head;
    curr = NULL;
    if (head) {
        slow = head->first;
        fast = slow;
        for (curr = slow; curr; curr = curr->next) {
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
    }

    if (curr) {
        prev = curr->prev;
        next = curr->next;
        impl_heap_free_unlink(&head->first, curr);
        if (curr->total - total >= sizeof(afw_pool_free_node_t)) {
            rest = (afw_pool_free_node_t *)(((char *)curr) + total);
            rest->total = curr->total - total;
            rest->prev = prev;
            if (prev) {
                prev->next = rest;
            }
            else {
                head->first = rest;
            }
            if (next &&
                ((char *)rest) + rest->total == (char *)next &&
                impl_same_chunk(heap, rest, next))
            {
                rest->total += next->total;
                rest->next = next->next;
                if (next->next) {
                    next->next->prev = rest;
                }
            }
            else {
                rest->next = next;
                if (next) {
                    next->prev = rest;
                }
            }
        }
        *reused = true;
        return curr;
    }

    *reused = false;
    if (heap->remaining >= total) {
        start = heap->bump;
        heap->bump += total;
        heap->remaining -= total;
        return start;
    }

    if (heap->current_chunk &&
        heap->remaining >= sizeof(afw_pool_free_node_t))
    {
        impl_heap_add_to_free_list(heap, heap->bump,
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

    region = (heap->common.thread)
        ? heap->common.thread->memory_region : NULL;
    if (!region) {
        if (unhandled || !xctx) {
            return NULL;
        }
        AFW_THROW_ERROR_Z(general,
            "Heap requires thread->memory_region", xctx);
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
    return start;
}


static void
impl_heap_add_to_free_list(
    afw_pool_internal_heap_self_t *heap,
    void *start,
    afw_size_t total,
    afw_xctx_t *xctx)
{
    afw_pool_free_node_t *freeing;
    afw_pool_free_node_t *prev;
    afw_pool_free_node_t *curr;
    afw_pool_internal_free_memory_head_t *head;

    (void)xctx;
    head = heap->free_memory_head;
    if (!head) {
        return;
    }

    freeing = (afw_pool_free_node_t *)start;
    freeing->total = total;
    freeing->prev = NULL;
    freeing->next = NULL;

    prev = NULL;
    curr = head->first;
    while (curr && curr < freeing) {
        prev = curr;
        curr = curr->next;
    }

    freeing->prev = prev;
    freeing->next = curr;
    if (prev) {
        prev->next = freeing;
    }
    else {
        head->first = freeing;
    }
    if (curr) {
        curr->prev = freeing;
    }

    if (curr &&
        ((char *)freeing) + freeing->total == (char *)curr &&
        impl_same_chunk(heap, freeing, curr))
    {
        freeing->total += curr->total;
        freeing->next = curr->next;
        if (curr->next) {
            curr->next->prev = freeing;
        }
    }

    if (prev &&
        ((char *)prev) + prev->total == (char *)freeing &&
        impl_same_chunk(heap, prev, freeing))
    {
        prev->total += freeing->total;
        prev->next = freeing->next;
        if (freeing->next) {
            freeing->next->prev = prev;
        }
    }
}


#ifdef AFW_DEBUG_POOL
static void
impl_debug_prefix_set(
    AFW_POOL_SELF_T *self,
    void *user,
    afw_size_t size)
{
    afw_pool_debug_prefix_t *pre;

    pre = (afw_pool_debug_prefix_t *)((char *)user -
        sizeof(afw_pool_debug_prefix_t));
    pre->size = size;
    pre->pool = &self->pub;
}

static afw_boolean_t
impl_debug_prefix_ok(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size)
{
    afw_pool_debug_prefix_t *pre;

    pre = (afw_pool_debug_prefix_t *)((char *)address -
        sizeof(afw_pool_debug_prefix_t));
    return pre->pool == &self->pub && pre->size == size;
}

static void
impl_debug_check_prefix(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    afw_pool_debug_prefix_t *pre;

    pre = (afw_pool_debug_prefix_t *)((char *)address -
        sizeof(afw_pool_debug_prefix_t));
    if (pre->pool != &self->pub) {
        AFW_THROW_ERROR_Z(general,
            "afw_pool_free_memory: pool does not match allocation",
            xctx);
    }
    if (pre->size != size) {
        AFW_THROW_ERROR_Z(general,
            "afw_pool_free_memory: size does not match allocation",
            xctx);
    }
}

static void
impl_debug_poison_user(void *user, afw_size_t size)
{
    afw_size_t i;
    afw_size_t n;
    afw_size_t *w;
    unsigned char *b;
    unsigned char *end;
    afw_size_t poison;

    if (!user || size == 0) {
        return;
    }
    poison = AFW_POOL_DEBUG_POISON;
    w = (afw_size_t *)user;
    n = size / sizeof(afw_size_t);
    for (i = 0; i < n; i++) {
        w[i] = poison;
    }
    b = (unsigned char *)user + n * sizeof(afw_size_t);
    end = (unsigned char *)user + size;
    i = 0;
    while (b < end) {
        *b++ = (unsigned char)(poison >>
            (8 * (i % sizeof(afw_size_t))));
        i++;
    }
}
#else
#define impl_debug_prefix_set(self, user, size) ((void)0)
#define impl_debug_prefix_ok(self, address, size) (true)
#define impl_debug_check_prefix(self, address, size, xctx) ((void)0)
#define impl_debug_poison_user(user, size) ((void)0)
#endif


/* --------------------------- pool implementations ------------------------- */

static void
impl_clear_delay(
    afw_pool_internal_scope_self_t *me, afw_xctx_t *xctx)
{
    const afw_pool_t **pos;
    afw_pool_internal_scope_self_t *curr;

    if (!me->error_delaying_release) {
        return;
    }
    me->error_delaying_release = false;
    if (!xctx) {
        me->error_delaying_release_next = NULL;
        return;
    }
    pos = &xctx->error_delaying_release_first;
    while (*pos) {
        curr = impl_as_scope(
            (afw_pool_internal_self_t *)(void *)*pos);
        if (curr == me) {
            *pos = curr->error_delaying_release_next;
            curr->error_delaying_release_next = NULL;
            return;
        }
        pos = &curr->error_delaying_release_next;
    }
    me->error_delaying_release_next = NULL;
}


/*
 * While error_processing_count > 0, last release of a scope pool
 * is recorded and skipped. Catching ENDTRY runs
 * afw_pool_release_delayed() when the count is 0 again.
 */
static afw_boolean_t
impl_error_delaying_release(
    afw_pool_internal_scope_self_t *me,
    afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *self;

    self = &me->heap.common;
    if (!xctx || xctx->error_processing_count == 0) {
        return false;
    }
    if (me->error_delaying_release) {
        return true;
    }
    if (self->reference_count != 1) {
        return false;
    }
    me->error_delaying_release = true;
    me->error_delaying_release_next =
        xctx->error_delaying_release_first;
    xctx->error_delaying_release_first = &self->pub;
    return true;
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
    if (impl_counts_on_thread(&heap->common)) {
        impl_thread_sub_chunks(heap->common.thread, heap->chunk_bytes);
    }
    heap->chunk_bytes = 0;
    heap->chunk_count = 0;
    chunk = heap->first_chunk;
    heap->first_chunk = NULL;
    heap->current_chunk = NULL;
    heap->bump = NULL;
    heap->remaining = 0;
    region = (heap->common.thread)
        ? heap->common.thread->memory_region : NULL;
    while (chunk) {
        next = chunk->next;
        size = chunk->size;
        if (region) {
            afw_memory_region_free(region, chunk, size, xctx);
        }
        chunk = next;
    }
}


static void
impl_tracker_return_leftovers(
    afw_pool_internal_tracker_self_t *tracker, afw_xctx_t *xctx)
{
    afw_pool_internal_heap_self_t *heap;
    afw_pool_tracker_node_t *curr;
    afw_pool_tracker_node_t *next;

    heap = impl_reservoir_heap(&tracker->common);
    curr = tracker->first_allocated_memory;
    tracker->first_allocated_memory = NULL;
    while (curr) {
        next = AFW_POOL_TRACKER_NEXT(curr);
        impl_debug_poison_user(AFW_POOL_TRACKER_TO_USER(curr),
            AFW_POOL_TRACKER_USER_SIZE(curr));
        impl_heap_add_to_free_list(heap, curr,
            impl_block_bytes(AFW_POOL_TRACKER_PREFIX_BYTES,
                AFW_POOL_TRACKER_USER_SIZE(curr), xctx, false),
            xctx);
        curr = next;
    }
}


/*
 * Run cleanup callbacks only. Storage stays so sibling callbacks can
 * still value_release tracker-allocated managed headers.
 */
static void
impl_pool_run_cleanups(AFW_POOL_SELF_T *self, afw_xctx_t *xctx)
{
    afw_pool_cleanup_t *e;

    /*
     * Detach the list first. A callback may last-release this pool
     * (closure drops its scope); that must not walk the same list.
     */
    e = self->first_cleanup;
    self->first_cleanup = NULL;
    for (; e; e = e->next_cleanup) {
        e->cleanup(e->data, e->data2, &self->pub, xctx);
    }
}


static void
impl_unlink_from_parent(
    AFW_POOL_SELF_T *self, afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *parent;

    parent = self->parent;
    if (!parent) {
        return;
    }
    if (impl_pool_is_multithreaded(&parent->pub)) {
        IMPL_MULTITHREADED_LOCK_BEGIN(parent) {
            impl_unlink_child(parent, self, xctx);
        }
        IMPL_MULTITHREADED_LOCK_END;
    }
    else {
        impl_unlink_child(parent, self, xctx);
    }
}


static void
impl_heap_teardown_store(AFW_POOL_SELF_T *self, afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *parent;
    afw_boolean_t parent_destroying;

    parent = self->parent;
    parent_destroying = parent && parent->destroying;
    impl_unlink_from_parent(self, xctx);
    impl_account_destroy(self, xctx);
    if (parent && !parent_destroying) {
        afw_pool_release(&parent->pub, xctx);
    }
    impl_heap_free_chunks(impl_as_heap(self), xctx);
}


static void
impl_tracker_teardown_store(
    AFW_POOL_SELF_T *self,
    afw_size_t self_bytes,
    afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *parent;
    afw_boolean_t parent_destroying;

    parent = self->parent;
    parent_destroying = parent && parent->destroying;
    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Tracker has no parent", xctx);
    }
    impl_unlink_from_parent(self, xctx);
    impl_tracker_return_leftovers(impl_as_tracker(self), xctx);
    impl_account_destroy(self, xctx);
    afw_pool_free_memory(&parent->pub, self, self_bytes, xctx);
    if (!parent_destroying) {
        afw_pool_release(&parent->pub, xctx);
    }
}


static void
impl_tracker_teardown(AFW_POOL_SELF_T *self, afw_xctx_t *xctx)
{
    impl_tracker_teardown_store(self,
        sizeof(afw_pool_internal_tracker_self_t), xctx);
}


static void
impl_scope_teardown(AFW_POOL_SELF_T *self, afw_xctx_t *xctx)
{
    impl_heap_teardown_store(self, xctx);
}


static void
impl_pool_mark_destroying(AFW_POOL_SELF_T *self)
{
    afw_pool_internal_self_t *child;

    self->destroying = true;
    for (child = self->first_child; child; child = child->next_sibling) {
        impl_pool_mark_destroying(child);
    }
}


static void
impl_destroy_children(AFW_POOL_SELF_T *self, afw_xctx_t *xctx)
{
    while (self->first_child) {
        afw_pool_internal_self_t *child;

        child = self->first_child;
        afw_pool_destroy(&child->pub, xctx);
        if (self->first_child == child) {
            impl_unlink_child(self, child, xctx);
        }
    }
}


static void
impl_run_child_cleanups(AFW_POOL_SELF_T *self, afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *child;
    afw_pool_internal_self_t *next;

    child = self->first_child;
    while (child) {
        next = child->next_sibling;
        afw_pool_run_cleanups(&child->pub, xctx);
        child = next;
    }
}


static const afw_pool_t *
impl_release_common(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx,
    void (*teardown)(AFW_POOL_SELF_T *self, afw_xctx_t *xctx))
{
    if (--(self->reference_count) == 0) {
        if (self->destroying) {
            impl_pool_run_cleanups(self, xctx);
            return NULL;
        }
        if (self->first_child) {
            AFW_THROW_ERROR_Z(general,
                "Pool last-release with children remaining", xctx);
        }
        impl_pool_run_cleanups(self, xctx);
        teardown(self, xctx);
        return NULL;
    }
    return &self->pub;
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
    return impl_release_common(self, xctx, impl_heap_teardown_store);
}


void
afw_pool_release_delayed(
    const afw_pool_t *instance,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p;
    afw_pool_internal_scope_self_t *delay;

    (void)instance;
    if (!xctx) {
        return;
    }
    while (xctx->error_delaying_release_first) {
        p = xctx->error_delaying_release_first;
        delay = impl_as_scope((afw_pool_internal_self_t *)p);
        impl_clear_delay(delay, xctx);
        afw_pool_release(p, xctx);
    }
}


/*
 * Implementation of method get_reference for interface afw_pool.
 */
void
impl_afw_pool_get_reference(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "get_reference");

    self->reference_count++;
}

/*
 * Implementation of method run_cleanups for interface afw_pool.
 */
static void
impl_heap_afw_pool_run_cleanups(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "run_cleanups");
    if (!self->destroying) {
        impl_pool_mark_destroying(self);
    }
    impl_run_child_cleanups(self, xctx);
    impl_pool_run_cleanups(self, xctx);
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
        impl_pool_mark_destroying(self);
    }
    impl_destroy_children(self, xctx);
    impl_heap_teardown_store(self, xctx);
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

    total = impl_block_bytes(AFW_POOL_HEAP_PREFIX_BYTES, size,
        xctx, unhandled);
    if (unhandled && total == 0) {
        return NULL;
    }

    start = impl_heap_take_from_free_list_or_chunk(impl_as_heap(self),
        total, &reused, xctx, unhandled);
    if (!start) {
        return NULL;
    }
    IMPL_PRINT_DEBUG_INFO_FZ(detail, "alloc %s " AFW_SIZE_T_FMT,
        reused ? "reuse" : "chunk", size);
    if (xctx) {
        impl_account_alloc(self, total, xctx);
    }
    user = AFW_POOL_HEAP_USER_FROM_START(start);
    impl_debug_prefix_set(self, user, size);
    return user;
}

/*
 * Implementation of method calloc for interface afw_pool.
 */
void *
impl_heap_afw_pool_calloc(
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
impl_heap_afw_pool_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    return impl_heap_malloc_internal(self, size, xctx, false);
}

void *
impl_heap_afw_pool_malloc_no_throw(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    return impl_heap_malloc_internal(self, size, xctx, true);
}

void *
impl_heap_afw_pool_calloc_no_throw(
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
        if (!impl_debug_prefix_ok(self, address, size)) {
            return;
        }
    }
    else {
        impl_debug_check_prefix(self, address, size, xctx);
    }
    impl_debug_poison_user(address, size);
    total = impl_block_bytes(AFW_POOL_HEAP_PREFIX_BYTES, size,
        xctx, no_throw);
    if (no_throw && total == 0) {
        return;
    }
    start = AFW_POOL_HEAP_ALLOC_START(address);
    IMPL_PRINT_DEBUG_INFO_FZ(
        detail, "free %p " AFW_SIZE_T_FMT,
        address, total);
    impl_account_free(self, total, xctx);
    impl_heap_add_to_free_list(impl_as_heap(self), start, total, xctx);
}

void
impl_heap_afw_pool_free_memory(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    impl_heap_free_internal(self, address, size, xctx, false);
}

void
impl_heap_afw_pool_free_memory_no_throw(
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
static void
impl_afw_pool_garbage_collect(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "garbage_collect");
    (void)self;
    (void)xctx;
}

/*
 * Implementation of method register_cleanup for interface afw_pool.
 */
void
impl_afw_pool_register_cleanup(
    AFW_POOL_SELF_T *self,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    afw_pool_cleanup_t *e;

    IMPL_PRINT_DEBUG_INFO_FZ(minimal,
        "register_cleanup %p %p",
        data, cleanup);

    /* Allocate entry which will also make sure its ok to use pool. */
    e = afw_pool_calloc_type(&self->pub, afw_pool_cleanup_t, xctx);

    /* Add entry to front of list of cleanup functions. */
    e->data = data;
    e->data2 = data2;
    e->cleanup = cleanup;
    e->next_cleanup = self->first_cleanup;
    self->first_cleanup = e;
}

/*
 * Implementation of method deregister_cleanup for interface afw_pool.
 */
void
impl_afw_pool_deregister_cleanup(
    AFW_POOL_SELF_T *self,
    void * data,
    void * data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    afw_pool_cleanup_t *e, *prev;

    IMPL_PRINT_DEBUG_INFO_FZ(minimal,
        "deregister_cleanup %p %p",
        data, cleanup);

    /* Search for entry and remove. */
    for (prev = (afw_pool_cleanup_t *)& self->first_cleanup,
        e = self->first_cleanup;
        e; prev = e, e = e->next_cleanup)
    {
        if (e->data == data && e->data2 == data2 && e->cleanup == cleanup) {
            prev->next_cleanup = e->next_cleanup;
            afw_pool_free_memory(&self->pub, e,
                sizeof(afw_pool_cleanup_t), xctx);
            break;
        }
    }
}


/* --- heap multithreaded wrappers (lock, then ST heap methods) --------- */

static const afw_pool_t *
impl_mt_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    const afw_pool_t *result;

    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        result = impl_heap_afw_pool_release(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
    return result;
}

static void
impl_mt_afw_pool_get_reference(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        impl_afw_pool_get_reference(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_afw_pool_run_cleanups(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    impl_heap_afw_pool_run_cleanups(self, xctx);
}

static void
impl_mt_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        impl_heap_afw_pool_destroy(self, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void *
impl_mt_afw_pool_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        result = impl_heap_afw_pool_calloc(self, size, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
    return result;
}

static void *
impl_mt_afw_pool_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        result = impl_heap_afw_pool_malloc(self, size, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
    return result;
}

static void *
impl_mt_afw_pool_calloc_no_throw(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    const afw_memory_region_t *region;
    void *result;

    /* xctx init cannot AFW_TRY. */
    region = impl_pool_region(self);
    if (region) {
        afw_memory_region_lock(region, xctx);
    }
    result = impl_heap_afw_pool_calloc_no_throw(self, size, xctx);
    if (region) {
        afw_memory_region_unlock(region, xctx);
    }
    return result;
}

static void *
impl_mt_afw_pool_malloc_no_throw(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    const afw_memory_region_t *region;
    void *result;

    region = impl_pool_region(self);
    if (region) {
        afw_memory_region_lock(region, xctx);
    }
    result = impl_heap_afw_pool_malloc_no_throw(self, size, xctx);
    if (region) {
        afw_memory_region_unlock(region, xctx);
    }
    return result;
}

static void
impl_mt_afw_pool_free_memory(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        impl_heap_afw_pool_free_memory(self, address, size, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_afw_pool_free_memory_no_throw(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    const afw_memory_region_t *region;

    region = impl_pool_region(self);
    if (region) {
        afw_memory_region_lock(region, xctx);
    }
    impl_heap_afw_pool_free_memory_no_throw(self, address, size, xctx);
    if (region) {
        afw_memory_region_unlock(region, xctx);
    }
}

static void
impl_mt_afw_pool_register_cleanup(
    AFW_POOL_SELF_T *self,
    void *data,
    void *data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        impl_afw_pool_register_cleanup(
            self, data, data2, cleanup, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_afw_pool_deregister_cleanup(
    AFW_POOL_SELF_T *self,
    void *data,
    void *data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    IMPL_MULTITHREADED_LOCK_BEGIN(self) {
        impl_afw_pool_deregister_cleanup(
            self, data, data2, cleanup, xctx);
    }
    IMPL_MULTITHREADED_LOCK_END;
}

#define impl_afw_pool_release impl_mt_afw_pool_release
#define impl_afw_pool_get_reference impl_mt_afw_pool_get_reference
#define impl_afw_pool_run_cleanups impl_mt_afw_pool_run_cleanups
#define impl_afw_pool_destroy impl_mt_afw_pool_destroy
#define impl_afw_pool_calloc impl_mt_afw_pool_calloc
#define impl_afw_pool_malloc impl_mt_afw_pool_malloc
#define impl_afw_pool_calloc_no_throw impl_mt_afw_pool_calloc_no_throw
#define impl_afw_pool_malloc_no_throw impl_mt_afw_pool_malloc_no_throw
#define impl_afw_pool_free_memory impl_mt_afw_pool_free_memory
#define impl_afw_pool_free_memory_no_throw \
    impl_mt_afw_pool_free_memory_no_throw
#define impl_afw_pool_register_cleanup \
    impl_mt_afw_pool_register_cleanup
#define impl_afw_pool_deregister_cleanup impl_mt_afw_pool_deregister_cleanup

#define AFW_IMPLEMENTATION_ID "heap_multithreaded"
#define AFW_IMPLEMENTATION_INF_LABEL impl_afw_pool_heap_multithreaded_inf

static const afw_pool_internal_inf_implementation_specific_t
impl_pool_mt_implementation_specific =
    {
        /* multithreaded */ true,
        /* tracker */ false
    };

#define AFW_IMPLEMENTATION_SPECIFIC &impl_pool_mt_implementation_specific

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
#undef impl_afw_pool_calloc_no_throw
#undef impl_afw_pool_malloc_no_throw
#undef impl_afw_pool_free_memory
#undef impl_afw_pool_free_memory_no_throw
#undef impl_afw_pool_register_cleanup
#undef impl_afw_pool_deregister_cleanup


/* --------------------------- tracker implementations ---------------------- */

const afw_pool_t *
impl_tracker_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "release");
    return impl_release_common(self, xctx, impl_tracker_teardown);
}

static void
impl_tracker_afw_pool_run_cleanups(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "run_cleanups");
    if (!self->destroying) {
        impl_pool_mark_destroying(self);
    }
    impl_run_child_cleanups(self, xctx);
    impl_pool_run_cleanups(self, xctx);
}

void
impl_tracker_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "destroy");
    if (!self->destroying) {
        impl_pool_mark_destroying(self);
    }
    impl_destroy_children(self, xctx);
    impl_tracker_teardown(self, xctx);
}

const afw_pool_t *
impl_scope_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "release");
    if (impl_error_delaying_release(impl_as_scope(self), xctx)) {
        return &self->pub;
    }
    return impl_release_common(self, xctx, impl_scope_teardown);
}

static void
impl_scope_afw_pool_run_cleanups(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "run_cleanups");
    if (!self->destroying) {
        impl_clear_delay(impl_as_scope(self), xctx);
        impl_pool_mark_destroying(self);
    }
    impl_run_child_cleanups(self, xctx);
    impl_pool_run_cleanups(self, xctx);
}

static void
impl_scope_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    IMPL_PRINT_DEBUG_INFO_Z(minimal, "destroy");
    if (!self->destroying) {
        impl_clear_delay(impl_as_scope(self), xctx);
        impl_pool_mark_destroying(self);
    }
    impl_destroy_children(self, xctx);
    impl_scope_teardown(self, xctx);
}


static void *
impl_tracker_malloc_internal(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx,
    afw_boolean_t unhandled)
{
    afw_pool_internal_tracker_self_t *tracker;
    void *start;
    void *user;
    afw_pool_tracker_node_t *node;
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

    total = impl_block_bytes(AFW_POOL_TRACKER_PREFIX_BYTES, size,
        xctx, unhandled);
    if (unhandled && total == 0) {
        return NULL;
    }

    start = impl_heap_take_from_free_list_or_chunk(
        impl_reservoir_heap(self), total, &reused, xctx, unhandled);
    if (!start) {
        return NULL;
    }
    IMPL_PRINT_DEBUG_INFO_FZ(detail, "alloc %s " AFW_SIZE_T_FMT,
        reused ? "reuse" : "chunk", size);
    tracker = impl_as_tracker(self);
    node = (afw_pool_tracker_node_t *)start;
    node->next = tracker->first_allocated_memory;
    tracker->first_allocated_memory = node;
    user = AFW_POOL_TRACKER_TO_USER(node);
#ifdef AFW_DEBUG_POOL
    node->debug.size = size;
    node->debug.pool = &self->pub;
#else
    node->size = size;
#endif
    if (xctx) {
        impl_account_alloc(self, total, xctx);
    }
    return user;
}


void *
impl_tracker_afw_pool_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    result = impl_tracker_malloc_internal(self, size, xctx, false);
    memset(result, 0, size);
    return result;
}


static void *
impl_tracker_afw_pool_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    return impl_tracker_malloc_internal(self, size, xctx, false);
}


static void *
impl_tracker_afw_pool_malloc_no_throw(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    return impl_tracker_malloc_internal(self, size, xctx, true);
}


static void *
impl_tracker_afw_pool_calloc_no_throw(
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
    afw_pool_tracker_node_t *node;
    afw_size_t total;

    if (!address) {
        IMPL_PRINT_DEBUG_INFO_Z(detail, "free");
        return;
    }
    if (no_throw) {
        if (!impl_debug_prefix_ok(self, address, size)) {
            return;
        }
    }
    else {
        impl_debug_check_prefix(self, address, size, xctx);
    }
    node = AFW_POOL_TRACKER_NODE(address);
    if (AFW_POOL_TRACKER_IS_FREED(node)) {
        if (no_throw) {
            return;
        }
        AFW_THROW_ERROR_Z(general,
            "afw_pool_free_memory: already freed",
            xctx);
    }
    impl_debug_poison_user(address, size);
    total = impl_block_bytes(AFW_POOL_TRACKER_PREFIX_BYTES, size,
        xctx, no_throw);
    if (no_throw && total == 0) {
        return;
    }
    IMPL_PRINT_DEBUG_INFO_FZ(
        detail, "free %p " AFW_SIZE_T_FMT,
        address, total);
    impl_account_free(self, total, xctx);
    AFW_POOL_TRACKER_MARK_FREED(node);
}

static void
impl_tracker_afw_pool_free_memory(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    impl_tracker_free_internal(self, address, size, xctx, false);
}

static void
impl_tracker_afw_pool_free_memory_no_throw(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    impl_tracker_free_internal(self, address, size, xctx, true);
}


static void
impl_tracker_afw_pool_garbage_collect(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    afw_pool_internal_tracker_self_t *tracker;
    afw_pool_internal_heap_self_t *heap;
    afw_pool_tracker_node_t *prev;
    afw_pool_tracker_node_t *curr;
    afw_pool_tracker_node_t *next;

    IMPL_PRINT_DEBUG_INFO_Z(minimal, "garbage_collect");
    tracker = impl_as_tracker(self);
    heap = impl_reservoir_heap(self);
    prev = NULL;
    curr = tracker->first_allocated_memory;
    while (curr) {
        next = AFW_POOL_TRACKER_NEXT(curr);
        if (AFW_POOL_TRACKER_IS_FREED(curr)) {
            if (prev) {
                prev->next = next;
            }
            else {
                tracker->first_allocated_memory = next;
            }
            impl_heap_add_to_free_list(heap, curr,
                impl_block_bytes(AFW_POOL_TRACKER_PREFIX_BYTES,
                    AFW_POOL_TRACKER_USER_SIZE(curr), xctx, false),
                xctx);
        }
        else {
            prev = curr;
        }
        curr = next;
    }
}


/* ---------------------------- extern functions ---------------------------- */


afw_boolean_t
afw_pool_internal_is_heap(const afw_pool_t *p)
{
    return p && (p->inf == &impl_afw_pool_inf ||
        p->inf == &impl_afw_pool_heap_multithreaded_inf ||
        p->inf == &impl_afw_pool_scope_inf ||
        p->inf == &impl_afw_pool_scope_multithreaded_inf);
}


afw_boolean_t
afw_pool_internal_is_heap_multithreaded(const afw_pool_t *p)
{
    return p && p->inf == &impl_afw_pool_heap_multithreaded_inf;
}


afw_boolean_t
afw_pool_internal_is_tracker(const afw_pool_t *p)
{
    const afw_pool_internal_inf_implementation_specific_t *spec;

    if (!p || !p->inf) {
        return false;
    }
    spec = p->inf->rti.implementation_specific;
    return spec && spec->is_tracker;
}


const afw_pool_t *
afw_pool_internal_heap_create(
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
    self = impl_heap_create(parent, inf, as_managed_p, chunk_min,
        sizeof(afw_pool_internal_self_with_free_memory_head_t),
        NULL, xctx);
    return &self->pub;
}


const afw_pool_t *
afw_pool_internal_heap_create_st_for_thread(
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
    self = impl_heap_create(parent, &impl_afw_pool_inf, as_managed_p,
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
    return afw_pool_internal_heap_create(parent,
        impl_pool_is_multithreaded(parent), false,
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
    return afw_pool_internal_heap_create(parent,
        impl_pool_is_multithreaded(parent), true,
        chunk_min, xctx);
}


void *
afw_pool_malloc_unhandled(
    const afw_pool_t *instance,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    if (!instance) {
        return NULL;
    }
    return afw_pool_malloc_no_throw(instance, size, xctx);
}


void *
afw_pool_calloc_unhandled(
    const afw_pool_t *instance,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    if (!instance) {
        return NULL;
    }
    return afw_pool_calloc_no_throw(instance, size, xctx);
}


const afw_pool_t *
afw_pool_internal_create_base_pool(const afw_thread_t *thread)
{
    afw_pool_internal_self_t *self;

    if (!thread || !thread->memory_region) {
        return NULL;
    }
    self = impl_heap_allocate_self(
        &impl_afw_pool_heap_multithreaded_inf,
        AFW_ENVIRONMENT_CHUNK_MIN,
        sizeof(afw_pool_internal_self_with_free_memory_head_t),
        thread->memory_region, NULL);
    if (!self) {
        return NULL;
    }
    self->pool_number = 1;
    self->thread = thread;
    impl_base_pool_self = impl_as_heap(self);
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
        self = (AFW_POOL_SELF_T *)
            afw_pool_internal_heap_create_st_for_thread(
                xctx->p, true, xctx->env->xctx_chunk_min,
                thread, xctx);
        impl_pool_set_owning_thread(impl_as_heap(self), thread);
        thread->p = &self->pub;
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


void
afw_pool_print_debug_info(
    int indent,
    const afw_pool_t *pool,
    afw_xctx_t *xctx)
{
    const AFW_POOL_SELF_T *self = (const AFW_POOL_SELF_T *)pool;
    const afw_pool_internal_self_t *child;
    const afw_pool_internal_heap_self_t *heap;
    int i;
    afw_size_t chunk_count;
    afw_size_t chunk_bytes;

    (void)xctx;
    chunk_count = 0;
    chunk_bytes = 0;
    if (afw_pool_internal_is_heap(pool)) {
        heap = impl_as_heap(self);
        chunk_count = heap->chunk_count;
        chunk_bytes = heap->chunk_bytes;
    }
    for (i = 0; i < indent; i++) {
        printf("  ");
    }
    printf(
        "thread " AFW_INTEGER_FMT " pool " AFW_INTEGER_FMT
        " in_use " AFW_SIZE_T_FMT
        " chunks " AFW_SIZE_T_FMT " " AFW_SIZE_T_FMT
        " refs " AFW_INTEGER_FMT
        " parent " AFW_INTEGER_FMT "\n",
        self->thread ? self->thread->thread_number : (afw_integer_t)0,
        self->pool_number,
        self->bytes_allocated,
        chunk_count,
        chunk_bytes,
        self->reference_count,
        self->parent ? self->parent->pool_number : (afw_integer_t)0);

    for (child = self->first_child; child; child = child->next_sibling) {
        afw_pool_print_debug_info(indent + 2, &child->pub, xctx);
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
    self = impl_create_for_tracker(parent_self,
        impl_pool_is_multithreaded(parent)
            ? &impl_afw_pool_tracker_multithreaded_inf
            : &impl_afw_pool_tracker_inf,
        sizeof(afw_pool_internal_tracker_self_t), xctx);
    return &self->pub;
}


AFW_DEFINE(const afw_pool_t *)
afw_pool_scope_create(
    const afw_pool_t *parent, afw_xctx_t *xctx)
{
    AFW_POOL_SELF_T *self;

    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required", xctx);
    }
    if (!afw_pool_internal_is_heap(parent) &&
        !afw_pool_internal_is_tracker(parent))
    {
        AFW_THROW_ERROR_Z(general,
            "afw_pool_scope_create() parent must be a heap or tracker",
            xctx);
    }

    self = impl_heap_create(parent,
        impl_pool_is_multithreaded(parent)
            ? &impl_afw_pool_scope_multithreaded_inf
            : &impl_afw_pool_scope_inf,
        false,
        (xctx->env && xctx->env->compile_chunk_min)
            ? xctx->env->compile_chunk_min : (afw_size_t)4096,
        sizeof(afw_pool_internal_scope_self_t), NULL, xctx);
    return &self->pub;
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
    return afw_pool_internal_heap_create(parent,
        impl_pool_is_multithreaded(parent), false, 0, xctx);
}


AFW_DEFINE(const afw_pool_t *)
afw_pool_multithread_create(
    const afw_pool_t *parent,
    afw_xctx_t *xctx)
{
    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required", xctx);
    }
    if (!afw_pool_internal_is_heap_multithreaded(parent)) {
        AFW_THROW_ERROR_Z(general,
            "afw_pool_multithread_create() parent must be a "
            "multithreaded heap",
            xctx);
    }
    return afw_pool_internal_heap_create(parent, true, false, 0, xctx);
}


AFW_DEFINE(const afw_pool_t *)
afw_pool_multithread_create_as_managed_p(
    const afw_pool_t *parent,
    afw_xctx_t *xctx)
{
    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required", xctx);
    }
    if (!afw_pool_internal_is_heap_multithreaded(parent)) {
        AFW_THROW_ERROR_Z(general,
            "afw_pool_multithread_create_as_managed_p() parent must "
            "be a multithreaded heap",
            xctx);
    }
    return afw_pool_internal_heap_create(parent, true, true, 0, xctx);
}


AFW_DEFINE(afw_size_t)
afw_pool_bytes_allocated(const afw_pool_t *instance)
{
    if (!instance) {
        return 0;
    }
    return ((const afw_pool_internal_self_t *)instance)->bytes_allocated;
}


AFW_DEFINE(afw_size_t)
afw_pool_chunk_bytes(const afw_pool_t *instance)
{
    if (!instance || !afw_pool_internal_is_heap(instance)) {
        return 0;
    }
    return impl_as_heap(instance)->chunk_bytes;
}


AFW_DEFINE(afw_size_t)
afw_pool_chunk_count(const afw_pool_t *instance)
{
    if (!instance || !afw_pool_internal_is_heap(instance)) {
        return 0;
    }
    return impl_as_heap(instance)->chunk_count;
}


static afw_size_t
impl_subtree_bytes_allocated(const afw_pool_internal_self_t *self)
{
    const afw_pool_internal_self_t *child;
    afw_size_t n;

    n = self->bytes_allocated;
    for (child = self->first_child; child; child = child->next_sibling) {
        n += impl_subtree_bytes_allocated(child);
    }
    return n;
}


static afw_size_t
impl_subtree_chunk_bytes(const afw_pool_internal_self_t *self)
{
    const afw_pool_internal_self_t *child;
    afw_size_t n;

    n = 0;
    if (afw_pool_internal_is_heap(&self->pub)) {
        n = impl_as_heap(self)->chunk_bytes;
    }
    for (child = self->first_child; child; child = child->next_sibling) {
        n += impl_subtree_chunk_bytes(child);
    }
    return n;
}


AFW_DEFINE(afw_size_t)
afw_pool_subtree_bytes_allocated(const afw_pool_t *instance)
{
    if (!instance) {
        return 0;
    }
    return impl_subtree_bytes_allocated(
        (const afw_pool_internal_self_t *)instance);
}


AFW_DEFINE(afw_size_t)
afw_pool_subtree_chunk_bytes(const afw_pool_t *instance)
{
    if (!instance) {
        return 0;
    }
    return impl_subtree_chunk_bytes(
        (const afw_pool_internal_self_t *)instance);
}


/* Release the value registered with afw_pool_release_value_at_cleanup(). */
static void
impl_release_value_at_cleanup(
    void *data, void *data2, const afw_pool_t *p, afw_xctx_t *xctx)
{
    (void)data2;
    (void)p;
    afw_value_release((const afw_value_t *)data, xctx);
}


AFW_DEFINE(afw_boolean_t)
afw_pool_is_value_release_registered(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *self;
    afw_pool_cleanup_t *e;

    (void)xctx;
    if (!value || !p) {
        return false;
    }
    self = (afw_pool_internal_self_t *)p;
    for (e = self->first_cleanup; e; e = e->next_cleanup) {
        if (e->cleanup == impl_release_value_at_cleanup &&
            e->data == (void *)value &&
            e->data2 == NULL)
        {
            return true;
        }
    }
    return false;
}


/* Release a value when a pool is destroyed. */
AFW_DEFINE(void)
afw_pool_release_value_at_cleanup(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    if (!value) {
        return;
    }
    /* Permanents / compile literals: nothing to release. */
    if (!value->inf || !value->inf->optional_release) {
        return;
    }
    if (afw_pool_is_value_release_registered(value, p, xctx)) {
        return;
    }
    afw_pool_register_cleanup(p, (void *)value, NULL,
        impl_release_value_at_cleanup, xctx);
}
