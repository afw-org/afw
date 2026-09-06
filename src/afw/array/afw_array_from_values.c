// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework afw_array from_values implementations
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_array_from_values.c
 * @brief Immutable array of value pointers (unmanaged, managed, permanent).
 */

#include "afw_internal.h"



#define impl_afw_array_get_entry_meta afw_array_impl_get_entry_meta
#define impl_afw_array_get_next_entry_meta afw_array_impl_get_next_entry_meta
#define impl_afw_array_initialize_iterator afw_array_impl_initialize_iterator

#define AFW_ARRAY_SELF_T afw_array_from_values_self_t

static void
impl_afw_array_managed_from_values_release(
    AFW_ARRAY_SELF_T *self, afw_xctx_t *xctx);
static void
impl_afw_array_managed_from_values_get_reference(
    AFW_ARRAY_SELF_T *self, afw_xctx_t *xctx);


/* Declares and rti/inf defines for interface afw_array */
#define AFW_IMPLEMENTATION_ID "unmanaged_from_values"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_array_unmanaged_from_values_inf
#include "afw_array_impl_declares.h"
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_SPECIFIER
#undef AFW_IMPLEMENTATION_INF_LABEL

#define AFW_IMPLEMENTATION_ID "permanent_from_values"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_array_permanent_from_values_inf
#define AFW_ARRAY_INF_ONLY
#include "afw_array_impl_declares.h"
#undef AFW_ARRAY_INF_ONLY
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_SPECIFIER
#undef AFW_IMPLEMENTATION_INF_LABEL

#define AFW_IMPLEMENTATION_ID "managed_from_values"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_array_managed_from_values_inf
#define AFW_ARRAY_INF_ONLY
#define impl_afw_array_release impl_afw_array_managed_from_values_release
#define impl_afw_array_get_reference \
    impl_afw_array_managed_from_values_get_reference
#include "afw_array_impl_declares.h"
#undef AFW_ARRAY_INF_ONLY
#undef impl_afw_array_release
#undef impl_afw_array_get_reference
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_SPECIFIER
#undef AFW_IMPLEMENTATION_INF_LABEL


/* Create an immutable array from an array of objects. */
AFW_DEFINE(const afw_array_t *)
afw_array_create_unmanaged_from_objects(
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

    return afw_array_create_unmanaged_from_values(
        afw_data_type_object, values, count, p, xctx);
}


/* Create an array from NULL terminated array of objects. */
AFW_DEFINE(const afw_array_t *)
afw_array_create_unmanaged_from_null_terminated_objects(
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

    return afw_array_create_unmanaged_from_objects(objects, count, p, xctx);
}



/* Create an immutable array from an array of values. */
AFW_DEFINE(const afw_array_t *)
afw_array_create_unmanaged_from_values(
    const afw_data_type_t *data_type,
    const afw_value_t *const *values,
    afw_size_t count,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_array_from_values_self_t *self;
    afw_value_array_t *value;

    self = afw_pool_calloc(p,
        sizeof(afw_array_from_values_self_t) +
        sizeof(afw_value_array_t),
        xctx);
    self->pub.inf = &afw_array_unmanaged_from_values_inf;
    self->pub.p = p;
    value = (afw_value_array_t *)
        ((char *)self + sizeof(afw_array_from_values_self_t));
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
afw_array_create_unmanaged_from_null_terminated_values(
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

    return afw_array_create_unmanaged_from_values(
        data_type, values, count, p, xctx);
}



static afw_size_t
impl_count_c_array(
    const void *internal,
    afw_boolean_t indirect,
    const afw_data_type_t *data_type,
    afw_size_t count,
    afw_xctx_t *xctx)
{
    const afw_octet_t *ptr;

    if (count != (afw_size_t)-1) {
        return count;
    }
    if (!internal) {
        return 0;
    }
    if (indirect) {
        for (count = 0, ptr = (const afw_octet_t *)internal;
            *(const void * const *)ptr;
            count++, ptr += sizeof(void *));
        return count;
    }
    if (data_type->c_type_size < sizeof(const void * const *)) {
        AFW_THROW_ERROR_Z(general,
            "count -1 is not supported for this data type",
            xctx);
    }
    for (count = 0, ptr = (const afw_octet_t *)internal;
        *(const void * const *)ptr;
        count++, ptr += data_type->c_type_size);
    return count;
}


static const afw_value_t **
impl_values_from_c_array(
    const void *internal,
    afw_boolean_t indirect,
    const afw_data_type_t *data_type,
    afw_size_t count,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t **values;
    const afw_octet_t *ptr;
    const void *e;
    afw_size_t i;
    afw_size_t size;

    values = afw_pool_malloc(p, count * sizeof(const afw_value_t *), xctx);
    size = indirect ? sizeof(void *) : data_type->c_type_size;
    ptr = (const afw_octet_t *)internal;
    for (i = 0; i < count; i++) {
        e = indirect ? *(const void * const *)ptr : (const void *)ptr;
        values[i] = afw_value_common_create(e, data_type, p, xctx);
        ptr += size;
    }
    return values;
}


/* Copy C internals into a typed unmanaged from_values array. */
AFW_DEFINE(const afw_array_t *)
afw_array_create_unmanaged_from_c_array(
    const void *internal,
    afw_boolean_t indirect,
    const afw_data_type_t *data_type,
    afw_size_t count,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t **values;

    if (!data_type) {
        AFW_THROW_ERROR_Z(general,
            "afw_array_create_unmanaged_from_c_array requires a data type",
            xctx);
    }

    count = impl_count_c_array(internal, indirect, data_type, count, xctx);
    if (count == 0) {
        return afw_array_create_unmanaged_from_values(
            data_type, NULL, 0, p, xctx);
    }
    if (!internal) {
        AFW_THROW_ERROR_Z(general,
            "afw_array_create_unmanaged_from_c_array requires internals",
            xctx);
    }

    values = impl_values_from_c_array(
        internal, indirect, data_type, count, p, xctx);
    return afw_array_create_unmanaged_from_values(
        data_type, values, count, p, xctx);
}


/* Create a managed from_values array from value pointers. */
AFW_DEFINE(const afw_array_t *)
afw_array_create_managed_from_values(
    const afw_data_type_t *data_type,
    const afw_value_t *const *values,
    afw_size_t count,
    afw_xctx_t *xctx)
{
    afw_array_from_values_self_t *self;
    afw_value_array_t *value;
    const afw_value_t **copied;
    const afw_pool_t *p;
    afw_size_t i;

    p = xctx->p;
    self = afw_pool_calloc(p,
        sizeof(afw_array_from_values_self_t) +
        sizeof(afw_value_array_t),
        xctx);
    self->pub.inf = &afw_array_managed_from_values_inf;
    self->pub.p = p;
    value = (afw_value_array_t *)
        ((char *)self + sizeof(afw_array_from_values_self_t));
    self->pub.value = (const afw_value_t *)value;
    value->inf = &afw_value_managed_array_inf;
    value->internal = (const afw_array_t *)self;
    self->data_type = data_type;
    self->count = count;
    self->reference_count = 1;
    if (count > 0) {
        copied = afw_pool_malloc(p,
            count * sizeof(const afw_value_t *), xctx);
        for (i = 0; i < count; i++) {
            copied[i] = afw_value_as_assignable(values[i], xctx);
        }
        self->values = copied;
    }

    return (const afw_array_t *)self;
}


/* Create a managed from_values array from object pointers. */
AFW_DEFINE(const afw_array_t *)
afw_array_create_managed_from_objects(
    const afw_object_t *const *objects,
    afw_size_t count,
    afw_xctx_t *xctx)
{
    const afw_value_t **values;
    const afw_value_t **v;
    const afw_object_t *const *o;
    const afw_pool_t *p;
    afw_size_t i;

    p = xctx->p;
    values = NULL;
    if (count > 0) {
        values = afw_pool_malloc(p, count * sizeof(afw_value_t *), xctx);
        for (o = objects, v = values, i = 0; i < count; o++, v++, i++) {
            *v = afw_value_create_unmanaged_object(*o, p, xctx);
        }
    }

    return afw_array_create_managed_from_values(
        afw_data_type_object, values, count, xctx);
}


/* Create a managed from_values array from NULL-terminated values. */
AFW_DEFINE(const afw_array_t *)
afw_array_create_managed_from_null_terminated_values(
    const afw_data_type_t *data_type,
    const afw_value_t * const *values,
    afw_xctx_t *xctx)
{
    const afw_value_t *const *v;
    afw_size_t count;

    count = 0;
    if (values) {
        for (v = values; *v; count++, v++);
    }

    return afw_array_create_managed_from_values(
        data_type, values, count, xctx);
}


/* Create a managed from_values array from NULL-terminated objects. */
AFW_DEFINE(const afw_array_t *)
afw_array_create_managed_from_null_terminated_objects(
    const afw_object_t *const *objects,
    afw_xctx_t *xctx)
{
    const afw_object_t *const *o;
    afw_size_t count;

    count = 0;
    if (objects) {
        for (o = objects; *o; count++, o++);
    }

    return afw_array_create_managed_from_objects(objects, count, xctx);
}


/* Copy C internals into a typed managed from_values array. */
AFW_DEFINE(const afw_array_t *)
afw_array_create_managed_from_c_array(
    const void *internal,
    afw_boolean_t indirect,
    const afw_data_type_t *data_type,
    afw_size_t count,
    afw_xctx_t *xctx)
{
    const afw_value_t **values;
    const afw_pool_t *p;

    if (!data_type) {
        AFW_THROW_ERROR_Z(general,
            "afw_array_create_managed_from_c_array requires a data type",
            xctx);
    }

    p = xctx->p;
    count = impl_count_c_array(internal, indirect, data_type, count, xctx);
    if (count == 0) {
        return afw_array_create_managed_from_values(
            data_type, NULL, 0, xctx);
    }
    if (!internal) {
        AFW_THROW_ERROR_Z(general,
            "afw_array_create_managed_from_c_array requires internals",
            xctx);
    }

    values = impl_values_from_c_array(
        internal, indirect, data_type, count, p, xctx);
    return afw_array_create_managed_from_values(
        data_type, values, count, xctx);
}



/* Convert an array to an array of strings. */
AFW_DEFINE(const afw_array_t *)
afw_array_convert_to_array_of_strings(
    const afw_array_t *array,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t **values;
    const afw_value_t *value;
    const afw_data_type_t *data_type;
    const afw_utf8_t *s;
    const afw_iterator_old_t *iterator;
    afw_size_t count;
    afw_size_t i;

    if (afw_array_get_data_type(array, xctx) == afw_data_type_string) {
        return array;
    }

    count = afw_array_get_count(array, xctx);
    if (count == 0) {
        return afw_data_type_string->empty_array;
    }

    values = afw_pool_malloc(p, count * sizeof(const afw_value_t *), xctx);
    i = 0;
    for (iterator = NULL;;) {
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
        values[i++] = afw_value_common_create(
            s, afw_data_type_string, p, xctx);
    }

    return afw_array_create_unmanaged_from_values(
        afw_data_type_string, values, i, p, xctx);
}



/*
 * Implementation of method release of interface afw_array.
 */
void
impl_afw_array_release (
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{
    (void)self;
    (void)xctx;
}


void
impl_afw_array_get_reference(
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{
    (void)self;
    (void)xctx;
}


void
impl_afw_array_managed_from_values_release(
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{
    afw_size_t i;

    if (self->reference_count <= 0) {
        return;
    }
    self->reference_count--;
    if (self->reference_count != 0) {
        return;
    }
    if (self->values) {
        for (i = 0; i < self->count; i++) {
            if (self->values[i]) {
                afw_value_release(self->values[i], xctx);
            }
        }
        afw_pool_free_memory(xctx->p, (void *)self->values,
            self->count * sizeof(const afw_value_t *), xctx);
        self->values = NULL;
    }
    afw_pool_free_memory(xctx->p, self,
        sizeof(afw_array_from_values_self_t) +
        sizeof(afw_value_array_t), xctx);
}


void
impl_afw_array_managed_from_values_get_reference(
    AFW_ARRAY_SELF_T *self,
    afw_xctx_t *xctx)
{
    (void)xctx;
    self->reference_count++;
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
