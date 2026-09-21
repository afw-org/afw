// See the 'COPYING' file in the project root for licensing information.
/*
 * Tracker pool internals.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_POOL_TRACKER_INTERNAL_H__
#define __AFW_POOL_TRACKER_INTERNAL_H__

#include "afw_pool_heap_internal.h"

/**
 * @file afw_pool_tracker_internal.h
 * @brief Tracker store internals (`afw_pool_tracker.c`).
 *
 * A tracker gets memory from a heap, tracks live USER blocks, and
 * returns them to the heap on tracker destroy (or garbage_collect
 * for marked frees). Includes the heap store header because collect
 * and destroy call `afw_pool_heap_internal_add_to_free_list`.
 *
 * Tracker live: [next][size][USER] or, if AFW_DEBUG_POOL,
 * [next][size][pool][USER]. Low bit of next marks a freed block.
 */

AFW_BEGIN_DECLARES

typedef struct afw_pool_tracker_node_s afw_pool_tracker_node_t;
struct afw_pool_tracker_node_s {
    /* Forward-only. Low bit of next marks freed (collect / destroy). */
    afw_pool_tracker_node_t *next;
#ifdef AFW_DEBUG_POOL
    afw_pool_debug_prefix_t debug;
#else
    afw_size_t size;
#endif
};

#define AFW_POOL_TRACKER_PREFIX_BYTES sizeof(afw_pool_tracker_node_t)

#define AFW_POOL_TRACKER_FREED_BIT ((uintptr_t)1)

#define AFW_POOL_TRACKER_NODE(_user) \
    ((afw_pool_tracker_node_t *)((char *)(_user) - \
        AFW_POOL_TRACKER_PREFIX_BYTES))

#define AFW_POOL_TRACKER_TO_USER(_node) \
    ((void *)((char *)(_node) + AFW_POOL_TRACKER_PREFIX_BYTES))

#ifdef AFW_DEBUG_POOL
#define AFW_POOL_TRACKER_USER_SIZE(_node) ((_node)->debug.size)
#else
#define AFW_POOL_TRACKER_USER_SIZE(_node) ((_node)->size)
#endif

#define AFW_POOL_TRACKER_NEXT(_node) \
    ((afw_pool_tracker_node_t *) \
        ((uintptr_t)((_node)->next) & ~AFW_POOL_TRACKER_FREED_BIT))

#define AFW_POOL_TRACKER_IS_FREED(_node) \
    (((uintptr_t)((_node)->next) & AFW_POOL_TRACKER_FREED_BIT) != 0)

#define AFW_POOL_TRACKER_MARK_FREED(_node) \
    ((_node)->next = (afw_pool_tracker_node_t *) \
        ((uintptr_t)((_node)->next) | AFW_POOL_TRACKER_FREED_BIT))


typedef struct afw_pool_internal_tracker_self_s
afw_pool_internal_tracker_self_t;

struct afw_pool_internal_tracker_self_s {

    afw_pool_internal_self_t common;

    /** @brief First live or marked allocation. */
    afw_pool_tracker_node_t *first_allocated_memory;
};


#define afw_pool_tracker_internal_as_tracker(_self) \
    ((afw_pool_internal_tracker_self_t *)(_self))

AFW_END_DECLARES

#endif /* __AFW_POOL_TRACKER_INTERNAL_H__ */
