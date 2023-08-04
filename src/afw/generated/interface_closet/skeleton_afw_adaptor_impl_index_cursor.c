<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_adaptor_impl_index_cursor for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_adaptor_impl_index_cursor */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
#include "afw_adaptor_impl_index_cursor_impl_declares.h"

/*
 * Implementation of method release for interface
 * afw_adaptor_impl_index_cursor.
 */
void
impl_afw_adaptor_impl_index_cursor_release(
    const afw_adaptor_impl_index_cursor_t * instance,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_next_object for interface
 * afw_adaptor_impl_index_cursor.
 */
const afw_object_t *
impl_afw_adaptor_impl_index_cursor_get_next_object(
    const afw_adaptor_impl_index_cursor_t * instance,
    const afw_pool_t * pool,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method contains_object for interface
 * afw_adaptor_impl_index_cursor.
 */
afw_boolean_t
impl_afw_adaptor_impl_index_cursor_contains_object(
    const afw_adaptor_impl_index_cursor_t * instance,
    const afw_object_t * object,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method inner_join for interface
 * afw_adaptor_impl_index_cursor.
 */
const afw_adaptor_impl_index_cursor_t *
impl_afw_adaptor_impl_index_cursor_inner_join(
    const afw_adaptor_impl_index_cursor_t * instance,
    const afw_adaptor_impl_index_cursor_t * cursor,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_count for interface
 * afw_adaptor_impl_index_cursor.
 */
afw_boolean_t
impl_afw_adaptor_impl_index_cursor_get_count(
    const afw_adaptor_impl_index_cursor_t * instance,
    size_t * count,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}
