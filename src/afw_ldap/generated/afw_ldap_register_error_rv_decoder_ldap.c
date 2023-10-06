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
