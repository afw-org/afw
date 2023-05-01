// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_object Implementation for <afwdev {srcdir}>
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_object_const_key_value.c
 * @brief Implementation of afw_object interface for <afwdev {srcdir}>
 */

#include "afw_internal.h"



#define impl_afw_object_get_meta \
    afw_object_impl_internal_get_meta

#define impl_afw_object_get_property_meta \
    afw_object_impl_internal_get_property_meta

#define impl_afw_object_get_next_property_meta \
    afw_object_impl_internal_get_next_property_meta

/* Declares and rti/inf defines for interface afw_object */
#define AFW_IMPLEMENTATION_ID "afw_object_const_key_value"
#include "afw_object_impl_declares.h"



/* Create a const object from key/value utf-8 strings. */
AFW_DEFINE(const afw_object_t *)
afw_object_create_const_from_key_value_strings_z(
    const afw_utf8_t *path,
    const afw_utf8_z_t * const *pairs,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_object_const_key_value_self_t *self;
    const afw_utf8_z_t * const *c;
    afw_object_const_key_value_string_entry_t *e;

    self = afw_pool_calloc_type(p,
        afw_object_const_key_value_self_t, xctx);
    self->pub.inf = &impl_afw_object_inf;
    self->pub.p = p;

    /* Count number of pairs and allocate memory for key/values */
    for (c = pairs; *c; self->entry_count++, c += 2) {
        if (!*(c + 1)) {
            AFW_THROW_ERROR_Z(general, "Missing value for pair", xctx);
        }
    }
    self->entry = e = afw_pool_calloc(p,
        self->entry_count * sizeof(afw_object_const_key_value_self_t), xctx);
    self->entry_end = e + self->entry_count;

    /* Set entries */
    for (c = pairs; *c; c += 2, e++) {
        e->key.s = *c;
        e->key.len = strlen(*c);
        e->value.inf = &afw_value_evaluated_string_inf;
        e->value.internal.s = *(c + 1);
        e->value.internal.len = strlen(*(c + 1));
    }

    /* Set ids. */
    afw_object_meta_set_ids_using_path(
        (const afw_object_t *)self,
        path, xctx);

    /* Return objects. */
    return (const afw_object_t *)self;
}



/*
 * Implementation of method release for interface afw_object.
 */
void
impl_afw_object_release(
    const afw_object_t * instance,
    afw_xctx_t *xctx)
{
    /* Not reference counted. */
}

/*
 * Implementation of method add_reference for interface afw_object.
 */
void
impl_afw_object_add_reference(
    const afw_object_t * instance,
    afw_xctx_t *xctx)
{
    /* Not reference counted. */
}

/*
 * Implementation of method get_count for interface afw_object.
 */
afw_size_t
impl_afw_object_get_count(
    const afw_object_t * instance,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_entry for interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_entry(
    const afw_object_t * instance,
    afw_integer_t index,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_entry_meta for interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_entry_meta(
    const afw_object_t * instance,
    afw_integer_t index,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)instance;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_property for interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_property(
    const afw_object_t * instance,
    const afw_utf8_t * property_name,
    afw_xctx_t *xctx)
{
    afw_object_const_key_value_self_t *self =
        (afw_object_const_key_value_self_t *)instance;
    const afw_object_const_key_value_string_entry_t *e;

    for (e = self->entry; e < self->entry_end; e++) {
        if (afw_utf8_equal(&e->key, property_name)) {
            return (const afw_value_t *)&e->value;
        }
    }

    return NULL;
}

/*
 * Implementation of method get_next_property for interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_next_property(
    const afw_object_t * instance,
    const afw_iterator_t * * iterator,
    const afw_utf8_t * * property_name,
    afw_xctx_t *xctx)
{
    afw_object_const_key_value_self_t *self =
        (afw_object_const_key_value_self_t *)instance;
    const afw_object_const_key_value_string_entry_t *e;
    const afw_value_t *result;

    e = (const afw_object_const_key_value_string_entry_t *)*iterator;
    if (!e) {
        e = self->entry;
    }

    if (e < self->entry_end) {
        result = (const afw_value_t *)& e->value;
        if (property_name) {
            *property_name = &e->key;
        }
        *iterator = (afw_iterator_t *)(e + 1);
    }
    else {
        result = NULL;
        if (property_name) {
            *property_name = NULL;
        }
        *iterator = NULL;
    }

    return result;
}

/*
 * Implementation of method has_property for interface afw_object.
 */
afw_boolean_t
impl_afw_object_has_property(
    const afw_object_t * instance,
    const afw_utf8_t * property_name,
    afw_xctx_t *xctx)
{
    return impl_afw_object_get_property(instance, property_name, xctx) != NULL;
}

/*
 * Implementation of method get_setter for interface afw_object.
 */
const afw_object_setter_t *
impl_afw_object_get_setter(
    const afw_object_t * instance,
    afw_xctx_t *xctx)
{
    return NULL;
}
