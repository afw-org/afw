<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_writer for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_writer */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
/* Change this to the name of the self typedef for this implementation */
#define AFW_WRITER_SELF_T <afwdev {prefixed_interface_name}>_self_t
#include "afw_writer_impl_declares.h"

/*
 * Implementation of method release for interface afw_writer.
 */
void
impl_afw_writer_release(
    AFW_WRITER_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method flush for interface afw_writer.
 */
void
impl_afw_writer_flush(
    AFW_WRITER_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method write for interface afw_writer.
 */
void
impl_afw_writer_write(
    AFW_WRITER_SELF_T *self,
    const void * buffer,
    afw_size_t size,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method write_eol for interface afw_writer.
 */
void
impl_afw_writer_write_eol(
    AFW_WRITER_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method increment_indent for interface afw_writer.
 */
void
impl_afw_writer_increment_indent(
    AFW_WRITER_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method decrement_indent for interface afw_writer.
 */
void
impl_afw_writer_decrement_indent(
    AFW_WRITER_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}
