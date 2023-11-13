// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Request.
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_request.c
 * @brief AFW request Support
 */

#include "afw_internal.h"

/* Declares and rti/inf defines for interface afw_content_type */
#define AFW_IMPLEMENTATION_ID "request"
#include "afw_stream_impl_declares.h"


/* Self typedef for application/afw implementation of afw_stream. */
typedef struct
afw_request_response_body_raw_writer_self_s {
    afw_stream_t pub;

    /* Private implementation variables */
    const afw_request_t *request;

} afw_request_response_body_raw_writer_self_t;



/* Get response content type. */
AFW_DEFINE(void)
afw_request_get_response_content_type(
    const afw_request_t *instance,
    const afw_content_type_t **response_content_type,
    const afw_utf8_t **type,
    const afw_utf8_t **type_parameter,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *const *accept;

    *response_content_type = NULL;
    *type_parameter = NULL;

    /* If accept was not specified, default to application/json. */
    if (!instance->accept || !*instance->accept) {
        *type = AFW_JSON_S_CONTENT_TYPE;
        *response_content_type = afw_environment_get_content_type(
        AFW_JSON_S_CONTENT_TYPE, xctx);
    }

    /* It accept specified, find first first one with registered handler. */
    /*NOTE: This does not process wildcard or q= properly. */
    else {
        for (accept = instance->accept; accept && *accept; accept++) {
            if (!afw_utf8_compare(*accept, afw_s_a_star_slash_star)) {
                // if */* is specified, use default of application/json
                *type = AFW_JSON_S_CONTENT_TYPE;
                *response_content_type = afw_environment_get_content_type(
                    AFW_JSON_S_CONTENT_TYPE, xctx);
                break;
            } else {
                *response_content_type = afw_environment_get_content_type(
                    *accept, xctx);
                
                if (*response_content_type) {
                    *type = *accept;
                    break;
                }
            }
        }
    }

    /* If an accept was specified but none match, throw error. */
    if (!*response_content_type) {
        *response_content_type = afw_environment_get_content_type(
            AFW_JSON_S_CONTENT_TYPE, xctx);
        AFW_THROW_ERROR_Z(unsupported_accept, "Unsupported accept.", xctx);
    }
}


/* Get response content type. */
AFW_DEFINE(const afw_content_type_t *)
afw_request_prepare_response_content_type(
    const afw_request_t * instance, afw_xctx_t *xctx)
{
    afw_request_t *self = (afw_request_t *)instance;
    const afw_content_type_t *response_content_type;
    const afw_utf8_t *type;
    const afw_utf8_t *type_parameter;

    /* If already obtained, just return it. */
    if (instance->response_content_type) {
        return instance->response_content_type;
    }

    afw_request_get_response_content_type(instance,
        &response_content_type, &type, &type_parameter, xctx);

    /* Get content type. */
    self->response_content_type = response_content_type;

    /* Write response_content_type header. */
    afw_request_write_response_header(instance,
        afw_s_a_Content_dash_Type,
        type,
        xctx);

    /* Return content type. */
    return self->response_content_type;
}


/* Read a request body into a string in a specifed pool. */
AFW_DEFINE(const afw_utf8_t *)
afw_request_body_to_utf8(
    const afw_request_t * instance,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_utf8_z_t *buffer_z;
    afw_utf8_z_t *cursor_z;
    afw_boolean_t more;
    afw_size_t read;
    

    /** @fixme Put in support of chunked encoding. */
    if (instance->content_length <= 0) return NULL;

    /* Read request body into string and return. */
    buffer_z = afw_pool_calloc(p, instance->content_length, xctx);
    cursor_z = buffer_z;
    read = 0;
    do {
        cursor_z += read;
        afw_request_read_raw_request_body(instance,
            instance->content_length, cursor_z,
            &read, &more, xctx);
    } while (more);
    return afw_utf8_create(buffer_z, instance->content_length, p, xctx);
}

/* Read a request body to value a specifed pool. */
AFW_DEFINE(const afw_value_t *)
afw_request_body_to_value(
    const afw_request_t * instance,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *string;
    const afw_content_type_t *content_type;
    afw_utf8_t content_type_id;
    const afw_utf8_octet_t *s;
    const afw_utf8_octet_t *end;

    string = afw_request_body_to_utf8(instance, p, xctx);
    if (!string) return NULL;

    for (s = instance->content_type->s, end = s + instance->content_type->len;
        s < end && *s != ';' && *s != ' ';
        s++);
    
    content_type_id.s = instance->content_type->s;
    content_type_id.len = s - instance->content_type->s;

    /** @fixme should decode content type parameter (stuff after ;). */

    content_type = afw_environment_get_content_type(
        &content_type_id, xctx);
    if (!content_type) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "Unsupported content-type " AFW_UTF8_FMT_Q ".",
            AFW_UTF8_FMT_ARG(instance->content_type));
    }

    return afw_content_type_raw_to_value(content_type, 
        afw_utf8_as_raw(string, xctx->p, xctx), afw_s_a_request_body,
        xctx->p, xctx);
}

/* Write value to response body. */
AFW_DECLARE(void)
afw_request_write_value_to_response_body(
    const afw_request_t * instance,
    const afw_value_t *value,
    const afw_object_options_t *options,
    afw_xctx_t *xctx)
{
    const afw_memory_t *raw;
    const afw_content_type_t *response_content_type;
    const afw_stream_t *writer;
 
    /* Get response content type. */
    response_content_type = afw_request_prepare_response_content_type(instance, xctx);

    /* If closed, return. */
    if (instance->is_closed) {
        return;
    }

    /* Write encoded content. */
    raw = afw_content_type_value_to_raw(response_content_type, value, options,
        xctx->p, xctx);
    writer = afw_stream_standard(response_body, xctx);
    writer->write_cb((void *)writer, raw->ptr, raw->size, xctx->p, xctx);
    afw_stream_flush(writer, xctx);
}


/* Write xctx error to response body. */
AFW_DEFINE(void)
afw_request_write_error_to_response_body(
    const afw_request_t * instance,
    const afw_utf8_t *code,
    const afw_error_t *error,
    afw_xctx_t *xctx)
{
    const afw_object_t *response;
    const afw_value_t *value;
    const afw_object_t *err;

    afw_request_set_response_status_code(instance, code, NULL, xctx);

    /* If response should not include error object, skip writing it. */
    if (!afw_error_allow_in_response(error->code)) {
        ((afw_request_t *)instance)->is_closed = true;
        return;
    }

    response = instance->error_info;
    if (!response) {
        response = afw_object_create_managed(xctx->p, xctx);
    }
    afw_object_set_property_as_string(response, afw_s_status, afw_s_error, xctx);
    err = afw_object_create_embedded(response, afw_s_error, xctx);
    afw_error_add_to_object(err, error, xctx);
    value = afw_value_create_unmanaged_object(response, xctx->p, xctx);
    afw_request_write_value_to_response_body(instance, value, NULL, xctx);
}


/* Write simple success to response body. */
AFW_DEFINE(void)
afw_request_write_success_response(
    const afw_request_t * instance,
    const afw_object_t * response,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;

    afw_request_set_response_status_code(instance, afw_s_200,
        afw_s_OK, xctx);

    /* If response is NULL, make empty one. */
    if (!response) {
        response = afw_object_create_managed(xctx->p, xctx);
    }

    /* Default status to success. */
    if (!afw_object_has_property(response, afw_s_status, xctx)) {
        afw_object_set_property_as_string(response, afw_s_status,
            afw_s_success, xctx);
    }

    /* Write response to body. */
    value = afw_value_create_unmanaged_object(response, xctx->p, xctx);
    afw_request_write_value_to_response_body(instance, value, NULL, xctx);
}



static afw_size_t
impl_response_write_cb(
    void *context,
    const void *buffer,
    afw_size_t size,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_request_response_body_raw_writer_self_t *self =
        (afw_request_response_body_raw_writer_self_t *)context;

    afw_request_write_raw_response_body(self->request, size, buffer, xctx);

    return size;
}

/*
 * Implementation of method release for interface afw_stream.
 */
void
impl_afw_stream_release(
    const afw_stream_t *instance,
    afw_xctx_t *xctx)
{
    afw_request_response_body_raw_writer_self_t *self =
        (afw_request_response_body_raw_writer_self_t *)instance;

    impl_afw_stream_flush(instance, xctx);
    afw_pool_release(self->pub.p, xctx);
}

/*
 * Implementation of method read for interface afw_stream.
 */
void
impl_afw_stream_read(
    const afw_stream_t *instance,
    const void *buffer,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);
}

/*
 * Implementation of method flush for interface afw_stream.
 */
void
impl_afw_stream_flush(
    const afw_stream_t *instance,
    afw_xctx_t *xctx)
{
    afw_request_response_body_raw_writer_self_t *self =
        (afw_request_response_body_raw_writer_self_t *)instance;

    afw_request_flush_response(self->request, xctx);
}

/*
 * Implementation of method write for interface afw_stream.
 */
void
impl_afw_stream_write(
    const afw_stream_t *instance,
    const void *buffer,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    if (size == 0) {
        return;
    }

    impl_response_write_cb((void *)instance, buffer, size,
        xctx->p, xctx);
}

AFW_DEFINE(const afw_stream_t *)
afw_request_response_body_raw_writer_create(
    const afw_request_t *request,
    afw_xctx_t *xctx)
{
    afw_request_response_body_raw_writer_self_t *self;

    self = afw_pool_calloc_type(xctx->p,
        afw_request_response_body_raw_writer_self_t, xctx);
    self->pub.inf = &impl_afw_stream_inf;
    self->pub.p = xctx->p;
    self->pub.streamId = afw_s_raw_response_body;
    self->pub.write_cb = &impl_response_write_cb;
    self->request = request;

    return (const afw_stream_t *)self;
}
