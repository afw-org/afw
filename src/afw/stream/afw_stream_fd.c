// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework file-descriptor stream
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_stream_fd.c
 * @brief afw_stream implementation over FILE *
 */

#include "afw_internal.h"


/* Declares and rti/inf defines for interface afw_stream */
#define AFW_IMPLEMENTATION_ID "afw_stream_fd"
#include "afw_stream_impl_declares.h"


/* Self for afw_stream_fd implementation of afw_stream. */
typedef struct
afw_stream_fd_self_s {
    afw_stream_t pub;
    FILE *fd;
    afw_boolean_t allow_read;
    afw_boolean_t allow_write;
    afw_boolean_t auto_flush;
    afw_boolean_t close_on_release;
} afw_stream_fd_self_t;



/*
 * Implementation of write callback for interface afw_stream.
 */
static afw_size_t
impl_afw_stream_fd_write_cb(
    void *context,
    const void *buffer,
    afw_size_t size,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_stream_fd_self_t *self =
        (afw_stream_fd_self_t *)context;
    const afw_octet_t *ptr;
    size_t remaining;
    size_t n;

    (void)p;

    if (!self->allow_write) {
        AFW_THROW_ERROR_Z(general,
            "Stream is not open for write", xctx);
    }

    ptr = (const afw_octet_t *)buffer;
    remaining = size;
    while (remaining > 0) {
        n = fwrite(ptr, 1, remaining, self->fd);
        if (n == 0) {
            AFW_THROW_ERROR_RV_Z(general, NULL, ferror(self->fd),
                "fwrite() failed",
                xctx);
        }
        ptr += n;
        remaining -= n;
    }
    if (self->auto_flush) {
        fflush(self->fd);
    }
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
    afw_stream_fd_self_t *self =
        (afw_stream_fd_self_t *)instance;

    (void)xctx;

    if (self->close_on_release && self->fd) {
        fclose(self->fd);
        self->fd = NULL;
    }
}



/*
 * Implementation of method flush for interface afw_stream.
 */
void
impl_afw_stream_flush(
    const afw_stream_t *instance,
    afw_xctx_t *xctx)
{
    afw_stream_fd_self_t *self =
        (afw_stream_fd_self_t *)instance;

    (void)xctx;

    if (self->fd) {
        fflush(self->fd);
    }
}



/*
 * Implementation of method read for interface afw_stream.
 * Returns number of octets read (0 at EOF).
 */
afw_size_t
impl_afw_stream_read(
    const afw_stream_t *instance,
    const void *buffer,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    afw_stream_fd_self_t *self =
        (afw_stream_fd_self_t *)instance;
    size_t n;

    if (!self->allow_read) {
        AFW_THROW_ERROR_Z(general,
            "Stream is not open for read", xctx);
    }
    if (size == 0 || !buffer) {
        return 0;
    }

    n = fread((void *)buffer, 1, size, self->fd);
    if (n == 0 && ferror(self->fd)) {
        AFW_THROW_ERROR_RV_Z(general, NULL, ferror(self->fd),
            "fread() failed",
            xctx);
    }
    return (afw_size_t)n;
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
    impl_afw_stream_fd_write_cb((void *)instance, buffer, size,
        instance->p, xctx);
}



/* Create a stream to a file descriptor. */
AFW_DEFINE(const afw_stream_t *)
afw_stream_fd_create(
    FILE *fd,
    const afw_utf8_t *streamId,
    afw_boolean_t allow_read,
    afw_boolean_t allow_write,
    afw_boolean_t auto_flush,
    afw_boolean_t close_on_release,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_stream_fd_self_t *self;

    self = afw_xctx_calloc_type(afw_stream_fd_self_t, xctx);
    self->pub.inf = &impl_afw_stream_inf;
    self->pub.p = p;
    self->pub.streamId = streamId;
    self->pub.write_cb = allow_write ? impl_afw_stream_fd_write_cb : NULL;
    self->fd = fd;
    self->allow_read = allow_read;
    self->allow_write = allow_write;
    self->auto_flush = auto_flush;
    self->close_on_release = close_on_release;

    return (const afw_stream_t *)self;
}



/* Open a file and create a stream for it. Path must already be resolved. */
AFW_DEFINE(const afw_stream_t *)
afw_stream_fd_open_and_create(
    const afw_utf8_t *streamId,
    const afw_utf8_t *path,
    const afw_utf8_t *mode,
    afw_boolean_t auto_flush,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_stream_t *stream;
    const afw_utf8_z_t *path_z;
    const afw_utf8_z_t *mode_z;
    const afw_utf8_z_t *c;
    FILE *fd;
    afw_boolean_t allow_read;
    afw_boolean_t allow_write;

    path_z = afw_utf8_to_utf8_z(path, p, xctx);
    mode_z = afw_utf8_to_utf8_z(mode, p, xctx);
    fd = fopen(path_z, mode_z);
    if (!fd) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "streamId " AFW_UTF8_FMT_Q " failed to open %s: %s",
            AFW_UTF8_FMT_ARG(streamId), path_z, strerror(errno));
    }

    allow_read = false;
    allow_write = false;
    for (c = mode_z; *c; c++) {
        if (*c == 'r') {
            allow_read = true;
        }
        else if (*c == 'w' || *c == 'a') {
            allow_write = true;
        }
        else if (*c == '+') {
            allow_read = true;
            allow_write = true;
        }
    }

    stream = afw_stream_fd_create(fd, streamId,
        allow_read, allow_write, auto_flush, true, p, xctx);

    return stream;
}
