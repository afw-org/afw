// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Lock Internal Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_LOCK_INTERNAL_H__
#define __AFW_LOCK_INTERNAL_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_lock
 * @{
 */

/**
 * @file afw_lock_internal.h
 * @brief Internal lock helpers for libafw (lock/ module).
 *
 * Declarations for symbols implemented in afw_lock.c that are not part of
 * the public lock API. Include via afw_internal.h only.
 */

AFW_BEGIN_DECLARES

/** Lock type (impl). */
typedef enum {
    afw_lock_type_global_mutex,
    afw_lock_type_process_mutex,
    afw_lock_type_thread_mutex,
    afw_lock_type_thread_recursive_mutex,
    afw_lock_type_thread_read_write
} afw_lock_type_t;

struct afw_lock_s {
    const afw_utf8_t *lock_id;
    const afw_utf8_t *brief;
    const afw_utf8_t *description;
    const afw_utf8_t *flag_id_debug;
    afw_size_t flag_index_debug;
    union {
        afw_thread_mutex_t *mutex;
        afw_thread_rwlock_t *rwlock;
    };
    afw_lock_type_t lock_type;
};

/**
 * Same layout as afw_lock_s; separate type so rw-only macros/functions
 * fail to compile with a non-rw lock.
 */
struct afw_lock_rw_s {
    struct afw_lock_s lock;
};

/**
 * @brief Create nested env lock before pool is fully ready (libafw only).
 * @param lock_id of lock.
 * @param p for pool used for the lock.
 * @param xctx of caller.
 *
 * Called from environment create.
 */
const afw_lock_t *
afw_lock_create_environment_nested_lock(
    const afw_utf8_t *lock_id,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_END_DECLARES

/** @} */

#endif /* __AFW_LOCK_INTERNAL_H__ */
