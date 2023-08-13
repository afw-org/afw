<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_extension for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_extension */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
/* Change this to the name of the self typedef for this implementation */
#define AFW_EXTENSION_SELF_T <afwdev {prefixed_interface_name}>_self_t
#include "afw_extension_impl_declares.h"

      
#include "generated/<afwdev {prefix}>generated.h"

#define AFW_IMPLEMENTATION_VERSION <afwdev {prefix.upper()}>VERSION_STRING
#include "generated/<afwdev {prefix}>version_info.h"

//* Define extension implementation. */
AFW_ENVIRONMENT_DEFINE_EXTENSION_IMPL();


    
/*
 * Implementation of method initialize for interface afw_extension.
 */
const afw_extension_t *
impl_afw_extension_initialize(
    AFW_EXTENSION_SELF_T *self,
    const afw_object_t * properties,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{

        
    /*
     *  IMPORTANT:
     *
     *  Make sure associated _AdaptiveManifest_ is up to date,
     *  especially the "registers" property.
     *
     */
    
    /* Call the generated register function for this extension. */
    <afwdev {prefix}>generated_register(xctx);

    /** @todo Add additional extension initialization here, if needed. */

    /*
     * Return the extension instance.
     *
     * Note: The instance returned can also a copy of the instance passed with
     * additional private instance data appended to its end. The instance
     * returned here is the instance registered in the AFW environment and the
     * instance passed to the other methods.
     */
    return &impl_extension;

      
}

/*
 * Implementation of method release for interface afw_extension.
 */
void
impl_afw_extension_release(
    AFW_EXTENSION_SELF_T *self,
    afw_xctx_t * xctx)
{

        
    /* Extension release() is not currently used. */

      
}
