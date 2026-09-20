// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework thread internal header.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_THREAD_INTERNAL_H__
#define __AFW_THREAD_INTERNAL_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_thread_internal
 * @{
 */

/**
 * @file afw_thread_internal.h
 * @brief Internal thread create for environment bootstrap.
 */

AFW_BEGIN_DECLARES

/**
 * @internal
 * @brief Create the process base thread before the base pool.
 * @return new thread, or NULL on allocation failure.
 *
 * No pool yet: C calloc for the struct, C calloc for
 * memory_region. Does not throw. Caller passes the result to
 * afw_pool_internal_create_base_pool(). On failure after this
 * returns, call afw_thread_internal_release_base_thread().
 */
afw_thread_t *
afw_thread_internal_create_base_thread(void);

/**
 * @internal
 * @brief Release a malloc'd base thread (region then free).
 * @param thread from create_base_thread. May be NULL.
 * @param xctx of caller. May be NULL.
 */
void
afw_thread_internal_release_base_thread(
    afw_thread_t *thread,
    afw_xctx_t *xctx);

AFW_END_DECLARES

/** @} */

#endif /* __AFW_THREAD_INTERNAL_H__ */
