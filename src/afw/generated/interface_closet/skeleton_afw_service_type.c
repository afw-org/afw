<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_service_type for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_service_type */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
/* Change this to the name of the self typedef for this implementation */
#define AFW_SERVICE_TYPE_SELF_T <afwdev {prefixed_interface_name}>_self_t
#include "afw_service_type_impl_declares.h"

/*
 * Implementation of method related_instance_count for interface
 * afw_service_type.
 */
afw_integer_t
impl_afw_service_type_related_instance_count(
    AFW_SERVICE_TYPE_SELF_T *self,
    const afw_utf8_t * id,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method start_cede_p for interface afw_service_type.
 */
void
impl_afw_service_type_start_cede_p(
    AFW_SERVICE_TYPE_SELF_T *self,
    const afw_object_t * properties,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method stop for interface afw_service_type.
 */
void
impl_afw_service_type_stop(
    AFW_SERVICE_TYPE_SELF_T *self,
    const afw_utf8_t * id,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method restart_cede_p for interface afw_service_type.
 */
void
impl_afw_service_type_restart_cede_p(
    AFW_SERVICE_TYPE_SELF_T *self,
    const afw_object_t * properties,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}
