// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework pool create entry points.
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

/**
 * @file afw_pool.c
 * @brief Create() / thread_create() / xctx_p: heap or tracker, not a third impl.
 *
 * APR remains the heap reservoir (apr_palloc / apr_pool_destroy), not an
 * AFW pool implementation. Multithreaded is lock wrappers on the heap.
 */

#include "afw_internal.h"

AFW_DEFINE(const afw_pool_t *)
afw_pool_create(
    const afw_pool_t *parent,
    afw_xctx_t *xctx)
{
    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required", xctx);
    }

    if (afw_pool_internal_is_tracker(parent)) {
        return afw_pool_heap_tracker_create(parent, xctx);
    }
    return afw_pool_internal_heap_create(parent,
        afw_pool_internal_is_heap_multithreaded(parent), xctx);
}


AFW_DEFINE(const afw_pool_t *)
afw_pool_create_as_managed_p(
    const afw_pool_t *parent,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p;

    p = afw_pool_create(parent, xctx);
    ((afw_pool_t *)p)->managed_p = p;
    return p;
}


AFW_DEFINE(const afw_pool_t *)
afw_pool_create_xctx_p(
    const afw_pool_t *parent,
    afw_xctx_t *xctx)
{
    if (!parent) {
        AFW_THROW_ERROR_Z(general, "Parent required", xctx);
    }

    return afw_pool_heap_create(parent, xctx);
}
