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

/**
 * @brief Create nested env lock before pool is fully ready (libafw only).
 * @param lock_id of lock.
 * @param p for pool used for the lock.
 * @param xctx of caller.
 *
 * Called from environment create.
 */
AFW_DECLARE(const afw_lock_t *)
afw_lock_create_environment_nested_lock(
    const afw_utf8_t *lock_id,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_END_DECLARES

/** @} */

#endif /* __AFW_LOCK_INTERNAL_H__ */
