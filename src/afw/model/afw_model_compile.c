// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Model Compile
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_model_compile.c
 * @brief Adaptive Framework Model Compile
 */

#include "afw_internal.h"


static const afw_object_t *
impl_compile_custom(
    const afw_object_t *custom,
    const afw_model_t *model,
    afw_xctx_t *xctx);


static const afw_model_property_type_t *
impl_compile_property_type(
    afw_model_object_type_t *object_type,
    const afw_object_t *object,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx);


static const afw_model_object_type_t *
impl_object_type_compile(
    afw_model_t *model,
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *object_type_id,
    const afw_object_t *object,
    afw_xctx_t *xctx);


static const afw_object_t *
impl_compile_custom(
    const afw_object_t *custom,
    const afw_model_t *model,
    afw_xctx_t *xctx)
{
    const afw_object_t *result;
    const afw_iterator_t *iterator;
    const afw_utf8_t *property_name;
    const afw_utf8_t *s;
    const afw_value_t *value;
    const afw_utf8_t *source_location;
    const afw_utf8_t *path;

    result = afw_object_create_managed(model->p, xctx);
    path = afw_object_meta_get_path(custom, xctx);

    for (iterator = NULL;;) {
        s = afw_object_old_get_next_property_as_string(custom,
            &iterator, &property_name, xctx);
        if (!s) break;
        source_location = afw_utf8_printf(model->p, xctx,
            AFW_UTF8_FMT "/custom/" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(path),
            AFW_UTF8_FMT_ARG(property_name));
        value = afw_compile_template_source(s, source_location,
            NULL, model->shared, NULL, xctx);
        afw_object_set_property(result, property_name, value, xctx);
    }

    return result;
}



const afw_model_property_type_t *
impl_compile_property_type(
    afw_model_object_type_t *object_type,
    const afw_object_t *object,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx)
{
    const afw_model_t *model = object_type->model;
    const afw_pool_t *p = model->p;
    afw_model_property_type_t *pt;
    const afw_utf8_t *s;
    const afw_object_t *custom;
    const afw_utf8_t *source_location;
    const afw_utf8_t *path;
    afw_boolean_t b;
    afw_boolean_t found;
 
    /* Allocate and initialize property type struct. */
    pt = afw_pool_calloc_type(p, afw_model_property_type_t, xctx);
    pt->property_name = property_name;
    pt->property_name_value = afw_value_create_string(
        property_name, p, xctx);
    pt->property_type_object = object;
    pt->property_type_object_value = afw_value_create_object(
        pt->property_type_object, p, xctx);
    path = pt->property_type_path = afw_object_meta_get_path(object, xctx);
    pt->property_type_path_value = afw_value_create_anyURI(
        pt->property_type_path, p, xctx);

    /* dataType */
    s = afw_object_old_get_property_as_string(object,
        &afw_self_s_dataType, xctx);
    if (s) {
        pt->data_type = afw_environment_get_data_type(s, xctx);
        if (!pt->data_type) {
            AFW_THROW_ERROR_FZ(general, xctx,
                AFW_UTF8_FMT
                "/dataType is invalid",
                AFW_UTF8_FMT_ARG(path));
        }
    }

    /* constraint */
    /** @fixme*/

    /* custom */
    custom = afw_object_old_get_property_as_object(object,
        &afw_self_s_custom, xctx);
    if (custom) {
        pt->custom_variables = impl_compile_custom(
            custom, model, xctx);
    }

    /* allowQuery - default false */
    b = afw_object_old_get_property_as_boolean(object,
        &afw_self_s_allowQuery, &found, xctx);
    pt->allow_query = found && b;

    /* allowRead - defaults to true */
    b = afw_object_old_get_property_as_boolean(object,
        &afw_self_s_allowRead, &found, xctx);
    pt->allow_query = !found || b;

    /* allowWrite - default to true */
    b = afw_object_old_get_property_as_boolean(object,
        &afw_self_s_allowWrite, &found, xctx);
    pt->allow_write = !found || b;

    /* required - default false */
    b = afw_object_old_get_property_as_boolean(object,
        &afw_self_s_required, &found, xctx);
    pt->required = found && b;

    /* transitory - default false */
    b = afw_object_old_get_property_as_boolean(object,
        &afw_self_s_transitory, &found, xctx);
    pt->transitory = found && b;

    /* unique - default false */
    b = afw_object_old_get_property_as_boolean(object,
        &afw_self_s_unique, &found, xctx);
    pt->unique = found && b;

    /* defaultValue */
    s = afw_object_old_get_property_as_string(object,
        &afw_self_s_defaultValue, xctx);
    if (s) {
        source_location = afw_utf8_printf(p, xctx,
            AFW_UTF8_FMT "/defaultValue",
            AFW_UTF8_FMT_ARG(path));
        pt->default_value = afw_compile_template_source(
            s,source_location, NULL, model->shared, NULL, xctx);
    }

    /* mappedPropertyName */
    pt->mapped_property_name = afw_object_old_get_property_as_string(object,
        &afw_self_s_mappedPropertyName, xctx);
    if (!pt->mapped_property_name) {
        pt->mapped_property_name = pt->property_name;
    }
    pt->mapped_property_name_value = afw_value_create_string(
        pt->mapped_property_name, p, xctx);

    /* onGetProperty */
    s = afw_object_old_get_property_as_string(object,
        &afw_self_s_onGetProperty, xctx);
    if (s) {
        source_location = afw_utf8_printf(p, xctx,
            AFW_UTF8_FMT "/onGetProperty",
            AFW_UTF8_FMT_ARG(path));
        pt->onGetProperty = afw_compile_script_source(
            s, source_location, NULL, model->shared, NULL, xctx);
    }

    /* onGetInitialValue */
    s = afw_object_old_get_property_as_string(object,
        &afw_self_s_onGetInitialValue, xctx);
    if (s) {
        source_location = afw_utf8_printf(p, xctx,
            AFW_UTF8_FMT "/onGetInitialValue",
            AFW_UTF8_FMT_ARG(path));
        pt->onGetInitialValue = afw_compile_script_source(
            s, source_location, NULL, model->shared, NULL, xctx);
    }

    /* setProperty */
    s = afw_object_old_get_property_as_string(object,
        &afw_self_s_onSetProperty, xctx);
    if (s) {
        source_location = afw_utf8_printf(p, xctx,
            AFW_UTF8_FMT "/onSetProperty",
            AFW_UTF8_FMT_ARG(path));
        pt->onSetProperty = afw_compile_script_source(
            s, source_location, NULL, model->shared, NULL, xctx);
    }

    /* Return property type struct. */
    return pt;
}


static afw_boolean_t
impl_is_inherited(
    const afw_object_t *object,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx)
{
    afw_boolean_t result;
    const afw_object_t *propertyTypes;
    const afw_object_t *property;
    const afw_value_t *inheritedFrom;

    result = false;
    if (object->meta.meta_object) {
        propertyTypes = afw_object_old_get_property_as_object(
            afw_object_meta_object(object), &afw_self_s_propertyTypes, xctx);
        if (propertyTypes) {
            property = afw_object_old_get_property_as_object(
                propertyTypes, property_name, xctx);
            if (property) {
                inheritedFrom = afw_object_get_property(
                    property, &afw_self_s_inheritedFrom, xctx);
                if (inheritedFrom) {
                    result = true;
                }
            }
        }
    }

    return result;
}


static void
impl_harvest_property_type(
    const afw_object_t *embedding_object,
    const afw_utf8_t *property_name,
    afw_boolean_t composite,
    const afw_object_t *object,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_object_t *result;
    const afw_utf8_t *s;
    const afw_value_t *value;

    result = afw_object_create_embedded(
        embedding_object, property_name, xctx);
    afw_object_meta_set_object_type_id(result,
        &afw_self_s__AdaptiveValueMeta_, xctx);

    /** @fixme This should be a script. */
    /* additionalConstraints */
    value = afw_object_get_property(object,
        &afw_self_s_additionalConstraints, xctx);
    if (value) {
        afw_object_set_property(result,
            &afw_self_s_additionalConstraints, value, xctx);
    }

    /* allowQuery */
    value = afw_object_get_property(object,
        &afw_self_s_allowQuery, xctx);
    if (value) {
        afw_object_set_property(result,
            &afw_self_s_allowQuery, value, xctx);
    }

    /* allowWrite */
    value = afw_object_get_property(object,
        &afw_self_s_allowWrite, xctx);
    if (value) {
        afw_object_set_property(result,
            &afw_self_s_allowWrite, value, xctx);
    }

    /* collectionURIs */
    s = afw_object_old_get_property_as_utf8(object,
        &afw_self_s_collectionURIs, p, xctx);
    if (s) {
        afw_object_set_property_as_anyURI(result,
            &afw_self_s_collectionURIs, s, xctx);
    }

    /* contextType */
    s = afw_object_old_get_property_as_utf8(object,
        &afw_self_s_contextType, p, xctx);
    if (s) {
        afw_object_set_property_as_string(result,
            &afw_self_s_contextType, s, xctx);
    }

    /* dataType */
    s = afw_object_old_get_property_as_utf8(object,
        &afw_self_s_dataType, p, xctx);
    if (s) {
        afw_object_set_property_as_string(result,
            &afw_self_s_dataType, s, xctx);
    }

    /* dataTypeParameter */
    s = afw_object_old_get_property_as_utf8(object,
        &afw_self_s_dataTypeParameter, p, xctx);
    if (s) {
        afw_object_set_property_as_string(result,
            &afw_self_s_dataTypeParameter, s, xctx);
    }

    /* description */
    s = afw_object_old_get_property_as_utf8(object,
        &afw_self_s_description, p, xctx);
    if (s) {
        afw_object_set_property_as_string(result,
            &afw_self_s_description, s, xctx);
    }

    /* label */
    s = afw_object_old_get_property_as_utf8(object,
        &afw_self_s_label, p, xctx);
    if (s) {
        afw_object_set_property_as_string(result,
            &afw_self_s_label, s, xctx);
    }

    /* originURI */
    s = afw_object_old_get_property_as_utf8(object,
        &afw_self_s_originURI, p, xctx);
    if (s) {
        afw_object_set_property_as_anyURI(result,
            &afw_self_s_originURI, s, xctx);
    }

    /* referenceURI */
    s = afw_object_old_get_property_as_utf8(object,
        &afw_self_s_referenceURI, p, xctx);
    if (s) {
        afw_object_set_property_as_anyURI(result,
            &afw_self_s_referenceURI, s, xctx);
    }

    /* required */
    value = afw_object_get_property(object,
        &afw_self_s_required, xctx);
    if (value) {
        afw_object_set_property(result,
            &afw_self_s_required, value, xctx);
    }

    /* testDataParameter */
    s = afw_object_old_get_property_as_utf8(object,
        &afw_self_s_testDataParameter, p, xctx);
    if (s) {
        afw_object_set_property_as_string(result,
            &afw_self_s_testDataParameter, s, xctx);
    }

    /* unique */
    value = afw_object_get_property(object,
        &afw_self_s_unique, xctx);
    if (value) {
        afw_object_set_property(result,
            &afw_self_s_unique, value, xctx);
    }
}



static const afw_object_t *
impl_harvest_object_type(
    afw_model_t *model,
    afw_boolean_t composite,
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *object_type_id,
    const afw_object_t *object,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_object_t *result;
    const afw_utf8_t *s;
    const afw_object_t *obj;
    const afw_object_t *property_types;
    const afw_object_t *propertyTypes;
    const afw_utf8_t *property_name;
    const afw_iterator_t *iterator;
    const afw_value_array_t *from_parent_paths;
    afw_value_array_t *to_parent_paths;
    const afw_utf8_t *path;
    afw_boolean_t flag;
    afw_boolean_t found;
    const afw_value_t *value;

    result = afw_object_create(p, xctx);
    afw_object_meta_set_ids(result,
        adaptor_id,
        &afw_self_s__AdaptiveObjectType_,
        object_type_id, xctx);
    afw_object_meta_set_property(result,
        &afw_self_s_allowAdd, afw_value_false, xctx);
    afw_object_meta_set_property(result,
        &afw_self_s_allowChange, afw_value_false, xctx);
    afw_object_meta_set_property(result,
        &afw_self_s_allowDelete, afw_value_false, xctx);

    /* If object has resolvedParentPaths, use as parentPaths. */
    value = afw_object_meta_get_property(object,
        &afw_self_s_resolvedParentPaths, xctx);
    if (value) {
        AFW_VALUE_ASSERT_IS_DATA_TYPE(value, array, xctx);
        from_parent_paths = (const afw_value_array_t *)value;
        to_parent_paths = afw_value_allocate_array(p, xctx);
        to_parent_paths->internal =  
            afw_array_of_create(afw_data_type_anyURI, p, xctx);
        for (iterator = NULL;;) {
            path = afw_array_of_utf8_get_next(
                from_parent_paths->internal, &iterator, xctx);
            if (!path) break;
            if (afw_utf8_starts_with(path, model->objectType_path)) {
                s = afw_utf8_printf(p, xctx,
                    "/*/_AdaptiveObjectType_/" AFW_UTF8_FMT,
                    (int)(path->len - model->objectType_path->len),
                    (char *)(path->s + model->objectType_path->len));
                afw_array_of_anyURI_add(to_parent_paths->internal, s, xctx);
            }
            else {
                afw_array_of_anyURI_add(to_parent_paths->internal, path, xctx);
            }
        }
        afw_object_meta_set_property(result,
            &afw_self_s_parentPaths, (const afw_value_t *)to_parent_paths, xctx);
    }

    /* allowAdd */
    flag = afw_object_old_get_property_as_boolean(object,
        &afw_self_s_allowAdd, &found, xctx);
    if (found) {
        afw_object_set_property(result,
            &afw_self_s_allowAdd, afw_value_for_boolean(flag), xctx);
    }

    /* allowChange */
    flag = afw_object_old_get_property_as_boolean(object,
        &afw_self_s_allowChange, &found, xctx);
    if (found) {
        afw_object_set_property(result,
            &afw_self_s_allowChange, afw_value_for_boolean(flag), xctx);
    }

    /* allowDelete */
    flag = afw_object_old_get_property_as_boolean(object,
        &afw_self_s_allowDelete, &found, xctx);
    if (found) {
        afw_object_set_property(result,
            &afw_self_s_allowDelete, afw_value_for_boolean(flag), xctx);
    }

    /* allowEntity */
    flag = afw_object_old_get_property_as_boolean(object,
        &afw_self_s_allowEntity, &found, xctx);
    if (found) {
        afw_object_set_property(result,
            &afw_self_s_allowEntity, afw_value_for_boolean(flag), xctx);
    }

    /* collectionURIs */
    s = afw_object_old_get_property_as_utf8(object,
        &afw_self_s_collectionURIs, p, xctx);
    if (s) {
        afw_object_set_property_as_anyURI(result,
            &afw_self_s_collectionURIs, s, xctx);
    }

    /* description */
    s = afw_object_old_get_property_as_utf8(object,
        &afw_self_s_description, p, xctx);
    if (s) {
        afw_object_set_property_as_string(result,
            &afw_self_s_description, s, xctx);
    }

    /* descriptionPropertyName */
    s = afw_object_old_get_property_as_utf8(object,
        &afw_self_s_descriptionPropertyName, p, xctx);
    if (s) {
        afw_object_set_property_as_string(result,
            &afw_self_s_descriptionPropertyName, s, xctx);
    }

    /* label */
    s = afw_object_old_get_property_as_utf8(object,
        &afw_self_s_label, p, xctx);
    if (s) {
        afw_object_set_property_as_string(result,
            &afw_self_s_label, s, xctx);
    }

    /* objectIdPropertyName */
    s = afw_object_old_get_property_as_utf8(object,
        &afw_self_s_objectIdPropertyName, p, xctx);
    if (s) {
        afw_object_set_property_as_string(result,
            &afw_self_s_objectIdPropertyName, s, xctx);
    }

    /* objectType */
    afw_object_set_property_as_string(result,
        &afw_self_s_objectType, object_type_id, xctx);

    /* originURI */
    s = afw_object_old_get_property_as_utf8(object,
        &afw_self_s_originURI, p, xctx);
    if (s) {
        afw_object_set_property_as_anyURI(result,
            &afw_self_s_originURI, s, xctx);
    }

    /* otherProperties. */
    obj = afw_object_old_get_property_as_object(object,
        &afw_self_s_otherProperties, xctx);
    if (obj) {
        impl_harvest_property_type(
            result, &afw_self_s_otherProperties,
            composite, obj, p, xctx);
    }

    /* propertyTypes */
    property_types = afw_object_old_get_property_as_object(object,
        &afw_self_s_propertyTypes, xctx);
    if (property_types) {

        /* Make new propertyTypes object. */
        propertyTypes = afw_object_create_embedded(
            result, &afw_self_s_propertyTypes, xctx);
        afw_object_meta_set_object_type_id(propertyTypes,
            &afw_self_s__AdaptivePropertyTypes_, xctx);

        /* If object has resolvedParentPaths, use as parentPaths. */
        value = afw_object_meta_get_property(property_types,
            &afw_self_s_resolvedParentPaths, xctx);
        if (value) {
            afw_object_meta_set_property(propertyTypes,
                &afw_self_s_parentPaths, value, xctx);
        }

        /* Add all properties that were not inherited. */
        for (iterator = NULL;;) {
            obj = afw_object_old_get_next_property_as_object(
                property_types, &iterator, &property_name, xctx);
            if (!obj) break;
            if (!impl_is_inherited(property_types, property_name, xctx)) {
                impl_harvest_property_type(
                    propertyTypes, property_name,
                    composite, obj, p, xctx);
            }
        }
    }

    /* referenceURI */
    s = afw_object_old_get_property_as_utf8(object,
        &afw_self_s_referenceURI, p, xctx);
    if (s) {
        afw_object_set_property_as_anyURI(result,
            &afw_self_s_referenceURI, s, xctx);
    }

    /* Return result. */
    return result;
}


const afw_model_object_type_t *
impl_object_type_compile(
    afw_model_t *model,
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *object_type_id,
    const afw_object_t *object,
    afw_xctx_t *xctx)
{
    const afw_compile_shared_t *shared = model->shared;
    const afw_pool_t *p = model->p;
    afw_model_object_type_t *ot;
    const afw_object_t *properties;
    const afw_iterator_t *iterator;
    const afw_object_t *pt_object;
    const afw_model_property_type_t * *pt;
    const afw_utf8_t *property_name;
    afw_size_t count;
    const afw_object_t *custom;
    const afw_utf8_t *path;
    const afw_utf8_t *source_location;
    const afw_utf8_t *s;

    /* Allocate afw_model_object_type_t and initialize. */
    ot = afw_pool_calloc_type(p, afw_model_object_type_t, xctx);
    ot->model = model;
    ot->object_type_object = impl_harvest_object_type(model, false,
        adaptor_id, object_type_id, object,
        p, xctx);
    ot->object_type_object_value = afw_value_create_object(
        ot->object_type_object, p, xctx);
    ot->object_type_path = afw_object_meta_get_path(object, xctx);
    ot->object_type_path_value = afw_value_create_anyURI(
        ot->object_type_path, p, xctx);
    ot->object_type_id = afw_object_meta_get_property_name(object, xctx);
    ot->object_type_id_value = afw_value_create_string(
        ot->object_type_id, p, xctx);

    /* custom */
    custom = afw_object_old_get_property_as_object(object,
        &afw_self_s_custom, xctx);
    if (custom) {
        ot->custom_variables = impl_compile_custom(
            custom, model, xctx);
    }

    /* Get path. */
    path = afw_object_meta_get_path(object, xctx);

    /* onGetInitialObjectId */
    s = afw_object_old_get_property_as_utf8(object,
        &afw_self_s_onGetInitialObjectId, p, xctx);
    if (s) {
        source_location = afw_utf8_printf(p, xctx,
            AFW_UTF8_FMT "/onGetInitialObjectId",
            AFW_UTF8_FMT_ARG(path));
        ot->onGetInitialObjectId = afw_compile_script_source(s,
            source_location,
            NULL, model->shared, NULL, xctx);
    }

    /* mappedObjectType */
    ot->mapped_object_type_id_value = afw_object_get_property(object,
        &afw_self_s_mappedObjectType, xctx);
    if (ot->mapped_object_type_id_value) {
        ot->mapped_object_type_id = afw_value_as_string(
            ot->mapped_object_type_id_value, xctx);
    }
    else {
        ot->mapped_object_type_id_value = ot->object_type_id_value;
        ot->mapped_object_type_id = ot->object_type_id;
    }

    /* descriptionPropertyName */
    ot->description_property_name_value = afw_object_get_property(object,
        &afw_self_s_descriptionPropertyName, xctx);
    if (ot->description_property_name_value) {
        ot->description_property_name = afw_value_as_string(
            ot->description_property_name_value, xctx);
    }

    /* objectIdPropertyName */
    ot->object_id_property_name_value = afw_object_get_property(object,
        &afw_self_s_objectIdPropertyName, xctx);
    if (ot->object_id_property_name_value) {
        ot->object_id_property_name = afw_value_as_string(
            ot->object_id_property_name_value, xctx);
    }

    /* Count the number of properties. */
    properties = afw_object_old_get_property_as_object(object,
        &afw_self_s_propertyTypes, xctx);
    count = 0;
    if (properties) {
        for (iterator = NULL;
            (afw_object_old_get_next_property_as_object(
                properties, &iterator, &property_name, xctx));
            count++);
    }

    /* Create NULL terminated array of compiled property types. */
    ot->property_type = afw_pool_malloc(p,
        (count + 1) * sizeof(afw_model_property_type_t *),
        xctx);
    ot->property_type[count] = NULL;
    if (properties) {
        for (iterator = NULL,
            pt = ot->property_type;
            (pt_object = afw_object_old_get_next_property_as_object(
                properties, &iterator, &property_name, xctx));
            pt++)
        {
            *pt = impl_compile_property_type(
                ot, pt_object, pt_object->meta.id, xctx);
        }
    }

    /* onAddObject */
    s = afw_object_old_get_property_as_utf8(object,
        &afw_self_s_onAddObject, p, xctx);
    if (s) {
        source_location = afw_utf8_printf(p, xctx,
            AFW_UTF8_FMT "/onAddObject",
            AFW_UTF8_FMT_ARG(path));
        ot->onAddObject = afw_compile_script_source(
            s, source_location, NULL, shared, p, xctx);
    }

    /* onDeleteObject */
    s = afw_object_old_get_property_as_utf8(object,
        &afw_self_s_onDeleteObject, p, xctx);
    if (s) {
        source_location = afw_utf8_printf(p, xctx,
            AFW_UTF8_FMT "/onDeleteObject",
            AFW_UTF8_FMT_ARG(path));
        ot->onDeleteObject = afw_compile_script_source(
            s, source_location, NULL, shared, p, xctx);
    }

    /* onGetObject */
    s = afw_object_old_get_property_as_utf8(object,
        &afw_self_s_onGetObject, p, xctx);
    if (s) {
        source_location = afw_utf8_printf(p, xctx,
            AFW_UTF8_FMT "/onGetObject",
            AFW_UTF8_FMT_ARG(path));
        ot->onGetObject = afw_compile_script_source(
            s, source_location, NULL, shared, p, xctx);
    }

    /* onModifyObject */
    s = afw_object_old_get_property_as_utf8(object,
        &afw_self_s_onModifyObject, p, xctx);
    if (s) {
        source_location = afw_utf8_printf(p, xctx,
            AFW_UTF8_FMT "/onModifyObject",
            AFW_UTF8_FMT_ARG(path));
        ot->onModifyObject = afw_compile_script_source(
            s, source_location, NULL, shared, p, xctx);
    }

    /* onReplaceObject */
    s = afw_object_old_get_property_as_utf8(object,
        &afw_self_s_onReplaceObject, p, xctx);
    if (s) {
        source_location = afw_utf8_printf(p, xctx,
            AFW_UTF8_FMT "/onReplaceObject",
            AFW_UTF8_FMT_ARG(path));
        ot->onReplaceObject = afw_compile_script_source(s,
            source_location, NULL, shared, p, xctx);
    }

    /* onRetrieveObjects */
    s = afw_object_old_get_property_as_utf8(object,
        &afw_self_s_onRetrieveObjects, p, xctx);
    if (s) {
        source_location = afw_utf8_printf(p, xctx,
            AFW_UTF8_FMT "/onRetrieveObjects",
            AFW_UTF8_FMT_ARG(path));
        ot->onRetrieveObjects = afw_compile_script_source(
            s, source_location, NULL, shared, p, xctx);
    }

    /* Compile otherProperties, if it exists. */
    pt_object = afw_object_old_get_property_as_object(object,
        &afw_self_s_otherProperties, xctx);
    if (pt_object) {
        ot->property_type_other =
            impl_compile_property_type(
                ot, pt_object, pt_object->meta.id, xctx);
    }

    /* Return object type struct. */
    return ot;
}



AFW_DEFINE(afw_model_t *)
afw_model_compile(
    const afw_utf8_t *adaptor_id,
    const afw_object_t *model_object,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_object_t *objectTypes;
    const afw_iterator_t *iterator;
    const afw_object_t *object_type;
    const afw_model_object_type_t *model_object_type;
    const afw_utf8_t *property_name;
    afw_model_t *model;
    const afw_utf8_t *path;
    const afw_object_t *clone;
    const afw_object_t *object;

    /* Allocate model and clone model_object to correct p. */
    model = afw_pool_calloc_type(p, afw_model_t, xctx);
    model->p = p;
    model->shared = afw_compile_shared_create(p, xctx);

    /* Create composite view and clone it. */
    clone = afw_object_create_clone(
        model_object, p, xctx);
    object = afw_object_view_create(clone, NULL,
        &afw_object_options_composite_inheritedFrom_resolvedParentPaths,
        p, xctx);
    model->model_object = object;
    model->model_id = afw_object_meta_get_object_id(model->model_object, xctx);
    model->model_object_types = apr_hash_make(afw_pool_get_apr_pool(p));
    path = afw_object_meta_get_path(object, xctx);
    model->objectType_path = afw_utf8_printf(p, xctx,
        AFW_UTF8_FMT "/objectTypes/", AFW_UTF8_FMT_ARG(path));

    /* custom */
    model->custom_variables = afw_object_old_get_property_as_object(object,
        &afw_self_s_custom, xctx);
    if (model->custom_variables) {
        model->custom_variables = impl_compile_custom(
            model->custom_variables, model, xctx);
    }

    /* Get objectTypes property from model. */
    objectTypes = afw_object_old_get_property_as_object(
        model->model_object, &afw_self_s_objectTypes, xctx);
    if (!objectTypes) {
        AFW_THROW_ERROR_Z(general,
            "Object model must have objectTypes property", xctx);
    }

    /* Iterate compiling objectTypes and adding to object_types. */
    for (iterator = NULL;;) {
        object_type = afw_object_old_get_next_property_as_object(
            objectTypes, &iterator, &property_name, xctx);
        if (!object_type) break;
        model_object_type = impl_object_type_compile(model,
            adaptor_id, property_name, object_type, xctx);
        apr_hash_set(model->model_object_types,
            property_name->s, property_name->len, model_object_type);
    }

    /* Return model. */
    return model;
}
