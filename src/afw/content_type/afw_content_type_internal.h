// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework content type internals
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_CONTENT_TYPE_INTERNAL_H__
#define __AFW_CONTENT_TYPE_INTERNAL_H__

#include "afw_interface.h"

/**
 * @file afw_content_type_internal.h
 * @brief Content-type declarations that are not part of the public API.
 */

AFW_BEGIN_DECLARES

afw_utf8_t *
afw_content_type_x_to_utf8_with_options(
    const afw_content_type_t * instance,
    const afw_value_t * value,
    const afw_object_options_t *options,
    afw_xctx_t *xctx);

const afw_value_t *
afw_content_type_x_to_object_value_with_ids(
    const afw_content_type_t * instance,
    const afw_utf8_t * string,
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    afw_xctx_t *xctx);

AFW_END_DECLARES

#endif /* __AFW_CONTENT_TYPE_INTERNAL_H__ */
