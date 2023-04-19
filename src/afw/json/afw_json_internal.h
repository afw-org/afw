// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework JSON Support Internal Header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_JSON_INTERNAL_H__
#define __AFW_JSON_INTERNAL_H__

/**
 * @defgroup afw_json_internal JSON support
 * @ingroup afw_c_internal
 *
 * Adaptive Framework JSON internal support
 * 
 * @{
 */

/**
 * @file afw_json_internal.h
 * @brief Adaptive Framework JSON Support Internal Header
 * @internal
 */

#include "afw_interface.h"

AFW_BEGIN_DECLARES

/* Convert a value to json and write it. */
void afw_json_internal_write_value(
    const afw_value_t *value,
    const afw_object_options_t *options,
    void * context,
    afw_write_cb_t callback,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

AFW_END_DECLARES

/** @} */

#endif /* __AFW_JSON_INTERNAL_H__ */
