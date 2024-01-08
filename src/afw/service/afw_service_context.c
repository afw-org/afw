// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Service Context
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_service.c
 * @brief Adaptive Framework service context
 */

#include "afw_internal.h"



/* Prepare expression evaluation context for service::. */
AFW_DECLARE(void)
afw_service_context_prepare(
    const afw_service_t *service,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}
