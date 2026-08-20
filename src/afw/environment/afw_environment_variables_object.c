// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_object interface for environment variables.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_environment_variables_object.c
 * @brief Implementation of afw_object interface for process environment variables.
 *
 * Lazy object over process environ/getenv. Values are Adaptive string when the
 * external bytes are valid UTF-8 (NFC), otherwise hexBinary. Property names that
 * are not valid UTF-8 are exposed with a forced_safe encode (^hex^). First
 * access caches into an internal memory object; retrieve prefers
 * cached/set properties and does not emit duplicates.
 */

#include "afw.h"
#include "afw_object_impl.h"


/* Declares and rti/inf defines for interface afw_object */
#define AFW_IMPLEMENTATION_ID "environment_variables"
typedef struct impl_self_s impl_self_t;
#define AFW_OBJECT_SELF_T impl_self_t
#include "afw_object_impl_declares.h"

typedef struct impl_self_s {
    afw_object_t pub;
    afw_value_object_t value;
    const afw_object_t *properties;
    afw_boolean_t all_variables_loaded;
} impl_self_t;


/* Get process environ pointer (Windows-aware). */
static char **
impl_get_environ(void)
{
#if defined(WIN) && (_MSC_VER >= 1900)
    return *__p__environ();
#else
    extern char **environ;
    return environ;
#endif
}


/*
 * Parse one environ entry "name=value", create property name and value, and
 * set on the cache if the name is not already present (cache preferred).
 */
static void
impl_cache_environ_entry(
    impl_self_t *self,
    const char *entry,
    afw_xctx_t *xctx)
{
    const afw_utf8_octet_t *s;
    const afw_utf8_octet_t *c;
    const afw_utf8_t *property_name_utf8;
    const afw_value_t *property_name;
    const afw_value_t *value;
    afw_size_t name_len;
    const afw_utf8_octet_t *value_octets;
    afw_size_t value_len;

    if (!entry) {
        return;
    }

    for (s = c = (const afw_utf8_octet_t *)entry; *c && *c != '='; c++);
    name_len = (afw_size_t)(c - s);
    property_name_utf8 = afw_utf8_create_property_name(
        s, name_len, self->pub.p, xctx);
    /* FIXME #2: utf8 name wrap; create the string value once. */
    property_name = afw_value_create_unmanaged_string(
        property_name_utf8, self->pub.p, xctx);

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


/* Load all process environment variables into the cache (non-duplicates). */
static void
impl_load_all_variables(impl_self_t *self, afw_xctx_t *xctx)
{
    char **envp;

    envp = impl_get_environ();
    for (; envp && *envp != 0; envp++) {
        impl_cache_environ_entry(self, *envp, xctx);
    }

    self->all_variables_loaded = true;
}



AFW_DEFINE(const afw_object_t *)
afw_environment_create_environment_variables_object(
    afw_boolean_t preload_variables,
    afw_xctx_t *xctx)
{
    impl_self_t *self;
    const afw_pool_t *p;

    static const afw_utf8_t impl_path =
        AFW_UTF8_LITERAL("/afw/_AdaptiveEnvironmentVariables_/current");

    /* Allocate memory for self and initialize. */
    p = afw_pool_create(xctx->p, xctx);
    self = afw_pool_calloc_type(p, impl_self_t, xctx);
    self->pub.inf = &impl_afw_object_inf;
    self->pub.p = p;
    self->value.inf = &afw_value_managed_object_inf;
    self->value.internal = (const afw_object_t *)self;
    self->pub.value = (const afw_value_t *)&self->value;
    self->pub.meta.id = afw_s_current;
    self->pub.meta.object_type_uri = afw_s__AdaptiveEnvironmentVariables_;
    self->pub.meta.object_uri = &impl_path;

    /* Empty memory object used as first-access cache. */
    self->properties = afw_object_create_cede_p(p, xctx);

    if (preload_variables) {
        impl_load_all_variables(self, xctx);
    }

    return (const afw_object_t *)self;
}



/*
 * Implementation of method release of interface afw_object.
 */
void
impl_afw_object_release(
    AFW_OBJECT_SELF_T *self,
    afw_xctx_t *xctx)
{

    afw_object_release(self->properties, xctx);
}



/*
 * Implementation of method get_reference of interface afw_object.
 */
void
impl_afw_object_get_reference (
    AFW_OBJECT_SELF_T *self,
    afw_xctx_t *xctx)
{

    afw_object_get_reference(self->properties, xctx);
}

/*
 * Implementation of method get_count for interface afw_object.
 */
afw_size_t
impl_afw_object_get_count(
    AFW_OBJECT_SELF_T *self,
    afw_xctx_t * xctx)
{

    /* If all variable not loaded yet, load them. */
    if (!self->all_variables_loaded) {
        impl_load_all_variables(self, xctx);
    }

    return afw_object_get_count(self->properties, xctx);
}

/*
 * Implementation of method get_meta for interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_meta(
    AFW_OBJECT_SELF_T *self,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return afw_object_impl_get_meta(
        &self->pub, p, xctx);
}


/*
 * Implementation of method get_property of interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_property(
    AFW_OBJECT_SELF_T *self,
    const afw_value_t * property_name,
    afw_xctx_t *xctx)
{
    const char *s;
    const afw_value_t *value;
    const afw_utf8_t *property_name_utf8;
    const afw_utf8_z_t *property_name_z;

    /* Look for property in cache first. */
    value = afw_object_get_property(self->properties, property_name,
        xctx);
    if (value) {
        return value;
    }

    /*
     * Lazy getenv for UTF-8 property names. Encoded names (^hex^) are only
     * populated via load_all / iterate (raw name is not a C string key).
     */
    value = NULL;
    /* FIXME #2: utf8 name wrap; getenv still wants a C string. */
    property_name_utf8 = afw_object_string_property_name_as_utf8(
        property_name, xctx);
    property_name_z = afw_utf8_z_create(
        property_name_utf8->s, property_name_utf8->len,
        xctx->p, xctx);
    s = getenv(property_name_z);
    if (s) {
        value = afw_value_create_from_external_z(s, self->pub.p, xctx);
        afw_object_set_property(self->properties, property_name, value, xctx);
    }

    return value;
}



/*
 * Implementation of method get_property_meta for interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_property_meta(
    AFW_OBJECT_SELF_T *self,
    const afw_value_t *property_name,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return afw_object_impl_get_property_meta(
        &self->pub, property_name, p, xctx);
}



/*
 * Implementation of method get_next_property of interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_next_property(
    AFW_OBJECT_SELF_T *self,
    const afw_iterator_old_t * * iterator,
    const afw_value_t * * property_name,
    afw_xctx_t *xctx)
{

    /* Materialize full environ into cache once; cache is source of truth. */
    if (!self->all_variables_loaded) {
        impl_load_all_variables(self, xctx);
    }

    return afw_object_get_next_property(self->properties,
        iterator, property_name, xctx);
}



/*
 * Implementation of method get_next_property_meta for interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_next_property_meta(
    AFW_OBJECT_SELF_T *self,
    const afw_iterator_old_t **iterator,
    const afw_value_t **property_name,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return afw_object_impl_get_next_property_meta(
        &self->pub, iterator, property_name, p, xctx);
}



/*
 * Implementation of method has_property of interface afw_object.
 */
afw_boolean_t
impl_afw_object_has_property(
    AFW_OBJECT_SELF_T *self,
    const afw_value_t * property_name,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;

    value = impl_afw_object_get_property(self, property_name, xctx);

    return (value != NULL);
}



/*
 * Implementation of method get_setter of interface afw_object.
 */
const afw_object_setter_t *
impl_afw_object_get_setter (
    AFW_OBJECT_SELF_T *self,
    afw_xctx_t *xctx)
{
    /* Is readonly. */
    return NULL;
}
