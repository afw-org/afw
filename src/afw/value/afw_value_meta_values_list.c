// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework array implementation: meta values of an array
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_value_meta_values_list.c
 * @brief Immutable array view of meta() for each entry of an associated array.
 */

#include "afw_internal.h"



#define impl_afw_array_get_entry_meta afw_array_impl_get_entry_meta
#define impl_afw_array_get_next_entry_meta afw_array_impl_get_next_entry_meta
#define impl_afw_array_initialize_iterator afw_array_impl_initialize_iterator

/* Declares and rti/inf defines for interface afw_array */
#define AFW_IMPLEMENTATION_ID "afw_value_meta_values_list"
#define AFW_ARRAY_SELF_T afw_value_meta_values_list_list_self_t
#include "afw_array_impl_declares.h"


typedef struct {
    const afw_iterator_old_t *inner;
    afw_integer_t index;
} impl_meta_values_list_iterator_t;


/*
 * Implementation of method release for interface afw_array.
 */
void
impl_afw_array_release(
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{
    /* Nothing to release; &self->pub lives in its create pool. */
}


/*
 * Implementation of method get_count for interface afw_array.
 */
afw_size_t
impl_afw_array_get_count(
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{

    return afw_array_get_count(self->associated_value->internal, xctx);
}


/*
 * Implementation of method get_data_type for interface afw_array.
 */
const afw_data_type_t *
impl_afw_array_get_data_type(
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{
    /* Each entry is a meta object. */
    return afw_data_type_object;
}


/*
 * Implementation of method get_entry_internal for interface afw_array.
 */
afw_boolean_t
impl_afw_array_get_entry_internal(
    AFW_ARRAY_SELF_T *self,
    afw_integer_t index,
    const afw_data_type_t **data_type,
    const void **internal,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;

    value = impl_afw_array_get_entry_value(self, index, self->pub.p, xctx);
    if (value) {
        *internal = AFW_VALUE_INTERNAL(value);
        if (data_type) {
            *data_type = afw_data_type_object;
        }
        return true;
    }

    *internal = NULL;
    if (data_type) {
        *data_type = NULL;
    }
    return false;
}


/*
 * Implementation of method get_entry_value for interface afw_array.
 */
const afw_value_t *
impl_afw_array_get_entry_value(
    AFW_ARRAY_SELF_T *self,
    afw_integer_t index,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *entry_value;
    afw_value_meta_object_self_t *meta_self;
    const afw_pool_t *use_p;

    use_p = p ? p : self->pub.p;
    entry_value = afw_array_get_entry_value(
        self->associated_value->internal, index, use_p, xctx);
    if (!entry_value) {
        return NULL;
    }

    meta_self = afw_value_internal_create_meta_object_self(
        entry_value, use_p, xctx);
    if (index >= 0) {
        meta_self->key = afw_number_integer_to_utf8(index, use_p, xctx);
    }

    return &meta_self->meta_object_value.pub;
}


/*
 * Implementation of method get_next_internal for interface afw_array.
 */
afw_boolean_t
impl_afw_array_get_next_internal(
    AFW_ARRAY_SELF_T *self,
    const afw_iterator_old_t **iterator,
    const afw_data_type_t **data_type,
    const void **internal,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;

    value = impl_afw_array_get_next_value(self, iterator, self->pub.p, xctx);
    if (value) {
        *internal = AFW_VALUE_INTERNAL(value);
        if (data_type) {
            *data_type = afw_data_type_object;
        }
        return true;
    }

    *internal = NULL;
    if (data_type) {
        *data_type = NULL;
    }
    return false;
}


/*
 * Implementation of method get_next_value for interface afw_array.
 */
const afw_value_t *
impl_afw_array_get_next_value(
    AFW_ARRAY_SELF_T *self,
    const afw_iterator_old_t **iterator,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *entry_value;
    afw_value_meta_object_self_t *meta_self;
    const afw_pool_t *use_p;
    impl_meta_values_list_iterator_t *state;

    use_p = p ? p : self->pub.p;
    if (!use_p) {
        use_p = xctx->p;
    }

    if (!*iterator) {
        state = afw_pool_calloc_type(use_p,
            impl_meta_values_list_iterator_t, xctx);
        state->inner = NULL;
        state->index = 0;
        *iterator = (const afw_iterator_old_t *)state;
    }
    else {
        state = (impl_meta_values_list_iterator_t *)*iterator;
    }

    entry_value = afw_array_get_next_value(
        self->associated_value->internal, &state->inner, use_p, xctx);
    if (!entry_value) {
        *iterator = NULL;
        return NULL;
    }

    meta_self = afw_value_internal_create_meta_object_self(
        entry_value, use_p, xctx);
    if (state->index >= 0) {
        meta_self->key = afw_number_integer_to_utf8(
            state->index, use_p, xctx);
    }
    state->index++;

    return &meta_self->meta_object_value.pub;
}


/*
 * Implementation of method get_setter for interface afw_array.
 */
const afw_array_setter_t *
impl_afw_array_get_setter(
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{
    /* Meta values views are immutable. */
    return NULL;
}


const afw_value_t *
afw_value_meta_values_list_for_list_create(
    const afw_value_t *associated_value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_meta_values_list_list_self_t *self;

    AFW_VALUE_ASSERT_IS_DATA_TYPE(associated_value, array, xctx);

    self = afw_pool_calloc_type(p, afw_value_meta_values_list_list_self_t, xctx);
    self->pub.inf = &impl_afw_array_inf;
    self->pub.p = p;
    self->value.inf = &afw_value_unmanaged_array_inf;
    self->value.internal = (const afw_array_t *)self;
    self->pub.value = (const afw_value_t *)&self->value;
    self->associated_value = (const afw_value_array_t *)associated_value;

    return (const afw_value_t *)&self->value;
}
