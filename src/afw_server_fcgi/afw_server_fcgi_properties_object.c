// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_object interface for FCGI properties
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_server_fcgi_properties_object.c
 * @brief Implementation of afw_object interface for FCGI request properties.
 *
 * Lazy object over FCGX request envp. Values are Adaptive string when valid
 * UTF-8 (NFC), otherwise hexBinary. Non-UTF-8 names use "_NONUTF8_" + hex.
 * First access caches into an internal memory object; retrieve prefers cache
 * and does not emit duplicates from envp.
 */

#include "afw.h"
#include "afw_server_fcgi_internal.h"
#include "afw_object_impl.h"


/* Declares and rti/inf defines for interface afw_object */
#define AFW_IMPLEMENTATION_ID "fcgi_request_properties"
#include "afw_object_impl_declares.h"

typedef struct impl_self_s {
    afw_object_t pub;
    afw_value_object_t value;
    afw_server_fcgi_internal_request_t *request;
    const afw_object_t *properties;
    afw_boolean_t all_loaded;
} impl_self_t;


/* Cache one name=value envp entry if name not already present. */
static void
impl_cache_envp_entry(
    impl_self_t *self,
    const char *entry,
    afw_xctx_t *xctx)
{
    const afw_utf8_octet_t *s;
    const afw_utf8_octet_t *c;
    const afw_utf8_t *property_name;
    const afw_value_t *value;
    afw_size_t name_len;
    const afw_utf8_octet_t *value_octets;
    afw_size_t value_len;

    if (!entry) {
        return;
    }

    for (s = c = (const afw_utf8_octet_t *)entry; *c && *c != '='; c++);
    name_len = (afw_size_t)(c - s);
    property_name = afw_utf8_create_property_name_from_external_octets(
        s, name_len, self->pub.p, xctx);

    if (afw_object_has_property(self->properties, property_name, xctx)) {
        return;
    }

    if (!*c) {
        value = afw_v_a_empty_string;
    }
    else {
        value_octets = c + 1;
        value_len = strlen(value_octets);
        value = afw_value_create_from_external_octets(
            value_octets, value_len, self->pub.p, xctx);
    }

    afw_object_set_property(self->properties, property_name, value, xctx);
}


/* Load all FCGI envp entries into the cache (non-duplicates, cache preferred). */
static void
impl_load_all(impl_self_t *self, afw_xctx_t *xctx)
{
    char **envp;

    envp = self->request->fcgx_request->envp;
    for (; envp && *envp != 0; envp++) {
        impl_cache_envp_entry(self, *envp, xctx);
    }

    self->all_loaded = true;
}


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
    /* Request xctx pool; unmanaged value face. */
    self->value.inf = &afw_value_unmanaged_object_inf;
    self->value.internal = (const afw_object_t *)self;
    self->pub.value = (const afw_value_t *)&self->value;
    self->request = request;

    /* Empty memory object used as first-access cache. */
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
    impl_self_t *self = (impl_self_t *)instance;

    if (!self->all_loaded) {
        impl_load_all(self, xctx);
    }

    return afw_object_get_count(self->properties, xctx);
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

    /* Look for property in cache first. */
    value = afw_object_get_property(self->properties, property_name,
        xctx);
    if (value) {
        return value;
    }

    /* Lazy FCGX_GetParam for UTF-8 property names; cache on first access. */
    value = NULL;
    property_name_z = afw_utf8_z_create(property_name->s, property_name->len,
        xctx->p, xctx);
    s = FCGX_GetParam(property_name_z,
        self->request->fcgx_request->envp);
    if (s) {
        value = afw_value_create_from_external_z(
            (const char *)s, self->pub.p, xctx);
        afw_object_set_property(self->properties, property_name, value, xctx);
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

    /* Materialize full envp into cache once; cache is source of truth. */
    if (!self->all_loaded) {
        impl_load_all(self, xctx);
    }

    return afw_object_get_next_property(self->properties,
        iterator, property_name, xctx);
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
    impl_self_t * self = (impl_self_t *)instance;

    return afw_object_get_setter(self->properties, xctx);
}
