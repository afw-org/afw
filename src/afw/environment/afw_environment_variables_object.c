// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_object interface for environment variables.
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_environment_variables_object.c
 * @brief Implementation of afw_object interface for environment variables.
 */

#include "afw.h"
#include "afw_object_impl.h"


/* Declares and rti/inf defines for interface afw_object */
#define AFW_IMPLEMENTATION_ID "fcgi"
#include "afw_object_impl_declares.h"

typedef struct impl_self_s {
    afw_object_t pub;
    const afw_object_t *properties;
    afw_boolean_t all_variables_loaded;
} impl_self_t;


/* Called first time impl_afw_object_get_next_property() is called. */
static void
impl_load_all_variables(impl_self_t *self, afw_xctx_t *xctx)
{
    char ** envp;
    const afw_utf8_octet_t *s;
    const afw_utf8_octet_t *c;
    afw_utf8_t property_name;
    const afw_value_t *value;

    /* Get point to environment variables special way for Windows. */
#if defined(WIN) && (_MSC_VER >= 1900)
    envp = *__p__environ();
#else
    extern char ** environ;
    envp = environ;
#endif

    /* Loop adding missing variables. */
    for (; *envp != 0; envp++) {
        property_name.s = *envp;
        for (s = c = *envp; *c && *c != '='; c++);
        property_name.len = c - s;

        if (!afw_object_has_property(self->properties, &property_name, xctx)) {
            if (!*c) {
                value = afw_value_empty_string;
            }
            else {
                value = afw_value_create_string_from_u8z(c + 1,
                    self->pub.p, xctx);
            }
            afw_object_set_property(self->properties,
                afw_utf8_clone(&property_name, self->pub.p, xctx),
                value, xctx);
        }
    }

    /* Indicate loaded. */
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
    self->pub.meta.id = &afw_s_current;
    self->pub.meta.object_type_uri = &afw_s__AdaptiveEnvironmentVariables_;
    self->pub.meta.object_uri = &impl_path;

    /* Create request properties object. */
    self->properties = afw_object_create_and_cede_p(p, xctx);

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
    const afw_object_t * instance,
    afw_xctx_t *xctx)
{
    impl_self_t *self = (impl_self_t *)instance;

    afw_object_release(self->properties, xctx);
}



/*
 * Implementation of method add_reference of interface afw_object.
 */
void
impl_afw_object_add_reference (
    const afw_object_t * instance,
    afw_xctx_t *xctx)
{
    impl_self_t *self = (impl_self_t *)instance;

    afw_object_add_reference(self->properties, xctx);
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
    if (value) {
        return value;
    }

    /* If not in memory, try FCGX_GetParam(). */
    value = NULL;
    property_name_z = afw_utf8_z_create(property_name->s, property_name->len,
        xctx->p, xctx);
    s = getenv(property_name_z);
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
 
    /* If all variable not loaded yet, load them. */
    if (!self->all_variables_loaded) {
        impl_load_all_variables(self, xctx);
    }

    /* Call method of properties object. */
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
    /* Is readonly. */
    return NULL;
}
