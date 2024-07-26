<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_adapter_object_type_cache for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_adapter_object_type_cache */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
/* Change this to the name of the self typedef for this implementation */
#define AFW_ADAPTER_OBJECT_TYPE_CACHE_SELF_T <afwdev {prefixed_interface_name}>_self_t
#include "afw_adapter_object_type_cache_impl_declares.h"

/*
 * Implementation of method get for interface afw_adapter_object_type_cache.
 */
const afw_object_type_t *
impl_afw_adapter_object_type_cache_get(
    AFW_ADAPTER_OBJECT_TYPE_CACHE_SELF_T *self,
    const afw_utf8_t * object_type_id,
    afw_boolean_t * final_result,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method set for interface afw_adapter_object_type_cache.
 */
void
impl_afw_adapter_object_type_cache_set(
    AFW_ADAPTER_OBJECT_TYPE_CACHE_SELF_T *self,
    const afw_object_type_t * object_type,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}
