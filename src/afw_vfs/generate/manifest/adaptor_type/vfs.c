#include "afw.h"

extern const afw_adaptor_factory_t
afw_vfs_adaptor_factory_vfs;

void
afw_vfs_adaptor_type_vfs_register(afw_xctx_t *xctx)
{
    /* Register adaptor_type afw_vfs_. */
    afw_environment_register_adaptor_type(
        &afw_vfs_adaptor_factory_vfs.adaptor_type,
        &afw_vfs_adaptor_factory_vfs,
        xctx);
}
