<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_iterator for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_iterator */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
/* Change this to the name of the self typedef for this implementation */
#define AFW_ITERATOR_SELF_T <afwdev {prefixed_interface_name}>_self_t
#include "afw_iterator_impl_declares.h"

/*
 * Implementation of method get_next for interface afw_iterator.
 */
const afw_value_t *
impl_afw_iterator_get_next(
    AFW_ITERATOR_SELF_T *self,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method get_by_index for interface afw_iterator.
 */
const afw_value_t *
impl_afw_iterator_get_by_index(
    AFW_ITERATOR_SELF_T *self,
    afw_integer_t index,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method get_count for interface afw_iterator.
 */
afw_size_t
impl_afw_iterator_get_count(
    AFW_ITERATOR_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}
