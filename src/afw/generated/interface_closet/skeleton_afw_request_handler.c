<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_request_handler for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_request_handler */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
#include "afw_request_handler_impl_declares.h"

          
/* Create an instance of the request_handler. */
AFW_DEFINE(const afw_request_handler_t *)
<afwdev {prefixed_interface_name}>_create_cede_p(
    const afw_object_t *properties,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    <afwdev {prefixed_interface_name}>_self_t *self;
    afw_request_handler_t *request_handler;
    
    /* Create request_handler and process common properties. */
    request_handler = afw_request_handler_impl_create_cede_p(
            &impl_afw_request_handler_inf,
            sizeof(<afwdev {prefixed_interface_name}>_self_t),
            properties, p, xctx);
    self = (<afwdev {prefixed_interface_name}>_self_t *)request_handler;

    /** @todo Handle implementation specific configuration parameters, etc. */
       
    /* Return request_handler. */
    return request_handler;
}
      
    
/*
 * Implementation of method release for interface afw_request_handler.
 */
void
impl_afw_request_handler_release(
    const afw_request_handler_t * instance,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method process for interface afw_request_handler.
 */
void
impl_afw_request_handler_process(
    const afw_request_handler_t * instance,
    const afw_request_t * request,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}
