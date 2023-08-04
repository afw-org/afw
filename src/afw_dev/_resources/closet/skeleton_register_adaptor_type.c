#include "afw.h"

extern const afw_adaptor_factory_t
<afwdev {prefix}>adaptor_factory_<afwdev {registry_key}>;

void
<afwdev {prefix}>adaptor_type_<afwdev {registry_key}>_register(afw_xctx_t *xctx)
{
    /* Register adaptor_type <afwdev {prefix}>. */
    afw_environment_register_adaptor_type(
        &<afwdev {prefix}>adaptor_factory_<afwdev {registry_key}>.adaptor_type,
        &<afwdev {prefix}>adaptor_factory_<afwdev {registry_key}>,
        xctx);
}
