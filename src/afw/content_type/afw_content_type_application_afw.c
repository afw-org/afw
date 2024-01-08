// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_content_type Implementation for <afwdev {srcdir}>
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_content_type_application_afw.c
 * @brief Implementation of afw_content_type interface for application/x-afw
 */

#include "afw_internal.h"

/* Declares and rti/inf defines for interface afw_content_type */
#define AFW_IMPLEMENTATION_ID "application_afw"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_content_type_application_afw_inf
#include "afw_content_type_impl_declares.h"
#undef AFW_IMPLEMENTATION_INF_SPECIFIER
#undef AFW_IMPLEMENTATION_INF_LABEL
#include "afw_stream_impl_declares.h"


/* Self typedef for application/x-afw implementation of afw_content_type. */
typedef struct
afw_content_type_application_afw_self_s {
    afw_content_type_t pub;

    /* Private implementation variables */

} afw_content_type_application_afw_self_t;



/*
 * Self typedef for application/x-afw implementation of
 * afw_content_type_object_list_writer.
 */
typedef struct
afw_content_type_application_afw_object_list_writer_self_s {
    afw_content_type_object_list_writer_t pub;

    /* Private implementation variables */

} afw_content_type_application_afw_object_list_writer_self_t;



/* Self typedef for application/afw implementation of afw_stream. */
typedef struct
afw_content_type_application_afw_stream_self_s {
    afw_stream_t pub;

    /* Private implementation variables */
    const afw_stream_t *response_writer;
    const afw_utf8_t *info;
    const afw_memory_writer_t *cache_writer;
    afw_boolean_t needs_leading_white_space;

} afw_content_type_application_afw_stream_self_t;




/* Media types supported by this afw_content_type implementation. */
static const afw_utf8_t impl_media_types[] = {
    AFW_UTF8_LITERAL("application/x-afw")
};


/* Raw begin object list. */
static const afw_memory_t impl_raw_begin_object_list = {
    (const afw_octet_t *)"[\n",
    sizeof("[\n") - 1
};

/* Raw object separator. */
static const afw_memory_t impl_raw_object_separator = {
    (const afw_octet_t *)",\n",
    sizeof(",\n") - 1
};

/* Raw last object separator. */
static const afw_memory_t impl_raw_last_object_separator = {
    (const afw_octet_t *)"\n",
    sizeof("\n") - 1
};

/* Raw end object list. */
static const afw_memory_t impl_raw_end_object_list = {
    (const afw_octet_t *)"]\n",
    sizeof("]\n") - 1
};



/* Content type singleton instance for this implementation. */
static const afw_content_type_t
impl_afw_content_type_application_afw =
{
    &afw_content_type_application_afw_inf,
    AFW_UTF8_LITERAL(AFW_IMPLEMENTATION_ID),
    sizeof(impl_media_types) / sizeof(afw_utf8_t),
    &impl_media_types[0]
};

    
/*
 * Implementation of method raw_to_value for interface afw_content_type.
 */
const afw_value_t *
impl_afw_content_type_raw_to_value(
    const afw_content_type_t * instance,
    const afw_memory_t * raw,
    const afw_utf8_t * source_location,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    /* Not supported for application/x-afw. */
    AFW_THROW_ERROR_Z(general,
        "afw_content_type_raw_to_value() is not supported for application/x-afw",
        xctx);

}

/*
 * Implementation of method raw_to_object for interface afw_content_type.
 */
const afw_object_t *
impl_afw_content_type_raw_to_object(
    const afw_content_type_t * instance,
    const afw_memory_t * raw,
    const afw_utf8_t * source_location,
    const afw_utf8_t * adaptor_id,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    afw_boolean_t cede_p,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    /* Not supported for application/x-afw. */
    AFW_THROW_ERROR_Z(general,
        "afw_content_type_raw_to_object() is not supported for application/x-afw",
        xctx);
}

/*
 * Implementation of method write_value for interface afw_content_type.
 */
void
impl_afw_content_type_write_value(
    const afw_content_type_t * instance,
    const afw_value_t * value,
    const afw_object_options_t * options,
    void * context,
    afw_write_cb_t callback,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    afw_json_internal_write_value(value, options, context, callback,
        p, xctx);
}

/*
 * Implementation of method create_object_list_writer for interface
 * afw_content_type.
 */
const afw_content_type_object_list_writer_t *
impl_afw_content_type_create_object_list_writer(
    const afw_content_type_t * instance,
    const afw_object_options_t * options,
    void * context,
    afw_write_cb_t callback,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    return afw_content_type_impl_create_object_list_writer(
        instance, options, context, callback,
        &impl_raw_begin_object_list,
        &impl_raw_object_separator,
        &impl_raw_last_object_separator,
        &impl_raw_end_object_list,
        p, xctx);
}



static afw_size_t
impl_afw_stream_write_cb(
    void *context,
    const void *buffer,
    afw_size_t size,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_content_type_application_afw_stream_self_t *self =
        (afw_content_type_application_afw_stream_self_t *)context;
    afw_size_t result;

    if (!self->cache_writer) {
        self->cache_writer = afw_memory_create_writer(p, xctx);
    }

    result = self->cache_writer->callback(self->cache_writer->context,
        buffer, size, p, xctx);

    return result;
}



/*
 * Implementation of method release for interface afw_stream.
 */
void
impl_afw_stream_release(
    const afw_stream_t *instance,
    afw_xctx_t *xctx)
{
    afw_content_type_application_afw_stream_self_t *self =
        (afw_content_type_application_afw_stream_self_t *)instance;

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
    afw_content_type_application_afw_stream_self_t *self =
        (afw_content_type_application_afw_stream_self_t *)instance;
    const afw_utf8_t *header;
    afw_size_t size;

    if (self->cache_writer) {
        size = afw_memory_writer_get_current_size(self->cache_writer, xctx);
        header = afw_utf8_printf(self->cache_writer->p, xctx,
            AFW_INTEGER_FMT
            " " AFW_SIZE_T_FMT
            " " AFW_UTF8_FMT
            "%s" AFW_UTF8_FMT
            "\n",
            ++(xctx->write_sequence),
            size,
            AFW_UTF8_FMT_ARG(self->pub.streamId),
            (self->info && self->info->len > 0) ? " " : "",
            AFW_UTF8_FMT_OPTIONAL_ARG(self->info) );

        afw_writer_write_utf8(self->response_writer, header, xctx);
        afw_memory_writer_retrieve_using_callback_and_release(
            self->cache_writer,
            (void *)self->response_writer,
            self->response_writer->write_cb,
            xctx);
    }
    self->cache_writer = NULL;
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
    impl_afw_stream_write_cb((void *)instance, buffer, size, instance->p, xctx);
}



/* Register application/x-afw content handler. */
AFW_DEFINE_INTERNAL(void)
afw_content_type_application_afw_register(afw_xctx_t *xctx)
{
    afw_content_type_register(&impl_afw_content_type_application_afw, xctx);
}



AFW_DEFINE(const afw_stream_t *)
afw_content_type_application_afw_stream_create(
    const afw_stream_t *response_stream,
    const afw_utf8_t *streamId,
    const afw_utf8_t *info,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_content_type_application_afw_stream_self_t *self;

    self = afw_pool_calloc_type(p,
        afw_content_type_application_afw_stream_self_t, xctx);
    self->pub.inf = &impl_afw_stream_inf;
    self->pub.p = afw_pool_create(p, xctx);
    self->pub.streamId = streamId;
    self->pub.write_cb = &impl_afw_stream_write_cb;
    self->response_writer = response_stream;
    self->info = info;
    self->needs_leading_white_space = false;
 
    return (const afw_stream_t *)self;
}
