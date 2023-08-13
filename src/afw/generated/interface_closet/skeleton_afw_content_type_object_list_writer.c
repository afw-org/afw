<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_content_type_object_list_writer for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_content_type_object_list_writer */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
/* Change this to the name of the self typedef for this implementation */
#define AFW_CONTENT_TYPE_OBJECT_LIST_WRITER_SELF_T <afwdev {prefixed_interface_name}>_self_t
#include "afw_content_type_object_list_writer_impl_declares.h"

/*
 * Implementation of method release for interface
 * afw_content_type_object_list_writer.
 */
void
impl_afw_content_type_object_list_writer_release(
    AFW_CONTENT_TYPE_OBJECT_LIST_WRITER_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method write_object for interface
 * afw_content_type_object_list_writer.
 */
void
impl_afw_content_type_object_list_writer_write_object(
    AFW_CONTENT_TYPE_OBJECT_LIST_WRITER_SELF_T *self,
    const afw_object_t * object,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}
