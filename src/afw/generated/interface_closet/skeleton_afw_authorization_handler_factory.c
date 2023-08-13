<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_authorization_handler_factory for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_authorization_handler_factory */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
/* Change this to the name of the self typedef for this implementation */
#define AFW_AUTHORIZATION_HANDLER_FACTORY_SELF_T <afwdev {prefixed_interface_name}>_self_t
#include "afw_authorization_handler_factory_impl_declares.h"

      
#include "<afwdev {prefixed_interface_name.replace('_factory','')}>.h"
    
static const afw_utf8_t
impl_<afwdev {prefixed_interface_name}>_description =
    AFW_UTF8_LITERAL("<afwdev {description}>.");

/* Singleton instance of afw_authorization_handler_factory for <afwdev {implementation_id}> */
<afwdev {srcdir_prefix.upper()}>DEFINE_CONST_DATA(afw_authorization_handler_factory_t)
<afwdev {prefixed_interface_name}> =
{
    &impl_afw_authorization_handler_factory_inf,
    AFW_UTF8_LITERAL("<afwdev {implementation_id}>"),
    &impl_<afwdev {prefixed_interface_name}>_description
};

/* Get singleton instance of afw_authorization_handler_factory for <afwdev {implementation_id}> */
<afwdev {srcdir_prefix.upper()}>DEFINE(const afw_authorization_handler_factory_t *)
<afwdev {prefix}>get_afw_authorization_handler_factory(
    const afw_object_t *parameters,
    afw_xctx_t *xctx)
{
    return &<afwdev {prefixed_interface_name}>;
}


/*
 * Implementation of method create_authorization_handler_cede_p for interface
 * afw_authorization_handler_factory.
 */
const afw_authorization_handler_t *
impl_afw_authorization_handler_factory_create_authorization_handler_cede_p(
    AFW_AUTHORIZATION_HANDLER_FACTORY_SELF_T *self,
    const afw_object_t * properties,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{

        
    /** @todo If needed, add/modify the following code to implement method. */

    /* Return adaptor instance. */
    return <afwdev {prefixed_interface_name.replace('_factory','')}>_create_cede_p(properties, p, xctx);
      
      
}
