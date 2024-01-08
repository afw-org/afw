// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Adaptor Add Object
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_adaptor_add.c
 * @brief Adaptive Framework adaptor add object.
 */

#include "afw_internal.h"


/* Get object callback. */
static afw_boolean_t
impl_get_object_cb(
    const afw_object_t *object,
    void *context,
    afw_xctx_t *xctx)
{
    afw_adaptor_internal_object_cb_context_t *ctx = context;
    const afw_object_t *adapted_object;
    const afw_object_t *view;

    /* Process object from adaptor and cache it. */
    view = object;
    if (object) {
        afw_adaptor_internal_process_object_from_adaptor(
            &adapted_object, &view, ctx, object, ctx->p, xctx);
    }

    *(const afw_object_t **)ctx->original_context = view;

    /* Return indicating not to short circuit */
    return false;
}



/* Get and cache object. */
AFW_DEFINE(const afw_object_t *)
afw_adaptor_get_object(
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_object_options_t *options,
    const afw_query_criteria_t *criteria,
    const afw_object_t *journal_entry,
    const afw_object_t *adaptor_type_specific,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_adaptor_session_t *session;
    afw_adaptor_internal_object_cb_context_t ctx;
    const afw_object_t *obj;
    const afw_object_t *request;
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
        AFW_UTF8_FMT "/"
        AFW_UTF8_FMT,
        AFW_UTF8_FMT_ARG(adaptor_id),
        AFW_UTF8_FMT_ARG(object_type_id),
        AFW_UTF8_FMT_ARG(object_id));
    afw_object_set_property_as_string(request,
        afw_s_resourceId, impl_request.resource_id, xctx);
    impl_request.options = options;
    afw_object_set_property_as_string(request,
        afw_s_function, afw_s_get_object, xctx);
    afw_object_set_property_as_string(request,
        afw_s_adaptorId, adaptor_id, xctx);
    afw_object_set_property_as_string(request,
        afw_s_objectType, object_type_id, xctx);
    afw_object_set_property_as_string(request,
        afw_s_objectId, object_id, xctx);

    /* Get an active session with adaptor. */
    session = afw_adaptor_session_get_cached(adaptor_id, false, xctx);
    impl = (afw_adaptor_impl_t *)session->adaptor->impl;

    /* Initialize context. */
    afw_memory_clear(&ctx);
    ctx.p = p;
    ctx.session = session;
    ctx.impl = impl;
    ctx.adaptor_id = adaptor_id;
    ctx.object_type_id = object_type_id;
    ctx.object_id = object_id;
    ctx.original_context = (void *)&obj;
    ctx.original_callback = NULL;
    ctx.journal_entry = journal_entry;
    ctx.options = options;


    /* If object_type_id is _AdaptiveJournalEntry_, handle special */
    if (afw_utf8_equal(object_type_id,
        AFW_OBJECT_S_OBJECT_TYPE_ID_JOURNAL_ENTRY))
    {
        return afw_adaptor_internal_journal_get_entry(session, object_id, journal_entry,
            xctx);
    }

    /* Get objects via session using intermediate callback. */
    afw_adaptor_session_get_object(session, &impl_request,
        object_type_id, object_id,
        &ctx, impl_get_object_cb, 
        adaptor_type_specific,
        p, xctx);

    /* Return object. */
    return obj;
}



/* Get and cache object by path. */
AFW_DEFINE(const afw_object_t *)
afw_adaptor_get_object_by_path(
    const afw_utf8_t *path,
    const afw_object_t *journal_entry,
    const afw_object_t *adaptor_type_specific,
    afw_xctx_t *xctx)
{
    const afw_object_path_parsed_t *parsed_path;

    /* Parse path. */
    parsed_path = afw_object_path_parse(path, NULL, NULL, xctx->p, xctx);
    if (!parsed_path ||
        !parsed_path->entity_object_id.s ||
        parsed_path->first_property_name)
    {
        AFW_THROW_ERROR_Z(general, "Invalid path", xctx);
    }

    /* Return object. */
    return afw_adaptor_get_object(
        &parsed_path->adaptor_id,
        &parsed_path->object_type_id,
        &parsed_path->entity_object_id,
        NULL, NULL, journal_entry,
        adaptor_type_specific, xctx->p, xctx);
}
