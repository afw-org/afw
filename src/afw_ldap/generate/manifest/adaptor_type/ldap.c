#include "afw.h"

extern const afw_adaptor_factory_t
afw_ldap_adaptor_factory;

/* Register LDAP adaptor factory. */
void
afw_ldap_adaptor_type_ldap_register(afw_xctx_t *xctx)
{
    afw_environment_register_adaptor_type(
        &afw_ldap_adaptor_factory.adaptor_type,
        &afw_ldap_adaptor_factory, xctx);
}
