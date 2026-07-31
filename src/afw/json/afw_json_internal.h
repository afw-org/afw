// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework JSON Support Internal Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_JSON_INTERNAL_H__
#define __AFW_JSON_INTERNAL_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_json_internal
 * @{
 */

/**
 * @file afw_json_internal.h
 * @brief Adaptive Framework JSON Support Internal Header
 * @internal
 */

AFW_BEGIN_DECLARES

/* Convert a value to json and write it.
 *
 * indent — optional per-level indent unit (e.g. four spaces). When non-NULL
 * and non-empty, whitespace is enabled and this string is used instead of the
 * default four spaces. When NULL, AFW_OBJECT_OPTION_whitespace still enables
 * the default four-space indent.
 */
void afw_json_internal_write_value(
    const afw_value_t *value,
    const afw_object_options_t *options,
    const afw_utf8_t *indent,
    void * context,
    afw_write_cb_t callback,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_END_DECLARES

/** @} */  // end of @addtogroup @addtogroup

#endif /* __AFW_JSON_INTERNAL_H__ */
