// See the 'COPYING' file in the project root for licensing information.
/*
 * Register error_rv_decoder `ldap` for afw_ldap
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_ldap_register_error_rv_decoder_ldap.c
 * @brief Generated environment register for error_rv_decoder `ldap` (afw_ldap).
 */

#include "afw.h"
#include "afw_ldap_strings.h"
#include "../afw_ldap_internal.h"

/* LDAP RV decoder. */
static const afw_utf8_z_t *
impl_rv_decoder_z_ldap(int rv,
    afw_utf8_z_t *wa, afw_size_t wa_size)
{
    return ldap_err2string(rv);
}


/* Register LDAP RV decoder. */
void
afw_ldap_error_rv_decoder_ldap_register(afw_xctx_t *xctx)
{
    afw_environment_register_error_rv_decoder(afw_ldap_s_ldap,
        impl_rv_decoder_z_ldap, xctx);
}
