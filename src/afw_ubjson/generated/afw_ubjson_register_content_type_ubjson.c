// See the 'COPYING' file in the project root for licensing information.
/*
 * Register content_type `ubjson` for afw_ubjson
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_ubjson_register_content_type_ubjson.c
 * @brief Generated environment register for content_type `ubjson` (afw_ubjson).
 */

#include "afw.h"

extern const afw_content_type_t
afw_ubjson_internal_content_type;

/* Register ubjson adapter factory. */
void
afw_ubjson_content_type_ubjson_register(afw_xctx_t *xctx)
{
    afw_content_type_register(&afw_ubjson_internal_content_type, xctx);
}
