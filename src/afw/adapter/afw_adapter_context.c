// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Adapter Context
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_adapter_context.c
 * @brief Adapter session/request context plumbing.
 */

#include "afw_internal.h"



/* Prepare expression evaluation context for adapter::. */
AFW_DECLARE(void)
afw_adapter_context_prepare(
    const afw_adapter_t *adapter,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}

