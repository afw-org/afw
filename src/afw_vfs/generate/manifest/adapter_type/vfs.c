/**
 * @file vfs.c
 * @brief Manifest register snippet for adapter_type `vfs` (afw_vfs); copied into generated/register_*.c.
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
