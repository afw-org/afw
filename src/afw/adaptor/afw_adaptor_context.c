// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Adaptor Context
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_adaptor_context.c
 * @brief Adaptive framework adaptor context
 */

#include "afw_internal.h"



/* Prepare expression evaluation context for adaptor::. */
AFW_DECLARE(void)
afw_adaptor_context_prepare(
    const afw_adaptor_t *adaptor,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}

