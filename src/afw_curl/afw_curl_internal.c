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


/**
 * The libCURL API provides three callback functions for reading and writing:
 * 
 * 1. CURLOPT_WRITEFUNCTION: This callback is called to write the response data
 *    received from the server. The data is passed to this callback in chunks,
 *    and the callback is responsible for writing it to the appropriate 
 *    location.
 * 
 * 2. CURLOPT_READFUNCTION: This callback is called to read the request data
 *    that will be sent to the server. The data is read from this callback in
 *    chunks, and the callback is responsible for providing the data to be 
 *    sent.
 * 
 * 3. CURLOPT_HEADERFUNCTION: This callback is called to write the response
 *    headers received from the server. The headers are passed to this callback
 *    in chunks, and the callback is responsible for writing them to the
 *    appropriate location.
 * 
 * We can, by default, provide the callbacks internally and load data to and 
 * from the server using the apr_brigade_write() and apr_brigade_read() 
 * functions. This makes using the curl extension from Adaptive Script easy to 
 * use, but at the expense of increased memory usage. 
 * 
 * However, if the user wants to use their own callbacks, they can do so by 
 * passing them in as parameters from adaptive script, and we will use them 
 * instead of the internal ones.
 */

/**
 * afw_curl_internal_write_response_headers_cb()
 * 
 * This function is registered as the CURLOPT_HEADERFUNCTION callback for 
 * libCURL when no callback is provided by the user. It is called by libCURL 
 * to write the response headers received from the server to the internal 
 * buffer.
 */
size_t 
afw_curl_internal_write_response_headers_cb(
    char    * buffer,
    size_t    size,
    size_t    nitems,
    void    * userdata)
{
    /*
     * According to the libcurl documentation:
     *
     * This callback function gets invoked by libcurl as soon as it has 
     * received header data. The header callback is called once for each header 
     * and only complete header lines are passed on to the callback. Parsing 
     * headers is easy to do using this callback. buffer points to the 
     * delivered data, and the size of that data is nitems; size is always 1. 
     * The provided header line is not null-terminated. Do not modify the
     * passed in buffer.
     */
    afw_curl_internal_write_cb_t * appdata = 
        (afw_curl_internal_write_cb_t *) userdata;
    afw_curl_internal_script_cb_t * header = appdata->header;
    const afw_value_t *return_value;
    size_t realsize = 0;

    if (header && header->callback) 
    {
        header->argv[0] = header->callback;
        header->argv[1] = afw_value_create_unmanaged_string(
            afw_utf8_create_copy(buffer, nitems, appdata->pool, appdata->xctx),
            appdata->pool, appdata->xctx);
        header->argv[2] = header->userData;
        if (!header->call) {
            header->call = afw_value_call_create(NULL,
                2, &header->argv[0], false, appdata->pool, appdata->xctx);
        }
        return_value = afw_value_evaluate(header->call, 
            appdata->pool, appdata->xctx);

        realsize = afw_value_as_integer(return_value, appdata->xctx);
    }

    else 
    {
        afw_array_add_value(appdata->headers,
            afw_value_create_unmanaged_string(
                afw_utf8_create_copy(buffer, nitems, appdata->pool, appdata->xctx),
                appdata->pool, appdata->xctx),
            appdata->xctx
        );
        
        realsize = size * nitems;
    }

    return realsize;
}

/**
 * afw_curl_internal_response_cb()
 * 
 * This function is registered as the CURLOPT_WRITEFUNCTION callback for 
 * libCURL when no callback is provided by the user. It is called by libCURL to 
 * write data received from the server to the internal buffer.
 */
size_t
afw_curl_internal_response_cb(
    void    * ptr,
    size_t    size,
    size_t    nmemb,
    void    * userdata)
{
    /*
     * According to the libcurl documentation:
     *
     * This callback function gets called by libcurl as soon as there is data 
     * received that needs to be saved. For most transfers, this callback gets 
     * called many times and each invoke delivers another chunk of data. ptr 
     * points to the delivered data, and the size of that data is nmemb; size 
     * is always 1.
     * 
     * The data passed to this function is not null-terminated.
     */
    afw_curl_internal_write_cb_t * appdata = 
        (afw_curl_internal_write_cb_t *) userdata;
    afw_curl_internal_script_cb_t * writer = appdata->writer;
    afw_memory_t buf;
    const afw_value_t *return_value;
    size_t realsize = 0;
    apr_status_t rc;

    /* check to see if a callback was provided by adaptive script */
    if (writer && writer->callback) 
    {
        buf.ptr = ptr;
        buf.size = nmemb;

        writer->argv[0] = writer->callback;
        writer->argv[1] = afw_value_create_unmanaged_hexBinary(
            &buf, appdata->pool, appdata->xctx);
        writer->argv[2] = writer->userData;
        if (!writer->call) {
            writer->call = afw_value_call_create(NULL,
                2, &writer->argv[0], false, appdata->pool, appdata->xctx);
        }
        return_value = afw_value_evaluate(writer->call, 
            appdata->pool, appdata->xctx);
        realsize = afw_value_as_integer(return_value, appdata->xctx);
    }

    else 
    {
        realsize = size * nmemb;
        rc = apr_brigade_write(appdata->response,
            NULL, NULL, ptr, realsize);
        if (rc != APR_SUCCESS) {
            AFW_THROW_ERROR_Z(general, 
                "Error writing response to internal buffer.", 
                appdata->xctx);
        }
    }

    return realsize;
}

/**
 * afw_curl_internal_request_cb()
 * 
 * This function is registered as the CURLOPT_READFUNCTION callback for libCURL 
 * when no callback is provided by the user. It is called by libCURL to read 
 * data from the request payload and sent to the remote server. 
 */
size_t
afw_curl_internal_request_cb(
    void    * buffer,
    size_t    size,
    size_t    nitems,
    void    * userdata)
{
    /*
     * This callback function gets called by libcurl as soon as it needs to 
     * read data in order to send it to the peer - like if you ask it to 
     * upload or post data to the server. The data area pointed at by the 
     * pointer buffer should be filled up with at most size multiplied with 
     * nitems number of bytes by your function. size is always 1.
     * 
     * Your function must return the actual number of bytes that it stored in 
     * the data area pointed at by the pointer buffer. Returning 0 signals 
     * end-of-file to the library and causes it to stop the current transfer.
     * 
     * If you stop the current transfer by returning 0 "pre-maturely" (i.e 
     * before the server expected it, like when you have said you would upload 
     * N bytes and you upload less than N bytes), you may experience that the 
     * server "hangs" waiting for the rest of the data that is not sent.
     */
    afw_curl_internal_read_cb_t * appdata =
        (afw_curl_internal_read_cb_t *) userdata;
    afw_curl_internal_script_cb_t * reader = appdata->reader;
    const afw_value_t *return_value;
    const afw_memory_t *payload;

    if ((size == 0) || (nitems == 0) || ((size * nitems) < 1)) {
        return 0;
    }
    
    /* check to see if a callback was provided by adaptive script */
    if (reader && reader->callback)
    {
        reader->argv[0] = reader->callback;
        reader->argv[1] = reader->userData;
        reader->argv[2] = afw_value_create_unmanaged_integer(
            size, appdata->pool, appdata->xctx);
        reader->argv[3] = afw_value_create_unmanaged_integer(
            nitems, appdata->pool, appdata->xctx);
        if (!reader->call) {
            reader->call = afw_value_call_create(NULL,
                3, &reader->argv[0], false, appdata->pool, appdata->xctx);
        }
        return_value = afw_value_evaluate(reader->call, 
            appdata->pool, appdata->xctx);

        /* copy the bytes returned into buffer (null indicates end of data) */
        if (afw_value_is_null(return_value)) {
            return 0;
        }

        payload = afw_value_as_hexBinary(return_value, appdata->xctx);
        memcpy(buffer, payload->ptr, payload->size);
        
        return payload->size;
    }

    else
    {
        if (appdata->bytes_sent == appdata->payload->len)
            return 0;
    
        memcpy(buffer, appdata->payload->s, appdata->payload->len); 
        appdata->bytes_sent = appdata->payload->len;

        return appdata->bytes_sent;
    }
}
afw_curl_internal_write_cb_t *
afw_curl_internal_register_response_callbacks(
    CURL                * curl,
    afw_curl_internal_script_cb_t * header,
    afw_curl_internal_script_cb_t * writer,
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
    appdata->headers = afw_array_create_generic(pool, xctx);
    appdata->header = header;
    appdata->writer = writer;

    res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) appdata);
    if (res != CURLE_OK)
        AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting WRITEDATA.", xctx);

    res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, afw_curl_internal_response_cb);
    if (res != CURLE_OK)
        AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting WRITEFUNCTION.", xctx);

    res = curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void *)appdata);
    if (res != CURLE_OK)
        AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting HEADERDATA.", xctx);

    res = curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, afw_curl_internal_write_response_headers_cb);
    if (res != CURLE_OK)
        AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting HEADERFUNCTION.", xctx);

    return appdata;
}

afw_curl_internal_read_cb_t *
afw_curl_internal_register_request_callbacks(
    CURL                * curl,
    const afw_utf8_t    * payload,
    afw_curl_internal_script_cb_t * reader,
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
    appdata->reader = reader;

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
    afw_curl_internal_script_cb_t * reader,
    afw_curl_internal_script_cb_t * writer,
    afw_curl_internal_script_cb_t * header,
    const afw_pool_t    * pool,
    afw_xctx_t          * xctx)
{
    CURLcode res;
    afw_boolean_t found;
    afw_boolean_t sslVerifyHost, sslVerifyPeer, sslVerifyStatus;
    afw_boolean_t followLocation, verbose;
    const afw_utf8_t *proxy, *userPassword;
    const afw_utf8_t *awsSigv4, *caInfo, *caBlob, *caPath;
    const afw_value_t *readFunc, *writeFunc, *headerFunc;
    const afw_value_t *readUserData, *writeUserData, *headerUserData;
    

    /*
     * Process any additional options.
     * 
     * Note: it may be feasible in the future to use libCURL's curl_easy_option_by_name()
     * to set options by name, but they would require that the types that curl expects to
     * be mapped to the types that afw expects.  This is especially difficult for function 
     * types. For now, we will just set the options that we know about.
     */
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
#ifdef CURLOPT_AWS_SIGV4
            res = curl_easy_setopt(curl, CURLOPT_AWS_SIGV4, afw_utf8_to_utf8_z(awsSigv4, xctx->p, xctx));
            if (res != CURLE_OK)
                AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting awsSigv4.", xctx);
#else
            AFW_THROW_ERROR_Z(general, "libcurl was not compiled with support for awsSigv4.", xctx);
#endif
        }

        caInfo = afw_object_old_get_property_as_string(options, afw_curl_s_caInfo, xctx);
        if (caInfo) {
            res = curl_easy_setopt(curl, CURLOPT_CAINFO, afw_utf8_to_utf8_z(caInfo, xctx->p, xctx));
            if (res != CURLE_OK)
                AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting caInfo.", xctx);
        }

        caBlob = afw_object_old_get_property_as_string(options, afw_curl_s_caBlob, xctx);
        if (caBlob) {
#ifdef CURLOPT_CAINFO_BLOB
            struct curl_blob *blob;

            blob = afw_pool_malloc(xctx->p, sizeof(struct curl_blob), xctx);
            blob->data = (char *)afw_utf8_to_utf8_z(caBlob, xctx->p, xctx);
            blob->len = caBlob->len;    
            blob->flags = CURL_BLOB_COPY;

            res = curl_easy_setopt(curl, CURLOPT_CAINFO_BLOB, blob);
            if (res != CURLE_OK)
                AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting caBlob.", xctx);
#else
            AFW_THROW_ERROR_Z(general, "libcurl was not compiled with support for caBlob.", xctx);
#endif
        }

        caPath = afw_object_old_get_property_as_string(options, afw_curl_s_caPath, xctx);
        if (caPath) {
            res = curl_easy_setopt(curl, CURLOPT_CAPATH, afw_utf8_to_utf8_z(caPath, xctx->p, xctx));
            if (res != CURLE_OK)
                AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt() setting caPath.", xctx);
        }

        readFunc = afw_object_get_property(options, 
            afw_curl_s_readFunction, xctx);
        if (readFunc && reader) {
            reader->callback = readFunc;
        }

        readUserData = afw_object_get_property(options, 
            afw_curl_s_readUserData, xctx);
        if (readUserData && reader) {
            reader->userData = readUserData;
        }

        writeFunc = afw_object_get_property(options, 
            afw_curl_s_writeFunction, xctx);
        if (writeFunc && writer) {
            writer->callback = writeFunc;
        }

        writeUserData = afw_object_get_property(options, 
            afw_curl_s_writeUserData, xctx);
        if (writeUserData && writer) {
            writer->userData = writeUserData;
        }

        headerFunc = afw_object_get_property(options, 
            afw_curl_s_headerFunction, xctx);
        if (headerFunc && header) {
            header->callback = headerFunc;
        }

        headerUserData = afw_object_get_property(options, 
            afw_curl_s_headerUserData, xctx);
        if (headerUserData && header) {
            header->userData = headerUserData;
        }

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
    afw_curl_internal_script_cb_t * header = NULL;
    afw_curl_internal_script_cb_t * writer = NULL;
    afw_curl_internal_script_cb_t * reader = NULL;
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

        writer = afw_pool_calloc_type(pool,
            afw_curl_internal_script_cb_t, xctx);
        header = afw_pool_calloc_type(pool,
            afw_curl_internal_script_cb_t, xctx);
        reader = afw_pool_calloc_type(pool,
            afw_curl_internal_script_cb_t, xctx);

        /* set any options, that may have been specified */
        afw_curl_internal_options(curl, options, reader, writer, header, pool, xctx);

        /* set the error buffer as empty before performing a request */
        errbuf = afw_pool_calloc(pool, CURL_ERROR_SIZE, xctx);

        /* set handling of more detailed error messages */
        res = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
        if (res != CURLE_OK)
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt()", xctx);

        /* setup our response callbacks to handle data send back from the server */
        response = afw_curl_internal_register_response_callbacks(curl, 
            header, writer, pool, xctx);

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

        /* append the headers */
        if (response->headers) {
            afw_object_set_property_as_array(result,
                afw_curl_s_headers, response->headers, xctx);
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
    afw_curl_internal_script_cb_t * header = NULL;
    afw_curl_internal_script_cb_t * writer = NULL;
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

        writer = afw_pool_calloc_type(pool,
            afw_curl_internal_script_cb_t, xctx);
        header = afw_pool_calloc_type(pool,
            afw_curl_internal_script_cb_t, xctx);

        /* set any options, that may have been specified */
        afw_curl_internal_options(curl, options, NULL, writer, header, pool, xctx);

        /* set the error buffer as empty before performing a request */
        errbuf = afw_pool_calloc(pool, CURL_ERROR_SIZE, xctx);

        /* set handling of more detailed error messages */
        res = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
        if (res != CURLE_OK) {
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt()", xctx);
        }

        /* setup our response callbacks to handle data send back from the server */
        response = afw_curl_internal_register_response_callbacks(curl, 
            header, writer, pool, xctx);

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

        /* append the headers */
        if (response->headers) {
            afw_object_set_property_as_array(result,
                afw_curl_s_headers, response->headers, xctx);
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
 * Internal routine for doing a HTTP DELETE request.
 */
const afw_object_t *
afw_curl_internal_http_delete(
    const afw_utf8_t        * url,
    const afw_array_t       * headers,
    const afw_object_t      * options,
    const afw_pool_t        * pool,
    afw_xctx_t              * xctx)
{
    CURL *curl = NULL;
    CURLcode res;
    const afw_object_t *result = NULL;
    struct curl_slist *curl_headers = NULL;
    afw_curl_internal_write_cb_t * response = NULL;
    long response_code;
    const afw_value_t *value;
    const afw_iterator_t *header_iterator;
    afw_memory_t * response_body;
    afw_utf8_t * encoded_response;
    afw_curl_internal_script_cb_t * header = NULL;
    afw_curl_internal_script_cb_t * writer = NULL;
    char *errbuf;

    curl = curl_easy_init();
    AFW_TRY {
        if (!curl) {
            AFW_THROW_ERROR_Z(general, "Error in curl_easy_init().", xctx);
        }

        /* First, set the common, required options. */
        res = curl_easy_setopt(curl, CURLOPT_URL, afw_utf8_to_utf8_z(url, xctx->p, xctx));
        if (res != CURLE_OK)
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error setting CURLOPT_URL", xctx);

        res = curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        if (res != CURLE_OK)
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error setting CURLOPT_CUSTOMREQUEST", xctx);
        

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

        writer = afw_pool_calloc_type(pool,
            afw_curl_internal_script_cb_t, xctx);
        header = afw_pool_calloc_type(pool,
            afw_curl_internal_script_cb_t, xctx);

        /* set any options, that may have been specified */
        afw_curl_internal_options(curl, options, header, NULL, writer, pool, xctx);

        /* set the error buffer as empty before performing a request */
        errbuf = afw_pool_calloc(pool, CURL_ERROR_SIZE, xctx);

        /* set handling of more detailed error messages */
        res = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
        if (res != CURLE_OK)
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error in curl_easy_setopt()", xctx);

        /* setup our response callbacks to handle data send back from the server */
        response = afw_curl_internal_register_response_callbacks(curl, 
            NULL, NULL, pool, xctx);

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

        /* append the headers */
        if (response->headers) {
            afw_object_set_property_as_array(result,
                afw_curl_s_headers, response->headers, xctx);
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
 * Internal routine for doing a simple HTTP PUT request.
 */
const afw_object_t *
afw_curl_internal_http_put(
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
    afw_curl_internal_script_cb_t * header = NULL;
    afw_curl_internal_script_cb_t * writer = NULL;
    afw_curl_internal_script_cb_t * reader = NULL;
    char *errbuf;

    curl = curl_easy_init();
    AFW_TRY {
        if (!curl) {
            AFW_THROW_ERROR_Z(general, "Error in curl_easy_init().", xctx);
        }

        /* First, set the common, required options. */
        res = curl_easy_setopt(curl, CURLOPT_URL, afw_utf8_to_utf8_z(url, xctx->p, xctx));
        if (res != CURLE_OK)
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error setting CURLOPT_URL", xctx);

        res = curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        if (res != CURLE_OK)
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error setting CURLOPT_CUSTOMREQUEST", xctx);

        res = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, payload->len);
        if (res != CURLE_OK)
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error setting CURLOPT_POSTFIELDSIZE", xctx);

        res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload->s);
        if (res != CURLE_OK)
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error setting CURLOPT_POSTFIELDS", xctx);

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
                AFW_THROW_ERROR_RV_Z(general, curl, res, "Error setting CURLOPT_HTTPHEADER", xctx);
        }

        reader = afw_pool_calloc_type(pool,
            afw_curl_internal_script_cb_t, xctx);
        writer = afw_pool_calloc_type(pool,
            afw_curl_internal_script_cb_t, xctx);
        header = afw_pool_calloc_type(pool,
            afw_curl_internal_script_cb_t, xctx);

        /* set any options, that may have been specified */
        afw_curl_internal_options(curl, options, reader, writer, header, pool, xctx);

        /* set the error buffer as empty before performing a request */
        errbuf = afw_pool_calloc(pool, CURL_ERROR_SIZE, xctx);

        /* set handling of more detailed error messages */
        res = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
        if (res != CURLE_OK)
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error setting CURLOPT_ERRORBUFFER", xctx);

        /* setup our response callbacks to handle data send back from the server */
        response = afw_curl_internal_register_response_callbacks(curl, 
            NULL, NULL, pool, xctx);

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

        /* append the headers */
        if (response->headers) {
            afw_object_set_property_as_array(result,
                afw_curl_s_headers, response->headers, xctx);
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
 * Internal routine for doing a simple HTTP PATCH request.
 */
const afw_object_t *
afw_curl_internal_http_patch(
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
    afw_curl_internal_script_cb_t * header = NULL;
    afw_curl_internal_script_cb_t * writer = NULL;
    afw_curl_internal_script_cb_t * reader = NULL;
    char *errbuf;

    curl = curl_easy_init();
    AFW_TRY {
        if (!curl) {
            AFW_THROW_ERROR_Z(general, "Error in curl_easy_init().", xctx);
        }

        /* First, set the common, required options. */
        res = curl_easy_setopt(curl, CURLOPT_URL, afw_utf8_to_utf8_z(url, xctx->p, xctx));
        if (res != CURLE_OK)
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error setting CURLOPT_URL", xctx);

        res = curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
        if (res != CURLE_OK)
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error setting CURLOPT_CUSTOMREQUEST", xctx);

        res = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, payload->len);
        if (res != CURLE_OK)
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error setting CURLOPT_POSTFIELDSIZE", xctx);

        res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload->s);
        if (res != CURLE_OK)
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error setting CURLOPT_POSTFIELDS", xctx);

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
                AFW_THROW_ERROR_RV_Z(general, curl, res, "Error setting CURLOPT_HTTPHEADER", xctx);
        }

        writer = afw_pool_calloc_type(pool,
            afw_curl_internal_script_cb_t, xctx);
        header = afw_pool_calloc_type(pool,
            afw_curl_internal_script_cb_t, xctx);
        reader = afw_pool_calloc_type(pool,
            afw_curl_internal_script_cb_t, xctx);

        /* set any options, that may have been specified */
        afw_curl_internal_options(curl, options, reader, writer, header, pool, xctx);

        /* set the error buffer as empty before performing a request */
        errbuf = afw_pool_calloc(pool, CURL_ERROR_SIZE, xctx);

        /* set handling of more detailed error messages */
        res = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
        if (res != CURLE_OK)
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error setting CURLOPT_ERRORBUFFER", xctx);

        /* setup our response callbacks to handle data send back from the server */
        response = afw_curl_internal_register_response_callbacks(curl, 
            NULL, NULL, pool, xctx);

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

        /* append the headers */
        if (response->headers) {
            afw_object_set_property_as_array(result,
                afw_curl_s_headers, response->headers, xctx);
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

const afw_object_t *
afw_curl_internal_http_head(
    const afw_utf8_t        * url,
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
    afw_curl_internal_script_cb_t * header = NULL;
    afw_curl_internal_script_cb_t * writer = NULL;
    char *errbuf;

    curl = curl_easy_init();
    AFW_TRY {
        if (!curl) {
            AFW_THROW_ERROR_Z(general, "Error in curl_easy_init().", xctx);
        }

        /* First, set the common, required options. */
        res = curl_easy_setopt(curl, CURLOPT_URL, afw_utf8_to_utf8_z(url, xctx->p, xctx));
        if (res != CURLE_OK)
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error setting CURLOPT_URL", xctx);

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
                AFW_THROW_ERROR_RV_Z(general, curl, res, "Error setting CURLOPT_HTTPHEADER", xctx);
        }

        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);  // HEAD request

        writer = afw_pool_calloc_type(pool,
            afw_curl_internal_script_cb_t, xctx);
        header = afw_pool_calloc_type(pool,
            afw_curl_internal_script_cb_t, xctx);

        /* set any options, that may have been specified */
        afw_curl_internal_options(curl, options, header, NULL, writer, pool, xctx);

        /* set the error buffer as empty before performing a request */
        errbuf = afw_pool_calloc(pool, CURL_ERROR_SIZE, xctx);

        /* set handling of more detailed error messages */
        res = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
        if (res != CURLE_OK)
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error setting CURLOPT_ERRORBUFFER", xctx);

        /* setup our response callbacks to handle data send back from the server */
        response = afw_curl_internal_register_response_callbacks(curl, 
            NULL, NULL, pool, xctx);

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

        /* append the headers */
        if (response->headers) {
            afw_object_set_property_as_array(result,
                afw_curl_s_headers, response->headers, xctx);
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

const afw_object_t *
afw_curl_internal_http_options(
    const afw_utf8_t        * url,
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
    afw_curl_internal_script_cb_t * header = NULL;
    afw_curl_internal_script_cb_t * writer = NULL;
    char *errbuf;

    curl = curl_easy_init();
    AFW_TRY {
        if (!curl) {
            AFW_THROW_ERROR_Z(general, "Error in curl_easy_init().", xctx);
        }

        /* First, set the common, required options. */
        res = curl_easy_setopt(curl, CURLOPT_URL, afw_utf8_to_utf8_z(url, xctx->p, xctx));
        if (res != CURLE_OK)
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error setting CURLOPT_URL", xctx);

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
                AFW_THROW_ERROR_RV_Z(general, curl, res, "Error setting CURLOPT_HTTPHEADER", xctx);
        }

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "OPTIONS");

        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);  // HEAD request

        writer = afw_pool_calloc_type(pool,
            afw_curl_internal_script_cb_t, xctx);
        header = afw_pool_calloc_type(pool,
            afw_curl_internal_script_cb_t, xctx);

        /* set any options, that may have been specified */
        afw_curl_internal_options(curl, options, header, NULL, writer, pool, xctx);

        /* set the error buffer as empty before performing a request */
        errbuf = afw_pool_calloc(pool, CURL_ERROR_SIZE, xctx);

        /* set handling of more detailed error messages */
        res = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
        if (res != CURLE_OK)
            AFW_THROW_ERROR_RV_Z(general, curl, res, "Error setting CURLOPT_ERRORBUFFER", xctx);

        /* setup our response callbacks to handle data send back from the server */
        response = afw_curl_internal_register_response_callbacks(curl, 
            NULL, NULL, pool, xctx);

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

        /* append the headers */
        if (response->headers) {
            afw_object_set_property_as_array(result,
                afw_curl_s_headers, response->headers, xctx);
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
        afw_curl_internal_options(curl, options, NULL, NULL, NULL, pool, xctx);

        /* send the body of the email, using a callback */
        afw_curl_internal_register_request_callbacks(curl, payload, NULL, pool, xctx);

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
