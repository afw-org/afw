#include "afw.h"
#include "<afwdev {srcdir}>_generated.h"

/* Extern for inf. */
extern const afw_content_type_inf_t *
<afwdev {prefix}>content_type_<afwdev {registry_key}>_inf;

/*
 * The generate process looks for #define of the form:
 *
 * #define AFW_ADDITIONAL_REGISTRY_KEY_<n> <key>
 *
 * Where <n> is anything unique in this .c and <key> is the
 * additional key to register.
 *
 * For example the #define for application/xml is:
 *
 * #define AFW_ADDITIONAL_REGISTRY_KEY_1 application/xml
 *
 * Then add each of them to the impl_additional_mime_types[] in this way:
 *
 * AFW_UTF8_LITERAL(AFW_STRINGIFY(AFW_ADDITIONAL_REGISTRY_KEY_<n>))
 *
 * For example, to add AFW_ADDITIONAL_REGISTRY_KEY_1:
 *
 * static afw_utf8_t
 * impl_additional_mime_types[] = {
 *     AFW_UTF8_LITERAL(AFW_STRINGIFY(AFW_ADDITIONAL_REGISTRY_KEY_1))
 * };
 *
 * Then add this code to the self create in register function below:
 *
 * self->mime_type_count =
 *   sizeof(impl_additional_mime_types) / sizeof(afw_utf8_t);
 * self->mime_types = &impl_additional_mime_types[0];
 *
 */

void
<afwdev {prefix}>content_type_<afwdev {registry_key}>_register(afw_xctx_t *xctx)
{
    const afw_pool_t *p = xctx->env->p;
    afw_content_type_t *self;   

    /* Make self for content type. */
    self = afw_pool_calloc_type(p, afw_content_type_t, xctx);
    self->inf = <afwdev {prefix}>content_type_<afwdev {registry_key}>_inf;
    afw_memory_copy(&self->content_type_id, &self->inf->rti.implementation_id);

    /* Register content_type. */
    afw_content_type_register(self, xctx);
}
