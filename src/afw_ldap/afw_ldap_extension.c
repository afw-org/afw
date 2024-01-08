// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_extension interface
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_ldap_extension.c
 * @brief Implementation of afw_extension interface for ldap adaptor.
 */

#include "afw.h"
#include "afw_ldap_internal.h"
#include "generated/afw_ldap_generated.h"


/* Declares and rti/inf defines for interface afw_extension */
#define AFW_IMPLEMENTATION_ID "afw_ldap"
#define AFW_IMPLEMENTATION_VERSION AFW_LDAP_VERSION_STRING
#include "afw_extension_impl_declares.h"
#include "generated/afw_ldap_version_info.h"

//* Define extension implementation. */
AFW_ENVIRONMENT_DEFINE_EXTENSION_IMPL();



/*
 * Implementation of method initialize for interface afw_extension.
 */
const afw_extension_t *
impl_afw_extension_initialize(
    const afw_extension_t * instance,
    const afw_object_t * properties,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    /*
     *  IMPORTANT:
     *
     *  Make sure associated _AdaptiveManifest_ is up to date,
     *  especially the "registers" property.
     *
     */
    
    /* See function in generated/afw_ldap_generated.c. */
    afw_ldap_generated_register(xctx);

    /* Return extension info. */
    return &impl_extension;
}

/*
 * Implementation of method release of interface afw_extension.
 */
void
impl_afw_extension_release(
    const afw_extension_t * instance,
    afw_xctx_t *xctx)
{
    /* Extension release() is not currently called. */
}
