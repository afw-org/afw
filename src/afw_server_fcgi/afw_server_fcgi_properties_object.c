// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_object interface for FCGI properties
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_server_fcgi_properties_object.c
 * @brief Implementation of afw_object interface for FCGI properties
 */

#include "afw.h"
#include "afw_server_fcgi_internal.h"
#include "afw_object_impl.h"


/* Declares and rti/inf defines for interface afw_object */
#define AFW_IMPLEMENTATION_ID "fcgi"
#include "afw_object_impl_declares.h"

typedef struct impl_self_s {
    afw_object_t pub;
    afw_server_fcgi_internal_request_t *request;
    const afw_object_t *properties;
} impl_self_t;


const afw_object_t *
afw_server_fcgi_internal_create_properties_object(
    afw_server_fcgi_internal_request_t *request,
    afw_xctx_t *xctx)
{
    impl_self_t *self;

    static const afw_utf8_t impl_path = 
        AFW_UTF8_LITERAL("/afw/_AdaptiveRequestProperties_/current");

    /* Allocate memory for self and initialize. */
    self = afw_xctx_calloc_type(impl_self_t, request->pub.xctx);
    self->pub.inf = &impl_afw_object_inf;
    self->pub.meta.id = afw_s_current;
    self->pub.meta.object_type_uri = afw_s__AdaptiveRequestProperties_;
    self->pub.meta.object_uri = &impl_path;
    self->pub.p = xctx->p;
    self->request = request;

    /* Create request properties object. */
    self->properties = afw_object_create_unmanaged(xctx->p, xctx);

    return (const afw_object_t *)self;
}



/*
 * Implementation of method release of interface afw_object.
 */
void
impl_afw_object_release(
    const afw_object_t * instance,
    afw_xctx_t *xctx)
{
    /* Always releases with xctx. */
}



/*
 * Implementation of method get_reference of interface afw_object.
 */
void
impl_afw_object_get_reference (
    const afw_object_t * instance,
    afw_xctx_t *xctx)
{
    /* Always releases with xctx. */
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
 * Implementation of method get_meta for interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_meta(
    const afw_object_t *instance,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return afw_object_impl_get_meta(
        instance, p, xctx);
}



/*
 * Implementation of method get_property of interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_property(
    const afw_object_t * instance,
    const afw_utf8_t * property_name,
    afw_xctx_t *xctx)
{
    impl_self_t *self = (impl_self_t *)instance;
    const void *s;
    const afw_value_t *value;
    const afw_utf8_z_t *property_name_z;

    /* Look for property in memory first. */
    value = afw_object_get_property(self->properties, property_name,
        xctx);
    if (value) return value;

    /* If not in memory, try FCGX_GetParam(). */
    value = NULL;
    property_name_z = afw_utf8_z_create(property_name->s, property_name->len,
        xctx->p, xctx);
    s = FCGX_GetParam(property_name_z,
        self->request->fcgx_request->envp);
    if (s) {
        value = afw_value_make_string_copy(s, AFW_UTF8_Z_LEN, xctx->p, xctx);
    }

    return value;
}



/*
 * Implementation of method get_property_meta for interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_property_meta(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return afw_object_impl_get_property_meta(
        instance, property_name, p, xctx);
}



typedef struct impl_request_properties_iterator_s {
    const afw_iterator_t *iterator;
    char **envp;
} impl_request_properties_iterator_t;



/*
 * Implementation of method get_next_property of interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_next_property(
    const afw_object_t * instance,
    const afw_iterator_t * * iterator,
    const afw_utf8_t * * property_name,
    afw_xctx_t *xctx)
{
    impl_self_t *self = (impl_self_t *)instance;
    impl_request_properties_iterator_t *it;
    const afw_value_t *value;
    const afw_utf8_octet_t *s;
    const afw_utf8_octet_t *c;
    const afw_utf8_t *string;

    if (!*iterator) {
        *iterator = (afw_iterator_t *)afw_xctx_calloc_type(
            impl_request_properties_iterator_t, xctx);

    }
    it = (impl_request_properties_iterator_t *)*iterator;

    if (!it->envp) {
        value = afw_object_get_next_property(self->properties,
            &it->iterator, property_name, xctx);
        if (value) {
            return value;
        }
        it->envp = self->request->fcgx_request->envp;
    }

    if (!it->envp || !*it->envp) {
        *iterator = NULL;
        return NULL;
    }

    for (s = c = *it->envp; *c && *c != '='; c++);
    if (property_name) {
        *property_name = afw_utf8_create(s, c - s, xctx->p, xctx);
    }

    if (!*c) {
        string = afw_s_a_empty_string;
    }
    else {
        string = afw_utf8_create(c + 1, AFW_UTF8_Z_LEN, xctx->p, xctx);
    }

    value = afw_value_create_string(string, xctx->p, xctx);
    (it->envp)++;
    return value;
}



/*
 * Implementation of method get_next_property_meta for interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_next_property_meta(
    const afw_object_t *instance,
    const afw_iterator_t **iterator,
    const afw_utf8_t **property_name,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return afw_object_impl_get_next_property_meta(
        instance, iterator, property_name, p, xctx);
}



/*
 * Implementation of method has_property of interface afw_object.
 */
afw_boolean_t
impl_afw_object_has_property(
    const afw_object_t * instance,
    const afw_utf8_t * property_name,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;

    /* Call impl_get_own_property and throw away value. */
    value = impl_afw_object_get_property(
        instance, property_name, xctx);

    return (value != NULL);
}



/*
 * Implementation of method get_setter of interface afw_object.
 */
const afw_object_setter_t *
impl_afw_object_get_setter (
    const afw_object_t * instance,
    afw_xctx_t *xctx)
{
    /* Assign instance pointer to self. */
    impl_self_t * self = (impl_self_t *)instance;

    return afw_object_get_setter(self->properties, xctx);
}
