// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework array implementation: meta values of an object
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_value_meta_values_object.c
 * @brief Immutable array view of meta() for each property of an associated object.
 */

#include "afw_internal.h"



#define impl_afw_array_get_entry_meta afw_array_impl_get_entry_meta
#define impl_afw_array_get_next_entry_meta afw_array_impl_get_next_entry_meta

/* Declares and rti/inf defines for interface afw_array */
#define AFW_IMPLEMENTATION_ID "afw_value_meta_values_object"
#define AFW_ARRAY_SELF_T afw_value_meta_values_object_list_self_t
#include "afw_array_impl_declares.h"


typedef struct {
    const afw_iterator_old_t *inner;
} impl_meta_values_object_iterator_t;


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
    const afw_iterator_old_t *iterator;
    const afw_utf8_t *property_name;
    afw_size_t count;

    count = 0;
    for (iterator = NULL;;) {
        if (!afw_object_get_next_property(
                self->associated_value->internal, &iterator,
                &property_name, xctx))
        {
            break;
        }
        count++;
    }

    return count;
}


/*
 * Implementation of method get_data_type for interface afw_array.
 */
const afw_data_type_t *
impl_afw_array_get_data_type(
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{
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
    const afw_iterator_old_t *iterator;
    const afw_utf8_t *property_name;
    const afw_value_t *property_value;
    afw_value_meta_object_self_t *meta_self;
    const afw_pool_t *use_p;
    afw_integer_t resolved;
    afw_integer_t count;
    afw_integer_t i;

    use_p = p ? p : self->pub.p;

    /* Resolve signed index against property count. */
    count = 0;
    for (iterator = NULL;;) {
        if (!afw_object_get_next_property(
                self->associated_value->internal, &iterator,
                &property_name, xctx))
        {
            break;
        }
        count++;
    }

    if (count == 0) {
        return NULL;
    }

    if (index < 0) {
        resolved = count + index;
    }
    else {
        resolved = index;
    }
    if (resolved < 0 || resolved >= count) {
        return NULL;
    }

    i = 0;
    for (iterator = NULL;;) {
        property_value = afw_object_get_next_property(
            self->associated_value->internal, &iterator,
            &property_name, xctx);
        if (!property_value) {
            return NULL;
        }
        if (i == resolved) {
            meta_self = afw_value_internal_create_meta_object_self(
                property_value, use_p, xctx);
            meta_self->key = property_name;
            return &meta_self->meta_object_value.pub;
        }
        i++;
    }
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
    const afw_value_t *property_value;
    const afw_utf8_t *property_name;
    afw_value_meta_object_self_t *meta_self;
    const afw_pool_t *use_p;
    impl_meta_values_object_iterator_t *state;

    use_p = p ? p : self->pub.p;
    if (!use_p) {
        use_p = xctx->p;
    }

    if (!*iterator) {
        state = afw_pool_calloc_type(use_p,
            impl_meta_values_object_iterator_t, xctx);
        state->inner = NULL;
        *iterator = (const afw_iterator_old_t *)state;
    }
    else {
        state = (impl_meta_values_object_iterator_t *)*iterator;
    }

    property_value = afw_object_get_next_property(
        self->associated_value->internal, &state->inner,
        &property_name, xctx);
    if (!property_value) {
        *iterator = NULL;
        return NULL;
    }

    meta_self = afw_value_internal_create_meta_object_self(
        property_value, use_p, xctx);
    meta_self->key = property_name;

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
    return NULL;
}


AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_value_meta_values_list_for_object_create(
    const afw_value_t *associated_value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_meta_values_object_list_self_t *self;

    AFW_VALUE_ASSERT_IS_DATA_TYPE(associated_value, object, xctx);

    self = afw_pool_calloc_type(p,
        afw_value_meta_values_object_list_self_t, xctx);
    self->pub.inf = &impl_afw_array_inf;
    self->pub.p = p;
    self->value.inf = &afw_value_unmanaged_array_inf;
    self->value.internal = (const afw_array_t *)self;
    self->pub.value = (const afw_value_t *)&self->value;
    self->associated_value = (const afw_value_object_t *)associated_value;

    return (const afw_value_t *)&self->value;
}
