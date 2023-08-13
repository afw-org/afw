<afwdev {license_c}>

/**
 * @file <afwdev {prefixed_interface_name}>.c
 * @brief <afwdev {brief}>
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_adaptor_journal for <afwdev {implementation_id}>.
 */

#include "afw.h"
#include "<afwdev {prefixed_interface_name}>.h"

/* Declares and rti/inf defines for interface afw_adaptor_journal */
#define AFW_IMPLEMENTATION_ID "<afwdev {implementation_id}>"
/* Change this to the name of the self typedef for this implementation */
#define AFW_ADAPTOR_JOURNAL_SELF_T <afwdev {prefixed_interface_name}>_self_t
#include "afw_adaptor_journal_impl_declares.h"

/*
 * Implementation of method add_entry for interface afw_adaptor_journal.
 */
const afw_utf8_t *
impl_afw_adaptor_journal_add_entry(
    AFW_ADAPTOR_JOURNAL_SELF_T *self,
    const afw_adaptor_impl_request_t * impl_request,
    const afw_object_t * entry,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method get_entry for interface afw_adaptor_journal.
 */
void
impl_afw_adaptor_journal_get_entry(
    AFW_ADAPTOR_JOURNAL_SELF_T *self,
    const afw_adaptor_impl_request_t * impl_request,
    afw_adaptor_journal_option_t option,
    const afw_utf8_t * consumer_id,
    const afw_utf8_t * entry_cursor,
    afw_size_t limit,
    const afw_object_t * response,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method mark_entry_consumed for interface
 * afw_adaptor_journal.
 */
void
impl_afw_adaptor_journal_mark_entry_consumed(
    AFW_ADAPTOR_JOURNAL_SELF_T *self,
    const afw_adaptor_impl_request_t * impl_request,
    const afw_utf8_t * consumer_id,
    const afw_utf8_t * entry_cursor,
    afw_xctx_t * xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}
