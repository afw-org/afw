<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_data_type for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_data_type */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
/* Change this to the name of the self typedef for this implementation */
#define AFW_DATA_TYPE_SELF_T <afwdev {prefixed_interface_name}>_self_t
#include "afw_data_type_impl_declares.h"

/*
 * Implementation of method internal_to_utf8 for interface afw_data_type.
 */
const afw_utf8_t *
impl_afw_data_type_internal_to_utf8(
    AFW_DATA_TYPE_SELF_T *self,
    const void * from_internal,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method utf8_to_internal for interface afw_data_type.
 */
void
impl_afw_data_type_utf8_to_internal(
    AFW_DATA_TYPE_SELF_T *self,
    void * to_internal,
    const afw_utf8_t * from_utf8,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method compare_internal for interface afw_data_type.
 */
int
impl_afw_data_type_compare_internal(
    AFW_DATA_TYPE_SELF_T *self,
    const void * internal1,
    const void * internal2,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method convert_internal for interface afw_data_type.
 */
void
impl_afw_data_type_convert_internal(
    AFW_DATA_TYPE_SELF_T *self,
    void * to_internal,
    const void * from_internal,
    const afw_data_type_t * to_data_type,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method clone_internal for interface afw_data_type.
 */
void
impl_afw_data_type_clone_internal(
    AFW_DATA_TYPE_SELF_T *self,
    void * to_internal,
    const void * from_internal,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method value_compiler_listing for interface afw_data_type.
 */
void
impl_afw_data_type_value_compiler_listing(
    AFW_DATA_TYPE_SELF_T *self,
    const afw_writer_t * writer,
    const afw_value_t * value,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method write_as_expression for interface afw_data_type.
 */
void
impl_afw_data_type_write_as_expression(
    AFW_DATA_TYPE_SELF_T *self,
    const afw_writer_t * writer,
    const void * from_internal,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}
