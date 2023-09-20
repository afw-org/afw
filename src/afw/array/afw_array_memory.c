// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_array interface
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_array_memory.c
 * @brief Implementation of afw_array interface for in-memory objects
 */

#include "afw_internal.h"



#define impl_afw_array_get_entry_meta afw_array_impl_get_entry_meta
#define impl_afw_array_get_next_entry_meta afw_array_impl_get_next_entry_meta

/* Declares and rti/inf defines for interface afw_array */
#define AFW_IMPLEMENTATION_ID "memory"
#include "afw_array_impl_declares.h"
#include "afw_array_setter_impl_declares.h"

typedef struct afw_memory_internal_array_s
afw_memory_internal_array_t;

typedef struct afw_memory_internal_array_entry_s
afw_memory_internal_array_entry_t;

typedef struct afw_memory_internal_array_ring_s
afw_memory_internal_array_ring_t;

struct afw_memory_internal_array_entry_s {
    APR_RING_ENTRY(afw_memory_internal_array_entry_s) link;
    afw_integer_t index;
    const afw_value_t *value;
};

APR_RING_HEAD(afw_memory_internal_array_ring_s,
    afw_memory_internal_array_entry_s);

struct afw_memory_internal_array_s {
    afw_array_t pub;
    const afw_pool_t *p;
    const afw_data_type_t *data_type;
    afw_array_setter_t setter;
    afw_memory_internal_array_ring_t *ring;
    afw_boolean_t immutable;
    afw_boolean_t generic;
};


AFW_DEFINE(const afw_array_t *)
afw_array_create_with_options(
    int options,
    const afw_data_type_t *data_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{

    afw_memory_internal_array_t *self;
    afw_memory_internal_array_ring_t *ring;

    /* Allocate memory for self. */
    self = afw_pool_calloc_type(p, afw_memory_internal_array_t, xctx);

    /* Allocate memory for value array ring container. */
    ring = afw_pool_calloc_type(p, afw_memory_internal_array_ring_t, xctx);

    /* Initialize self. */
    self->pub.inf = &impl_afw_array_inf;
    self->p = p;
    self->data_type = data_type;
    self->generic = data_type == NULL;
    APR_RING_INIT(ring, afw_memory_internal_array_entry_s, link);
    self->ring = ring;
    self->setter.inf = &impl_afw_array_setter_inf;
    self->setter.array = (const afw_array_t *)self;

    /* Return new object. */
    return (const afw_array_t *)self;

}



/*
 * Implementation of method release of interface afw_array.
 */
void
impl_afw_array_release(
    const afw_array_t * instance,
    afw_xctx_t *xctx) {

    /*
     * Continue release, even if there is already an error.  Don't overwrite
     * existing error.
     */

    /*
     * Storage for value array is allocated in the pool provided, so nothing
     * needs to be done.
     */
}



/*
 * Implementation of method get_count for interface afw_array.
 */
afw_size_t
impl_afw_array_get_count(
    const afw_array_t * instance,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *self = (afw_memory_internal_array_t *)instance;
    afw_memory_internal_array_entry_t *ep;
    afw_size_t count;

    count = 0;
    APR_RING_FOREACH(ep, self->ring, afw_memory_internal_array_entry_s,
        link)
    {
        count++;
    }

    return count;
}



/*
 * Implementation of method get_data_type for interface afw_array.
 */
const afw_data_type_t *
impl_afw_array_get_data_type(
    const afw_array_t * instance,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *self = (afw_memory_internal_array_t *)instance;

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
    const void * * internal,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;

    value = impl_afw_array_get_entry_value(instance, index, NULL, xctx);
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
    afw_memory_internal_array_t *self = (afw_memory_internal_array_t *)instance;
    afw_memory_internal_array_entry_t *ep;
    afw_size_t count;
    const afw_value_t *value;
    afw_size_t i;

    /* Return NULL if negative index or it doesn't fit in afw_size_t. */
    i = (afw_size_t)index;
    if (index < 0 || i != index) {
        return NULL;
    }

    count = 0;
    value = NULL;
    APR_RING_FOREACH(ep, self->ring, afw_memory_internal_array_entry_s,
        link)
    {
        if (count >= i) {
            value = ep->value;
            break;
        }
        count++;
    }

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
    afw_memory_internal_array_t *self = (afw_memory_internal_array_t *)instance;
    afw_memory_internal_array_entry_t *ep;

    /* If iterator is NULL, locate first else locate next and update iterator. */
    if (!*iterator) {
        ep = APR_RING_FIRST(self->ring);
    }
    else {
        ep = (afw_memory_internal_array_entry_t *)*iterator;
        ep = APR_RING_NEXT(ep, link);
    }
    *iterator = (afw_iterator_t *)ep;


    /* If sentinel, return !found. */
    if (ep == APR_RING_SENTINEL(self->ring,
        afw_memory_internal_array_entry_s, link))
    {
        *internal = NULL;
        if (data_type) {
            *data_type = NULL;
        }
        return false;
    }

    /* Return next value. */
    *iterator = (afw_iterator_t *)ep;
    *internal = AFW_VALUE_INTERNAL(ep->value);
    if (data_type) {
        *data_type = afw_value_get_data_type(ep->value, xctx);

    }
    return true;
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
    afw_memory_internal_array_t *self = (afw_memory_internal_array_t *)instance;
    afw_memory_internal_array_entry_t *ep;

    /* If iterator is NULL, locate first else locate next and update iterator. */
    if (!*iterator) {
        ep = APR_RING_FIRST(self->ring);
    }
    else {
        ep = (afw_memory_internal_array_entry_t *)*iterator;
        ep = APR_RING_NEXT(ep, link);
    }

    /* If sentinel, return !found. */
    if (ep == APR_RING_SENTINEL(self->ring,
        afw_memory_internal_array_entry_s, link))
    {
        *iterator = NULL;
        return NULL;
    }

    /* Return next value. */
    *iterator = (afw_iterator_t *)ep;
    return ep->value;
}



/*
 * Implementation of method get_setter for interface afw_array.
 */
const afw_array_setter_t *
impl_afw_array_get_setter(
    const afw_array_t * instance,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *self = (afw_memory_internal_array_t *)instance;

    return (self->immutable) ? NULL : &self->setter;
}



/*
 * Implementation of method set_immutable for interface afw_array_setter.
 */
void
impl_afw_array_setter_set_immutable(
    const afw_array_setter_t * instance,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)instance)->array;

    self->immutable = true;
}



/*
 * Implementation of method determine_data_type_and_set_immutable for
 * interface afw_array_setter.
 */
const afw_data_type_t *
impl_afw_array_setter_determine_data_type_and_set_immutable(
    const afw_array_setter_t * instance,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)instance)->array;
    afw_memory_internal_array_entry_t *ep;

    /* Make immutable if not already. */
    if (self->immutable) {
        AFW_LIST_ERROR_OBJECT_IMMUTABLE;
    }
    self->immutable = true;

    /* If data type not known yet, try to determine it. */
    if (!self->data_type) {
        for (ep = APR_RING_FIRST(self->ring);
            ep != APR_RING_SENTINEL(self->ring,
                afw_memory_internal_array_entry_s, link);
            ep = APR_RING_NEXT(ep, link))
        {
            if (!self->data_type) {
                self->data_type =
                    afw_value_get_data_type(ep->value, xctx);
            }
            else {
                if (self->data_type !=
                    afw_value_get_data_type(ep->value, xctx))
                {
                    self->data_type = NULL;
                    break;
                }
            }
        }
    }

    /* Return next value. */
    return self->data_type;
}



/*
 * Implementation of method add_internal for interface afw_array_setter.
 */
void
impl_afw_array_setter_add_internal(
    const afw_array_setter_t * instance,
    const afw_data_type_t *data_type,
    const void * internal,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)instance)->array;
    const afw_value_t *value;

    value = afw_value_evaluated_create(internal, data_type, self->p, xctx);
    impl_afw_array_setter_add_value(instance, value, xctx);
}


/*
 * Implementation of method add_value of interface afw_array_setter.
 */
void
impl_afw_array_setter_add_value(
    const afw_array_setter_t * instance,
    const afw_value_t * value,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)instance)->array;
    afw_memory_internal_array_entry_t *ep;

    /*
     * If generic, set data type on first entry and set to NULL if any entry
     * after that has a different data type.
     */
    if (self->generic) {
        if (APR_RING_EMPTY(self->ring, afw_memory_internal_array_entry_s, link))
        {
            if (value) {
                self->data_type = afw_value_get_data_type(value, xctx);;
            }
        }
        else if (
            !value ||
            (self->data_type &&
            self->data_type != afw_value_get_data_type(value, xctx)))
        {
            self->data_type = NULL;
        }
    }

    /* If not generic, make sure data type of value is okay. */
    else if (self->data_type && self->data_type !=
        afw_value_get_data_type(value, xctx))
    {
        AFW_THROW_ERROR_Z(general,
            "Value data_type is not array's data type.", xctx);
    }

    /* Add value. */
    ep = afw_pool_calloc_type(self->p, afw_memory_internal_array_entry_t, xctx);
    ep->value = value;
    APR_RING_INSERT_TAIL(self->ring, ep, afw_memory_internal_array_entry_s, link);
}



/*
 * Implementation of method insert_internal for interface afw_array_setter.
 */
void
impl_afw_array_setter_insert_internal(
    const afw_array_setter_t * instance,
    const afw_data_type_t * data_type,
    const void * internal,
    afw_size_t index,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)instance)->array;
    const afw_value_t *value;

    value = afw_value_evaluated_create(internal, data_type, self->p, xctx);
    impl_afw_array_setter_insert_value(instance, value, index, xctx);
}



/*
 * Implementation of method insert_value for interface afw_array_setter.
 */
void
impl_afw_array_setter_insert_value(
    const afw_array_setter_t * instance,
    const afw_value_t * value,
    afw_size_t index,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)instance)->array;
    afw_memory_internal_array_entry_t *lep;
    afw_memory_internal_array_entry_t *nep;
    afw_size_t count;

    /* Add value. */
    nep = afw_pool_calloc_type(self->p, afw_memory_internal_array_entry_t, xctx);
    nep->value = value;
    
    /*
     * If generic, set data type on first entry and set to NULL if any entry
     * after that has a different data type.
     */
    if (self->generic) {
        if (APR_RING_EMPTY(self->ring, afw_memory_internal_array_entry_s, link))
        {
            self->data_type = afw_value_get_data_type(value, xctx);;
        }
        else if (
            self->data_type &&
            self->data_type != afw_value_get_data_type(value, xctx))
        {
            self->data_type = NULL;
        }
    }

    /* If not generic, make sure data type of value is okay. */
    else if (self->data_type && self->data_type !=
        afw_value_get_data_type(value, xctx))
    {
        AFW_THROW_ERROR_Z(general,
            "Value data_type is not array's data type.", xctx);
    }

    /* If index is 0, insert at head. */
    if (index == 0) {
        APR_RING_INSERT_HEAD(self->ring, nep, afw_memory_internal_array_entry_s, link);
        return;
    }

    /* Insert before the current entry at index if not past end. */
    count = 0;
    APR_RING_FOREACH(lep, self->ring, afw_memory_internal_array_entry_s,
        link)
    {
        if (index == count) {
            APR_RING_INSERT_BEFORE(lep, nep, link);
            return;
        }
        count++;
    }

    /* If index past end, insert at tail. */
    APR_RING_INSERT_TAIL(self->ring, nep, afw_memory_internal_array_entry_s, link);
}



/*
 * Implementation of method remove_all_values of interface afw_array_setter.
 */
void
impl_afw_array_setter_remove_all_values(
    const afw_array_setter_t * instance,
    afw_xctx_t *xctx)

{
    afw_memory_internal_array_t *self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)instance)->array;

    /* Clear array. */
    APR_RING_INIT(self->ring, afw_memory_internal_array_entry_s, link);

    /* If generic, clear data type. */
    if (self->generic) {
        self->data_type = NULL;
    }
}



/*
 * Implementation of method remove_internal for interface afw_array_setter.
 */
void
impl_afw_array_setter_remove_internal(
    const afw_array_setter_t * instance,
    const afw_data_type_t *data_type,
    const void * internal,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)instance)->array;
    afw_memory_internal_array_entry_t *ep;

    /* Search for matching value.  If found, remove it. */
    APR_RING_FOREACH(ep, self->ring, afw_memory_internal_array_entry_s,
        link)
    {
        if (afw_value_get_data_type(ep->value, xctx) == data_type &&
            memcmp(
                &((const afw_value_evaluated_t *)ep->value)->internal,
                internal, data_type->c_type_size) == 0)
        {
            APR_RING_REMOVE(ep, link);
            return;
        }
    }

    AFW_THROW_ERROR_Z(general, "Value not in array", xctx);
}



/*
 * Implementation of method remove_value of interface afw_array_setter.
 */
void
impl_afw_array_setter_remove_value(
    const afw_array_setter_t * instance,
    const afw_value_t * value,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)instance)->array;
    afw_memory_internal_array_entry_t *ep;

    /* Search for matching value.  If found, remove it. */
    APR_RING_FOREACH(ep, self->ring, afw_memory_internal_array_entry_s,
        link)
    {
        if (afw_value_equal(value, ep->value, xctx)) {
            APR_RING_REMOVE(ep, link);
            /* If generic and no more entries, set data type to NULL. */
            if (self->generic &&
                APR_RING_EMPTY(self->ring,
                    afw_memory_internal_array_entry_s, link))
                {
                    self->data_type = NULL;
                }
                return;
        }
    }

    AFW_THROW_ERROR_Z(general, "Value not in array", xctx);
}



/*
 * Implementation of method set_value_by_index for interface afw_array_setter.
 */
void
impl_afw_array_setter_set_value_by_index(
    const afw_array_setter_t *instance,
    afw_size_t index,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    afw_memory_internal_array_t *self =
        (afw_memory_internal_array_t *)((afw_array_setter_t *)instance)->array;
    afw_memory_internal_array_entry_t *lep;
    afw_size_t count;

    /*
     * If generic, set data type on first entry and set to NULL if any entry
     * after that has a different data type.
     */
    if (self->generic) {
        if (APR_RING_EMPTY(self->ring, afw_memory_internal_array_entry_s, link))
        {
            self->data_type = afw_value_get_data_type(value, xctx);;
        }
        else if (
            self->data_type &&
            self->data_type != afw_value_get_data_type(value, xctx))
        {
            self->data_type = NULL;
        }
    }

    /* If not generic, make sure data type of value is okay. */
    else if (self->data_type && self->data_type !=
        afw_value_get_data_type(value, xctx))
    {
        AFW_THROW_ERROR_Z(general,
            "Value data_type is not array's data type.", xctx);
    }

    /* Insert before the current entry at index if not past end. */
    count = 0;
    APR_RING_FOREACH(lep, self->ring, afw_memory_internal_array_entry_s,
        link)
    {
        if (index == count) {
            /** @fixme Consider reducing reference count. */
            lep->value = value;
            return;
        }
        count++;
    }

    /* Throw error if index past end. */
    AFW_THROW_ERROR_Z(general, "Index out of bounds", xctx);
}


/* Create or clone of an array. */
AFW_DEFINE(const afw_array_t *)
afw_array_create_or_clone(
    const afw_array_t *array,
    const afw_data_type_t *data_type,
    afw_boolean_t clone_values,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_iterator_t *iterator;
    const afw_array_t *result;
    const afw_value_t *value;
    const afw_data_type_t *use_data_type;

    use_data_type = data_type;
    if (array) {
        use_data_type = afw_array_get_data_type(array, xctx);
        if (data_type && use_data_type && data_type != use_data_type)
        {
            AFW_THROW_ERROR_Z(general, "data_type does not match", xctx);
        }
    }
    result = afw_array_of_create(use_data_type, p, xctx);
    if (array) for (iterator = NULL;;)
    {
        value = afw_array_get_next_value(array, &iterator, p, xctx);
        if (!value) {
            break;
        }
        if (clone_values) {
            value = afw_value_clone(value, p, xctx);
        }
        afw_array_add_value(result, value, xctx);
    }

    return result;
}



/* Create a typed array from a value. */
AFW_DEFINE(const afw_array_t *)
afw_array_of_create_from_value(
    const afw_data_type_t *data_type,
    const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_data_type_t *value_data_type;
    const afw_array_t *value_array;
    const afw_array_t *old_array;
    const afw_iterator_t *iterator;
    const afw_value_t *v;

    value_data_type = afw_value_get_data_type(value, xctx);

    if (value_data_type == afw_data_type_array) {
        value_array = ((afw_value_array_t *)value)->internal;
        value_data_type = afw_array_get_data_type(value_array, xctx);
        if (value_data_type != data_type) {
            old_array = value_array;
            value_array = afw_array_of_create(data_type, p, xctx);
            for (iterator = NULL;;) {
                v = afw_array_get_next_value(old_array, &iterator, p, xctx);
                if (!v) {
                    break;
                }
                v = afw_value_convert(v, data_type, true, p, xctx);
                afw_array_add_value(value_array, v, xctx);
            }        
        }
    }
    else {
        v = afw_value_convert(value, data_type, true, p, xctx);
        value_array = afw_array_create_wrapper_for_array(
            AFW_VALUE_INTERNAL(v), false, data_type, 1, p, xctx);
    }

    return value_array;
}
