// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Object View
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_object_view.c
 * @brief General adaptive object view.
 */


/* __AFW_OBJECT_VIEW_DEBUG__ causes debug log messages to be produced. */
// #define __AFW_OBJECT_VIEW_DEBUG__

#include "afw_internal.h"



#define impl_afw_object_get_meta \
    afw_object_impl_internal_get_meta

#define impl_afw_object_get_property_meta \
    afw_object_impl_internal_get_property_meta

#define impl_afw_object_get_next_property_meta \
    afw_object_impl_internal_get_next_property_meta

/* Object view object implementation. */
#define AFW_IMPLEMENTATION_ID "afw_object_view"
#include "afw_object_impl_declares.h"

/*--- Macros. ---*/


/* Set meta.properties property. */
#define impl_meta_property_type_get(self, property_name, xctx) \
afw_object_meta_get_property_type( \
    ((const afw_object_t *)self), \
    property_name, xctx)

/*--- Static declares. ---*/

static const afw_value_t *
impl_shared_string_value(
    afw_object_view_internal_view_t *view,
    const afw_utf8_t *string,
    afw_xctx_t *xctx);


static const afw_value_t *
impl_shared_path_value(
    afw_object_view_internal_object_self_t *self,
    const afw_utf8_t *path,
    afw_xctx_t *xctx);


static void
impl_set_property(
    afw_object_view_internal_object_self_t *self,
    const afw_object_t *origin,
    const afw_utf8_t *original_name,
    const afw_value_t *original_value,
    afw_xctx_t *xctx);


static afw_object_view_property_t *
impl_get_property(
    afw_object_view_internal_object_self_t *self,
    const afw_utf8_t *property_name);


/* Add requested meta properties. */
static void
impl_additional_object_option_processing(
    afw_object_view_internal_object_self_t *self,
    afw_xctx_t *xctx);


static void
impl_add_origin_properties(
    afw_object_view_internal_object_self_t *embedding_object,
    afw_xctx_t *xctx);


static void
impl_merge_properties(
    afw_object_view_internal_object_self_t *self,
    afw_object_view_internal_object_self_t *parent,
    afw_xctx_t *xctx);


static void
impl_resolve_parents(
    afw_object_view_internal_object_self_t *self,
    afw_xctx_t *xctx);


static void
impl_add_inherited_properties(
    afw_object_view_internal_object_self_t *self,
    afw_xctx_t *xctx);


static afw_object_view_internal_object_self_t *
impl_get_object_by_uri(
    const afw_utf8_t *uri,
    afw_object_view_internal_object_self_t *current,
    afw_xctx_t *xctx);


static afw_object_view_internal_object_self_t *
impl_object_create_entity(
    afw_object_view_internal_view_t *view,
    const afw_object_t *origin,
    const afw_uri_parsed_t *uri_parsed,
    afw_xctx_t *xctx);


static afw_object_view_internal_object_self_t *
impl_object_create(
    afw_object_view_internal_view_t *view,
    afw_object_view_internal_object_self_t *embedding_object,
    const afw_utf8_t *property_name,
    const afw_object_t *origin,
    const afw_uri_parsed_t *uri_parsed,
    afw_xctx_t *xctx);


/*--- impl function defines. ---*/


static void
impl_meta_set_property(
    afw_object_view_internal_object_self_t *self,
    const afw_utf8_t *property_name,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    afw_object_meta_set_property(
        ((const afw_object_t *)self),
        property_name, value, xctx);
}

static void
impl_meta_set_property_type_property(
    afw_object_view_internal_object_self_t *self,
    const afw_utf8_t *property_type_id,
    const afw_utf8_t *property_name,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    afw_object_meta_set_property_type_property(
        ((const afw_object_t *)self),
        property_type_id, property_name, value, xctx);
}


static const afw_value_t *
impl_shared_string_value(
    afw_object_view_internal_view_t *view,
    const afw_utf8_t *string,
    afw_xctx_t *xctx)
{
    afw_value_string_t *value;

    if (!view->string_values) {
        view->string_values = apr_hash_make(afw_pool_get_apr_pool(view->p));
    }

    value = apr_hash_get(view->string_values, string->s, string->len);
    if (!value) {
        value = afw_value_allocate_string(view->p, xctx);
        value->internal.s = string->s;
        value->internal.len = string->len;
        apr_hash_set(view->string_values,
            value->internal.s, value->internal.len, value);
    }

    return (const afw_value_t *)value;
}


static const afw_value_t *
impl_shared_path_value(
    afw_object_view_internal_object_self_t *self,
    const afw_utf8_t *path,
    afw_xctx_t *xctx)
{
    afw_object_view_internal_view_t *view = self->view;
    const afw_pool_t *p = view->p;
    afw_value_anyURI_t *value;
    const afw_utf8_octet_t *c;
    afw_size_t len;
    const afw_uri_parsed_t *parsed;
    const afw_utf8_t *current_path;

    if (!view->path_values) {
        view->path_values = apr_hash_make(afw_pool_get_apr_pool(p));
    }

    value = apr_hash_get(view->path_values, path->s, path->len);
    if (!value) {
        for (c = path->s, len = path->len; len > 0; c++, len--) {
            if (*c == '*') {
                current_path = afw_object_meta_get_path(
                    (const afw_object_t *)self, xctx);
                parsed = afw_uri_parse(path, true, current_path,
                    p, xctx);
                value = apr_hash_get(view->path_values,
                    parsed->normalized_uri.s, parsed->normalized_uri.len);
                if (!value) {
                    value = afw_value_allocate_anyURI(p, xctx);
                    value->internal.s = parsed->normalized_uri.s;
                    value->internal.len = parsed->normalized_uri.len;
                    apr_hash_set(view->path_values,
                        value->internal.s, value->internal.len, value);
                }
                apr_hash_set(view->path_values,
                        path->s, path->len, value);
                break;
            }
        }
    }

    if (!value) {
        value = afw_value_allocate_anyURI(p, xctx);
        value->internal.s = path->s;
        value->internal.len = path->len;
        apr_hash_set(view->path_values,
            path->s, path->len, value);
    }

    return (const afw_value_t *)value;
}


static const afw_value_t *
impl_make_value(
    afw_object_view_internal_object_self_t *self,
    const afw_object_t *origin,
    const afw_utf8_t *property_name,
    const afw_value_t *original_value,
    afw_xctx_t *xctx)
{
    const afw_value_t *result;
    const afw_value_t *original_entry_value;
    const afw_value_t *value;
    afw_object_view_internal_view_t *view;
    const afw_pool_t *p;
    const afw_array_t *list;
    const afw_data_type_t *data_type;
    const afw_iterator_t *iterator;
    afw_object_view_internal_object_self_t *embedded_object;

    view = self->view;
    p = view->p;

    /* Default result will be original value. */
    result = original_value;

    /* If single object, create copy. */
    if (afw_value_is_object(original_value)) {
        embedded_object = impl_object_create(
            view, self, property_name,
            ((const afw_value_object_t *)original_value)->internal,
            NULL, xctx);
        result = afw_value_create_object(
            (const afw_object_t *)embedded_object, p, xctx);
    }

    /* If single list, make new list with entries using impl_make_value(). */
    else if (afw_value_is_array(original_value)) {
        data_type = afw_array_get_data_type(
            ((const afw_value_array_t *)original_value)->internal,
            xctx);
        list = afw_array_of_create(data_type, p, xctx);
        for (iterator = NULL;;) {
            original_entry_value = afw_array_get_next_value(
                ((afw_value_array_t *)original_value)->internal,
                &iterator, p, xctx);
            if (!original_entry_value) {
                break;
            }
            value = impl_make_value(self, origin, property_name,
                original_entry_value, xctx);
            afw_array_add_value(list, value, xctx);
        }
        result = afw_value_create_array(list, p, xctx);
    }

    /* Return result. */
    return result;
}


static afw_object_view_property_t *
impl_get_property(
    afw_object_view_internal_object_self_t *self,
    const afw_utf8_t *property_name)
{
    afw_object_view_property_t *prop;

    for (prop = self->first_property;
        prop && !afw_utf8_equal(prop->name, property_name);
        prop = prop->next_property);

    return prop;
}



static void
impl_set_property(
    afw_object_view_internal_object_self_t *self,
    const afw_object_t *origin,
    const afw_utf8_t *property_name,
    const afw_value_t *original_value,
    afw_xctx_t *xctx)
{
    afw_object_view_internal_view_t *view;
    const afw_pool_t *p;
    afw_object_view_property_t *prop;

    view = self->view;
    p = view->p;

    prop = afw_pool_calloc_type(p, afw_object_view_property_t, xctx);
    prop->origin = origin;
    prop->name = property_name;
    prop->value = impl_make_value(self, origin,
        prop->name, original_value, xctx);
    prop->next_property = self->first_property;
    self->first_property = prop;
}



/* Update property value. */
static void
impl_update_property_value(
    afw_object_view_internal_object_self_t *self,
    const afw_utf8_t *property_name,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    afw_object_view_property_t *prop;

    prop = impl_get_property(self, property_name);
    if (!prop) {
        AFW_THROW_ERROR_Z(general,
            "Internal error - impl_update_property_value()",
            xctx);
    }

    prop->value = value;
}



static const afw_value_t *
impl_normalize_value(
    afw_object_view_internal_object_self_t *self,
    const afw_object_type_property_type_t *pt,
    const afw_utf8_t *property_name,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    afw_object_view_internal_view_t *view = self->view;
    const afw_pool_t *p = view->p;
    const afw_value_t *result = NULL;

    AFW_TRY {
        result = afw_object_type_property_type_normalize(
            pt, value, p, xctx);
    }

    AFW_CATCH_UNHANDLED{
        afw_object_meta_add_thrown_property_error(
            (const afw_object_t *)self,
            property_name, -1, AFW_ERROR_THROWN, xctx);
    }

    AFW_ENDTRY;

    return result;
}



/* Add objectTypeMetaDelta/objectTypeMetaFull */
static void
impl_object_type_related_object_option_processing(
    afw_object_view_internal_object_self_t *self,
    afw_xctx_t *xctx)
{
    afw_object_view_internal_view_t *view = self->view;
    const afw_object_t *instance = (const afw_object_t *)self;
    const afw_object_options_t *options = view->options;

    const afw_utf8_t *object_type_id;
    const afw_object_type_t *object_type;
    const afw_iterator_t *iterator;
    const afw_value_t *value;
    const afw_utf8_t *property_name;
    const afw_utf8_t *s;
    const afw_value_t *s_value;
    const afw_value_t *new_value;
    const afw_object_t *object;
    const afw_object_type_property_type_t *pt;
    afw_boolean_t check_required;
    afw_boolean_t include_defaults;
    const afw_data_type_t *value_data_type;

    /* Convenience */
    check_required = AFW_OBJECT_OPTION_IS(options, checkRequired);
    include_defaults = AFW_OBJECT_OPTION_IS(options, includeDefaultValues);

    /* Get object type id. Default is _AdaptiveObject_. */
    object_type_id = afw_object_meta_get_object_type_id(instance, xctx);
    if (!object_type_id) {
        object_type_id = &afw_self_s__AdaptiveObject_;
    }

    /* Get object type object. */
    object_type = afw_adaptor_get_object_type(view->adaptor_id,
        object_type_id, view->journal_entry, xctx);
    if (!object_type) {
        afw_object_meta_add_error_fz(instance, xctx,
            "objectType " AFW_UTF8_FMT_Q
            " needed for normalize object option is not found",
            AFW_UTF8_FMT_ARG(object_type_id));
        return;
    }
    if (AFW_OBJECT_OPTION_IS(options, metaFull)) {
        afw_object_meta_set_object_type(instance, object_type, xctx);
    }

    /* If requested, check required and include default values. */
    if (check_required || include_defaults) {
        for (iterator = NULL;;) {
            pt = afw_object_type_property_type_get_next(
                object_type, &iterator, &property_name, xctx);
            if (!pt) break;

            value = afw_object_get_property(instance, property_name, xctx);
            if (value) {
                value_data_type = afw_value_get_data_type(value, xctx);

                /** @fixme Data type inf should have a function for this. */
                if (afw_utf8_equal(&value_data_type->cType,
                    &afw_self_s_afw_utf8_t))
                {
                    if (((afw_value_string_t *)value)
                        ->internal.len == 0)
                    {
                        value = NULL;
                    }
                }
                else if (afw_utf8_equal(&value_data_type->cType,
                    &afw_self_s_afw_memory_t))
                {
                    if (((afw_value_base64Binary_t *)value)
                        ->internal.size == 0)
                    {
                        value = NULL;
                    }
                }
            }
            if (!value)
            {
                if (include_defaults) {
                    value = pt->default_value;
                    if (value) {
                        impl_set_property(self, pt->property_type_object,
                            property_name, value, xctx);
                    }
                }
                if (!value && check_required && pt->required) {
                    afw_object_meta_add_property_error_z(instance,
                        property_name, "value required", xctx);
                }
            }
        }
    }

    /** @fixme Need to deal with relatedPropertyTypePurpose. */

    /* Normalize values. */
    for (iterator = NULL;;) {
        value = afw_object_get_next_property(instance,
            &iterator, &property_name, xctx);
        if (!value) {
            break;
        }

        /* Get property type. */
        pt = afw_object_type_property_type_get(object_type,
            property_name, xctx);
        if (!pt) {
            afw_object_meta_add_property_error_z(instance, property_name,
                "no property type", xctx);
            continue;
        }

        /* If property value needed normalization, update it in instance. */
        new_value = impl_normalize_value(self, pt,
            property_name, value, xctx);
        if (new_value != value) {
            impl_update_property_value(self, property_name, new_value, xctx);
        }
    }


    /*
     * If objectTypes is true, add it to the objectTypes object of the
     * entity's meta.
     */
    if (AFW_OBJECT_OPTION_IS(options, objectTypes))
    {
        afw_object_meta_add_needed_object_type(instance, 
            object_type->object_type_object,
            xctx);
    }

    /*
     * Fix up dataType, and if object, objectType in propertyTypes based on
     * actual property values in self.
     */
    for (iterator = NULL;;) {
        value = afw_object_get_next_property(instance,
            &iterator, &property_name, xctx);
        if (!value) {
            break;
        }

        AFW_TRY {

            value_data_type = afw_value_get_data_type(value, xctx);

            /*  Get property type from object type object.  May be NULL. */
            pt = afw_object_type_property_type_get(object_type,
                property_name, xctx);

            /* Set dataType if different. */
            if (value_data_type && (!pt || value_data_type != pt->data_type))
            {
                s_value = impl_shared_string_value(view,
                    &value_data_type->data_type_id, xctx);
                afw_object_meta_set_property_type_property(instance,
                    property_name, &afw_self_s_dataType, s_value, xctx);
            }

            /* If object, set dataTypeParameter if different */
            if (afw_value_is_object(value)) {
                object =
                    ((const afw_value_object_t *)value)->internal;
                object_type_id = afw_object_meta_get_object_type_id(object,
                    xctx);
                if (object_type_id)
                {
                    s = NULL;
                    if (pt) {
                        s = afw_object_old_get_property_as_string(
                            pt->property_type_object,
                            &afw_self_s_dataTypeParameter,
                            xctx);
                    }
                    if (s && !afw_utf8_equal(s, object_type_id)) {
                        s_value = impl_shared_string_value(view,
                            object_type_id, xctx);
                        afw_object_meta_set_property_type_property(instance,
                            property_name, &afw_self_s_dataTypeParameter, s_value, xctx);
                    }
                }
            }
        }

        AFW_CATCH_UNHANDLED{
            afw_object_meta_add_thrown_property_error(instance,
                property_name, -1, AFW_ERROR_THROWN, xctx);
        }

        AFW_ENDTRY;

    }
}


/* Add requested meta properties. */
static void
impl_additional_object_option_processing(
    afw_object_view_internal_object_self_t *self,
    afw_xctx_t *xctx)
{
    afw_object_view_internal_view_t *view = self->view;
    const afw_object_options_t *options = view->options;
    const afw_pool_t *p = view->p;

    const afw_value_t *v;
    const afw_utf8_t *path;
    afw_object_view_property_t *prop;
    const afw_value_array_t *parent_paths;
    afw_value_array_t *resolved_parent_paths;
    const afw_iterator_t *iterator;

    /*NOTE
     *
     * objectId, objectType, path, and pathEmbedded are honored even though
     * content types will provide them if they are passed options.  There were
     * situations where content types do not have the options available, or in
     * the case of perform, where there may be multiple actions with different
     * options.
     */

    /* normalize related options */
    if (AFW_OBJECT_OPTION_IS(options, normalize)) {
        impl_object_type_related_object_option_processing(self, xctx);
    }

    /* objectId  */
    if (AFW_OBJECT_OPTION_IS(options, objectId) &&
        !self->pub.meta.embedding_object)
    {
        if (self->pub.meta.id) {
            v = impl_shared_string_value(self->view, self->pub.meta.id, xctx);
            impl_meta_set_property(self, &AFW_OBJECT_S_PN_OBJECT_ID, v,
                xctx);
        }
    }

    /* objectType */
    if (AFW_OBJECT_OPTION_IS(options, objectType))
    {
        if (afw_object_meta_get_object_type_id(&self->pub, xctx)) {
            v = impl_shared_string_value(self->view,
                afw_object_meta_get_object_type_id(&self->pub, xctx),
                xctx);
            impl_meta_set_property(self, &afw_self_s_objectType, v, xctx);
        }
    }

    /* path and pathEmbedded */
    if ((AFW_OBJECT_OPTION_IS(options, path) &&
            !self->pub.meta.embedding_object) ||
        (AFW_OBJECT_OPTION_IS(options, pathEmbedded) &&
            self->pub.meta.embedding_object))
    {
        if (!self->pub.meta.object_uri) {
            self->pub.meta.object_uri = afw_object_meta_get_path(
                (const afw_object_t *)self, xctx);
        }
        if (self->pub.meta.object_uri) {
            v = impl_shared_path_value(self, self->pub.meta.object_uri, xctx);
            impl_meta_set_property(self, &afw_self_s_path, v, xctx);
        }
    }

    /*
     * If parentPaths, resolve.  If composite option, change name of
     * parentPaths to resolvedParentPaths.
     */
    parent_paths = afw_object_meta_get_parent_paths_value(
        (const afw_object_t *)self, xctx);
    if (parent_paths) {

        resolved_parent_paths = afw_value_allocate_array(p, xctx);
        resolved_parent_paths->internal = afw_array_of_create(
            afw_data_type_anyURI, p, xctx);
        for (iterator = NULL;;) {
            path = afw_array_of_utf8_get_next(parent_paths->internal,
                &iterator, xctx);
            if (!path) break;

            v = impl_shared_path_value(self, path, xctx);
            afw_array_add_value(resolved_parent_paths->internal, v, xctx);
        }

        if (AFW_OBJECT_OPTION_IS(options, composite)) {
            if (AFW_OBJECT_OPTION_IS(options, resolvedParentPaths)) {
                impl_meta_set_property(self, &afw_self_s_resolvedParentPaths,
                    (const afw_value_t *)resolved_parent_paths, xctx);
            }
            afw_object_meta_set_property((const afw_object_t *)self,
                &afw_self_s_parentPaths, NULL, xctx);
        }

        else {
            impl_meta_set_property(self, &afw_self_s_parentPaths,
                (const afw_value_t *)resolved_parent_paths, xctx);
        }
    }

    /* inheritedFrom */
    if (AFW_OBJECT_OPTION_IS(options, inheritedFrom))
    {
        for (prop = self->first_property; prop; prop = prop->next_property)
        {
            if (prop->origin != self->origin) {
                path = afw_object_meta_get_path(prop->origin, xctx);
                if (!path) {
                    AFW_THROW_ERROR_FZ(general, xctx,
                        "Property " AFW_UTF8_FMT_Q
                            " is missing path for inheritedFrom", 
                        AFW_UTF8_FMT_ARG(prop->name));
                }
                v = impl_shared_path_value(self, path, xctx);
                impl_meta_set_property_type_property(self,
                    prop->name, &afw_self_s_inheritedFrom,
                    v, xctx);
            }
        }
    }

    /* Add requested meta for any embedded object properties. */
    for (prop = self->first_property; prop; prop = prop->next_property)
    {
        if (afw_value_is_object(prop->value)) {
            impl_additional_object_option_processing(
                (afw_object_view_internal_object_self_t *)
                ((const afw_value_object_t *)prop->value)->internal,
                xctx);
        }
    }
}


static void
impl_resolve_parents(
    afw_object_view_internal_object_self_t *self,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p;
    afw_object_view_internal_view_t *view;
    const afw_object_t *origin;
    afw_object_view_internal_object_self_t * *parent;
    const afw_utf8_t *path;
    const afw_iterator_t *iterator;
    const afw_value_array_t *parent_paths;
    afw_size_t count;

    view = self->view;
    p = view->p;
    origin = self->origin;

    parent_paths = afw_object_meta_get_parent_paths_value(origin, xctx);
    if (!parent_paths) {
        return;
    }

    /* Allocate memory for parents. */
    count = afw_array_get_count(parent_paths->internal, xctx);
    self->parents = parent = afw_pool_malloc(p,
        (count + 1) *
        sizeof(afw_object_view_internal_object_self_t *),
        xctx);

    AFW_TRY {
        for (iterator = NULL;;) {
            path = afw_array_of_utf8_get_next(parent_paths->internal,
                &iterator, xctx);
            if (!path) break;

            *parent++ = impl_get_object_by_uri(path, self, xctx);
        }
        *parent = NULL;
    }

    AFW_CATCH_UNHANDLED{
        *parent = NULL;
        afw_object_meta_add_error_fz((const afw_object_t *)self,
            xctx, "Error resolving parentPaths - %s",
            AFW_ERROR_THROWN->message_z);
    }

    AFW_ENDTRY;
}


static void
impl_merge_properties(
    afw_object_view_internal_object_self_t *self,
    AFW_COMPILER_ANNOTATION_NONNULL
        afw_object_view_internal_object_self_t *parent,
    afw_xctx_t *xctx)
{
    afw_object_view_property_t *parent_prop;
    afw_object_view_property_t *self_prop;
    afw_object_view_internal_object_self_t *ancestor;
    afw_object_view_internal_object_self_t *obj;
    afw_size_t count;
    afw_size_t count2;

    /*
     * Determine then number of ancestors that need to have inherited
     * properties added.
     */
    for (count = 0, ancestor = parent;

        ancestor &&
        !ancestor->inherited_properties_being_added &&
        !ancestor->inherited_properties_added;

        count++)
    {
        ancestor = (afw_object_view_internal_object_self_t *)
            ancestor->pub.meta.embedding_object;
    }

    /*
     * Add inherited properties to ancestors that need it from most distant
     * to closest.
     */
    for (ancestor = parent; count > 0; count--)
    {
        for (obj = ancestor, count2 = count; count2 > 1; count2--)
        {
            obj = (afw_object_view_internal_object_self_t *)
                obj->pub.meta.embedding_object;
        }
        impl_add_inherited_properties(obj, xctx);
    }

    /*
     * Merge properties from parent to self.
     *
     * If an object property is exists in both self and parent, the parent's
     * property is merged into self's property.
     *
     * Object properties that are lists are not merged.
     */
    for (parent_prop = parent->first_property;
        parent_prop;
        parent_prop = parent_prop->next_property)
    {
        self_prop = impl_get_property(self, parent_prop->name);
        if (!self_prop) {
            impl_set_property(self,
                parent_prop->origin, parent_prop->name, parent_prop->value, xctx);
        }
        else if (
            afw_value_is_object(self_prop->value) &&
            afw_value_is_object(parent_prop->value))
        {
            impl_merge_properties(
                (afw_object_view_internal_object_self_t *)
                    ((afw_value_object_t *)self_prop->value)->internal,
                (afw_object_view_internal_object_self_t *)
                    ((afw_value_object_t *)parent_prop->value)->internal,
                xctx);
        }
    }
}



static void
impl_add_inherited_properties(
    afw_object_view_internal_object_self_t *self,
    afw_xctx_t *xctx)
{
    afw_object_view_internal_object_self_t * *parent;
    const afw_utf8_t *path;
    afw_object_view_property_t *prop;


    /* If inherited properties already added, just return. */
    if (self->inherited_properties_added) {

#ifdef __AFW_OBJECT_VIEW_DEBUG__
        path = afw_object_meta_get_path((const afw_object_t *)self, xctx);
        AFW_LOG_FZ(debug, xctx,
        "Properties already added   - " AFW_UTF8_FMT_Q, AFW_UTF8_FMT_ARG(path));
#endif

        return;
    }

#ifdef __AFW_OBJECT_VIEW_DEBUG__
    path = afw_object_meta_get_path((const afw_object_t *)self, xctx);
    AFW_LOG_FZ(debug, xctx,
        "Starting to add properties - " AFW_UTF8_FMT_Q, AFW_UTF8_FMT_ARG(path));
#endif

    /* Check for recursion loop. */
    if (self->inherited_properties_being_added) {
        path = afw_object_meta_get_path((const afw_object_t *)self, xctx);
        AFW_THROW_ERROR_FZ(general, xctx,
            "parentPaths recursion loop while processing " AFW_UTF8_FMT_Q,
            AFW_UTF8_FMT_ARG(path));
    }
    self->inherited_properties_being_added = true;

    /* Process direct parents first. */
    impl_resolve_parents(self, xctx);
    if (self->parents) {
        for (parent = self->parents; *parent; parent++) {
            impl_merge_properties(self, *parent, xctx);
        }
    }    

    /* Add inherited to any object properties properties and children. */
    for (prop = self->first_property; prop; prop = prop->next_property)
    {
        if (afw_value_is_object(prop->value)) {
            impl_add_inherited_properties(
                (afw_object_view_internal_object_self_t *)
                ((const afw_value_object_t *)prop->value)->internal,
                xctx);
        }
    }

    /* Indicate that inherited properties have been added. */
    self->inherited_properties_being_added = false;
    self->inherited_properties_added = true;


#ifdef __AFW_OBJECT_VIEW_DEBUG__
    AFW_LOG_FZ(debug, xctx,
        "Finished adding properties - " AFW_UTF8_FMT_Q, AFW_UTF8_FMT_ARG(path));
#endif
}



static void
impl_add_origin_properties(
    afw_object_view_internal_object_self_t *self,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;
    const afw_utf8_t *name;
    const afw_iterator_t *iterator;

    /* Make array of properties. */
    for (iterator = NULL;;) {
        value = afw_object_get_next_property(self->origin, &iterator, &name, xctx);
        if (!value) {
            break;
        }

        if (!afw_utf8_equal(name, &afw_self_s__meta_)) {
            impl_set_property(self, self->origin, name, value, xctx);
        }
    }
}



static afw_object_view_internal_object_self_t *
impl_get_object_by_uri(
    const afw_utf8_t *uri,
    afw_object_view_internal_object_self_t *current,
    afw_xctx_t *xctx)
{
    const afw_uri_parsed_t *uri_parsed;
    const afw_uri_parsed_t *entity_uri_parsed;
    const afw_object_path_parsed_t *path_parsed;
    const afw_utf8_t *current_path;
    afw_object_view_internal_view_t *view;
    const afw_pool_t *p;
    afw_object_view_internal_object_self_t *entity;
    const afw_object_t *object;
    afw_object_view_internal_object_self_t *result;
    const afw_object_path_property_name_entry_t *name;

    view = current->view;
    p = view->p;

    /* Get current path. */
    current_path = afw_object_meta_get_path((afw_object_t *)current, xctx);

    /* Parse URI. */
    uri_parsed = afw_uri_parse(uri, true, current_path, p, xctx);
    path_parsed = uri_parsed->path_parsed;

    /* See if entity has already been loaded. */
    for (entity = view->main_entity; entity; entity = entity->next_entity) {
        if (afw_utf8_equal(&entity->uri_parsed->normalized_uri,
            &uri_parsed->normalized_uri))
        {
            break;
        }
        if (entity->uri_parsed->path_parsed &&
            uri_parsed->path_parsed &&
            afw_utf8_equal(
                &entity->uri_parsed->path_parsed->entity_path,
                &uri_parsed->path_parsed->entity_path))
        {
            break;
        }
    }

    /* If entity not already loaded, try to load it. */
    if (!entity) {

        /* If local path, call adaptor directly. */
        if (path_parsed) {
            object = afw_adaptor_get_object(
                &path_parsed->adaptor_id,
                &path_parsed->object_type_id,
                &path_parsed->entity_object_id,
                NULL,
                NULL,
                view->journal_entry,
                NULL, view->p, xctx);
            if (!object) goto error;
            entity_uri_parsed = uri_parsed;
            if (path_parsed && path_parsed->first_property_name) {
                entity_uri_parsed = afw_uri_parse(
                    &uri_parsed->path_parsed->entity_path, true, current_path, p, xctx);
            }
            entity = impl_object_create_entity(view, object, entity_uri_parsed, xctx);
        }

        /* If full URI, call resolver. */
        else {
            AFW_THROW_ERROR_Z(general,
                "Not implemented - only local path supported", xctx);
        }
    }

    /* If entity not loaded, not found error. */
    if (!entity) {
        goto error;
    }

    /* Result is entity or an embedded object of entity. */
    result = entity;
    if (path_parsed) {
        for (name = path_parsed->first_property_name; name; name = name->next) {
            result = (afw_object_view_internal_object_self_t *)
                afw_object_old_get_property_as_object(
                (const afw_object_t *)result, &name->property_name, xctx);
            if (!result) {
                goto error;
            }
        }
    }

    /* Return result. */
    return result;

error:
    AFW_THROW_ERROR_FZ(general, xctx, AFW_UTF8_FMT_Q
        " not found or invalid",
        AFW_UTF8_FMT_ARG(uri));
}


static afw_object_view_internal_object_self_t *
impl_object_create_entity(
    afw_object_view_internal_view_t *view,
    const afw_object_t *origin,
    const afw_uri_parsed_t *uri_parsed,
    afw_xctx_t *xctx)
{
    afw_object_view_internal_object_self_t *self;

#ifdef __AFW_OBJECT_VIEW_DEBUG__
    const afw_utf8_t *path;
#endif

    self = impl_object_create(view, NULL, NULL, origin, uri_parsed, xctx);
    if (AFW_OBJECT_OPTION_IS(view->options, composite)) {

#ifdef __AFW_OBJECT_VIEW_DEBUG__
        path = afw_object_meta_get_path((const afw_object_t *)self, xctx);
        AFW_LOG_FZ(debug, xctx,
            "Starting composite view    - " AFW_UTF8_FMT_Q,
            AFW_UTF8_FMT_ARG(path));
#endif

        impl_add_inherited_properties(self, xctx);

#ifdef __AFW_OBJECT_VIEW_DEBUG__
        path = afw_object_meta_get_path((const afw_object_t *)self, xctx);
        AFW_LOG_FZ(debug, xctx,
            "Finished composite view    - " AFW_UTF8_FMT_Q,
            AFW_UTF8_FMT_ARG(path));
#endif

    }

    return self;
}


/* uri_parsed required for entities. */
static afw_object_view_internal_object_self_t *
impl_object_create(
    afw_object_view_internal_view_t *view,
    afw_object_view_internal_object_self_t *embedding_object,
    const afw_utf8_t *property_name,
    const afw_object_t *origin,
    const afw_uri_parsed_t *uri_parsed,
    afw_xctx_t *xctx)
{
    afw_object_view_internal_object_self_t *self;
    const afw_pool_t *p;
    const afw_value_t *value;

    p = view->p;
    value = NULL;

    /* Allocate and initialize self. */
    self = afw_pool_calloc_type(p, afw_object_view_internal_object_self_t,
        xctx);
    self->pub.inf = &impl_afw_object_inf;
    self->pub.p = p;
    self->view = view;
    self->pub.meta.embedding_object = (const afw_object_t *)embedding_object;
    self->pub.meta.id = property_name;
    self->pub.meta.object_type_uri = origin->meta.object_type_uri;
    self->origin = origin;
    self->uri_parsed = uri_parsed;

    /*
     * If a uri_parsed is passed with a parsed uri path, use it's path,
     * object_type_id, and object_id.  If not, these will be determined
     * by the corresponding get_*() method.  This is so the they will not
     * be produced unnecessarily.
     */
    if (uri_parsed && uri_parsed->path_parsed) {
        self->pub.meta.object_type_uri =
            &uri_parsed->path_parsed->object_type_id;
        self->pub.meta.id = &uri_parsed->path_parsed->entity_object_id;
        if (uri_parsed->path_parsed->first_property_name) {
            AFW_THROW_ERROR_Z(general,
                "Dotted property names in object id not allowed here",
                xctx);
        }
        self->pub.meta.object_uri = &uri_parsed->path_parsed->normalized_path;

        value = afw_value_create_anyURI(self->pub.meta.object_uri,
            p, xctx);
    }

    /* If this is entity object, add to list of loaded entity objects. */
    if (!embedding_object) {
        if (!view->main_entity) {
            view->main_entity = self;
        } else {
            self->next_entity = view->main_entity->next_entity;
            view->main_entity->next_entity = self;
        }
    }

    /* Set meta using clone of origin's meta and set path if entity. */
    afw_object_meta_clone_and_set((const afw_object_t *)self, origin, xctx);
    if (uri_parsed && uri_parsed->path_parsed) {
        impl_meta_set_property(self, &afw_self_s_path, value, xctx);
    }

    /* Add self properties from origin. */
    impl_add_origin_properties(self, xctx);

    /* Return instance. */
    return self;
}

/*--- Object implementation for internal view objects. ---*/



/*
 * Implementation of method release of interface afw_object.
 */
void
impl_afw_object_release (
    const afw_object_t * instance,
    afw_xctx_t *xctx)
{
    afw_object_view_internal_object_self_t * self;
    const afw_object_t *entity;

    /* Methods release and get_reference act on the entity. */
    AFW_OBJECT_GET_ENTITY(entity, instance);
    self = (afw_object_view_internal_object_self_t *)entity;

    /* Decrement count and release object's pool if zero. */
    if (afw_atomic_integer_decrement(&self->view->reference_count) == 0) {
        afw_pool_release(instance->p, xctx);
    }
}



/*
 * Implementation of method get_reference of interface afw_object.
 */
void
impl_afw_object_get_reference (
    const afw_object_t * instance,
    afw_xctx_t *xctx)
{
    afw_object_view_internal_object_self_t * self;
    const afw_object_t *entity;

    /* Methods release and get_reference act on the entity. */
    AFW_OBJECT_GET_ENTITY(entity, instance);
    self = (afw_object_view_internal_object_self_t *)entity;

    /* Increment reference count. */
    afw_atomic_integer_increment(&self->view->reference_count);
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
 * Implementation of method get_property of interface afw_object.
 */
const afw_value_t *
impl_afw_object_get_property(
    const afw_object_t * instance,
    const afw_utf8_t * property_name,
    afw_xctx_t *xctx)
{
    afw_object_view_internal_object_self_t * self =
        (afw_object_view_internal_object_self_t *)instance;
    afw_object_view_property_t *prop;

    prop = impl_get_property(self, property_name);

    return (prop) ? prop->value : NULL;
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
    afw_object_view_internal_object_self_t * self =
        (afw_object_view_internal_object_self_t *)instance;
    const afw_value_t *result;
    afw_object_view_property_t *prop;

    /* If iterator is NULL, point iterator to first property. */ 
    if (!*iterator) {
        prop = self->first_property;
    }
    else {
        prop = ((afw_object_view_property_t *)*iterator)->next_property;
    }
    *iterator = (afw_iterator_t *)prop;

    /*
     * If there are no more properties, return NULL for result and property
     * name.
     */
    if (!prop)
    {
        if (property_name) {
            *property_name = NULL;
        }
        result = NULL;
    }

    /* If there is another value, return it. */
    else {
        result = prop->value;
        if (property_name) {
            *property_name = prop->name;
        }
    }

    /* Return result. */
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
    return (impl_afw_object_get_property(instance,
        property_name, xctx) != NULL);
}



/*
 * Implementation of method get_setter of interface afw_object.
 */
const afw_object_setter_t *
impl_afw_object_get_setter (
    const afw_object_t * instance,
    afw_xctx_t *xctx)
{
    /* Views are immutable. */
    return NULL;
}



/*--- Public function defines. ---*/



/* Create an object view of an object in specified pool. */
AFW_DEFINE(const afw_object_t *)
afw_object_view_create(
    const afw_object_t *instance,
    const afw_utf8_t *entity_path,
    const afw_object_options_t *options,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_object_view_internal_object_self_t *self;
    afw_object_view_internal_view_t *view;
    const afw_utf8_t *reconcilable;
    afw_value_object_t self_value;
    const afw_value_t *value;

    /*
     * Add reference to instance.  Instance most have lifetime at least as
     * long as returned view object since uncloned values are used.
     */
    afw_object_get_reference(instance, xctx);

    /* If no options specified, just return instance asis. */
    if (!options || options->mask == 0)
    {
        return instance;
    }

    /* Create view common. */
    p = afw_pool_create(p, xctx);
    view = afw_pool_calloc_type(p, afw_object_view_internal_view_t, xctx);
    view->p = p;
    view->reference_count = 1;
    view->options = afw_object_options_create(NULL, options, p, xctx);

    /* Parse entity path and adaptor id. */
    if (!entity_path) {
        entity_path = afw_object_meta_get_path(instance, xctx);
    }
    if (entity_path) {
        view->uri_parsed = afw_uri_parse(entity_path, true, NULL, p, xctx);
    }
    if (!entity_path || !view->uri_parsed || !view->uri_parsed->path_parsed ||
        view->uri_parsed->path_parsed->adaptor_id.len == 0)
    {
        AFW_THROW_ERROR_Z(general,
            "Entity path required for afw_object_view_create()",
            xctx);
    }
    view->adaptor_id = &view->uri_parsed->path_parsed->adaptor_id;

    /** @fixme Should probably come in as parameter. */
    view->journal_entry = afw_object_create_managed(p, xctx);

    /* Create view object. */
    self = impl_object_create_entity(view, instance, view->uri_parsed, xctx);

    /* Add requested meta. */
    impl_additional_object_option_processing(self, xctx);

    /* If reconcilable option, add reconcilable meta. */
    if (AFW_OBJECT_OPTION_IS(options, reconcilable)) {
        self_value.inf = &afw_value_evaluated_object_inf;
        self_value.internal = (const afw_object_t *)self;
        reconcilable = afw_json_from_value(
            (const afw_value_t *)&self_value,
            &afw_object_options_reconcilable_meta_property,
            p, xctx);
        value = afw_value_create_string(reconcilable, p, xctx);
        impl_meta_set_property(self, &afw_self_s_reconcilable, value, xctx);
    }

    /* Return view. */
    return (const afw_object_t *)self;
}
