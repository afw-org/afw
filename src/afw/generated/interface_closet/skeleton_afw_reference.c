<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_reference for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_reference */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
/* Change this to the name of the self typedef for this implementation */
#define AFW_REFERENCE_SELF_T <afwdev {prefixed_interface_name}>_self_t
#include "afw_reference_impl_declares.h"

/*
 * Implementation of method get_reference for interface afw_reference.
 */
const afw_reference_t *
impl_afw_reference_get_reference(
    AFW_REFERENCE_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method release for interface afw_reference.
 */
void
impl_afw_reference_release(
    AFW_REFERENCE_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method get_reference_count for interface afw_reference.
 */
afw_size_t
impl_afw_reference_get_reference_count(
    AFW_REFERENCE_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method for_each_reference for interface afw_reference.
 */
void
impl_afw_reference_for_each_reference(
    AFW_REFERENCE_SELF_T *self,
    afw_reference_cb_t callback,
    void * context,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}
