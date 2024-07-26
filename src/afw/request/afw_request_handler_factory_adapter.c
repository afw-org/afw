// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_request_handler_factory interface for adapter
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_request_handler_factory_adapter.c
 * @brief Implementation of afw_request_handler_factory interface for adapter.
 */

#include "afw_internal.h"



/*
 * Declarations for methods of interface afw_request_handler_factory implementation.
 */

/* Declares and rti/inf defines for interface afw_request_handler_factory */
#define AFW_IMPLEMENTATION_ID "adapter"
#include "afw_request_handler_factory_impl_declares.h"


static const afw_utf8_t impl_factory_description =
AFW_UTF8_LITERAL("Request handler type to provide \"rest-like\" access to adaptive objects plus access to actions.");

AFW_DEFINE_CONST_DATA(afw_request_handler_factory_t)
afw_request_handler_factory_adapter =
{
    &impl_afw_request_handler_factory_inf,
    AFW_UTF8_LITERAL("adapter"),
    &impl_factory_description
};


/*
 * Implementation of method create_request_handler_cede_p of interface afw_request_handler_factory.
 */
const afw_request_handler_t *
impl_afw_request_handler_factory_create_request_handler_cede_p (
    const afw_request_handler_factory_t * instance,
    const afw_object_t * properties,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    /* Create adapter request handler. */
    return afw_request_handler_adapter_create_cede_p(properties, p, xctx);
}
