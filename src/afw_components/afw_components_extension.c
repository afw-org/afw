// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_extension Implementation for afw_components
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_components_extension.c
 * @brief Implementation of afw_extension interface for afw_components
 */

#include "afw.h"


/* Version string for this extension. */
#define AFW_IMPLEMENTATION_VERSION AFW_COMPONENTS_VERSION_STRING

/* Declares and rti/inf defines for interface afw_extension */
#define AFW_IMPLEMENTATION_ID "afw_components"
#include "afw_extension_impl_declares.h"
#include "generated/afw_components_generated.h"
#include "generated/afw_components_version_info.h"


/* Define extension implementation. */
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
    
    /* Call the generated register function for this extension. */
    afw_components_generated_register(xctx);

    /* Return extension instance. */
    return &impl_extension;
}


/*
 * Implementation of method release for interface afw_extension.
 */
void
impl_afw_extension_release(
    const afw_extension_t * instance,
    afw_xctx_t *xctx)
{
    /* Extension release() is not currently called. */
}
