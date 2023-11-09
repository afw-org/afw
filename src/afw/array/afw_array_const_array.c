// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework afw_array interface for const arrays
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_array_const_array.c
 * @brief Adaptive Framework afw_array interface for const arrays.
 */

#include "afw_internal.h"



#define impl_afw_array_get_entry_meta afw_array_impl_get_entry_meta
#define impl_afw_array_get_next_entry_meta afw_array_impl_get_next_entry_meta


/* Declares and rti/inf defines for interface afw_array */
#define AFW_IMPLEMENTATION_ID "afw_array_const_array_of_values"
#include "afw_array_impl_declares.h"

/** @fixme
 *
 * Remove typedef if you don't need.
 *
 * You may want to have a self struct that includes instance variables that are
 * not public.  Here is a skeleton, if you do or you may want to include one in
 * your _internal.h header.
 */
typedef struct impl_afw_array_const_array_of_values_self_s {
    afw_array_t pub;

    /* Private implementation variables */
    const afw_value_t *const *values;
    const afw_value_t *const *end_of_values;

} impl_afw_array_const_array_of_values_self_t;


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
            *v = afw_value_create_object(*o, p, xctx);
        }
    }

    return afw_array_const_create_array_of_values(values, count, p, xctx);
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
    const afw_value_t *const *values,
    afw_size_t count,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    impl_afw_array_const_array_of_values_self_t *self;

    self = afw_pool_calloc_type(p,
        impl_afw_array_const_array_of_values_self_t, xctx);
    self->pub.inf = &impl_afw_array_inf;
    if (count > 0) {
        self->values = values;
        self->end_of_values = &values[count];
    }

    return (const afw_array_t *)self;
}



/* Create an immutable array from NULL terminated array of values. */
AFW_DEFINE(const afw_array_t *)
afw_array_const_create_null_terminated_array_of_values(
    const afw_value_t * const *values,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *const *v;
    afw_size_t count;

    {
        for (count = 0, v = values; *v; count++, v++);
    }

    return afw_array_const_create_array_of_values(values, count, p, xctx);
}



/*
 * Implementation of method release of interface afw_array.
 */
void
impl_afw_array_release (
    const afw_array_t * instance,
    afw_xctx_t *xctx)
{
    /* Nothing to do. */
}



/*
 * Implementation of method get_count for interface afw_array.
 */
afw_size_t
impl_afw_array_get_count(
    const afw_array_t * instance,
    afw_xctx_t *xctx)
{
    impl_afw_array_const_array_of_values_self_t * self = 
        (impl_afw_array_const_array_of_values_self_t *)instance;

    return self->end_of_values - self->values;
}



/*
 * Implementation of method get_data_type for interface afw_array.
 */
const afw_data_type_t *
impl_afw_array_get_data_type(
    const afw_array_t * instance,
    afw_xctx_t *xctx)
{
    return NULL;
}



/*
 * Implementation of method get_entry_internal for interface afw_array.
 */
afw_boolean_t
impl_afw_array_get_entry_internal(
    const afw_array_t * instance,
    afw_integer_t index,
    const afw_data_type_t * * data_type,
    const void * * internal,
    afw_xctx_t *xctx)
{
    impl_afw_array_const_array_of_values_self_t * self = 
        (impl_afw_array_const_array_of_values_self_t *)instance;
    afw_size_t i, count;
    const afw_value_t *value;

    /* Safecast index to afw_size_t and get value from array. */
    count = self->end_of_values - self->values;
    i = afw_safe_cast_integer_to_size(index, xctx);
    value = (i >= count) ? NULL : self->values[i];

    if (value) {
        *internal = AFW_VALUE_INTERNAL(value);
        if (data_type) {
            *data_type = afw_value_get_data_type(value, xctx);
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
    const afw_array_t * instance,
    afw_integer_t index,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    impl_afw_array_const_array_of_values_self_t * self = 
        (impl_afw_array_const_array_of_values_self_t *)instance;
    afw_size_t i, count;
    const afw_value_t *value;

    /* Safecast index to afw_size_t and get value from array. */
    count = self->end_of_values - self->values;
    i = afw_safe_cast_integer_to_size(index, xctx);
    value = (i >= count) ? NULL : self->values[i];

    /* Return result. */
    return value;
}



/*
 * Implementation of method get_next_internal for interface afw_array.
 */
afw_boolean_t
impl_afw_array_get_next_internal(
    const afw_array_t * instance,
    const afw_iterator_t * * iterator,
    const afw_data_type_t * * data_type,
    const void * * internal,
    afw_xctx_t *xctx)
{
    impl_afw_array_const_array_of_values_self_t * self = 
        (impl_afw_array_const_array_of_values_self_t *)instance;
    const afw_value_t *const *values;

    /* If iterator is NULL, set it to values. */
    if (!*iterator) {
        if (self->values) {
            *iterator = (const afw_iterator_t *)self->values;
        }
    }
    else {
        *iterator = (const afw_iterator_t * )
            ((*(const afw_value_t *const * const *)iterator) + 1);
    }

    values = *(const afw_value_t *const *const *)iterator;

    if (!values || values >= self->end_of_values) {
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
            *data_type = afw_value_get_data_type(*values, xctx);
        }
        return true;
    }
}



/*
 * Implementation of method get_next_value for interface afw_array.
 */
const afw_value_t *
impl_afw_array_get_next_value(
    const afw_array_t * instance,
    const afw_iterator_t * * iterator,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    impl_afw_array_const_array_of_values_self_t *self =
        (impl_afw_array_const_array_of_values_self_t *)instance;
    const afw_value_t *const *values;

    /* If iterator is NULL, set it to values. */
    if (!*iterator) {
        if (self->values) {
            *iterator = (const afw_iterator_t *)self->values;
        }
    }
    else {
        *iterator = (const afw_iterator_t *)
            ((*(const afw_value_t *const * const *)iterator) + 1);
    }

    values = *(const afw_value_t *const * const *)iterator;

    if (!values || values >= self->end_of_values) {
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
    const afw_array_t * instance,
    afw_xctx_t *xctx)
{
    return NULL;
}
