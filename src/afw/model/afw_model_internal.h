// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework model internal header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_MODEL_INTERNAL_H__
#define __AFW_MODEL_INTERNAL_H__


/** @addtogroup afw_c_api
 * @{
 */

/**
 * @addtogroup afw_model Model
 *
 * Adaptive Framework model internal header.
 *
 * @{
 */

/**
 * @file afw_model_internal.h
 * @brief Internal header for Adaptive Framework model.
 *
 * Nothing in this header should be accessed outside of core.
 */

#include "afw_interface.h"

AFW_BEGIN_DECLARES


#define AFW_MODEL_ON_MAP(XX) \
XX(_AdaptiveModelCurrentOnAddObject_) \
XX(_AdaptiveModelCurrentOnDeleteObject_) \
XX(_AdaptiveModelCurrentOnGetInitialObjectId_) \
XX(_AdaptiveModelCurrentOnGetInitialValue_) \
XX(_AdaptiveModelCurrentOnGetObject_) \
XX(_AdaptiveModelCurrentOnGetProperty_) \
XX(_AdaptiveModelCurrentOnModifyObject_) \
XX(_AdaptiveModelCurrentOnReplaceObject_) \
XX(_AdaptiveModelCurrentOnRetrieveObjects_) \
XX(_AdaptiveModelCurrentOnSetProperty_) \


typedef enum {
#define XX(id) \
    afw_model_on_map_ ## id,
    AFW_MODEL_ON_MAP(XX)
#undef XX
    afw_model_on_map_count
} afw_model_on_map_enum_t;



/**
 * @brief Internal struct used by afw_model* functions.
 *
 * Do not access outside of afw core as it may change often.
 * 
 */
struct afw_model_internal_context_s {

    /* Next 6 set in afw_model_internal_create_skeleton_context() */
    const afw_pool_t *p;
    afw_model_internal_adaptor_session_self_t *session_self;
    const afw_adaptor_impl_request_t *impl_request;
    const afw_context_cb_variable_t * const *current_variables;
    const afw_model_object_type_t *model_object_type;
    const afw_query_criteria_t *criteria;
    const afw_object_t *adaptorTypeSpecific;
    afw_runtime_object_indirect_t runtime_object_level;
    afw_runtime_object_indirect_t runtime_property_level;
    afw_boolean_t from_adaptor;
    afw_boolean_t process_initial_values;

    const afw_object_t *object;
    const afw_object_t *mapped_object;

    /* Object id from original request. */
    const afw_utf8_t *object_id;

    /*
     * Next two set in context by
     * afw_model_internal_create_to_adaptor_skeleton_context().
     */
    const afw_utf8_t *mapped_object_type_id;
    const afw_utf8_t *mapped_object_id;
 
    const afw_value_t *object_value;
    const afw_value_t *adaptor_id_value;
    const afw_value_t *object_type_id_value;
    const afw_value_t *object_id_value;

    const afw_value_t *mapped_object_value;
    const afw_value_t *mapped_object_type_id_value;
    const afw_value_t *mapped_object_id_value;

    const afw_value_t *adaptorTypeSpecific_value;  /* From adaptorTypeSpecific. */
    const afw_value_t *mapObject_value;            /* mapObject function. */
    const afw_value_t *mapBackObject_value;        /* mapBackObject function. */
    const afw_value_t *queryCriteria_value;        /* From criteria. */
    const afw_value_t *returnObject_value;         /* returnObject function. */
    const afw_value_t *useDefaultProcessing_value;


    /** @brief Original entries for modify. */
    const afw_adaptor_modify_entry_t * const *modify_entries;

    /** @brief Original entries for modify as list value. */
    const afw_value_t *modify_entries_value;

    /** @brief Mapped entries for modify. */
    const afw_list_t *mapped_entries;

    /* current:: property level. */
    struct {

        const afw_model_property_type_t *model_property_type;

        const afw_value_t *property_name_value;
        const afw_value_t *value;

        const afw_value_t *mapped_property_name_value;
        const afw_value_t *mapped_value;

    } property_level;

};



/**
 * @brief Define afw_model_internal_object_type_associative_array template.
 *
 * This defines afw_model_internal_object_type_associative_array_t and associated
 * typesafe afw_associative_array functions.
 *
 * See afw_associative_array_template.h for more information.
 */
AFW_ASSOCIATIVE_ARRAY_TEMPLATE(
    afw_model_associative_array,
    afw_model_object_type_t)



struct afw_model_internal_s {

    /** @brief Pool used for model. */
    const afw_pool_t *p;

    /** @brief Model id. */
    const afw_utf8_t *model_id;

    /** @brief The shared resources used by this model. */
    const afw_compile_shared_t *shared;

    /** @brief First loaded model. */
    const afw_model_t * AFW_ATOMIC next_model;

    /** @brief Immutable object types for this model. */
    apr_hash_t *object_types_ht;

    /** @brief Model object */
    const afw_object_t *model_object;

    /** @brief Model objectType. path */
    const afw_utf8_t *objectType_path;

    /** @brief Hash table holding afw_model_object_type_t objects */
    apr_hash_t *model_object_types;

    /** @brief Custom variables. */
    const afw_object_t *custom_variables;

};



/** @brief Struct for afw_model_object_type_t property_type_other member. */
struct afw_model_internal_object_type_s {

    /** @brief Model containing this object type. */
    const afw_model_t *model;

    /** @brief Compiled value for this object type. */
    const afw_value_compiled_value_t *compiled_value;

    /** @brief Object type path. */
    const afw_utf8_t *object_type_path;

    /** @brief Object type path value. */
    const afw_value_t *object_type_path_value;

    /** @brief Object type id. */
    const afw_utf8_t *object_type_id;

    /** @brief Object type id value. */
    const afw_value_t *object_type_id_value;

    /** @brief Property type object. */
    const afw_object_t *object_type_object;

    /** @brief Property type object value. */
    const afw_value_t *object_type_object_value;

    /** @brief NULL terminated list of parents. */
    const afw_model_object_type_t * *parents;

    /** @brief NULL terminated list of descendants. */
    const afw_model_object_type_t * *descendants;

    /** @brief descriptionPropertyName. */
    const afw_utf8_t *description_property_name;

    /** @brief descriptionPropertyName value. */
    const afw_value_t *description_property_name_value;

    /** @brief objectIdPropertyName. */
    const afw_utf8_t *object_id_property_name;

    /** @brief objectIdPropertyName value. */
    const afw_value_t *object_id_property_name_value;

    /** @brief NULL terminated list of property types for this object type. */
    const afw_model_property_type_t * *property_type;

    /** @brief Property type for other properties. */
    const afw_model_property_type_t *property_type_other;

    /** @brief Custom variables. */
    const afw_object_t *custom_variables;
    
    /** @brief onGetInitialObjectId or NULL. */
    const afw_value_t *onGetInitialObjectId;

    /** @brief mapped object type. */
    const afw_utf8_t *mapped_object_type_id;

    /** @brief mappedObjectType value. */
    const afw_value_t *mapped_object_type_id_value;

    /** @brief onAddObject or NULL. */
    const afw_value_t *onAddObject;

    /** @brief onDeleteObject or NULL. */
    const afw_value_t *onDeleteObject;

    /** @brief onGetObject or NULL. */
    const afw_value_t *onGetObject;

    /** @brief onModifyObject or NULL. */
    const afw_value_t *onModifyObject;

    /** @brief onReplaceObject or NULL. */
    const afw_value_t *onReplaceObject;

    /** @brief onRetrieveObjects or NULL. */
    const afw_value_t *onRetrieveObjects;

};


struct afw_model_internal_property_type_s {

    /** @brief Property name or regular expression if other. */
    const afw_utf8_t *property_name;

    /** @brief Property name as adaptive value. */
    const afw_value_t *property_name_value;

    /** @brief Compiled value for this property. */
    const afw_value_compiled_value_t *compiled_value;

    /** @brief Property type path. */
    const afw_utf8_t *property_type_path;

    /** @brief Property type path value. */
    const afw_value_t *property_type_path_value;

    /** @brief Property type object. */
    const afw_object_t *property_type_object;

    /** @brief Property type object value. */
    const afw_value_t *property_type_object_value;

    /** @brief Data type or NULL if not restricted. */
    const afw_data_type_t *data_type;

    /** @brief Data type to use if value is evaluated or NULL. */
    const afw_data_type_t *evaluated_data_type;

    /** @brief constraint. */
    /** @fixme*/

    /** @brief Custom variables. */
    const afw_object_t *custom_variables;

    /** @brief allowQuery. */
    afw_boolean_t allow_query;

    /** @brief allowRead. */
    afw_boolean_t allow_read;

    /** @brief allowRead. */
    afw_boolean_t allow_write;

    /** @brief required. */
    afw_boolean_t required;

    /** @brief transitory. */
    afw_boolean_t transitory;

    /** @brief unique. */
    afw_boolean_t unique;

    /** @brief defaultValue or NULL. */
    const afw_value_t *default_value;

    /** @brief Mapped property name. */
    const afw_utf8_t *mapped_property_name;

    /** @brief Mapped property name as adaptive value. */
    const afw_value_t *mapped_property_name_value;
 
    /** @brief initial value or NULL if value from object used asis. */
    const afw_value_t *onGetInitialValue;

    /** @brief onGetProperty value or NULL if value from object used asis. */
    const afw_value_t *onGetProperty;

    /** @brief onSetProperty value or NULL if value from object used asis. */
    const afw_value_t *onSetProperty;

    /*
     * @brief Extra boolean expression that can test for uniqueness.
     *
     * @fixme  Concept???  Maybe???
     *
     * This is an optional additional uniqueness boolean test expression
     * that can be used to test if a value is unique.  If not specified,
     * it up to the adaptor to determine if value is unique.
     *
     */
    const afw_value_t *uniqueness_test;
};


struct afw_model_internal_adaptor_self_s {
    afw_adaptor_t pub;

    const afw_utf8_t *model_location_adaptor_id;

    const afw_utf8_t *mapped_adaptor_id;

    const afw_utf8_t *model_id;

    /** @brief Custom variables. */
    const afw_object_t *custom_variables;

    /** @brief mappedPropertyName or NULL. */
    const afw_value_t *mapped_property_name;

    const afw_value_t *mappedAdaptorId_value;

    const afw_value_t *mapBackObject_signature;

    const afw_value_t *mapObject_signature;

    const afw_value_t *returnObject_signature;

    /* Instance skeletons for runtime on* objects */
    union {
        afw_runtime_object_indirect_t instance_skeleton[1];
        struct {
#define XX(id) \
            afw_runtime_object_indirect_t instance_skeleton_ ## id;
            AFW_MODEL_ON_MAP(XX)
#undef XX
        };
    };
};


struct afw_model_internal_adaptor_session_self_s {
    afw_adaptor_session_t pub;
    afw_model_internal_adaptor_self_t *adaptor;
    const afw_adaptor_t *model_location_adaptor;
    const afw_model_t *model;   
    afw_adaptor_object_type_cache_t object_type_cache;
};


/* Object callback context. */
typedef struct afw_model_internal_object_cb_context_s {
    const afw_pool_t *p;
    afw_model_internal_adaptor_session_self_t *session;
    const afw_adaptor_impl_request_t *impl_request;
    void * original_context;
    afw_object_cb_t original_callback;
    const afw_model_object_type_t *model_object_type;
    const afw_utf8_t *object_id;
    const afw_query_criteria_t *criteria;
} afw_model_internal_object_cb_context_t;


void
afw_model_internal_register_context_type_model(afw_xctx_t *xctx);

/*
 * This is used by add and replace to create basic mapped object.  Each
 * does further appropriated processing on returned object.  ctx must be
 * initialized before call.
 */
AFW_DECLARE_INTERNAL(const afw_object_t *)
afw_model_internal_create_basic_to_adaptor_mapped_object(
    afw_model_internal_context_t *ctx,
    afw_xctx_t *xctx);
    
AFW_DECLARE_INTERNAL(afw_model_internal_context_t *)
afw_model_internal_create_skeleton_context(
    afw_runtime_object_indirect_t *runtime_object_level_skeleton,
    const afw_context_cb_variable_t * const *current_variables,
    afw_model_internal_adaptor_session_self_t *self_session,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_model_object_type_t *model_object_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


AFW_DECLARE_INTERNAL(afw_model_internal_context_t *)
afw_model_internal_create_to_adaptor_skeleton_context(
    afw_model_internal_adaptor_session_self_t *self,
    afw_runtime_object_indirect_t *runtime_object_level_skeleton,
    const afw_model_t *model,
    const afw_adaptor_impl_request_t *impl_request,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    afw_xctx_t *xctx);


AFW_DECLARE_INTERNAL(void)
afw_model_internal_complete_ctx_default_add_object(
    afw_model_internal_context_t *ctx,
    afw_xctx_t *xctx);


AFW_DECLARE_INTERNAL(void)
afw_model_internal_complete_ctx_default_delete_object(
    afw_model_internal_context_t *ctx,
    afw_xctx_t *xctx);


AFW_DECLARE_INTERNAL(void)
afw_model_internal_complete_ctx_default_modify_object(
    afw_model_internal_context_t *ctx,
    afw_xctx_t *xctx);


AFW_DECLARE_INTERNAL(void)
afw_model_internal_complete_ctx_default_replace_object(
    afw_model_internal_context_t *ctx,
    afw_xctx_t *xctx);


AFW_DECLARE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_property_to_mapped[];


AFW_DECLARE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_add_object[];


AFW_DECLARE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_delete_object[];


AFW_DECLARE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_get_object[];


AFW_DECLARE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_modify_object[];


AFW_DECLARE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_replace_object[];


AFW_DECLARE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_retrieve_objects[];


AFW_DECLARE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_property_from_mapped[];


AFW_DECLARE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_for_initial_object_id[];

AFW_DECLARE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_runtime_ctx[];



/**
 * @internal
 * @brief Convert a property based on model object_type and adapt_type.
 * @param object_type to use.
 * @param adapt_type to use.
 * @param to_property_name is place to return property name.
 * @param to_value is place to return value.
 * @param from_property_name to map.
 * @param from_value to map.
 * @param variable_objects is NULL terminated list of objects to search for
 *    variables or NULL.
 * @param p to use for result.
 * @param xctx of caller;
 */
AFW_DECLARE_INTERNAL(void)
afw_model_internal_convert_property(
    const afw_model_object_type_t *object_type,
    afw_model_adapt_t adapt_type,
    const afw_utf8_t * *to_property_name,
    const afw_value_t * *to_value,
    const afw_utf8_t * from_property_name,
    const afw_value_t * from_value,
    const afw_object_t * const *variable_objects,
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @internal
 * @brief Convert a property name based on object_type and adapt_type.
 * @param object_type to use for mapping.
 * @param adapt_type to use.
 * @param to_property_name is place to return property name.
 * @param from_property_name to map.
 * @param variable_objects is NULL terminated list of objects to search for
 *    variables or NULL.
 * @param p to use for result.
 * @param xctx of caller;
 */
AFW_DECLARE_INTERNAL(void)
afw_model_internal_convert_property_name(
    const afw_model_object_type_t *object_type,
    afw_model_adapt_t adapt_type,
    const afw_utf8_t * *to_property_name,
    const afw_utf8_t * from_property_name,
    const afw_object_t * const *variable_objects,
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Convert query criteria based on model.
 * @param model_object_type to use.
 * @param criteria to convert.
 * @param p to use.
 * @param xctx of caller.
 * @return converted query criteria.
 */
AFW_DECLARE_INTERNAL(const afw_query_criteria_t *)
afw_model_internal_convert_query_criteria(
    const afw_model_object_type_t *model_object_type,
    const afw_query_criteria_t *criteria,
    const afw_pool_t *p, afw_xctx_t *xctx);



AFW_DECLARE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_property_from_mapped[];

AFW_DECLARE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_property_to_mapped[];

AFW_DECLARE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_for_initial_object_id[];

AFW_DECLARE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_add_object[];

AFW_DECLARE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_delete_object[];

AFW_DECLARE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_modify_object[];

AFW_DECLARE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_replace_object[];

/*
 * This is an internal runtime context that includes all of the current::
 * variable access functions.  This one is not registered.  The values
 * in afw_model_internal_context_t *ctx is used to determine if a variable
 * is present or not.
 */
AFW_DECLARE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
afw_model_internal_context_current_runtime_ctx[];


AFW_END_DECLARES

/** @} */
/** @} */

#endif /* __AFW_MODEL_INTERNAL_H__ */
