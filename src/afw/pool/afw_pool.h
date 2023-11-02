// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework memory pool support header.
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_POOL_H__
#define __AFW_POOL_H__

#include "afw_interface.h"

/**
 * @defgroup afw_pool Pool
 * @ingroup afw_c_api_public
 *
 * Pool support
 *
 * @{
 */

/**
 * @file afw_pool.h
 * @brief Adaptive Framework memory pool support header.
 */

AFW_BEGIN_DECLARES

/** @brief Typedef for registered cleanup functions. */
typedef struct afw_pool_cleanup_s afw_pool_cleanup_t;


/** @brief Struct for registered cleanup functions. */
struct afw_pool_cleanup_s {

    /* *** next_cleanup MUST BE FIRST *** */
    /** @brief Next cleanup function. */
    afw_pool_cleanup_t *next_cleanup;

    /* @brief Cleanup function. */
    afw_pool_cleanup_function_p_t cleanup;

    /* @brief Parameter data passed to function. */
    void *data;

    /* @brief Parameter data2 passed to function. */
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
 * The base pool (xctx->env->p) for the environment is created when the
 * AFW environment is created and is a multithreaded pool.
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_create(
    const afw_pool_t *parent,
    afw_xctx_t *xctx);


/**
 * @brief Create a subpool of a pool.
 * @param parent of new subpool.
 * @param xctx of caller.
 * @return new pool.
 *
 * This creates a subpool of a parent pool. Memory is allocated from the parent
 * pool. When memory is freed in the subpool or it is destroyed, the freed
 * memory is returned to the parent pool.
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_create_subpool(
    const afw_pool_t *parent, 
    afw_xctx_t *xctx);



/**
 * @brief Create a new pool.
 * @param parent of new pool.
 * @param xctx of caller.
 * @return new pool.
 *
 * A pool created with this function is either thread specific or a
 * multithreaded unmanaged pool, depending on the parent. The free() method
 * of an unmanaged pool is ignored. The only way to free the memory allocated
 * from an unmanaged pool is to destroy the pool.
 *
 * If the parent is a thread specific pool, the created pool will also be thread
 * specific. Thread specific pools are single threaded and are not thread safe.
 * If any of the pool functions are called from other than the specific thread,
 * an error is thrown.
 *
 * The only way to create a thread specific pool is by calling the
 * afw_thread_create() function and accessing the thread struct's p member. If
 * running in an AFW server, this is done when a request thread is created.
 *
 * If the parent is a multithread pool, the created pool will also be a
 * multithreaded pool.
 *
 * The base pool (xctx->env->p) for the environment is created when the
 * AFW environment is created and is a multithreaded pool.
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_create_unmanaged(
    const afw_pool_t *parent,
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


AFW_END_DECLARES

/** @} */

#endif /* __AFW_POOL_H__ */
