// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Properties Callback Object Implementation
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_object_properties_callback.c
 * @brief Properties callback object implementation.
 */

#include "afw_internal.h"



#define impl_afw_object_get_meta \
    afw_object_impl_internal_get_meta

#define impl_afw_object_get_property_meta \
    afw_object_impl_internal_get_property_meta

#define impl_afw_object_get_next_property_meta \
    afw_object_impl_internal_get_next_property_meta


/* Declares and rti/inf defines for interface afw_object */
#define AFW_IMPLEMENTATION_ID "properties_callback"
#include "afw_object_impl_declares.h"
#include "afw_object_setter_impl_declares.h"


#define IMPL_ASSERT_MUTABLE(instance, xctx) \
if (!(instance)->p) {\
    AFW_THROW_ERROR_Z(general, \
        "Can not set meta in a const object", xctx); \
}


/* Create a mutable composite of immutable objects. */
AFW_DEFINE(const afw_object_t *)
afw_object_create_properties_callback(
    void *data,
    afw_size_t count,
    const afw_object_properties_callback_entry_t callbacks[],
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_object_internal_properties_callback_self_t *self;

    self = afw_pool_calloc_type(p,
        afw_object_internal_properties_callback_self_t, xctx);
    self->pub.inf = &impl_afw_object_inf;
    self->pub.p = p;
    self->setter.inf = &impl_afw_object_setter_inf;
    self->setter.object = (const afw_object_t *)self;
    self->data = data;
    self->callbacks = &callbacks[0];
    self->end = &callbacks[count];
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
 * Implementation of method add_reference for interface afw_object.
 */
void
impl_afw_object_add_reference(
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
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx)
{
    afw_object_internal_properties_callback_self_t *self =
        (afw_object_internal_properties_callback_self_t *)instance;
    const afw_object_properties_callback_entry_t *e;
    const afw_value_t *result;

    result = NULL;
    for (e = self->callbacks; e < self->end; e++)
    {
        if (afw_utf8_equal(e->property_name, property_name)) {
            result = e->get(self, property_name, xctx);
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
    const afw_object_t *instance,
    const afw_iterator_t **iterator,
    const afw_utf8_t **property_name,
    afw_xctx_t *xctx)
{
    afw_object_internal_properties_callback_self_t *self =
        (afw_object_internal_properties_callback_self_t *)instance;
    const afw_value_t *result;
    const afw_object_properties_callback_entry_t *e;

    result = NULL;
    if (property_name) {
        *property_name = NULL;
    }

    if (!*iterator) {
        *iterator = (afw_iterator_t *)self->callbacks;
    }

    for (e = self->callbacks; e < self->end; e++)
    {
        result = e->get(self, e->property_name, xctx);
        if (result) {
            *iterator = (afw_iterator_t *)(e + 1);
            if (property_name) {
                *property_name = e->property_name;
            }
            break;
        }
    }

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
    afw_object_internal_properties_callback_self_t *self =
        (afw_object_internal_properties_callback_self_t *)instance;

    return (self->immutable) ? NULL : &self->setter;
}

/*
 * Implementation of method set_immutable for interface afw_object_setter.
 */
void
impl_afw_object_setter_set_immutable(
    const afw_object_setter_t * instance,
    afw_xctx_t *xctx)
{
    afw_object_internal_properties_callback_self_t *self =
        (afw_object_internal_properties_callback_self_t *)instance->object;

    self->immutable = true;
}

/*
 * Implementation of method set_property for interface afw_object_setter.
 */
void
impl_afw_object_setter_set_property(
    const afw_object_setter_t * instance,
    const afw_utf8_t * property_name,
    const afw_value_t * value,
    afw_xctx_t *xctx)
{
    afw_object_internal_properties_callback_self_t *self =
        (afw_object_internal_properties_callback_self_t *)instance->object;
    const afw_object_properties_callback_entry_t *e;

    if (!self->immutable) {
        for (e = self->callbacks; e < self->end; e++)
        {
            if (afw_utf8_equal(e->property_name, property_name)) {
                if (!e->set) {
                    break;
                }
                e->set(self->data, property_name, value, xctx);
                return;
            }
        }
    }

    AFW_OBJECT_ERROR_PROPERTY_IMMUTABLE(property_name);
}
