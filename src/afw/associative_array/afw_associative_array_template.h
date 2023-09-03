// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Associative Array Templates
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_ASSOCIATIVE_ARRAY_TEMPLATE_H__
#define __AFW_ASSOCIATIVE_ARRAY_TEMPLATE_H__

/**
 * @addtogroup afw_array_template_internal
 * @{
 */

/**
 * @file afw_associative_array_template.h
 * @brief (deprecated) Adaptive Framework associative array templates
 *
 * This header is included by afw_common.h.  I can only include "templates".
 */

#define AFW_ASSOCIATIVE_ARRAY_TEMPLATE(name, type) \
\
struct name##_s { \
    const afw_pool_t *p; \
}; \
\
AFW_DEFINE_STATIC_INLINE( const name##_t * ) \
name##_create( \
    afw_associative_array_get_reference_value_cb get_reference_value, \
    afw_associative_array_release_value_cb release_value, \
    const afw_pool_t *p, afw_xctx_t *xctx) \
{\
    return (const name##_t *)afw_associative_array_create( \
        get_reference_value, release_value, p, xctx); \
}\
\
AFW_DEFINE_STATIC_INLINE( void ) \
name##_release(const name##_t *instance, \
    afw_xctx_t *xctx) \
{\
    afw_associative_array_release( \
        (const afw_associative_array_t *)instance, \
        xctx); \
}\
\
AFW_DEFINE_STATIC_INLINE( void ) \
name##_get_reference(const name##_t *instance, \
    afw_xctx_t *xctx) \
{\
    afw_associative_array_get_reference( \
        (const afw_associative_array_t *)instance, \
        xctx); \
}\
\
AFW_DEFINE_STATIC_INLINE( const type * ) \
name##_get(const name##_t *instance, const afw_utf8_t *key, \
    afw_xctx_t *xctx) \
{\
    return afw_associative_array_get( \
        (const afw_associative_array_t *)instance, \
        key, xctx); \
}\
\
AFW_DEFINE_STATIC_INLINE( const type * ) \
name##_get_associated_object_reference( \
    const name##_t *instance, const afw_utf8_t *key, afw_xctx_t *xctx) \
{\
    return afw_associative_array_get_associated_object_reference( \
        (const afw_associative_array_t *)instance, \
        key,  xctx); \
}\
\
AFW_DEFINE_STATIC_INLINE( void ) \
name##_for_each(const name##_t *instance, void *context, \
    afw_value_cb_t callback, afw_xctx_t *xctx) \
{\
    afw_associative_array_for_each( \
        (const afw_associative_array_t *)instance, \
        context, callback, xctx); \
}\
\
AFW_DEFINE_STATIC_INLINE( void ) \
name##_set(const name##_t *instance, \
    const afw_utf8_t *key, const type *value, \
    afw_xctx_t *xctx) \
{\
    afw_associative_array_set( \
        (const afw_associative_array_t *)instance, \
        key, ( const void * )value,  xctx); \
}\

/** @} */

#endif /* __AFW_ASSOCIATIVE_ARRAY_TEMPLATE_H__ */
