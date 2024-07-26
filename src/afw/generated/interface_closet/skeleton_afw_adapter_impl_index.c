<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_adapter_impl_index for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_adapter_impl_index */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
/* Change this to the name of the self typedef for this implementation */
#define AFW_ADAPTER_IMPL_INDEX_SELF_T <afwdev {prefixed_interface_name}>_self_t
#include "afw_adapter_impl_index_impl_declares.h"

/*
 * Implementation of method open for interface afw_adapter_impl_index.
 */
void
impl_afw_adapter_impl_index_open(
    AFW_ADAPTER_IMPL_INDEX_SELF_T *self,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * key,
    afw_boolean_t integer,
    afw_boolean_t unique,
    afw_boolean_t reverse,
    const afw_pool_t * pool,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method release for interface afw_adapter_impl_index.
 */
void
impl_afw_adapter_impl_index_release(
    AFW_ADAPTER_IMPL_INDEX_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method get_index_definitions for interface
 * afw_adapter_impl_index.
 */
const afw_object_t *
impl_afw_adapter_impl_index_get_index_definitions(
    AFW_ADAPTER_IMPL_INDEX_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method update_index_definitions for interface
 * afw_adapter_impl_index.
 */
void
impl_afw_adapter_impl_index_update_index_definitions(
    AFW_ADAPTER_IMPL_INDEX_SELF_T *self,
    const afw_object_t * indexDefinitions,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method add for interface afw_adapter_impl_index.
 */
afw_rc_t
impl_afw_adapter_impl_index_add(
    AFW_ADAPTER_IMPL_INDEX_SELF_T *self,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    const afw_utf8_t * key,
    const afw_utf8_t* value,
    afw_boolean_t unique,
    const afw_pool_t * pool,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method delete for interface afw_adapter_impl_index.
 */
afw_rc_t
impl_afw_adapter_impl_index_delete(
    AFW_ADAPTER_IMPL_INDEX_SELF_T *self,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    const afw_utf8_t * key,
    const afw_utf8_t* value,
    const afw_pool_t * pool,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method replace for interface afw_adapter_impl_index.
 */
afw_rc_t
impl_afw_adapter_impl_index_replace(
    AFW_ADAPTER_IMPL_INDEX_SELF_T *self,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    const afw_utf8_t * key,
    const afw_utf8_t* old_value,
    const afw_utf8_t* new_value,
    const afw_pool_t * pool,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method drop for interface afw_adapter_impl_index.
 */
afw_rc_t
impl_afw_adapter_impl_index_drop(
    AFW_ADAPTER_IMPL_INDEX_SELF_T *self,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * key,
    const afw_pool_t * pool,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method open_cursor for interface afw_adapter_impl_index.
 */
afw_adapter_impl_index_cursor_t *
impl_afw_adapter_impl_index_open_cursor(
    AFW_ADAPTER_IMPL_INDEX_SELF_T *self,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * index_key,
    int operator,
    const afw_utf8_t * value,
    afw_boolean_t unique,
    const afw_pool_t * pool,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method get_session for interface afw_adapter_impl_index.
 */
const afw_adapter_session_t *
impl_afw_adapter_impl_index_get_session(
    AFW_ADAPTER_IMPL_INDEX_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}
