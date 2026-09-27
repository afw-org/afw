// See the 'COPYING' file in the project root for licensing information.
/*
 * Multithreaded heap pool inf.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_pool_heap_multithreaded.c
 * @brief Multithreaded heap inf. Wrappers around the heap store.
 *
 * Holds the multithreaded region's mutex, then calls the
 * single-threaded heap methods. The store stays in afw_pool_heap.c.
 */

#include "afw_internal.h"
#include <stddef.h>

#define AFW_POOL_SELF_T afw_pool_internal_self_t

static const afw_pool_t *
impl_mt_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    const afw_pool_t *result;

    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_BEGIN(self) {
        result = afw_pool_heap_internal_release(self, xctx);
    }
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_END;
    return result;
}

static void
impl_mt_afw_pool_get_reference(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_BEGIN(self) {
        afw_pool_internal_get_reference(self, xctx);
    }
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_afw_pool_run_cleanups(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_BEGIN(self) {
        afw_pool_heap_internal_run_cleanups(self, xctx);
    }
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_BEGIN(self) {
        afw_pool_heap_internal_destroy(self, xctx);
    }
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_END;
}

static void *
impl_mt_afw_pool_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_BEGIN(self) {
        result = afw_pool_heap_internal_calloc(self, size, xctx);
    }
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_END;
    return result;
}

static void *
impl_mt_afw_pool_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_BEGIN(self) {
        result = afw_pool_heap_internal_malloc(self, size, xctx);
    }
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_END;
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
    region = afw_pool_internal_memory_region(self);
    if (region) {
        afw_memory_region_lock(region, xctx);
    }
    result = afw_pool_heap_internal_calloc_no_throw(self, size, xctx);
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

    region = afw_pool_internal_memory_region(self);
    if (region) {
        afw_memory_region_lock(region, xctx);
    }
    result = afw_pool_heap_internal_malloc_no_throw(self, size, xctx);
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
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_BEGIN(self) {
        afw_pool_heap_internal_free_memory(self, address, size, xctx);
    }
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_afw_pool_free_memory_no_throw(
    AFW_POOL_SELF_T *self,
    void *address,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    const afw_memory_region_t *region;

    region = afw_pool_internal_memory_region(self);
    if (region) {
        afw_memory_region_lock(region, xctx);
    }
    afw_pool_heap_internal_free_memory_no_throw(self, address, size, xctx);
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
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_BEGIN(self) {
        afw_pool_internal_register_cleanup(
            self, data, data2, cleanup, xctx);
    }
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_afw_pool_deregister_cleanup(
    AFW_POOL_SELF_T *self,
    void *data,
    void *data2,
    afw_pool_cleanup_function_p_t cleanup,
    afw_xctx_t *xctx)
{
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_BEGIN(self) {
        afw_pool_internal_deregister_cleanup(
            self, data, data2, cleanup, xctx);
    }
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_END;
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
#define impl_afw_pool_garbage_collect afw_pool_heap_internal_garbage_collect

#define AFW_IMPLEMENTATION_ID "heap_multithreaded"
#define AFW_IMPLEMENTATION_INF_LABEL afw_pool_heap_internal_multithreaded_inf
#define AFW_IMPLEMENTATION_INF_SPECIFIER(type_) const type_

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
#undef AFW_IMPLEMENTATION_INF_SPECIFIER
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
#undef impl_afw_pool_register_cleanup
#undef impl_afw_pool_deregister_cleanup
#undef impl_afw_pool_garbage_collect

afw_pool_internal_self_t *
afw_pool_heap_internal_multithreaded_create_self(
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

    if (!xctx) {
        return NULL;
    }
    if (!thread) {
        thread = xctx->thread;
    }
    if (!thread) {
        AFW_THROW_ERROR_Z(general, "Heap requires a thread", xctx);
    }
    region = (xctx->env)
        ? ((const afw_environment_t *)xctx->env)
            ->multithreaded_memory_region
        : NULL;
    if (!region) {
        AFW_THROW_ERROR_Z(general,
            "Multithreaded heap requires the environment region",
            xctx);
    }
    afw_memory_region_lock(region, xctx);
    self = afw_pool_heap_internal_allocate_self(inf, chunk_min, self_bytes,
        region, xctx);
    if (!self) {
        afw_memory_region_unlock(region, xctx);
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
    if (heap->chunk_bytes) {
        afw_pool_internal_env_add_chunks((afw_environment_t *)xctx->env,
            heap->chunk_bytes);
    }
    afw_memory_region_unlock(region, xctx);
    return self;
}


const afw_pool_t *
afw_pool_heap_internal_multithreaded_create(
    const afw_pool_t *parent,
    afw_boolean_t as_managed_p,
    afw_size_t chunk_min,
    afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *self;

    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required", xctx);
    }
    self = afw_pool_heap_internal_multithreaded_create_self(parent,
        &afw_pool_heap_internal_multithreaded_inf,
        as_managed_p, chunk_min,
        sizeof(afw_pool_heap_internal_self_with_free_memory_head_t),
        NULL, xctx);
    return &self->pub;
}
