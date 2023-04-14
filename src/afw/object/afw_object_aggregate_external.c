// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive framework memory functions
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_object_aggregate_external.c
 * @brief Implementation for aggregate object external.
 *
 * This is the implementation of afw_object for aggregate object external.
 */

#include "afw_internal.h"

#define impl_afw_object_get_meta afw_object_impl_internal_get_meta

#define impl_afw_object_get_property_meta \
    afw_object_impl_internal_get_property_meta

#define impl_afw_object_get_next_property_meta \
    afw_object_impl_internal_get_next_property_meta

/* Declares and rti/inf defines for interface afw_object */
#define AFW_IMPLEMENTATION_ID "afw_object_aggregate_external"
#include "afw_object_impl_declares.h"



AFW_DEFINE(const afw_object_t *)
afw_object_aggregate_external_create(
    const afw_object_t * const *object_list,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_object_aggregate_external_self_t *self;

    self = afw_pool_calloc_type(p, afw_object_aggregate_external_self_t, xctx);
    self->pub.inf = &impl_afw_object_inf;
    self->pub.p = p;
    self->object_list = object_list;
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
    /* Not managed. */
}

/*
 * Implementation of method add_reference for interface afw_object.
 */
void
impl_afw_object_add_reference(
    const afw_object_t * instance,
    afw_xctx_t *xctx)
{
    /* Not managed. */
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
    afw_object_aggregate_external_self_t *self =
        (afw_object_aggregate_external_self_t *)instance;
    const afw_object_t * const *obj;
    const afw_value_t *result;

    for (obj = self->object_list, result = NULL; *obj; obj++) {
        result = afw_object_get_property(*obj, property_name, xctx);
        if (result) {
            break;
        }
    }
    return result;
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
    afw_object_aggregate_external_self_t *self =
        (afw_object_aggregate_external_self_t *)instance;
    const afw_value_t *result;

    if (!*iterator) {
        *iterator = (const afw_iterator_t *)&self->iterator;
        self->iterator.object = self->object_list;
        self->iterator.iterator = NULL;
    }

    result = NULL;
    for (; *self->iterator.object; self->iterator.object++) {
        result = afw_object_get_next_property(
            *self->iterator.object, &self->iterator.iterator,
            property_name, xctx);
        if (result) {
            break;
        }       
    }

    if (!result) {
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
    afw_object_aggregate_external_self_t *self =
        (afw_object_aggregate_external_self_t *)instance;

    return afw_object_get_setter(*self->object_list, xctx);
}
