// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Lock Support
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_LOCK_H__
#define __AFW_LOCK_H__

#include "afw_interface.h"

/**
 * @defgroup afw_lock Locks
 * @ingroup afw_c_api_public
 *
 * Locks API.
 * @{
 */

/**
 * @file afw_lock.h
 * @brief Adaptive Framework locks
 */

AFW_BEGIN_DECLARES

/**  @brief Lock type. */
typedef enum {
    afw_lock_type_global_mutex,
    afw_lock_type_process_mutex,
    afw_lock_type_thread_mutex,
    afw_lock_type_thread_recursive_mutex,
    afw_lock_type_thread_read_write
} afw_lock_type_t;


/**
 * @internal
 * @brief Internal struct of lock
 */
struct afw_lock_s {
    const afw_utf8_t *lock_id;
    const afw_utf8_t *brief;
    const afw_utf8_t *description;
    const afw_utf8_t *flag_id_debug;
    afw_size_t flag_index_debug;
    union {

        /** @brief for type thread_mutex */
        apr_thread_mutex_t *mutex;

        /** @brief for type thread_read_write */
        apr_thread_rwlock_t *rwlock;
    };
    afw_lock_type_t lock_type;
};


/**
 * @internal
 * @brief Internal struct of read/write lock
 *
 * afw_lock_rw_s is the same struct as afw_lock_s but is done this way
 * to get a compile-time casting error if read/write only functions/macros
 * are used with non read/write locks.
 */
struct afw_lock_rw_s {
    struct afw_lock_s lock;
};


/**
 * @brief Create a lock that will last for life of pool.
 * @param lock_id
 * @param insure_recursive_lock true insures this is a recursive lock.  If
 *    false the lock will be recursive only if it's the platform default.
 * @param p for lock.
 * @param xctx of caller.
 */
AFW_DECLARE(const afw_lock_t *)
afw_lock_create(
    const afw_utf8_t *lock_id,
    const afw_utf8_t *brief,
    const afw_utf8_t *description,
    afw_boolean_t insure_recursive_lock,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Create a lock and register in environment.
 * @param lock_id is required
 * @param insure_recursive_lock true insures this is a recursive lock.  If
 *    false the lock will be recursive only if it's the platform default.
 * @param xctx of caller.
 */
AFW_DECLARE(const afw_lock_t *)
afw_lock_create_and_register(
    const afw_utf8_t *lock_id,
    const afw_utf8_t *brief,
    const afw_utf8_t *description,
    afw_boolean_t insure_recursive_lock,
    afw_xctx_t *xctx);


/**
 * @internal
 * @brief Internally called to create lock before afw_pool is ready.
 * @param lock_id
 * @param p for lock.
 * @param xctx of caller.
 */
AFW_DECLARE(const afw_lock_t *)
afw_lock_create_environment_lock(
    const afw_utf8_t *lock_id,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Obtain lock.
 * @param instance of lock or NULL to ignore.
 * @param xctx of caller.
 * 
 * Call afw_lock_release() to release the lock.
 */
AFW_DECLARE(void)
afw_lock_obtain(const afw_lock_t *instance, afw_xctx_t *xctx);


/**
 * @brief Debug version of obtain lock.
 * @param instance of lock or NULL to ignore.
 * @param xctx of caller.
 * @param source_z file:line where function called.
 * @sa afw_lock_read()
 */
AFW_DECLARE(void)
afw_lock_obtain_debug(const afw_lock_t *instance,
    afw_xctx_t *xctx, const afw_utf8_z_t *source_z);


#ifdef AFW_LOCK_DEBUG
#define afw_lock_obtain(instance,xctx) \
    afw_lock_obtain_debug(instance, xctx, AFW__FILE_LINE__)
#endif


/**
 * @brief Release lock.
 * @param instance of lock or NULL to ignore.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_lock_release(const afw_lock_t *instance, afw_xctx_t *xctx);


/**
 * @brief Debug version of release lock.
 * @param instance of lock or NULL to ignore.
 * @param xctx of caller.
 * @param source_z file:line where function called.
 * @sa afw_lock_unlock_read()
 */
AFW_DECLARE(void)
afw_lock_release_debug(const afw_lock_t *instance,
    afw_xctx_t *xctx, const afw_utf8_z_t *source_z);


#ifdef AFW_LOCK_DEBUG
#define afw_lock_release(instance,xctx) \
    afw_lock_release_debug(instance, xctx, AFW__FILE_LINE__)
#endif


/**
 * @brief Macro to begin a lock section.
 * @param instance lock.
 * 
 * The variable name of the current xctx must be named xctx.
 *
 * Usage:
 *
 * AFW_LOCK_BEGIN(instance) {
 *    ... code
 * }
 * AFW_LOCK_END(instance);
 */
#define AFW_LOCK_BEGIN(instance) \
const afw_lock_t *this_LOCK = instance; \
afw_lock_obtain(this_LOCK, xctx); \
AFW_TRY


/**
 * @brief Macro to end a lock section.
 *
 * See AFW_LOCK_BEGIN for usage.
 */
#define AFW_LOCK_END \
AFW_FINALLY { \
    afw_lock_release(this_LOCK, xctx); \
} \
AFW_ENDTRY



/**
 * @brief Create a read/write lock that will last for life of pool.
 * @param lock_id
 * @param p for lock.
 * @param xctx of caller.
 */
AFW_DECLARE(const afw_lock_rw_t *)
afw_lock_create_rw(
    const afw_utf8_t *lock_id,
    const afw_utf8_t *brief,
    const afw_utf8_t *description,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create a read/write lock and register in environment.
 * @param lock_id
 * @param xctx of caller.
 */
AFW_DECLARE(const afw_lock_rw_t *)
afw_lock_create_rw_and_register(
    const afw_utf8_t *lock_id,
    const afw_utf8_t *brief,
    const afw_utf8_t *description,
    afw_xctx_t *xctx);



/**
 * @brief Obtain read lock.
 * @param instance of read write lock or NULL to ignore.
 * @param xctx of caller.
 * 
 * Call afw_lock_unlock_read() to release the lock.
 */
AFW_DECLARE(void)
afw_lock_read_obtain(const afw_lock_rw_t *instance, afw_xctx_t *xctx);


/**
 * @brief Debug version of obtain read lock.
 * @param instance of read write lock or NULL to ignore.
 * @param xctx of caller.
 * @param source_z file:line where function called.
 * @sa afw_lock_read()
 */
AFW_DECLARE(void)
afw_lock_read_obtain_debug(const afw_lock_rw_t *instance,
    afw_xctx_t *xctx, const afw_utf8_z_t *source_z);


#ifdef AFW_LOCK_DEBUG
#define afw_lock_read_obtain(instance,xctx) \
    afw_lock_read_obtain_debug(instance, xctx, AFW__FILE_LINE__)
#endif


/**
 * @brief Release read lock.
 * @param instance of read write lock.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_lock_read_release(const afw_lock_rw_t *instance, afw_xctx_t *xctx);


/**
 * @brief Debug version of release read lock.
 * @param instance of read write lock or NULL to ignore.
 * @param xctx of caller.
 * @param source_z file:line where function called.
 * @sa afw_lock_unlock_read()
 */
AFW_DECLARE(void)
afw_lock_read_release_debug(const afw_lock_rw_t *instance,
    afw_xctx_t *xctx, const afw_utf8_z_t *source_z);


#ifdef AFW_LOCK_DEBUG
#define afw_lock_read_release(instance,xctx) \
    afw_lock_read_release_debug(instance, xctx, AFW__FILE_LINE__)
#endif


/**
 * @brief Macro to begin a read lock section for pool.
 * @param instance of read write lock or NULL to ignore.
 *
 * The variable name of the current xctx must be named xctx.
 *
 * Usage:
 * 
 * AFW_LOCK_READ_BEGIN(instance, xctx) {
 *    ... code that uses pool
 * }
 * AFW_LOCK_READ_END;
 */
#define AFW_LOCK_READ_BEGIN(instance) \
const afw_lock_rw_t *this_LOCK = instance; \
    afw_lock_read_obtain(this_LOCK, xctx); \
AFW_TRY


/**
 * @brief Macro to end a read lock section for pool.
 *
 * See AFW_LOCK_READ_BEGIN for usage.
 */
#define AFW_LOCK_READ_END \
AFW_FINALLY { \
    afw_lock_read_release(this_LOCK, xctx); \
} \
AFW_ENDTRY



/**
 * @brief Obtain write lock.
 * @param instance of read write lock or NULL to ignore.
 * @param xctx of caller.
 * 
 * Call afw_lock_unlock_write() to release the lock. 
 */
AFW_DECLARE(void)
afw_lock_write_obtain(const afw_lock_rw_t *instance, afw_xctx_t *xctx);


/**
 * @brief Debug version of obtain write lock.
 * @param instance of read write lock or NULL to ignore.
 * @param xctx of caller.
 * @param source_z file:line where function called.
 * @sa afw_lock_write()
 */
AFW_DECLARE(void)
afw_lock_write_obtain_debug(const afw_lock_rw_t *instance,
    afw_xctx_t *xctx, const afw_utf8_z_t *source_z);


#ifdef AFW_LOCK_DEBUG
#define afw_lock_write_obtain(instance,xctx) \
    afw_lock_write_obtain_debug(instance, xctx, AFW__FILE_LINE__)
#endif


/**
 * @brief Release write lock.
 * @param instance of read write lock or NULL to ignore.
 * @param xctx of caller.
 *
 * afw_lock_unlock_write() must be called from the thread that has the
 * pool locked for write.
 */
AFW_DECLARE(void)
afw_lock_write_release(const afw_lock_rw_t *instance, afw_xctx_t *xctx);


/**
 * @brief Debug version of release write lock.
 * @param instance of read write lock or NULL to ignore.
 * @param xctx of caller.
 * @param source_z file:line where function called.
 * @sa afw_lock_unlock_write()
 */
AFW_DECLARE(void)
afw_lock_write_release_debug(const afw_lock_rw_t *instance,
    afw_xctx_t *xctx, const afw_utf8_z_t *source_z);


#ifdef AFW_LOCK_DEBUG
#define afw_lock_write_release(instance,xctx) \
    afw_lock_write_release_debug(instance, xctx, AFW__FILE_LINE__)
#endif


/**
 * @brief Macro to begin a write lock section.
 * @param instance of read write lock or NULL to ignore.
 *
 * The variable name of the current xctx must be named xctx.
 *
 * Usage:
 * 
 * AFW_LOCK_WRITE_BEGIN(instance, xctx) {
 *    ... code that uses pool
 * }
 * AFW_LOCK_WRITE_END;
 */
#define AFW_LOCK_WRITE_BEGIN(instance) \
const afw_lock_rw_t *this_LOCK = instance; \
    afw_lock_write_obtain(this_LOCK, xctx); \
AFW_TRY


/**
 * @brief Macro to end a write lock section.
 *
 * See AFW_LOCK_WRITE_BEGIN for usage.
 */
#define AFW_LOCK_WRITE_END \
AFW_FINALLY { \
    afw_lock_write_release(this_LOCK, xctx); \
} \
AFW_ENDTRY

AFW_END_DECLARES

/** @} */

#endif /* __AFW_LOCK_H__ */
