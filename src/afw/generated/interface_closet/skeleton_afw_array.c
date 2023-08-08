<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_array for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_array */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
#include "afw_array_impl_declares.h"

/*
 * Implementation of method release for interface afw_array.
 */
void
impl_afw_array_release(
    const afw_array_t * instance,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_count for interface afw_array.
 */
afw_size_t
impl_afw_array_get_count(
    const afw_array_t * instance,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_data_type for interface afw_array.
 */
const afw_data_type_t *
impl_afw_array_get_data_type(
    const afw_array_t * instance,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_entry_meta for interface afw_array.
 */
const afw_value_t *
impl_afw_array_get_entry_meta(
    const afw_array_t * instance,
    afw_integer_t index,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_entry_internal for interface afw_array.
 */
afw_boolean_t
impl_afw_array_get_entry_internal(
    const afw_array_t * instance,
    afw_integer_t index,
    const afw_data_type_t * * data_type,
    const void * * internal,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_entry_value for interface afw_array.
 */
const afw_value_t *
impl_afw_array_get_entry_value(
    const afw_array_t * instance,
    afw_integer_t index,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_next_entry_meta for interface afw_array.
 */
const afw_value_t *
impl_afw_array_get_next_entry_meta(
    const afw_array_t * instance,
    const afw_iterator_t * * iterator,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_next_internal for interface afw_array.
 */
afw_boolean_t
impl_afw_array_get_next_internal(
    const afw_array_t * instance,
    const afw_iterator_t * * iterator,
    const afw_data_type_t * * data_type,
    const void * * internal,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_next_value for interface afw_array.
 */
const afw_value_t *
impl_afw_array_get_next_value(
    const afw_array_t * instance,
    const afw_iterator_t * * iterator,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_setter for interface afw_array.
 */
const afw_array_setter_t *
impl_afw_array_get_setter(
    const afw_array_t * instance,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}
