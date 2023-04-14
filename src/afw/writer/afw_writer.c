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
#define AFW_IMPLEMENTATION_ID "afw_writer_fd"
#include "afw_writer_impl_declares.h"


/* FIXME If you prefer, you can move this struct to a .h file. */
/* Self typedef for afw_writer_fd implementation of afw_writer. */
typedef struct
afw_writer_fd_afw_writer_self_s {
    afw_writer_t pub;
    FILE *fd;
    int tab_size;
    afw_boolean_t close_on_release;
} afw_writer_fd_afw_writer_self_t;


/*
 * Implementation of method release for interface afw_writer.
 */
void
impl_afw_writer_release(
    const afw_writer_t *instance,
    afw_xctx_t *xctx)
{
    afw_writer_fd_afw_writer_self_t *self =
        (afw_writer_fd_afw_writer_self_t *)instance;

    if (self->close_on_release) {
        fclose(self->fd);
    }
}

/*
 * Implementation of method flush for interface afw_writer.
 */
void
impl_afw_writer_flush(
    const afw_writer_t *instance,
    afw_xctx_t *xctx)
{
    /* Ignore flush. */
}

/*
 * Implementation of method write for interface afw_writer.
 */
void
impl_afw_writer_write(
    const afw_writer_t *instance,
    const void *buffer,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    afw_writer_fd_afw_writer_self_t *self =
        (afw_writer_fd_afw_writer_self_t *)instance;
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
}

/*
 * Implementation of method write_eol for interface afw_writer.
 */
void
impl_afw_writer_write_eol(
    const afw_writer_t *instance,
    afw_xctx_t *xctx)
{
    afw_writer_fd_afw_writer_self_t *self =
        (afw_writer_fd_afw_writer_self_t *)instance;
    size_t len;

    /** @todo loop if everything is not returned??? */
    len = fwrite("\n", 1, 1, self->fd);
    if (len == 0) {
        AFW_THROW_ERROR_RV_Z(general, NULL, ferror(self->fd),
            "fwrite() failed",
            xctx);
    }
}

/*
 * Implementation of method increment_indent for interface afw_writer.
 */
void
impl_afw_writer_increment_indent(
    const afw_writer_t *instance,
    afw_xctx_t *xctx)
{
    afw_writer_fd_afw_writer_self_t *self =
        (afw_writer_fd_afw_writer_self_t *)instance;

    (self->pub.indent)++;
}

/*
 * Implementation of method decrement_indent for interface afw_writer.
 */
void
impl_afw_writer_decrement_indent(
    const afw_writer_t *instance,
    afw_xctx_t *xctx)
{
    afw_writer_fd_afw_writer_self_t *self =
        (afw_writer_fd_afw_writer_self_t *)instance;

    if (self->pub.indent == 0) {
        AFW_THROW_ERROR_Z(general,
            "afw_writer_decrement_indent() call when indent is already 0",
            xctx);
    }
    (self->pub.indent)--;
}



/* Call afw_writer_write() with an integer. */
AFW_DEFINE(void)
afw_writer_write_integer(
    const afw_writer_t *writer,
    afw_integer_t integer,
    afw_xctx_t *xctx)
{
    char buf[AFW_INTEGER_MAX_BUFFER];

    snprintf(buf, AFW_INTEGER_MAX_BUFFER, "%" AFW_INTEGER_FMT,
        integer);
    afw_writer_write_z(writer, buf, xctx);
}



/* Call afw_writer_write() with a size. */
AFW_DEFINE(void)
afw_writer_write_size(
    const afw_writer_t *writer,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    char buf[AFW_SIZE_T_MAX_BUFFER];

    snprintf(buf, AFW_SIZE_T_MAX_BUFFER, "%" AFW_SIZE_T_FMT,
        size);
    afw_writer_write_z(writer, buf, xctx);
}
