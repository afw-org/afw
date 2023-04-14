#include "afw.h"

extern const afw_log_factory_t
<afwdev {prefix}>log_factory_<afwdev {registry_key}>;

void
<afwdev {prefix}>log_type_<afwdev {registry_key}>_register(afw_xctx_t *xctx)
{
    /* Register log_type <afwdev {prefix}>. */
    afw_environment_register_log_type(
        &<afwdev {prefix}>log_factory_<afwdev {registry_key}>.log_type,
        &<afwdev {prefix}>log_factory_<afwdev {registry_key}>,
        xctx);
}
