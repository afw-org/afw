// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Adapter Journal
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_adapter_journal.c
 * @brief Adapter journal entry helpers and cursor support.
 */

#include "afw_internal.h"



void
afw_adapter_internal_journal_prologue(
    const afw_adapter_session_t *session,
    const afw_object_t *journal_entry,
    afw_xctx_t *xctx)
{
    const afw_value_t *now;

    now = afw_value_create_dateTime_now_utc(journal_entry->p, xctx);
    afw_object_set_property(journal_entry, afw_v_beginTime, now, xctx);
}


/* Authorize a journal operation. See afw_adapter_internal.h. */
void
afw_adapter_internal_journal_authorize(
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *object_id,
    const afw_value_t *action_id_value,
    const afw_object_t *object,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *resource_id;

    resource_id = afw_utf8_printf(xctx->p, xctx,
        "/%ku/" AFW_OBJECT_Q_OBJECT_TYPE_ID_JOURNAL_ENTRY "/%ku",
        adapter_id, (object_id) ? object_id : afw_s_a_empty_string);
    afw_authorization_check(true, NULL,
        afw_value_create_unmanaged_string(resource_id, xctx->p, xctx),
        (object)
            ? afw_value_create_unmanaged_object(object, xctx->p, xctx)
            : NULL,
        action_id_value, xctx->p, xctx);
}


void
afw_adapter_internal_journal_epilogue(
    const afw_adapter_session_t *session,
    const afw_object_t *journal_entry,
    afw_boolean_t modification,
    afw_xctx_t *xctx)
{
    const afw_value_t *now;
    const afw_adapter_session_t *journal_session;
    const afw_adapter_journal_t *journal;
    afw_adapter_impl_request_t impl_request;

    now = afw_value_create_dateTime_now_utc(journal_entry->p, xctx);
    afw_object_set_property(journal_entry, afw_v_endTime, now, xctx);

    /** @fixme Might want to record failures too???? */
    /* If this is a modification, add event to journal if requested. */
    if (modification && session->adapter->impl->journal_adapter_id) {
        journal_session = afw_adapter_session_get_cached(
            session->adapter->impl->journal_adapter_id, true, xctx);
        journal = afw_adapter_session_get_journal_interface(journal_session,
            xctx);
        afw_memory_clear(&impl_request);
        afw_adapter_journal_add_entry(journal, &impl_request,
            journal_entry, xctx);
    }
}



static const afw_adapter_journal_t *
impl_get_journal_interface(const afw_utf8_t *adapter_id,
    afw_boolean_t begin_transaction, afw_xctx_t *xctx)
{
    const afw_adapter_session_t *session;
    const afw_adapter_journal_t *journal;

    /* Get an active session. */
    session = afw_adapter_session_get_cached(adapter_id, begin_transaction, xctx);

    journal = afw_adapter_session_get_journal_interface(session, xctx);
    if (!journal) goto error;
    return journal;

error:
    AFW_THROW_ERROR_FZ(method_not_supported, xctx,
        "Adapter '%ku' does not support journal",
        adapter_id);
}


static const afw_utf8_t impl_s_get_first = AFW_UTF8_LITERAL("get_first");

/*
 * The objectId part of the resource id a journal read is authorized on:
 * the special objectId of its REST form without the limit, so the
 * built-in and the REST form of an operation are checked on the same
 * path. A read of the entry at a cursor is the entry's own path.
 */
static const afw_utf8_t *
impl_resource_object_id(
    afw_adapter_journal_option_t option,
    const afw_utf8_t *consumer_id,
    const afw_utf8_t *cursor,
    afw_xctx_t *xctx)
{
    switch (option) {
    case afw_adapter_journal_option_get_first:
        return &impl_s_get_first;
    case afw_adapter_journal_option_get_by_cursor:
        return cursor;
    case afw_adapter_journal_option_get_next_after_cursor:
        return afw_utf8_printf(xctx->p, xctx,
            "get_next_after_cursor:%ku", cursor);
    case afw_adapter_journal_option_get_next_for_consumer:
        return afw_utf8_printf(xctx->p, xctx,
            "get_next_for_consumer:%ku", consumer_id);
    case afw_adapter_journal_option_get_next_for_consumer_after_cursor:
        return afw_utf8_printf(xctx->p, xctx,
            "get_next_for_consumer_after_cursor:%ku:%ku",
            consumer_id, cursor);
    case afw_adapter_journal_option_advance_cursor_for_consumer:
        return afw_utf8_printf(xctx->p, xctx,
            "advance_cursor_for_consumer:%ku", consumer_id);
    }
    AFW_THROW_ERROR_Z(general, "Unknown journal option", xctx);
}


/*
 * Authorize and do a journal read for both the built-ins and the REST
 * forms. limit 0 is no limit; it applies only to the consumer forms. The
 * consumer forms change the peer, so they begin a transaction.
 */
static void
impl_get_entry(
    const afw_utf8_t *adapter_id,
    afw_adapter_journal_option_t option,
    const afw_utf8_t *consumer_id,
    const afw_utf8_t *cursor,
    afw_size_t limit,
    const afw_object_t *result,
    afw_xctx_t *xctx)
{
    const afw_adapter_journal_t *journal;
    afw_adapter_impl_request_t impl_request;

    journal = impl_get_journal_interface(adapter_id, consumer_id != NULL,
        xctx);

    afw_adapter_internal_journal_authorize(adapter_id,
        impl_resource_object_id(option, consumer_id, cursor, xctx),
        afw_authorization_action_id_read, NULL, xctx);

    afw_memory_clear(&impl_request);
    afw_adapter_journal_get_entry(journal, &impl_request,
        option, consumer_id, cursor, limit, result, xctx);
}


/* Journal - get first entry. */
AFW_DEFINE(const afw_object_t *)
afw_adapter_journal_get_first(
    const afw_utf8_t *adapter_id,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *result;

    result = afw_object_create_unmanaged_new_p(p, xctx);
    impl_get_entry(adapter_id, afw_adapter_journal_option_get_first,
        NULL, NULL, 0, result, xctx);
    return result;
}


/* Journal - get entry at cursor. */
AFW_DEFINE(const afw_object_t *)
afw_adapter_journal_get_by_cursor(
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *cursor,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *result;

    result = afw_object_create_unmanaged_new_p(p, xctx);
    impl_get_entry(adapter_id, afw_adapter_journal_option_get_by_cursor,
        NULL, cursor, 0, result, xctx);
    return result;
}


/* Journal - get next entry after cursor. */
AFW_DEFINE(const afw_object_t *)
afw_adapter_journal_get_next_after_cursor(
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *cursor,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *result;

    result = afw_object_create_unmanaged_new_p(p, xctx);
    impl_get_entry(adapter_id,
        afw_adapter_journal_option_get_next_after_cursor,
        NULL, cursor, 0, result, xctx);
    return result;
}


/* Journal - get next entry for consumer. */
AFW_DEFINE(const afw_object_t *)
afw_adapter_journal_get_next_for_consumer(
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *consumer_id,
    afw_size_t limit,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *result;

    result = afw_object_create_unmanaged_new_p(p, xctx);
    impl_get_entry(adapter_id,
        afw_adapter_journal_option_get_next_for_consumer,
        consumer_id, NULL, limit, result, xctx);
    return result;
}


/* Journal - get next entry after cursor for consumer. */
AFW_DEFINE(const afw_object_t *)
afw_adapter_journal_get_next_for_consumer_after_cursor(
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *consumer_id,
    const afw_utf8_t *cursor,
    afw_size_t limit,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *result;

    result = afw_object_create_unmanaged_new_p(p, xctx);
    impl_get_entry(adapter_id,
        afw_adapter_journal_option_get_next_for_consumer_after_cursor,
        consumer_id, cursor, limit, result, xctx);
    return result;
}


/* Journal - advance cursor for consumer. */
AFW_DEFINE(const afw_object_t *)
afw_adapter_journal_advance_cursor_for_consumer(
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *consumer_id,
    afw_size_t limit,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *result;

    result = afw_object_create_unmanaged_new_p(p, xctx);
    impl_get_entry(adapter_id,
        afw_adapter_journal_option_advance_cursor_for_consumer,
        consumer_id, NULL, limit, result, xctx);
    return result;
}


/*
 * Journal - mark entry consumed by consumer. Authorized as modify of
 * mark_consumed:<consumer_id>:<cursor>, the special objectId of its REST
 * form.
 */
AFW_DEFINE(void)
afw_adapter_journal_mark_consumed(
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *consumer_id,
    const afw_utf8_t *cursor,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_adapter_journal_t *journal;
    afw_adapter_impl_request_t impl_request;

    /* Get journal interface. */
    journal = impl_get_journal_interface(adapter_id, true, xctx);

    afw_adapter_internal_journal_authorize(adapter_id,
        afw_utf8_printf(xctx->p, xctx, "mark_consumed:%ku:%ku",
            consumer_id, cursor),
        afw_authorization_action_id_modify, NULL, xctx);

    afw_memory_clear(&impl_request);
    afw_adapter_journal_mark_entry_consumed(journal, &impl_request,
        consumer_id, cursor, xctx);
}


/*
 * Split the arguments of a special journal objectId at ':' into at most
 * max non-empty fields. Returns the number of fields, or 0 if there are
 * more than max or one is empty.
 */
static afw_size_t
impl_split_special_id_args(
    const afw_utf8_octet_t *s,
    afw_size_t len,
    const afw_utf8_t *fields[],
    afw_size_t max,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_utf8_octet_t *start;
    afw_size_t count;

    for (count = 0, start = s; ; s++, len--) {
        if (len == 0 || *s == ':') {
            if (count == max || s == start) return 0;
            fields[count++] = afw_utf8_create(start, s - start, p, xctx);
            if (len == 0) return count;
            start = s + 1;
        }
    }
}


/* Parse a special journal objectId limit field: decimal digits, not 0. */
static afw_boolean_t
impl_parse_special_id_limit(
    const afw_utf8_t *field,
    afw_size_t *limit)
{
    afw_size_t i;
    afw_size_t result;

    for (i = 0, result = 0; i < field->len; i++) {
        if (field->s[i] < '0' || field->s[i] > '9' ||
            result > (AFW_SIZE_T_MAX - 9) / 10)
        {
            return false;
        }
        result = result * 10 + (field->s[i] - '0');
    }
    if (result == 0) return false;
    *limit = result;
    return true;
}


const afw_object_t *
afw_adapter_internal_journal_get_entry(
    const afw_adapter_session_t *session,
    const afw_utf8_t *object_id,
    const afw_object_t *journal_entry,
    afw_xctx_t *xctx)
{
    afw_adapter_journal_option_t option;
    const afw_utf8_t *consumer_id;
    const afw_utf8_t *entry_cursor;
    afw_size_t limit;
    const afw_utf8_t *fields[3];
    afw_size_t count;
    const afw_utf8_z_t *option_z;
    const afw_utf8_z_t *syntax_z;
    const afw_object_t *request;

    /*
     * Get request object.  Make one if necessary.  Additional properties
     * will be set for the get_object() properties.
     */
    request = afw_object_get_property_as_object_internal(journal_entry, afw_v_request,
        xctx);
    if (!request) {
        request = afw_object_create_embedded(
            journal_entry, afw_v_request, xctx);
    }

    /* No limit when a consumer form does not give one, as the built-ins. */
    limit = 0;

    consumer_id = NULL;
    entry_cursor = NULL;

    /* get_first */
    if (afw_utf8_starts_with_utf8_z(object_id, "get_first")) {
        syntax_z = "get_first";
        option = afw_adapter_journal_option_get_first;
        option_z = "get_first";
        if (strlen("get_first") != object_id->len) goto error_special_id;
    }

    /* get_by_cursor:<event_cursor> */
    else if (afw_utf8_starts_with_utf8_z(object_id, "get_by_cursor:")) {
        syntax_z = "get_by_cursor:<event_cursor>";
        option = afw_adapter_journal_option_get_by_cursor;
        option_z = "get_by_cursor";
        entry_cursor = afw_utf8_create(
            object_id->s + strlen("get_by_cursor:"),
            object_id->len - strlen("get_by_cursor:"),
            request->p, xctx);
        if (entry_cursor->len == 0) goto error_special_id;
    }

    /* get_next_after_cursor:<event_cursor> */
    else if (afw_utf8_starts_with_utf8_z(object_id, "get_next_after_cursor:")) {
        syntax_z = "get_next_after_cursor:<event_cursor>";
        option = afw_adapter_journal_option_get_next_after_cursor;
        option_z = "get_next_after_cursor";
        entry_cursor = afw_utf8_create(
            object_id->s + strlen("get_next_after_cursor:"),
            object_id->len - strlen("get_next_after_cursor:"),
            request->p, xctx);
        if (entry_cursor->len == 0) goto error_special_id;
    }

    /* get_next_for_consumer:<consumer_id>[:<limit>] */
    else if (afw_utf8_starts_with_utf8_z(object_id, "get_next_for_consumer:"))
    {
        syntax_z = "get_next_for_consumer:<consumer_id>[:<limit>]";
        option = afw_adapter_journal_option_get_next_for_consumer;
        option_z = "get_next_for_consumer";
        count = impl_split_special_id_args(
            object_id->s + strlen("get_next_for_consumer:"),
            object_id->len - strlen("get_next_for_consumer:"),
            fields, 2, request->p, xctx);
        if (count == 0) goto error_special_id;
        consumer_id = fields[0];
        if (count == 2 && !impl_parse_special_id_limit(fields[1], &limit)) {
            goto error_special_id;
        }
    }

    /*
     * get_next_for_consumer_after_cursor:
     *     <consumer_id>:<event_cursor>[:<limit>]
     */
    else if (afw_utf8_starts_with_utf8_z(object_id,
        "get_next_for_consumer_after_cursor:"))
    {
        syntax_z = "get_next_for_consumer_after_cursor:"
            "<consumer_id>:<event_cursor>[:<limit>]";
        option =
            afw_adapter_journal_option_get_next_for_consumer_after_cursor;
        option_z = "get_next_for_consumer_after_cursor";
        count = impl_split_special_id_args(
            object_id->s + strlen("get_next_for_consumer_after_cursor:"),
            object_id->len - strlen("get_next_for_consumer_after_cursor:"),
            fields, 3, request->p, xctx);
        if (count < 2) goto error_special_id;
        consumer_id = fields[0];
        entry_cursor = fields[1];
        if (count == 3 && !impl_parse_special_id_limit(fields[2], &limit)) {
            goto error_special_id;
        }
    }

    /* advance_cursor_for_consumer:<consumer_id>[:<limit>] */
    else if (afw_utf8_starts_with_utf8_z(object_id,
        "advance_cursor_for_consumer:"))
    {
        syntax_z = "advance_cursor_for_consumer:<consumer_id>[:<limit>]";
        option =
            afw_adapter_journal_option_advance_cursor_for_consumer;
        option_z = "advance_cursor_for_consumer";
        count = impl_split_special_id_args(
            object_id->s + strlen("advance_cursor_for_consumer:"),
            object_id->len - strlen("advance_cursor_for_consumer:"),
            fields, 2, request->p, xctx);
        if (count == 0) goto error_special_id;
        consumer_id = fields[0];
        if (count == 2 && !impl_parse_special_id_limit(fields[1], &limit)) {
            goto error_special_id;
        }
    }

    /*
     * mark_consumed:<consumer_id>:<event_cursor>
     *
     * The same as journal_mark_consumed().
     */
    else if (afw_utf8_starts_with_utf8_z(object_id, "mark_consumed:")) {
        syntax_z = "mark_consumed:<consumer_id>:<event_cursor>";
        count = impl_split_special_id_args(
            object_id->s + strlen("mark_consumed:"),
            object_id->len - strlen("mark_consumed:"),
            fields, 2, request->p, xctx);
        if (count != 2) goto error_special_id;
        afw_object_set_property_as_string_from_utf8_z(request,
            afw_v_option, "mark_consumed", xctx);
        afw_object_set_property_as_string_internal(request, afw_v_consumerId,
            fields[0], xctx);
        afw_object_set_property_as_string_internal(request, afw_v_entryCursor,
            fields[1], xctx);
        afw_adapter_journal_mark_consumed(&session->adapter->adapter_id,
            fields[0], fields[1], journal_entry->p, xctx);
        afw_object_set_property(journal_entry,
            afw_v_status, afw_v_success, xctx);
        return journal_entry;
    }

    /* <event_cursor> */
    else {
        option = afw_adapter_journal_option_get_by_cursor;
        option_z = "get_by_cursor";
        entry_cursor = object_id;
    }

    /* Set more specific request function. */
    afw_object_set_property(request, afw_v_function,
        afw_v_a_journal_get_entry, xctx);

    /* Set option. */
    afw_object_set_property_as_string_from_utf8_z(request,
        afw_v_option, option_z, xctx);

    /* Set entry consumerId property, if applicable. */
    if (consumer_id) {
        afw_object_set_property_as_string_internal(request, afw_v_consumerId,
            consumer_id, xctx);
    }

    /* Set entry consumerId property, if applicable. */
    if (entry_cursor) {
        afw_object_set_property_as_string_internal(request, afw_v_entryCursor,
            entry_cursor, xctx);
    }

    /* Set entry limit property, if one was given. */
    if (limit > 0) {
        afw_object_set_property_as_integer_internal(request, afw_v_limit,
            limit, xctx);
    }

    /* Get entry the same way as the built-in, and return. */
    impl_get_entry(&session->adapter->adapter_id, option,
        consumer_id, entry_cursor, limit, journal_entry, xctx);
    afw_object_set_property(journal_entry,
        afw_v_status, afw_v_success, xctx);
    return journal_entry;

error_special_id:
    AFW_THROW_ERROR_FZ(general, xctx,
        "Expecting special objectId in the form: %s", syntax_z);
}
