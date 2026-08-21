// See the 'COPYING' file in the project root for licensing information.
/*
 * Parked wrap-APR pool (not wired).
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_POOL_APR_H__
#define __AFW_POOL_APR_H__

#include "afw_interface.h"

/**
 * @file afw_pool_apr.h
 * @brief Parked wrap-APR pool create API.
 *
 * Production code uses afw_pool.h. These functions are compiled so the
 * wrap-APR implementation stays in the tree while we design scope vs
 * request pool management. Do not switch env/xctx onto this yet.
 */

AFW_BEGIN_DECLARES

AFW_DECLARE(const afw_pool_t *)
afw_pool_apr_create(
    const afw_pool_t *parent,
    afw_xctx_t *xctx);

AFW_DECLARE(afw_thread_t *)
afw_pool_apr_create_thread(
    afw_size_t size,
    afw_xctx_t *xctx);

AFW_DECLARE(void)
afw_pool_apr_free_memory(
    void *address,
    afw_xctx_t *xctx);

AFW_END_DECLARES

#endif /* __AFW_POOL_APR_H__ */
