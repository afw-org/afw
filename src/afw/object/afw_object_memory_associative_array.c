// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_object_associative_array interface in memory
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_object_memory_associative_array.c
 * @brief Implementation of afw_object_associative_array interface in memory. 
 */

#include "afw_internal.h"

/* Declares and rti/inf defines for interface afw_object_associative_array */
#define AFW_IMPLEMENTATION_ID "memory"
#include "afw_object_associative_array_impl_declares.h"

/* Self */
typedef struct impl_afw_object_associative_array_self_s {
    afw_object_associative_array_t pub;

    /* Hash table with key of key and value of object instance. */
    apr_hash_t *objects;

    /* Reference_count starting at 1 on create. */
    AFW_ATOMIC afw_integer_t reference_count;

} impl_afw_object_associative_array_self_t;



/* Create memory object associative array. */
AFW_DEFINE(const afw_object_associative_array_t *)
afw_object_memory_associative_array_create(
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    impl_afw_object_associative_array_self_t *self;
    const afw_pool_t *new_p;

    /* Create new pool for object associative array. */
    new_p = afw_pool_create(p, xctx);

    /* Allocate self and initialize. */
    self = afw_pool_calloc_type(new_p,
        impl_afw_object_associative_array_self_t, xctx);
    self->pub.inf = &impl_afw_object_associative_array_inf;
    self->pub.p = new_p;
    self->reference_count = 1;
    self->objects = apr_hash_make(afw_pool_get_apr_pool(new_p));

    /* Return new instance. */
    return (afw_object_associative_array_t *)self;
}


/*
 * Implementation of method release of interface afw_object_associative_array.
 */
void
impl_afw_object_associative_array_release (
    const afw_object_associative_array_t * instance,
    afw_xctx_t *xctx)
{
    /* Assign instance pointer to self. */
    impl_afw_object_associative_array_self_t * self = 
        (impl_afw_object_associative_array_self_t *)instance;

    apr_hash_index_t *hi;
    const void *key;
    apr_ssize_t klen;
    const afw_object_t *object;

    /* Decrement reference count and release object and array's pool if zero. */
    if (afw_atomic_integer_decrement(&self->reference_count) == 0) {
        for (hi = apr_hash_first(
                afw_pool_get_apr_pool(instance->p), self->objects);
            hi;
            hi = apr_hash_next(hi))
        {
            apr_hash_this(hi, &key, &klen, (void **)&object);
            afw_object_release(object, xctx);
        }

        afw_pool_release(instance->p, xctx);
    }
}



/*
 * Implementation of method get_reference of interface afw_object_associative_array.
 */
void
impl_afw_object_associative_array_get_reference (
    const afw_object_associative_array_t * instance,
    afw_xctx_t *xctx)
{
    /* Assign instance pointer to self. */
    impl_afw_object_associative_array_self_t * self = 
        (impl_afw_object_associative_array_self_t *)instance;

    /* Increment reference count. */
    afw_atomic_integer_increment(&self->reference_count);
}


static void
impl_release_object(void *data, void *data2,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_object_release((const afw_object_t *)data, xctx);
}


/*
 * Implementation of method get of interface afw_object_associative_array.
 */
const afw_object_t *
impl_afw_object_associative_array_get (
    const afw_object_associative_array_t * instance,
    const afw_utf8_t * key,
    afw_xctx_t *xctx)
{
    /* Assign instance pointer to self. */
    impl_afw_object_associative_array_self_t * self = 
        (impl_afw_object_associative_array_self_t *)instance;

    const afw_object_t *object;

    /* Get object associated with key. */
    object = apr_hash_get(self->objects, key->s, key->len);

    /*
     * If object found, add reference and register automatic release when
     * xctx is released.
     */
    if (object) {
        afw_object_get_reference(object, xctx);
        afw_pool_register_cleanup_before(xctx->p, (void *)object, NULL,
            impl_release_object, xctx);
    }

    /* Return result. */
    return object;
}



/*
 * Implementation of method get_reference of interface afw_object_associative_array.
 */
const afw_object_t *
impl_afw_object_associative_array_get_associated_object_reference (
    const afw_object_associative_array_t * instance,
    const afw_utf8_t * key,
    afw_xctx_t *xctx)
{
    /* Assign instance pointer to self. */
    impl_afw_object_associative_array_self_t * self = 
        (impl_afw_object_associative_array_self_t *)instance;

    const afw_object_t *object;

    /* Get object associated with key. */
    object = apr_hash_get(self->objects, key->s, key->len);

    /* If object found, add reference. */
    if (object) {
        afw_object_get_reference(object, xctx);
    }

    /* Return result. */
    return object;
}



/*
 * Implementation of method for_each of interface afw_object_associative_array.
 */
void
impl_afw_object_associative_array_for_each (
    const afw_object_associative_array_t * instance,
    void * context,
    afw_object_cb_t cb,
    afw_xctx_t *xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}



/*
 * Implementation of method set of interface afw_object_associative_array.
 */
void
impl_afw_object_associative_array_set (
    const afw_object_associative_array_t * instance,
    const afw_utf8_t * key,
    const afw_object_t * object,
    afw_xctx_t *xctx)
{
    /* Assign instance pointer to self. */
    impl_afw_object_associative_array_self_t * self = 
        (impl_afw_object_associative_array_self_t *)instance;

    const afw_object_t *existing;

    /* If an object is associated with key, call its release(). */
    existing = apr_hash_get(self->objects, key->s, key->len);
    if (existing) {
        afw_object_release(object, xctx);
    }

    /* If object passed, add reference. */
    if (object) {
        afw_object_get_reference(object, xctx);
    }

    /* Set/remove association. */
    apr_hash_set(self->objects, key->s, key->len, object);
}
