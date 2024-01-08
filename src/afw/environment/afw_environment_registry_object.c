// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_object interface for _AdaptiveEnvironmentRegistry_
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_environment_registry_object.c
 * @brief Implementation of afw_object interface for
 *    _AdaptiveEnvironmentRegistry_
 */

#include "afw_internal.h"


#define impl_afw_object_get_meta \
    afw_object_impl_internal_get_meta

#define impl_afw_object_get_property_meta \
    afw_object_impl_internal_get_property_meta

#define impl_afw_object_get_next_property_meta \
    afw_object_impl_internal_get_next_property_meta

/* Declares and rti/inf defines for interface afw_object */
#define AFW_IMPLEMENTATION_ID "afw_environment_registry"
#include "afw_object_impl_declares.h"

static const afw_utf8_t impl_path =
AFW_UTF8_LITERAL("/afw/_AdaptiveEnvironmentRegistry_/current");

static const afw_object_t
impl_current_object = {
    &impl_afw_object_inf,
    NULL,
    {
        NULL,
        NULL,
        &afw_self_s_current,
        &afw_self_s__AdaptiveEnvironmentRegistry_,
        &impl_path
    }
};



/* Get current environment registry object. */
AFW_DEFINE(const afw_object_t *)
afw_environment_registry_object_get_current(afw_xctx_t *xctx)
{
    return &impl_current_object;
}



/*
 * Implementation of method release of interface afw_object.
 */
void
impl_afw_object_release (
    const afw_object_t * instance,
    afw_xctx_t *xctx)
{
    /* Nothing to do. */
}



/*
 * Implementation of method get_reference of interface afw_object.
 */
void
impl_afw_object_get_reference (
    const afw_object_t * instance,
    afw_xctx_t *xctx)
{
    /* Nothing to do. */
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



static afw_boolean_t
impl_entry_cb(
    const afw_object_t *object,
    void *context,
    afw_xctx_t *xctx)
{
    const afw_object_t *entries = (const afw_object_t *)context;
    const afw_utf8_t *property_name;

    if (object) {
        property_name = afw_object_meta_get_object_id(object, xctx);
        afw_object_set_property_as_object(entries, property_name,
            object, xctx);
    }

    return false;
}



static const afw_value_t *
impl_make_registry_type_value(
    const afw_utf8_t * object_type_id,
    afw_xctx_t *xctx)
{
    const afw_object_t *entries;

    entries = afw_object_create_managed(xctx->p, xctx);

    afw_runtime_foreach(object_type_id, (void *)entries,
        impl_entry_cb, xctx);

    return afw_value_create_unmanaged_object(entries, xctx->p, xctx);
}



/*
 * Implementation of method get_property of interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_property (
    const afw_object_t * instance,
    const afw_utf8_t * property_name,
    afw_xctx_t *xctx)
{
    const afw_environment_registry_type_t *type;

    type = afw_environment_get_registry_type(
        property_name, xctx);

    return (type && type->object_type_id)
        ? impl_make_registry_type_value(type->object_type_id, xctx)
        : NULL;
}



/*
 * Implementation of method get_next_property of interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_next_property (
    const afw_object_t * instance,
    const afw_iterator_t * * iterator,
    const afw_utf8_t * * property_name,
    afw_xctx_t *xctx)
{
    int registry_type_number;
    const afw_value_t *result = NULL;
    const afw_environment_registry_type_t *registry_type;

    for (
        registry_type_number = (int)(*((char **)iterator) - (char *)0);

        (registry_type = afw_environment_get_registry_type_by_number(
            registry_type_number, xctx));

        registry_type_number++)
    {
        if (registry_type->object_type_id) {
            result = impl_make_registry_type_value(
                registry_type->object_type_id, xctx);
            break;
        }
    }

    if (registry_type) {
        *(char **)iterator = (char *)0 + registry_type_number + 1;
        if (property_name) {
            *property_name = registry_type->property_name;
        }
    }
    else {
        *iterator = NULL;
        result = NULL;
        if (property_name) {
            *property_name = NULL;
        }
    }

    return result;
}



/*
 * Implementation of method has_property of interface afw_object.
 */
afw_boolean_t
impl_afw_object_has_property (
    const afw_object_t * instance,
    const afw_utf8_t * property_name,
    afw_xctx_t *xctx)
{
    return impl_afw_object_get_property(instance, property_name, xctx) != NULL;
}



/*
 * Implementation of method get_setter of interface afw_object.
 */
const afw_object_setter_t *
impl_afw_object_get_setter (
    const afw_object_t * instance,
    afw_xctx_t *xctx)
{
    /* Immutable. */
    return NULL;
}
