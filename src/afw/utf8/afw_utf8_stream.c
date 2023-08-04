// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Value UTF-8 Stream
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_utf8_stream.c
 * @brief Adaptive Framework utf-8 stream.
 */

#include "afw_internal.h"



typedef struct {
    afw_stream_t pub;
    apr_array_header_t *ary;
    afw_boolean_t needs_leading_white_space;
} impl_utf8_stream_self_t;


#define AFW_IMPLEMENTATION_ID "afw_utf8_stream"
#include "afw_stream_impl_declares.h"

/*
 * Implementation of method write_eol for interface afw_stream.
 */
static afw_size_t
impl_afw_stream_write_cb(
    void *context,
    const void *buffer,
    afw_size_t size,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    impl_utf8_stream_self_t *self = (impl_utf8_stream_self_t *)context;
    afw_size_t i;

    for (i = 0; i < size; i++) {
        APR_ARRAY_PUSH(self->ary, char) =
            ((const afw_octet_t *)buffer)[i];
    }
    return size;
}


/* Create UTF-8 stream */
AFW_DEFINE(const afw_stream_t *)
afw_utf8_stream_create(
    const afw_utf8_t *streamId,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    impl_utf8_stream_self_t *self;

    /* Create self and ary in its own pool. */
    p = afw_pool_create(p, xctx);
    self = afw_pool_calloc_type(p,
        impl_utf8_stream_self_t,
        xctx);
    self->pub.inf = &impl_afw_stream_inf;
    self->pub.p = p;
    self->pub.streamId = streamId;
    self->pub.write_cb = impl_afw_stream_write_cb;
    self->ary = apr_array_make(afw_pool_get_apr_pool(p), 4000, 1);

    /* Return new instance. */
    return (const afw_stream_t *)self;
}



/* Get the current string in a UTF-8 stream. */
AFW_DEFINE(void)
afw_utf8_stream_get_current_cached_string(
    const afw_stream_t *stream,
    afw_utf8_t *current_cached_string,
    afw_xctx_t *xctx)
{
    impl_utf8_stream_self_t *self = (impl_utf8_stream_self_t *)stream;

    if (stream->inf != &impl_afw_stream_inf) {
        AFW_THROW_ERROR_Z(general,
            "afw_utf8_stream_get_current_cached_string() can only be called "
            "by stream created by afw_utf8_stream_create()", xctx);
    }
    current_cached_string->s = self->ary->elts;
    current_cached_string->len = self->ary->nelts;
}


/*
 * Implementation of method release for interface afw_stream.
 */
void
impl_afw_stream_release(
    const afw_stream_t * instance,
    afw_xctx_t *xctx)
{
    impl_utf8_stream_self_t *self =
        (impl_utf8_stream_self_t *)instance;

    /* Release stream resources. */
    afw_pool_release(self->pub.p, xctx);
}

/*
 * Implementation of method flush for interface afw_stream.
 */
void
impl_afw_stream_flush(
    const afw_stream_t *instance,
    afw_xctx_t *xctx)
{
    /* Ignore flush. */
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
 * Implementation of method write for interface afw_stream.
 */
void
impl_afw_stream_write(
    const afw_stream_t * instance,
    const void * buffer,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    impl_utf8_stream_self_t *self =
        (impl_utf8_stream_self_t *)instance;
    const afw_octet_t *c;
    afw_size_t count;

    if (size == 0) {
        return;
    }

    c = (const afw_octet_t *)buffer;
    for (count = 0; count < size; count++) {
        APR_ARRAY_PUSH(self->ary, char) = *c++;
    }
}
