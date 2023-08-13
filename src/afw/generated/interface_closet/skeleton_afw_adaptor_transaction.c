<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_adaptor_transaction for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_adaptor_transaction */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
/* Change this to the name of the self typedef for this implementation */
#define AFW_ADAPTOR_TRANSACTION_SELF_T <afwdev {prefixed_interface_name}>_self_t
#include "afw_adaptor_transaction_impl_declares.h"

/*
 * Implementation of method release for interface afw_adaptor_transaction.
 */
void
impl_afw_adaptor_transaction_release(
    AFW_ADAPTOR_TRANSACTION_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method commit for interface afw_adaptor_transaction.
 */
void
impl_afw_adaptor_transaction_commit(
    AFW_ADAPTOR_TRANSACTION_SELF_T *self,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}
