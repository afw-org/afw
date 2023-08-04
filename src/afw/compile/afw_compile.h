// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework AFW Compile AFW expressions/json/templates
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_COMPILE_H__
#define __AFW_COMPILE_H__

#include "afw_interface.h"

/**
 * @defgroup afw_compile Compile AFW syntax
 * @ingroup afw_c_api_public
 *
 * Adaptive Framework Compile AFW syntax.
 * @{
 */

/**
 * @file afw_compile.h
 * @brief Header file for Adaptive Framework Compile.
 */

AFW_BEGIN_DECLARES

/**  @brief Struc for afw_compile_type_info_t. */
struct afw_compile_type_info_s {
    afw_compile_type_t compile_type;
    const afw_utf8_t *name;
    const afw_value_t *name_value;
    const afw_utf8_t *description;
    const afw_value_t *description_value;
    const afw_data_type_t *data_type;
    const afw_value_function_definition_t *compile_function;
};


/**  @brief Residual checking options. */
typedef enum afw_compile_residual_check_e {
    afw_compile_residual_check_none,
    afw_compile_residual_check_to_newline,
    afw_compile_residual_check_to_full
} afw_compile_residual_check_t;



/**
 * @brief Return compile type info for a pneumonic.
 * @param pneumonic of a compile type. @see AFW_COMPILE_TYPE_MAP.
 * @return info for compile type or info for error with no compile function.
 */
AFW_DECLARE(const afw_compile_type_info_t *)
afw_compile_type_get_info_by_pneumonic(
    const afw_utf8_t *pneumonic,
    const afw_xctx_t *xctx);


/**
 * @brief Return info for a afw_compile_type_t.
 * @param compile_type.
 * @return info for compile type or info for error with no compile function.
 */
AFW_DECLARE(const afw_compile_type_info_t *)
afw_compile_type_get_info(
    afw_compile_type_t compile_type,
    const afw_xctx_t *xctx);



/**
 * @brief Created a struct for sharing resources by multiple compiles.
 * @param string to compile or NULL.
 * @param p to use for result.
 * @param xctx of caller.
 * @return stuct to be passed in the shared parameter of compiles.
 *
 * The purpose of this struct is to allow multiple compiles that have a
 * p with the same lifetime to share strings, literals, etc.
 */
AFW_DECLARE(const afw_compile_shared_t *)
afw_compile_shared_create(
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Compile and evaluate a string.
 * @param string to compile and evaluate.
 * @param source_location to associate with compiled string or NULL.
 * @param compile_type Compile type.
 * @param p to use for result.
 * @param xctx of caller.
 * @return result of compiling and evaluating string.
 */
AFW_DECLARE(const afw_value_t *)
afw_compile_and_evaluate(
    const afw_utf8_t *string,
    const afw_utf8_t *source_location,
    afw_compile_type_t compile_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Convience macro to call afw_compile_and_evaluate() with compile_type.
 * @param compile_type appended to "afw_compile_type_" for option. 
 * @param string to compile and evaluate.
 * @param source_location to associate with compiled string or NULL.
 * @param p to use for result.
 * @param xctx of caller.
 * @return result of compiling and evaluating string.
 */
#define afw_compile_and_evaluate_type( \
    compile_type, string, source_location, p, xctx) \
    afw_compile_and_evaluate(string, source_location, \
        afw_compile_type_ ## compile_type, p, xctx)



/**
 * @brief Compile string to adaptive value with callback.
 * @param string to compile or NULL.
 * @param callback to get an octet or NULL.
 * @param callback_data to pass to callback function or NULL.
 * @param source_location to associate with compiled string or NULL.
 * @param compile_type Compile type.
 * @param residual_check option.
 * @param parent compiled value for contextual and shared resource or NULL.
 * @param shared struct for shared compile resources or NULL.
 * @param p to use for result or NULL.
 * @param xctx of caller.
 * @return value or NULL if afw_compile_type_parenthesized_expression and
 *    all input is whitespace or comments.
 *
 * Either shared, parent, or p must be specified.  The p used by the parser
 * is shared->p, parent->p, or p as available in that order.
 *
 * Either string or callback must be non-NULL.  If both are non-NULL, the
 * string will be processed first.
 *
 * If source_location is NULL and path is specified, the path will be used
 * as the source location.
 *
 * The _meta_.path and _meta_.parentPaths of any compiled object is always
 * used to set the corresponding values in the new object.
 */
AFW_DECLARE(const afw_value_t *)
afw_compile_to_value_with_callback(
    const afw_utf8_t *string,
    afw_utf8_octet_get_cb_t callback,
    void *callback_data,
    const afw_utf8_t *source_location,
    afw_compile_type_t compile_type,
    afw_compile_residual_check_t residual_check,
    const afw_value_compiled_value_t *parent,
    const afw_compile_shared_t *shared,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Compile string to adaptive value.
 * @param string to compile.
 * @param source_location to associate with compiled string or NULL.
 * @param compile_type Compile type.
 * @param parent compiled value for contextual and shared resource or NULL.
 * @param shared struct for shared compile resources or NULL.
 * @param p to use for result or NULL.
 * @param xctx of caller.
 * @return value
 *
 * Either shared, parent, or p must be specified.  The p used by the parser
 * is shared->p, parent->p, or p as available in that order.
 *
 * This function can be used for callbacks of type afw_utf8_to_value_t.
 *
 * If source_location is NULL and path is specified, the path will be used
 * as the source location.
 *
 * The _meta_.path and _meta_.parentPaths of any compiled object is always
 * used to set the corresponding values in the new object.
 */
#define afw_compile_to_value(string, source_location, compile_type, \
    parent, shared, p, xctx) \
    afw_compile_to_value_with_callback(string, NULL, NULL, \
        source_location, compile_type, afw_compile_residual_check_to_full, \
        parent, shared, p, xctx)



/**
 * @brief Compile expression.
 * @param string containing expression to compile.
 * @param source_location to associate with compiled string or NULL.
 * @param parent compiled value for contextual and shared resource or NULL.
 * @param shared struct for shared compile resources or NULL.
 * @param p to use for result or NULL.
 * @param xctx of caller.
 * @return value
 *
 * Either shared, parent, or p must be specified.  The p used by the parser
 * is shared->p, parent->p, or p as available in that order.
 */
#define afw_compile_expression(string, source_location, parent, shared, \
    p, xctx) \
    afw_compile_to_value_with_callback(string, NULL, NULL, \
        source_location, afw_compile_type_expression, \
        afw_compile_residual_check_to_full, \
        parent, shared, p, xctx)



/**
 * @brief Compile hybrid.
 * @param value containing hybrid to compile.
 * @param source_location to associate with compiled string or NULL.
 * @param parent compiled value for contextual and shared resource or NULL.
 * @param shared struct for shared compile resources or NULL.
 * @param p to use for result or NULL.
 * @param xctx of caller.
 * @return value
 *
 * Either shared, parent, or p must be specified.  The p used by the parser
 * is shared->p, parent->p, or p as available in that order.
 */
AFW_DECLARE(const afw_value_t *)
afw_compile_hybrid(
    const afw_value_t *value,
    const afw_utf8_t *source_location,
    const afw_value_compiled_value_t *parent,
    const afw_compile_shared_t *shared,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Compile hybrid.
 * @param string containing hybrid source to compile.
 * @param source_location to associate with compiled string or NULL.
 * @param parent compiled value for contextual and shared resource or NULL.
 * @param shared struct for shared compile resources or NULL.
 * @param p to use for result or NULL.
 * @param xctx of caller.
 * @return value
 *
 * Either shared, parent, or p must be specified.  The p used by the parser
 * is shared->p, parent->p, or p as available in that order.
 */
#define afw_compile_hybrid_source(string, source_location, parent, shared, \
    p, xctx) \
    afw_compile_to_value_with_callback(string, NULL, NULL, \
        source_location, afw_compile_type_hybrid, \
        afw_compile_residual_check_to_full, \
        parent, shared, p, xctx)



/**
 * @brief Compile template.
 * @param string containing template to compile.
 * @param source_location to associate with compiled string or NULL.
 * @param parent compiled value for contextual and shared resource or NULL.
 * @param shared struct for shared compile resources or NULL.
 * @param p to use for result or NULL.
 * @param xctx of caller.
 * @return value
 *
 * Either shared, parent, or p must be specified.  The p used by the parser
 * is shared->p, parent->p, or p as available in that order.
 */
#define afw_compile_template(string, source_location, parent, shared, \
    p, xctx) \
    afw_compile_to_value_with_callback(string, NULL, NULL, \
        source_location, afw_compile_type_template, \
        afw_compile_residual_check_to_full, \
        parent, shared, p, xctx)



/**
 * @brief Compile string to adaptive object.
 * @param string to compile.
 * @param source_location to associate with compiled string or NULL.
 * @param adaptor_id to use for created object or NULL.
 * @param object_type_id to use for created object or NULL.
 * @param object_id to use for created object or NULL.
 * @param cede_p if true, cede control of p to the created object.
 * @param p to use for result or NULL.
 * @param xctx of caller.
 * @return object instance.
 *
 * If source_location is NULL and path is specified, adaptor_id,
 * object_type_id, and object_id will be used to produce a path that will
 * be used as the source location.
 * 
 * If adaptor_id is NULL, object_type_id and object_id must be NULL. If they're
 * NULL, the object's meta will not be set.
 */
AFW_DECLARE(const afw_object_t *)
afw_compile_to_object(
    const afw_utf8_t *string,
    const afw_utf8_t *source_location,
    const afw_utf8_t * adaptor_id,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    afw_boolean_t cede_p,
    const afw_pool_t * p,
    afw_xctx_t *xctx);



/**
 * @brief Compile object's expressions, templates, and hybrids.
 * @param object to process.
 * @param source_location to associate with compiled string or NULL.
 * @param recursive indicates object should be processed recursively.
 * @param shared struct for shared compile resources or NULL.
 * @param xctx of caller.
 *
 * The object must be mutable.  The p of the object will be used.
 *
 * Embedded objects are processed as well.
 *
 * Property source_location defaults to the object path.
 *
 * " property " followed by the property name will be appended to the
 * source_location when appropriate.
 *
 * All single data type expression, template, and hybrid values in
 * object will be replaced with the their compiled value.
 *
 * The compiled value will be evaluated each time the property in the object
 * is accessed in the context of that access.
 *
 * This function is called for each conf and map object.
 */
AFW_DECLARE(void)
afw_compile_expressions_templates_and_hybrids(
    const afw_object_t *object,
    const afw_utf8_t *source_location,
    afw_boolean_t recursive,
    const afw_compile_shared_t *shared,
    afw_xctx_t *xctx);



/**
 * @brief Compile an object with all hybrid properties.
 * @param object to process.
 * @param source_location to associate with compiled string or NULL.
 * @param shared struct for shared compile resources or NULL.
 * @param p to use for result or NULL.
 * @param xctx of caller.
 * @return object instance.
 *
 * This was originally written to process a qualifier object which
 * is just an object where all properties are hybrids.  The resulting
 * object contains properties representing variables and their evaluated
 * values.  See afw_xctx_push_qualifier_object().  
 */
AFW_DECLARE(const afw_object_t *)
afw_compile_object_all_hybrid_properties(
    const afw_object_t *object,
    const afw_utf8_t *source_location,
    const afw_compile_shared_t *shared,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Split name with optional qualifier.
 * @param qualified_name (qualifier:: optional)
 * @param qualifier is preallocated place to return qualifier.
 * @param name is preallocated place to return name.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_compile_split_qualified_name(
    const afw_utf8_t *qualified_name,
    afw_utf8_t *qualifier,
    afw_utf8_t *name,
    afw_xctx_t *xctx);



/**
 * @brief Return the source location of a value.
 * @param value
 * @param p to use for result or NULL.
 * @param xctx of caller
 * @return source location of value or NULL if it can't be determined.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_compile_source_location_of_value(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_COMPILE_H__ */
