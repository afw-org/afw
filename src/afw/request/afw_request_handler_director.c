// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_request_handler interface for director
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_request_handler_director.c
 * @brief Implementation of afw_request_handler interface for director.
 */

#include "afw_internal.h"



/* Declares and rti/inf defines for interface afw_request_handler */
#define AFW_IMPLEMENTATION_ID "director"
#include "afw_request_handler_impl_declares.h"


typedef struct self_request_handler_director_s {
    afw_request_handler_t pub;
    const afw_pool_t *p;
    const afw_object_t *properties;
    afw_request_handler_head_t *head;
} self_request_handler_director_t;



AFW_DEFINE(const afw_request_handler_t *)
afw_request_handler_director_create(
    const afw_server_t *server,
    const afw_utf8_z_t *config_file,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    self_request_handler_director_t *self = NULL;

    /* Allocate memory for self. */
    self = afw_pool_calloc_type(p, self_request_handler_director_t, xctx);

    /* Initialize Self. */
    self->pub.inf = &impl_afw_request_handler_inf;
    self->p = p;

    /* Get handler head. */
    self->head = afw_environment_get_singleton(
        &AFW_REQUEST_HANDLER_S_SINGLETON_KEY_HEAD,
        xctx);
    if (!self->head) {
        AFW_THROW_ERROR_Z(general, "Request handler head missing.", xctx);
    }

    /* Return new adaptor. */
    return (const afw_request_handler_t *)self;
}



/*
 * Implementation of method release of interface afw_request_handler.
 */
void
impl_afw_request_handler_release(
    const afw_request_handler_t * instance,
    afw_xctx_t *xctx)
{
    /*
     * Continue release, even if there is already an error.  Don't overwrite
     * existing error.
     */

    /* Resources released when execution context (xctx) is released. */
}



/*
 * Implementation of method process of interface afw_request_handler.
 */
void
impl_afw_request_handler_process(
    const afw_request_handler_t * instance,
    const afw_request_t * request,
    afw_xctx_t *xctx)
{
    self_request_handler_director_t *self =
        (self_request_handler_director_t *)instance;
    const afw_request_handler_entry_t *request_handler_entry;

    /* Call appropriate handler. */
    for (request_handler_entry = self->head->first_handler; request_handler_entry;
        request_handler_entry = request_handler_entry->next)
    {
        if (afw_utf8_starts_with(request->uri,
            request_handler_entry->uri_prefix))
        {
            afw_request_handler_process(request_handler_entry->rh, request, xctx);
            return;
        }
    }

    AFW_THROW_ERROR_FZ(not_found, xctx,
        "Prefix " AFW_UTF8_FMT " not found",
        AFW_UTF8_FMT_ARG(request->uri));
}
