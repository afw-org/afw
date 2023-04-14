#include "afw.h"

extern const afw_adaptor_factory_t
afw_lmdb_adaptor_factory;

/* Register lmdb adaptor factory. */
void
afw_lmdb_adaptor_type_lmdb_register(afw_xctx_t *xctx)
{
    afw_environment_register_adaptor_type(
        &afw_lmdb_adaptor_factory.adaptor_type,
        &afw_lmdb_adaptor_factory, xctx);
}
