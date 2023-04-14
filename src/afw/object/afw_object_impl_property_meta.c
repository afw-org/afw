// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Object Implementation Helpers
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_object_impl.c
 * @brief Object interface implementation helpers.
 */

#include "afw_internal.h"

#define impl_afw_object_get_meta \
    afw_object_impl_internal_get_meta

#define impl_afw_object_get_property_meta \
    afw_object_impl_internal_get_property_meta

#define impl_afw_object_get_next_property_meta \
    afw_object_impl_internal_get_next_property_meta

/*
 * This implementation is a helper to support:
 * impl_afw_object_get_meta,
 * impl_afw_object_get_property_meta, and
 * impl_afw_object_get_next_property_meta.
 *
 * See afw_object_impl_internal_get_meta,
 * afw_object_impl_internal_get_property_meta, and
 * impl_afw_object_get_next_property_meta.
 *
 * The implementation accesses the meta of an object instead of
 * the just the property values.
 */
/* Declares and rti/inf defines for interface afw_object */
#define AFW_IMPLEMENTATION_ID "object_impl_property_meta"
#include "afw_object_impl_declares.h"
#include "afw_object_setter_impl_declares.h"



static const afw_value_t *
impl_get_dataType(
    afw_object_impl_property_meta_object_self_t *self,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx)
{
    const afw_data_type_t *data_type;
    const afw_value_t *result;

    result = NULL;

    data_type = afw_value_get_data_type(self->property_value, xctx);
    if (data_type) {
        result = afw_value_create_string(
            &data_type->data_type_id, self->pub.p, xctx);
    }

    return result;
}


static void
impl_set_dataType(
    afw_object_impl_property_meta_object_self_t *self,
    const afw_utf8_t *property_name,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    AFW_THROW_ERROR_Z(general, "Set dataType not implemented.", xctx);
}


static const afw_value_t *
impl_get_key(
    afw_object_impl_property_meta_object_self_t *self,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx)
{
    const afw_value_t *result;

    result = NULL;

    if (self->property_name) {
        result = afw_value_create_string(
            self->property_name, self->pub.p, xctx);
    }

    return result;
}


static const afw_value_t *
impl_get_value(
    afw_object_impl_property_meta_object_self_t *self,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx)
{
    return self->property_value;
}


static void
impl_set_value(
    afw_object_impl_property_meta_object_self_t *self,
    const afw_utf8_t *property_name,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    afw_object_set_property(self->owning_object,
        self->property_name, value, xctx);
}


static const afw_value_t *
impl_get_valueInfId(
    afw_object_impl_property_meta_object_self_t *self,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx)
{
    const afw_value_t *result;

    result = afw_value_create_string(
        &self->property_value->inf->rti.implementation_id,
        self->pub.p, xctx);

    return result;
}



static void
impl_set_error_immutable(
    afw_object_impl_property_meta_object_self_t *self,
    const afw_utf8_t *property_name,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    AFW_THROW_ERROR_FZ(general, xctx,
        "Property %" AFW_UTF8_FMT " immutable",
        AFW_UTF8_FMT_ARG(property_name));
}



static const afw_object_impl_name_handler_t impl_handler[] =
{
    { &afw_s_dataType, impl_get_dataType, impl_set_dataType },
    { &afw_s_key, impl_get_key, impl_set_error_immutable },
    { &afw_s_value, impl_get_value, impl_set_value },
    { &afw_s_valueInfId, impl_get_valueInfId, impl_set_error_immutable }
};

static const afw_object_impl_name_handler_t *impl_handler_end =
(const afw_object_impl_name_handler_t *)
(((char *)& impl_handler[0]) + sizeof(impl_handler));



/*
 * Implementation of method release for interface afw_object.
 */
void
impl_afw_object_release(
    const afw_object_t *instance,
    afw_xctx_t *xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}



/*
 * Implementation of method add_reference for interface afw_object.
 */
void
impl_afw_object_add_reference(
    const afw_object_t *instance,
    afw_xctx_t *xctx)
{
    /* Nothing to do? */
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
    afw_object_impl_property_meta_object_self_t *self =
        (afw_object_impl_property_meta_object_self_t *)instance;
    const afw_object_impl_name_handler_t *h;

    /* If get callback for this property, return it's result. */
    for (h = &impl_handler[0]; h < impl_handler_end; h++) {
        if (afw_utf8_equal(h->property_name, property_name)) {
            if (h->get) {
                return h->get(self, property_name, xctx);
            }
            break;
        }
    }

    /* If property_type_object, get property from it. */
    if (self->property_type_object) {
        return afw_object_get_property(
            self->property_type_object,
            property_name, xctx);
    }

    /* Return NULL if not found. */
    return NULL;
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
    afw_object_impl_property_meta_object_self_t *self =
        (afw_object_impl_property_meta_object_self_t *)instance;
    const afw_object_impl_name_handler_t *h;
    const afw_value_t *result;
    const afw_utf8_t *next_property_name;

    /*
     * If this is the first time (*iterator is NULL), set iterator to first
     * impl_handler[] entry.
     */
    if (!*iterator) {
        *iterator = (const afw_iterator_t *)&impl_handler[0];
    }

    /*
     * If *iterator is within bounds for impl_handler array, *iterator points
     * to the next impl_handler array entry to process.  Loop calling the
     * callback of each entry from this point until a callback returns a
     * non-NULL result or the end of the array is encountered.  Set *iterator
     * to the next entry to process or the end of the array.
     *
     * If a callback returns a non-NULL result, return from this function
     * with that result.
     */
    if (*(void **)iterator >= (void *)&impl_handler[0])
    {
        h = (const afw_object_impl_name_handler_t *)*iterator;
        for (; h < impl_handler_end; h++)
        {
            result = h->get(self, h->property_name, xctx);
            *iterator = (const afw_iterator_t *)(h + 1);
            if (result) {
                if (property_name) {
                    *property_name = h->property_name;
                }
                return result;
            }
        }
    }

    /*
     * If there is an additional object that contains properties to return,
     * return the result of calling its get_next_property() if it's not for
     * a property name in impl_handler array.  If the previous iterator points
     * to the end of impl_handler indicating that the impl_handler array has
     * just been completely processed, set the iterator to NULL before calling
     * get_next_property().
     */
    if (self->property_type_object) {
        if ((const afw_object_impl_name_handler_t *)*iterator == impl_handler_end)
        {
            *iterator = NULL;
        }
        for (;;) {
            result = afw_object_get_next_property(self->property_type_object,
                iterator, &next_property_name, xctx);
            if (!result) {
                break;
            }
            for (h = &impl_handler[0];
                h < impl_handler_end &&
                !afw_utf8_equal(h->property_name, next_property_name);
                h++);
            if (h >= impl_handler_end) {
                if (property_name) {
                    *property_name = next_property_name;
                }
                return result;
            }
        }
    }

    /* At this point there are no more properties to return. */
    *iterator = NULL;
    if (property_name) {
        *property_name = NULL;
    }
    return NULL;
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
    return impl_afw_object_get_property(instance, property_name, xctx) != NULL;
}



/*
 * Implementation of method get_setter for interface afw_object.
 */
const afw_object_setter_t *
impl_afw_object_get_setter(
    const afw_object_t *instance,
    afw_xctx_t *xctx)
{
    afw_object_impl_property_meta_object_self_t *self =
        (afw_object_impl_property_meta_object_self_t *)instance;

    return (!self->is_immutable &&
        afw_object_get_setter(self->owning_object, xctx))
        ? &self->setter
        : NULL;
}



/*
 * Implementation of method set_immutable for interface afw_object_setter.
 */
void
impl_afw_object_setter_set_immutable(
    const afw_object_setter_t *instance,
    afw_xctx_t *xctx)
{
    afw_object_impl_property_meta_object_self_t *self =
        (afw_object_impl_property_meta_object_self_t *)instance->object;

    self->is_immutable = true;
}



/*
 * Implementation of method set_property for interface afw_object_setter.
 */
void
impl_afw_object_setter_set_property(
    const afw_object_setter_t *instance,
    const afw_utf8_t *property_name,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    afw_object_impl_property_meta_object_self_t *self =
        (afw_object_impl_property_meta_object_self_t *)instance->object;
    const afw_object_impl_name_handler_t *h;

    for (h = &impl_handler[0]; h < impl_handler_end; h++) {
        if (afw_utf8_equal(h->property_name, property_name)) {
            if (h->set) {
                h->set(self, property_name, value, xctx);
                return;
            }
            break;
        }
    }

    afw_object_meta_set_property_type_property(
        ((const afw_value_object_t *)&self->meta_object_value)->internal,
        self->property_name,
        property_name, value, xctx);
}



/*
 * A general impl of method get_property_meta for interface afw_object
 * that can be accessed externally.
 */
AFW_DECLARE(const afw_value_t *)
afw_object_impl_get_property_meta(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return afw_object_impl_internal_get_property_meta(
        instance, property_name, p, xctx);
}



/*
 * A general impl of method get_next_property_meta for interface
 * afw_object that can be accessed externally.
 */
AFW_DECLARE(const afw_value_t *)
afw_object_impl_get_next_property_meta(
    const afw_object_t *instance,
    const afw_iterator_t **iterator,
    const afw_utf8_t **property_name,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return afw_object_impl_internal_get_next_property_meta(
        instance, iterator, property_name, p, xctx);
}



/*
 * Implementation of method get_property_meta for interface afw_object.
 */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_object_impl_internal_get_property_meta(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_object_impl_property_meta_object_self_t *self;
    const afw_value_t *property_value;

    property_value = afw_object_get_property(instance,
        property_name, xctx);
    if (!property_value) {
        /* Possibly have a future option to get meta with no value??? */
        return NULL;
    }

    self = afw_pool_calloc_type(p, afw_object_impl_property_meta_object_self_t, xctx);
    self->pub.inf = &impl_afw_object_inf;
    self->pub.p = p;
    self->owning_object = instance;
    self->setter.inf = &impl_afw_object_setter_inf;
    self->setter.object = (const afw_object_t *)self;
    self->meta_object_value.inf = &afw_value_evaluated_object_inf;
    self->meta_object_value.internal = (const afw_object_t *)self;
    self->property_value = property_value;
    self->property_value = afw_value_evaluate(property_value, p, xctx);
    self->property_name = property_name;
    self->property_type_object =
        afw_object_meta_get_property_type(instance, property_name, xctx);

    return (const afw_value_t *)& self->meta_object_value;
}



/*
 * Implementation of method get_next_property_meta for interface afw_object.
 */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_object_impl_internal_get_next_property_meta(
    const afw_object_t *instance,
    const afw_iterator_t **iterator,
    const afw_utf8_t **property_name,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *property_value;
    const afw_value_t *result;
    const afw_utf8_t *next_property_name;

    result = NULL;
    property_value = afw_object_get_next_property(instance,
        iterator, &next_property_name, xctx);

    if (property_value) {
        if (property_name) {
            *property_name = next_property_name;
        }
        result = afw_object_get_property_meta(instance,
            next_property_name, p, xctx);
    }

    return result;
}
