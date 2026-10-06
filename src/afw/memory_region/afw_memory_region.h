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
 * allocated with C calloc; release() frees it. Chunks are mapped
 * pages (page aligned, 4096) via afw_os_map_pages(). Unmap returns
 * pages the free list does not keep.
 */

AFW_BEGIN_DECLARES

/** @brief Page alignment for mapped chunks. Same as AFW_POOL_HEAP_INTERNAL_CHUNK_ALIGN. */
#define AFW_MEMORY_REGION_ALIGN ((afw_size_t)4096)

/**
 * @brief Default cap on free_list_bytes. 0 on create() is still
 * passthrough (map/unmap every get/free).
 */
#define AFW_MEMORY_REGION_FREE_LIST_MAX_BYTES \
    ((afw_size_t)(256 * 1024))

/**
 * @brief Default keep_small_count: newest small regions trim() keeps
 * resident.
 */
#define AFW_MEMORY_REGION_KEEP_SMALL_COUNT ((afw_size_t)8)

/**
 * @brief Default keep_large_count: newest large regions trim() keeps
 * resident.
 */
#define AFW_MEMORY_REGION_KEEP_LARGE_COUNT ((afw_size_t)1)

/**
 * @brief Create a memory_region instance.
 * @param free_list_max_bytes cap on the free list; 0 = no reuse.
 * @param xctx of caller. May be NULL (environment create).
 * @return new instance, or NULL on allocation failure.
 *
 * Does not throw. Instance is C calloc; release() cleanup then free.
 * Until afw_memory_region_configure(), there are no small or large
 * lists (every size is on the other list) and the keep counts are
 * the defaults.
 */
AFW_DECLARE(const afw_memory_region_t *)
afw_memory_region_create(
    afw_size_t free_list_max_bytes,
    afw_xctx_t *xctx);

/**
 * @brief Copy the env memory_region knobs into an instance.
 * @param instance from create(). May be NULL.
 * @param env with the knobs: memory_region_free_list_max_bytes,
 *    memory_region_keep_small_count, memory_region_keep_large_count,
 *    and small_chunk_min / xctx_chunk_min as the small and large
 *    sizes.
 * @param xctx of caller. May be NULL.
 *
 * A changed small or large size drains the free list (cleanup), so
 * every region on a list matches its size. Otherwise does not drain;
 * later free() and trim() use the new values. The caller holds the
 * region's lock if other threads use it.
 */
AFW_DECLARE(void)
afw_memory_region_configure(
    const afw_memory_region_t *instance,
    const afw_environment_t *env,
    afw_xctx_t *xctx);

AFW_END_DECLARES

/** @} */

#endif /* __AFW_MEMORY_REGION_H__ */
