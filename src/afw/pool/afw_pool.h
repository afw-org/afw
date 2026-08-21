// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework memory pool support header.
 *
 * Copyright (c) 2010-2024 Clemson University
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
 * - Allocations are from p or parent; subpools track for release.
 * - Thread specific pools must only be used from their thread.
 * - Use afw_pool_calloc_type for typed zeroed allocs.
 * - Cleanup functions are called on release.
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
 * A pool created with this function is either thread specific or a
 * multithreaded pool, depending on the parent.
 *
 * If the parent is a thread specific pool, the created pool will also be thread
 * specific. Thread specific pools are single threaded and are not thread safe.
 * If any of the pool functions are called from other than the specific thread,
 * an error is thrown.
 *
 * The only way to create a thread specific pool is by calling the
 * afw_thread_create() function and accessing the thread struct's p member.
 *
 * If the parent is a multithread pool, the created pool will also be a
 * multithreaded pool.
 *
 * The base pool (xctx->env->p) for the environment is created when the AFW
 * environment is created and is a multithreaded pool.
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
 * An xctx is one thread's work. This avoids the env pool lock on every
 * alloc when afw creates a child xctx of base. Factory pools stay on
 * afw_pool_create_as_managed_p().
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_create_xctx_p(
    const afw_pool_t *parent,
    afw_xctx_t *xctx);


/**
 * @brief Create an evaluation heap.
 * @param parent of the heap (the p passed to compiled_value evaluate).
 * @param xctx of caller.
 * @return new heap. managed_p is self.
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_heap_create(
    const afw_pool_t *parent,
    afw_xctx_t *xctx);


/**
 * @brief Create a heap tracker (scope pool).
 * @param parent heap from afw_pool_heap_create().
 * @param xctx of caller.
 * @return tracker. managed_p is the heap.
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_heap_tracker_create(
    const afw_pool_t *parent,
    afw_xctx_t *xctx);


/**
 * @brief Create thread struct in new thread specific pool with p set.
 * @param size of thread struct or -1 if sizeof(afw_thread_t) should be used.
 * @param xctx of caller.
 * @return new thread struct with p set.
 *
 * This function may be enhanced at a future time, but at this point it should
 * be considered internal and only called from afw_thread_create().
 */
AFW_DECLARE(afw_thread_t *)
afw_pool_create_thread(
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
 * @brief Free memory in it's associated pool.
 * @param address of memory to free.
 * @param xctx of caller.
 *
 * The memory address must be one returned by a afw_pool_calloc() or
 * afw_pool_malloc() call.
 */
AFW_DECLARE(void)
afw_pool_free_memory(
    void *address,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */  // end of @addtogroup @addtogroup

#endif /* __AFW_POOL_H__ */
