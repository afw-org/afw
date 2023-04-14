// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework List Implementation Helpers
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_list_impl.c
 * @brief List interface implementation helpers.
 */

#include "afw_internal.h"



/*
 * Implementation of method get_entry_meta for interface afw_list.
 */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_list_impl_get_entry_meta(
    const afw_list_t *instance,
    afw_integer_t index,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *entry_value;
    const afw_value_t *result;
    afw_value_meta_object_self_t *meta_self;

    /*FIXME Meta should include list meta. */

    result = NULL;
    entry_value = afw_list_get_entry_value(instance, index, p, xctx);
    if (entry_value) {
        meta_self = afw_value_internal_create_meta_object_self(
            entry_value, p, xctx);
        result = (const afw_value_t *)& meta_self->meta_object_value;
    }

    return result;
}



/*
 * Implementation of method get_next_entry_meta for interface afw_list.
 */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_list_impl_get_next_entry_meta(
    const afw_list_t *instance,
    const afw_iterator_t **iterator,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *entry_value;
    const afw_value_t *result;
    afw_value_meta_object_self_t *meta_self;

    result = NULL;
    entry_value = afw_list_get_next_value(instance,
        iterator, p, xctx);

    if (entry_value) {
        meta_self = afw_value_internal_create_meta_object_self(
            entry_value, p, xctx);
        result = (const afw_value_t *)& meta_self->meta_object_value;
    }

    return result;
}


/*
 * A general impl of method get_entry_meta for interface afw_list
 * that can be accessed externally.
 */
AFW_DECLARE(const afw_value_t *)
afw_list_external_impl_get_entry_meta(
    const afw_list_t *instance,
    afw_integer_t index,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return afw_list_impl_get_entry_meta(
        instance, index, p, xctx);
}



/*
 * A general impl of method get_next_entry_meta for interface
 * afw_list that can be accessed externally.
 */
AFW_DECLARE(const afw_value_t *)
afw_list_external_impl_get_next_entry_meta(
    const afw_list_t *instance,
    const afw_iterator_t **iterator,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return afw_list_impl_get_next_entry_meta(
        instance, iterator, p, xctx);
}
