// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework CURL Internal Header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_CURL_INTERNAL_H__
#define __AFW_CURL_INTERNAL_H__


/**
 * @file afw_curl_internal.h
 * @brief Adaptive Framework CURL Internal Header  
 */

#include "afw_minimal.h"
#include "afw_curl.h"
#include "generated/afw_curl_generated.h"

#include <apr_buckets.h>


#ifdef __cplusplus
extern "C" {
#endif

/* holds info for adaptive script callback routines */
typedef struct afw_curl_internal_script_cb_s {
    const afw_value_t   * callback;
    const afw_value_t   * userData;
    const afw_value_t   * call;
    const afw_value_t   * argv[4];
} afw_curl_internal_script_cb_t;

/* callback structs for writing responses and reading requests */
typedef struct afw_curl_internal_write_cb_s {
    apr_bucket_alloc_t              * allocator;
    apr_bucket_brigade              * response;
    afw_curl_internal_script_cb_t   * header;
    afw_curl_internal_script_cb_t   * writer;
    const afw_pool_t                * pool;
    afw_xctx_t                      * xctx;
} afw_curl_internal_write_cb_t;

typedef struct afw_curl_internal_read_cb_s {
    const afw_utf8_t                * payload;
    size_t                            bytes_sent;
    afw_curl_internal_script_cb_t   * header;
    afw_curl_internal_script_cb_t   * reader;
    const afw_pool_t                * pool;
    afw_xctx_t                      * xctx;
} afw_curl_internal_read_cb_t;

typedef struct afw_curl_cb_ctx_s {
    const afw_pool_t *p;
    const afw_value_t *callback;
    const afw_value_t *userData;
    const afw_value_t *call;
    const afw_value_t *argv[4];
} afw_curl_cb_ctx_t;


const afw_object_t *
afw_curl_internal_http_post(
    const afw_utf8_t        * url,
    const afw_utf8_t        * payload,
    const afw_array_t       * headers,
    const afw_object_t      * options,
    const afw_pool_t        * pool,
    afw_xctx_t              * xctx);

const afw_object_t *
afw_curl_internal_http_get(
    const afw_utf8_t        * url,
    const afw_array_t       * headers,
    const afw_object_t      * options,
    afw_curl_internal_script_cb_t * header_reader,
    afw_curl_internal_script_cb_t * reader,
    const afw_pool_t        * pool,
    afw_xctx_t              * xctx);

const afw_object_t *
afw_curl_internal_http_delete(
    const afw_utf8_t        * url,
    const afw_array_t       * headers,
    const afw_object_t      * options,
    const afw_pool_t        * pool,
    afw_xctx_t              * xctx);

const afw_object_t *
afw_curl_internal_http_put(
    const afw_utf8_t        * url,
    const afw_utf8_t        * payload,
    const afw_array_t       * headers,
    const afw_object_t      * options,
    const afw_pool_t        * pool,
    afw_xctx_t              * xctx);

const afw_object_t *
afw_curl_internal_http_patch(
    const afw_utf8_t        * url,
    const afw_utf8_t        * payload,
    const afw_array_t       * headers,
    const afw_object_t      * options,
    const afw_pool_t        * pool,
    afw_xctx_t              * xctx);

const afw_object_t *
afw_curl_internal_http_head(
    const afw_utf8_t        * url,
    const afw_array_t       * headers,
    const afw_object_t      * options,
    const afw_pool_t        * pool,
    afw_xctx_t              * xctx);

const afw_object_t *
afw_curl_internal_http_options(
    const afw_utf8_t        * url,
    const afw_array_t       * headers,
    const afw_object_t      * options,
    const afw_pool_t        * pool,
    afw_xctx_t              * xctx);

void 
afw_curl_internal_smtp_send(
    const afw_utf8_t        * url,
    const afw_utf8_t        * mail_from,
    const afw_array_t       * mail_recipients,
    const afw_utf8_t        * payload,
    const afw_object_t      * options,
    const afw_pool_t        * pool,
    afw_xctx_t              * xctx);


#ifdef __cplusplus
}
#endif

#endif /* __AFW_CURL_INTERNAL_H__ */
