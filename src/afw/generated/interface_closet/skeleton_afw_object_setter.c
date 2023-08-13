<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_object_setter for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_object_setter */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
/* Change this to the name of the self typedef for this implementation */
#define AFW_OBJECT_SETTER_SELF_T <afwdev {prefixed_interface_name}>_self_t
#include "afw_object_setter_impl_declares.h"

/*
 * Implementation of method set_immutable for interface afw_object_setter.
 */
void
impl_afw_object_setter_set_immutable(
    AFW_OBJECT_SETTER_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method set_property for interface afw_object_setter.
 */
void
impl_afw_object_setter_set_property(
    AFW_OBJECT_SETTER_SELF_T *self,
    const afw_utf8_t * property_name,
    const afw_value_t * value,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}
