<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_request for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_request */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
/* Change this to the name of the self typedef for this implementation */
#define AFW_REQUEST_SELF_T <afwdev {prefixed_interface_name}>_self_t
#include "afw_request_impl_declares.h"

/*
 * Implementation of method release for interface afw_request.
 */
void
impl_afw_request_release(
    AFW_REQUEST_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method set_error_info for interface afw_request.
 */
void
impl_afw_request_set_error_info(
    AFW_REQUEST_SELF_T *self,
    const afw_object_t * error_info,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method read_raw_request_body for interface afw_request.
 */
void
impl_afw_request_read_raw_request_body(
    AFW_REQUEST_SELF_T *self,
    afw_size_t buffer_size,
    void * buffer,
    afw_size_t * size,
    afw_boolean_t * more_to_read,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method set_response_status_code for interface afw_request.
 */
void
impl_afw_request_set_response_status_code(
    AFW_REQUEST_SELF_T *self,
    const afw_utf8_t * code,
    const afw_utf8_t * reason,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method write_response_header for interface afw_request.
 */
void
impl_afw_request_write_response_header(
    AFW_REQUEST_SELF_T *self,
    const afw_utf8_t * name,
    const afw_utf8_t * value,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method write_raw_response_body for interface afw_request.
 */
void
impl_afw_request_write_raw_response_body(
    AFW_REQUEST_SELF_T *self,
    afw_size_t size,
    const void * buffer,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method flush_response for interface afw_request.
 */
void
impl_afw_request_flush_response(
    AFW_REQUEST_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method finish_response for interface afw_request.
 */
void
impl_afw_request_finish_response(
    AFW_REQUEST_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}
