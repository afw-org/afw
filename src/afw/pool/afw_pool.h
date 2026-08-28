// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework memory pool support header.
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

#ifndef __AFW_POOL_H__
#define __AFW_POOL_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_pool
 * @{
 */

/**
 * @file afw_pool.h
 * @brief Adaptive Framework memory pool support header.
 *
 * See the @ref afw_pool group (defined in afw_doxygen.h) for the mental model.
 *
 * Key invariants:
 * - A pool is a heap unless it is a tracker. A tracker gets memory
 *   from a heap, tracks it, and returns it on free or tracker destroy.
 *   Multithreaded heap is lock wrappers. APR is the heap reservoir,
 *   not a third AFW pool kind.
 * - afw_pool_create() of a heap is a heap (mt if the parent is mt).
 *   Of a tracker, a tracker. xctx->p is always single-thread heap.
 * - afw_pool_get_apr_pool() is a door for leftover APR function calls,
 *   not the heap's store.
 * - Optional free is afw_pool_free_memory(p, address, size, xctx).
 * - Use afw_pool_calloc_type for typed zeroed allocs.
 * - Cleanup functions run before the pool is destroyed.
 */

AFW_BEGIN_DECLARES

/** @brief Typedef for registered cleanup functions. */
typedef struct afw_pool_cleanup_s afw_pool_cleanup_t;


/** @brief Struct for registered cleanup functions. */
struct afw_pool_cleanup_s {

    /* *** next_cleanup MUST BE FIRST *** */
    /** @brief Next cleanup function. */
    afw_pool_cleanup_t *next_cleanup;

    /** @brief Cleanup function. */
    afw_pool_cleanup_function_p_t cleanup;

    /** @brief Parameter data passed to function. */
    void *data;

    /** @brief Parameter data2 passed to function. */
    void *data2;
};


/**
 * @brief Create a new pool.
 * @param parent of new pool.
 * @param xctx of caller.
 * @return new pool.
 *
 * Heap if the parent is a heap (multithreaded lock wrappers if the
 * parent is multithreaded). Tracker if the parent is a tracker
 * (extra rule; may revisit).
 *
 * env->p is a multithreaded heap. xctx->p is always a single-thread
 * heap (see afw_pool_create_xctx_p()). Thread-specific heaps are not
 * safe from another thread.
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_create(
    const afw_pool_t *parent,
    afw_xctx_t *xctx);


/**
 * @brief Create a pool whose managed_p is itself.
 * @param parent of new pool.
 * @param xctx of caller.
 * @return new pool.
 *
 * Same as afw_pool_create() then p->managed_p = p. Use for factory/conf
 * instance pools (adapter->p, server->p, …). Parent decides mt vs
 * single-thread. For xctx->p use afw_pool_create_xctx_p().
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_create_as_managed_p(
    const afw_pool_t *parent,
    afw_xctx_t *xctx);


/**
 * @brief Create xctx->p (managed_p = self, always single-threaded).
 * @param parent of new pool (may be multithreaded env/base).
 * @param xctx of caller.
 * @return new pool.
 *
 * An xctx is one thread's work. Child xctx->p is a heap so optional
 * free can recycle. afw_pool_create() of a heap parent is a heap.
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_create_xctx_p(
    const afw_pool_t *parent,
    afw_xctx_t *xctx);


/**
 * @brief Create a tracker (scope pool).
 * @param parent heap or tracker.
 * @param xctx of caller.
 * @return tracker. managed_p is the heap.
 *
 * Single-thread only, same thread as the parent. Used as scope->p.
 * The tracker header is a heap block (`free_memory` on destroy).
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_tracker_create(
    const afw_pool_t *parent,
    afw_xctx_t *xctx);


/**
 * @brief Create thread struct in new thread specific pool with p set.
 * @param size of thread struct or -1 if sizeof(afw_thread_t) should be used.
 * @param xctx of caller.
 * @return new thread struct with p set.
 *
 * Internal. Only called from afw_thread_create(). Does not start a
 * pthread; it creates the thread-specific pool and the thread struct
 * in that pool.
 */
AFW_DECLARE(afw_thread_t *)
afw_pool_thread_create(
    afw_size_t size,
    afw_xctx_t *xctx);


/**
 * @brief Macro to allocate cleared memory to hold type in pool.
 * @param instance of pool.
 * @param type to allocate.
 * @param xctx of caller.
 * @return pointer to memory allocated cast to pointer of type.
 *
 * This is a helper macro to call afw_pool_calloc() to allocate
 * memory for a specified type and cast the return pointer to
 * a pointer to that type.
 */
#define afw_pool_calloc_type(instance, type, xctx) \
    (type *) afw_pool_calloc(instance, sizeof(type), xctx)


/**
 * @brief Macro to allocate uncleared memory to hold type in pool.
 * @param instance of pool.
 * @param type to allocate.
 * @param xctx of caller.
 * @return pointer to memory allocated cast to pointer of type.
 *
 * This is a helper macro to call afw_pool_malloc() to allocate
 * memory for a specified type and cast the return pointer to a pointer
 * to that type.
 */
#define afw_pool_malloc_type(instance, type, xctx) \
    (type *) afw_pool_malloc(instance, sizeof(type), xctx)


/**
 * @brief Optionally free a typed allocation from a pool.
 * @param instance of pool.
 * @param address returned by afw_pool_malloc/calloc.
 * @param type allocated.
 * @param xctx of caller.
 */
#define afw_pool_free_memory_type(instance, address, type, xctx) \
    afw_pool_free_memory(instance, address, sizeof(type), xctx)


AFW_END_DECLARES

/** @} */  // end of @addtogroup @addtogroup

#endif /* __AFW_POOL_H__ */
