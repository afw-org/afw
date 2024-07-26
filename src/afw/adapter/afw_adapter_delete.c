// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Adapter Delete Object
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_adapter_delete.c
 * @brief Adaptive Framework adapter delete object.
 */

#include "afw_internal.h"


/* Delete object from cache and via adapter. */
AFW_DEFINE(void)
afw_adapter_delete_object(
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_object_t *journal_entry,
    const afw_object_t *adapter_type_specific,
    afw_xctx_t *xctx)
{
    const afw_adapter_session_t *session;
    const afw_object_t *request;
    afw_adapter_impl_request_t impl_request;

    /* Set request in journal entry. */
    afw_memory_clear(&impl_request);
    impl_request.request = request = afw_object_create_embedded(
        journal_entry, afw_s_request, xctx);
    impl_request.p = request->p;
    impl_request.journal_entry = journal_entry;
    impl_request.resource_id = afw_utf8_printf(impl_request.p, xctx,
        "/"
        AFW_UTF8_FMT "/"
        AFW_UTF8_FMT "/"
        AFW_UTF8_FMT,
        AFW_UTF8_FMT_ARG(adapter_id),
        AFW_UTF8_FMT_ARG(object_type_id),
        AFW_UTF8_FMT_ARG(object_id));
    afw_object_set_property_as_string(request,
        afw_s_resourceId, impl_request.resource_id, xctx);
    afw_object_set_property_as_string(request,
        afw_s_function, afw_s_delete_object, xctx);
    afw_object_set_property_as_string(request,
        afw_s_adapterId, adapter_id, xctx);
    afw_object_set_property_as_string(request,
        afw_s_objectType, object_type_id, xctx);
    afw_object_set_property_as_string(request,
        afw_s_objectId, object_id, xctx);

    /* Get an active session with adapter. */
    session = afw_adapter_session_get_cached(adapter_id, true, xctx);

    /* Action prologue. */
    afw_adapter_internal_journal_prologue(session, journal_entry, xctx);

    /* If one of the core object types, handle special */
    if (afw_utf8_starts_with(object_type_id, AFW_OBJECT_S_CORE_ID_PREFIX)) {

        /* Journal entry. */
        if (afw_utf8_equal(object_type_id,
            AFW_OBJECT_S_OBJECT_TYPE_ID_JOURNAL_ENTRY))
        {
            AFW_THROW_ERROR_Z(general,
                "delete_object() is not supported for "
                AFW_OBJECT_Q_OBJECT_TYPE_ID_JOURNAL_ENTRY,
                xctx);
        }
    }


    /* Delete object. */
    afw_adapter_session_delete_object(session, &impl_request,
        object_type_id, object_id, adapter_type_specific, xctx);

    /* Action epilogue. */
    afw_adapter_internal_journal_epilogue(session, journal_entry, true, xctx);
}
