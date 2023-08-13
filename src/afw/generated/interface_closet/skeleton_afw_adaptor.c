<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_adaptor for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_adaptor */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
/* Change this to the name of the self typedef for this implementation */
#define AFW_ADAPTOR_SELF_T <afwdev {prefixed_interface_name}>_self_t
#include "afw_adaptor_impl_declares.h"

          
/* Create an instance of the adaptor. */
AFW_DEFINE(const afw_adaptor_t *)
<afwdev {prefixed_interface_name}>_create_cede_p(
    const afw_object_t *properties,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    AFW_ADAPTOR_SELF_T *self;
    afw_adaptor_t *adaptor;
    
    /* Create adaptor and process common properties. */
    adaptor = afw_adaptor_impl_create_cede_p(
            &impl_afw_adaptor_inf,
            sizeof(AFW_ADAPTOR_SELF_T),
            properties, p, xctx);
    self = (AFW_ADAPTOR_SELF_T *)adaptor;

    /** @todo Handle implementation specific configuration parameters, etc. */
       
    /* Return adaptor. */
    return adaptor;
}
      
    
/*
 * Implementation of method destroy for interface afw_adaptor.
 */
void
impl_afw_adaptor_destroy(
    AFW_ADAPTOR_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method create_adaptor_session for interface afw_adaptor.
 */
const afw_adaptor_session_t *
impl_afw_adaptor_create_adaptor_session(
    AFW_ADAPTOR_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method get_additional_metrics for interface afw_adaptor.
 */
const afw_object_t *
impl_afw_adaptor_get_additional_metrics(
    AFW_ADAPTOR_SELF_T *self,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}
