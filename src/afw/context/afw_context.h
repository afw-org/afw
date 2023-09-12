// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Context
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_CONTEXT_H__
#define __AFW_CONTEXT_H__

#include "afw_interface.h"

/**
 * @defgroup afw_context Context
 * @ingroup afw_c_api_public
 *
 * Adaptive Framework Context.
 *
 * @{
 */

/**
 * @file afw_context.h
 * @brief Adaptive Framework Context.
 */

AFW_BEGIN_DECLARES

/** @brief Struct used in afw_context_cb_variable_s. */
struct afw_context_cb_variable_meta_s {
    const afw_utf8_t *name;
    const afw_value_inf_t *value_inf;
    const afw_data_type_t *data_type;
    const afw_utf8_z_t *label_z;
    const afw_utf8_z_t *data_type_parameter_z;
    const afw_utf8_z_t *data_type_parameter_formatted_z;
};



/** @brief Struct passed to afw_context_type_register_cb_variables() */
struct afw_context_cb_variable_s {
    const afw_context_cb_variable_meta_t *meta;
    const afw_utf8_z_t *description_z;
    afw_xctx_get_variable_t get_cb;
    int include_mask;
};



/**
 * @brief Push NULL terminated list of variable callback structs.
 * @param qualifier_id for variables.
 * @param variables NULL terminated list.
 * @param data to pass to callbacks.  Usually ctx.
 * @param p to use.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_context_push_cb_variables(
    const afw_utf8_t *qualifier_id,
    const afw_context_cb_variable_t *const *variables,
    void *data,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create a context type object.
 * @param context_type_id.
 * @param p to use for context.
 * @param xctx of caller.
 *
 * The returned object is unmanaged and will last for the life of the pool.
 */
AFW_DECLARE(const afw_object_t *)
afw_context_type_create(
    const afw_utf8_t *context_type_id,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


    
/**
 * @brief Insure qualifier definitions object exists.
 * @param context_type_object.
 * @param xctx of caller.
 * @return object associated with variable definitions
 *
 * If there is already an object associated with the qualifier, it is
 * returned; otherwise, a new empty one is created and added to
 * qualifierDefinitions object.
 */
AFW_DECLARE(const afw_object_t *)
afw_context_type_insure_qualifier_definitions_object_exists(
    const afw_object_t *context_type_object,
    afw_xctx_t *xctx);



/**
 * @brief Insure variable definitions object exists for qualifier id.
 * @param context_type_object.
 * @param qualifier_id.
 * @param xctx of caller.
 * @return object associated with variable definitions
 *
 * If there is already an object associated with the qualifier, it is
 * returned; otherwise, a new empty one is created and added to
 * qualifierDefinitions object.
 */
AFW_DECLARE(const afw_object_t *)
afw_context_type_insure_variable_definitions_object_exists(
    const afw_object_t *context_type_object,
    const afw_utf8_t *qualifier_id,
    afw_xctx_t *xctx);



/**
 * @brief Merge qualifier definitions.
 * @param qualifier_definitions preallocated object.
 * @param definitions_to_add.
 * @param replace_duplicates true will replace variables with same name.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_context_qualifier_definitions_merge(
    const afw_object_t *qualifier_definitions,
    const afw_object_t *definitions_to_add,
    afw_boolean_t replace_duplicates,
    afw_xctx_t *xctx);



/**
 * @brief Register context type id for array of current variable callbacks.
 * @param context_type_id to register.
 * @param qualifier_id of variables.
 * @param description_z of context.
 * @param variables is a NULL terminated array of afw_context_cb_variable_t *.
 * @param include_mask logical anded with entry's include_mask determines if
 *    entry should be included.
 * @param xctx of caller.
 *
 * The returned object is unmanaged and will last for the life of the pool.
 */
AFW_DECLARE(void)
afw_context_type_register_cb_variables(
    const afw_utf8_t *context_type_id,
    const afw_utf8_t *qualifier_id,
    const afw_utf8_octet_t *description_z,
    const afw_context_cb_variable_t *const *variables,
    int include_mask,
    afw_xctx_t *xctx);



/**
 * @brief Add variable definition using 0 terminated label and description.
 * @param variable_definitions object.
 * @param variable_name
 * @param source to include in definition.  Will be "internal" if NULL.
 * @param value_inf
 * @param label_z
 * @param description_z
 * @param data_type_parameter_z or NULL
 * @param data_type_parameter_formatted_z or NULL
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_context_variable_definition_add_z(
    const afw_object_t *variable_definitions,
    const afw_utf8_t *variable_name,
    const afw_utf8_t *source,
    const afw_value_inf_t *value_inf,
    const afw_utf8_z_t *label_z,
    const afw_utf8_z_t *description_z,
    const afw_utf8_z_t *data_type_parameter_z,
    const afw_utf8_z_t *data_type_parameter_formatted_z,
    afw_xctx_t *xctx);


/**
 * @brief Add variable definitions.
 * @param variable_definitions object.
 * @param definitions_to_add.
 * @param replace_duplicates true will replace variables with same name.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_context_variable_definitions_add(
    const afw_object_t *variable_definitions,
    const afw_object_t *definitions_to_add,
    afw_boolean_t replace_duplicates,
    afw_xctx_t *xctx);



/**
 * @brief Add variable definitions based on object.
 * @param variable_definitions preallocated object.
 * @param object used as basis for new definitions.
 * @param xctx of caller.
 *
 * See afw_xctx_qualifier_stack_qualifier_push() for use pattern.
 *
 * Any object properties that have a data type whose value must be compiled
 * will be ignored, so compile the property that should be included before
 * this call. These compiled values will use the property type
 * associatedPropertyType to produce the variable definition.
 */
AFW_DECLARE(void)
afw_context_variable_definitions_add_based_on_object(
    const afw_object_t *variable_definitions,
    const afw_object_t *object,
    afw_xctx_t *xctx);



/**
 * @brief Add variable definitions based on object type id.
 * @param variable_definitions preallocated object.
 * @param object_type_id.
 * @param include_evaluated determines if variables with an evaluated data
 *    type should be included.
 * @param xctx of caller.
 *
 * See afw_xctx_qualifier_stack_qualifier_push() for use pattern.
 */
AFW_DECLARE(void)
afw_context_variable_definitions_add_based_on_object_type_id(
    const afw_object_t *variable_definitions,
    const afw_utf8_t *object_type_id,
    afw_boolean_t include_evaluated,
    afw_xctx_t *xctx);
    


/**
 * @brief Compile and add variable definitions based on
 *    _AdaptiveTemplateProperties_ object.
 * @param context_type_object.
 * @param object used as basis for new definitions.
 * @param qualifier_id.
 * @param source_location to associate with compiled string or NULL.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_context_variable_definitions_compile_and_add_based_on_object(
    const afw_object_t *context_type_object,
    const afw_object_t *object,
    const afw_utf8_t *qualifier_id,
    const afw_utf8_t *source_location,
    afw_xctx_t *xctx);


/**
 * @brief Compile and add variable definitions based on
 *    _AdaptiveTemplateProperties_ object.
 * @param context_type_object.
 * @param objects used as basis for new definitions.
 * @param source_location to associate with compiled string or NULL.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_context_variable_definitions_compile_and_add_based_on_qualifiers_object(
    const afw_object_t *context_type_object,
    const afw_object_t *objects,
    const afw_utf8_t *source_location,
    afw_xctx_t *xctx);




AFW_END_DECLARES

/** @} */

#endif /* __AFW_CONTEXT_H__ */
