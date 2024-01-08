// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Value UTF-8 Writer
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_utf8_writer.c
 * @brief Adaptive Framework utf-8 writer.
 */

#include "afw_internal.h"



typedef struct {
    afw_writer_t pub;
    apr_array_header_t *ary;
    afw_boolean_t needs_leading_white_space;
} impl_utf8_writer_self_t;


#define AFW_IMPLEMENTATION_ID "afw_utf8_writer"
#include "afw_writer_impl_declares.h"

/*
 * Implementation of method write_eol for interface afw_writer.
 */
static afw_size_t
impl_afw_writer_write_raw_cb(
    void *context,
    const void *buffer,
    afw_size_t size,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    impl_utf8_writer_self_t *self = (impl_utf8_writer_self_t *)context;
    afw_size_t i;

    for (i = 0; i < size; i++) {
        APR_ARRAY_PUSH(self->ary, char) =
            ((const afw_octet_t *)buffer)[i];
    }
    return size;
}


/* Create UTF-8 writer */
AFW_DEFINE(const afw_writer_t *)
afw_utf8_writer_create(
    const afw_utf8_t *tab,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    impl_utf8_writer_self_t *self;

    /* Create self and ary in its own pool. */
    p = afw_pool_create(p, xctx);
    self = afw_pool_calloc_type(p,
        impl_utf8_writer_self_t,
        xctx);
    self->pub.inf = &impl_afw_writer_inf;
    self->pub.p = p;
    self->pub.write_raw_cb = impl_afw_writer_write_raw_cb;
    self->ary = apr_array_make(afw_pool_get_apr_pool(p), 4000, 1);
    self->pub.tab = tab;

    /* Return new instance. */
    return (const afw_writer_t *)self;
}



/* Get the current string in a UTF-8 writer. */
AFW_DEFINE(void)
afw_utf8_writer_current_string(
    const afw_writer_t *writer,
    afw_utf8_t *current_string,
    afw_xctx_t *xctx)
{
    impl_utf8_writer_self_t *self = (impl_utf8_writer_self_t *)writer;

    if (writer->inf != &impl_afw_writer_inf) {
        AFW_THROW_ERROR_Z(general,
            "afw_utf8_writer_get_string() can only be called by writer "
            "created by afw_utf8_writer_create()", xctx);
    }
    current_string->s = self->ary->elts;
    current_string->len = self->ary->nelts;
}


/*
 * Implementation of method release for interface afw_writer.
 */
void
impl_afw_writer_release(
    const afw_writer_t * instance,
    afw_xctx_t *xctx)
{
    impl_utf8_writer_self_t *self =
        (impl_utf8_writer_self_t *)instance;

    /* Release writer resources. */
    afw_pool_release(self->pub.p, xctx);
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
    const afw_writer_t * instance,
    const void * buffer,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    impl_utf8_writer_self_t *self =
        (impl_utf8_writer_self_t *)instance;
    afw_size_t count, i;
    const afw_octet_t *c;

    if (size == 0) {
        return;
    }

    if (self->needs_leading_white_space) {
        for (count = 0; count < self->pub.indent; count++) {
            for (i = 0; i < self->pub.tab->len; i++) {
                APR_ARRAY_PUSH(self->ary, char) = self->pub.tab->s[i];
            }
        }
        self->needs_leading_white_space = false;
    }

    c = (const afw_octet_t *)buffer;
    for (count = 0; count < size; count++) {
        APR_ARRAY_PUSH(self->ary, char) = *c++;
    }
}


/*
 * Implementation of method write_eol for interface afw_writer.
 */
void
impl_afw_writer_write_eol(
    const afw_writer_t * instance,
    afw_xctx_t *xctx)
{
    impl_utf8_writer_self_t *self =
        (impl_utf8_writer_self_t *)instance;

    if (self->pub.tab) {
        APR_ARRAY_PUSH(self->ary, char) = '\n';
        self->needs_leading_white_space = true;
    }
}

/*
 * Implementation of method increment_indent for interface afw_writer.
 */
void
impl_afw_writer_increment_indent(
    const afw_writer_t * instance,
    afw_xctx_t *xctx)
{
    impl_utf8_writer_self_t *self =
        (impl_utf8_writer_self_t *)instance;

    self->pub.indent++;
}

/*
 * Implementation of method decrement_indent for interface afw_writer.
 */
void
impl_afw_writer_decrement_indent(
    const afw_writer_t * instance,
    afw_xctx_t *xctx)
{
    impl_utf8_writer_self_t *self =
        (impl_utf8_writer_self_t *)instance;

    self->pub.indent--;
}
