// See the 'COPYING' file in the project root for licensing information.
/*
 * Helpers for afw_content_type* implementation development
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_content_type_impl.c
 * @brief Helpers for afw_content_type* implementation development
 */

#include "afw_internal.h"


/*
 * Declares and rti/inf defines for interface
 * afw_content_type_object_list_writer
 */
#define AFW_IMPLEMENTATION_ID "impl"
#include "afw_content_type_object_list_writer_impl_declares.h"


/* Self for afw_content_type_object_list_writer implementation. */
typedef struct impl_afw_content_type_object_list_writer_self_s {
    afw_content_type_object_list_writer_t pub;

    /* Private implementation variables */
    const afw_pool_t *p;
    const afw_content_type_t *content_type;
    const afw_object_options_t *options;
    void * context;
    afw_write_cb_t callback;
    const afw_memory_t * begin_object_list;
    const afw_memory_t * object_separator;
    const afw_memory_t * last_object_separator;
    const afw_memory_t * end_object_list;
    afw_size_t object_count;

} impl_afw_content_type_object_list_writer_self_t;



AFW_DEFINE(const afw_content_type_object_list_writer_t *)
afw_content_type_impl_create_object_list_writer(
    const afw_content_type_t * instance,
    const afw_object_options_t *options,
    void * context,
    afw_write_cb_t callback,
    const afw_memory_t * begin_object_list,
    const afw_memory_t * object_separator,
    const afw_memory_t * last_object_separator,
    const afw_memory_t * end_object_list,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    impl_afw_content_type_object_list_writer_self_t *self;

    self = afw_pool_calloc_type(p, impl_afw_content_type_object_list_writer_self_t,
        xctx);
    self->pub.inf = &impl_afw_content_type_object_list_writer_inf;

    /* Finish processing parameters and initializing new instance. */
    self->p = p;
    self->content_type = instance;
    self->options = options;
    self->context = context;
    self->callback = callback;
    self->begin_object_list = begin_object_list;
    self->object_separator = object_separator;
    self->last_object_separator = last_object_separator;
    self->end_object_list = end_object_list;

    /* Write object list begin for content type. */
    if (self->begin_object_list && self->begin_object_list->size > 0) {
        callback(self->context,
            self->begin_object_list->ptr,
            self->begin_object_list->size,
            p, xctx);
    }

    /* Return new instance. */
    return (afw_content_type_object_list_writer_t *)self;
}



/*
 * Implementation of method release of interface afw_content_type_object_list_writer.
 */
void
impl_afw_content_type_object_list_writer_release(
    const afw_content_type_object_list_writer_t * instance,
    afw_xctx_t *xctx)
{
    /* Assign instance pointer to self. */
    impl_afw_content_type_object_list_writer_self_t * self =
        (impl_afw_content_type_object_list_writer_self_t *)instance;

    /* If objects have been written, write last separator if there is one. */
    if (self->object_count > 0 &&
        self->last_object_separator &&
        self->last_object_separator->size > 0)
    {
        self->callback(self->context,
            self->last_object_separator->ptr,
            self->last_object_separator->size,
            self->p, xctx);
    }

    /* Write object list end for content type. */
    if (self->end_object_list && self->end_object_list->size > 0) {
        self->callback(self->context,
            self->end_object_list->ptr,
            self->end_object_list->size,
            self->p, xctx);
    }
}



/*
 * Implementation of method write_object of interface afw_content_type_object_list_writer.
 */
void
impl_afw_content_type_object_list_writer_write_object(
    const afw_content_type_object_list_writer_t * instance,
    const afw_object_t * object,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    /* Assign instance pointer to self. */
    impl_afw_content_type_object_list_writer_self_t * self =
        (impl_afw_content_type_object_list_writer_self_t *)instance;
    const afw_memory_t *raw;
    const afw_value_t *value;

    /* If objects have been written, write separator if there is one. */
    if (self->object_count > 0 &&
        self->object_separator &&
        self->object_separator->size > 0)
    {
        self->callback(self->context,
            self->object_separator->ptr,
            self->object_separator->size,
            p, xctx);
    }

    /* Convert value and write. */
    (self->object_count)++;
    value = afw_value_create_unmanaged_object(object, p, xctx);
    raw = afw_content_type_value_to_raw(self->content_type,
        value, self->options, p, xctx);
    self->callback(self->context, raw->ptr, raw->size, p, xctx);
}
