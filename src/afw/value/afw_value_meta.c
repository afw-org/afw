// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Object Implementation for an Adaptive Value's Meta
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_value_meta.c
 * @brief Object implementation an adaptive value's meta.
 */

#include "afw_internal.h"


#define impl_afw_object_get_meta \
    afw_object_impl_internal_get_meta

#define impl_afw_object_get_property_meta \
    afw_object_impl_internal_get_property_meta

#define impl_afw_object_get_next_property_meta \
    afw_object_impl_internal_get_next_property_meta


/* Declares and rti/inf defines for interface afw_object */
#define AFW_IMPLEMENTATION_ID "value_meta"
#define AFW_OBJECT_SELF_T afw_value_meta_object_self_t
#include "afw_object_impl_declares.h"
#include "afw_object_setter_impl_declares.h"

static const afw_value_t *
impl_get_dataType(
    afw_value_meta_object_self_t *self,
    const afw_value_t *property_name,
    afw_xctx_t *xctx)
{
    const afw_data_type_t *data_type;
    const afw_value_t *result;

    result = NULL;

    data_type = afw_value_get_data_type(self->evaluated_value, xctx);
    if (data_type) {
        result = afw_value_create_unmanaged_string(
            &data_type->data_type_id, self->pub.p, xctx);
    }

    return result;
}


static const afw_value_t *
impl_get_key(
    afw_value_meta_object_self_t *self,
    const afw_value_t *property_name,
    afw_xctx_t *xctx)
{
    const afw_value_t *result;

    result = NULL;

    if (self->key) {
        result = afw_value_create_unmanaged_string(
            self->key, self->pub.p, xctx);
    }

    return result;
}


static const afw_value_t *
impl_get_value(
    afw_value_meta_object_self_t *self,
    const afw_value_t *property_name,
    afw_xctx_t *xctx)
{
    return self->evaluated_value;
}


static const afw_value_t *
impl_get_valueInfId(
    afw_value_meta_object_self_t *self,
    const afw_value_t *property_name,
    afw_xctx_t *xctx)
{
    const afw_value_t *result;

    result = afw_value_create_unmanaged_string(
        &self->associated_value->inf->rti.implementation_id,
        self->pub.p, xctx);

    return result;
}


static const afw_value_meta_name_handler_t
impl_handler[] =
{
    { afw_v_dataType, impl_get_dataType, NULL },
    { afw_v_key, impl_get_key, NULL },
    { afw_v_value, impl_get_value, NULL },
    { afw_v_valueInfId, impl_get_valueInfId, NULL }
};

static const afw_value_meta_name_handler_t *impl_handler_end =
(const afw_value_meta_name_handler_t *)
    ((char *)&impl_handler[0] + sizeof(impl_handler));



/*
 * Implementation of method release for interface afw_object.
 */
void
impl_afw_object_release(
    AFW_OBJECT_SELF_T *self,
    afw_xctx_t *xctx)
{
    /* Unmanaged grab-and-read; slot last-release may call this. */
    (void)self;
    (void)xctx;
}



/*
 * Implementation of method get_reference for interface afw_object.
 */
void
impl_afw_object_get_reference(
    AFW_OBJECT_SELF_T *self,
    afw_xctx_t *xctx)
{
    /* Nothing to do? */
}

/*
 * Implementation of method get_count for interface afw_object.
 */
afw_size_t
impl_afw_object_get_count(
    AFW_OBJECT_SELF_T *self,
    afw_xctx_t * xctx)
{
//    <afwdev {prefixed_interface_name}>_self_t *self =
//        (<afwdev {prefixed_interface_name}>_self_t *)&self->pub;

    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

}

/*
 * Implementation of method get_property for interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_property(
    AFW_OBJECT_SELF_T *self,
    const afw_value_t *property_name,
    afw_xctx_t *xctx)
{
    const afw_value_meta_name_handler_t *h;

    /* If get callback for this property, return it's result. */
    for (h = &impl_handler[0]; h < impl_handler_end; h++) {
        if (afw_value_equal(h->property_name, property_name, xctx)) {
            if (h->get) {
                return h->get(self, property_name, xctx);
            }
            break;
        }
    }

    /* If additional, get property from it. */
    if (self->additional) {
        return afw_object_get_property(self->additional,
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
    AFW_OBJECT_SELF_T *self,
    const afw_iterator_old_t **iterator,
    const afw_value_t **property_name,
    afw_xctx_t *xctx)
{
    const afw_value_meta_name_handler_t *h;
    const afw_value_t *result;
    const afw_value_t *next_property_name;

    /*
     * If this is the first time (*iterator is NULL), set iterator to first
     * impl_handler[] entry.
     */
    if (!*iterator) {
        *iterator = (const afw_iterator_old_t *)&impl_handler[0];
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
        h = (const afw_value_meta_name_handler_t *)*iterator;
        for (; h < impl_handler_end; h++)
        {
            result = h->get(self, h->property_name, xctx);
            *iterator = (const afw_iterator_old_t *)(h + 1);
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
    if (self->additional) {
        if ((const afw_value_meta_name_handler_t *)*iterator ==
            impl_handler_end)
        {
            *iterator = NULL;
        }
        for (;;) {
            result = afw_object_get_next_property(self->additional,
                iterator, &next_property_name, xctx);
            if (!result) {
                break;
            }
            for (h = &impl_handler[0];
                h < impl_handler_end &&
                !afw_value_equal(h->property_name, next_property_name, xctx);
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
    AFW_OBJECT_SELF_T *self,
    const afw_value_t *property_name,
    afw_xctx_t *xctx)
{
    return
        (impl_afw_object_get_property(self, property_name, xctx) != NULL);
}



/*
 * Implementation of method get_setter for interface afw_object.
 */
const afw_object_setter_t *
impl_afw_object_get_setter(
    AFW_OBJECT_SELF_T *self,
    afw_xctx_t *xctx)
{

    return (self->immutable) ? NULL : &self->setter;
}



/*
 * Implementation of method set_immutable for interface afw_object_setter.
 */
void
impl_afw_object_setter_set_immutable(
    const afw_object_setter_t *self,
    afw_xctx_t *xctx)
{
    afw_value_meta_object_self_t *meta_object_self =
        (afw_value_meta_object_self_t *)self->object;

    /* Set object to immutable. */
    meta_object_self->immutable = true;
}

/*
 * Implementation of method set_property for interface afw_object_setter.
 */
void
impl_afw_object_setter_set_property(
    const afw_object_setter_t *self,
    const afw_value_t *property_name,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    afw_value_meta_object_self_t *meta_object_self =
        (afw_value_meta_object_self_t *)self->object;
    const afw_value_meta_name_handler_t *h;

    do { if (meta_object_self->immutable) { AFW_OBJECT_ERROR_OBJECT_IMMUTABLE; } } while (0);

    /* If get callback for this property, return it's result. */
    for (h = &impl_handler[0]; h < impl_handler_end; h++) {
        if (afw_value_equal(h->property_name, property_name, xctx)) {
            if (h->set) {
                h->set(meta_object_self, property_name, value, xctx);
                return;
            }
            break;
        }
    }

    if (!meta_object_self->additional) {
        meta_object_self->additional = afw_object_create_unmanaged(meta_object_self->pub.p, xctx);
    }

    afw_object_set_property(meta_object_self->additional, property_name, value, xctx);
}


/*
 * Implementation of method set_property_internal for interface
 * afw_object_setter.
 */
void
impl_afw_object_setter_set_property_internal(
    const afw_object_setter_t *self,
    const afw_value_t *property_name,
    const afw_data_type_t *data_type,
    const void *internal,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;
    const afw_pool_t *p;

    if (!internal) {
        impl_afw_object_setter_set_property(self, property_name, NULL, xctx);
        return;
    }
    p = self->object->p;
    if (!p) {
        p = xctx->p;
    }
    value = afw_value_common_create(internal, data_type, p, xctx);
    impl_afw_object_setter_set_property(self, property_name, value, xctx);
}



afw_value_meta_object_self_t *
afw_value_internal_create_meta_object_self(
    const afw_value_t *associated_value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_meta_object_self_t *self;

    self = afw_pool_calloc_type(p, afw_value_meta_object_self_t, xctx);

    self->pub.inf = &impl_afw_object_inf;
    self->pub.p = p;
    self->associated_value = associated_value;
    self->evaluated_value = afw_value_evaluate(associated_value, p, xctx);
    self->meta_object_value.inf = &afw_value_unmanaged_object_inf;
    self->meta_object_value.internal = (const afw_object_t *)self;
    /* Dual face: instance->value must point at this object's Adaptive value. */
    self->pub.value = (const afw_value_t *)&self->meta_object_value;
    self->setter.inf = &impl_afw_object_setter_inf;
    self->setter.object = (const afw_object_t *)self;
    /* Script-facing: meta() is read-only. C uses afw_object_meta_set_*. */
    self->immutable = true;

    /*? self->pub.meta */

    return self;
}



const afw_value_t *
afw_value_internal_get_evaluated_meta_default(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_meta_object_self_t *self;

    self = afw_value_internal_create_meta_object_self(
        value, p, xctx);
    return &self->meta_object_value.pub;
}



const afw_value_t *
afw_value_internal_get_evaluated_metas_default(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t **metas;
    const afw_array_t *list;

    metas = afw_pool_malloc(p, sizeof(afw_value_t *) * 2, xctx);
    metas[0] = afw_value_get_evaluated_meta(value, p, xctx);
    metas[1] = NULL;
    list = afw_array_create_unmanaged_from_null_terminated_values(
        NULL, metas, p, xctx);

    return afw_value_create_unmanaged_array(list, p, xctx);
}



const afw_value_t *
afw_value_internal_get_evaluated_meta_for_array(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_meta_object_self_t *self;

    /*FIXME Meta should include extra list meta. */

    self = afw_value_internal_create_meta_object_self(
        value, p, xctx);
    return &self->meta_object_value.pub;
}



const afw_value_t *
afw_value_internal_get_evaluated_metas_for_array(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    /* Lazy immutable view of meta() for each array entry. */
    return afw_value_meta_values_list_for_list_create(value, p, xctx);
}



const afw_value_t *
afw_value_internal_get_evaluated_meta_for_object(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    //return afw_object_get_meta(
    //    ((const afw_value_object_t *)value)->internal,
    //    p, xctx);
    /*FIXME Meta should include extra object meta. */

    afw_value_meta_object_self_t *self;

    self = afw_value_internal_create_meta_object_self(
        value, p, xctx);
    self->additional = (((afw_value_object_t *)value)->internal)->
        meta.meta_object;
    return &self->meta_object_value.pub;
}



const afw_value_t *
afw_value_internal_get_evaluated_metas_for_object(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    /* Lazy immutable view of meta() for each object property value. */
    return afw_value_meta_values_list_for_object_create(value, p, xctx);
}
