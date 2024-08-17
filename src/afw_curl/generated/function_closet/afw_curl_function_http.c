// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_curl_function_execute_* functions for http
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_curl_function_http.c
 * @brief afw_curl_function_execute_* functions for http.
 */

#include "afw.h"



/*
 * Adaptive function: http_get
 *
 * afw_curl_function_execute_http_get
 *
 * See afw_curl_function_bindings.h for more information.
 *
 * Makes a HTTP GET Request.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function http_get(
 *       url: string,
 *       headers?: array,
 *       options?: (object _AdaptiveCurlOptions_)
 *   ): (object _AdaptiveCurlHttpResponse_);
 * ```
 *
 * Parameters:
 *
 *   url - (string) HTTP Url.
 *
 *   headers - (optional array) HTTP Headers.
 *
 *   options - (optional object _AdaptiveCurlOptions_) CURL Options.
 *
 * Returns:
 *
 *   (object _AdaptiveCurlHttpResponse_) Returns an object describing the
 *       response from the HTTP GET request.
 */
const afw_value_t *
afw_curl_function_execute_http_get(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: http_post
 *
 * afw_curl_function_execute_http_post
 *
 * See afw_curl_function_bindings.h for more information.
 *
 * Makes a HTTP Post Request.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function http_post(
 *       url: string,
 *       payload?: string,
 *       headers?: array,
 *       options?: (object _AdaptiveCurlOptions_)
 *   ): (object _AdaptiveCurlHttpResponse_);
 * ```
 *
 * Parameters:
 *
 *   url - (string) HTTP Url.
 *
 *   payload - (optional string) Data payload to POST to the url.
 *
 *   headers - (optional array) HTTP Headers.
 *
 *   options - (optional object _AdaptiveCurlOptions_) CURL Options.
 *
 * Returns:
 *
 *   (object _AdaptiveCurlHttpResponse_) Returns an object describing the
 *       response from the HTTP POST request.
 */
const afw_value_t *
afw_curl_function_execute_http_post(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
