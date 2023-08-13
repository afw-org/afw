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
/* Change this to the name of the self typedef for this implementation */
#define AFW_REQUEST_HANDLER_SELF_T <afwdev {prefixed_interface_name}>_self_t
#include "afw_request_handler_impl_declares.h"

          
/* Create an instance of the request_handler. */
AFW_DEFINE(const afw_request_handler_t *)
<afwdev {prefixed_interface_name}>_create_cede_p(
    const afw_object_t *properties,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    AFW_REQUEST_HANDLER_SELF_T *self;
    afw_request_handler_t *request_handler;
    
    /* Create request_handler and process common properties. */
    request_handler = afw_request_handler_impl_create_cede_p(
            &impl_afw_request_handler_inf,
            sizeof(AFW_REQUEST_HANDLER_SELF_T),
            properties, p, xctx);
    self = (AFW_REQUEST_HANDLER_SELF_T *)request_handler;

    /** @todo Handle implementation specific configuration parameters, etc. */
       
    /* Return request_handler. */
    return request_handler;
}
      
    
/*
 * Implementation of method release for interface afw_request_handler.
 */
void
impl_afw_request_handler_release(
    AFW_REQUEST_HANDLER_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method process for interface afw_request_handler.
 */
void
impl_afw_request_handler_process(
    AFW_REQUEST_HANDLER_SELF_T *self,
    const afw_request_t * request,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}
