// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_adapter_factory interface for LDAP
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_ldap_adapter_factory.c
 * @brief Implementation of afw_adapter_factory interface for LDAP
 */

#include "afw.h"
#include "afw_ldap_internal.h"


/* Declares and rti/inf defines for interface afw_adapter_factory */
#define AFW_IMPLEMENTATION_ID "ldap"
#include "afw_adapter_factory_impl_declares.h"


static const afw_utf8_t impl_factory_description =
AFW_UTF8_LITERAL("Adapter type for Lightweight Directory Access Protocol (LDAP).");

const afw_adapter_factory_t
afw_ldap_adapter_factory =
{
    &impl_afw_adapter_factory_inf,
    AFW_UTF8_LITERAL("ldap"),
    &impl_factory_description
};


/*
 * Implementation of method create_adapter_cede_p of interface afw_adapter_factory.
 */
const afw_adapter_t *
impl_afw_adapter_factory_create_adapter_cede_p (
    const afw_adapter_factory_t * instance,
    const afw_object_t * properties,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    return afw_ldap_internal_adapter_create_cede_p(properties, p, xctx);
}
