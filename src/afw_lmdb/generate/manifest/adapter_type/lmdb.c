/**
 * @file lmdb.c
 * @brief Manifest register snippet for adapter_type `lmdb` (afw_lmdb); copied into generated/register_*.c.
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
