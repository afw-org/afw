/**
 * @file ubjson.c
 * @brief Manifest register snippet for content_type `ubjson` (afw_ubjson); copied into generated/register_*.c.
 */

#include "afw.h"

extern const afw_content_type_t
afw_ubjson_internal_content_type;

/* Register ubjson adapter factory. */
void
afw_ubjson_content_type_ubjson_register(afw_xctx_t *xctx)
{
    afw_content_type_register(&afw_ubjson_internal_content_type, xctx);
}
