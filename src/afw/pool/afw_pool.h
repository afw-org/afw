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
 * multithreaded pool depending on the parent.
 *
 * If the parent is a thread specific pool, the created pool will also be
 * thread specific.  Thread specific pools are single threaded and no locking
 * is allowed.  If any of the pool functions are called from other than the
 * specific thread, an error is thrown.
 *
 * If the parent is a multithread pool, the created pool will also be a
 * multithreaded pool.  Calls to lock function will be directed to the nearest
 * ancestor pool that was created with afw_pool_create_multithreaded() or the
 * base pool for the environment.
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_create(const afw_pool_t *parent, afw_xctx_t *xctx);


/**
 * @brief Create a new multithreaded pool.
 * @param parent of new pool or NULL.
 * @param xctx of caller.
 * @return new pool.
 *
 * The parent specified must be a multithreaded pool.  If NULL is specified,
 * the base pool for the environment is used.
 *
 * Use paired afw_pool_lock_read()/afw_pool_unlock_read() and
 * afw_pool_lock_write()/afw_pool_unlock_write() as
 * appropriate.  Insure this happens using AFW_TRY/AFW_FINALLY.
 *
 * For example:
 *
 *  afw_pool_lock_write(p);
 *  AFW_TRY {
 *      ... code that updates things in pool.
 *  AFW_CATCH_UNHANDLED {
 *      AFW_ERROR_RETHROW;
 *  }
 *  AFW_FINALLY {
 *      afw_pool_unlock_write(p);
 *  }
 *  AFW_ENDTRY;
 *
 * There are helper macros to do this.  See:
 * 
 *  AFW_POOL_LOCK_READ
 *  AFW_POOL_UNLOCK_READ
 *  AFW_POOL_LOCK_WRITE
 *  AFW_POOL_UNLOCK_WRITE
 *
 * It is the responsibility of the caller to obtain locks before passing
 * a pool as a parameter to a function unless clearly documented
 * otherwise.
 */
AFW_DECLARE(const afw_pool_t *)
afw_pool_create_multithreaded(const afw_pool_t *parent, afw_xctx_t *xctx);


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
