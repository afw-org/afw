<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_adaptor_session for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_adaptor_session */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
/* Change this to the name of the self typedef for this implementation */
#define AFW_ADAPTOR_SESSION_SELF_T <afwdev {prefixed_interface_name}>_self_t
#include "afw_adaptor_session_impl_declares.h"

/* Create for <afwdev {implementation_id}> implementation of afw_adaptor_session */
<afwdev {srcdir_prefix.upper()}>DEFINE(const afw_adaptor_session_t *)
<afwdev {prefixed_interface_name}>_create_internal(
    const afw_object_t *parameters,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    <afwdev {prefixed_interface_name}>_self_t *self;

    /* Using a subpool of the p passed, allocate and initialize self. */
    p = afw_pool_create(p, xctx);
    self = afw_pool_calloc_type(p,AFW_ADAPTOR_SESSION_SELF_T, xctx);
    self->pub.inf = &impl_afw_adaptor_session_inf;
    self->pub.p = p;

    /** @todo Finish processing parameters and initializing new instance. */

    /* Return new instance. */
    return (afw_adaptor_session_t *)self;
}


/*
 * Implementation of method destroy for interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_destroy(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method retrieve_objects for interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_retrieve_objects(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    const afw_adaptor_impl_request_t * impl_request,
    const afw_utf8_t * object_type_id,
    const afw_query_criteria_t * criteria,
    void * context,
    afw_object_cb_t callback,
    const afw_object_t * adaptor_type_specific,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method get_object for interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_get_object(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    const afw_adaptor_impl_request_t * impl_request,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    void * context,
    afw_object_cb_t callback,
    const afw_object_t * adaptor_type_specific,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method add_object for interface afw_adaptor_session.
 */
const afw_utf8_t *
impl_afw_adaptor_session_add_object(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    const afw_adaptor_impl_request_t * impl_request,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * suggested_object_id,
    const afw_object_t * object,
    const afw_object_t * adaptor_type_specific,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method modify_object for interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_modify_object(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    const afw_adaptor_impl_request_t * impl_request,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    const afw_adaptor_modify_entry_t * const * entry,
    const afw_object_t * adaptor_type_specific,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method replace_object for interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_replace_object(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    const afw_adaptor_impl_request_t * impl_request,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    const afw_object_t * replacement_object,
    const afw_object_t * adaptor_type_specific,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method delete_object for interface afw_adaptor_session.
 */
void
impl_afw_adaptor_session_delete_object(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    const afw_adaptor_impl_request_t * impl_request,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    const afw_object_t * adaptor_type_specific,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method begin_transaction for interface
 * afw_adaptor_session.
 */
const afw_adaptor_transaction_t *
impl_afw_adaptor_session_begin_transaction(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method get_journal_interface for interface
 * afw_adaptor_session.
 */
const afw_adaptor_journal_t *
impl_afw_adaptor_session_get_journal_interface(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method get_key_value_interface for interface
 * afw_adaptor_session.
 */
const afw_adaptor_key_value_t *
impl_afw_adaptor_session_get_key_value_interface(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method get_index_interface for interface
 * afw_adaptor_session.
 */
const afw_adaptor_impl_index_t *
impl_afw_adaptor_session_get_index_interface(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method get_object_type_cache_interface for interface
 * afw_adaptor_session.
 */
const afw_adaptor_object_type_cache_t *
impl_afw_adaptor_session_get_object_type_cache_interface(
    AFW_ADAPTOR_SESSION_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}
