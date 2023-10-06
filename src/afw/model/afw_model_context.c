// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Model Adaptor
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_model_context.c
 * @brief Adaptive Framework model adaptor.
 */

/* Comment out next line to cause new model code to be called on modify, etc. */
#define __IMPL_DIRECT_MAPPED_CALLS_MODIFY_

#include "afw_internal.h"



/* Include mask for object level variables. */
#define IMPL_INCLUDE_MASK_OBJECT_LEVEL    1

/* Include mask for object and property level variables. */
#define IMPL_INCLUDE_MASK_PROPERTY_LEVEL  2

/* Include mask for object and property level variables. */
#define IMPL_INCLUDE_MASK_EITHER  3


/* Can be returned by on* to do default processing */
const afw_value_null_t
impl_useDefaultProcessing = {
    &afw_value_evaluated_null_inf
};


/* Value accessor afw_model_internal_get_current_adaptorId. */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_model_internal_get_current_adaptorId(
    const afw_runtime_object_map_property_t *prop,
    const void *internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = (afw_model_internal_context_t *)internal;

    if (!ctx->adaptor_id_value) {
        ctx->adaptor_id_value =
            afw_value_create_string(
                &ctx->session_self->adaptor->pub.adaptor_id,
                ctx->p, xctx);
    }

    return ctx->adaptor_id_value;
}

/* Value accessor afw_model_internal_get_current_adaptorTypeSpecific. */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_model_internal_get_current_adaptorTypeSpecific(
    const afw_runtime_object_map_property_t *prop,
    const void *internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = (afw_model_internal_context_t *)internal;

    if (!ctx->adaptorTypeSpecific_value && ctx->adaptorTypeSpecific) {
        ctx->adaptorTypeSpecific_value =
            afw_value_create_object(
                ctx->adaptorTypeSpecific, ctx->p, xctx);
    }

    return ctx->adaptorTypeSpecific_value;
}

/* Value accessor afw_model_internal_get_current_mapBackObject. */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_model_internal_get_current_mapBackObject(
    const afw_runtime_object_map_property_t *prop,
    const void *internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = (afw_model_internal_context_t *)internal;

    return ctx->mapBackObject_value;
}

/* Value accessor afw_model_internal_get_current_mapObject. */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_model_internal_get_current_mapObject(
    const afw_runtime_object_map_property_t *prop,
    const void *internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = (afw_model_internal_context_t *)internal;

    return ctx->mapObject_value;
}

/* Value accessor afw_model_internal_get_current_mappedAdaptorId. */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_model_internal_get_current_mappedAdaptorId(
    const afw_runtime_object_map_property_t *prop,
    const void *internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = (afw_model_internal_context_t *)internal;

    return ctx->session_self->adaptor->mappedAdaptorId_value;
}

/* Value accessor afw_model_internal_get_current_mappedObject. */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_model_internal_get_current_mappedObject(
    const afw_runtime_object_map_property_t *prop,
    const void *internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = (afw_model_internal_context_t *)internal;

    if (!ctx->mapped_object_value && ctx->mapped_object) {
        ctx->mapped_object_value = afw_value_create_object(
            ctx->mapped_object, ctx->p, xctx);
    }

    return ctx->mapped_object_value;
}

/* Value accessor afw_model_internal_get_current_mappedObjectId. */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_model_internal_get_current_mappedObjectId(
    const afw_runtime_object_map_property_t *prop,
    const void *internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = (afw_model_internal_context_t *)internal;

    if (!ctx->mapped_object_id_value && ctx->mapped_object) {
        ctx->mapped_object_id_value =
            afw_value_create_string(
                ctx->mapped_object->meta.id, ctx->p, xctx);
    }

    return ctx->mapped_object_id_value;
}

/* Value accessor afw_model_internal_get_current_mappedObjectType. */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_model_internal_get_current_mappedObjectType(
    const afw_runtime_object_map_property_t *prop,
    const void *internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = (afw_model_internal_context_t *)internal;

    if (!ctx->mapped_object_type_id_value && ctx->mapped_object) {
        ctx->mapped_object_type_id_value =
            afw_value_create_string(
                afw_object_meta_get_object_type_id(ctx->mapped_object, xctx),
                ctx->p, xctx);
    }

    return ctx->mapped_object_type_id_value;
}

/* Value accessor afw_model_internal_get_current_mappedPropertyName. */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_model_internal_get_current_mappedPropertyName(
    const afw_runtime_object_map_property_t *prop,
    const void *internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = (afw_model_internal_context_t *)internal;

    if (!ctx->property_level.mapped_property_name_value &&
        ctx->property_level.model_property_type)
    {
        ctx->property_level.mapped_property_name_value =
            ctx->property_level.model_property_type->mapped_property_name_value;
    }

    return ctx->property_level.mapped_property_name_value;
}

/* Value accessor afw_model_internal_get_current_mappedValue. */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_model_internal_get_current_mappedValue(
    const afw_runtime_object_map_property_t *prop,
    const void *internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = (afw_model_internal_context_t *)internal;

    return ctx->property_level.mapped_value;
}

/* Value accessor afw_model_internal_get_current_modifyEntries. */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_model_internal_get_current_modifyEntries(
    const afw_runtime_object_map_property_t *prop,
    const void *internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = (afw_model_internal_context_t *)internal;

    if (!ctx->adaptor_id_value) {
        ctx->adaptor_id_value =
            afw_value_create_string(
                &ctx->session_self->adaptor->pub.adaptor_id,
                ctx->p, xctx);
    }

    return ctx->adaptor_id_value;
}

/* Value accessor afw_model_internal_get_current_object. */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_model_internal_get_current_object(
    const afw_runtime_object_map_property_t *prop,
    const void *internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = (afw_model_internal_context_t *)internal;

    if (!ctx->object_value && ctx->object) {
        ctx->object_value = afw_value_create_object(
            ctx->object, ctx->p, xctx);
    }

    return ctx->object_value;
}

/* Value accessor afw_model_internal_get_current_objectId. */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_model_internal_get_current_objectId(
    const afw_runtime_object_map_property_t *prop,
    const void *internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = (afw_model_internal_context_t *)internal;

    if (!ctx->object_id_value && ctx->object_id) {
        ctx->object_id_value =
            afw_value_create_string(
                ctx->object_id, ctx->p, xctx);
    }

    return ctx->object_id_value;
}

/* Value accessor afw_model_internal_get_current_objectType. */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_model_internal_get_current_objectType(
    const afw_runtime_object_map_property_t *prop,
    const void *internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = (afw_model_internal_context_t *)internal;

    if (!ctx->object_type_id_value)
    {
        ctx->object_type_id_value =
            ctx->model_object_type->mapped_object_type_id_value;
    }

    return ctx->object_type_id_value;
}

/* Value accessor afw_model_internal_get_current_propertyName. */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_model_internal_get_current_propertyName(
    const afw_runtime_object_map_property_t *prop,
    const void *internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = (afw_model_internal_context_t *)internal;

    if (!ctx->property_level.property_name_value &&
        ctx->property_level.model_property_type)
    {
        ctx->property_level.property_name_value =
            ctx->property_level.model_property_type->property_name_value;
    }

    return ctx->property_level.property_name_value;
}

/* Value accessor afw_model_internal_get_current_queryCriteria. */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_model_internal_get_current_queryCriteria(
    const afw_runtime_object_map_property_t *prop,
    const void *internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = (afw_model_internal_context_t *)internal;
    const afw_object_t *query_object;

    if (!ctx->queryCriteria_value && ctx->criteria) {
        query_object = afw_query_criteria_to_AdaptiveQueryCriteria_object(
            ctx->criteria, ctx->p, xctx);
        ctx->queryCriteria_value = afw_value_create_object(query_object,
            ctx->p, xctx);
    }

    return ctx->queryCriteria_value;
}

/* Value accessor afw_model_internal_get_current_returnObject. */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_model_internal_get_current_returnObject(
    const afw_runtime_object_map_property_t *prop,
    const void *internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = (afw_model_internal_context_t *)internal;

    return ctx->returnObject_value;
}

/* Value accessor afw_model_internal_get_current_useDefaultProcessing. */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_model_internal_get_current_useDefaultProcessing(
    const afw_runtime_object_map_property_t *prop,
    const void *internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = (afw_model_internal_context_t *)internal;

    return ctx->useDefaultProcessing_value;
}

/* Value accessor afw_model_internal_get_current_value. */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_model_internal_get_current_value(
    const afw_runtime_object_map_property_t *prop,
    const void *internal,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = (afw_model_internal_context_t *)internal;

    return ctx->property_level.value;
}


static const afw_value_t *
impl_custom_variable_get_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx);


static const afw_value_t *
impl_mappedAdaptorId_from_mapped_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = entry->data;

    return ctx->session_self->adaptor->mappedAdaptorId_value;
}


static const afw_value_t *
impl_mappedObject_from_mapped_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = entry->data;

    if (!ctx->mapped_object_value && ctx->mapped_object) {
        ctx->mapped_object_value = afw_value_create_object(
            ctx->mapped_object, ctx->p, xctx);
    }

    return ctx->mapped_object_value;
}


static const afw_value_t *
impl_mappedObjectId_from_mapped_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = entry->data;

    if (!ctx->mapped_object_id_value && ctx->mapped_object) {
        ctx->mapped_object_id_value =
            afw_value_create_string(
                ctx->mapped_object->meta.id, ctx->p, xctx);
    }

    return ctx->mapped_object_id_value;
}


static const afw_value_t *
impl_mappedObjectType_from_mapped_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = entry->data;

    if (!ctx->mapped_object_type_id_value && ctx->mapped_object) {
        ctx->mapped_object_type_id_value =
            afw_value_create_string(
                afw_object_meta_get_object_type_id(ctx->mapped_object, xctx),
                ctx->p, xctx);
    }

    return ctx->mapped_object_type_id_value;
}


static const afw_value_t *
impl_mappedPropertyName_from_mapped_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = entry->data;

    if (!ctx->property_level.mapped_property_name_value &&
        ctx->property_level.model_property_type)
    {
        ctx->property_level.mapped_property_name_value =
            ctx->property_level.model_property_type->mapped_property_name_value;
    }

    return ctx->property_level.mapped_property_name_value;
}


static const afw_value_t *
impl_mappedValue_from_mapped_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = entry->data;

    return ctx->property_level.mapped_value;
}


static const afw_value_t *
impl_adaptorId_to_mapped_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = entry->data;

    if (!ctx->adaptor_id_value) {
        ctx->adaptor_id_value =
            afw_value_create_string(
                &ctx->session_self->adaptor->pub.adaptor_id,
                ctx->p, xctx);
    }

    return ctx->adaptor_id_value;
}


static const afw_value_t *
impl_modifyEntries_to_mapped_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = entry->data;

    return ctx->modify_entries_value;
}


static const afw_value_t *
impl_object_to_mapped_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = entry->data;

    if (!ctx->object_value && ctx->object) {
        ctx->object_value = afw_value_create_object(
            ctx->object, ctx->p, xctx);
    }

    return ctx->object_value;
}


static const afw_value_t *
impl_objectId_to_mapped_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = entry->data;

    if (!ctx->object_id_value && ctx->object_id) {
        ctx->object_id_value =
            afw_value_create_string(
                ctx->object_id, ctx->p, xctx);
    }

    return ctx->object_id_value;
}


static const afw_value_t *
impl_objectType_to_mapped_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = entry->data;

    if (!ctx->object_type_id_value)
    {
        ctx->object_type_id_value =
            ctx->model_object_type->mapped_object_type_id_value;
    }

    return ctx->object_type_id_value;
}


static const afw_value_t *
impl_propertyName_to_mapped_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = entry->data;

    if (!ctx->property_level.property_name_value &&
        ctx->property_level.model_property_type)
    {
        ctx->property_level.property_name_value =
            ctx->property_level.model_property_type->property_name_value;
    }

    return ctx->property_level.property_name_value;
}


static const afw_value_t *
impl_adaptorTypeSpecific_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = entry->data;

    if (!ctx->adaptorTypeSpecific_value && ctx->adaptorTypeSpecific) {
        ctx->adaptorTypeSpecific_value =
            afw_value_create_object(
                ctx->adaptorTypeSpecific, ctx->p, xctx);
    }

    return ctx->adaptorTypeSpecific_value;
}


static const afw_value_t *
impl_queryCriteria_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = entry->data;
    const afw_object_t *query_object;

    if (!ctx->queryCriteria_value && ctx->criteria) {
        query_object = afw_query_criteria_to_AdaptiveQueryCriteria_object(
            ctx->criteria, ctx->p, xctx);
        ctx->queryCriteria_value = afw_value_create_object(query_object,
            ctx->p, xctx);
    }

    return ctx->queryCriteria_value;
}


static const afw_value_t *
impl_mapBackObject_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = entry->data;

    return ctx->mapBackObject_value;
}



static const afw_value_t *
impl_returnObject_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = entry->data;

    return ctx->returnObject_value;
}


static const afw_value_t *
impl_useDefaultProcessing_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = entry->data;

    return ctx->useDefaultProcessing_value;
}


static const afw_value_t *
impl_value_to_mapped_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = entry->data;

    return ctx->property_level.value;
}



static const
afw_context_cb_variable_meta_t
impl_current_variable_meta_adaptorId =
{
    &afw_self_s_adaptorId,
    &afw_value_evaluated_string_inf,
    &afw_data_type_string_direct,
    "Adaptor Id"
};


static const
afw_context_cb_variable_meta_t
impl_current_variable_meta_adaptorTypeSpecific =
{
    &afw_self_s_adaptorTypeSpecific,
    &afw_value_evaluated_object_inf,
    &afw_data_type_object_direct,
    "Adaptor Specific"
};


static const
afw_context_cb_variable_meta_t
impl_current_variable_meta_mapBackObject =
{
    &afw_self_s_mapBackObject,
    &afw_value_evaluated_function_inf,
    &afw_data_type_function_direct,
    "Function to map back an object",
    "object (object object)",
    "object /* Mapped back object */ ( */ (\n"
    "    object mappedObject /* Object to map back. */ )\n"
};


static const
afw_context_cb_variable_meta_t
impl_current_variable_meta_modifyEntries =
{
    &afw_self_s_modifyEntries,
    &afw_value_evaluated_array_inf,
    &afw_data_type_array_direct,
    "Modify Entries",
    "of list"
};


static const
afw_context_cb_variable_meta_t
impl_current_variable_meta_object =
{
    &afw_self_s_object,
    &afw_value_evaluated_object_inf,
    &afw_data_type_object_direct,
    "Object",
    "_AdaptiveValueMeta_"
};


static const
afw_context_cb_variable_meta_t
impl_current_variable_meta_objectId =
{
    &afw_self_s_objectId,
    &afw_value_evaluated_string_inf,
    &afw_data_type_string_direct,
    "ObjectId"
};


static const
afw_context_cb_variable_meta_t
impl_current_variable_meta_objectType =
{
    &afw_self_s_objectType,
    &afw_value_evaluated_string_inf,
    &afw_data_type_string_direct,
    "ObjectType"
};


static const
afw_context_cb_variable_meta_t
impl_current_variable_meta_propertyName =
{
    &afw_self_s_propertyName,
    &afw_value_evaluated_string_inf,
    &afw_data_type_string_direct,
    "Property Name"
};


static const
afw_context_cb_variable_meta_t
impl_current_variable_meta_returnObject =
{
    &afw_self_s_returnObject,
    &afw_value_evaluated_function_inf,
    &afw_data_type_function_direct,
    "Function to call to return an object",
    "boolean (object object, boolean mapBack? /* true will mapback object */ )",
    "boolean /* true if no more objects can be returned. */ ( */ (\n"
    "    object object /* The object to return. */ )\n"
};


static const
afw_context_cb_variable_meta_t
impl_current_variable_meta_value =
{
    &afw_self_s_value,
    NULL,
    NULL,
    "Value"
};


static const
afw_context_cb_variable_meta_t
impl_current_variable_meta_mappedAdaptorId =
{
    &afw_self_s_mappedAdaptorId,
    &afw_value_evaluated_string_inf,
    &afw_data_type_string_direct,
    "Mapped Adaptor Id"
};


static const
afw_context_cb_variable_meta_t
impl_current_variable_meta_mappedObject =
{
    &afw_self_s_mappedObject,
    &afw_value_evaluated_object_inf,
    &afw_data_type_object_direct,
    "Mapped Object"
};


static const
afw_context_cb_variable_meta_t
impl_current_variable_meta_mappedObjectId =
{
    &afw_self_s_mappedObjectId,
    &afw_value_evaluated_string_inf,
    &afw_data_type_string_direct,
    "Mapped Object Id"
};


static const
afw_context_cb_variable_meta_t
impl_current_variable_meta_mappedObjectType =
{
    &afw_self_s_mappedObjectType,
    &afw_value_evaluated_string_inf,
    &afw_data_type_string_direct,
    "Mapped Object Type"
};


static const
afw_context_cb_variable_meta_t
impl_current_variable_meta_mappedPropertyName =
{
    &afw_self_s_mappedPropertyName,
    &afw_value_evaluated_string_inf,
    &afw_data_type_string_direct,
    "Mapped Property Name"
};


static const
afw_context_cb_variable_meta_t
impl_current_variable_meta_mappedValue =
{
    &afw_self_s_mappedValue,
    NULL,
    NULL,
    "Mapped Value"
};


static const
afw_context_cb_variable_meta_t
impl_current_variable_meta_queryCriteria =
{
    &afw_self_s_queryCriteria,
    &afw_value_evaluated_object_inf,
    &afw_data_type_object_direct,
    "Query Criteria",
    "_AdaptiveQueryCriteria_"
};


static const
afw_context_cb_variable_meta_t
impl_current_variable_meta_useDefaultProcessing =
{
    &afw_self_s_useDefaultProcessing,
    NULL,
    NULL,
    "Use Default Processing"
};


static const
afw_context_cb_variable_t
impl_current_variable_adaptorId = {
    &impl_current_variable_meta_adaptorId,
    "The adaptor id of the object being converted to mapped object.",
    impl_adaptorId_to_mapped_cb,
    IMPL_INCLUDE_MASK_OBJECT_LEVEL
};


static const
afw_context_cb_variable_t
impl_current_variable_adaptorTypeSpecific = {
    &impl_current_variable_meta_adaptorTypeSpecific,
    "This is the adaptorTypeSpecific parameter value.",
    impl_adaptorTypeSpecific_cb,
    IMPL_INCLUDE_MASK_OBJECT_LEVEL
};


static const
afw_context_cb_variable_t
impl_current_variable_mapBackObject = {
    &impl_current_variable_meta_mapBackObject,
    "The function to map back an object.",
    impl_mapBackObject_cb,
    IMPL_INCLUDE_MASK_OBJECT_LEVEL
};


static const
afw_context_cb_variable_t
impl_current_variable_modifyEntries = {
    &impl_current_variable_meta_modifyEntries,
    "The modify entries.",
    impl_modifyEntries_to_mapped_cb,
    IMPL_INCLUDE_MASK_OBJECT_LEVEL
};


static const
afw_context_cb_variable_t
impl_current_variable_object = {
    &impl_current_variable_meta_object,
    "This is object being converted to mapped object.",
    impl_object_to_mapped_cb,
    IMPL_INCLUDE_MASK_OBJECT_LEVEL
};


static const
afw_context_cb_variable_t
impl_current_variable_objectId = {
    &impl_current_variable_meta_objectId,
    "The object id of the object being converted to mapped object.",
    impl_objectId_to_mapped_cb,
    IMPL_INCLUDE_MASK_OBJECT_LEVEL
};


static const
afw_context_cb_variable_t
impl_current_variable_objectType = {
    &impl_current_variable_meta_objectType,
    "The object type of the object being converted to mapped object.",
    impl_objectType_to_mapped_cb,
    IMPL_INCLUDE_MASK_OBJECT_LEVEL
};


static const
afw_context_cb_variable_t
impl_current_variable_propertyName = {
    &impl_current_variable_meta_propertyName,
    "The property name in current::object of this property.",
    impl_propertyName_to_mapped_cb,
    IMPL_INCLUDE_MASK_PROPERTY_LEVEL
};


static const
afw_context_cb_variable_t
impl_current_variable_queryCriteria = {
    &impl_current_variable_meta_queryCriteria,
    "The queryCriteria object.",
    impl_queryCriteria_cb,
    IMPL_INCLUDE_MASK_OBJECT_LEVEL
};


static const
afw_context_cb_variable_t
impl_current_variable_returnObject = {
    &impl_current_variable_meta_returnObject,
    "The function to call to return an object.",
    impl_returnObject_cb,
    IMPL_INCLUDE_MASK_OBJECT_LEVEL
};


static const
afw_context_cb_variable_t
impl_current_variable_value = {
    &impl_current_variable_meta_value,
    "The value of this property from current::object.",
    impl_value_to_mapped_cb,
    IMPL_INCLUDE_MASK_PROPERTY_LEVEL
};


static const
afw_context_cb_variable_t
impl_current_variable_mappedAdaptorId = {
    &impl_current_variable_meta_mappedAdaptorId,
    "The adaptor id of the object being converted to object.",
    impl_mappedAdaptorId_from_mapped_cb,
    IMPL_INCLUDE_MASK_OBJECT_LEVEL
};


static const
afw_context_cb_variable_t
impl_current_variable_mappedObject = {
    &impl_current_variable_meta_mappedObject,
    "This is object being converted to object.",
    impl_mappedObject_from_mapped_cb,
    IMPL_INCLUDE_MASK_OBJECT_LEVEL
};


static const
afw_context_cb_variable_t
impl_current_variable_mappedObjectId = {
    &impl_current_variable_meta_mappedObjectId,
    "The object id of the object being converted to object.",
    impl_mappedObjectId_from_mapped_cb,
    IMPL_INCLUDE_MASK_OBJECT_LEVEL
};


static const
afw_context_cb_variable_t
impl_current_variable_mappedObjectType = {
    &impl_current_variable_meta_mappedObjectType,
    "The object type of the object being converted to object.",
    impl_mappedObjectType_from_mapped_cb,
    IMPL_INCLUDE_MASK_OBJECT_LEVEL
};


static const
afw_context_cb_variable_t
impl_current_variable_mappedPropertyName = {
    &impl_current_variable_meta_mappedPropertyName,
    "The property name in current::mappedObject of this property.",
    impl_mappedPropertyName_from_mapped_cb,
    IMPL_INCLUDE_MASK_PROPERTY_LEVEL
};


static const
afw_context_cb_variable_t
impl_current_variable_mappedValue = {
    &impl_current_variable_meta_mappedValue,
    "The value of this property from current::mappedObject.",
    impl_mappedValue_from_mapped_cb,
    IMPL_INCLUDE_MASK_PROPERTY_LEVEL
};


static const
afw_context_cb_variable_t
impl_current_variable_useDefaultProcessing = {
    &impl_current_variable_meta_useDefaultProcessing,
    "Return this value to cause default processing to occur.",
    impl_useDefaultProcessing_cb,
    IMPL_INCLUDE_MASK_OBJECT_LEVEL
};


AFW_DEFINE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_property_to_mapped[] = {
    &impl_current_variable_adaptorId,
    &impl_current_variable_object,
    &impl_current_variable_objectId,
    &impl_current_variable_objectType,
    &impl_current_variable_propertyName,
    &impl_current_variable_value,
    NULL
};


AFW_DEFINE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_add_object[] = {
    &impl_current_variable_adaptorId,
    &impl_current_variable_object,
    &impl_current_variable_objectId,
    &impl_current_variable_objectType,
    &impl_current_variable_useDefaultProcessing,
    NULL
};


AFW_DEFINE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_delete_object[] = {
    &impl_current_variable_adaptorId,
    &impl_current_variable_objectId,
    &impl_current_variable_objectType,
    &impl_current_variable_useDefaultProcessing,
    NULL
};


AFW_DEFINE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_get_object[] = {
    &impl_current_variable_adaptorId,
    &impl_current_variable_adaptorTypeSpecific,
    &impl_current_variable_mapBackObject,
    &impl_current_variable_objectId,
    &impl_current_variable_objectType,
    &impl_current_variable_useDefaultProcessing,
    &impl_current_variable_mappedAdaptorId,
    &impl_current_variable_mappedObjectType,
    NULL
};


AFW_DEFINE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_modify_object[] = {
    &impl_current_variable_modifyEntries,
    &impl_current_variable_adaptorId,
    &impl_current_variable_object,
    &impl_current_variable_objectId,
    &impl_current_variable_objectType,
    &impl_current_variable_useDefaultProcessing,
    NULL
};


AFW_DEFINE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_replace_object[] = {
    &impl_current_variable_adaptorId,
    &impl_current_variable_object,
    &impl_current_variable_objectId,
    &impl_current_variable_objectType,
    &impl_current_variable_useDefaultProcessing,
    NULL
};


AFW_DEFINE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_retrieve_objects[] = {
    &impl_current_variable_adaptorId,
    &impl_current_variable_adaptorTypeSpecific,
    &impl_current_variable_returnObject,
    &impl_current_variable_mapBackObject,
    &impl_current_variable_objectType,
    &impl_current_variable_queryCriteria,
    &impl_current_variable_useDefaultProcessing,
    &impl_current_variable_mappedAdaptorId,
    &impl_current_variable_mappedObjectType,
    NULL
};


AFW_DEFINE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_property_from_mapped[] = {
    &impl_current_variable_mappedAdaptorId,
    &impl_current_variable_mappedObject,
    &impl_current_variable_mappedObjectId,
    &impl_current_variable_mappedObjectType,
    &impl_current_variable_mappedPropertyName,
    &impl_current_variable_mappedValue,
    NULL
};


AFW_DEFINE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_for_initial_object_id[] = {
    &impl_current_variable_mappedAdaptorId,
    &impl_current_variable_mappedObject,
    &impl_current_variable_mappedObjectId,
    &impl_current_variable_mappedObjectType,
    &impl_current_variable_mappedPropertyName,
    &impl_current_variable_mappedValue,
    &impl_current_variable_adaptorId,
    &impl_current_variable_object,
    &impl_current_variable_objectId,
    &impl_current_variable_objectType,
    &impl_current_variable_propertyName,
    &impl_current_variable_value,
    NULL
};

AFW_DEFINE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_runtime_ctx[] = {
    &impl_current_variable_mappedAdaptorId,
    &impl_current_variable_mappedObject,
    &impl_current_variable_mappedObjectId,
    &impl_current_variable_mappedObjectType,
    &impl_current_variable_mappedPropertyName,
    &impl_current_variable_mappedValue,
    &impl_current_variable_adaptorId,
    &impl_current_variable_object,
    &impl_current_variable_objectId,
    &impl_current_variable_objectType,
    &impl_current_variable_propertyName,
    &impl_current_variable_value,
    NULL
};


void
afw_model_internal_register_context_type_model(afw_xctx_t *xctx)
{
    afw_context_type_register_cb_variables(
        &afw_self_s_modelGetObject,
        &afw_self_s_current,
        "Qualified variables available during onGetObject processing",
        &afw_model_internal_context_current_get_object[0],
        IMPL_INCLUDE_MASK_OBJECT_LEVEL, xctx);

    afw_context_type_register_cb_variables(
        &afw_self_s_modelRetrieveObjects,
        &afw_self_s_current,
        "Qualified variables available during onRetrieveObjects processing",
        &afw_model_internal_context_current_retrieve_objects[0],
        IMPL_INCLUDE_MASK_OBJECT_LEVEL, xctx);

    afw_context_type_register_cb_variables(
        &afw_self_s_modelAddObject,
        &afw_self_s_current,
        "Qualified variables available during onAddObject processing",
        &afw_model_internal_context_current_add_object[0],
        IMPL_INCLUDE_MASK_OBJECT_LEVEL, xctx);

    afw_context_type_register_cb_variables(
        &afw_self_s_modelDeleteObject,
        &afw_self_s_current,
        "Qualified variables available during onDeleteObject processing",
        &afw_model_internal_context_current_delete_object[0],
        IMPL_INCLUDE_MASK_OBJECT_LEVEL, xctx);

    afw_context_type_register_cb_variables(
        &afw_self_s_modelModifyObject,
        &afw_self_s_current,
        "Qualified variables available during onModifyObject processing",
        &afw_model_internal_context_current_modify_object[0],
        IMPL_INCLUDE_MASK_OBJECT_LEVEL, xctx);

    afw_context_type_register_cb_variables(
        &afw_self_s_modelReplaceObject,
        &afw_self_s_current,
        "Qualified variables available during onReplaceObject processing",
        &afw_model_internal_context_current_replace_object[0],
        IMPL_INCLUDE_MASK_OBJECT_LEVEL, xctx);

    afw_context_type_register_cb_variables(
        &afw_self_s_modelGetProperty,
        &afw_self_s_current,
        "Qualified variables available during model onGetProperty processing",
        &afw_model_internal_context_current_property_from_mapped[0],
        IMPL_INCLUDE_MASK_EITHER,
        xctx);

    afw_context_type_register_cb_variables(
        &afw_self_s_modelSetProperty,
        &afw_self_s_current,
        "Qualified variables available during model onSetProperty processing",
        &afw_model_internal_context_current_property_to_mapped[0],
        IMPL_INCLUDE_MASK_EITHER, xctx);

    afw_context_type_register_cb_variables(
        &afw_self_s_modelInitialObjectId,
        &afw_self_s_current,
        "Qualified variables available during onGetInitialObjectId processing",
        &afw_model_internal_context_current_for_initial_object_id[0],
        IMPL_INCLUDE_MASK_OBJECT_LEVEL, xctx);

}



static const afw_value_t *
impl_custom_variable_get_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx = entry->data;
    const afw_value_t *result;

    result = NULL;

    if (ctx->property_level.model_property_type &&
        ctx->property_level.model_property_type->custom_variables) {
        result = afw_object_get_property(
            ctx->property_level.model_property_type->custom_variables,
            name, xctx);
    }

    if (!result && ctx->model_object_type->custom_variables) {
        result = afw_object_get_property(
            ctx->model_object_type->custom_variables,
            name, xctx);
    }

    if (!result && ctx->model_object_type->model->custom_variables) {
        result = afw_object_get_property(
            ctx->model_object_type->model->custom_variables,
            name, xctx);
    }

    return result;
}



/*
 * Create a skeleton context used by many adaptor functions and push
 * qualifies.
 */
AFW_DEFINE_INTERNAL(afw_model_internal_context_t *)
afw_model_internal_create_skeleton_context(
    afw_runtime_object_indirect_t *runtime_object_level_skeleton,
    const afw_context_cb_variable_t * const* current_variables,
    afw_model_internal_adaptor_session_self_t *session_self,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_model_object_type_t *model_object_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx;

    /* Initialize ctx. */
    ctx = afw_pool_calloc_type(p, afw_model_internal_context_t, xctx);
    ctx->p = p;
    ctx->impl_request = impl_request;
    ctx->session_self = session_self;
    ctx->current_variables = current_variables;
    ctx->model_object_type = model_object_type;
    ctx->mapped_object_type_id = ctx->model_object_type->mapped_object_type_id;
    ctx->useDefaultProcessing_value =
        (const afw_value_t *)&impl_useDefaultProcessing;

    /* Push qualifiers. */
    afw_adaptor_impl_push_qualifiers(
        (const afw_adaptor_t *)session_self->adaptor, xctx);
    afw_xctx_qualifier_stack_qualifier_push(&afw_self_s_custom, NULL, true,
        impl_custom_variable_get_cb, ctx, p, xctx);

    /* Create and push current object level object. */
    afw_memory_copy(&ctx->runtime_object_level,
        runtime_object_level_skeleton);
    ctx->runtime_object_level.internal = ctx;
    ctx->runtime_object_level.pub.p = p;
    afw_xctx_qualifier_stack_qualifier_object_push(&afw_self_s_current,
        (const afw_object_t *)&ctx->runtime_object_level,
        true, p, xctx);

#ifdef _AFW_WORKING_ON___
    /* Push model custom variables. (First one push is last one checked) */
    if (ctx->model_object_type->model->custom_variables) {
        afw_xctx_qualifier_stack_qualifier_object_push(&afw_self_s_custom,
            ctx->model_object_type->model->custom_variables,
            true, p, xctx);
    }

    /* Push model object type custom variables  */
    if (ctx->model_object_type->custom_variables) {
        afw_xctx_qualifier_stack_qualifier_object_push(&afw_self_s_custom,
            ctx->model_object_type->custom_variables,
            true, p, xctx);
    }

    /* Push model property type custom variables  */
    if (ctx->property_level.model_property_type &&
        ctx->property_level.model_property_type->custom_variables)
    {
        afw_xctx_qualifier_stack_qualifier_object_push(&afw_self_s_custom,
            ctx->property_level.model_property_type->custom_variables,
            true, p, xctx);
    }
#endif

    return ctx;
}



AFW_DEFINE_INTERNAL(afw_model_internal_context_t *)
afw_model_internal_create_to_adaptor_skeleton_context(
    afw_model_internal_adaptor_session_self_t *self,
    afw_runtime_object_indirect_t *runtime_object_level_skeleton,
    const afw_model_t *model,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    afw_xctx_t *xctx)
{
    afw_model_internal_context_t *ctx;
    const afw_model_object_type_t *model_object_type;

    model_object_type = afw_model_get_object_type(model,
        object_type_id, xctx);
    if (!model_object_type) {
        AFW_THROW_ERROR_FZ(not_found, xctx,
            "Object type " AFW_UTF8_FMT_Q " not found",
            AFW_UTF8_FMT_ARG(object_type_id));
    }

    ctx = afw_model_internal_create_skeleton_context(
        runtime_object_level_skeleton,
        afw_model_internal_context_current_runtime_ctx,
        self, impl_request, model_object_type, xctx->p, xctx);
    ctx->object_id = object_id;
    ctx->mapped_object_id = object_id;

    return ctx;
}
