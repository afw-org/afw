// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Object Implementation Helpers
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_object_impl.c
 * @brief Object interface implementation helpers.
 */

#include "afw_internal.h"



AFW_DEFINE(const afw_utf8_t * const *)
afw_object_impl_get_parent_paths_from_parents(
    const afw_object_t * const *parents, afw_xctx_t *xctx)
{
    afw_size_t count;
    const afw_object_t * const *c;
    const afw_utf8_t * *o;
    const afw_utf8_t * const *result;

    for (count = 0, c = parents; *c; count++, c++);

    o = afw_xctx_malloc(
        sizeof(afw_utf8_t *) * (count + 1), xctx);
    result = o;
    for (c = parents; *c; c++, o++) {
        *o = afw_object_meta_get_path(*c, xctx);
        if (!*o) {
            AFW_THROW_ERROR_Z(general, "Path required", xctx);
        }
    }
    *o = NULL;

    return result;
}



/*
 * A general impl of method get_meta for interface afw_object
 * that can be accessed externally.
 */
AFW_DECLARE(const afw_value_t *)
afw_object_impl_get_meta(
    const afw_object_t *instance,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return afw_value_create_object_unmanaged(instance->meta.meta_object,
        p, xctx);
}



/*
 * Implementation of method get_meta for interface afw_object.
 */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_object_impl_internal_get_meta(
    const afw_object_t *instance,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return (instance->meta.meta_object)
        ? afw_value_create_object_unmanaged(instance->meta.meta_object,
            p, xctx)
        : NULL;
}
