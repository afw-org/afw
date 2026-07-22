// See the 'COPYING' file in the project root for licensing information.
/*
 * Register adapter_type `vfs` for afw_vfs
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_vfs_register_adapter_type_vfs.c
 * @brief Generated environment register for adapter_type `vfs` (afw_vfs).
 */

#include "afw.h"

extern const afw_adapter_factory_t
afw_vfs_adapter_factory_vfs;

void
afw_vfs_adapter_type_vfs_register(afw_xctx_t *xctx)
{
    /* Register adapter_type afw_vfs_. */
    afw_environment_register_adapter_type(
        &afw_vfs_adapter_factory_vfs.adapter_type,
        &afw_vfs_adapter_factory_vfs,
        xctx);
}
