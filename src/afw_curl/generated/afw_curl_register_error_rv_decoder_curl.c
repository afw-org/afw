// See the 'COPYING' file in the project root for licensing information.
/*
 * Register error_rv_decoder `curl` for afw_curl
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_curl_register_error_rv_decoder_curl.c
 * @brief Generated environment register for error_rv_decoder `curl` (afw_curl).
 */

#include "afw.h"
#include "afw_curl_generated_internal.h"

#include <curl/curl.h>

/* CURL RV decoder */
static const afw_utf8_z_t *
impl_rv_decoder_z_curl(int rv,
    afw_utf8_z_t *wa, afw_size_t wa_size)
{
    return curl_easy_strerror(rv);
}


/* Register curl RV decoder. */
void
afw_curl_error_rv_decoder_curl_register(afw_xctx_t *xctx)
{
    afw_environment_register_error_rv_decoder(
        afw_curl_s_curl, impl_rv_decoder_z_curl, xctx);
}
