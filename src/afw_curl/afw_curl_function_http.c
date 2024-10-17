// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_curl_function_execute_* functions for http
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_curl_function_http.c
 * @brief afw_curl_function_execute_* functions for http.
 */

#include "afw.h"
#include "afw_curl_internal.h"

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
    const afw_value_string_t     * url;
    const afw_value_string_t     * payload;
    const afw_value_array_t      * headers;
    const afw_value_object_t     * options;
    const afw_object_t           * result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(url, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(payload, 2, string);

    /* Optional headers */
    headers = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(headers, 3, array);
    }

    /* Optional options */
    options = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(4)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(options, 4, object);
    }

    /* use internal routine to do the POST */
    result = afw_curl_internal_http_post(
        &url->internal,
        &payload->internal,
        (headers ? headers->internal : NULL),
        (options ? options->internal : NULL),
        x->p, x->xctx);

    return afw_value_create_unmanaged_object(result, x->p, x->xctx);
}



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
    const afw_value_string_t    * url;
    const afw_value_array_t     * headers;
    const afw_value_object_t    * options;
    const afw_object_t          * result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(url, 1, string);

    /* Optional headers */
    headers = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(headers, 2, array);
    }

    /* Optional options */
    options = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(options, 3, object);
    }

    /* use internal routine to do the GET */
    result = afw_curl_internal_http_get(
        &url->internal,
        (headers ? headers->internal : NULL),
        (options ? options->internal : NULL),
        x->p, x->xctx);

    return afw_value_create_unmanaged_object(result, x->p, x->xctx);
}
