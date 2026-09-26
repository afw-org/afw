// See the 'COPYING' file in the project root for licensing information.
/*
 * Multithreaded tracker pool inf.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_pool_tracker_multithreaded.c
 * @brief Multithreaded tracker inf. Wrappers around the tracker store.
 */

#include "afw_internal.h"

#define AFW_POOL_SELF_T afw_pool_internal_self_t

static const afw_pool_t *
impl_mt_tracker_afw_pool_release(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    const afw_pool_t *result;

    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_BEGIN(self) {
        result = afw_pool_internal_tracker_release(self, xctx);
    }
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_END;
    return result;
}

static void
impl_mt_tracker_afw_pool_get_reference(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_BEGIN(self) {
        afw_pool_internal_get_reference(self, xctx);
    }
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_tracker_afw_pool_run_cleanups(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_BEGIN(self) {
        afw_pool_internal_tracker_run_cleanups(self, xctx);
    }
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_tracker_afw_pool_destroy(
    AFW_POOL_SELF_T *self,
    afw_xctx_t *xctx)
{
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_BEGIN(self) {
        afw_pool_internal_tracker_destroy(self, xctx);
    }
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_END;
}

static void *
impl_mt_tracker_afw_pool_calloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_BEGIN(self) {
        result = afw_pool_internal_tracker_calloc(self, size, xctx);
    }
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_END;
    return result;
}

static void *
impl_mt_tracker_afw_pool_malloc(
    AFW_POOL_SELF_T *self,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    void *result;

    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_BEGIN(self) {
        result = afw_pool_internal_tracker_malloc(self, size, xctx);
    }
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_END;
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

    region = afw_pool_internal_memory_region(self);
    if (region) {
        afw_memory_region_lock(region, xctx);
    }
    result = afw_pool_internal_tracker_calloc_no_throw(self, size, xctx);
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

    region = afw_pool_internal_memory_region(self);
    if (region) {
        afw_memory_region_lock(region, xctx);
    }
    result = afw_pool_internal_tracker_malloc_no_throw(self, size, xctx);
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
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_BEGIN(self) {
        afw_pool_internal_tracker_free_memory(self, address, size, xctx);
    }
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_tracker_afw_pool_free_memory_no_throw(
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
    afw_pool_internal_tracker_free_memory_no_throw(
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
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_BEGIN(self) {
        afw_pool_internal_tracker_garbage_collect(self, xctx);
    }
    AFW_POOL_INTERNAL_MULTITHREADED_LOCK_END;
}

static void
impl_mt_tracker_afw_pool_register_cleanup(
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
impl_mt_tracker_afw_pool_deregister_cleanup(
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

#define impl_afw_pool_release impl_mt_tracker_afw_pool_release
#define impl_afw_pool_get_reference impl_mt_tracker_afw_pool_get_reference
#define impl_afw_pool_run_cleanups impl_mt_tracker_afw_pool_run_cleanups
#define impl_afw_pool_destroy impl_mt_tracker_afw_pool_destroy
#define impl_afw_pool_calloc impl_mt_tracker_afw_pool_calloc
#define impl_afw_pool_malloc impl_mt_tracker_afw_pool_malloc
#define impl_afw_pool_calloc_no_throw impl_mt_tracker_afw_pool_calloc_no_throw
#define impl_afw_pool_malloc_no_throw impl_mt_tracker_afw_pool_malloc_no_throw
#define impl_afw_pool_free_memory impl_mt_tracker_afw_pool_free_memory
#define impl_afw_pool_free_memory_no_throw \
    impl_mt_tracker_afw_pool_free_memory_no_throw
#define impl_afw_pool_garbage_collect impl_mt_tracker_afw_pool_garbage_collect
#define impl_afw_pool_register_cleanup impl_mt_tracker_afw_pool_register_cleanup
#define impl_afw_pool_deregister_cleanup \
    impl_mt_tracker_afw_pool_deregister_cleanup

#define AFW_IMPLEMENTATION_ID "tracker_multithreaded"
#define AFW_IMPLEMENTATION_INF_LABEL afw_pool_internal_tracker_multithreaded_inf
#define AFW_IMPLEMENTATION_INF_SPECIFIER(type_) const type_

static const afw_pool_internal_inf_implementation_specific_t
impl_tracker_mt_implementation_specific =
    {
        /* multithreaded */ true,
        /* tracker */ true
    };

#define AFW_IMPLEMENTATION_SPECIFIC &impl_tracker_mt_implementation_specific

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
#undef impl_afw_pool_garbage_collect
#undef impl_afw_pool_register_cleanup
#undef impl_afw_pool_deregister_cleanup

afw_pool_internal_self_t *
afw_pool_internal_tracker_multithreaded_create_self(
    afw_pool_internal_self_t *parent,
    afw_size_t self_bytes,
    afw_xctx_t *xctx)
{
    return afw_pool_internal_tracker_create(parent,
        &afw_pool_internal_tracker_multithreaded_inf,
        self_bytes, xctx);
}
