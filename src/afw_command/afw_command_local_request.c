// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


#include "afw.h"
#include "afw_command_local_request.h"

/* Declares and rti/inf defines for interface afw_request */
#define AFW_IMPLEMENTATION_ID "afw_command_local"
#include "afw_request_impl_declares.h"
#include "afw_command_local_request.h"

/**
 * @addtogroup afw_command_internal
 * @{
 */

/**
 * @file afw_command_local_request.c
 * @brief Implementation for interface afw_command_local
 * @todo Search this file and its header for @todo and make appropriate changes.
 *
 * This is the implementation of afw_request for afw_command_local.
 */


/*
 * Implementation of method release for interface afw_request.
 */
void
impl_afw_request_release(
    const afw_request_t * instance,
    afw_xctx_t *xctx)
{
    /* Resource will be release when execution context (xctx) is released. */
}



/*
 * Implementation of method set_error_info for interface afw_request.
 */
void
impl_afw_request_set_error_info(
    const afw_request_t * instance,
    const afw_object_t * error_info,
    afw_xctx_t *xctx)
{
    afw_command_local_request_self_t *self =
        (afw_command_local_request_self_t *)instance;

    self->pub.error_info = error_info;
}



/*
 * Implementation of method read_raw_request_body for interface afw_request.
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
    afw_command_local_request_self_t *self =
        (afw_command_local_request_self_t *)instance;

    /* Make sure in correct state then set it. */
    if (self->state > afw_request_state_content_read) {
        AFW_THROW_ERROR_Z(general,
            "read_request_body() called out of order", xctx);
    }
    self->state = afw_request_state_content_read;

    *size = buffer_size;
    if (*size > self->remaining_body) {
        *size = self->remaining_body;
    }

    if (*size > 0) {
        memcpy(buffer,
            self->body->ptr + self->body->size - self->remaining_body,
            *size);
        self->remaining_body -= *size;
    }

    *more_to_read = self->remaining_body > 0;
}



/*
 * Implementation of method set_response_status_code for interface afw_request.
 */
void
impl_afw_request_set_response_status_code(
    const afw_request_t * instance,
    const afw_utf8_t * code,
    const afw_utf8_t * reason,
    afw_xctx_t *xctx)
{
    afw_command_local_request_self_t *self =
        (afw_command_local_request_self_t *)instance;
    
    /* Ignore status if already past state. */
    if (self->state >= afw_request_state_status_set) {
        return;
    }
    self->state = afw_request_state_status_set;

    /* Save reason and code */
    self->status_code = code;
    self->status_reason = reason;
}



/*
 * Implementation of method write_response_header for interface afw_request.
 */
void
impl_afw_request_write_response_header(
    const afw_request_t * instance,
    const afw_utf8_t * name,
    const afw_utf8_t * value,
    afw_xctx_t *xctx)
{
    afw_command_local_request_self_t *self =
        (afw_command_local_request_self_t *)instance;

    /* Make sure in correct state then set it. */
    if (self->state > afw_request_state_header_written) {
        AFW_THROW_ERROR_Z(general,
            "write_response_header() called out of order", xctx);
    }
    self->state = afw_request_state_header_written;

    /* Push name and value on response_headers array. */
    if (!self->response_headers) {
        self->response_headers = apr_array_make(
            afw_pool_get_apr_pool(xctx->p), 5, sizeof(afw_utf8_t *));
    }
    APR_ARRAY_PUSH(self->response_headers, const afw_utf8_t *) = name;
    APR_ARRAY_PUSH(self->response_headers, const afw_utf8_t *) = value;
}



/*
 * Implementation of method write_raw_response_body for interface afw_request.
 */
void
impl_afw_request_write_raw_response_body(
    const afw_request_t * instance,
    afw_size_t size,
    const void * buffer,
    afw_xctx_t *xctx)
{
    afw_command_local_request_self_t *self =
        (afw_command_local_request_self_t *)instance;

    /* Make sure in correct state. */
    if (self->state > afw_request_state_response_written) {
        AFW_THROW_ERROR_Z(general,
            "write_response_body() called out of order", xctx);
    }
    self->state = afw_request_state_response_written;

    /* Write response body. */
    afw_command_local_server_write_result(
        self->server_self,
        AFW_UTF8_FMT,
        (int)size, (const char *)buffer);
}



/*
 * Implementation of method flush_response for interface afw_request.
 */
void
impl_afw_request_flush_response(
    const afw_request_t * instance,
    afw_xctx_t *xctx)
{
    /* Nothing needs to be done since every write is flushed. */
}



/*
 * Implementation of method finish_response for interface afw_request.
 */
void
impl_afw_request_finish_response(
    const afw_request_t * instance,
    afw_xctx_t *xctx)
{
    afw_command_local_request_self_t *self =
        (afw_command_local_request_self_t *)instance;

    /* Make sure in correct state then set it. */
    if (self->state >= afw_request_state_response_finished) {
        AFW_THROW_ERROR_Z(general,
            "finish_response() called out of order", xctx);
    }
    self->state = afw_request_state_response_finished;

    /* Write end. */
    afw_command_local_server_write_end(self->server_self);
}



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



AFW_COMMAND_DECLARE_INTERNAL(const afw_request_t *)
afw_command_local_request_create(
    afw_command_local_server_self_t *server_self,
    const afw_memory_t *body,
    const afw_object_t *properties,
    afw_xctx_t *xctx)
{
    afw_command_local_request_self_t *self;
    const afw_pool_t *p = xctx->p;

    const afw_utf8_t *s;
    const afw_value_t *value;

    /* Allocate memory for self. */
    self = afw_xctx_calloc_type(afw_command_local_request_self_t, xctx);

    /* Initialize Self. */
    self->pub.inf = &impl_afw_request_inf;
    self->pub.xctx = xctx;
    self->server_self = server_self;
    self->pub.read_content_cb = impl_read_content_cb;
    self->pub.write_content_cb = impl_write_content_cb;
    self->pub.properties = properties;

    /* Payload. */
    self->pub.content_length = body->size;
    self->body = body;
    self->remaining_body = body->size;

    /* Method */
    self->pub.method = afw_object_old_get_property_as_string(
        properties, afw_s_REQUEST_METHOD, xctx);
    if (!self->pub.method) {
        self->pub.method = afw_s_POST;
        afw_object_set_property_as_string(
            properties, afw_s_REQUEST_METHOD, self->pub.method, xctx);      
    }

    /* Request URI. */
    self->pub.uri = afw_object_old_get_property_as_string(
        properties, afw_s_REQUEST_URI, xctx);
    if (!self->pub.uri) {
        self->pub.uri = afw_s_a_slash_afw;
        afw_object_set_property_as_string(
            properties, afw_s_REQUEST_URI, self->pub.uri, xctx);      
    }

    /* Content type. */
    self->pub.content_type = afw_object_old_get_property_as_string(
        properties, afw_s_CONTENT_TYPE, xctx);
    if (!self->pub.content_type) {
        self->pub.content_type = afw_object_old_get_property_as_string(
            properties, afw_s_CONTENT_TYPE, xctx);
        if (!self->pub.content_type) {
            self->pub.content_type = afw_s_a_application_json;
        }
        afw_object_set_property_as_string(
            properties, afw_s_CONTENT_TYPE, self->pub.content_type, xctx);      
    }

    /* Query string */
    self->pub.query_string = afw_object_old_get_property_as_string(
        properties, afw_s_QUERY_STRING, xctx);

    /* Overwrite CONTENT_LENGTH */
    value = afw_value_create_unmanaged_integer(
        (afw_integer_t)self->pub.content_length, p, xctx);
    s = afw_value_as_utf8(value, p, xctx);
    afw_object_set_property_as_string(
        properties, afw_s_CONTENT_LENGTH, s, xctx);

    /* accept */
    s = afw_object_old_get_property_as_string(properties, afw_s_ACCEPT, xctx);
    if (!s) {
        s =  afw_object_old_get_property_as_string(
            properties, afw_s_HTTP_ACCEPT, xctx);
        if (!s) {
            s =  afw_s_a_application_json;
        }
        afw_object_set_property_as_string(
            properties, afw_s_ACCEPT, s, xctx);
    }
    self->pub.accept = afw_utf8_parse_csv(s, p, xctx);

    return (const afw_request_t *)self;
}

/** @} */
