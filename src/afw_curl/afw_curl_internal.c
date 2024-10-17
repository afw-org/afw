// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_curl_internal.c  internal functions for curl
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_curl_internal.c
 * @brief afw_curl_internal* functions for curl.
 */

#include "afw.h"
#include "afw_curl_internal.h"

#include <curl/curl.h>
#include <apr_buckets.h>

size_t 
afw_curl_internal_write_response_headers(
    char    * ptr,
    size_t    size,
    size_t    nmemb,
    void    * userdata)
{
    /*
    afw_curl_internal_write_cb_t * appdata = 
        (afw_curl_internal_write_cb_t *) userdata;
    */
    size_t realsize = size * nmemb;


    return realsize;
}

size_t
afw_curl_internal_response_cb(
    void    * ptr,
    size_t    size,
    size_t    nmemb,
    void    * userdata)
{
    afw_curl_internal_write_cb_t * appdata = 
        (afw_curl_internal_write_cb_t *) userdata;
    size_t realsize = size * nmemb;
    apr_status_t rc;

    rc = apr_brigade_write(appdata->response,
        NULL, NULL, ptr, realsize);
    if (rc != APR_SUCCESS) {
        AFW_THROW_ERROR_Z(general, 
            "Error writing response to internal buffer.", 
            appdata->xctx);
    }

    return realsize;
}

size_t
afw_curl_internal_request_cb(
    void    * ptr,
    size_t    size,
    size_t    nmemb,
    void    * userdata)
{
    afw_curl_internal_read_cb_t * appdata =
        (afw_curl_internal_read_cb_t *) userdata;

    if ((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
        return 0;
    }

    if (appdata->bytes_sent == appdata->payload->len)
        return 0;
   
    memcpy(ptr, appdata->payload->s, appdata->payload->len); 
    appdata->bytes_sent = appdata->payload->len;

    return appdata->bytes_sent;;
}

afw_curl_internal_write_cb_t *
afw_curl_internal_response(
    CURL                * curl,
    const afw_pool_t    * pool,
    afw_xctx_t          * xctx)
{
    CURLcode res;
    afw_curl_internal_write_cb_t *appdata;
    
    appdata = afw_xctx_calloc_type(afw_curl_internal_write_cb_t, xctx);
    appdata->pool = pool;
    appdata->xctx = xctx;
    appdata->allocator = apr_bucket_alloc_create(
        afw_pool_get_apr_pool(appdata->pool));
    appdata->response = apr_brigade_create(
        afw_pool_get_apr_pool(appdata->pool), appdata->allocator);

    res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) appdata);
    if (res != CURLE_OK)
        AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting WRITEDATA.", xctx);

    res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, afw_curl_internal_response_cb);
    if (res != CURLE_OK)
        AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting WRITEFUNCTION.", xctx);

    res = curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void *)appdata);
    if (res != CURLE_OK)
        AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting HEADERDATA.", xctx);

    res = curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, afw_curl_internal_write_response_headers);
    if (res != CURLE_OK)
        AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting HEADERFUNCTION.", xctx);

    return appdata;
}

afw_curl_internal_read_cb_t *
afw_curl_internal_request(
    CURL                * curl,
    const afw_utf8_t    * payload,
    const afw_pool_t    * pool,
    afw_xctx_t          * xctx)
{
    CURLcode res;
    afw_curl_internal_read_cb_t *appdata;

    appdata = afw_xctx_calloc_type(afw_curl_internal_read_cb_t, xctx);
    appdata->pool = pool;
    appdata->xctx = xctx;
    appdata->payload = payload;
    appdata->bytes_sent = 0;

    res = curl_easy_setopt(curl, CURLOPT_READDATA, appdata);
    if (res != CURLE_OK)
        AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting READDATA.", xctx);

    res = curl_easy_setopt(curl, CURLOPT_READFUNCTION, afw_curl_internal_request_cb);
    if (res != CURLE_OK)
        AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting READFUNCTION.", xctx);

    res = curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    if (res != CURLE_OK)
        AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting UPLOAD.", xctx);

    return appdata;
}

/*
 * Common routine to set individual options that were specified by the user.
 */
void
afw_curl_internal_options(
    CURL                * curl,
    const afw_object_t  * options,
    const afw_pool_t    * pool,
    afw_xctx_t          * xctx)
{
    CURLcode res;
    afw_boolean_t found;
    afw_boolean_t sslVerifyHost, sslVerifyPeer, sslVerifyStatus;
    afw_boolean_t followLocation, verbose;
    const afw_utf8_t *proxy, *userPassword;
    const afw_utf8_t *awsSigv4, *caInfo, *caBlob, *caPath;

    /* process any additional options */
    if (options) {
        verbose = afw_object_old_get_property_as_boolean(options, afw_curl_s_verbose, &found, xctx);
        if (found && verbose == AFW_TRUE) {
            res = curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
            if (res != CURLE_OK)
                AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting verbose.", xctx);
        } else {
            res = curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
            if (res != CURLE_OK)
                AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting verbose.", xctx);
        }

        sslVerifyPeer = afw_object_old_get_property_as_boolean(options, afw_curl_s_sslVerifyPeer, &found, xctx);
        if (found && sslVerifyPeer == AFW_FALSE) {
            res = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
            if (res != CURLE_OK)
                AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting sslVerifyPeer.", xctx);
        }

        sslVerifyHost = afw_object_old_get_property_as_boolean(options, afw_curl_s_sslVerifyHost, &found, xctx);
        if (found && sslVerifyHost == AFW_FALSE) {
            res = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
            if (res != CURLE_OK)
                AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting sslVerifyHost.", xctx);
        }

        sslVerifyStatus = afw_object_old_get_property_as_boolean(options, afw_curl_s_sslVerifyStatus, &found, xctx);
        if (found && sslVerifyStatus == AFW_FALSE) {
#ifdef CURLOPT_SSL_VERIFYSTATUS
            res = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYSTATUS, 0);
            if (res != CURLE_OK)
                AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting sslVerifyStatus.", xctx);
#else
            AFW_THROW_ERROR_Z(general, "libcurl was not compiled with support for sslVerifyStatus.", xctx);
#endif
        }

        followLocation = afw_object_old_get_property_as_boolean(options, afw_curl_s_followLocation, &found, xctx);
        if (found && followLocation == AFW_TRUE) {
            res = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            if (res != CURLE_OK)
                AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting followLocation.", xctx);
        }

        proxy = afw_object_old_get_property_as_string(options, afw_curl_s_proxy, xctx);
        if (proxy) {
            res = curl_easy_setopt(curl, CURLOPT_PROXY, afw_utf8_to_utf8_z(proxy, xctx->p, xctx));
            if (res != CURLE_OK)
                AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting proxy.", xctx);
        }

        userPassword = afw_object_old_get_property_as_string(options, afw_curl_s_userPassword, xctx);
        if (userPassword) {
            res = curl_easy_setopt(curl, CURLOPT_USERPWD, afw_utf8_to_utf8_z(userPassword, xctx->p, xctx));
            if (res != CURLE_OK)
                AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting userPassword.", xctx);
        }

        awsSigv4 = afw_object_old_get_property_as_string(options, afw_curl_s_awsSigv4, xctx);
        if (awsSigv4) {
            res = curl_easy_setopt(curl, CURLOPT_AWS_SIGV4, afw_utf8_to_utf8_z(awsSigv4, xctx->p, xctx));
            if (res != CURLE_OK)
                AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting awsSigv4.", xctx);
        }

        caInfo = afw_object_old_get_property_as_string(options, afw_curl_s_caInfo, xctx);
        if (caInfo) {
            res = curl_easy_setopt(curl, CURLOPT_CAINFO, afw_utf8_to_utf8_z(caInfo, xctx->p, xctx));
            if (res != CURLE_OK)
                AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting caInfo.", xctx);
        }

        caBlob = afw_object_old_get_property_as_string(options, afw_curl_s_caBlob, xctx);
        if (caBlob) {
            res = curl_easy_setopt(curl, CURLOPT_CAINFO_BLOB, afw_utf8_to_utf8_z(caBlob, xctx->p, xctx));
            if (res != CURLE_OK)
                AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting caBlob.", xctx);
        }

        caPath = afw_object_old_get_property_as_string(options, afw_curl_s_caPath, xctx);
        if (caPath) {
            res = curl_easy_setopt(curl, CURLOPT_CAPATH, afw_utf8_to_utf8_z(caPath, xctx->p, xctx));
            if (res != CURLE_OK)
                AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting caPath.", xctx);
        }


        /** @fixme set many more in the object type definition */

    }
}

afw_memory_t *
afw_curl_internal_parse_response(
    afw_curl_internal_write_cb_t    * response,
    const afw_pool_t                * pool,
    afw_xctx_t                      * xctx)
{
    afw_memory_t * response_body = NULL;

    if (response && response->response) {
        apr_off_t len;
        apr_size_t size;

        response_body = afw_xctx_calloc_type(afw_memory_t, xctx);

        /* the response field is an APR Bucket Brigade that needs to be flattened */
        apr_brigade_length(response->response, 1, &len);
        size = (apr_size_t) len;
        response_body->ptr = apr_palloc(afw_pool_get_apr_pool(pool), size);
        response_body->size = size;

        apr_brigade_flatten(response->response, (char *)response_body->ptr, &size);
    }

    return response_body;
}

/*
 * Internal routine for doing a simple HTTP POST request.
 */
const afw_object_t *
afw_curl_internal_http_post(
    const afw_utf8_t        * url,
    const afw_utf8_t        * payload,
    const afw_array_t       * headers,
    const afw_object_t      * options,
    const afw_pool_t        * pool,
    afw_xctx_t              * xctx)
{
    CURL *curl = NULL;
    CURLcode res;
    const afw_object_t * result = NULL;
    struct curl_slist *curl_headers = NULL;
    afw_curl_internal_write_cb_t * response = NULL;
    long response_code;
    const afw_value_t *value;
    const afw_iterator_t *header_iterator;
    afw_memory_t * response_body;
    afw_utf8_t * encoded_response;
    char *errbuf;

    curl = curl_easy_init();
    AFW_TRY {
        if (!curl) {
            AFW_THROW_ERROR_Z(general, "Error in curl_easy_init().", xctx);
        }

        /* First, set the common, required options. */
        res = curl_easy_setopt(curl, CURLOPT_URL, afw_utf8_to_utf8_z(url, xctx->p, xctx));
        if (res != CURLE_OK)
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt()", xctx);

        res = curl_easy_setopt(curl, CURLOPT_POST, 1L);
        if (res != CURLE_OK)
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt()", xctx);

        res = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, payload->len);
        if (res != CURLE_OK)
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt()", xctx);

        res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload->s);
        if (res != CURLE_OK)
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt()", xctx);

        /* Now, send any optional headers across. */
        if (headers) {
            /* iterate through each header in our list */
            header_iterator = NULL;
            value = afw_array_get_next_value(headers, &header_iterator, pool, xctx);
            while (value) {
                const afw_utf8_z_t *next_header;

                next_header  = afw_value_as_utf8_z(value, pool, xctx);
                curl_headers = curl_slist_append(curl_headers, next_header);
                value = afw_array_get_next_value(headers, &header_iterator, pool, xctx);
            }

            res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_headers);
            if (res != CURLE_OK)
                AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt()", xctx);
        }

        /* set any options, that may have been specified */
        afw_curl_internal_options(curl, options, pool, xctx);

        /* set the error buffer as empty before performing a request */
        errbuf = afw_pool_calloc(pool, CURL_ERROR_SIZE, xctx);

        /* set handling of more detailed error messages */
        res = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);

        /* setup our response callbacks to handle data send back from the server */
        response = afw_curl_internal_response(curl, pool, xctx);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK) {
            size_t len = strlen(errbuf);
            if (len > 0) {
                AFW_THROW_ERROR_RV_Z(general, curl, res, errbuf, xctx);
            } else {
                AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_perform()", xctx);
            }
        }
    
        /* create a result object */ 
        result = afw_object_create(pool, xctx);

        /* set the response code */
        res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        if (res != CURLE_OK) 
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_getinfo()", xctx);

        afw_object_set_property_as_integer(result, afw_curl_s_response_code, response_code, xctx);

        /* parse the response body and attach it, too */
        response_body = afw_curl_internal_parse_response(response, pool, xctx);
        if (response_body && response_body->size) {
            encoded_response = afw_pool_calloc_type(pool, afw_utf8_t, xctx);
            afw_memory_encode_base64(encoded_response, response_body, pool, xctx);

            afw_object_set_property_as_string(result,
                afw_curl_s_response, encoded_response, xctx);
        }
    }

    AFW_FINALLY {

        /* free up headers list */
        if (curl_headers)
            curl_slist_free_all(curl_headers);

        /* cleanup curl resources */
        if (curl)
            curl_easy_cleanup(curl);
    }

    AFW_ENDTRY;

    return result;
}

/*
 * Internal routine for doing a simple HTTP GET request.
 */
const afw_object_t *
afw_curl_internal_http_get(
    const afw_utf8_t    * url,
    const afw_array_t   * headers,
    const afw_object_t  * options,
    const afw_pool_t    * pool,
    afw_xctx_t          * xctx)
{
    CURL *curl = NULL;
    CURLcode res;
    const afw_object_t * result = NULL;
    struct curl_slist * curl_headers = NULL;
    afw_curl_internal_write_cb_t * response = NULL;
    long response_code;
    const afw_value_t * value;
    const afw_iterator_t * header_iterator;
    afw_memory_t * response_body;
    afw_utf8_t * encoded_response;
    char *errbuf;

    curl = curl_easy_init();
    AFW_TRY {
        if (!curl)
            AFW_THROW_ERROR_Z(general, "Error in curl_easy_init().", xctx);

        /* First, set the common, required options. */
        res = curl_easy_setopt(curl, CURLOPT_URL, afw_utf8_to_utf8_z(url, xctx->p, xctx));
        if (res != CURLE_OK) {
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt()", xctx);
        }

        /* Now, send any optional headers across. */
        if (headers) {
            /* iterate through each header in our list */
            header_iterator = NULL;
            value = afw_array_get_next_value(headers, &header_iterator, pool, xctx);
            while (value) {
                const afw_utf8_z_t *next_header;

                next_header  = afw_value_as_utf8_z(value, pool, xctx);
                curl_headers = curl_slist_append(curl_headers, next_header);
                value = afw_array_get_next_value(headers, &header_iterator, pool, xctx);
            }

            res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_headers);
            if (res != CURLE_OK) {
                AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt()", xctx);
            }
        }

        /* set any options, that may have been specified */
        afw_curl_internal_options(curl, options, pool, xctx);

        /* set the error buffer as empty before performing a request */
        errbuf = afw_pool_calloc(pool, CURL_ERROR_SIZE, xctx);

        /* set handling of more detailed error messages */
        res = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);

        /* setup our response callbacks to handle data send back from the server */
        response = afw_curl_internal_response(curl, pool, xctx);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if (res != CURLE_OK) {
            size_t len = strlen(errbuf);
            if (len > 0) {
                AFW_THROW_ERROR_RV_Z(general, curl, res, errbuf, xctx);
            } else {
                AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_perform()", xctx);
            }
        }

        /* create a result object */
        result = afw_object_create(pool, xctx);

        /* set the response code */
        res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        if (res != CURLE_OK) {
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_getinfo()", xctx);
        }

        afw_object_set_property_as_integer(result, afw_curl_s_response_code, response_code, xctx);

        /* parse the response body and attach it, too */
        response_body = afw_curl_internal_parse_response(response, pool, xctx);
        if (response_body && response_body->size) {
            encoded_response = afw_pool_calloc_type(pool, afw_utf8_t, xctx);
            afw_memory_encode_base64(encoded_response, response_body, pool, xctx);

            afw_object_set_property_as_string(result,
                afw_curl_s_response, encoded_response, xctx);
        }
    }

    AFW_FINALLY {

        /* free up headers list */
        if (curl_headers)
            curl_slist_free_all(curl_headers);

        /* cleanup curl resources */
        if (curl)
            curl_easy_cleanup(curl);
    }

    AFW_ENDTRY;

    return result;
}


void
afw_curl_internal_smtp_send(
    const afw_utf8_t        * url,
    const afw_utf8_t        * mail_from,
    const afw_array_t       * mail_recipients,
    const afw_utf8_t        * payload,
    const afw_object_t      * options,
    const afw_pool_t        * pool,
    afw_xctx_t              * xctx)
{
    CURL * curl = NULL;
    CURLcode res = CURLE_OK;
    struct curl_slist * recipients = NULL;
    const afw_iterator_t * iterator;
    const afw_value_t * value;

    curl = curl_easy_init();
    AFW_TRY {
        if (!curl)
            AFW_THROW_ERROR_Z(general, "Error in curl_easy_init().", xctx);

        /* URL looks like:  smtp://mail.example.com */
        res = curl_easy_setopt(curl, CURLOPT_URL, afw_utf8_to_utf8_z(url, xctx->p, xctx));
        if (res != CURLE_OK)
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt()", xctx);

        /* set the required (by us) from address */
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, afw_utf8_to_utf8_z(mail_from, xctx->p, xctx));
        if (res != CURLE_OK)
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt()", xctx);

        /* Add recipients (TO:, followed by CC:) */
        iterator = NULL;
        value = afw_array_get_next_value(mail_recipients, &iterator, pool, xctx);
        while (value) {
            const afw_utf8_z_t * recipient;

            recipient = afw_value_as_utf8_z(value, pool, xctx);
            recipients = curl_slist_append(recipients, recipient);
            value = afw_array_get_next_value(mail_recipients, &iterator, pool, xctx);
        }

        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
        
        /* set any options, that may have been specified */
        afw_curl_internal_options(curl, options, pool, xctx);

        /* send the body of the email, using a callback */
        afw_curl_internal_request(curl, payload, pool, xctx);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_perform()", xctx);

    }

    AFW_FINALLY {

        /* free up recipients list */
        if (recipients)
            curl_slist_free_all(recipients);

        /* cleanup curl resources */
        if (curl)
            curl_easy_cleanup(curl);
    }

    AFW_ENDTRY;
}
