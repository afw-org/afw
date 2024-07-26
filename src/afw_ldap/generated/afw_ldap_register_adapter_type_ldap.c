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
