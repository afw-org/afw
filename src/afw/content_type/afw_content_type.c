// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework afw_content_type interface support
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_content_type.c
 * @brief Adaptive Framework afw_content_type interface support.
 */

#include "afw_internal.h"


/* Register content type with its id and all of its media types. */
AFW_DEFINE(void)
afw_content_type_register(
    const afw_content_type_t *content_type,
    afw_xctx_t *xctx)
{
    afw_size_t count;
    const afw_utf8_t *media_type;

    /* Register content type as its id. */
    afw_environment_register_content_type(
        &content_type->content_type_id, content_type,
        xctx);

    /* Register content type as all of its media types. */
    for (count = content_type->media_type_count,
        media_type = content_type->media_types;
        count > 0;
        count--, media_type++)
    {
        afw_environment_register_content_type(media_type, content_type,
            xctx);
    }
}

/*
 * Convert an adaptive value to the raw representation of this content
 * type using the specified pool.
 */
AFW_DEFINE(const afw_memory_t *)
afw_content_type_value_to_raw(
    const afw_content_type_t * instance,
    const afw_value_t * value,
    const afw_object_options_t *options,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_memory_writer_t *writer;

    writer = afw_memory_create_writer(p, xctx);
    afw_content_type_write_value(instance, value, options,
        writer->context, writer->callback, p, xctx);

    return afw_memory_writer_retrieve_and_release(writer, xctx);
}
