// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework List Implementation Helpers
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_array_impl.c
 * @brief List/array interface implementation helpers.
 */

#include "afw_internal.h"



/*
 * Implementation of method get_entry_meta for interface afw_array.
 */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_array_impl_get_entry_meta(
    const afw_array_t *instance,
    afw_integer_t index,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *entry_value;
    const afw_value_t *result;
    afw_value_meta_object_self_t *meta_self;

    /*FIXME Meta should include array meta. */

    result = NULL;
    entry_value = afw_array_get_entry_value(instance, index, p, xctx);
    if (entry_value) {
        meta_self = afw_value_internal_create_meta_object_self(
            entry_value, p, xctx);
        result = &meta_self->meta_object_value.pub;
    }

    return result;
}



/*
 * Implementation of method get_next_entry_meta for interface afw_array.
 */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_array_impl_get_next_entry_meta(
    const afw_array_t *instance,
    const afw_iterator_old_t **iterator,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *entry_value;
    const afw_value_t *result;
    afw_value_meta_object_self_t *meta_self;

    result = NULL;
    entry_value = afw_array_get_next_value(instance,
        iterator, p, xctx);

    if (entry_value) {
        meta_self = afw_value_internal_create_meta_object_self(
            entry_value, p, xctx);
        result = &meta_self->meta_object_value.pub;
    }

    return result;
}


/*
 * A general impl of method get_entry_meta for interface afw_array
 * that can be accessed externally.
 */
AFW_DECLARE(const afw_value_t *)
afw_array_external_impl_get_entry_meta(
    const afw_array_t *instance,
    afw_integer_t index,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return afw_array_impl_get_entry_meta(
        instance, index, p, xctx);
}



/*
 * A general impl of method get_next_entry_meta for interface
 * afw_array that can be accessed externally.
 */
AFW_DECLARE(const afw_value_t *)
afw_array_external_impl_get_next_entry_meta(
    const afw_array_t *instance,
    const afw_iterator_old_t **iterator,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return afw_array_impl_get_next_entry_meta(
        instance, iterator, p, xctx);
}


/* ---- defined-instance afw_iterator over any afw_array (#153) ---- */

#define AFW_IMPLEMENTATION_ID "afw_array_as_iterator"
#define AFW_IMPLEMENTATION_INF_LABEL impl_afw_array_as_iterator_inf
/* Non-const self in impl_*; public API stays const (inf casts at vtable). */
#define AFW_ITERATOR_SELF_T afw_iterator_t
#include "afw_iterator_impl_declares.h"


/*
 * Implementation of method get_next for interface afw_iterator (array host).
 * Returns next value, or NULL when done.
 */
const afw_value_t *
impl_afw_iterator_get_next(
    AFW_ITERATOR_SELF_T *self,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_array_t *array;
    const afw_value_t *value;
    afw_size_t count;
    afw_size_t i;

    array = (const afw_array_t *)self->impl;
    count = afw_array_get_count(array, xctx);
    i = self->offset;
    if (i >= count) {
        return NULL; /* done */
    }
    value = afw_array_get_entry_value(array, (afw_integer_t)i, p, xctx);
    self->offset = i + 1;
    return value;
}


/*
 * Implementation of method get_by_index for interface afw_iterator (array).
 */
const afw_value_t *
impl_afw_iterator_get_by_index(
    AFW_ITERATOR_SELF_T *self,
    afw_integer_t index,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_array_t *array;

    array = (const afw_array_t *)self->impl;
    return afw_array_get_entry_value(array, index, p, xctx);
}


/*
 * Implementation of method get_count for interface afw_iterator (array host).
 */
afw_size_t
impl_afw_iterator_get_count(
    AFW_ITERATOR_SELF_T *self,
    afw_xctx_t *xctx)
{
    const afw_array_t *array;

    array = (const afw_array_t *)self->impl;
    return afw_array_get_count(array, xctx);
}


/*
 * Shared initialize_iterator for afw_array implementations.
 * Sets inf for array-as-iterator and resets cursor fields.
 */
AFW_DEFINE_INTERNAL(void)
afw_array_impl_initialize_iterator(
    const afw_array_t *instance,
    const afw_iterator_t *iterator,
    afw_xctx_t *xctx)
{
    afw_iterator_t *it;

    AFW_ASSERT(instance);
    AFW_ASSERT(iterator);

    /* Const on the API marks storage as opaque to the caller; we fill it. */
    it = (afw_iterator_t *)iterator;
    it->inf = &impl_afw_array_as_iterator_inf;
    it->impl = (void *)instance;
    it->offset = 0;
    it->reserved = 0;

    (void)xctx;
}
