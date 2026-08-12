// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_crypto_extension.c
 * @brief Cryptographic functions for Adaptive Script (afw_crypto extension)
 *
 * This is the implementation of afw_extension for afw_crypto.
 */

#include "afw.h"
#include "afw_crypto_extension.h"
#include "afw_crypto_internal.h"

/* Declares and rti/inf defines for interface afw_extension */
#define AFW_IMPLEMENTATION_ID "afw_crypto"
#define AFW_EXTENSION_SELF_T afw_crypto_extension_self_t
#include "afw_extension_impl_declares.h"

#include "generated/afw_crypto_generated_internal.h"

#define AFW_IMPLEMENTATION_VERSION AFW_CRYPTO_VERSION_STRING
#include "generated/afw_crypto_version_info.h"

/* Define extension implementation. */
AFW_ENVIRONMENT_DEFINE_EXTENSION_IMPL();


/*
 * Implementation of method initialize for interface afw_extension.
 */
const afw_extension_t *
impl_afw_extension_initialize(
    AFW_EXTENSION_SELF_T *self,
    const afw_object_t * properties,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
    (void)self;
    (void)properties;
    (void)p;

    /* Process-global keystore + OpenSSL init before function registration. */
    afw_crypto_internal_keystore_initialize(xctx);

    /* Call the generated register function for this extension. */
    afw_crypto_generated_register(xctx);

    return &impl_extension;
}

/*
 * Implementation of method release for interface afw_extension.
 */
void
impl_afw_extension_release(
    AFW_EXTENSION_SELF_T *self,
    afw_xctx_t * xctx)
{
    (void)self;

    /* Extension release() is not currently used; best-effort cleanse. */
    afw_crypto_internal_keystore_shutdown(xctx);
}
