// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Execution Context (xctx) Internal Header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_XCTX_INTERNAL_H__
#define __AFW_XCTX_INTERNAL_H__

#include "afw_interface.h"
#include "afw_environment_internal.h"

/**
 * @defgroup afw_xctx_internal Execution Context (xctx)
 * @ingroup afw_c_api_internal
 *
 * Adaptive Framework Execution Context (xctx) internal.
 *
 * @{
 */

/**
 * @file afw_xctx_internal.h
 * @brief Adaptive Framework The execution context (xctx) Internal Header.
 */

AFW_BEGIN_DECLARES

/* Self typedef for afw_xctx_qualifier implementation of afw_object. */
typedef struct
afw_xctx_qualifier_object_self_s {
    afw_object_t pub;
    afw_size_t reference_count;
    const afw_utf8_t *qualifier;
    afw_boolean_t for_testing;
} afw_xctx_qualifier_object_self_t;


AFW_STACK_STRUCT(afw_xctx_internal_qualifier_stack_s,
    afw_xctx_qualifier_stack_entry_t);

/**
 * @internal
 * @brief Internal function used during xctx/environment create.
 * @param unhandled_error
 * @param error
 * @param p to use.
 */
AFW_DECLARE(afw_xctx_t *)
afw_xctx_internal_create_initialize(
    afw_try_t *unhandled_error,
    afw_error_t *error,
    afw_environment_internal_t *env,
    const afw_pool_t *p);

/**
 * @internal
 * @brief Internal function used during xctx/environment create.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_xctx_internal_create_finishup(afw_xctx_t *xctx);


/**
 * @internal
 * @brief Internal function used by afw_thread_create().
 * @param thread.
 * @param xctx of caller.
 */
AFW_DECLARE(afw_xctx_t *)
afw_xctx_internal_create_thread_xctx(
    const afw_thread_t *thread, afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_XCTX_INTERNAL_H__ */
