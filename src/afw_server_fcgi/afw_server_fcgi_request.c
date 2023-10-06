// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_request interface
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_server_fcgi_request.c
 * @brief Implementation of afw_request interface for Fast CGI
 */

/* This header has to be first because it includes fcgi_stdio.h. */
#include "afw_server_fcgi_internal.h"
#include "afw.h"


/* Declares and rti/inf defines for interface afw_request */
#define AFW_IMPLEMENTATION_ID "afw_server_fcgi"
#define AFW_IMPLEMENTATION_SPECIFIC NULL /* Change to &label if needed. */
#include "afw_request_impl_declares.h"


static afw_size_t
impl_read_content_cb(
    void *context,
    const void * buffer,
    afw_size_t size,
    afw_boolean_t *more_to_read,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_size_t size_read;

    impl_afw_request_read_raw_request_body(
        (const afw_request_t *)context,
        size, (void *)buffer, &size_read, more_to_read, xctx);

    return size_read;
}


static afw_size_t
impl_write_content_cb(
    void *context,
    const void * buffer,
    afw_size_t size,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    impl_afw_request_write_raw_response_body(
        (const afw_request_t *)context, size, buffer, xctx);

    return size;
}


/* Create an adaptive object in memory. */
afw_server_fcgi_internal_request_t *
afw_server_fcgi_internal_create_request(
    afw_server_fcgi_internal_t *server,
    FCGX_Request *fcgx_request,
    afw_xctx_t *xctx)
{
    afw_server_fcgi_internal_request_t *self = NULL;
    const afw_value_t *value;
    const afw_utf8_z_t *length_z;
    const afw_utf8_octet_t *c;
    afw_size_t len;

    /* Allocate memory for self. */
    self = afw_xctx_calloc_type(afw_server_fcgi_internal_request_t, xctx);

    /* Initialize Self. */
    self->pub.inf = &impl_afw_request_inf;
    self->pub.xctx = xctx;
    self->fcgx_request = fcgx_request;
    self->server = server;
    self->pub.read_content_cb = impl_read_content_cb;
    self->pub.write_content_cb = impl_write_content_cb;

    /* Create properties object and request:: qualifier. */
    self->pub.properties = afw_server_fcgi_internal_create_properties_object(
        self, xctx);
    afw_runtime_xctx_set_object(self->pub.properties, true, xctx);
    afw_xctx_qualifier_stack_qualifier_object_push(afw_s_request, self->pub.properties,
        true, xctx->p, xctx);

    /* Set environment object and qualifier. */
    afw_runtime_xctx_set_object(server->environment_variables_object,
        true, xctx);
    afw_xctx_qualifier_stack_qualifier_object_push(afw_s_environment,
        server->environment_variables_object, true, xctx->p, xctx);   

    /* Get request method. */
    value = afw_object_get_property(self->pub.properties,
        AFW_REQUEST_s_PN_REQUEST_METHOD, xctx);
    if (!value) {
        AFW_THROW_ERROR_Z(general,
            AFW_REQUEST_Q_PN_REQUEST_METHOD " property required.", xctx);
    }
    self->pub.method = afw_value_as_utf8(value, xctx->p, xctx);

    /* Get request URI. */
    self->pub.uri = afw_object_old_get_property_as_utf8(self->pub.properties,
        AFW_REQUEST_s_PN_PATH_INFO, xctx->p, xctx);
    if (!self->pub.uri || self->pub.uri->len == 0) {
        self->pub.uri = afw_object_old_get_property_as_utf8(self->pub.properties,
            AFW_REQUEST_s_PN_REQUEST_URI, xctx->p, xctx);
        if (self->pub.uri && self->pub.uri->len != 0) {
            for (c = self->pub.uri->s, len = self->pub.uri->len;
                len > 0; c++, len--)
            {
                if (*c == '?') {
                    self->pub.uri = afw_utf8_create(
                        self->pub.uri->s, c - self->pub.uri->s,
                        xctx->p, xctx);
                }
            }
        }
    }
    if (!self->pub.uri || self->pub.uri->len == 0) {
        AFW_THROW_ERROR_Z(general,
            AFW_REQUEST_Q_PN_PATH_INFO " or " AFW_REQUEST_Q_PN_REQUEST_URI
            " property required", xctx);
    }

    /* Get request query string. */
    value = afw_object_get_property(self->pub.properties,
        AFW_REQUEST_s_PN_QUERY_STRING, xctx);
    if (!value) {
        AFW_THROW_ERROR_Z(general,
            AFW_REQUEST_Q_PN_QUERY_STRING " property required.", xctx);
    }
    self->pub.query_string = afw_value_as_utf8(value, xctx->p, xctx);

    /* Get request content type. */
    self->pub.content_type = afw_object_old_get_property_as_utf8(
        self->pub.properties,
        AFW_REQUEST_s_PN_CONTENT_TYPE, xctx->p, xctx);

    /* Get request content length. */
    value = afw_object_get_property(self->pub.properties,
        AFW_REQUEST_s_PN_CONTENT_LENGTH, xctx);
    if (value) {
        length_z = afw_value_as_utf8_z(value, xctx->p, xctx);
        self->pub.content_length = atoi(length_z);
    }

    /* Get request accept header. */
    value = afw_object_get_property(self->pub.properties,
        AFW_REQUEST_s_PN_HTTP_ACCEPT, xctx);
    if (value) {
        self->pub.accept = afw_utf8_parse_csv(
            afw_value_as_utf8(value, xctx->p, xctx),
            xctx->p, xctx);
    }

    /* Return new adaptor. */
    return self;
}



/*
 * Implementation of method release of interface afw_request.
 */
void
impl_afw_request_release(
    const afw_request_t * instance,
    afw_xctx_t *xctx)
{
    /*
     * Continue release, even if there is already an error.  Don't overwrite
     * existing error.
     */

    /* Resource will be release when execution context (xctx) is released. */
}



/*
 * Implementation of method set_error_info of interface afw_request.
 */
void
impl_afw_request_set_error_info(
    const afw_request_t * instance,
    const afw_object_t * error_info,
    afw_xctx_t *xctx)
{
    afw_server_fcgi_internal_request_t *self =
        (afw_server_fcgi_internal_request_t *)instance;

    self->pub.error_info = error_info;
}



/*
 * Implementation of method read_request_body of interface afw_request.
 */
void
impl_afw_request_read_raw_request_body(
    const afw_request_t * instance,
    afw_size_t buffer_size,
    void * buffer,
    afw_size_t * size,
    afw_boolean_t * more_to_read,
    afw_xctx_t *xctx)
{
    afw_server_fcgi_internal_request_t *self =
        (afw_server_fcgi_internal_request_t *)instance;
    int c;
    unsigned char *b = buffer;

    /* Make sure in correct state then set it. */
    if (self->state > afw_request_state_content_read) {
        AFW_THROW_ERROR_Z(general,
            "read_request_body() called out of order", xctx);
    }
    self->state = afw_request_state_content_read;


    /* Read into buffer until buffer full or EOF. */
    *more_to_read = 1;
    for (*size = 0; *size < buffer_size; (*size)++) {
        c = FCGX_GetChar(self->fcgx_request->in);
        if (c == EOF) {
            *more_to_read = 0;
            break;
        }
        *(b++) = c;
    }
}



/*
 * Implementation of method set_response_status_code of interface afw_request.
 */
void
impl_afw_request_set_response_status_code(
    const afw_request_t * instance,
    const afw_utf8_t * code,
    const afw_utf8_t * reason,
    afw_xctx_t *xctx)
{
    afw_server_fcgi_internal_request_t *self =
        (afw_server_fcgi_internal_request_t *)instance;
    int rv = 0;

    /* Ignore status if already past state. */
    if (self->state >= afw_request_state_status_set) {
        return;
    }
    self->state = afw_request_state_status_set;

    /*
     * Set response status. This is done in FastCGI by setting a "Status"
     * header that is examined but not sent to the client client.
     */
    if (reason) {
        rv = FCGX_FPrintF(self->fcgx_request->out,
            "Status:" AFW_UTF8_FMT " " AFW_UTF8_FMT
            AFW_CRLF,
            AFW_UTF8_FMT_ARG(code), AFW_UTF8_FMT_ARG(reason));
    }
    else {
        rv = FCGX_FPrintF(self->fcgx_request->out,
            "Status:" AFW_UTF8_FMT AFW_CRLF,
            AFW_UTF8_FMT_ARG(code));
    }
    if (rv == -1) {
        AFW_THROW_ERROR_Z(general,
            "FCGX_FPrintF() failed", xctx);
    }
}



/*
 * Implementation of method write_response_header of interface afw_request.
 */
void
impl_afw_request_write_response_header(
    const afw_request_t * instance,
    const afw_utf8_t * name,
    const afw_utf8_t * value,
    afw_xctx_t *xctx)
{
    afw_server_fcgi_internal_request_t *self =
        (afw_server_fcgi_internal_request_t *)instance;
    int rv;

    /* Make sure in correct state then set it. */
    if (self->state > afw_request_state_header_written) {
        AFW_THROW_ERROR_Z(general,
            "write_response_header() called out of order", xctx);
    }
    self->state = afw_request_state_header_written;

    /* Write response header */
    rv = FCGX_FPrintF(self->fcgx_request->out,
        AFW_UTF8_FMT ":" AFW_UTF8_FMT AFW_CRLF,
        AFW_UTF8_FMT_ARG(name), AFW_UTF8_FMT_ARG(value));
    if (rv == -1) {
        AFW_THROW_ERROR_Z(general,
            "FCGX_FPrintF() failed.", xctx);
    }
}



/*
 * Implementation of method write_response_body of interface afw_request.
 */
void
impl_afw_request_write_raw_response_body(
    const afw_request_t * instance,
    apr_size_t size,
    const void * buffer,
    afw_xctx_t *xctx)
{
    afw_server_fcgi_internal_request_t *self =
        (afw_server_fcgi_internal_request_t *)instance;
    apr_size_t remaining;
    const char *next;
    int rv;
    int n;

    /* Make sure in correct state. */
    if (self->state > afw_request_state_response_written) {
        AFW_THROW_ERROR_Z(general,
            "write_response_body() called out of order", xctx);
    }

    /* If this is the first write_response, write AFW_CRLF. */
    if (self->state < afw_request_state_response_written) {
        rv = FCGX_PutStr(AFW_CRLF, AFW_CRLF_STRLEN,
            self->fcgx_request->out);
        if (rv == -1) {
            AFW_THROW_ERROR_RV_Z(client_closed, fcgi, rv,
                "FCGX_PutStr() failed", xctx);
        }
    }

    /* Set state*/
    self->state = afw_request_state_response_written;

    /* Write response. */
    for (next = (const char *)buffer, remaining = size;
        remaining > 0;
        next = next + n,
        remaining = remaining - n)
    {
        n = (remaining > INT_MAX) ? INT_MAX : (int)remaining;
        rv = FCGX_PutStr(next, n, self->fcgx_request->out);
        if (rv == -1) {
            AFW_THROW_ERROR_RV_Z(client_closed, fcgi, rv,
                "FCGX_PutStr() failed", xctx);
        }
    }
}



/*
 * Implementation of method flush_response of interface afw_request.
 */
void
impl_afw_request_flush_response(
    const afw_request_t * instance,
    afw_xctx_t *xctx)
{
    afw_server_fcgi_internal_request_t *self =
        (afw_server_fcgi_internal_request_t *)instance;
    int rv;

    rv = FCGX_FFlush(self->fcgx_request->out);
    if (rv == -1) {
        AFW_THROW_ERROR_RV_Z(client_closed, fcgi, rv,
            "FCGX_FFlush() failed", xctx);
    }
}



/*
 * Implementation of method finish_response of interface afw_request.
 */
void
impl_afw_request_finish_response(
    const afw_request_t * instance,
    afw_xctx_t *xctx)
{
    int rv;

    afw_server_fcgi_internal_request_t *self =
        (afw_server_fcgi_internal_request_t *)instance;

    /* If there was no write_response, write AFW_CRLF. */
    if (self->state < afw_request_state_response_written) {
        rv = FCGX_PutStr(AFW_CRLF, AFW_CRLF_STRLEN,
            self->fcgx_request->out);
        if (rv == -1) {
            AFW_THROW_ERROR_RV_Z(client_closed, fcgi, rv,
                "FCGX_PutStr() failed", xctx);
        }
    }

    /* Make sure in correct state then set it. */
    if (self->state >= afw_request_state_response_finished) {
        AFW_THROW_ERROR_Z(general,
            "finish_response() called out of order", xctx);
    }
    self->state = afw_request_state_response_finished;

    /*
     * FCGX_FFlush documents that this is not necessary.  It is left here
     * commented out.  It might be needed for server-push.
     */
    rv = FCGX_FFlush(self->fcgx_request->out);
    if (rv == -1) {
        AFW_THROW_ERROR_RV_Z(client_closed, fcgi, rv,
            "FCGX_FFlush() failed", xctx);
    }
}
