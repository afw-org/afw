// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Request Handler
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_request_handler.c
 * @brief Adaptive Framework Request Handler
 *
 */

#include "afw_internal.h"



/* Configuration handler for entry type "requestHandler". */
void afw_request_handler_internal_conf_type_create_cede_p(
    const afw_utf8_t *type,
    const afw_object_t *entry,
    const afw_utf8_t *source_location,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_request_handler_entry_t *e;
    afw_request_handler_head_t *head;
    afw_integer_t count;

    head = afw_environment_get_singleton(
        &AFW_REQUEST_HANDLER_S_SINGLETON_KEY_HEAD,
        xctx);
    if (!head) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "%" AFW_UTF8_CONTEXTUAL_LABEL_FMT
            "request handler head missing.",
            AFW_UTF8_FMT_ARG(source_location));
    }

    /* Allocate new entry. */
    e = afw_pool_calloc_type(p, afw_request_handler_entry_t, xctx);
    e->properties = afw_environment_prepare_conf_type_properties(
        entry, xctx);;

    /* Get uri_prefix and handler_type. */
    e->uri_prefix = afw_object_old_get_property_as_utf8(entry,
        &afw_s_uriPrefix, p, xctx);
    e->handler_type = afw_object_old_get_property_as_utf8(entry,
        &afw_s_requestHandlerType, p, xctx);

    /* Log request handler active. */
    AFW_LOG_FZ(info, xctx, "Request handler for URI prefix %" AFW_UTF8_FMT
        " starting.",
        AFW_UTF8_FMT_ARG(e->uri_prefix));

    /* Use request handler factory for handler id. */
    e->factory = afw_environment_get_request_handler_type(
        e->handler_type, xctx);
    if (!e->factory) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "%" AFW_UTF8_CONTEXTUAL_LABEL_FMT
            "invalid handler_type %" AFW_UTF8_FMT ".",
            AFW_UTF8_FMT_ARG(source_location),
            AFW_UTF8_FMT_ARG(e->handler_type));
    }

    /* Create handler. */
    e->rh = afw_request_handler_factory_create_request_handler_cede_p(
        e->factory, e->properties, p, xctx);

    /* Add handler entry to list. */
    if (!head->first_handler) {
        head->first_handler = e;
    } else {
        head->last_handler->next = e;
    }
    head->last_handler = e;

    /* Set runtime object for handler. */
    for (e = head->first_handler, count = 0; e; e = e->next, count++);
    head->last_handler->object_id =
        afw_number_integer_to_utf8(count, p, xctx);
    afw_object_meta_set_ids(entry, &afw_s_afw,
        &afw_s__AdaptiveRequestHandler_, head->last_handler->object_id, xctx);
    afw_runtime_env_set_object(entry, false, xctx);

    /* Log request handler active. */
    AFW_LOG_FZ(info, xctx, "Request handler for URI prefix %" AFW_UTF8_FMT
        " successfully started.",
        AFW_UTF8_FMT_ARG(head->last_handler->uri_prefix));
}
