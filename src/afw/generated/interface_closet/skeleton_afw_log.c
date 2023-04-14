<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_log for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_log */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
#include "afw_log_impl_declares.h"

          
/* Create an instance of the log. */
AFW_DEFINE(const afw_log_t *)
<afwdev {prefixed_interface_name}>_create_cede_p(
    const afw_object_t *properties,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    <afwdev {prefixed_interface_name}>_self_t *self;
    afw_log_t *log;
    
    /* Create log and process common properties. */
    log = afw_log_impl_create_cede_p(
            &impl_afw_log_inf,
            sizeof(<afwdev {prefixed_interface_name}>_self_t),
            properties, p, xctx);
    self = (<afwdev {prefixed_interface_name}>_self_t *)log;

    /** @todo Handle implementation specific configuration parameters, etc. */
       
    /* Return log. */
    return log;
}
      
    
/*
 * Implementation of method destroy for interface afw_log.
 */
void
impl_afw_log_destroy(
    const afw_log_t * instance,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method set_own_mask for interface afw_log.
 */
void
impl_afw_log_set_own_mask(
    const afw_log_t * instance,
    afw_log_priority_mask_t mask,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method write for interface afw_log.
 */
void
impl_afw_log_write(
    const afw_log_t * instance,
    afw_log_priority_t priority,
    const afw_utf8_z_t * source_z,
    const afw_utf8_t * message,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}
