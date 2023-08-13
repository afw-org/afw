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
/* Change this to the name of the self typedef for this implementation */
#define AFW_OBJECT_ASSOCIATIVE_ARRAY_SELF_T <afwdev {prefixed_interface_name}>_self_t
#include "afw_object_associative_array_impl_declares.h"

/*
 * Implementation of method release for interface afw_object_associative_array.
 */
void
impl_afw_object_associative_array_release(
    AFW_OBJECT_ASSOCIATIVE_ARRAY_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method add_reference for interface
 * afw_object_associative_array.
 */
void
impl_afw_object_associative_array_add_reference(
    AFW_OBJECT_ASSOCIATIVE_ARRAY_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method get for interface afw_object_associative_array.
 */
const afw_object_t *
impl_afw_object_associative_array_get(
    AFW_OBJECT_ASSOCIATIVE_ARRAY_SELF_T *self,
    const afw_utf8_t * key,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method get_reference for interface
 * afw_object_associative_array.
 */
const afw_object_t *
impl_afw_object_associative_array_get_reference(
    AFW_OBJECT_ASSOCIATIVE_ARRAY_SELF_T *self,
    const afw_utf8_t * key,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method for_each for interface
 * afw_object_associative_array.
 */
void
impl_afw_object_associative_array_for_each(
    AFW_OBJECT_ASSOCIATIVE_ARRAY_SELF_T *self,
    void * context,
    afw_object_cb_t cb,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method set for interface afw_object_associative_array.
 */
void
impl_afw_object_associative_array_set(
    AFW_OBJECT_ASSOCIATIVE_ARRAY_SELF_T *self,
    const afw_utf8_t * key,
    const afw_object_t * object,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}
