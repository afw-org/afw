<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_object_associative_array for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_object_associative_array */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
#include "afw_object_associative_array_impl_declares.h"

/*
 * Implementation of method release for interface afw_object_associative_array.
 */
void
impl_afw_object_associative_array_release(
    const afw_object_associative_array_t * instance,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method add_reference for interface
 * afw_object_associative_array.
 */
void
impl_afw_object_associative_array_add_reference(
    const afw_object_associative_array_t * instance,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get for interface afw_object_associative_array.
 */
const afw_object_t *
impl_afw_object_associative_array_get(
    const afw_object_associative_array_t * instance,
    const afw_utf8_t * key,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_reference for interface
 * afw_object_associative_array.
 */
const afw_object_t *
impl_afw_object_associative_array_get_reference(
    const afw_object_associative_array_t * instance,
    const afw_utf8_t * key,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method for_each for interface
 * afw_object_associative_array.
 */
void
impl_afw_object_associative_array_for_each(
    const afw_object_associative_array_t * instance,
    void * context,
    afw_object_cb_t cb,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method set for interface afw_object_associative_array.
 */
void
impl_afw_object_associative_array_set(
    const afw_object_associative_array_t * instance,
    const afw_utf8_t * key,
    const afw_object_t * object,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}
