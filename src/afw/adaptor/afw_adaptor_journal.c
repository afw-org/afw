// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Adaptor Journal
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_adaptor_journal.c
 * @brief Adaptive Framework adaptor journal.
 */

#include "afw_internal.h"



void
afw_adaptor_internal_journal_prologue(
    const afw_adaptor_session_t *session,
    const afw_object_t *journal_entry,
    afw_xctx_t *xctx)
{
    const afw_value_t *now;

    now = afw_value_create_dateTime_now_utc(journal_entry->p, xctx);
    afw_object_set_property(journal_entry, &afw_s_beginTime, now, xctx);
}


void
afw_adaptor_internal_journal_epilogue(
    const afw_adaptor_session_t *session,
    const afw_object_t *journal_entry,
    afw_boolean_t modification,
    afw_xctx_t *xctx)
{
    const afw_value_t *now;
    const afw_adaptor_session_t *journal_session;
    const afw_adaptor_journal_t *journal;
    afw_adaptor_impl_request_t impl_request;

    now = afw_value_create_dateTime_now_utc(journal_entry->p, xctx);
    afw_object_set_property(journal_entry, &afw_s_endTime, now, xctx);

    /** @fixme Might want to record failures too???? */
    /* If this is a modification, add event to journal if requested. */
    if (modification && session->adaptor->impl->journal_adaptor_id) {
        journal_session = afw_adaptor_session_get_cached(
            session->adaptor->impl->journal_adaptor_id, true, xctx);
        journal = afw_adaptor_session_get_journal_interface(journal_session,
            xctx);
        afw_memory_clear(&impl_request);
        afw_adaptor_journal_add_entry(journal, &impl_request,
            journal_entry, xctx);
    }
}



/* _AdaptiveJournalEntry update_object(). */
AFW_DEFINE(void)
afw_adaptor_journal_entry_consume(
    const afw_adaptor_session_t *session,
    const afw_utf8_t *object_id,
    const afw_object_t *update_object,
    afw_xctx_t *xctx)
{
    const afw_adaptor_journal_t *journal;
    afw_boolean_t consumed;
    afw_boolean_t found;
    const afw_utf8_t *consumer_id;
    afw_adaptor_impl_request_t impl_request;

    /* Get journal interface. */
    journal = afw_adaptor_session_get_journal_interface(session, xctx);
    if (!journal) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "adaptor_id " AFW_UTF8_FMT
            " session get_journal() returned NULL",
            AFW_UTF8_FMT_ARG(&session->adaptor->adaptor_id));
    }

    /* Get consumed property from update object. */
    consumed = afw_object_old_get_property_as_boolean(update_object,
        &afw_s_consumed, &found, xctx);
    if (!found || !consumed) {
        AFW_THROW_ERROR_Z(general,
            AFW_OBJECT_Q_OBJECT_TYPE_ID_JOURNAL_ENTRY
            " update_object() must have consumed property set to true", xctx);
    }

    /* Get consumer_id from update object. */
    consumer_id = afw_object_old_get_property_as_string(update_object,
        &afw_s_consumerId, xctx);
    if (!consumer_id) {
        AFW_THROW_ERROR_Z(general,
            AFW_OBJECT_Q_OBJECT_TYPE_ID_JOURNAL_ENTRY
            " update_object() must have consumerId property", xctx);
    }

    /* Mark entry consumed. */
    afw_memory_clear(&impl_request);
    afw_adaptor_journal_mark_entry_consumed(journal, &impl_request,
        consumer_id, object_id, xctx);
}


static const afw_adaptor_journal_t *
impl_get_journal_interface(const afw_utf8_t *adaptor_id,
    afw_boolean_t begin_transaction, afw_xctx_t *xctx)
{
    const afw_adaptor_session_t *session;
    const afw_adaptor_journal_t *journal;

    /* Get an active session. */
    session = afw_adaptor_session_get_cached(adaptor_id, begin_transaction, xctx);

    journal = afw_adaptor_session_get_journal_interface(session, xctx);
    if (!journal) goto error;
    return journal;

error:
    AFW_THROW_ERROR_FZ(general, xctx,
        "Adaptor " AFW_UTF8_FMT " does not support journal",
        AFW_UTF8_FMT_ARG(adaptor_id));
}


/* Journal - get first entry. */
AFW_DEFINE(const afw_object_t *)
afw_adaptor_journal_get_first(
    const afw_utf8_t *adaptor_id,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_adaptor_journal_t *journal;
    const afw_object_t *result;
    afw_adaptor_impl_request_t impl_request;

    /* Create memory object for result. */
    result = afw_object_create_managed(p, xctx);

    /* Get journal interface. */
    journal = impl_get_journal_interface(adaptor_id, false, xctx);

    /** @fixme Add authorization check before and after call. */

    /* Get first entry. */
    afw_memory_clear(&impl_request);
    afw_adaptor_journal_get_entry(journal, &impl_request,
        afw_adaptor_journal_option_get_first, NULL, NULL, -1, result, xctx);

    /* Return result object. */
    return result;
}


/* Journal - get entry at cursor. */
AFW_DEFINE(const afw_object_t *)
afw_adaptor_journal_get_by_cursor(
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *cursor,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_adaptor_journal_t *journal;
    const afw_object_t *result;
    afw_adaptor_impl_request_t impl_request;

    /* Create memory object for result. */
    result = afw_object_create_managed(p, xctx);

    /* Get journal interface. */
    journal = impl_get_journal_interface(adaptor_id, false, xctx);


    /** @fixme Add authorization check before and after call. */

    /* Get first entry. */
    afw_memory_clear(&impl_request);
    afw_adaptor_journal_get_entry(journal, &impl_request,
        afw_adaptor_journal_option_get_by_cursor, NULL, cursor, -1, result, xctx);

    /* Return result object. */
    return result;
}


/* Journal - get next entry after cursor. */
AFW_DEFINE(const afw_object_t *)
afw_adaptor_journal_get_next_after_cursor(
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *cursor,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_adaptor_journal_t *journal;
    const afw_object_t *result;
    afw_adaptor_impl_request_t impl_request;

    /* Create memory object for result. */
    result = afw_object_create_managed(p, xctx);

    /* Get journal interface. */
    journal = impl_get_journal_interface(adaptor_id, false, xctx);


    /** @fixme Add authorization check before and after call. */

    /* Get first entry. */
    afw_memory_clear(&impl_request);
    afw_adaptor_journal_get_entry(journal, &impl_request,
        afw_adaptor_journal_option_get_next_after_cursor, NULL, cursor, -1, result,
        xctx);

    /* Return result object. */
    return result;
}


/* Journal - get next entry for consumer. */
AFW_DEFINE(const afw_object_t *)
afw_adaptor_journal_get_next_for_consumer(
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *consumer_id,
    afw_size_t limit,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_adaptor_journal_t *journal;
    const afw_object_t *result;
    afw_adaptor_impl_request_t impl_request;

    /* Create memory object for result. */
    result = afw_object_create_managed(p, xctx);

    /* Get journal interface. */
    journal = impl_get_journal_interface(adaptor_id, true, xctx);


    /** @fixme Add authorization check before and after call. */

    /* Get first entry. */
    afw_memory_clear(&impl_request);
    afw_adaptor_journal_get_entry(journal, &impl_request,
        afw_adaptor_journal_option_get_next_for_consumer, consumer_id, NULL,
        limit, result, xctx);

    /* Return result object. */
    return result;
}


/* Journal - get next entry after cursor for consumer. */
AFW_DEFINE(const afw_object_t *)
afw_adaptor_journal_get_next_for_consumer_after_cursor(
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *consumer_id,
    const afw_utf8_t *cursor,
    afw_size_t limit,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_adaptor_journal_t *journal;
    const afw_object_t *result;
    afw_adaptor_impl_request_t impl_request;

    /* Create memory object for result. */
    result = afw_object_create_managed(p, xctx);

    /* Get journal interface. */
    journal = impl_get_journal_interface(adaptor_id, true, xctx);


    /** @fixme Add authorization check before and after call. */

    /* Get first entry. */
    afw_memory_clear(&impl_request);
    afw_adaptor_journal_get_entry(journal, &impl_request,
        afw_adaptor_journal_option_get_next_for_consumer_after_cursor,
        consumer_id, cursor, limit, result, xctx);

    /* Return result object. */
    return result;
}


/* Journal - advance cursor for consumer. */
AFW_DEFINE(const afw_object_t *)
afw_adaptor_journal_advance_cursor_for_consumer(
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *consumer_id,
    afw_size_t limit,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_adaptor_journal_t *journal;
    const afw_object_t *result;
    afw_adaptor_impl_request_t impl_request;

    /* Create memory object for result. */
    result = afw_object_create_managed(p, xctx);

    /* Get journal interface. */
    journal = impl_get_journal_interface(adaptor_id, true, xctx);


    /** @fixme Add authorization check before and after call. */

    /* Get first entry. */
    afw_memory_clear(&impl_request);
    afw_adaptor_journal_get_entry(journal, &impl_request,
        afw_adaptor_journal_option_advance_cursor_for_consumer,
        consumer_id, NULL, limit, result, xctx);

    /* Return result object. */
    return result;
}

/* Journal - mark entry consumed by consumer. */
AFW_DEFINE(void)
afw_adaptor_journal_mark_consumed(
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *consumer_id,
    const afw_utf8_t *cursor,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_adaptor_journal_t *journal;
    afw_adaptor_impl_request_t impl_request;

    /* Get journal interface. */
    journal = impl_get_journal_interface(adaptor_id, true, xctx);

    /** @fixme Add authorization check before and after call. */

    /* Get first entry. */
    afw_memory_clear(&impl_request);
    afw_adaptor_journal_mark_entry_consumed(journal, &impl_request,
        consumer_id, cursor, xctx);
}


const afw_object_t *
afw_adaptor_internal_journal_get_entry(
    const afw_adaptor_session_t *session,
    const afw_utf8_t *object_id,
    const afw_object_t *journal_entry,
    afw_xctx_t *xctx)
{
    afw_adaptor_journal_option_t option;
    const afw_utf8_t *consumer_id;
    const afw_utf8_t *entry_cursor;
    afw_size_t limit;
    const afw_utf8_octet_t *s;
    const afw_utf8_octet_t *c;
    const afw_utf8_z_t *option_z;
    const afw_utf8_z_t *syntax_z;
    afw_size_t len;
    const afw_adaptor_journal_t *journal;
    const afw_object_t *request;
    afw_adaptor_impl_request_t impl_request;
    afw_boolean_t limit_applies;

    /*
     * Get request object.  Make one if necessary.  Additional properties
     * will be set for the get_object() properties.
     */
    request = afw_object_old_get_property_as_object(journal_entry, &afw_s_request,
        xctx);
    if (!request) {
        request = afw_object_create_embedded(
            journal_entry, &afw_s_request, xctx);
    }

    /* Set default limit. */
    limit_applies = false;
    limit = 100; /** @fixme Should this be configurable and where? By adaptor? */

    consumer_id = NULL;
    entry_cursor = NULL;
    journal = afw_adaptor_session_get_journal_interface(session, xctx);
    if (!journal) return NULL; /** @fixme Should this be error? */
    /** @fixme Might want to url_decode consumer_id and entry_cursor. */

    /* get_first */
    if (afw_utf8_starts_with_z(object_id, "get_first")) {
        syntax_z = "get_first";
        option = afw_adaptor_journal_option_get_first;
        option_z = "get_first";
        if (strlen("get_first") != object_id->len) goto error_special_id;
    }

    /* get_by_cursor:<event_cursor> */
    else if (afw_utf8_starts_with_z(object_id, "get_by_cursor:")) {
        syntax_z = "get_by_cursor:<event_cursor>";
        option = afw_adaptor_journal_option_get_by_cursor;
        option_z = "get_by_cursor";
        entry_cursor = afw_utf8_create(
            object_id->s + strlen("get_by_cursor:"),
            object_id->len - strlen("get_by_cursor:"),
            request->p, xctx);
        if (entry_cursor->len == 0) goto error_special_id;
    }

    /* get_next_after_cursor:<event_cursor> */
    else if (afw_utf8_starts_with_z(object_id, "get_next_after_cursor:")) {
        syntax_z = "get_next_after_cursor:<event_cursor>";
        option = afw_adaptor_journal_option_get_next_after_cursor;
        option_z = "get_next_after_cursor";
        entry_cursor = afw_utf8_create(
            object_id->s + strlen("get_next_after_cursor:"),
            object_id->len - strlen("get_next_after_cursor:"),
            request->p, xctx);
        if (entry_cursor->len == 0) goto error_special_id;
    }

    /* get_next_for_consumer:<consumer_id> */
    else if (afw_utf8_starts_with_z(object_id, "get_next_for_consumer:"))
    {
        limit_applies = true;
        syntax_z = "get_next_for_consumer:<consumer_id>";
        option = afw_adaptor_journal_option_get_next_for_consumer;
        option_z = "get_next_for_consumer";
        consumer_id = afw_utf8_create(
            object_id->s + strlen("get_next_for_consumer:"),
            object_id->len - strlen("get_next_for_consumer:"),
            request->p, xctx);
        if (consumer_id->len == 0) goto error_special_id;
    }

    /* get_next_for_consumer_after_cursor:<consumer_id>:<event_cursor> */
    else if (afw_utf8_starts_with_z(object_id,
        "get_next_for_consumer_after_cursor:"))
    {
        limit_applies = true;
        syntax_z =
            "get_next_for_consumer_after_cursor:<consumer_id>:<event_cursor>";
        option = afw_adaptor_journal_option_get_next_for_consumer;
        option_z = "get_next_for_consumer_after_cursor";
        s = c = object_id->s + strlen("get_next_for_consumer_after_cursor:");
        len = object_id->len - strlen("get_next_for_consumer_after_cursor:");
        for (; len > 0; c++, len--) {
            if (*c == ':') {
                consumer_id = afw_utf8_create(s, c - s, request->p, xctx);
                break;
            }
        }
        if (len <= 0) goto error_special_id;
        entry_cursor = afw_utf8_create(c + 1, len - 1, request->p, xctx);
        if (entry_cursor->len == 0) goto error_special_id;
    }

    /* advance_cursor_for_consumer:<consumer_id> */
    else if (afw_utf8_starts_with_z(object_id,
        "advance_cursor_for_consumer:"))
    {
        limit_applies = true;
        syntax_z = "advance_cursor_for_consumer:<consumer_id>";
        option =
            afw_adaptor_journal_option_advance_cursor_for_consumer;
        option_z = "advance_cursor_for_consumer";
        consumer_id = afw_utf8_create(
            object_id->s + strlen("advance_cursor_for_consumer:"),
            object_id->len - strlen("advance_cursor_for_consumer:"),
            request->p, xctx);
        if (consumer_id->len == 0) goto error_special_id;
    }

    /* <event_cursor> */
    else {
        option = afw_adaptor_journal_option_get_by_cursor;
        option_z = "get_by_cursor";
        entry_cursor = object_id;
    }

    /* Set more specific request function. */
    afw_object_set_property_as_string(request, &afw_s_function,
        &afw_s_a_journal_get_entry, xctx);

    /* Set option. */
    afw_object_set_property_as_string_from_utf8_z(request,
        &afw_s_option, option_z, xctx);

    /* Set entry consumerId property, if applicable. */
    if (consumer_id) {
        afw_object_set_property_as_string(request, &afw_s_consumerId,
            consumer_id, xctx);
    }

    /* Set entry consumerId property, if applicable. */
    if (entry_cursor) {
        afw_object_set_property_as_string(request, &afw_s_entryCursor,
            entry_cursor, xctx);
    }

    /* Set entry limit property, if applicable. */
    if (limit_applies) {
        afw_object_set_property_as_integer(request, &afw_s_limit,
            limit, xctx);
    } else {
        limit = 1;
    }

    /* Get entry and return. */
    afw_memory_clear(&impl_request);
    afw_adaptor_journal_get_entry(journal, &impl_request,
        option, consumer_id, entry_cursor, limit, journal_entry, xctx);
    afw_object_set_property_as_string(journal_entry,
        &afw_s_status, &afw_s_success, xctx);
    return journal_entry;

error_special_id:
    AFW_THROW_ERROR_FZ(general, xctx,
        "Expecting special objectId in the form: %s", syntax_z);
}
