<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_adapter_impl_index_cursor for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_adapter_impl_index_cursor */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
/* Change this to the name of the self typedef for this implementation */
#define AFW_ADAPTER_IMPL_INDEX_CURSOR_SELF_T <afwdev {prefixed_interface_name}>_self_t
#include "afw_adapter_impl_index_cursor_impl_declares.h"

/*
 * Implementation of method release for interface afw_adapter_impl_index_cursor.
 */
void
impl_afw_adapter_impl_index_cursor_release(
    AFW_ADAPTER_IMPL_INDEX_CURSOR_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method get_next_object for interface
 * afw_adapter_impl_index_cursor.
 */
const afw_object_t *
impl_afw_adapter_impl_index_cursor_get_next_object(
    AFW_ADAPTER_IMPL_INDEX_CURSOR_SELF_T *self,
    const afw_pool_t * pool,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method contains_object for interface
 * afw_adapter_impl_index_cursor.
 */
afw_boolean_t
impl_afw_adapter_impl_index_cursor_contains_object(
    AFW_ADAPTER_IMPL_INDEX_CURSOR_SELF_T *self,
    const afw_object_t * object,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method inner_join for interface
 * afw_adapter_impl_index_cursor.
 */
const afw_adapter_impl_index_cursor_t *
impl_afw_adapter_impl_index_cursor_inner_join(
    AFW_ADAPTER_IMPL_INDEX_CURSOR_SELF_T *self,
    const afw_adapter_impl_index_cursor_t * cursor,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method get_count for interface
 * afw_adapter_impl_index_cursor.
 */
afw_boolean_t
impl_afw_adapter_impl_index_cursor_get_count(
    AFW_ADAPTER_IMPL_INDEX_CURSOR_SELF_T *self,
    size_t * count,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}
