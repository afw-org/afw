// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Associative Array
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_associative_array.c
 * @brief Adaptive Framework implementation of associative array
 */

#include "afw_internal.h"

/* Self */
typedef struct impl_associative_array_s {

    const afw_pool_t *p;

    afw_associative_array_add_reference_value_cb add_reference_value;

    afw_associative_array_release_value_cb release_value;

    /* Hash table with key of key and value of object instance. */
    apr_hash_t *values;

    /* Reference_count starting at 1 on create. */
    AFW_ATOMIC afw_integer_t reference_count;

} impl_associative_array_t;



/* Create associative array. */
AFW_DEFINE(const afw_associative_array_t *)
afw_associative_array_create(
    afw_associative_array_add_reference_value_cb add_reference_value,
    afw_associative_array_release_value_cb release_value,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    impl_associative_array_t *self;
    const afw_pool_t *new_p;

    /* Create new pool for object associative array. */
    new_p = afw_pool_create(p, xctx);

    /* Allocate self and initialize. */
    self = afw_pool_calloc_type(new_p, impl_associative_array_t, xctx);
    self->p = new_p;
    self->reference_count = 1;
    self->add_reference_value = add_reference_value;
    self->release_value = release_value;
    self->values = apr_hash_make(afw_pool_get_apr_pool(new_p));

    /*
     * If either add_reference_value or release_value is specified,
     * both must be specified.
     */
    if ((add_reference_value && !release_value) ||
        (release_value && !add_reference_value))
    {
        AFW_THROW_ERROR_Z(general,
            "If either add_reference_value or release_value is specified, "
            "both must be specified.",
            xctx);
    }

    /* Return new instance. */
    return (const afw_associative_array_t *)self;
}


/* Release. */
AFW_DEFINE(void)
afw_associative_array_release (
    const afw_associative_array_t * instance,
    afw_xctx_t *xctx)
{
    impl_associative_array_t *self = (impl_associative_array_t *)instance;
    apr_hash_index_t *hi;
    const void *key;
    apr_ssize_t klen;
    void *value;

    /* Decrement reference count and release value and array's pool if zero. */
    if (afw_atomic_integer_decrement(&self->reference_count) == 0) {

        /* If there is a release_value(), call it for each value. */
        if (self->release_value) {
            for (hi = apr_hash_first(
                    afw_pool_get_apr_pool(instance->p), self->values);
                hi;
                hi = apr_hash_next(hi))
            {
                apr_hash_this(hi, &key, &klen, (void **)&value);
                self->release_value(value, xctx);
            }
        }
 
        /* Release pool. */
        afw_pool_release(instance->p, xctx);
    }
}



/* Add reference. */
AFW_DEFINE(void)
afw_associative_array_add_reference (
    const afw_associative_array_t *instance,
    afw_xctx_t *xctx)
{
    impl_associative_array_t *self = (impl_associative_array_t *)instance;

    /* Increment reference count. */
    afw_atomic_integer_increment(&self->reference_count);
}


static void
impl_release_value(void *data, void *data2,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_associative_array_release_value_cb cb = data2;

    cb(data, xctx);
}



/*
 * Implementation of method get of interface afw_object_associative_array.
 */
AFW_DEFINE(const void *)
afw_associative_array_get (
    const afw_associative_array_t *instance,
    const afw_utf8_t *key, afw_xctx_t *xctx)
{
    impl_associative_array_t *self = (impl_associative_array_t *)instance;
    const void *value;

    /* Get value associated with key. */
    value = apr_hash_get(self->values, key->s, key->len);

    /*
     * If value found, add reference and register automatic release when
     * xctx is released.
     */
    if (value && self->add_reference_value) {
        self->add_reference_value(value, xctx);
        afw_pool_register_cleanup_before(xctx->p, (void *)value, self->release_value,
            impl_release_value, xctx);
    }

    /* Return result. */
    return value;
}



/*
 * Implementation of method get_reference of interface afw_object_associative_array.
 */
AFW_DEFINE(const void *)
afw_associative_array_get_reference(
    const afw_associative_array_t *instance,
    const afw_utf8_t *key, afw_xctx_t *xctx)
{
    impl_associative_array_t *self = (impl_associative_array_t *)instance;
    const void *value;

    /* Get value associated with key. */
    value = apr_hash_get(self->values, key->s, key->len);

    /* If value found, add reference. */
    if (value && self->add_reference_value) {
        self->add_reference_value(value, xctx);
    }

    /* Return result. */
    return value;
}



/*
 * Implementation of method get_reference of interface afw_object_associative_array.
 */
AFW_DEFINE(void)
afw_associative_array_for_each(
    const afw_associative_array_t *instance,
    void *context, afw_value_cb_t callback, afw_xctx_t *xctx)
{
    impl_associative_array_t *self = (impl_associative_array_t *)instance;
    apr_hash_index_t *hi;
    afw_object_t *value;

    for (hi = apr_hash_first(afw_pool_get_apr_pool(xctx->p), self->values);
        hi;
        hi = apr_hash_next(hi))
    {
        apr_hash_this(hi, NULL, NULL, (void **)&value);
        if (callback(value, context, xctx)) {
            return;
        }
    }

    callback(NULL, context, xctx);
}



/*
 * Implementation of method set of interface afw_object_associative_array.
 */
AFW_DEFINE(void)
afw_associative_array_set(
    const afw_associative_array_t *instance,
    const afw_utf8_t *key, const void *value, afw_xctx_t *xctx)
{
    impl_associative_array_t *self = (impl_associative_array_t *)instance;
    const void *existing;

    /* If an value is associated with key, call its release(). */
    existing = apr_hash_get(self->values, key->s, key->len);
    if (existing && self->release_value) {
        self->release_value(existing, xctx);
    }

    /* If value passed, add reference. */
    if (value && self->add_reference_value) {
        self->add_reference_value(value, xctx);
    }

    /* Set/remove association. */
    apr_hash_set(self->values, key->s, key->len, value);
}
