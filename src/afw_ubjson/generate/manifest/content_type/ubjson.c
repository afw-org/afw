#include "afw.h"

extern const afw_content_type_t
afw_ubjson_internal_content_type;

/* Register ubjson adaptor factory. */
void
afw_ubjson_content_type_ubjson_register(afw_xctx_t *xctx)
{
    afw_content_type_register(&afw_ubjson_internal_content_type, xctx);
}
