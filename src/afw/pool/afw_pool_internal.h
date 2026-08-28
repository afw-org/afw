// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework memory pool internal header.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_POOL_INTERNAL_H__
#define __AFW_POOL_INTERNAL_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_pool_internal
 * @{
 */

/**
 * @file afw_pool_internal.h
 * @brief Shared pool internals for libafw (create base pool, debug).
 *
 * Heap/tracker structs live in afw_pool_heap_internal.h (afw_pool_heap.c
 * only). The general-pool instance struct lives in afw_pool.c.
 */

AFW_BEGIN_DECLARES

/**
 * @internal
 * @brief Allocate base pool.
 * @return Pointer to base pool or NULL if there is an error.
 *
 * Multithreaded. Only used in afw_environment_create().
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_internal_create_base_pool();

AFW_DECLARE(afw_boolean_t)
afw_pool_internal_is_heap(const afw_pool_t *p);

AFW_DECLARE(afw_boolean_t)
afw_pool_internal_is_heap_multithreaded(const afw_pool_t *p);

AFW_DECLARE(afw_boolean_t)
afw_pool_internal_is_tracker(const afw_pool_t *p);

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

/** @} */

#endif /* __AFW_POOL_INTERNAL_H__ */
