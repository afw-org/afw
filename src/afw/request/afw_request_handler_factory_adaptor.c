// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_request_handler_factory interface for adaptor
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_request_handler_factory_adaptor.c
 * @brief Implementation of afw_request_handler_factory interface for adaptor.
 */

#include "afw_internal.h"



/*
 * Declarations for methods of interface afw_request_handler_factory implementation.
 */

/* Declares and rti/inf defines for interface afw_request_handler_factory */
#define AFW_IMPLEMENTATION_ID "adaptor"
#include "afw_request_handler_factory_impl_declares.h"


static const afw_utf8_t impl_factory_description =
AFW_UTF8_LITERAL("Request handler type to provide \"rest-like\" access to adaptive objects plus access to actions.");

AFW_DEFINE_CONST_DATA(afw_request_handler_factory_t)
afw_request_handler_factory_adaptor =
{
    &impl_afw_request_handler_factory_inf,
    AFW_UTF8_LITERAL("adaptor"),
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
    /* Create adaptor request handler. */
    return afw_request_handler_adaptor_create_cede_p(properties, p, xctx);
}
