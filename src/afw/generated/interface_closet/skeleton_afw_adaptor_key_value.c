<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_adaptor_key_value for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_adaptor_key_value */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
/* Change this to the name of the self typedef for this implementation */
#define AFW_ADAPTOR_KEY_VALUE_SELF_T <afwdev {prefixed_interface_name}>_self_t
#include "afw_adaptor_key_value_impl_declares.h"

/*
 * Implementation of method add for interface afw_adaptor_key_value.
 */
void
impl_afw_adaptor_key_value_add(
    AFW_ADAPTOR_KEY_VALUE_SELF_T *self,
    const afw_utf8_t * namespace,
    const afw_memory_t * key,
    const afw_memory_t * value,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method delete for interface afw_adaptor_key_value.
 */
void
impl_afw_adaptor_key_value_delete(
    AFW_ADAPTOR_KEY_VALUE_SELF_T *self,
    const afw_utf8_t * namespace,
    const afw_memory_t * key,
    const afw_memory_t * value,
    afw_boolean_t must_exist,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method replace for interface afw_adaptor_key_value.
 */
void
impl_afw_adaptor_key_value_replace(
    AFW_ADAPTOR_KEY_VALUE_SELF_T *self,
    const afw_utf8_t * namespace,
    const afw_memory_t * key,
    const afw_memory_t * value,
    afw_boolean_t must_exist,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method get for interface afw_adaptor_key_value.
 */
const afw_memory_t *
impl_afw_adaptor_key_value_get(
    AFW_ADAPTOR_KEY_VALUE_SELF_T *self,
    const afw_utf8_t * namespace,
    const afw_memory_t * key,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}
