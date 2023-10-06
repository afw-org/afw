// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Adaptor Add Object
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_adaptor_retrieve.c
 * @brief Adaptive Framework adaptor add object.
 */

#include "afw_internal.h"



/* Retrieve objects callback. */
static afw_boolean_t
impl_retrieve_objects_cb(
    const afw_object_t *object,
    void *context,
    afw_xctx_t *xctx)
{
    afw_adaptor_internal_object_cb_context_t *ctx = context;
    const afw_object_t *adapted_object;
    const afw_object_t *view;

    /* Process object from adaptor. */
    view = object;
    if (object) {
        afw_adaptor_internal_process_object_from_adaptor(
            &adapted_object, &view, ctx, object, ctx->p, xctx);
    }

    /* Callback with view or NULL.  Callback will release view.  */
    return ctx->original_callback(view, ctx->original_context, xctx);
}



/* Retrieve and cache objects. */
AFW_DEFINE(void)
afw_adaptor_retrieve_objects(
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *object_type_id,
    const afw_object_options_t *options,
    const afw_query_criteria_t *criteria,
    const afw_object_t *journal_entry,
    void * context,
    afw_object_cb_t callback,
    const afw_object_t *adaptor_type_specific,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_adaptor_session_t *session;
    const afw_object_t *request;
    afw_adaptor_internal_object_cb_context_t ctx;
    afw_adaptor_impl_t *impl;
    afw_adaptor_impl_request_t impl_request;


    /* Set request in journal entry. */
    afw_memory_clear(&impl_request);
    impl_request.request = request = afw_object_create_embedded(
        journal_entry, afw_s_request, xctx);
    impl_request.p = request->p;
    impl_request.journal_entry = journal_entry;
    impl_request.resource_id = afw_utf8_printf(impl_request.p, xctx,
        "/"
        AFW_UTF8_FMT "/"
        AFW_UTF8_FMT "/",
        AFW_UTF8_FMT_ARG(adaptor_id),
        AFW_UTF8_FMT_ARG(object_type_id));
    afw_object_set_property_as_string(request,
        afw_s_resourceId, impl_request.resource_id, xctx);
    impl_request.options = options;
    afw_object_set_property_as_string(request,
        afw_s_function, afw_s_retrieve_objects, xctx);
    afw_object_set_property_as_string(request,
        afw_s_adaptorId, adaptor_id, xctx);
    afw_object_set_property_as_string(request,
        afw_s_objectType, object_type_id, xctx);

    /* Journal entry is not supported for retrieve. */
    if (afw_utf8_equal(object_type_id,
        &AFW_OBJECT_S_OBJECT_TYPE_ID_JOURNAL_ENTRY))
    {
        AFW_THROW_ERROR_Z(general,
            "retrieve_objects() is not supported for "
            AFW_OBJECT_Q_OBJECT_TYPE_ID_JOURNAL_ENTRY,
            xctx);
    }

    /* Get an active session with adaptor. */
    session = afw_adaptor_session_get_cached(adaptor_id, false, xctx);
    impl = (afw_adaptor_impl_t *)session->adaptor->impl;

    /* Retrieve objects via session using intermediate callback. */
    afw_memory_clear(&ctx);
    ctx.p = p;
    ctx.session = session;
    ctx.impl = impl;
    ctx.adaptor_id = adaptor_id;
    ctx.object_type_id = object_type_id;
    ctx.original_context = context;
    ctx.original_callback = callback;
    ctx.journal_entry = journal_entry;
    ctx.options = options;
    afw_adaptor_session_retrieve_objects(session, &impl_request,
        object_type_id, criteria,
        &ctx, impl_retrieve_objects_cb,
        adaptor_type_specific,
        p, xctx);
}
