// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework afw_array interface for const arrays
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_array_const_array.c
 * @brief Adaptive Framework afw_array interface for const arrays of values.
 */

#include "afw_internal.h"



#define impl_afw_array_get_entry_meta afw_array_impl_get_entry_meta
#define impl_afw_array_get_next_entry_meta afw_array_impl_get_next_entry_meta
#define impl_afw_array_initialize_iterator afw_array_impl_initialize_iterator


/* Declares and rti/inf defines for interface afw_array */
#define AFW_IMPLEMENTATION_ID "afw_array_const_array_of_values"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_array_const_array_of_values_inf
#define AFW_ARRAY_SELF_T afw_array_const_array_of_values_self_t
#include "afw_array_impl_declares.h"
#undef AFW_IMPLEMENTATION_INF_SPECIFIER
#undef AFW_IMPLEMENTATION_INF_LABEL


/* Create an immutable array from an array of objects. */
AFW_DEFINE(const afw_array_t *)
afw_array_const_create_array_of_objects(
    const afw_object_t *const *objects,
    afw_size_t count,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t **values;
    const afw_value_t **v;
    const afw_object_t *const *o;
    afw_size_t i;

    values = NULL;
    if (count > 0) {
        values = afw_pool_malloc(p, count * sizeof(afw_value_t *), xctx);
        for (o = objects, v = values, i = 0; i < count; o++, v++, i++) {
            *v = afw_value_create_unmanaged_object(*o, p, xctx);
        }
    }

    return afw_array_const_create_array_of_values(
        afw_data_type_object, values, count, p, xctx);
}


/* Create an array from NULL terminated array of objects. */
AFW_DEFINE(const afw_array_t *)
afw_array_const_create_null_terminated_array_of_objects(
    const afw_object_t * const *objects,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *const *o;
    afw_size_t count;

    count = 0;
    if (objects) {
        for (o = objects; *o; count++, o++);
    }

    return afw_array_const_create_array_of_objects(objects, count, p, xctx);
}



/* Create an immutable array from an array of values. */
AFW_DEFINE(const afw_array_t *)
afw_array_const_create_array_of_values(
    const afw_data_type_t *data_type,
    const afw_value_t *const *values,
    afw_size_t count,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_array_const_array_of_values_self_t *self;
    afw_value_array_t *value;

    self = afw_pool_calloc(p,
        sizeof(afw_array_const_array_of_values_self_t) +
        sizeof(afw_value_array_t),
        xctx);
    self->pub.inf = &afw_array_const_array_of_values_inf;
    self->pub.p = p;
    value = (afw_value_array_t *)
        ((char *)self + sizeof(afw_array_const_array_of_values_self_t));
    self->pub.value = (const afw_value_t *)value;
    /* Pool-owned immutable wrapper; unmanaged value face. */
    value->inf = &afw_value_unmanaged_array_inf;
    value->internal = (const afw_array_t *)self;
    self->data_type = data_type;
    self->count = count;
    if (count > 0) {
        self->values = values;
    }

    return (const afw_array_t *)self;
}



/* Create an immutable array from NULL terminated array of values. */
AFW_DEFINE(const afw_array_t *)
afw_array_const_create_null_terminated_array_of_values(
    const afw_data_type_t *data_type,
    const afw_value_t * const *values,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *const *v;
    afw_size_t count;

    count = 0;
    if (values) {
        for (v = values; *v; count++, v++);
    }

    return afw_array_const_create_array_of_values(
        data_type, values, count, p, xctx);
}



/*
 * Implementation of method release of interface afw_array.
 */
void
impl_afw_array_release (
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{
    /* Nothing to do. */
}


void
impl_afw_array_get_reference(
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{
    (void)self;
    (void)xctx;
}



/*
 * Implementation of method get_count for interface afw_array.
 */
afw_size_t
impl_afw_array_get_count(
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{
    (void)xctx;

    return self->count;
}



/*
 * Implementation of method get_data_type for interface afw_array.
 */
const afw_data_type_t *
impl_afw_array_get_data_type(
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{
    (void)xctx;

    return self->data_type;
}



/*
 * Implementation of method get_entry_internal for interface afw_array.
 */
afw_boolean_t
impl_afw_array_get_entry_internal(
    AFW_ARRAY_SELF_T *self,
    afw_integer_t index,
    const afw_data_type_t * * data_type,
    const void * * internal,
    afw_xctx_t *xctx)
{
    afw_size_t i;
    const afw_value_t *value;

    i = afw_safe_cast_integer_to_size(index, xctx);
    value = (i >= self->count || !self->values) ? NULL : self->values[i];

    if (value) {
        *internal = AFW_VALUE_INTERNAL(value);
        if (data_type) {
            *data_type = self->data_type
                ? self->data_type
                : afw_value_get_data_type(value, xctx);
        }
        return true;
    }
    else {
        *internal = NULL;
        if (data_type) {
            *data_type = NULL;
        }
        return false;
    }
}



/*
 * Implementation of method get_entry_value for interface afw_array.
 */
const afw_value_t *
impl_afw_array_get_entry_value(
    AFW_ARRAY_SELF_T *self,
    afw_integer_t index,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    afw_size_t i;

    (void)p;

    i = afw_safe_cast_integer_to_size(index, xctx);
    if (i >= self->count || !self->values) {
        return NULL;
    }

    return self->values[i];
}



/*
 * Implementation of method get_next_internal for interface afw_array.
 */
afw_boolean_t
impl_afw_array_get_next_internal(
    AFW_ARRAY_SELF_T *self,
    const afw_iterator_old_t * * iterator,
    const afw_data_type_t * * data_type,
    const void * * internal,
    afw_xctx_t *xctx)
{
    const afw_value_t *const *values;
    const afw_value_t *const *end;

    if (!self->values || self->count == 0) {
        *internal = NULL;
        *iterator = NULL;
        if (data_type) {
            *data_type = NULL;
        }
        return false;
    }

    end = self->values + self->count;

    if (!*iterator) {
        *iterator = (const afw_iterator_old_t *)self->values;
    }
    else {
        *iterator = (const afw_iterator_old_t *)
            ((*(const afw_value_t *const * const *)iterator) + 1);
    }

    values = *(const afw_value_t *const *const *)iterator;

    if (!values || values >= end) {
        *internal = NULL;
        *iterator = NULL;
        if (data_type) {
            *data_type = NULL;
        }
        return false;
    }
    else {
        *internal = AFW_VALUE_INTERNAL(*values);
        if (data_type) {
            *data_type = self->data_type
                ? self->data_type
                : afw_value_get_data_type(*values, xctx);
        }
        return true;
    }
}



/*
 * Implementation of method get_next_value for interface afw_array.
 */
const afw_value_t *
impl_afw_array_get_next_value(
    AFW_ARRAY_SELF_T *self,
    const afw_iterator_old_t * * iterator,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    const afw_value_t *const *values;
    const afw_value_t *const *end;

    (void)p;
    (void)xctx;

    if (!self->values || self->count == 0) {
        *iterator = NULL;
        return NULL;
    }

    end = self->values + self->count;

    if (!*iterator) {
        *iterator = (const afw_iterator_old_t *)self->values;
    }
    else {
        *iterator = (const afw_iterator_old_t *)
            ((*(const afw_value_t *const * const *)iterator) + 1);
    }

    values = *(const afw_value_t *const * const *)iterator;

    if (!values || values >= end) {
        *iterator = NULL;
        return NULL;
    }
    else {
        return *values;
    }
}



/*
 * Implementation of method get_setter for interface afw_array.
 */
const afw_array_setter_t *
impl_afw_array_get_setter(
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{
    (void)self;
    (void)xctx;

    return NULL;
}
