<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_adapter for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_adapter */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
/* Change this to the name of the self typedef for this implementation */
#define AFW_ADAPTER_SELF_T <afwdev {prefixed_interface_name}>_self_t
#include "afw_adapter_impl_declares.h"

          
/* Create an instance of the adapter. */
AFW_DEFINE(const afw_adapter_t *)
<afwdev {prefixed_interface_name}>_create_cede_p(
    const afw_object_t *properties,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    AFW_ADAPTER_SELF_T *self;
    afw_adapter_t *adapter;
    
    /* Create adapter and process common properties. */
    adapter = afw_adapter_impl_create_cede_p(
            &impl_afw_adapter_inf,
            sizeof(AFW_ADAPTER_SELF_T),
            properties, p, xctx);
    self = (AFW_ADAPTER_SELF_T *)adapter;

    /** @todo Handle implementation specific configuration parameters, etc. */
       
    /* Return adapter. */
    return adapter;
}
      
    
/*
 * Implementation of method destroy for interface afw_adapter.
 */
void
impl_afw_adapter_destroy(
    AFW_ADAPTER_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method create_adapter_session for interface afw_adapter.
 */
const afw_adapter_session_t *
impl_afw_adapter_create_adapter_session(
    AFW_ADAPTER_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method get_additional_metrics for interface afw_adapter.
 */
const afw_object_t *
impl_afw_adapter_get_additional_metrics(
    AFW_ADAPTER_SELF_T *self,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}
