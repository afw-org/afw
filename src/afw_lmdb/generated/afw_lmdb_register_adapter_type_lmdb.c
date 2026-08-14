// See the 'COPYING' file in the project root for licensing information.
/*
 * Register adapter_type `lmdb` for afw_lmdb
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_lmdb_register_adapter_type_lmdb.c
 * @brief Generated environment register for adapter_type `lmdb` (afw_lmdb).
 */

#include "afw.h"

extern const afw_adapter_factory_t
afw_lmdb_adapter_factory;

/* Register lmdb adapter factory. */
void
afw_lmdb_adapter_type_lmdb_register(afw_xctx_t *xctx)
{
    afw_environment_register_adapter_type(
        &afw_lmdb_adapter_factory.adapter_type,
        &afw_lmdb_adapter_factory, xctx);
}
