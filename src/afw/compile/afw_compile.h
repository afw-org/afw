// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework AFW Compile AFW expressions/json/templates
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_COMPILE_H__
#define __AFW_COMPILE_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_compile
 * @{
 */

/**
 * @file afw_compile.h
 * @brief Header file for Adaptive Framework Compile.
 *
 * See @ref afw_compile. Main APIs are the to_value family.
 * Residual check controls how much source must be consumed.
 *
 * Compile types in AFW_COMPILE_TYPE_MAP. Parsers in afw_compile_parse_*.c .
 * EBNF in comments harvested by afwdev.
 *
 * Important: functions must be registered before compile (bootstrap).
 */

AFW_BEGIN_DECLARES

/**
 * @brief Per-compile policy (flags as defaults; #compile overrides this only).
 *
 * Snapshot of effective compile:* knobs for one compiled unit. Process flags
 * are defaults; #compile mutates this snapshot only (never process flags).
 * Type checks resolve policy via contextual->compiled_value, else flags.
 * See designs/pragma-hash-design.md and afw_compile_parse_pragma.c.
 */
struct afw_compile_policy_s {
    /** Full typeCheck (compile + runtime). compile_only wins if both set. */
    afw_boolean_t type_check;
    /** Compile-time type checking only. */
    afw_boolean_t type_check_compile_only;
    /** Require annotations when type checking is active. */
    afw_boolean_t no_implicit_any;
    /** Strict null/undefined assignability when type checking is active. */
    afw_boolean_t strict_null_checks;
    /** Prefer unoptimized built-in call evaluation. */
    afw_boolean_t no_optimize;
};

/**
 * @brief Initialize compile policy from current process/xctx flags.
 * @param policy out; fully written.
 * @param xctx of caller.
 *
 * Used at the start of each compile. #compile then mutates the policy on the
 * compiled value only — never flag_set.
 */
AFW_DECLARE(void)
afw_compile_policy_init_from_flags(
    afw_compile_policy_t *policy,
    afw_xctx_t *xctx);


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
    afw_compile_residual_check_to_full,
    afw_compile_residual_check_to_close_brace,
} afw_compile_residual_check_t;



/**
 * @brief Return compile type info for a mnemonic.
 * @param mnemonic of a compile type. @see AFW_COMPILE_TYPE_MAP.
 * @return info for compile type or info for error with no compile function.
 */
AFW_DECLARE(const afw_compile_type_info_t *)
afw_compile_type_get_info_by_mnemonic(
    const afw_utf8_t *mnemonic,
    const afw_xctx_t *xctx);


/**
 * @brief Return info for a afw_compile_type_t.
 * @param compile_type
 * @return info for compile type or info for error with no compile function.
 */
AFW_DECLARE(const afw_compile_type_info_t *)
afw_compile_type_get_info(
    afw_compile_type_t compile_type,
    const afw_xctx_t *xctx);



/**
 * @brief Create a struct for sharing resources by multiple compiles.
 * @param p to use for result.
 * @param xctx of caller.
 * @return struct to be passed in the shared parameter of compiles.
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
 * @brief Convenience macro to call afw_compile_and_evaluate() with compile_type.
 * @param compile_type appended to "afw_compile_type_" for option. 
 * @param string to compile and evaluate.
 * @param source_location to associate with compiled string or NULL.
 * @param p to use for result.
 * @param xctx of caller.
 * @return result of compiling and evaluating string.
 */
#define afw_compile_and_evaluate_type(\
    compile_type, _string, _source_location, _p, _xctx) \
    afw_compile_and_evaluate(_string, _source_location, \
        afw_compile_type_ ## compile_type, _p, _xctx)



/**
 * @brief Compile string to adaptive value with callback.
 * @param string to compile or NULL.
 * @param callback to get an octet or NULL.
 * @param callback_data to pass to callback function or NULL.
 * @param source_location to associate with compiled string or NULL.
 * @param compile_type Compile type.
 * @param residual_check option.
 * @param shared struct for shared compile resources or NULL.
 * @param p to use for result or NULL.
 * @param xctx of caller.
 * @return compiled or evaluated value.
 *
 * Either shared or p must be specified. The p used by the parser is
 * shared->p, else a child heap of p->managed_p. Compiled units (script,
 * template, test_script) are managed values (RC 1). JSON / relaxed_json
 * return evaluated data. Evaluate dest p is separate: pass the pool
 * where you want managed results of running the unit (see
 * afw_value_evaluate).
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
    const afw_compile_shared_t *shared,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Compile string to adaptive value.
 * @param string to compile.
 * @param source_location to associate with compiled string or NULL.
 * @param compile_type Compile type.
 * @param shared struct for shared compile resources or NULL.
 * @param p to use for result or NULL.
 * @param xctx of caller.
 * @return value
 *
 * Either shared or p must be specified. See
 * afw_compile_to_value_with_callback().
 *
 * This function can be used for callbacks of type afw_utf8_to_value_t.
 *
 * If source_location is NULL and path is specified, the path will be used
 * as the source location.
 *
 * The _meta_.path and _meta_.parentPaths of any compiled object is always
 * used to set the corresponding values in the new object.
 */
#define afw_compile_to_value(_string, _source_location, _compile_type, \
    shared, _p, _xctx) \
    afw_compile_to_value_with_callback(_string, NULL, NULL, \
        _source_location, _compile_type, afw_compile_residual_check_to_full, \
        shared, _p, _xctx)


/**
 * @brief Compile script.
 * @param string containing script source to compile.
 * @param source_location to associate with compiled string or NULL.
 * @param shared struct for shared compile resources or NULL.
 * @param p to use for result or NULL.
 * @param xctx of caller.
 * @return value
 *
 * Either shared or p must be specified.
 */
#define afw_compile_script_source(_string, _source_location, _shared, \
    p, _xctx) \
    afw_compile_to_value_with_callback(_string, NULL, NULL, \
        _source_location, afw_compile_type_script, \
        afw_compile_residual_check_to_full, \
        _shared, p, _xctx)



/**
 * @brief Compile script.
 * @param value containing hybrid to compile.
 * @param source_location to associate with compiled string or NULL.
 * @param shared struct for shared compile resources or NULL.
 * @param p to use for result or NULL.
 * @param xctx of caller.
 * @return value
 *
 * Either shared or p must be specified.
 */
AFW_DECLARE(const afw_value_t *)
afw_compile_script(
    const afw_value_t *value,
    const afw_utf8_t *source_location,
    const afw_compile_shared_t *shared,
    const afw_pool_t *p,
    afw_xctx_t *xctx);




/**
 * @brief Compile template.
 * @param string containing template source to compile.
 * @param source_location to associate with compiled string or NULL.
 * @param shared struct for shared compile resources or NULL.
 * @param p to use for result or NULL.
 * @param xctx of caller.
 * @return value
 *
 * Either shared or p must be specified.
 */
#define afw_compile_template_source(_string, _source_location, _shared, \
    p, _xctx) \
    afw_compile_to_value_with_callback(_string, NULL, NULL, \
        _source_location, afw_compile_type_template, \
        afw_compile_residual_check_to_full, \
        _shared, p, _xctx)



/**
 * @brief Compile template.
 * @param value containing hybrid to compile.
 * @param source_location to associate with compiled string or NULL.
 * @param shared struct for shared compile resources or NULL.
 * @param p to use for result or NULL.
 * @param xctx of caller.
 * @return value
 *
 * Either shared or p must be specified.
 */
AFW_DECLARE(const afw_value_t *)
afw_compile_template(
    const afw_value_t *value,
    const afw_utf8_t *source_location,
    const afw_compile_shared_t *shared,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Compile a JSON string to an unmanaged object.
 * @param string JSON to compile.
 * @param source_location to associate with compiled string or NULL.
 * @param adapter_id to use for created object or NULL.
 * @param object_type_id to use for created object or NULL.
 * @param object_id to use for created object or NULL.
 * @param cede_p if true, cede control of p to the created object.
 * @param p to use for result or NULL.
 * @param xctx of caller.
 * @return unmanaged object instance.
 *
 * Old JSON-compiler door: string of JSON syntax → unmanaged
 * `const afw_object_t *`. Callers (adapters, content-type raw_to_object)
 * often make a pool, put bits in it, then cede that pool so object
 * release last-releases the pool. That is intended.
 *
 * If source_location is NULL and path is specified, adapter_id,
 * object_type_id, and object_id will be used to produce a path that will
 * be used as the source location.
 *
 * If adapter_id is NULL, object_type_id and object_id must be NULL. If they're
 * NULL, the object's meta will not be set.
 */
AFW_DECLARE(const afw_object_t *)
afw_compile_json_to_object(
    const afw_utf8_t *string,
    const afw_utf8_t *source_location,
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    afw_boolean_t cede_p,
    const afw_pool_t *p,
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
 * All single data type template values in object will be replaced with
 * their compiled value.
 *
 * Used for application and authorization-handler qualifiedVariables.
 * Accessing those names via qualifier::name evaluates the compiled
 * template (#{…} already ran here; ${…} runs on that get).
 */
AFW_DECLARE(void)
afw_compile_templates(
    const afw_object_t *object,
    const afw_utf8_t *source_location,
    afw_boolean_t recursive,
    const afw_compile_shared_t *shared,
    afw_xctx_t *xctx);



/**
 * @brief Compile an object with all template properties.
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
 * values.  See afw_xctx_qualifier_stack_qualifier_object_push().  
 */
AFW_DECLARE(const afw_object_t *)
afw_compile_object_all_template_properties(
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


/**
 * @brief Parse a script Type expression from a UTF-8 string.
 * @param source Type source (e.g. FunctionSignature metadata
 *     `(...values: any) => boolean`).
 * @param source_location optional label for errors, or NULL.
 * @param p pool for the type graph (and temporary parser subpool unless
 *     cede semantics use p).
 * @param xctx of caller.
 * @return type graph on `p`, or NULL if source is empty.
 *
 * Used for Adaptive function Formals whose dataTypeParameter is a
 * FunctionSignature (issue #28). Returns NULL if source is empty or the
 * Type does not parse (caller may fall back to a leaf function type).
 */
AFW_DECLARE(const afw_value_type_t *)
afw_compile_type_from_utf8(
    const afw_utf8_t *source,
    const afw_utf8_t *source_location,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */  // end of @addtogroup @addtogroup

#endif /* __AFW_COMPILE_H__ */
