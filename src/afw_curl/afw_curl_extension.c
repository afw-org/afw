// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_extension Implementation for afw_curl
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_curl_extension.c
 * @brief Implementation of afw_extension interface for afw_curl
 */

#include "afw.h"

#include <curl/curl.h>

/* Version string for this extension. */
#define AFW_IMPLEMENTATION_VERSION AFW_CURL_VERSION_STRING

/* Declares and rti/inf defines for interface afw_extension */
#define AFW_IMPLEMENTATION_ID "afw_curl"
#include "afw_extension_impl_declares.h"
#include "generated/afw_curl_generated.h"
#include "generated/afw_curl_version_info.h"


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
    CURLcode rc;

    /*
     *  IMPORTANT:
     *
     *  Make sure associated _AdaptiveManifest_ is up to date,
     *  especially the "registers" property.
     *
     */

    /* Global initialization of libCURL */
    rc = curl_global_init(CURL_GLOBAL_ALL);
    if (rc != 0) {
        AFW_THROW_ERROR_Z(general, "Error initializing cURL library.", xctx);
    }
     
    /* Call the generated register function for this extension. */
    afw_curl_generated_register(xctx);

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
