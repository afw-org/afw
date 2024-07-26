// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Adapter Add Object
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_adapter_add.c
 * @brief Adaptive Framework adapter add object.
 */

#include "afw_internal.h"



/* Call adapter to add object and remove from cache. */
AFW_DEFINE(const afw_utf8_t *)
afw_adapter_add_object(
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *suggested_object_id,
    const afw_object_t *object,
    const afw_object_t *journal_entry,
    const afw_object_t *adapter_type_specific,
    afw_xctx_t *xctx)
{
    const afw_adapter_session_t *session;
    const afw_utf8_t *path;
    const afw_utf8_t *object_id;
    const afw_object_t *request;
    const afw_adapter_journal_t *journal;
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
        AFW_UTF8_FMT_OPTIONAL_ARG(suggested_object_id));
    afw_object_set_property_as_string(request,
        afw_s_resourceId, impl_request.resource_id, xctx);
    afw_object_set_property_as_string(request,
        afw_s_function, afw_s_add_object, xctx);
    afw_object_set_property_as_string(request,
        afw_s_adapterId, adapter_id, xctx);
    afw_object_set_property_as_string(request,
        afw_s_objectType, object_type_id, xctx);
    if (suggested_object_id) {
        afw_object_set_property_as_string(request,
            afw_s_suggestedObjectId, suggested_object_id, xctx);
    }
    afw_object_set_property_as_object(request,
        afw_s_object, object, xctx);

    /* Get an active session with adapter. */
    session = afw_adapter_session_get_cached(adapter_id, true, xctx);

    /* Action prologue. */
    afw_adapter_internal_journal_prologue(session, journal_entry, xctx);

    /* If one of the core object types, handle special */
    object_id = NULL;
    if (afw_utf8_starts_with(object_type_id, AFW_OBJECT_S_CORE_ID_PREFIX)) {

        /* Journal entry. */
        if (afw_utf8_equal(object_type_id,
            AFW_OBJECT_S_OBJECT_TYPE_ID_JOURNAL_ENTRY))
        {
            journal = afw_adapter_session_get_journal_interface(session,
                xctx);
            if (!journal) {
                AFW_THROW_ERROR_FZ(general, xctx,
                    "adapter_id " AFW_UTF8_FMT_Q
                    " session get_journal() returned NULL",
                    AFW_UTF8_FMT_ARG(&session->adapter->adapter_id));
            }
            object_id = afw_adapter_journal_add_entry(journal,
                &impl_request, object, xctx);
        }
    }

    /* If special handling didn't add object, do normal add object. */
    if (!object_id) {
        object_id = afw_adapter_session_add_object(session, &impl_request,
            object_type_id, suggested_object_id, object,
            adapter_type_specific, xctx);
    }

    /* Set info in journal entry. */
    afw_object_set_property_as_string(journal_entry,
        afw_s_objectId, object_id, xctx);
    path = afw_object_path_make(adapter_id, object_type_id, object_id,
        journal_entry->p, xctx);
    afw_object_set_property_as_string(journal_entry,
        afw_s_path, path, xctx);

    /* If requested, write to event journal. */
    //if (session->adapter->)

    /* Action epilogue. */
    afw_adapter_internal_journal_epilogue(session, journal_entry, true, xctx);

    return object_id;
}

