// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_curl_function_execute_* functions for http
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_curl_function_http.c
 * @brief Adaptive function execute implementations for HTTP (curl extension).
 *
 * Hand implementations of generate/objects function metadata for category
 * http. Declares live in generated function bindings; do not edit closet
 * skeletons for production.
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
 * Makes a HTTP POST Request.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * This function requires 'execute' access.
 *
 * Declaration:
 *
 * ```
 *   function http_post(
 *       url: string,
 *       payload?: string,
 *       headers?: array,
 *       options?: object // _AdaptiveCurlOptions_
 *   ): object; // _AdaptiveCurlHttpResponse_
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
        afw_function_execute_contextual(x),
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
 * This function requires 'execute' access.
 *
 * Declaration:
 *
 * ```
 *   function http_get(
 *       url: string,
 *       headers?: array,
 *       options?: object // _AdaptiveCurlOptions_
 *   ): object; // _AdaptiveCurlHttpResponse_
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
        afw_function_execute_contextual(x),
        x->p, x->xctx);

    return afw_value_create_unmanaged_object(result, x->p, x->xctx);
}



/*
 * Adaptive function: http_delete
 *
 * afw_curl_function_execute_http_delete
 *
 * See afw_curl_function_bindings.h for more information.
 *
 * Makes a HTTP DELETE Request.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * This function requires 'execute' access.
 *
 * Declaration:
 *
 * ```
 *   function http_delete(
 *       url: string,
 *       headers?: array,
 *       options?: object // _AdaptiveCurlOptions_
 *   ): object; // _AdaptiveCurlHttpResponse_
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
 *       response from the HTTP delete request.
 */
const afw_value_t *
afw_curl_function_execute_http_delete(
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

    /* use internal routine to do the DELETE */
    result = afw_curl_internal_http_delete(
        &url->internal,
        (headers ? headers->internal : NULL),
        (options ? options->internal : NULL),
        afw_function_execute_contextual(x),
        x->p, x->xctx);

    return afw_value_create_unmanaged_object(result, x->p, x->xctx);
}



/*
 * Adaptive function: http_patch
 *
 * afw_curl_function_execute_http_patch
 *
 * See afw_curl_function_bindings.h for more information.
 *
 * Makes a HTTP PATCH Request.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * This function requires 'execute' access.
 *
 * Declaration:
 *
 * ```
 *   function http_patch(
 *       url: string,
 *       payload?: string,
 *       headers?: array,
 *       options?: object // _AdaptiveCurlOptions_
 *   ): object; // _AdaptiveCurlHttpResponse_
 * ```
 *
 * Parameters:
 *
 *   url - (string) HTTP Url.
 *
 *   payload - (optional string) Data payload for PATCH.
 *
 *   headers - (optional array) HTTP Headers.
 *
 *   options - (optional object _AdaptiveCurlOptions_) CURL Options.
 *
 * Returns:
 *
 *   (object _AdaptiveCurlHttpResponse_) Returns an object describing the
 *       response from the HTTP PATCH request.
 */
const afw_value_t *
afw_curl_function_execute_http_patch(
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

    /* use internal routine to do the PATCH */
    result = afw_curl_internal_http_patch(
        &url->internal,
        &payload->internal,
        (headers ? headers->internal : NULL),
        (options ? options->internal : NULL),
        afw_function_execute_contextual(x),
        x->p, x->xctx);

    return afw_value_create_unmanaged_object(result, x->p, x->xctx);
}



/*
 * Adaptive function: http_put
 *
 * afw_curl_function_execute_http_put
 *
 * See afw_curl_function_bindings.h for more information.
 *
 * Makes a HTTP PUT Request.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * This function requires 'execute' access.
 *
 * Declaration:
 *
 * ```
 *   function http_put(
 *       url: string,
 *       payload?: string,
 *       headers?: array,
 *       options?: object // _AdaptiveCurlOptions_
 *   ): object; // _AdaptiveCurlHttpResponse_
 * ```
 *
 * Parameters:
 *
 *   url - (string) HTTP Url.
 *
 *   payload - (optional string) Data payload for PUT.
 *
 *   headers - (optional array) HTTP Headers.
 *
 *   options - (optional object _AdaptiveCurlOptions_) CURL Options.
 *
 * Returns:
 *
 *   (object _AdaptiveCurlHttpResponse_) Returns an object describing the
 *       response from the HTTP PUT request.
 */
const afw_value_t *
afw_curl_function_execute_http_put(
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

    /* use internal routine to do the PUT */
    result = afw_curl_internal_http_put(
        &url->internal,
        &payload->internal,
        (headers ? headers->internal : NULL),
        (options ? options->internal : NULL),
        afw_function_execute_contextual(x),
        x->p, x->xctx);

    return afw_value_create_unmanaged_object(result, x->p, x->xctx);
}



/*
 * Adaptive function: http_head
 *
 * afw_curl_function_execute_http_head
 *
 * See afw_curl_function_bindings.h for more information.
 *
 * Makes a HTTP HEAD Request.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * This function requires 'execute' access.
 *
 * Declaration:
 *
 * ```
 *   function http_head(
 *       url: string,
 *       headers?: array,
 *       options?: object // _AdaptiveCurlOptions_
 *   ): object; // _AdaptiveCurlHttpResponse_
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
 *       response from the HTTP head request.
 */
const afw_value_t *
afw_curl_function_execute_http_head(
    afw_function_execute_t *x)
{
    const afw_value_string_t     * url;
    const afw_value_array_t      * headers;
    const afw_value_object_t     * options;
    const afw_object_t           * result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(url, 1, string);

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

    /* use internal routine to do the HEAD */
    result = afw_curl_internal_http_head(
        &url->internal,
        (headers ? headers->internal : NULL),
        (options ? options->internal : NULL),
        afw_function_execute_contextual(x),
        x->p, x->xctx);

    return afw_value_create_unmanaged_object(result, x->p, x->xctx);
}



/*
 * Adaptive function: http_options
 *
 * afw_curl_function_execute_http_options
 *
 * See afw_curl_function_bindings.h for more information.
 *
 * Makes a HTTP OPTIONS Request.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * This function requires 'execute' access.
 *
 * Declaration:
 *
 * ```
 *   function http_options(
 *       url: string,
 *       headers?: array,
 *       options?: object // _AdaptiveCurlOptions_
 *   ): object; // _AdaptiveCurlHttpResponse_
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
 *       response from the HTTP OPTIONS request.
 */
const afw_value_t *
afw_curl_function_execute_http_options(
    afw_function_execute_t *x)
{
    const afw_value_string_t     * url;
    const afw_value_array_t      * headers;
    const afw_value_object_t     * options;
    const afw_object_t           * result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(url, 1, string);

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

    /* use internal routine to do the OPTIONS */
    result = afw_curl_internal_http_options(
        &url->internal,
        (headers ? headers->internal : NULL),
        (options ? options->internal : NULL),
        afw_function_execute_contextual(x),
        x->p, x->xctx);

    return afw_value_create_unmanaged_object(result, x->p, x->xctx);
}
