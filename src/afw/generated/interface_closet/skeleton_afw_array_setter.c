<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_array_setter for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_array_setter */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
/* Change this to the name of the self typedef for this implementation */
#define AFW_ARRAY_SETTER_SELF_T <afwdev {prefixed_interface_name}>_self_t
#include "afw_array_setter_impl_declares.h"

/*
 * Implementation of method set_immutable for interface afw_array_setter.
 */
void
impl_afw_array_setter_set_immutable(
    AFW_ARRAY_SETTER_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method determine_data_type_and_set_immutable for interface
 * afw_array_setter.
 */
const afw_data_type_t *
impl_afw_array_setter_determine_data_type_and_set_immutable(
    AFW_ARRAY_SETTER_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method add_internal for interface afw_array_setter.
 */
void
impl_afw_array_setter_add_internal(
    AFW_ARRAY_SETTER_SELF_T *self,
    const afw_data_type_t * data_type,
    const void * internal,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method add_value for interface afw_array_setter.
 */
void
impl_afw_array_setter_add_value(
    AFW_ARRAY_SETTER_SELF_T *self,
    const afw_value_t * value,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method insert_internal for interface afw_array_setter.
 */
void
impl_afw_array_setter_insert_internal(
    AFW_ARRAY_SETTER_SELF_T *self,
    const afw_data_type_t * data_type,
    const void * internal,
    afw_size_t index,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method insert_value for interface afw_array_setter.
 */
void
impl_afw_array_setter_insert_value(
    AFW_ARRAY_SETTER_SELF_T *self,
    const afw_value_t * value,
    afw_size_t index,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method remove_all_values for interface afw_array_setter.
 */
void
impl_afw_array_setter_remove_all_values(
    AFW_ARRAY_SETTER_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method remove_internal for interface afw_array_setter.
 */
void
impl_afw_array_setter_remove_internal(
    AFW_ARRAY_SETTER_SELF_T *self,
    const afw_data_type_t * data_type,
    const void * internal,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method remove_value for interface afw_array_setter.
 */
void
impl_afw_array_setter_remove_value(
    AFW_ARRAY_SETTER_SELF_T *self,
    const afw_value_t * value,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method set_value_by_index for interface afw_array_setter.
 */
void
impl_afw_array_setter_set_value_by_index(
    AFW_ARRAY_SETTER_SELF_T *self,
    afw_size_t index,
    const afw_value_t * value,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}
