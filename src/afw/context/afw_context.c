// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Context
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_context.c
 * @brief Adaptive framework context support.
 */

#include "afw_internal.h"



static const afw_value_t *
impl_current_variable_get_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    const afw_value_t *result;
    const afw_context_cb_variable_t * const *current_variables =
        (const afw_context_cb_variable_t * const *)entry->wa;
    const afw_context_cb_variable_t *const *current_variable;

    result = NULL;

    if (current_variables) for (
        current_variable = current_variables;
        *current_variable;
        current_variable++)
    {
        if (afw_utf8_equal((*current_variable)->meta->name, name)) {
            result = (*current_variable)->get_cb(entry, name, xctx);
            break;
        }
    }

    return result;
}


AFW_DEFINE(void)
afw_context_push_cb_variables(
    const afw_utf8_t *qualifier_id,
    const afw_context_cb_variable_t *const *variables,
    void *data,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_xctx_qualifier_stack_entry_t *entry;

    entry = afw_xctx_qualifier_stack_qualifier_push(qualifier_id, NULL, true,
        impl_current_variable_get_cb, data, p, xctx);
    entry->wa = (void *)variables;
}


/* Create an unmanaged context type object. */
AFW_DEFINE(const afw_object_t *)
afw_context_type_create(
    const afw_utf8_t *context_type_id,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *result;

    result = afw_object_create(p, xctx);
    afw_object_meta_set_ids(result, &afw_s_afw,
        &afw_s__AdaptiveContextType_, context_type_id, xctx);

    afw_object_set_property_as_string(result,
        &afw_s_contextTypeId, context_type_id, xctx);

    return result;
}
    


/* Insure qualifier definitions object exists. */
AFW_DEFINE(const afw_object_t *)
afw_context_type_insure_qualifier_definitions_object_exists(
    const afw_object_t *context_type_object,
    afw_xctx_t *xctx)
{
    const afw_object_t *result;

    result = afw_object_old_get_property_as_object(
        context_type_object, &afw_s_qualifierDefinitions, xctx);
    
    if (!result) {
        result = afw_object_create_embedded(
            context_type_object, &afw_s_qualifierDefinitions,
            xctx);
        afw_object_meta_set_object_type_id(result,
            &afw_s__AdaptiveQualifierDefinitions_, xctx);
    }
    
    return result;
}



/* Insure qualifier_object exists for qualifier id. */
AFW_DEFINE(const afw_object_t *)
afw_context_type_insure_variable_definitions_object_exists(
    const afw_object_t *context_type_object,
    const afw_utf8_t *qualifier_id,
    afw_xctx_t *xctx)
{
    const afw_object_t *qualifier_definitions;
    const afw_object_t *result;

    qualifier_definitions =
        afw_context_type_insure_qualifier_definitions_object_exists(
            context_type_object, xctx);
    result = afw_object_old_get_property_as_object(qualifier_definitions,
        qualifier_id, xctx);
    if (!result) {
        result = afw_object_create_embedded(
            qualifier_definitions, qualifier_id, xctx);
        afw_object_meta_set_object_type_id(result,
            &afw_s__AdaptivePropertyTypes_, xctx);
    }

    return result;
}



AFW_DEFINE(void)
afw_context_type_register_cb_variables(
    const afw_utf8_t *context_type_id,
    const afw_utf8_t *qualifier_id,
    const afw_utf8_octet_t *description_z,
    const afw_context_cb_variable_t *const *variables,
    int include_mask,
    afw_xctx_t *xctx)
{
    const afw_object_t *context_type_object;
    const afw_object_t *qualifier_definitions;
    const afw_object_t *variable_definitions;
    const afw_context_cb_variable_t *const *variable;

    context_type_object = afw_context_type_create(
        context_type_id, xctx->env->p, xctx);
    qualifier_definitions =
        afw_context_type_insure_qualifier_definitions_object_exists(
            context_type_object, xctx);
    afw_object_meta_add_parent_path(qualifier_definitions,
        &afw_s_a_context_type_application_qualifier_definitions_path,
        xctx);
    afw_object_set_property_as_string_from_utf8_z(context_type_object,
        &afw_s_description, description_z, xctx);

    variable_definitions =
        afw_context_type_insure_variable_definitions_object_exists(
            context_type_object, qualifier_id, xctx);

    for (variable = variables;
        *variable;
        variable++)
    {
        if (include_mask & ((*variable)->include_mask))
        {
            afw_context_variable_definition_add_z(
                variable_definitions,
                (*variable)->meta->name,
                &afw_s_internal,
                (*variable)->meta->value_inf,
                (*variable)->meta->label_z,
                (*variable)->description_z,
                (*variable)->meta->data_type_parameter_z,
                (*variable)->meta->data_type_parameter_formatted_z,
                xctx);
        }
    }

    afw_environment_register_context_type(context_type_id,
        context_type_object, xctx);
}



/* Add variable definition using 0 terminated label and description. */
AFW_DEFINE(void)
afw_context_variable_definition_add_z(
    const afw_object_t *variable_definitions,
    const afw_utf8_t *variable_name,
    const afw_utf8_t *source,
    const afw_value_inf_t *value_inf,
    const afw_utf8_z_t *label_z,
    const afw_utf8_z_t *description_z,
    const afw_utf8_z_t *data_type_parameter_z,
    const afw_utf8_z_t *data_type_parameter_formatted_z,
    afw_xctx_t *xctx)
{
    const afw_object_t *definition;
    afw_value_string_t *string_value;
    const afw_data_type_t *data_type;
    afw_value_t value;

    definition = afw_object_create_embedded(variable_definitions,
        variable_name, xctx);
    afw_object_meta_set_object_type_id(definition,
        &afw_s__AdaptiveValueMeta_, xctx);

    value.inf = value_inf;
    data_type = (value_inf) ? afw_value_get_data_type(&value, xctx) : NULL;
    if (data_type) {
        afw_object_set_property_as_string(definition, &afw_s_dataType,
            &data_type->data_type_id, xctx);
    }
    if (label_z) {
        string_value = afw_value_allocate_string(definition->p, xctx);
        string_value->internal.s = label_z;
        string_value->internal.len = strlen(label_z);
        afw_object_set_property(definition, &afw_s_label,
            (const afw_value_t *)string_value, xctx);
    }
    if (description_z) {
        string_value = afw_value_allocate_string(definition->p, xctx);
        string_value->internal.s = description_z;
        string_value->internal.len = strlen(description_z);
        afw_object_set_property(definition, &afw_s_description,
            (const afw_value_t *)string_value, xctx);
    }
    if (data_type_parameter_z) {
        string_value = afw_value_allocate_string(definition->p, xctx);
        string_value->internal.s = data_type_parameter_z;
        string_value->internal.len = strlen(data_type_parameter_z);
        afw_object_set_property(definition, &afw_s_dataTypeParameter,
            (const afw_value_t *)string_value, xctx);
    }
    if (data_type_parameter_formatted_z) {
        string_value = afw_value_allocate_string(definition->p, xctx);
        string_value->internal.s = data_type_parameter_formatted_z;
        string_value->internal.len = strlen(data_type_parameter_formatted_z);
        afw_object_set_property(definition, &afw_s_dataTypeParameterFormatted,
            (const afw_value_t *)string_value, xctx);
    }
}



/* Add variable definitions. */
AFW_DEFINE(void)
afw_context_variable_definitions_add(
    const afw_object_t *variable_definitions,
    const afw_object_t *definitions_to_add,
    afw_boolean_t replace_duplicates,
    afw_xctx_t *xctx)
{
    const afw_iterator_t *iterator;
    const afw_utf8_t *property_name;
    const afw_value_t *value;

    iterator = NULL;
    while ((value = afw_object_get_next_property(
        definitions_to_add, &iterator, &property_name, xctx)))
    {
        if (replace_duplicates ||
            !afw_object_has_property(variable_definitions, property_name, xctx))
        {
            afw_object_set_property(variable_definitions, property_name, value,
                xctx);
        }
    }
}


/* Merge qualifier definitions. */
AFW_DEFINE(void)
afw_context_qualifier_definitions_merge(
    const afw_object_t *qualifier_definitions,
    const afw_object_t *definitions_to_add,
    afw_boolean_t replace_duplicates,
    afw_xctx_t *xctx)
{
    const afw_iterator_t *iterator;
    const afw_utf8_t *property_name;
    const afw_value_t *value;
    const afw_object_t *variable_definitions;
    const afw_object_t *variable_definitions_to_add;

    iterator = NULL;
    while ((value = afw_object_get_next_property(
        definitions_to_add, &iterator, &property_name, xctx)))
    {
        variable_definitions =
            afw_object_old_get_property_as_object(
                qualifier_definitions, property_name, xctx);
        if (variable_definitions)
        {
            variable_definitions_to_add =
                afw_value_as_object(value, xctx);
            afw_context_variable_definitions_add(
                variable_definitions, variable_definitions_to_add,
                replace_duplicates, xctx);
        }
        else {
            afw_object_set_property(qualifier_definitions,
                property_name, value, xctx);
        }
    }
}



/* Add variable definitions based on object. */
AFW_DEFINE(void)
afw_context_variable_definitions_add_based_on_object(
    const afw_object_t *variable_definitions,
    const afw_object_t *object,
    afw_xctx_t *xctx)
{
    const afw_object_t *object_type_object;
    const afw_object_t *property_types;
    const afw_object_t *pt;
    const afw_object_t *other_properties;
    const afw_value_t *value;
    const afw_iterator_t *iterator;
    const afw_utf8_t *property_name;
    const afw_pool_t *p = variable_definitions->p;
    const afw_utf8_t *s;
    const afw_data_type_t *data_type;
    const afw_data_type_t *value_data_type;

    object_type_object = NULL;
    if (afw_object_meta_get_object_type_id(object, xctx))
    {
        object_type_object = afw_runtime_get_object(&afw_s__AdaptiveObjectType_,
            afw_object_meta_get_object_type_id(object, xctx), xctx);
    }
    if (!object_type_object) {
        AFW_THROW_ERROR_Z(general, "Object requires valid object type", xctx);
    }

    /** @fixme Should be way to get one that is already composite. */
    object_type_object = afw_object_view_create(object_type_object, NULL,
        &afw_object_options_composite, p, xctx);

    property_types = afw_object_old_get_property_as_object(
        object_type_object, &afw_s_propertyTypes, xctx);
    other_properties = afw_object_old_get_property_as_object(
        object_type_object, &afw_s_otherProperties, xctx);
    iterator = NULL;
    while ((value = afw_object_get_next_property(object,
        &iterator, &property_name, xctx)))
    {
        /* Determine property type for this property name. */
        pt = NULL;
        if (property_types) {
            pt = afw_object_old_get_property_as_object(property_types,
                property_name, xctx);
        }
        if (!pt) {
            pt = other_properties;
        }
        if (!pt) {
            continue; /** @fixme How should this be handled? */
            AFW_THROW_ERROR_FZ(general, xctx,
                "Missing property type for " AFW_UTF8_FMT_Q,
                AFW_UTF8_FMT_ARG(property_name));
        }

        value_data_type = afw_value_get_data_type(value, xctx);


        /** @fixme NOW relatedPropertyType no longer exists.  Use dataTypeParameter. */
        /*
         * If this is a compiled data type, ignore if not already compiled or
         * use relatedPropertyType for pt if it is.
         */
        s = afw_object_old_get_property_as_string(pt,
            &afw_s_dataType, xctx);
        if (s) {
            data_type = afw_environment_get_data_type(s, xctx);
            if (data_type && data_type->evaluated) {
                if (value_data_type && value_data_type->evaluated) {
                    continue;
                }
                /** @fixme NOW relatedPropertyType no longer exists.
                pt = afw_object_old_get_property_as_object(pt,
                    &afw_s_relatedPropertyType, xctx);*/
            }
        }

        /* If not a pt at this point, make one using value meta. */
        if (!pt) {
            pt = afw_object_create(p, xctx);
            if (value_data_type) {
                afw_object_set_property_as_string(pt,
                    &afw_s_dataType, &value_data_type->data_type_id, xctx);
            }
        }

        /* Add variable definition using this property name and pt. */
        afw_object_set_property_as_object(variable_definitions,
            property_name, pt, xctx);
    }
}



/* Add variable definitions based on object type id. */
AFW_DEFINE(void)
afw_context_variable_definitions_add_based_on_object_type_id(
    const afw_object_t *variable_definitions,
    const afw_utf8_t *object_type_id,
    afw_boolean_t include_evaluated,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p = variable_definitions->p;
    const afw_object_t *object_type_object;
    const afw_object_t *property_types;
    const afw_object_t *pt;
    const afw_object_t *related_pt;
    const afw_iterator_t *iterator;
    const afw_utf8_t *property_name;
    const afw_utf8_t *data_type_id;
    const afw_data_type_t *data_type = NULL;
    afw_value_string_t *source_value;


    object_type_object = afw_runtime_get_object(&afw_s__AdaptiveObjectType_,
        object_type_id, xctx);
    if (!object_type_object) {
        AFW_THROW_ERROR_Z(general, "Object type does not exist", xctx);
    }

    /** @fixme Should be way to get one that is already composite. */
    object_type_object = afw_object_view_create(object_type_object, NULL,
        &afw_object_options_composite, p, xctx);

    property_types = afw_object_old_get_property_as_object(
        object_type_object, &afw_s_propertyTypes, xctx);
    iterator = NULL;
    while ((pt = afw_object_old_get_next_property_as_object(
        property_types, &iterator, &property_name, xctx)))
    {
        /* Skip custom. */
        if (afw_utf8_equal(property_name, &afw_s_custom)) {
            continue;
        }

        /* Get data type. */
        data_type_id = afw_object_old_get_property_as_string(pt,
            &afw_s_dataType, xctx);
        if (data_type_id) {
            data_type = afw_environment_get_data_type(data_type_id, xctx);
        }

        /* If evaluated value and they should not included, skip. */
        if (data_type && data_type->evaluated) {
            if (!include_evaluated) {
                continue;
            }
        /** @fixme NOW relatedPropertyType no longer exists.  Use dataTypeParameter. */
            /* Use relatedPropertyType for property type if available or skip. */
            /** @fixme NOW relatedPropertyType no longer exists.
            related_pt = afw_object_old_get_property_as_object(
                pt, &afw_s_relatedPropertyType, xctx);*/
            related_pt = NULL;
            if (!related_pt) {
                continue;
            }
            pt = related_pt;
        }

        /* Clone pt and set source. */
        pt = afw_object_create_clone(pt, p, xctx);
        source_value = afw_value_allocate_string(p, xctx);
        source_value->internal.len = object_type_id->len;
        source_value->internal.s = afw_memory_dup(
            object_type_id->s, object_type_id->len, p, xctx);

        /* Add variable definition using this property name and pt. */
        afw_object_set_property_as_object(variable_definitions,
            property_name, pt, xctx);
    }
}



/* Compile and add variable definitions (_AdaptiveTemplateProperties_) */
AFW_DEFINE(void)
afw_context_variable_definitions_compile_and_add_based_on_object(
    const afw_object_t *context_type_object,
    const afw_object_t *object,
    const afw_utf8_t *qualifier_id,
    const afw_utf8_t *source_location,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *detail_source_location;
    const afw_object_t *variable_definitions_object;

    detail_source_location = afw_utf8_printf(object->p, xctx,
        AFW_UTF8_FMT "/" AFW_UTF8_FMT,
        AFW_UTF8_FMT_ARG(source_location),
        AFW_UTF8_FMT_ARG(qualifier_id));
    afw_compile_templates(object,
        detail_source_location, false, NULL, xctx);
    variable_definitions_object =
        afw_context_type_insure_variable_definitions_object_exists(
            context_type_object, qualifier_id, xctx);
    afw_context_variable_definitions_add_based_on_object(
        variable_definitions_object, object, xctx);
}


/* Compile and add variable definitions (_AdaptiveTemplateProperties_) */
AFW_DEFINE(void)
afw_context_variable_definitions_compile_and_add_based_on_qualifiers_object(
    const afw_object_t *context_type_object,
    const afw_object_t *objects,
    const afw_utf8_t *source_location,
    afw_xctx_t *xctx)
{
    const afw_iterator_t *iterator;
    const afw_utf8_t *qualifier_id;
    const afw_utf8_t *detail_source_location;
    const afw_object_t *object;

    iterator = NULL;
    while ((object = afw_object_old_get_next_property_as_object(
        objects, &iterator, &qualifier_id, xctx)))
    {
        detail_source_location = afw_utf8_printf(
            object->p, xctx,
            AFW_UTF8_FMT "/" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(source_location),
            AFW_UTF8_FMT_ARG(qualifier_id));
        afw_context_variable_definitions_compile_and_add_based_on_object(
            context_type_object, object,
            qualifier_id, detail_source_location, xctx);
    }
}
