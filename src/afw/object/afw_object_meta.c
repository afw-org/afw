// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework interface helpers for afw_object*
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_object_meta.c
 * @brief Interface helpers for afw_object.
 */

#include "afw_internal.h"


#define impl_afw_object_get_meta \
    afw_object_impl_internal_get_meta

#define impl_afw_object_get_property_meta \
    afw_object_impl_internal_get_property_meta

#define impl_afw_object_get_next_property_meta \
    afw_object_impl_internal_get_next_property_meta


/* Declares and rti/inf defines for interface afw_object */
#define AFW_IMPLEMENTATION_ID "object_meta"
#include "afw_object_impl_declares.h"
#include "afw_object_setter_impl_declares.h"


#define IMPL_ASSERT_META_MUTABLE(instance, xctx) \
if (!(instance)->p) {\
    AFW_THROW_ERROR_Z(general, \
        "Can not set meta in a const object", xctx); \
}


static const afw_object_t *
impl_set_meta_object(
    afw_object_t *self,
    const afw_object_t *delta,
    afw_xctx_t *xctx)
{
    afw_object_meta_object_t *meta_self;

    meta_self = afw_pool_calloc_type(self->p,
        afw_object_meta_object_t, xctx);
    meta_self->pub.p = self->p;
    meta_self->pub.inf = &impl_afw_object_inf;
    meta_self->pub.meta.embedding_object = self;
    meta_self->pub.meta.id = &afw_s_a_meta_key;
    meta_self->pub.meta.object_type_uri = &afw_s__AdaptiveMeta_;
    meta_self->setter.inf = &impl_afw_object_setter_inf;
    meta_self->setter.object = (const afw_object_t *)meta_self;
    meta_self->delta = delta;
    self->meta.meta_object = (const afw_object_t *)meta_self;

    return delta;
}



/* Add a needed object type object. */
AFW_DEFINE(void)
afw_object_meta_add_needed_object_type(
    const afw_object_t *instance,
    const afw_object_t *object_type,
    afw_xctx_t *xctx)
{
    const afw_object_t *entity;
    const afw_object_t *meta;
    const afw_object_t *objectTypes;

    entity = afw_object_get_entity(instance, xctx);
    meta = afw_object_meta_get_nonempty_delta(entity, xctx);
    objectTypes = afw_object_old_get_property_as_object(meta,
        &afw_s_objectTypes, xctx);
    if (!objectTypes) {
        objectTypes = afw_object_create_embedded(meta,
            &afw_s_objectTypes, xctx);
    }
    afw_object_set_property_as_object(objectTypes,
        object_type->meta.id, object_type, xctx);
}



/* Set object's meta using a clone of the meta of another object. */
AFW_DEFINE(void)
afw_object_meta_clone_and_set(
    const afw_object_t *instance,
    const afw_object_t *from,
    afw_xctx_t *xctx)
{
    afw_object_t *self = (afw_object_t *)instance;

    if (!from->meta.meta_object) {
        return;
    }

    if (self->meta.meta_object) {
        AFW_THROW_ERROR_Z(general, "Instance already has meta", xctx);
    }

    impl_set_meta_object(
        (afw_object_t *)instance,
        afw_object_create_clone(
            afw_object_meta_object(from),
            instance->p, xctx),
        xctx);
}



/* Get entity object's object id value. */
AFW_DEFINE(const afw_value_string_t *)
afw_object_meta_get_object_id_value(
    const afw_object_t *instance,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *object_id;
    const afw_value_t *value;

    /** @fixme Check meta first?? Store in meta??? */
    object_id = afw_object_meta_get_object_id(instance, xctx);
    if (!object_id) {
        return NULL;
    }
    value = afw_value_create_string(object_id, instance->p, xctx);
    return (const afw_value_string_t *)value;
}



/* Add parent path. */
AFW_DEFINE(void)
afw_object_meta_add_parent_path(
    const afw_object_t *instance,
    const afw_utf8_t *parent_path,
    afw_xctx_t *xctx)
{
    const afw_value_array_t *existing_parent_paths;
    afw_value_array_t *parent_paths;
    const afw_object_t *meta;

    existing_parent_paths = afw_object_meta_get_parent_paths_value(instance, xctx);
    parent_paths = afw_value_allocate_array(instance->p, xctx);

    if (existing_parent_paths) {
        parent_paths->internal = afw_array_create_or_clone(
            existing_parent_paths->internal, afw_data_type_anyURI, false,
            instance->p, xctx);
    }
    else {
        parent_paths->internal = afw_array_of_create(
            afw_data_type_anyURI, instance->p, xctx);
    }
    
    afw_array_of_anyURI_add(parent_paths->internal, parent_path, xctx);
    meta = afw_object_meta_get_nonempty_delta(instance, xctx);
    afw_object_set_property(meta, &afw_s_parentPaths,
        (const afw_value_t *)parent_paths, xctx);
}



/* Get meta parentPaths property. */
AFW_DEFINE(const afw_value_array_t *)
afw_object_meta_get_parent_paths_value(
    const afw_object_t *instance,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;
    const afw_object_t *meta;

    value = NULL;
    meta = afw_object_meta_object(instance);
    if (meta) {
        value = afw_object_get_property(
            afw_object_meta_object(instance),
            &afw_s_parentPaths, xctx);
        if (value && !afw_value_is_array_of_anyURI(value))
        {
            AFW_THROW_ERROR_Z(general,
                "Expecting parentPaths to be a list of anyURI",
                xctx);
        }
    }

    return (const afw_value_array_t *)value;
}



/* Get entity object's object id. */
AFW_DEFINE(const afw_utf8_t *)
afw_object_meta_get_object_id(
    const afw_object_t *instance,
    afw_xctx_t *xctx)
{
    if (instance->meta.embedding_object) {
        AFW_THROW_ERROR_Z(general,
            "afw_object_meta_get_object_id() called for embedded object",
            xctx);
    }
    return instance->meta.id;
}



/* Get Get object's property name in embedding object.. */
AFW_DEFINE(const afw_utf8_t *)
afw_object_meta_get_property_name(
    const afw_object_t *instance,
    afw_xctx_t *xctx)
{
    if (!instance->meta.embedding_object) {
        AFW_THROW_ERROR_Z(general,
            "afw_object_meta_get_property_name() called for entity object",
            xctx);
    }
    return instance->meta.id;
}



/* Get an object's path. */
AFW_DEFINE(const afw_utf8_t *)
afw_object_meta_get_path(
    const afw_object_t *instance,
    afw_xctx_t *xctx)
{
    afw_object_t *self = (afw_object_t *)instance;
    const afw_utf8_t *path;

    /** @fixme prune to just path. */
    /* Get path from meta. */
    path = instance->meta.object_uri;

    /*
     * If there is not already a path and this is an embedded object, attempt
     * to make one and save it if it does not contain an asterisk.
     */
    if (!path && instance->p && instance->meta.embedding_object) {
        path = afw_object_path_make_for_embedded(instance,
            instance->p, xctx);
        self->meta.object_uri = path;
    }

    return path;
}



/*
 * Get the property type object for an object's property from the meta
 * of an object, creating it if needed
 */
AFW_DEFINE(const afw_object_t *)
afw_object_meta_get_property_type(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx)
{
    const afw_object_t *meta;
    const afw_object_t *property_types;
    const afw_object_t *property_type;

    /* This is here for const objects for now. */
    if (!instance->meta.meta_object && !instance->p) {
        return NULL;
    }

    meta = afw_object_meta_get_nonempty_delta(instance, xctx);

    property_types = afw_object_old_get_property_as_object(
        instance->meta.meta_object,
        &afw_s_propertyTypes, xctx);
    if (!property_types) {
        property_types = afw_object_create_embedded(
            meta, &afw_s_propertyTypes, xctx);
        ((afw_object_t *)property_types)->meta.object_type_uri =
            &afw_s__AdaptiveMetaPropertyTypes_;
    }

    property_type = afw_object_old_get_property_as_object(property_types,
        property_name, xctx);
    if (!property_type) {
        property_type = afw_object_old_get_property_as_object(
            instance->meta.meta_object,
            &afw_s_otherProperties, xctx);
        if (property_type) {
            property_type = afw_object_create_clone(property_type,
                instance->p, xctx);
            afw_object_set_property_as_object(property_types,
                property_name, property_type, xctx);
        }
        else {
            property_type = afw_object_create_embedded(property_types,
                property_name, xctx);
        }
        ((afw_object_t *)property_type)->meta.object_type_uri =
            &afw_s__AdaptiveMetaPropertyType_;
    }

    return property_type;
}



/* Set object's meta object. */
AFW_DEFINE(void)
afw_object_meta_set_meta_object(
    const afw_object_t *instance,
    const afw_object_t *meta,
    afw_xctx_t *xctx)
{
    afw_object_t *self = (afw_object_t *)instance;
    const afw_pool_t *p = instance->p;
    const afw_object_setter_t *setter;
    const afw_value_t *value;
    const afw_utf8_t *path;
    const afw_utf8_t *object_type_id;
    const afw_object_t *property_types;
    const afw_object_t *property_type;
    const afw_object_path_parsed_t *parsed_path;
    const afw_array_t *parent_paths;

    /* If no meta, just return. */
    if (!meta) {
        return;
    }

    /* If there is already meta, fail. */
    if (self->meta.meta_object) {
        AFW_THROW_ERROR_Z(general, "Instance already has meta", xctx);
    }

    /* Get setter for meta and fail if there is not one. */
    setter = afw_object_get_setter(meta, xctx);
    if (!setter) {
        AFW_THROW_ERROR_Z(general, "Meta must be mutable", xctx);
    }

    /* Make sure parentPaths is a list. */
    value = afw_object_get_property(meta, &afw_s_parentPaths, xctx);
    if (value) {
        parent_paths = afw_array_of_create_from_value(
            afw_data_type_anyURI, value, meta->p, xctx);
        afw_object_set_property_as_array(meta,
            &afw_s_parentPaths, parent_paths, xctx);
    }

    /* If path in meta, remove it from meta and if entity, use it to set path. */
    path = afw_object_old_get_property_as_utf8(meta, &afw_s_path,
        p, xctx);
    if (path) {
        parsed_path = afw_object_path_parse(path, NULL, NULL, p, xctx);
        if (!parsed_path->first_property_name) {
            afw_object_meta_set_ids_using_path(instance, path, xctx);
        }
        afw_object_set_property(meta, &afw_s_path, NULL, xctx);
    }

    /* Try to determine object type if path didn't set it. */
    if (!instance->meta.object_type_uri) {
        object_type_id = afw_object_old_get_property_as_string(meta,
            &afw_s_objectType, xctx);
        if (!object_type_id &&
            instance->meta.embedding_object &&
            instance->meta.embedding_object->meta.meta_object)
        {
            property_types = afw_object_old_get_property_as_object(
                meta, &afw_s_propertyTypes, xctx);
            if (property_types) {
                property_type = afw_object_old_get_property_as_object(
                    property_types, instance->meta.id, xctx);
                if (property_type) {
                    object_type_id = afw_object_old_get_property_as_string(
                        property_type, &afw_s_dataTypeParameter, xctx);
                }
            }
        }
        if (object_type_id) {
            afw_object_meta_set_object_type_id(instance, object_type_id, xctx);
        }
    }

    /* Set meta in instance. */
    impl_set_meta_object(self, meta, xctx);
}



AFW_DEFINE(const afw_object_t *)
afw_object_meta_set_empty(
    const afw_object_t *instance,
    afw_xctx_t *xctx)
{
    const afw_object_t *result;

    if (instance->meta.meta_object) {
        AFW_THROW_ERROR_Z(general, "Object already has meta", xctx);
    }

    IMPL_ASSERT_META_MUTABLE(instance, xctx);

    /* Set meta in instance. */
    result = impl_set_meta_object(
        (afw_object_t *)instance,
        afw_object_create(instance->p, xctx),
        xctx);

    return result;
}



AFW_DEFINE(const afw_object_meta_object_t *)
afw_object_meta_get_nonempty_meta_object(
    const afw_object_t *instance,
    afw_xctx_t *xctx)
{
    if (!instance->meta.meta_object) {
        afw_object_meta_set_empty(instance, xctx);
    }

    if (instance->meta.meta_object->inf != &impl_afw_object_inf) {
        AFW_THROW_ERROR_Z(general, "Object meta is not mutable", xctx);
    }

    return (const afw_object_meta_object_t *)instance->meta.meta_object;
}



AFW_DEFINE(const afw_object_t *)
afw_object_meta_get_nonempty_delta(
    const afw_object_t *instance,
    afw_xctx_t *xctx)
{
    if (!instance->meta.meta_object) {
        afw_object_meta_set_empty(instance, xctx);
    }

    if (instance->meta.meta_object->inf != &impl_afw_object_inf) {
        AFW_THROW_ERROR_Z(general, "Object meta is not mutable", xctx);
    }

    return ((const afw_object_meta_object_t *)instance->meta.meta_object)
        ->delta;
}



/* Set object's object type using afw object type id. */
AFW_DEFINE(void)
afw_object_meta_set_object_type(
    const afw_object_t *instance,
    const afw_object_type_t *object_type,
    afw_xctx_t *xctx)
{
    afw_object_meta_object_t *meta_object;

    IMPL_ASSERT_META_MUTABLE(instance, xctx);

    meta_object = (afw_object_meta_object_t *)
        afw_object_meta_get_nonempty_meta_object(instance, xctx);

    meta_object->object_type = object_type;
}



/* Set object's object type id. */
AFW_DEFINE(void)
afw_object_meta_set_object_type_id(
    const afw_object_t *instance,
    const afw_utf8_t *object_type_id,
    afw_xctx_t *xctx)
{
    IMPL_ASSERT_META_MUTABLE(instance, xctx);

    ((afw_object_t *)instance)->meta.object_type_uri = object_type_id;
}



/* Set meta parentPaths property. */
AFW_DEFINE(void)
afw_object_meta_set_parent_paths(
    const afw_object_t *instance,
    const afw_value_array_t *parent_paths,
    afw_xctx_t *xctx)
{
    const afw_object_t *meta;

    meta = afw_object_meta_get_nonempty_delta(instance, xctx);

    afw_object_set_property(meta, &afw_s_parentPaths,
        (const afw_value_t *)parent_paths, xctx);
}



/* Set object's ids. */
AFW_DEFINE(void)
afw_object_meta_set_ids(
    const afw_object_t *instance,
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    afw_xctx_t *xctx)
{
    afw_object_t *self = (afw_object_t *)instance;
    const afw_utf8_t *path;

    IMPL_ASSERT_META_MUTABLE(instance, xctx);
    AFW_OBJECT_ASSERT_ENTITY(instance, xctx);

    adaptor_id = afw_utf8_clone(adaptor_id, instance->p, xctx);
    object_type_id = afw_utf8_clone(object_type_id, instance->p, xctx);
    object_id = afw_utf8_clone(object_id, instance->p, xctx);

    self->meta.object_type_uri = object_type_id;
    self->meta.id = object_id;
    path = afw_object_path_make(
        adaptor_id, object_type_id, object_id, instance->p, xctx);
    self->meta.object_uri = path;
}



/* Set object's ids using path. */
AFW_DEFINE(void)
afw_object_meta_set_ids_using_path(
    const afw_object_t *instance,
    const afw_utf8_t *path,
    afw_xctx_t *xctx)
{
    afw_object_t *self = (afw_object_t *)instance;
    const afw_utf8_t *adaptor_id;
    const afw_utf8_t *object_type_id;
    const afw_utf8_t *object_id;

    IMPL_ASSERT_META_MUTABLE(instance, xctx);

    path = afw_utf8_clone(path, instance->p, xctx);
    afw_object_path_parse_simple(path,
        &adaptor_id, &object_type_id, &object_id,
        instance->p, xctx);
    self->meta.object_type_uri = object_type_id;
    self->meta.id = object_id;
    self->meta.object_uri = path;
    if (afw_utf8_equal(adaptor_id, &afw_s_a_asterisk) ||
        afw_utf8_equal(object_type_id, &afw_s_a_asterisk) ||
        afw_utf8_equal(object_id, &afw_s_a_asterisk))
    {
        AFW_THROW_ERROR_Z(general, "Unexpected asterisk", xctx);
    }
}



/* Set object's meta to indicate object is read-only. */
AFW_DEFINE(void)
afw_object_meta_set_read_only(
    const afw_object_t *instance,
    afw_xctx_t *xctx)
{
    afw_object_meta_set_property(instance,
        &afw_s_allowChange, afw_value_false, xctx);
    afw_object_meta_set_property(instance,
        &afw_s_allowDelete, afw_value_false, xctx);
}



AFW_DEFINE(void)
afw_object_meta_add_error(
    const afw_object_t *instance,
    const afw_utf8_t *message,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;
    const afw_object_t *meta;
    const afw_array_t *errors;

    meta = afw_object_meta_get_nonempty_delta(instance, xctx);
    errors = afw_object_old_get_property_as_array(meta, &afw_s_errors, xctx);
    if (!errors) {
        errors = afw_array_of_create(
            afw_data_type_string, instance->p, xctx);
        afw_object_set_property_as_array(meta,
            &afw_s_errors, errors, xctx);
        afw_object_set_property(meta, &afw_s_hasErrors, afw_value_true, xctx);
        afw_object_meta_set_property(afw_object_get_entity(instance, xctx),
            &afw_s_hasErrors, afw_value_true, xctx);
    }

    value = afw_value_create_string(message, instance->p, xctx);
    afw_array_add_value(errors, value, xctx);
}


/* Check if object flagged for errors. */
AFW_DEFINE(afw_boolean_t)
afw_object_meta_has_errors(
    const afw_object_t *instance,
    afw_xctx_t *xctx)
{
    afw_boolean_t result;

    result = false;
    if (instance->meta.meta_object) {
        result = afw_object_old_get_property_as_boolean_deprecated(
            afw_object_meta_object(instance),
            &afw_s_hasErrors, xctx);
    }

    return result;
}



static void
impl_log_errors(
    const afw_array_t *errors,
    const afw_utf8_t *source_location,
    afw_xctx_t *xctx)
{
    const afw_iterator_t *iterator;
    const afw_utf8_t *error;

    iterator = NULL;
    while ((error = afw_array_of_string_get_next(errors,
        &iterator, xctx)))
    {
        AFW_LOG_FZ(info, xctx,
            AFW_UTF8_CONTEXTUAL_LABEL_FMT
            AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(source_location),
            AFW_UTF8_FMT_ARG(error));
    }
}



/* Log meta errors. */
AFW_DEFINE(void)
afw_object_meta_log_errors(
    const afw_object_t *instance,
    const afw_utf8_t *source_location,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p;
    const afw_array_t *errors;
    const afw_object_t *meta;
    const afw_object_t *property_types;
    const afw_object_t *property_type;
    const afw_iterator_t *iterator;
    const afw_utf8_t *property_name;
    const afw_utf8_t *property_source_location;
    const afw_object_t *embedded;
    const afw_value_t *value;

    /* If no errors, return. */
    if (!afw_object_meta_has_errors(instance, xctx)) {
        return;
    }
    meta = afw_object_meta_object(instance);

    /* Use instance's p. */
    p = instance->p;

    /* Log object level errors. */
    errors = afw_object_old_get_property_as_array(meta, &afw_s_errors, xctx);
    if (errors) {
        impl_log_errors(errors, source_location, xctx);
    }

    /* Log property level errors. */
    property_types = afw_object_old_get_property_as_object(meta,
        &afw_s_propertyTypes, xctx);
    if (property_types) {
        iterator = NULL;
        while ((property_type = afw_object_old_get_next_property_as_object(
            property_types, &iterator, &property_name, xctx)))
        {
            errors = afw_object_old_get_property_as_array(property_type,
                &afw_s_errors, xctx);
            if (errors) {
                property_source_location = afw_utf8_printf(p, xctx,
                    AFW_UTF8_FMT
                    "." AFW_UTF8_FMT,
                    AFW_UTF8_FMT_ARG(source_location),
                    AFW_UTF8_FMT_ARG(property_name));
                impl_log_errors(errors, property_source_location, xctx);
            }
        }
    }

    /* Log errors for embedded objects. */
    iterator = NULL;
    while ((value = afw_object_get_next_property(instance,
        &iterator, &property_name, xctx)))
    {
        if (!afw_value_is_object(value)) {
            continue;
        }
        embedded = ((afw_value_object_t *)value)->internal;
        if (afw_object_meta_has_errors(embedded, xctx)) {
            property_source_location = afw_utf8_printf(p, xctx,
                AFW_UTF8_FMT
                "." AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(source_location),
                AFW_UTF8_FMT_ARG(property_name));
            afw_object_meta_log_errors(embedded,
                property_source_location, xctx);
        }
    }
}



AFW_DEFINE(void)
afw_object_meta_add_thrown_error(
    const afw_object_t *instance,
    const afw_error_t *error,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *message;

    if (afw_flag_is_active(
        xctx->env->flag_index_response_error_hasAdditionalDetail, xctx))
    {
        message = afw_error_to_utf8(error, instance->p, xctx);
    }

    else {
        message = afw_utf8_create_copy(error->message_z, AFW_UTF8_Z_LEN,
            instance->p, xctx);
    }

    afw_object_meta_add_error(instance, message, xctx);
}


AFW_DEFINE(void)
afw_object_meta_add_property_error(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    const afw_utf8_t *message,
    afw_xctx_t *xctx)
{
    const afw_object_t *property_type;
    const afw_value_t *value;
    const afw_array_t *errors;
 
    property_type = afw_object_meta_get_property_type(instance,
        property_name, xctx);
    errors = afw_object_old_get_property_as_array(property_type,
        &afw_s_errors, xctx);
    if (!errors) {
        errors = afw_array_of_create(
            afw_data_type_string, instance->p, xctx);
        afw_object_set_property_as_array(property_type,
            &afw_s_errors, errors, xctx);
        afw_object_meta_set_property(instance,
            &afw_s_hasErrors, afw_value_true, xctx);
        afw_object_meta_set_property(afw_object_get_entity(instance, xctx),
            &afw_s_hasErrors, afw_value_true, xctx);
    }

    value = afw_value_create_string(message, instance->p, xctx);
    afw_array_add_value(errors, value, xctx);
}

AFW_DEFINE(void)
afw_object_meta_add_thrown_property_error(
    const afw_object_t *instance,
    const afw_utf8_t *property_name,
    afw_integer_t index,
    const afw_error_t *error,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *message;

    /* If property name is NULL, just add error to object. */
    if (!property_name) {
        afw_object_meta_add_thrown_error(instance, error,  xctx);
        return;
    }

    /** @fixme Is the desired??? Log error.
    afw_error_write_log(afw_log_priority_debug, error, xctx);
     */

    if (afw_flag_is_active(
        xctx->env->flag_index_response_error_hasAdditionalDetail, xctx))
    {
        message = afw_error_to_utf8(error, instance->p, xctx);
        if (index >= 0) {
            message = afw_utf8_printf(instance->p, xctx,
                "[" AFW_INTEGER_FMT "] " AFW_UTF8_FMT,
                index,
                AFW_UTF8_FMT_ARG(message));
        }
    }

    else if (index >= 0) {
        message = afw_utf8_printf(instance->p, xctx,
                "[" AFW_INTEGER_FMT "] %s",
                index,
                error->message_z);
    }

    else {
        message = afw_utf8_create_copy(error->message_z, AFW_UTF8_Z_LEN,
            instance->p, xctx);
    }

    /* Add error to property's meta. */
    afw_object_meta_add_property_error(instance, property_name,
        message, xctx);
}



/*
 * Implementation of method release for interface afw_object.
 */
void
impl_afw_object_release(
    const afw_object_t *instance,
    afw_xctx_t *xctx)
{
    afw_object_meta_object_t *self =
        (afw_object_meta_object_t *)instance;

    afw_object_release(self->pub.meta.embedding_object, xctx);
}

/*
 * Implementation of method add_reference for interface afw_object.
 */
void
impl_afw_object_add_reference(
    const afw_object_t *instance,
    afw_xctx_t *xctx)
{
    afw_object_meta_object_t *self =
        (afw_object_meta_object_t *)instance;

    afw_object_add_reference(self->pub.meta.embedding_object, xctx);
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
    afw_object_meta_object_t *self =
        (afw_object_meta_object_t *)instance;
    const afw_value_t *result;

    result = afw_object_get_property(self->delta, property_name, xctx);

    if (!result && afw_utf8_equal(property_name, &afw_s_properties)) {

    }

    if (!result && self->object_type && self->object_type->object_type_object) {
        result = afw_object_get_property(self->object_type->object_type_object,
            property_name, xctx);
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
    afw_object_meta_object_t *self =
        (afw_object_meta_object_t *)instance;
    const afw_value_t *result;
    const afw_utf8_t *next_property_name;
    impl_get_next_property_iterator_t *i;

    if (!*iterator) {
        *iterator = (afw_iterator_t *)afw_xctx_calloc_type(
            impl_get_next_property_iterator_t, xctx);
        if (!self->object_type || !self->object_type->object_type_object)
        {
            if (!self->delta) {
                if (property_name) {
                    *property_name = NULL;
                }
                return NULL;
            }
            ((impl_get_next_property_iterator_t *)*iterator)->on_delta = true;
        }
    }
    i = (impl_get_next_property_iterator_t *)* iterator;

    result = NULL;
    if (!i->on_delta) {
        for (;;) {
            result = afw_object_get_next_property(
                self->object_type->object_type_object,
                &i->iterator, &next_property_name, xctx);
            if (!result) {
                if (!self->delta) {
                    if (property_name) {
                        *property_name = NULL;
                    }
                    return NULL;
                }
                i->on_delta = true;
                i->iterator = NULL;
                break;
            }
            if (self->delta &&
                !afw_object_has_property(self->delta, next_property_name,
                    xctx))
            {
                if (property_name) {
                    *property_name = next_property_name;
                }
                return result;
            }
        }
    }

    result = afw_object_get_next_property(
        self->delta, &i->iterator, property_name, xctx);
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
    afw_object_meta_object_t *self =
        (afw_object_meta_object_t *)instance;
    afw_boolean_t result;

    result = afw_object_has_property(self->delta, property_name, xctx);

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
    afw_object_meta_object_t *self =
        (afw_object_meta_object_t *)instance;

    return &self->setter;
}


/*
 * Implementation of method set_immutable for interface afw_object_setter.
 */
void
impl_afw_object_setter_set_immutable(
    const afw_object_setter_t * instance,
    afw_xctx_t *xctx)
{
    /** @todo Add code to implement method. */
    AFW_THROW_ERROR_Z(general, "Method not implemented.", xctx);

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
    afw_object_meta_object_t *self =
        (afw_object_meta_object_t *)instance->object;

    if (afw_utf8_equal(property_name, &afw_s_path)) {
        AFW_VALUE_ASSERT_IS_ANYURI_OR_STRING(value, xctx);
        afw_object_meta_set_ids_using_path(
            self->pub.meta.embedding_object,
            &((const afw_value_anyURI_t *)value)->internal,
            xctx);
    }

    else {
        if (!self->delta) {
            self->delta = afw_object_create(self->pub.p, xctx);
        }
        afw_object_set_property(self->delta, property_name, value, xctx);
    }
}
