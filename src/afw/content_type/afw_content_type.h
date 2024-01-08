// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework afw_content_type interface support header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_CONTENT_TYPE_H__
#define __AFW_CONTENT_TYPE_H__

#include "afw_interface.h"

/**
 * @defgroup afw_content_type Content type
 * @ingroup afw_c_api_public
 * 
 * Provide afw_content_type interface common support.
 * @{
 */

/**
 * @file afw_content_type.h
 * @brief Header Adaptive Framework afw_content_type interface support
 */

AFW_BEGIN_DECLARES

/** @fixme Decide what to do in dynamo_db code. */
AFW_DEFINE_STATIC_INLINE(afw_utf8_t *)
afw_content_type_x_to_utf8_with_options(
    const afw_content_type_t * instance,
    const afw_value_t * value,
    const afw_object_options_t *options,
    afw_xctx_t *xctx)
{
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}

/** @fixme Decide what to do in dynamo_db code. */
AFW_DEFINE_STATIC_INLINE(const afw_value_t *)
afw_content_type_x_to_object_value_with_ids(
    const afw_content_type_t * instance,
    const afw_utf8_t * string,
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    afw_xctx_t *xctx)
{
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}


/**
 * @brief Register content type with its id and all of its media types.
 * @param content_type to register.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_content_type_register(
    const afw_content_type_t *content_type,
    afw_xctx_t *xctx);



/**
 * @brief Convert value to the raw in specified pool.
 * @param instance of content type.
 * @param value to convert.
 * @param options.
 * @param p to use for result.
 * @param xctx of caller.
 * @return converted raw encoded memory.
 */
AFW_DECLARE(const afw_memory_t *)
afw_content_type_value_to_raw(
    const afw_content_type_t * instance,
    const afw_value_t * value,
    const afw_object_options_t *options,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Convert object to the raw in specified pool.
 * @param instance of content type.
 * @param object to convert.
 * @param options.
 * @param p to use for result.
 * @param xctx of caller.
 * @return converted raw encoded memory.
 */
#define afw_content_type_object_to_raw(instance, object, options, p, xctx) \
    afw_content_type_value_to_raw(instance, \
        afw_value_create_unmanaged_object(object, p, xctx), options, p, xctx)


/**
 * @brief Determine if content type it application/x-afw.
 * @param instance of content type.
 * @return result of test.
*/
#define afw_content_type_is_application_afw(instance) \
((instance) && (instance)->inf == &afw_content_type_application_afw_inf)



/** @brief application/x-afw content type inf. */
AFW_DECLARE_CONST_DATA(afw_content_type_inf_t)
afw_content_type_application_afw_inf;



/**
 * @brief Create a stream for content type application/afw.
 * @param response_stream this stream will flush to.
 * @param streamId.
 * @param info for header frame.
 * @param p to use.
 * @param xctx of caller.
 */
AFW_DECLARE(const afw_stream_t *)
afw_content_type_application_afw_stream_create(
    const afw_stream_t *response_stream,
    const afw_utf8_t *streamId,
    const afw_utf8_t *info,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_CONTENT_TYPE_H__ */
