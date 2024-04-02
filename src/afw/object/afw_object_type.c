// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Object Type
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_object_type.c
 * @brief Object type helpers.
 */

#include "afw_internal.h"


static afw_object_type_property_type_t *
impl_create_property_type(
    const afw_object_t *property_types_object,
    const afw_object_t *property_type_object,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_object_type_property_type_t *self;
    const afw_utf8_t *dataType;
    afw_boolean_t found;

    self = afw_pool_calloc_type(p, afw_object_type_property_type_t, xctx);
    self->property_type_object = property_type_object;

    self->context_type_id = afw_object_old_get_property_as_string(
        property_type_object, afw_s_contextType, xctx);

    self->default_value = afw_object_get_property(property_type_object,
        afw_s_defaultValue, xctx);

    self->data_type_parameter = afw_object_old_get_property_as_string(
        property_type_object,  afw_s_dataTypeParameter, xctx);

    dataType = afw_object_old_get_property_as_string(property_type_object,
        afw_s_dataType, xctx);
    if (dataType) {
        self->data_type = afw_environment_get_data_type(dataType, xctx);
    }

    self->allow_write = afw_object_old_get_property_as_boolean(
        property_type_object, afw_s_allowWrite, &found, xctx) || !found;

    self->allow_query = afw_object_old_get_property_as_boolean_deprecated(property_type_object,
        afw_s_allowQuery, xctx);

    self->required = afw_object_old_get_property_as_boolean_deprecated(property_type_object,
        afw_s_required, xctx);

    /** @fixme Normalize default_value, etc. */

    return self;
}


const afw_object_type_t *
afw_object_type_internal_create(
    const afw_adaptor_t *adaptor,
    const afw_object_t *object_type_object,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_object_type_t *self;
    const afw_iterator_t *iterator;
    const afw_object_t *property_type_object;
    afw_object_type_property_type_t *property_type;
    const afw_utf8_t *property_name;

    self = afw_pool_calloc_type(p, afw_object_type_t, xctx);
    self->adaptor_id = &adaptor->adaptor_id;
    self->object_type_object = object_type_object;
    self->object_type_id = object_type_object->meta.id;

    self->property_types_object = afw_object_old_get_property_as_object(
        object_type_object, afw_s_propertyTypes, xctx);
    if (self->property_types_object) {
        iterator = NULL;
        while ((property_type_object =
            afw_object_old_get_next_property_as_object(self->property_types_object,
                &iterator, &property_name, xctx)))
        {
            property_type = impl_create_property_type(
                self->property_types_object, property_type_object, p, xctx);
            property_type->property_name = property_name;
            property_type->next = self->first_property_type;
            self->first_property_type = property_type;
        }
    }

    self->other_properties_object = afw_object_old_get_property_as_object(
        object_type_object, afw_s_otherProperties, xctx);
    if (self->other_properties_object) {
        self->other_properties =
            impl_create_property_type(
                self->property_types_object, self->other_properties_object,
                p, xctx);
    }

    return self;
}



/* Get property type object for property */
AFW_DEFINE(const afw_object_type_property_type_t *)
afw_object_type_property_type_get(
    const afw_object_type_t *object_type,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx)
{
    const afw_object_type_property_type_t *result;

    for (result = object_type->first_property_type;
        result && !afw_utf8_equal(result->property_name, property_name);
        result = result->next);

    if (!result) {
        result = object_type->other_properties;
    }

    return result;
}


/* Get next property type for object type. */
AFW_DEFINE(const afw_object_type_property_type_t *)
afw_object_type_property_type_get_next(
    const afw_object_type_t *object_type,
    const afw_iterator_t * *iterator,
    const afw_utf8_t * *property_name,
    afw_xctx_t *xctx)
{
    const afw_object_type_property_type_t *result;

    /**  @fixme change after compile. This wastes a lot of time and space. */
    if (!object_type->first_property_type) {
        *iterator = NULL;
        if (property_name) {
            *property_name = NULL;
        }
        return NULL;
    }
    
    if (!*iterator) {
        result = object_type->first_property_type;
    }
    else {
        result = ((afw_object_type_property_type_t *)*iterator)->next;
    }

    if (result && property_name) {
        *property_name = result->property_name;
    }

    *iterator = (afw_iterator_t *)result;

    return result;
}


AFW_DEFINE(const afw_object_type_property_type_t *)
afw_object_type_property_type_get_extended(
    const afw_object_type_t *object_type,
    const afw_utf8_t *property_name_extended,
    afw_xctx_t *xctx)
{
    const afw_object_type_t *type;
    const afw_utf8_t *meta;
    afw_utf8_t after_dot;
    const afw_object_type_property_type_t *result;
    afw_utf8_t meta_pn;
    afw_utf8_t pn;

    /** @fixme Correct? Return NULL if object_type or property_name is missing. */
    if (!object_type ||
        !property_name_extended ||
        property_name_extended->len == 0)
    {
        return NULL;
    }

    /* Return property type from object type passed or an ancestor */
    type = object_type;
    result = NULL;

    /* If the property name starts with _meta_, process accordingly. 
       @fixme not implemented yet
     */
    if (afw_utf8_starts_with_z(property_name_extended,
        AFW_OBJECT_Q_PN_META  "."))
    {
        meta = NULL;
        afw_utf8_substring_byte(&meta_pn, property_name_extended,
            strlen(AFW_OBJECT_Q_PN_META  "."), property_name_extended->len);
        if (afw_utf8_equal_utf8_z(&meta_pn, "objectId")) {
            //meta = afw_object_meta_get_object_id(instance, xctx);            
        }
        else if (afw_utf8_equal_utf8_z(&meta_pn, "objectType")) {
            //meta = afw_object_meta_get_object_type(instance, xctx);
        }
        else if (afw_utf8_equal_utf8_z(&meta_pn, "path")) {
            //meta = afw_object_meta_get_path(instance, xctx);
        }
        else if (afw_utf8_equal_utf8_z(&meta_pn, "parentPaths")) {
            //meta = afw_object_get_parent_paths(instance, xctx);
        };
         
        if (meta) {
            //result = afw_value_create_unmanaged_object(
            //    meta, xctx->p, xctx);            
        }
        return result;
    }    

    /* Check for dotted property name. */
    pn.s = property_name_extended->s;
    pn.len = property_name_extended->len;

    for (after_dot.s = property_name_extended->s,
        after_dot.len = property_name_extended->len;
        after_dot.len > 0;
        (after_dot.s)++, (after_dot.len)--)
    {
        /** @fixme Property names like "xyz." and "xyz..x get through. */
        if (*after_dot.s == '.') {
            pn.len -= after_dot.len;
            (after_dot.s)++;
            (after_dot.len)--;
            break;
        }
    }

    /* Attempt to get property and return if error or found. */
    result = afw_object_type_property_type_get(object_type, &pn, xctx);
    if (result) {

        /* If dotted name, process rest of name if it's object. */
        if (after_dot.len > 0) {
            if (result->data_type == afw_data_type_object) {   
                type = result->object_type;
                
                result = afw_object_type_property_type_get_extended(type,
                    &after_dot, xctx);                                
            }
        }
    }

    /* Return result. */
    return result;
}


/* Normalize a value based on property type. */
AFW_DEFINE(const afw_value_t *)
afw_object_type_property_type_normalize(
    const afw_object_type_property_type_t *pt,
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_boolean_t normalization_needed;
    const afw_utf8_t *embedded_object_type_id;
    const afw_data_type_t *data_type;
    const afw_object_t *object;
    const afw_value_t *result;

    result = value;
    normalization_needed = false;

    value = afw_value_evaluate(value, p, xctx);

    data_type = afw_value_get_data_type(value, xctx);
    if (pt->data_type && pt->data_type != data_type) {
        normalization_needed = true;
        data_type = pt->data_type;
    }

    /** @fixme normalize list entries. */

    if (normalization_needed) {

        if (data_type == afw_data_type_object) {
            AFW_THROW_ERROR_Z(general,
                "Normalize not supported for object",
                xctx);
        }

        result = afw_value_convert(value, data_type, true, p, xctx);
    }

    /* Make user object id set for objects. */
    if (afw_value_is_object(result)) {
        object = ((const afw_value_object_t *)result)->internal;
        embedded_object_type_id =
            afw_object_old_get_property_as_string(
                pt->property_type_object, afw_s_dataTypeParameter, xctx);
        if (embedded_object_type_id) {
            afw_object_meta_set_object_type_id(
                object, embedded_object_type_id, xctx);
        }
    }

    return result;
}


/* Normalize a value based an object type and property name. */
AFW_DEFINE(const afw_value_t *)
afw_object_type_property_normalize(
    const afw_object_type_t *object_type,
    const afw_utf8_t *property_name,
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_type_property_type_t *pt;
    const afw_value_t *result;

    result = NULL;
    pt = afw_object_type_property_type_get(object_type,
        property_name, xctx);
    if (pt) {
        result = afw_object_type_property_type_normalize(pt, value,
            p, xctx);
    }

    return result;
}
