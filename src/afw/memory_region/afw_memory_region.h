// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework memory_region support header.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_MEMORY_REGION_H__
#define __AFW_MEMORY_REGION_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_memory_region
 * @{
 */

/**
 * @file afw_memory_region.h
 * @brief Thread-owned reuse of page-aligned heap chunks.
 *
 * See the @ref afw_memory_region group. Not a pool. The instance is
 * allocated with C calloc; release() frees it. Alignment matches
 * heap posix_memalign (4096).
 */

AFW_BEGIN_DECLARES

/** @brief posix_memalign alignment (page). Same as AFW_POOL_CHUNK_ALIGN. */
#define AFW_MEMORY_REGION_ALIGN ((afw_size_t)4096)

/**
 * @brief Default cap on free_list_bytes. 0 on create() is still
 * passthrough (posix_memalign/free every get/free).
 */
#define AFW_MEMORY_REGION_FREE_LIST_MAX_BYTES \
    ((afw_size_t)(256 * 1024))

/**
 * @brief Create a memory_region instance.
 * @param free_list_max_bytes cap on the free list; 0 = no reuse.
 * @param xctx of caller. May be NULL (environment create).
 * @return new instance, or NULL on allocation failure.
 *
 * Does not throw. Instance is C calloc; release() cleanup then free.
 */
AFW_DECLARE(const afw_memory_region_t *)
afw_memory_region_create(
    afw_size_t free_list_max_bytes,
    afw_xctx_t *xctx);

AFW_END_DECLARES

/** @} */

#endif /* __AFW_MEMORY_REGION_H__ */
