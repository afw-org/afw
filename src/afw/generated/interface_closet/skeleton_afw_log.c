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
/* Change this to the name of the self typedef for this implementation */
#define AFW_LOG_SELF_T <afwdev {prefixed_interface_name}>_self_t
#include "afw_log_impl_declares.h"

          
/* Create an instance of the log. */
AFW_DEFINE(const afw_log_t *)
<afwdev {prefixed_interface_name}>_create_cede_p(
    const afw_object_t *properties,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    AFW_LOG_SELF_T *self;
    afw_log_t *log;
    
    /* Create log and process common properties. */
    log = afw_log_impl_create_cede_p(
            &impl_afw_log_inf,
            sizeof(AFW_LOG_SELF_T),
            properties, p, xctx);
    self = (AFW_LOG_SELF_T *)log;

    /** @todo Handle implementation specific configuration parameters, etc. */
       
    /* Return log. */
    return log;
}
      
    
/*
 * Implementation of method destroy for interface afw_log.
 */
void
impl_afw_log_destroy(
    AFW_LOG_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method set_own_mask for interface afw_log.
 */
void
impl_afw_log_set_own_mask(
    AFW_LOG_SELF_T *self,
    afw_log_priority_mask_t mask,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method write for interface afw_log.
 */
void
impl_afw_log_write(
    AFW_LOG_SELF_T *self,
    afw_log_priority_t priority,
    const afw_utf8_z_t * source_z,
    const afw_utf8_t * message,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}
