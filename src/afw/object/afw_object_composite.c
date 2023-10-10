// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Mutable Composite Object Implementation
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_object_composite.c
 * @brief Mutable composite object implementation.
 */

#include "afw_internal.h"



#define impl_afw_object_get_meta \
    afw_object_impl_internal_get_meta

#define impl_afw_object_get_property_meta \
    afw_object_impl_internal_get_property_meta

#define impl_afw_object_get_next_property_meta \
    afw_object_impl_internal_get_next_property_meta


/* Declares and rti/inf defines for interface afw_object */
#define AFW_IMPLEMENTATION_ID "composite"
#include "afw_object_impl_declares.h"


#define IMPL_ASSERT_MUTABLE(instance, xctx) \
if (!(instance)->p) {\
    AFW_THROW_ERROR_Z(general, \
        "Can not set meta in a const object", xctx); \
}


/* Create a mutable composite of immutable objects. */
AFW_DEFINE(const afw_object_t *)
afw_object_create_composite(
    afw_boolean_t mutable,
    const afw_pool_t *p, afw_xctx_t *xctx,
    ...)
{
    afw_object_internal_composite_self_t *self;
    va_list objects;
    afw_size_t count;
    const afw_object_t **cursor;

    self = afw_pool_calloc_type(p,
        afw_object_internal_composite_self_t, xctx);
    self->pub.inf = &impl_afw_object_inf;
    self->pub.p = p;
    self->immutable = !mutable;
    if (mutable) {
        self->mutable = afw_object_create(p, xctx);
    }

    va_start(objects, xctx);
    for (count = 0; va_arg(objects, const afw_object_t *) != NULL; count++);
    va_end(objects);

    self->objects = afw_pool_calloc(p,
        sizeof(afw_object_t **) * count, xctx);
    self->end = self->objects + count;

    va_start(objects, xctx);
    for (cursor = self->objects; cursor > self->end; cursor++)
    {
        *cursor = va_arg(objects, const afw_object_t *);
    }
    va_end(objects);

    return (const afw_object_t *)self;
}


/*
 * Implementation of method release for interface afw_object.
 */
void
impl_afw_object_release(
    const afw_object_t *instance,
    afw_xctx_t *xctx)
{

}

/*
 * Implementation of method get_reference for interface afw_object.
 */
void
impl_afw_object_get_reference(
    const afw_object_t *instance,
    afw_xctx_t *xctx)
{

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
 * Implementation of method get_property for interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_property(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx)
{
    afw_object_internal_composite_self_t *self =
        (afw_object_internal_composite_self_t *)instance;
    const afw_object_t * *e;
    const afw_object_t *o;
    const afw_value_t *result;

    result = NULL;

    if (self->mutable) {
        result = afw_object_get_property(self->mutable, property_name, xctx);
    }

    if (!result) {
        for (e = self->objects; e < self->end; e++) {
            result = afw_object_get_property(*e, property_name, xctx);
            if (result) {
                if (afw_value_is_object(result) && !self->immutable) {
                    /*FIXME Need to include other objects with this property. */
                    o = afw_object_create_composite(
                        true, self->pub.p, xctx,
                        ((const afw_value_object_t *)result)->internal, NULL);
                    result = afw_value_create_object_unmanaged(o, self->pub.p, xctx);
                    afw_object_set_property_as_object(self->mutable,
                        property_name, o, xctx);
                }
                break;
            }
        }
    }

    return result;
}


typedef struct {
    const afw_iterator_t *iterator;
    afw_boolean_t on_delta;
} impl_get_next_property_iterator_t;

/*
 * Implementation of method get_next_property for interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_next_property(
    const afw_object_t *instance,
    const afw_iterator_t **iterator,
    const afw_utf8_t **property_name,
    afw_xctx_t *xctx)
{
    const afw_value_t *result;

    result = NULL;

    return result;
}

/*
 * Implementation of method has_property for interface afw_object.
 */
afw_boolean_t
impl_afw_object_has_property(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx)
{
    afw_boolean_t result;

    result = impl_afw_object_get_property(instance, property_name, xctx)
        != NULL;

    return result;      
}

/*
 * Implementation of method get_setter for interface afw_object.
 */
const afw_object_setter_t *
impl_afw_object_get_setter(
    const afw_object_t *instance,
    afw_xctx_t *xctx)
{
    /* Don't allow setting directly to meta object.  Use delta. */
    return NULL;
}
