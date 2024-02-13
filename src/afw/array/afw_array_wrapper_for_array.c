// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework afw_array interface for array
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_array_wrapper_for_array.c
 * @brief Adaptive Framework afw_array interface for array.
 */

#include "afw_internal.h"



#define impl_afw_array_get_entry_meta afw_array_impl_get_entry_meta
#define impl_afw_array_get_next_entry_meta afw_array_impl_get_next_entry_meta

/* Declares and rti/inf defines for interface afw_array */
#define AFW_IMPLEMENTATION_ID "afw_array_wrapper_for_array"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_array_wrapper_for_array_inf
#include "afw_array_impl_declares.h"
#undef AFW_IMPLEMENTATION_INF_SPECIFIER
#undef AFW_IMPLEMENTATION_INF_LABEL


/* Create a immutable array wrapper for an array. */
AFW_DEFINE(const afw_array_t *)
afw_array_create_wrapper_for_array(
    const void *internal,
    afw_boolean_t indirect,
    const afw_data_type_t *data_type,
    afw_size_t count,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_array_wrapper_for_array_self_t *self;
    afw_value_array_t *value;
    const afw_octet_t *ptr;

    self = afw_pool_calloc(p,
        sizeof(afw_array_wrapper_for_array_self_t) +
        sizeof(afw_value_array_t),
        xctx);
    self->pub.inf = &afw_array_wrapper_for_array_inf;
    self->pub.p = p;
    value = (afw_value_array_t *)
        ((char *)self + sizeof(afw_array_wrapper_for_array_self_t));
    self->pub.value = (const afw_value_t *)value;
    value->inf = &afw_value_managed_array_inf;
    value->internal = (const afw_array_t *)self;
    self->internal = internal;
    self->data_type = data_type;
    self->count = count;
    self->indirect = indirect;

    if (count == -1) {
        if (indirect) {
            for (count = 0, ptr = (const afw_octet_t *)internal;
                *(const void * const *)ptr;
                count++, ptr += sizeof(void *));
        }
        else {
            if (data_type->c_type_size < sizeof(const void * const *)) {
                AFW_THROW_ERROR_Z(general,
                    "count -1 is not supported for this data type",
                    xctx);
            }
            for (count = 0, ptr = (const afw_octet_t *)internal;
                *(const void * const *)ptr;
                count++, ptr += data_type->c_type_size);
        }
        self->count = count;
    }

    return (const afw_array_t *)self;
}



/* Convertan array toan array of strings. */
AFW_DEFINE(const afw_array_t *)
afw_array_convert_to_array_of_strings(
    const afw_array_t *array,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_array_wrapper_for_array_self_t *self;
    afw_size_t count;
    const afw_data_type_t *data_type;
    void *internal;
    const afw_utf8_t *s;
    const afw_value_t *value;
    const afw_iterator_t *iterator;

    /* Return original if already array of string. */
    if (afw_array_get_data_type(array, xctx) == afw_data_type_string) {
        return array;
    }

    /* Get count. */
    count = afw_array_get_count(array, xctx);

    /* If count is 0, return empty array of string. */
    if (count == 0) {
        return afw_data_type_string->empty_array;
    }

    /* Allocate array for internal values and self for wrapper. */
    internal = afw_pool_malloc(p, count * sizeof(afw_utf8_t), xctx);
    self = afw_pool_calloc_type(p, afw_array_wrapper_for_array_self_t, xctx);
    self->pub.inf = &afw_array_wrapper_for_array_inf;
    self->pub.p = p;
    self->internal = internal;
    self->data_type = afw_data_type_string;
    self->count = count;
    self->indirect = false;

    /* Convert each value in array to a string. */
    for (iterator = NULL;;
        internal = (void *)(((afw_byte_t *)internal) + sizeof(afw_utf8_t)))
    {
        value = afw_array_get_next_value(array, &iterator, p, xctx);
        if (!value) {
            break;
        }
        value = afw_value_evaluate(value, p, xctx);
        data_type = afw_value_get_data_type(value, xctx);
        if (!data_type) {
            AFW_THROW_ERROR_Z(general,
                "data type needed in afw_array_convert_to_array_of_strings()",
                xctx);
        }
        s = afw_data_type_internal_to_utf8(
            data_type, &((const afw_value_common_t *)value)->internal,
            p, xctx);
        memcpy(internal, s, sizeof(afw_utf8_t));
    }

    /* Return array of string. */
    return (const afw_array_t *)self;
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
    afw_array_wrapper_for_array_self_t * self = 
        (afw_array_wrapper_for_array_self_t *)instance;
    
    return self->count;
}



/*
 * Implementation of method get_data_type for interface afw_array.
 */
const afw_data_type_t *
impl_afw_array_get_data_type(
    const afw_array_t * instance,
    afw_xctx_t *xctx)
{
    afw_array_wrapper_for_array_self_t * self = 
        (afw_array_wrapper_for_array_self_t *)instance;
  
    return self->data_type;
}



/*
 * Implementation of method get_entry_internal for interface afw_array.
 */
afw_boolean_t
impl_afw_array_get_entry_internal(
    const afw_array_t * instance,
    afw_integer_t index,
    const afw_data_type_t * * data_type,
    const void * *internal,
    afw_xctx_t *xctx)
{
    afw_array_wrapper_for_array_self_t * self = 
        (afw_array_wrapper_for_array_self_t *)instance;
    const void *e;
    afw_size_t count;
    afw_size_t i;

    /* e is NULL if negative index or it doesn't fit in afw_size_t. */
    i = (afw_size_t)index;
    if (index < 0 || i != index) {
        e = NULL;
    }

    /* NULL terminated array. */
    else if (self->count == -1) {
        for (count = 0, e = self->internal;
            *(void **)e && count < i;
            count++, e = ((void **)e) + 1);
        e = *(void **)e;
    }

    /* List of internals. */
    else {
        if (i > self->count) {
            e = NULL;
        }
        else if (self->indirect) {
            e = *(((void **)self->internal) + i);
        }
        else {
            e = (void *)
                (((afw_byte_t *)self->internal) +
                (i * self->data_type->c_type_size));
        }
    }

    /* Found. */
    if (e) {
        *internal = e;
        if (data_type) {
            *data_type = self->data_type;
        }
        return true;
    }

    /* Not found. */
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
    const afw_data_type_t *data_type;
    const void *internal;
    const afw_value_t *result;

    if (impl_afw_array_get_entry_internal(instance,
        index, &data_type, &internal, xctx))
    {
        result = afw_value_common_create(
            internal, data_type, p, xctx);
    }

    else {
        result = NULL;
    }

    return result;
}



/*
 * Implementation of method get_entry_internal for interface afw_array.
 */
afw_boolean_t
impl_afw_array_get_next_internal(
    const afw_array_t * instance,
    const afw_iterator_t * * iterator,
    const afw_data_type_t * * data_type,
    const void * * internal,
    afw_xctx_t *xctx)
{
    afw_array_wrapper_for_array_self_t * self = 
        (afw_array_wrapper_for_array_self_t *)instance;
    afw_size_t size;

    /* If iterator is NULL, point to first value in array. */
    if (!*iterator) {
        *iterator = (const afw_iterator_t *)self->internal;
    }

    /* Determine size of entry. */
    size = (self->indirect) ? sizeof(void *) : self->data_type->c_type_size;

    /* If past end of values, result is NULL. */
    if (*iterator >= (const afw_iterator_t *)(
        ((const afw_octet_t *)self->internal) + (size * self->count)))
    {
        if (data_type) {
            *data_type = NULL;
        }
        *internal = NULL;
        *iterator = NULL;
    }

    /* If not past end, set return values and increment iterator. */
    else {
        if (data_type) {
            *data_type = self->data_type;
        }
        if (self->indirect) {
            *internal = *(const void **)*iterator;
        }
        else {
            *internal = (const void *)*iterator;
        }
        *iterator = (const afw_iterator_t *)
            (((afw_octet_t *)(*iterator)) + size);
    }

    return *internal != NULL;
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
    afw_array_wrapper_for_array_self_t * self = 
        (afw_array_wrapper_for_array_self_t *)instance;
    const afw_value_t *result;
    afw_size_t size;

    /* Require p. */
    if (!p) {
        AFW_THROW_ERROR_Z(general,
            "afw_array_get_next_value() requires p",
            xctx);
    }

    /* If iterator is NULL, point to first value in array. */
    if (!*iterator) {
        *iterator = (const afw_iterator_t *)self->internal;
    }

    /* Determine size of entry. */
    size = (self->indirect) ? sizeof(void *) : self->data_type->c_type_size;

    /* If past end of values, result is NULL. */
    if (*iterator >= (const afw_iterator_t *)(
        ((const afw_octet_t *)self->internal) + (size * self->count)))
    {
        result = NULL;
        *iterator = NULL;
    }

    /* If not past end, create a single value and increment iterator. */
    else {
        result = afw_value_common_create(
            (self->indirect) ? *(void **)*iterator: (void *)*iterator,
            self->data_type,
            p, xctx);
        *iterator = (const afw_iterator_t *)
            (((afw_octet_t *)(*iterator)) + size);
    }

    /* Return result. */
    return result;
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
