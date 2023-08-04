<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_adaptor_object_type_cache for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_adaptor_object_type_cache */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
#include "afw_adaptor_object_type_cache_impl_declares.h"

/*
 * Implementation of method get for interface afw_adaptor_object_type_cache.
 */
const afw_object_type_t *
impl_afw_adaptor_object_type_cache_get(
    const afw_adaptor_object_type_cache_t * instance,
    const afw_utf8_t * object_type_id,
    afw_boolean_t * final_result,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method set for interface afw_adaptor_object_type_cache.
 */
void
impl_afw_adaptor_object_type_cache_set(
    const afw_adaptor_object_type_cache_t * instance,
    const afw_object_type_t * object_type,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}
