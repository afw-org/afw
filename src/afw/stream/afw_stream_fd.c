// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Helpers for interfaces afw_writer*
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_writer.c
 * @brief Helpers for interfaces afw_writer* 
 */

#include "afw_internal.h"



/* FIXME Add include for headers and anything else you want in this part of code. */

/* Declares and rti/inf defines for interface afw_writer */
#define AFW_IMPLEMENTATION_ID "afw_stream_fd"
#include "afw_stream_impl_declares.h"


/* FIXME If you prefer, you can move this struct to a .h file. */
/* Self typedef for afw_stream_fd implementation of afw_writer. */
typedef struct
afw_stream_fd_self_s {
    afw_stream_t pub;
    FILE *fd;
    afw_boolean_t auto_flush;
    afw_boolean_t close_on_release;
} afw_stream_fd_self_t;



/*
 * Implementation of method write_eol for interface afw_writer.
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
    size_t len;

    /** @todo loop if everything is not returned??? */
    if (size != 0) {
        len = fwrite(buffer, size, 1, self->fd);
        if (len == 0) {
            AFW_THROW_ERROR_RV_Z(general, NULL, ferror(self->fd),
                "fwrite() failed",
                xctx);
        }
    }
    return size;
}

/*
 * Implementation of method release for interface afw_writer.
 */
void
impl_afw_stream_release(
    const afw_stream_t *instance,
    afw_xctx_t *xctx)
{
    afw_stream_fd_self_t *self =
        (afw_stream_fd_self_t *)instance;

    if (self->close_on_release) {
        fclose(self->fd);
    }
}

/*
 * Implementation of method flush for interface afw_writer.
 */
void
impl_afw_stream_flush(
    const afw_stream_t *instance,
    afw_xctx_t *xctx)
{
    afw_stream_fd_self_t *self =
        (afw_stream_fd_self_t *)instance;

    fflush(self->fd);
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
 * Implementation of method write for interface afw_writer.
 */
void
impl_afw_stream_write(
    const afw_stream_t *instance,
    const void *buffer,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    afw_stream_fd_self_t *self =
        (afw_stream_fd_self_t *)instance;
    size_t len;

    /** @todo loop if everything is not returned??? */
    if (size != 0) {
        len = fwrite(buffer, size, 1, self->fd);
        if (len == 0) {
            AFW_THROW_ERROR_RV_Z(general, NULL, ferror(self->fd),
                "fwrite() failed",
                xctx);
        }
        if (self->auto_flush) {
            fflush(self->fd);
        }
    }
}

/* Create a writer to a file descriptor. */
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

    /*
     * You may want to create a new pool for instance, but will just use
     * xctx's pool in this example.
     */
    self = afw_xctx_calloc_type(afw_stream_fd_self_t, xctx);
    self->pub.inf = &impl_afw_stream_inf;
    self->pub.p = p;
    self->pub.streamId = streamId;
    self->pub.write_cb = impl_afw_stream_fd_write_cb;
    self->fd = fd;
    self->auto_flush = auto_flush;
    self->close_on_release = close_on_release;

    /* Return new instance. */
    return (afw_stream_t *)self;
}



/* Open a file and create a stream for it. */
AFW_DECLARE(const afw_stream_t *)
afw_stream_fd_open_and_create(
    const afw_utf8_t *streamId,
    const afw_utf8_t *path,
    const afw_utf8_t *mode,
    afw_boolean_t auto_flush,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_stream_t *stream;
    const afw_utf8_z_t *actual_path_z;
    const afw_utf8_z_t *mode_z;
    const afw_utf8_z_t *c;
    FILE *fd;
    afw_boolean_t allow_read;
    afw_boolean_t allow_write;

    /*! @fixme use application rootPaths */
    actual_path_z = afw_utf8_to_utf8_z(mode, p, xctx);

    mode_z = afw_utf8_to_utf8_z(mode, p, xctx);
    fd = fopen(actual_path_z, mode_z);
    if (!fd) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "streamId %" AFW_UTF8_FMT " failed to open: %s",
            AFW_UTF8_FMT_ARG(streamId), strerror(errno));
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
