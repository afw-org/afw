// See the 'COPYING' file in the project root for licensing information.
/*
 * Register adapter_type `ldap` for afw_ldap
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_ldap_register_adapter_type_ldap.c
 * @brief Generated environment register for adapter_type `ldap` (afw_ldap).
 */

#include "afw.h"

extern const afw_adapter_factory_t
afw_ldap_adapter_factory;

/* Register LDAP adapter factory. */
void
afw_ldap_adapter_type_ldap_register(afw_xctx_t *xctx)
{
    afw_environment_register_adapter_type(
        &afw_ldap_adapter_factory.adapter_type,
        &afw_ldap_adapter_factory, xctx);
}
