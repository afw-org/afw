// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Associative Array
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_ASSOCIATIVE_ARRAY_H__
#define __AFW_ASSOCIATIVE_ARRAY_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_array_template_internal
 * @{
 */

/**
 * @file afw_associative_array.h
 * @brief (deprecated) Adaptive Framework implementation of associative array.
 *
 * These functions will normally not be called directly.  See
 * afw_associative_array_template.h.
 */

AFW_BEGIN_DECLARES

typedef struct afw_associative_array_s {
    const afw_pool_t *p;
} afw_associative_array_t;

typedef void(*afw_associative_array_get_reference_value_cb)
    (const void *value, afw_xctx_t *xctx);

typedef void(*afw_associative_array_release_value_cb)
    (const void *value, afw_xctx_t *xctx);

AFW_DECLARE(const afw_associative_array_t *)
afw_associative_array_create(
    afw_associative_array_get_reference_value_cb get_reference_value,
    afw_associative_array_release_value_cb release_value,
    const afw_pool_t *p, afw_xctx_t *xctx);

AFW_DECLARE(void)
afw_associative_array_release(
    const afw_associative_array_t *instance,
    afw_xctx_t *xctx);

AFW_DECLARE(void)
afw_associative_array_get_reference(
    const afw_associative_array_t *instance,
    afw_xctx_t *xctx);

AFW_DECLARE(const void *)
afw_associative_array_get(
    const afw_associative_array_t *instance,
    const afw_utf8_t *key, afw_xctx_t *xctx);

AFW_DECLARE(const void *)
afw_associative_array_get_associated_object_reference(
    const afw_associative_array_t *instance,
    const afw_utf8_t *key, afw_xctx_t *xctx);

AFW_DECLARE(void)
afw_associative_array_for_each(
    const afw_associative_array_t *instance,
    void *context, afw_value_cb_t callback, afw_xctx_t *xctx);

AFW_DECLARE(void)
afw_associative_array_set(
    const afw_associative_array_t *instance,
    const afw_utf8_t *key, const void *value, afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_ASSOCIATIVE_ARRAY_H__ */
