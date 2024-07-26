#include "afw.h"

extern const afw_adapter_factory_t
<afwdev {prefix}>adapter_factory_<afwdev {registry_key}>;

void
<afwdev {prefix}>adapter_type_<afwdev {registry_key}>_register(afw_xctx_t *xctx)
{
    /* Register adapter_type <afwdev {prefix}>. */
    afw_environment_register_adapter_type(
        &<afwdev {prefix}>adapter_factory_<afwdev {registry_key}>.adapter_type,
        &<afwdev {prefix}>adapter_factory_<afwdev {registry_key}>,
        xctx);
}
