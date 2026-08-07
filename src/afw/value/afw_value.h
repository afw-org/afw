// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Header for Adaptive Values
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_VALUE_H__
#define __AFW_VALUE_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_value
 * @{
 */

/**
 * @file afw_value.h
 * @brief Public API for adaptive values (`afw_value_t`).
 *
 * See the @ref afw_value group for the full mental model.
 *
 * **Public type:** always talk about values as `const afw_value_t *`.
 * That name is a typedef; many different C structs implement values.
 * Callers must not depend on a single `struct` body — use
 * `afw_value_evaluate()` and related helpers, and data-type create APIs.
 *
 * This header declares shared helpers (`afw_value_common_s` prefix access,
 * function definition structs, evaluate macros) and is the right include
 * for most extension/command code that touches values.
 *
 * Kind-specific layouts (block, call, symbol, …) live mainly in
 * `afw_value_internal.h` and generated `afw_data_type_*_binding.h`.
 */

AFW_BEGIN_DECLARES

/** @brief Filled in by afw_value get_info method. */
struct afw_value_info_s {
    const afw_utf8_t *value_inf_id;
    const afw_compile_value_contextual_t *contextual;
    const afw_utf8_t *detail;

    /* This is the data type of the evaluated value or NULL if not known. */
    const afw_data_type_t *evaluated_data_type;

    /*
     * This is the optimized value or self. If self can be evaluated at create
     * time, this will the evaluated result. If this value references other
     * values, their optimized value will be used. If no optimization can occur,
     * this will be self.
     */
    const afw_value_t *optimized_value;

    /* Original value type or NULL. */
    const afw_value_type_t *extended_value_type;
};


/**
 * @brief Shared prefix layout used to inspect any value kind.
 *
 * Not “the” only body for `afw_value_t`. Concrete kinds embed this pattern
 * (inf/pub union first) so a value pointer can be treated as `afw_value_t *`.
 * The `internal` byte is the start of type-specific payload, not a complete
 * value by itself.
 */
struct afw_value_common_s {
    /* Value inf union with afw_value_t pub to reduce casting needed. */
    union {
        const afw_value_inf_t *inf;
        afw_value_t pub;
    };
    afw_octet_t internal; /* type specific value */
};



/** @brief Struct for adaptive function parameter. */
struct afw_value_function_parameter_s {
    afw_object_t meta;
    const afw_data_type_t *data_type;
    const afw_value_string_t *dataType;
    const afw_value_string_t *dataTypeParameter;
    const afw_value_string_t *name;
    const afw_value_string_t *brief;
    const afw_value_string_t *description;

    /*
     * This is the minimum number of values that can be specified for this
     * parameter.  If -1, the parameter can be specified exactly once.  This
     * can only have a value other than -1 on last parameter where it can
     * have a value of -1 to 127.
     */
    const afw_value_integer_t *minArgs;

    /* Indicates that parameter is optional and can be undefined (NULL). */
    const afw_value_boolean_t *optional;

    /* Indicates that parameter can be undefined (NULL) even if not optional. */
    const afw_value_boolean_t *canBeUndefined;

    /* Indicates that dataType is polymorphic. */
    const afw_value_boolean_t *polymorphicDataType;

    /* Indicates that dataTypeParameter is polymorphic. */
    const afw_value_boolean_t *polymorphicDataTypeParameter;

    /**
     * If non-NULL, dataTypeParameter was resolved at generate to this Adaptive
     * data type (typically ArrayOf element type). NULL: do not use the
     * parameter string for compile-time type projection.
     */
    const afw_data_type_t *data_type_parameter_data_type;

};



/** @brief Struct for function value. */
struct afw_value_function_definition_s {
    /** @brief Value inf union with afw_value_t pub to reduce casting needed. */
    union {
        const afw_value_inf_t *inf;
        afw_value_t pub;
    };
    
    /** @brief Object representing function. */
    const afw_object_t *object;

    /** @brief Category of function. */
    const afw_value_string_t *category;

    /** @brief Function id of function. */
    const afw_value_string_t *functionId;

    /** @brief Function id of function resource id. */
    const afw_value_string_t *functionResourceId;

    /** @brief Function id without `<dataType>`.
     * 
     *  This is only different from functionId if polymorphic.
     */
    const afw_value_string_t *untypedFunctionId;

    /** @brief Label used internally for function. */
    const afw_value_string_t *functionLabel;

    /** @brief functionLabel converted to camel case. */
    const afw_value_string_t *camelCaseFunctionLabel;

    /** @brief functionLabel prefixed with "afw" and converted to camel case. */
    const afw_value_string_t *afwCamelCaseFunctionLabel;

    /** @brief Brief description. */
    const afw_value_string_t *brief;

    /** @brief Description of function. */
    const afw_value_string_t *description;

    /** @brief Function signature. */
    const afw_value_string_t *functionSignature;

    /** @brief Function declaration. */
    const afw_value_string_t *functionDeclaration;

    /** @brief Optional operator for function. */
    const afw_value_string_t *op;

    /** @brief Function called to execute this Adaptive function.
     * 
     * This will be the same as execute_implementation or a wrapper around it.
     * For instance, this will be a wrapper function for functions that requires
     * 'execute' access which will check access then call execute_implementation
     * if allowed.
     */
    afw_function_execute_cb_t execute;

    /** @brief The execute implementation for this function. */
    afw_function_execute_cb_t execute_implementation;

    /** @brief Optional function called to check args. */
    afw_function_arg_check_t arg_check;

    /** @brief The number of required parameters. */
    const afw_value_integer_t *numberOfRequiredParameters;

    /** @brief The maximum number of required parameters or -1 is no max. */
    const afw_value_integer_t *maximumNumberOfParameters;

    /** @brief Function parameters. */
    const afw_value_function_parameter_t * const *parameters;

    /** @brief Parameters count. */
    const afw_size_t parameters_count;

    /** @brief Function returns. */
    const afw_value_function_parameter_t *returns;

    /** @brief If this is polymorphic function, these are supported data types. */
    const afw_utf8_t *polymorphicDataTypes;

    /** @brief If this is a data type method, this is the data type. */
    const afw_data_type_t *data_type;

    /** @brief If this is a data type method, this is the data type id. */
    const afw_value_string_t *dataType;

    /**
     * @brief Errors thrown or NULL.
     * 
     * This is an array of afw_utf8_z_t * key, value pairs representing an
     * array of objects. A NULL indicates the end of an object and and
     * additional NULL represents the end of the list of objects.
     * 
     * This value accessor can be used:
     * null_terminated_array_of_utf8_z_key_value_pair_objects
     */
    const afw_utf8_z_t * const *errorsThrown;

    /** @brief Side effects or NULL. */
    const afw_utf8_t *sideEffects;

    /** @brief Data type member number. */
    afw_integer_t dataTypeMethodNumber;

    /** @brief True if this is a polymorphic function. */
    const afw_value_boolean_t *polymorphic;

    /** @brief True if polymorphic execute evaluates first parameter. */
    const afw_value_boolean_t *polymorphicExecuteFunctionEvaluatesFirstParameter;

    /** @brief This function is deprecated. */
    const afw_value_boolean_t *deprecated;

    /**
     * @brief Given the same exact parameter values, this function will always
     *     return the same result an not cause side effects.
     */
    const afw_value_boolean_t *pure;

    /**
     * @brief This is only a signature with an unimplemented execute function.
     */
    const afw_value_boolean_t *signatureOnly;

    /**
     * @brief Requires 'execute' access to function.
     */
    const afw_value_boolean_t *requiresExecuteAccess;

    /**
     * @brief True if this is a script-support / statement IR function
     *     (const, let, if, …). Not a normal user-callable adaptive function;
     *     formals in metadata may not match compiler argv shape.
     */
    const afw_value_boolean_t *scriptSupport;
};


/** @brief Value assignment targe inf. */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_assignment_target_inf;



/** @brief Value block inf. */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_block_inf;



/** @brief Value call inf. */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_call_inf;



/** @brief Value call built-in function inf. */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_call_built_in_function_inf;



/** @brief Value call script function inf. */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_call_script_function_inf;



/** @brief Value call inf. */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_compiled_value_inf;



/** @brief Value function inf. */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_function_definition_inf;



/** @brief Value function thunk inf. */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_function_thunk_inf;



/** @brief Value script function inf. */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_script_function_definition_inf;


/** @brief Value closure binding inf. */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_closure_binding_inf;



/** @brief Value list expression inf. */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_list_expression_inf;



/** @brief Value object expression inf. */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_object_expression_inf;



/** @brief Value object construct inf (expression property names). */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_object_construct_inf;



/** @brief Value qualified_variable_reference inf. */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_qualified_variable_reference_inf;



/** @brief Value evaluation_reference_by_key inf. */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_reference_by_key_inf;



/** @brief Value template inf. */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_template_definition_inf;



/** @brief Value symbol_reference inf. */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_symbol_reference_inf;



/**
 * @brief Adaptive value null (typed singleton).
 *
 * This is Adaptive/JSON **null**, not C NULL and not undefined. See
 * afw_value_undefined and afw_value_is_nullish().
 */
AFW_DECLARE_CONST_DATA(afw_value_t *)
afw_value_null;



/**
 * @brief Adaptive value undefined (permanent singleton).
 *
 * For **values**, undefined may be represented by C NULL **or** a pointer to
 * this exact singleton. Prefer storing/returning this singleton when the
 * meaning is “present but undefined” (e.g. object properties, qualifier
 * get_cb). C NULL is also treated as undefined by afw_value_is_undefined() /
 * is_nullish(), but in some APIs C NULL means something else (not bound, not
 * applicable, keep walking) — see afw_xctx_get_optionally_qualified_variable()
 * and afw_xctx_get_variable_cb_t.
 *
 * Always use afw_value_is_undefined() (or is_nullish()) to test; do not rely
 * on pointer identity alone unless you know you stored the singleton.
 */
AFW_DECLARE_CONST_DATA(afw_value_t *)
afw_value_undefined;



/**
 * @brief Unique value used to represent default case for switch().
 *
 * This is a unique value used to make the default case in parameters passed
 * to the switch() runtime support function. The address of this value is
 * used for uniqueness.
 */
AFW_DECLARE_CONST_DATA(afw_value_t *)
afw_value_unique_default_case_value;



/**
 * @brief Value for boolean variable.
 * @param variable
 * @return afw_boolean_v_true or afw_boolean_v_false
 *
 * Prefer existing static const Adaptive values when the value is fixed:
 * afw_boolean_v_true / afw_boolean_v_false, afw_v_* from afw_strings.h
 * (including a_* names), afw_integer_v_zero / afw_integer_v_one, and
 * afw_value_null / afw_value_undefined in this header. Avoid
 * create_unmanaged_* only to wrap those. Prefer
 * afw_object_set_property(..., afw_v_*, ...) over set_property_as_string
 * when an afw_v_* already exists for that constant.
 */
#define afw_value_for_boolean(variable) (variable) \
    ? afw_boolean_v_true \
    : afw_boolean_v_false



/**
 * @brief Determine if value is boolean true.
 * @param value to test.
 * @return boolean result.
 *
 * Requires an evaluated boolean (cast-safe like `afw_value_is_boolean`). If
 * A_VALUE is NULL or not an evaluated boolean, the result is false.
 */
#define afw_value_is_boolean_true(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf->is_evaluated_of_data_type == afw_data_type_boolean && \
    ((const afw_value_boolean_t *)A_VALUE)->internal \
)



/**
 * @brief Determine if value is undefined or Adaptive null.
 * @param value to test.
 * @return boolean result.
 *
 * True for C NULL, afw_value_undefined, or Adaptive null. Script nullish
 * coalescing / is_nullish and type-check nullish assignability use this idea.
 * Does **not** mean “variable name is unbound” — that is an xctx/symbol
 * question (see afw_xctx_scope_symbol_exists_by_name).
 */
#define afw_value_is_nullish(A_VALUE) \
( \
    (!A_VALUE) || (A_VALUE) == afw_value_undefined || \
    (A_VALUE)->inf->is_evaluated_of_data_type == afw_data_type_null \
)



/**
 * @brief Determine if value is undefined.
 * @param value to test.
 * @return boolean result.
 *
 * True for C NULL or the afw_value_undefined singleton. Not Adaptive null.
 * Type assignability treats C NULL like undefined for nullish /
 * strictNullChecks (issue #131). Prefer the singleton when storing a present
 * undefined value so APIs that use C NULL for “not defined” stay unambiguous.
 */
#define afw_value_is_undefined(A_VALUE) \
    (!A_VALUE || (A_VALUE) == afw_value_undefined)



/**
 * @brief Determine if value is scalar.
 * @param value to test.
 * @param xctx of caller.
 * @return boolean result.
 */
AFW_DECLARE(afw_boolean_t)
afw_value_is_scalar(const afw_value_t *value, afw_xctx_t *xctx);



/**
 * @brief Macro to return value implementation id of a value
 * @param A_VALUE to test.
 * @return value inf implementation id.
 */
#define AFW_VALUE_TYPE_ID(A_VALUE) \
&(A_VALUE)->inf->rti.implementation_id



/**
 * @brief True if A_VALUE is non-NULL and already evaluated (any data type).
 * @param A_VALUE to test.
 * @return boolean result.
 *
 * True when `inf->is_evaluated_of_data_type` is non-NULL: finished data-type
 * layout (cast-safe to the matching `const afw_value_<datatype>_t *` after an
 * `afw_value_is_*` / `AFW_VALUE_IS_DATA_TYPE` check for that type). False for
 * C NULL and for unevaluated IR (calls, constructs, references, …).
 */
#define afw_value_is_defined_and_evaluated(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf->is_evaluated_of_data_type \
)



/**
 * @brief True if A_VALUE is C NULL/undefined or already evaluated.
 * @param A_VALUE to test.
 * @return boolean result.
 *
 * Same “evaluated” sense as `afw_value_is_defined_and_evaluated` (field
 * `is_evaluated_of_data_type`), or A_VALUE is missing. Not a produce-type
 * check — see `AFW_VALUE_EVALUATES_TO_DATA_TYPE`.
 */
#define afw_value_is_undefined_or_evaluated(A_VALUE) \
( \
    (!A_VALUE) || \
    (A_VALUE)->inf->is_evaluated_of_data_type \
)



/**
 * @brief Determine if value and all of its contained values are evaluated.
 * @param value to test.
 * @param xctx of caller.
 * @return boolean result.
 *
 * Recursive: the value and nested contents are finished evaluated data-type
 * layouts, not merely known produce types.
 */
AFW_DECLARE(afw_boolean_t)
afw_value_is_fully_evaluated(
    const afw_value_t *value,
    afw_xctx_t *xctx);



/**
 * Value-kind predicates (`afw_value_is_block`, `afw_value_is_call`, …) test
 * `inf` identity for IR / structural kinds — not data type. When true, it is
 * safe to cast A_VALUE to the matching `const afw_value_<kind>_t *` (e.g.
 * `afw_value_block_t`). For evaluated data types use `afw_value_is_*` /
 * `AFW_VALUE_IS_DATA_TYPE` instead.
 */

/**
 * @brief Macro to determine if value is an assignment target.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_assignment_target(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf == &afw_value_assignment_target_inf \
)

 
 
/**
 * @brief Macro to determine if value is a block.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_block(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf == &afw_value_block_inf \
)



/**
 * @brief Macro to determine if value is a call.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_call(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf == &afw_value_call_inf \
)



/**
 * @brief Macro to determine if value is a call_built_in_function.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_call_built_in_function(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf == &afw_value_call_built_in_function_inf \
)



/**
 * @brief Macro to determine if value is a call_script_function.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_call_script_function(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf == &afw_value_call_script_function_inf \
)



/**
 * @brief Macro to determine if value is a call.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_any_call(A_VALUE) \
( \
    (A_VALUE) && \
    ( \
        (A_VALUE)->inf == &afw_value_call_built_in_function_inf || \
        (A_VALUE)->inf == &afw_value_call_script_function_inf || \
        (A_VALUE)->inf == &afw_value_call_inf \
    ) \
)



/**
 * @brief Macro to determine if value is a compiled value.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_compiled_value(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf == &afw_value_compiled_value_inf \
)



/**
 * @brief Macro to determine if value is a function definition.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_function_definition(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf == &afw_value_function_definition_inf \
)



/**
 * @brief Macro to determine if value is a function thunk.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_function_thunk(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf == &afw_value_function_thunk_inf \
)



/**
 * @brief Macro to determine if value is a list expression.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_array_expression(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf == &afw_value_list_expression_inf \
)



/**
 * @brief Macro to determine if value is an object expression.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_object_expression(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf == &afw_value_object_expression_inf \
)



/**
 * @brief Macro to determine if value is an object construct
 *     (ordered entries; expression property names).
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_object_construct(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf == &afw_value_object_construct_inf \
)



/**
 * @brief Macro to determine if value is lambda definition.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_script_function_definition(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf == &afw_value_script_function_definition_inf \
)



/**
 * @brief Macro to determine if value is closure binding.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_closure_binding(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf == &afw_value_closure_binding_inf \
)



/**
 * @brief Macro to determine if value is a qualified variable reference.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_qualified_variable_reference(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf == &afw_value_qualified_variable_reference_inf \
)



/**
 * @brief Macro to determine if value is a access property by index.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_reference_by_key(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf == &afw_value_reference_by_key_inf \
)



/**
 * @brief Macro to determine if value is a template definition.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_template_definition(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf == &afw_value_template_definition_inf \
)



/**
 * @brief Macro to determine if value is a symbol reference.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_symbol_reference(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf == &afw_value_symbol_reference_inf \
)

 
/**
 * @brief Throw if A_VALUE is not value kind A_TYPE_ID.
 * @param A_VALUE value to test.
 * @param A_TYPE_ID unquoted kind id (e.g. block, call, symbol_reference).
 * @param A_SCOPE xctx or scope for the throw.
 *
 * Kind / inf-id assert (not data type). After success it is safe to cast
 * A_VALUE to `const afw_value_<A_TYPE_ID>_t *`. For evaluated data types use
 * `AFW_VALUE_ASSERT_IS_DATA_TYPE` instead.
 */
#define AFW_VALUE_ASSERT_IS(A_VALUE, A_TYPE_ID, A_SCOPE) \
if (!A_VALUE || (A_VALUE)->inf != &afw_value_ ## A_TYPE_ID ## _inf) \
    AFW_THROW_ERROR_Z(cast_error, "Expecting " #A_TYPE_ID, A_SCOPE)


/**
 * @brief Get inf->data_type without calling get_data_type().
 * @param A_VALUE value (must be non-NULL).
 * @return data_type or NULL.
 *
 * Reads the produce-type field on the value's inf when set (evaluated values
 * and some unevaluated kinds that know their result type). Not a cast-safety
 * gate: NULL does not mean "not that type after evaluate," and a non-NULL
 * result does not alone justify casting to `afw_value_<datatype>_t`. Prefer
 * `afw_value_get_data_type()` / `AFW_VALUE_EVALUATES_TO_DATA_TYPE` when the
 * method may compute type, or `AFW_VALUE_IS_DATA_TYPE` before a typed cast.
 */
#define afw_value_quick_data_type(A_VALUE) \
((A_VALUE)->inf->data_type)


/**
 * @brief True if A_VALUE is evaluated and its data type supports keyless
 *        afw_iterator (#153).
 * @param A_VALUE value to test.
 *
 * Uses inf->is_evaluated_of_data_type (cast-safe face) and
 * optional_initialize_iterator on that data type. Not a produce-type probe.
 * Soft only — does not throw.
 */
#define afw_value_has_iterator(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf->is_evaluated_of_data_type && \
    (A_VALUE)->inf->is_evaluated_of_data_type->inf && \
    (A_VALUE)->inf->is_evaluated_of_data_type->inf->optional_initialize_iterator \
)


/**
 * @brief Fixed iterator step data type for A_VALUE's produce type, or NULL.
 * @param A_VALUE value (evaluated or not).
 * @return const afw_data_type_t * or NULL.
 *
 * Quick path only: inf->data_type->iterator_return_data_type. Does not call
 * get_data_type(). NULL if produce type is unknown on the inf, or the type
 * has no fixed iterator step type. See issue #153.
 */
#define afw_value_iterator_return_data_type(A_VALUE) \
( \
    ((A_VALUE) && (A_VALUE)->inf->data_type) \
    ? (A_VALUE)->inf->data_type->iterator_return_data_type \
    : NULL \
)


/**
 * @brief Initialize a keyless afw_iterator for an evaluated value (#153).
 * @param A_VALUE evaluated value with a data type that supports iterator.
 * @param iterator caller-defined afw_iterator_t storage (opaque; host fills).
 * @param xctx of caller.
 *
 * Requires finished evaluated layout (is_evaluated_of_data_type). Throws if
 * the value is missing, not evaluated, or its type has no iterator. Soft
 * probe: afw_value_has_iterator() first.
 */
#define afw_value_initialize_iterator(A_VALUE, iterator, xctx) \
do { \
    const afw_data_type_t *_afw_it_dt = \
        (A_VALUE) ? (A_VALUE)->inf->is_evaluated_of_data_type : NULL; \
    if (!_afw_it_dt || !_afw_it_dt->inf || \
        !_afw_it_dt->inf->optional_initialize_iterator) \
    { \
        AFW_THROW_ERROR_Z(general, \
            "Value does not support iterator", (xctx)); \
    } \
    _afw_it_dt->inf->optional_initialize_iterator( \
        _afw_it_dt, AFW_VALUE_INTERNAL(A_VALUE), (iterator), (xctx)); \
} while (0)


/**
 * @brief Get quick data type id string, or "unknown".
 * @param A_VALUE value (may be NULL).
 * @return pointer to data_type_id utf8, or afw_s_unknown.
 *
 * Same field as `afw_value_quick_data_type` (`inf->data_type`). Produce-type
 * hint only — not cast-safe; see that macro.
 */
#define afw_value_get_quick_data_type_id(A_VALUE) \
(((A_VALUE) && (A_VALUE)->inf->data_type) \
? &((A_VALUE)->inf->data_type->data_type_id) \
: afw_s_unknown)



/**
 * @brief True if A_VALUE is already evaluated as data type A_DATA_TYPE.
 * @param A_VALUE value to test.
 * @param A_DATA_TYPE unquoted data type id (e.g. object, string).
 *
 * For evaluated values only. Uses `inf->is_evaluated_of_data_type`. When true,
 * it is safe to cast A_VALUE to `const afw_value_<A_DATA_TYPE>_t *`.
 *
 * If you want to know if the value will be A_DATA_TYPE when fully evaluated
 * (not necessarily cast-safe yet), use
 * `AFW_VALUE_EVALUATES_TO_DATA_TYPE(A_VALUE, A_DATA_TYPE, xctx)` instead.
 */
#define AFW_VALUE_IS_DATA_TYPE(A_VALUE,A_DATA_TYPE) \
( \
    (A_VALUE) && (A_VALUE)->inf->is_evaluated_of_data_type && \
    (A_VALUE)->inf->is_evaluated_of_data_type == afw_data_type_ ## A_DATA_TYPE \
)



/**
 * @brief True if A_VALUE is known to evaluate to data type A_DATA_TYPE.
 * @param A_VALUE value to test (may be unevaluated).
 * @param A_DATA_TYPE unquoted data type id (e.g. object, string).
 * @param xctx of caller (required by afw_value_get_data_type).
 *
 * Uses `afw_value_get_data_type()` for known produce/return type when fully
 * evaluated (evaluated values, calls with known return type such as
 * wrap_literal_object, etc.).
 *
 * This is **not** a cast-safety gate. When true, keep the pointer as
 * `const afw_value_t *` only — do **not** cast to
 * `const afw_value_<A_DATA_TYPE>_t *` until the value is evaluated (or
 * otherwise finished) and `AFW_VALUE_IS_DATA_TYPE` / `afw_value_is_*` holds.
 */
#define AFW_VALUE_EVALUATES_TO_DATA_TYPE(A_VALUE, A_DATA_TYPE, xctx) \
( \
    (A_VALUE) && \
    afw_value_get_data_type((A_VALUE), (xctx)) == \
        afw_data_type_ ## A_DATA_TYPE \
)



/**
 * @brief Throw if A_VALUE is not evaluated data type A_DATA_TYPE.
 * @param A_VALUE value to test.
 * @param A_DATA_TYPE unquoted data type id (e.g. object, string).
 * @param A_SCOPE xctx or scope for the throw.
 *
 * Asserts the `AFW_VALUE_IS_DATA_TYPE` contract: after success it is safe to
 * cast A_VALUE to `const afw_value_<A_DATA_TYPE>_t *`.
 */
#define AFW_VALUE_ASSERT_IS_DATA_TYPE(A_VALUE, A_DATA_TYPE, A_SCOPE) \
do { \
if (!AFW_VALUE_IS_DATA_TYPE(A_VALUE, A_DATA_TYPE)) \
    AFW_THROW_ERROR_Z(cast_error, "Type safe exception.", A_SCOPE); \
} while (0)



/**
 * @brief Throw if A_VALUE is not evaluated anyURI or string.
 * @param A_VALUE value to test.
 * @param A_SCOPE xctx or scope for the throw.
 *
 * Asserts cast safety to `const afw_value_anyURI_t *` or
 * `const afw_value_string_t *` (check which with `AFW_VALUE_IS_DATA_TYPE`).
 */
#define AFW_VALUE_ASSERT_IS_ANYURI_OR_STRING(A_VALUE, A_SCOPE) \
do { \
if (!AFW_VALUE_IS_DATA_TYPE(A_VALUE, anyURI) && \
    !AFW_VALUE_IS_DATA_TYPE(A_VALUE, string) ) \
    AFW_THROW_ERROR_Z(cast_error, "Type safe exception.", A_SCOPE); \
} while (0)



/**
 * @brief Test whether two values are equal.
 * @param value1
 * @param value2
 * @param xctx of caller.
 * @return boolean result of test.
 */
AFW_DECLARE(afw_boolean_t)
afw_value_equal(const afw_value_t *value1, const afw_value_t *value2,
    afw_xctx_t *xctx);



/**
 * @brief Compare two evaluated values.
 * @param value1
 * @param value2
 * @param xctx of caller.
 * @return 0 if equal, < 0 if value1 < value2, > 0 if value1 > value2.
 *
 * Values must be evaluated and the same data type or an error is thrown.
 */
AFW_DECLARE(int)
afw_value_compare(
    const afw_value_t *value1, const afw_value_t *value2,
    afw_xctx_t *xctx);



/**
 * @brief Check to see if a value contains a substring.
 * @param value to check.
 * @param substring to check.
 * @param xctx of caller.
 * @return true if value contains substring.
 *
 * Value must be an evaluated value that has a cType of afw_utf8_t.
 */
AFW_DECLARE(afw_boolean_t)
afw_value_contains(
    const afw_value_t *value, const afw_value_t *substring,
    afw_xctx_t *xctx);



/**
 * @brief Evaluate value if needed using specific pool.
 * @param value to evaluate.
 * @param p to use.
 * @param xctx of caller.
 * @return evaluated value.
 *
 * If value is undefined or there is not a optional_evaluate(), the
 * value passed is returned asis.
 */
#define afw_value_evaluate(value, p, xctx) \
(((value) && (value)->inf->optional_evaluate) \
    ? (value)->inf->optional_evaluate(value, p, xctx) \
    : value)



/**
 * @brief Get the optimized version of this value.
 * @param value to optimize.
 * @param p to use (currently unused by implementation).
 * @param xctx of caller (currently unused by implementation).
 * @return optimized version of the value.
 *
 * If there is not an optional_get_optimized method for this value, the
 * value passed is returned asis.
 */
#define afw_value_get_optimized(value, p, xctx) \
(((value) && (value)->inf->optional_evaluate) \
    ? (value)->inf->optional_get_optimized(value) \
    : value)



/**
 * @brief Macro to get const void * of the internal of a value
 * @param _VALUE_ internal must align with afw_value_common_t *.
 * @return void * of internal (caller treats as const of the right type).
 *
 * Use only on finished evaluated data-type values (after
 * `AFW_VALUE_IS_DATA_TYPE` / `afw_value_is_*`). Not valid for unevaluated IR.
 */
#define AFW_VALUE_INTERNAL(_VALUE_) \
((void *)(&((afw_value_common_t *)(_VALUE_))->internal))

 
 
/**
 * @brief True if two values have the same known produce data type.
 * @param value1 is an adaptive value.
 * @param value2 is an adaptive value.
 * @param xctx of caller.
 * @return boolean result.
 *
 * Compares `afw_value_get_data_type()` results (produce type), not cast-safe
 * evaluated layout. Either side may still be unevaluated.
 */
#define AFW_VALUE_DATA_TYPES_EQUAL(value1, value2, xctx) \
(afw_value_get_data_type(value1, xctx) == \
    afw_value_get_data_type(value2, xctx))



/**
 * @brief Throw if two values do not have the same known produce data type.
 * @param value1 is an adaptive value.
 * @param value2 is an adaptive value.
 * @param xctx of caller.
 *
 * Uses `AFW_VALUE_DATA_TYPES_EQUAL` (`get_data_type`). Does not assert
 * cast-safe evaluated layouts.
 */
#define AFW_VALUE_ASSERT_DATA_TYPES_EQUAL(value1, value2, xctx) \
if (!AFW_VALUE_DATA_TYPES_EQUAL(value1, value2, xctx)) \
{ \
    AFW_THROW_ERROR_Z(cast_error, "Type safe exception.", xctx); \
}



/**
 * @brief Compile a value.
 * @param value must be a type that can be compiled.
 * @param source_location to associate with compiled value or NULL.
 * @param p to use for result.
 * @param xctx of caller.
 * @return result of compiling the value.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_compile(
    const afw_value_t *value,
    const afw_utf8_t *source_location,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Compile a value using specified compile type.
 * @param value can be type appropriate for compile_type or string.
 * @param source_location to associate with compiled value or NULL.
 * @param compile_type (set afw_compile_type_t)
 * @param p to use for result.
 * @param xctx of caller.
 * @return result of compiling the value.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_compile_as(
    const afw_value_t *value,
    const afw_utf8_t *source_location,
    afw_compile_type_t compile_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Compile and evaluate a value.
 * @param value must be a type that can be compiled
 * @param source_location to associate with compiled string or NULL.
 * @param compile_type
 * @param p to use for result.
 * @param xctx of caller.
 * @return result of compiling and evaluating the value.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_compile_and_evaluate(
    const afw_value_t *value,
    const afw_utf8_t *source_location,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Compile and evaluate a value using specified compile type.
 * @param value can be type appropriate for compile_type or string.
 * @param source_location to associate with compiled string or NULL.
 * @param compile_type
 * @param p to use for result.
 * @param xctx of caller.
 * @return result of compiling and evaluating the value.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_compile_and_evaluate_as(
    const afw_value_t *value,
    const afw_utf8_t *source_location,
    afw_compile_type_t compile_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Evaluate a value with additional insecure context.
 * @param value to evaluate.
 * @param untrusted_qualified_variables is an object value of type _AdaptiveContextType_.
 * @param p Pool
 * @param xctx of caller.
 * @return value or undefined if there is an error.
 *
 * The untrusted_qualified_variables supplied is checked before checking the
 * xctx's context while evaluating value.  This untrusted_qualified_variables
 * is ignored for all secure evaluations that happen in the process
 * such as authorization checking.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_evaluate_with_additional_untrusted_qualified_variables(
    const afw_value_t *value,
    const afw_value_t *untrusted_qualified_variables,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Convert a value to a value/data type.
 * @param value to evaluate and convert, if needed.
 * @param to_data_type
 * @param required if false returns NULL if value is NULL.
 * @param p to hold converted value.
 * @param xctx of caller.
 * @return converted value.  If not conversion needed, value passed.
 *
 * An undefined error is thrown if value is NULL and required is true.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_convert(
    const afw_value_t *value,
    const afw_data_type_t *to_data_type,
    afw_boolean_t required,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Convert a value to a string value.
 * @param value to evaluate and convert, if needed.
 * @param allow_undefined if true returns 'undefined' if value is NULL. 
 * @param p to hold converted value.
 * @param xctx of caller.
 * @return converted value.  If not conversion needed, value passed.
 *
 * An undefined error is thrown if result will be NULL and allow_undefined is
 * false.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_convert_to_string(
    const afw_value_t *value,
    afw_boolean_t allow_undefined,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * Convert value->value to afw_utf8_z_t *.  Must be evaluated list with
 * exactly one entry or single value.  Value can not contain x'00' bytes.
 *
 * Pool p will be used for result.
 */
AFW_DECLARE(const afw_utf8_z_t *)
afw_value_as_utf8_z(const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Array or keyless-iterator sequence as an array value (#153).
 * @param value evaluated value (may be NULL).
 * @param p pool for a materialized array when needed.
 * @param xctx of caller.
 * @return value unchanged if already an array, NULL, or non-iterable;
 *         otherwise a new array value of get_next elements (utf8 code
 *         points as one-code-point strings, etc.).
 *
 * Used when a built-in formal or HOF expects an array of values but the
 * author passed a utf8-backed sequence. Does not mutate the original
 * value; materialization is a temporary array. Not a syntax change.
 *
 * **C implementers:** prefer this helper (or keyless `afw_iterator`) over
 * open-coding utf8 walks when an API expects an array of values. Call only
 * when the formal truly wants a value sequence (e.g. EVALUATE … array, HOF
 * walked list, script `array` / `T[]` / tuple). Do **not** use for XACML
 * bag rest formals that take scalar bag members typed as `array` in
 * metadata — bag-of-one is not code-point expansion.
 *
 * **Deferred (not required for beta language semantics):** an immutable
 * array *face* over utf8 (lazy get_next / get_by_index without eager
 * materialize). Same public contract; optional later if cost or clear
 * mutation-reject matters. See `designs/utf8-code-point-sequences.md`.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_as_array_sequence(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Return value from one entry list or single value.
 * @param value list or single value.
 * @param p  Pool for result.
 * @param xctx of caller.
 * @return value or undefined.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_one_and_only(
    const afw_value_t *value, const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Return result of afw_value_one_and_only() as utf8.
 * @param value list or single value.
 * @param p  Pool for result.
 * @param xctx of caller.
 * @return string representation of value or undefined.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_value_one_and_only_as_utf8(
    const afw_value_t *value, const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * Convert value->value to afw_utf8_t *.  Must be an evaluated value or
 * list with exactly one entry
 *
 * Pool p will be used for result.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_value_as_utf8(
    const afw_value_t *value, const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Convert value to casted utf8 in specified pool.
 * @param value to convert.
 * @param p is pool to use for result.
 * @param xctx of caller.
 * 
 * Convert value to a utf8 string that is of the form:
 * 
 * `<datatype>("<string>")`
 *
 * Where `<datatype>` is the data type of value and `<string>` is the
 * quoted string representation of the value.
 *
 * For list values the result if "list(" followed by comma separated
 * result of recursively calling this function, followed by ")".
 */
AFW_DECLARE(const afw_utf8_t *)
afw_value_as_casted_utf8(
    const afw_value_t *value, const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Clone a value to specified pool.
 * @param value to clone.
 * @param p pool used for clone.
 * @param xctx of caller.
 * @return cloned value.
 *
 * Non-evaluated values will be evaluated as part of clone.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_clone(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Isolate a default value for script (issue #110 / #17).
 * @param value evaluated default (may be NULL / undefined).
 * @param p pool for any new face or clone.
 * @param xctx of caller.
 * @return value safe for the caller to mutate without sharing the base.
 *
 * Object/array: memory face (`create_wrapper_*`) over the default instance
 * when not already a face. Other types: `afw_value_clone`. Used by
 * `property_get` / `variable_get` when returning a default.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_isolate_mutable_default(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create assignment target value.
 * @param contextual information for assignment target.
 * @param assignment_target (see afw_compile_internal.h).
 * @param p pool used for value.
 * @param xctx of caller.
 * @return Created afw_value_t.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_assignment_target_create(
    const afw_compile_value_contextual_t *contextual,
    const afw_compile_assignment_target_t *assignment_target,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create and link a new block.
 * @param compiled_value to link block in.
 * @param block_count address to increment.
 * @param p pool used for value.
 * @param xctx of caller.
 * @return Created afw_value_t.
 */
AFW_DECLARE(const afw_value_block_t *)
afw_value_block_allocated_and_link(
    const afw_compile_value_contextual_t *contextual,
    afw_value_compiled_value_t *compiled_value,
    afw_size_t *block_count,
    const afw_pool_t *p,
    afw_xctx_t *xctx);




/**
 * @brief Create and link a new block.
 * @param block value to set statements in.
 * @param statement_count is number of statements.
 * @param statements is array of statements.
 * @param xctx of caller.
 * @return Finalized block value.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_block_finalize(
    const afw_value_block_t *block,
    afw_size_t statement_count,
    const afw_value_t * const *statements,
    afw_xctx_t *xctx);




/**
 * @brief Allocate an afw_value_common_t for an evaluated data type value.
 * @param data_type of value.
 * @param p pool used for value.
 * @param xctx of caller.
 * @return Allocated afw_value_common_t.
 */
AFW_DECLARE(afw_value_common_t *)
afw_value_common_allocate(
    const afw_data_type_t *data_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create an evaluated data type value.
 * @param value to copy.
 * @param data_type of value.
 * @param p pool used for value.
 * @param xctx of caller.
 * @return Created afw_value_t.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_common_create(
    const void * value,
    const afw_data_type_t *data_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create a closure binding value.
 * @param script_function_definition script function to enclose.
 * @param enclosing_lexical_scope for closure binding.
 * @param xctx of caller.
 * @return Created afw_value_t.
 */
AFW_DEFINE(const afw_value_t *)
afw_value_closure_binding_create(
    const afw_value_script_function_definition_t *script_function_definition,
    const afw_xctx_scope_t *enclosing_lexical_scope,
    afw_xctx_t *xctx);



/**
 * @brief Create a call value.
 * @param contextual information for function call.
 * @param argc number of arguments (does not include argv[0]).
 * @param argv list of argument value pointers. argv[0] must be function value.
 * @param allow_optimize if true, optimize call if possible.
 * @param p pool used for value.
 * @param xctx of caller.
 * @return Created afw_value_t.
 *
 * Make sure to set allow_optimize to false if you are changing arguments values
 * after the call to this function. For example, the higher order list functions
 * do this. If the arguments will not be changed, allow_optimize can be true.
 * 
 * The value can be a lambda definition (afw_value_script_function_definition_t *),
 * built-in function definition (afw_value_function_definition_t *) or
 * function thunk (afw_value_function_thunk_t *)
 */
AFW_DECLARE(const afw_value_t *)
afw_value_call_create(
    const afw_compile_value_contextual_t *contextual,
    afw_size_t argc,
    const afw_value_t * const *argv,
    const afw_boolean_t allow_optimize,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create a call_built_in_function value.
 * @param contextual information for function call.
 * @param argc number of arguments (does not include argv[0]).
 * @param argv list of argument value pointers. argv[0] must be function
 *        definition value.
 * @param allow_optimize if true, optimize call if possible.
 * @param p pool used for value.
 * @param xctx of caller.
 * @return Created afw_value_t.
 *
 * Make sure to set allow_optimize to false if you are changing arguments values
 * after the call to this function. For example, the higher order list functions
 * do this. If the arguments will not be changed, allow_optimize can be true.
 * 
 * Call this function instead of afw_value_call_create() when it's know that
 * argv[0] is a function definition to save a small amount of evaluation time.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_call_built_in_function_create(
    const afw_compile_value_contextual_t *contextual,
    afw_size_t argc,
    const afw_value_t * const *argv,
    const afw_boolean_t allow_optimize,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create a call_script_function value.
 * @param contextual information for function call.
 * @param script_function_definition script function to call.
 * @param enclosing_lexical_scope for closure binding or NULL if not enclosed.
 * @param argc number of arguments (does not include argv[0]).
 * @param argv list of argument value pointers. argv[0] must be function
 *        definition value.
 * @param allow_optimize if true, optimize call if possible.
 * @param p pool used for value.
 * @param xctx of caller.
 * @return Created afw_value_t.
 *
 * Make sure to set allow_optimize to false if you are changing arguments values
 * after the call to this function. For example, the higher order list functions
 * do this. If the arguments will not be changed, allow_optimize can be true.
 * 
 * Call this function instead of afw_value_call_create() when it's know that
 * argv[0] is a function definition to save a small amount of evaluation time.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_call_script_function_create(
    const afw_compile_value_contextual_t *contextual,
    const afw_value_script_function_definition_t *script_function_definition,
    const afw_xctx_scope_t *enclosing_lexical_scope,
    afw_size_t argc,
    const afw_value_t * const *argv,
    const afw_boolean_t allow_optimize,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create a call_test_script value.
 * @param contextual information for test script call.
 * @param test_script testScriptObject object<_AdaptiveTestScriptResult>.
 * @param p pool used for value.
 * @param xctx of caller.
 * @return Created afw_value_t.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_call_test_script_create(
    const afw_compile_value_contextual_t *contextual,
    const afw_object_t *test_script,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create a function thunk value.
 * @param name to be associated with thunk.
 * @param like_function_value for thunk.
 * @param execute is function to call.
 * @param ctx to pass to execute.
 * @param source_z where thunk created.
 * @param p pool used for value.
 * @param xctx of caller.
 * @return Created afw_value_t.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_function_thunk_create_impl(
    const afw_utf8_t *name,
    const afw_value_t *like_function_value,
    afw_function_thunk_execute_t execute,
    void *ctx,
    const afw_utf8_z_t *source_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create a function thunk value.
 * @param name to be associated with thunk.
 * @param like_function_value for thunk.
 * @param execute is function to call.
 * @param ctx to pass to execute.
 * @param p pool used for value.
 * @param xctx of caller.
 * @return Created afw_value_t.
 */
#define afw_value_function_thunk_create( \
    name, like_function_value, execute, ctx, p, xctx) \
afw_value_function_thunk_create_impl( \
    name, like_function_value, execute, ctx, AFW__FILE_LINE__, p, xctx)



/**
 * @brief Create a script function (lambda) definition value.
 * @param contextual information for lambda.
 * @param depth is static depth of function.
 * @param signature or NULL.
 * @param returns struct for lambda function.
 * @param count number of parameters.
 * @param parameters
 * @param body of lambda function (expression).
 * @param p pool used for value.
 * @param xctx of caller.
 * @return Created afw_value_t.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_script_function_definition_create(
    const afw_compile_value_contextual_t *contextual,
    afw_size_t depth,
    const afw_value_script_function_signature_t *signature,
    const afw_value_type_t *returns,
    afw_size_t count,
    const afw_value_script_function_parameter_t **parameters,
    const afw_value_t *body,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create an array (list) expression value.
 * @param contextual information for the expression.
 * @param internal value that will evaluate to a list or NULL.
 * @param p pool used for value.
 * @param xctx of caller.
 * @return Created afw_value_t.
 */
AFW_DEFINE(const afw_value_t *)
afw_value_create_array_expression(
    const afw_compile_value_contextual_t *contextual,
    const afw_value_t *internal,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Create an object expression value.
 * @param contextual information for the expression.
 * @param internal object.
 * @param p pool used for value.
 * @param xctx of caller.
 * @return Created afw_value_t.
 */
AFW_DEFINE(const afw_value_t *)
afw_value_create_object_expression(
    const afw_compile_value_contextual_t *contextual,
    const afw_object_t *internal,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Create an object construct value (expression property names).
 * @param contextual information for the expression.
 * @param entries head of ordered entry list (pool-owned).
 * @param meta optional meta object from literal `_meta_` or NULL.
 * @param p pool used for value.
 * @param xctx of caller.
 * @return Created afw_value_t.
 */
AFW_DEFINE(const afw_value_t *)
afw_value_create_object_construct(
    const afw_compile_value_contextual_t *contextual,
    const afw_value_object_construct_entry_t *entries,
    const afw_object_t *meta,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Create a qualified variable reference value.
 * @param contextual information for variable.
 * @param qualifier of variable or NULL.
 * @param name of variable.
 * @param p pool used for value.
 * @param xctx of caller.
 * @return Created afw_value_t.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_qualified_variable_reference_create(
    const afw_compile_value_contextual_t *contextual,
    const afw_utf8_t *qualifier,
    const afw_utf8_t *name,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create a reference_by_key value.
 * @param contextual information for property by index or NULL.
 * @param aggregate_value must be an evaluated object or list value.
 * @param key used to access value in aggregate_value.
 * @param p pool used for value.
 * @param xctx of caller.
 * @return Created afw_value_t.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_reference_by_key_create(
    const afw_compile_value_contextual_t *contextual,
    const afw_value_t *aggregate_value,
    const afw_value_t *key,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Create a template definition value.
 * @param contextual information for template.
 * @param count of values.
 * @param values list of value pointers.
 * @param p pool used for value.
 * @param xctx of caller.
 * @return Created afw_value_t.
 *
 * When an template is evaluated, the list of values is evaluated
 * and concatenated.  If there is only one value in list, the data type
 * of the result is the data type of the value.  If multiple values are
 * supplied, the result is a concatenation of the string values of all
 * the values with a data type is string.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_template_definition_create(
    const afw_compile_value_contextual_t *contextual,
    afw_size_t count,
    const afw_value_t * const *values,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create a symbol reference value.
 * @param contextual information for variable.
 * @param symbol for variable.
 * @param p pool used for value.
 * @param xctx of caller.
 * @return Created afw_value_t.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_symbol_reference_create(
    const afw_compile_value_contextual_t *contextual,
    const afw_value_block_symbol_t *symbol,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create an Adaptive value from untrusted external octets.
 * @param s pointer to octets (may be NULL).
 * @param len number of bytes, or AFW_UTF8_Z_LEN if s is NUL-terminated.
 * @param p pool for the value and any owned payload.
 * @param xctx of caller.
 * @return string value if s is valid UTF-8 (NFC via afw_utf8_*);
 *    otherwise hexBinary with a copy of the same bytes.
 *
 * Does not throw for invalid UTF-8. Use this for process environment,
 * FCGI/CGI parameters, and similar external byte bags. Do not construct
 * string values from untrusted bytes any other way.
 *
 * Empty or NULL input yields an empty string value.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_create_from_external_octets(
    const afw_utf8_octet_t *s,
    afw_size_t len,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Create an Adaptive value from a NUL-terminated external C string.
 * @param s_z NUL-terminated bytes (may be NULL).
 * @param p pool for the value and any owned payload.
 * @param xctx of caller.
 * @return See afw_value_create_from_external_octets().
 */
AFW_DECLARE(const afw_value_t *)
afw_value_create_from_external_z(
    const char *s_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Make an afw_value_string_t from utf8_z in specified pool.
 * @param string_z must be valid UTF-8 (will be NFC-normalized).
 * @param p pool for the value.
 * @param xctx of caller.
 * @return string value.
 *
 * Throws if string_z is not valid UTF-8. For untrusted external bytes use
 * afw_value_create_from_external_octets() or afw_value_create_from_external_z().
 */
AFW_DECLARE(const afw_value_t *)
afw_value_create_string_from_u8z(
    const afw_utf8_z_t *string_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/** Convert internal value to string value. */
AFW_DECLARE(const afw_value_t *)
afw_value_string_from_internal(
    const afw_value_t *value, const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Create a dateTime value with current time.
 * @param p pool used for value.
 * @param xctx of caller.
 * @return Created afw_value_t.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_create_dateTime_now_utc(
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Create a dateTime value with current local time.
 * @param p pool used for value.
 * @param xctx of caller.
 * @return Created afw_value_t.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_create_dateTime_now_local(
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Return a NULL terminated list of values in a specified pool.
 * @param value
 * @param p to use for returned value.
 * @param xctx of caller.
 * @return NULL terminated list of values or undefined if value is undefined.
 */
AFW_DECLARE(const afw_value_t * const *)
afw_value_as_array_of_values(
    const afw_value_t * value,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Return a NULL terminated list of utf8 in a specified pool.
 * @param value
 * @param p to use for returned value.
 * @param xctx of caller.
 * @return NULL terminated list of strings or undefined if value is undefined.
 */
AFW_DECLARE(const afw_utf8_t * const *)
afw_value_as_array_of_utf8(
    const afw_value_t * value,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Decompile a value to a compiler listing string.
 * @param value to decompile.
 * @param tab if not NULL, adds \n and the tab string for readability.
 * @param p for result.
 * @param xctx of caller.
 * @return decompiled string.
 *
 */
AFW_DEFINE(const afw_utf8_t *)
afw_value_compiler_listing_to_string(
    const afw_value_t *value,
    const afw_utf8_t *tab,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Decompile a value to a string.
 * @param value to decompile.
 * @param tab if not NULL, adds \n and the tab string for readability.
 * @param p for result.
 * @param xctx of caller.
 * @return decompiled string.
 *
 */
AFW_DECLARE(const afw_utf8_t *)
afw_value_decompile_to_string(
    const afw_value_t *value,
    const afw_utf8_t *tab,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Decompile call args.
 * @param writer
 * @param first_arg index into argv (1 = parameters only; 0 includes callee).
 * @param args to decompile (argc is parameter count; values are argv[1..argc]).
 * @param xctx of caller.
 */
AFW_DEFINE(void)
afw_value_decompile_call_args(
    const afw_writer_t *writer,
    afw_size_t first_arg,
    const afw_value_call_args_t *args,
    afw_xctx_t *xctx);


/**
 * @brief Expand call-site spreads (...arr) into a flat argv.
 * @param argc_in User parameter count (not including argv[0]).
 * @param argv_in argv[0]=function, argv[1..argc_in]=args; may contain
 *        list_expression values marking spreads.
 * @param argc_out Expanded user parameter count.
 * @param argv_out Expanded argv (argv[0] same as argv_in[0]); may equal
 *        argv_in when there is no spread.
 * @param p Pool for expanded argv when needed.
 * @param xctx of caller.
 *
 * list_expression entries evaluate to an array and are spliced as separate
 * arguments (issue #140). Non-spread args are left unevaluated.
 */
AFW_DEFINE(void)
afw_value_call_args_expand_spreads(
    afw_size_t argc_in,
    const afw_value_t * const *argv_in,
    afw_size_t *argc_out,
    const afw_value_t * const **argv_out,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Write synthetic decompile name `#` + implementation_id.
 * @param instance value whose inf id is used.
 * @param writer
 * @param xctx of caller.
 *
 * Used for compiled value kinds that have no Adaptive surface form. The leading
 * '#' matches the pound_identifier token family (not a registered function).
 */
AFW_DEFINE(void)
afw_value_decompile_write_synthetic_function_name(
    const afw_value_t *instance,
    const afw_writer_t *writer,
    afw_xctx_t *xctx);


/**
 * @brief Decompile a parenthesized list of values.
 * @param writer
 * @param argc number of values.
 * @param argv values (may contain NULL for undefined).
 * @param xctx of caller.
 */
AFW_DEFINE(void)
afw_value_decompile_value_list(
    const afw_writer_t *writer,
    afw_size_t argc,
    const afw_value_t * const *argv,
    afw_xctx_t *xctx);


/**
 * @brief Decompile Value::.
 * @param instance to decompile.
 * @param writer
 * @param xctx of caller.
 *
 * Value ::= Json | Lambda | Evaluation | ParenthesizedExpression
 *
 * This function will put parentheses around decompiled instance if
 * instance is an expression.  Call this in decompiles of productions
 * that contain Value.
 */
AFW_DEFINE(void)
afw_value_decompile_value(
    const afw_value_t *instance,
    const afw_writer_t *writer,
    afw_xctx_t *xctx);


/**
 * @brief Type-check mode (issue #28).
 *
 * Default is off. typeCheckCompileOnly wins over typeCheck.
 * Resolved from unit policy when contextual->compiled_value is set,
 * otherwise from process compile:* flags (fast afw_flag_is_active).
 */
typedef enum afw_value_type_check_mode_e {
    afw_value_type_check_mode_off = 0,
    afw_value_type_check_mode_compile_only,
    afw_value_type_check_mode_on
} afw_value_type_check_mode_t;

/*
 * Hot-path helpers are macros (AFW style): expand without a call even when
 * not optimized. See afw-c-runtime (prefer macros over static inline).
 *
 * AFW_VALUE_TYPE_CHECK_*_CV(cv, xctx) — cv is afw_value_compiled_value_t *
 *   or NULL (process flags only).
 * AFW_VALUE_TYPE_CHECK_*(contextual, xctx) — uses contextual->compiled_value
 *   when contextual is non-NULL; NULL contextual => process flags only.
 */

/** Mode from compiled value (NULL cv => process flags). */
#define AFW_VALUE_TYPE_CHECK_MODE_CV(cv, xctx) \
    ( (cv) \
        ? ( (cv)->compile_policy.type_check_compile_only \
            ? afw_value_type_check_mode_compile_only \
            : ( (cv)->compile_policy.type_check \
                ? afw_value_type_check_mode_on \
                : afw_value_type_check_mode_off ) ) \
        : ( afw_flag_is_active( \
                (xctx)->env->flag_index_compile_typeCheckCompileOnly_active, \
                (xctx)) \
            ? afw_value_type_check_mode_compile_only \
            : ( afw_flag_is_active( \
                    (xctx)->env->flag_index_compile_typeCheck_active, \
                    (xctx)) \
                ? afw_value_type_check_mode_on \
                : afw_value_type_check_mode_off ) ) )

/** Mode from call/assign/etc. contextual (NULL => process flags). */
#define AFW_VALUE_TYPE_CHECK_MODE(contextual, xctx) \
    AFW_VALUE_TYPE_CHECK_MODE_CV( \
        ( (contextual) ? (contextual)->compiled_value : NULL ), (xctx))

#define AFW_VALUE_TYPE_CHECK_COMPILE_ENABLED_CV(cv, xctx) \
    (AFW_VALUE_TYPE_CHECK_MODE_CV((cv), (xctx)) != \
        afw_value_type_check_mode_off)

#define AFW_VALUE_TYPE_CHECK_COMPILE_ENABLED(contextual, xctx) \
    (AFW_VALUE_TYPE_CHECK_MODE((contextual), (xctx)) != \
        afw_value_type_check_mode_off)

#define AFW_VALUE_TYPE_CHECK_RUNTIME_ENABLED_CV(cv, xctx) \
    (AFW_VALUE_TYPE_CHECK_MODE_CV((cv), (xctx)) == \
        afw_value_type_check_mode_on)

#define AFW_VALUE_TYPE_CHECK_RUNTIME_ENABLED(contextual, xctx) \
    (AFW_VALUE_TYPE_CHECK_MODE((contextual), (xctx)) == \
        afw_value_type_check_mode_on)

#define AFW_VALUE_TYPE_CHECK_NO_IMPLICIT_ANY_CV(cv, xctx) \
    ( (cv) \
        ? (cv)->compile_policy.no_implicit_any \
        : afw_flag_is_active( \
            (xctx)->env->flag_index_compile_noImplicitAny_active, (xctx)) )

#define AFW_VALUE_TYPE_CHECK_NO_IMPLICIT_ANY(contextual, xctx) \
    AFW_VALUE_TYPE_CHECK_NO_IMPLICIT_ANY_CV( \
        ( (contextual) ? (contextual)->compiled_value : NULL ), (xctx))

#define AFW_VALUE_TYPE_CHECK_STRICT_NULL_CHECKS_CV(cv, xctx) \
    ( (cv) \
        ? (cv)->compile_policy.strict_null_checks \
        : afw_flag_is_active( \
            (xctx)->env->flag_index_compile_strictNullChecks_active, \
            (xctx)) )

#define AFW_VALUE_TYPE_CHECK_STRICT_NULL_CHECKS(contextual, xctx) \
    AFW_VALUE_TYPE_CHECK_STRICT_NULL_CHECKS_CV( \
        ( (contextual) ? (contextual)->compiled_value : NULL ), (xctx))

#define AFW_VALUE_TYPE_CHECK_NO_OPTIMIZE_CV(cv, xctx) \
    ( (cv) \
        ? (cv)->compile_policy.no_optimize \
        : afw_flag_is_active( \
            (xctx)->env->flag_index_compile_noOptimize_active, (xctx)) )

#define AFW_VALUE_TYPE_CHECK_NO_OPTIMIZE(contextual, xctx) \
    AFW_VALUE_TYPE_CHECK_NO_OPTIMIZE_CV( \
        ( (contextual) ? (contextual)->compiled_value : NULL ), (xctx))

/**
 * @brief True if type is missing, any, or zero-init leaf any.
 */
AFW_DEFINE(afw_boolean_t)
afw_value_type_is_any(const afw_value_type_t *type);

/**
 * @brief Leaf Adaptive data type if kind is data_type; else NULL.
 */
AFW_DEFINE(const afw_data_type_t *)
afw_value_type_get_leaf_data_type(const afw_value_type_t *type);

/**
 * @brief Whether value is assignable to expected type.
 * @param expected target type graph.
 * @param value candidate (C NULL is treated as undefined for nullish rules).
 * @param contextual call/assign site (NULL => process flags for strictNull).
 * @param xctx of caller.
 *
 * Handles leaves, unions/intersections, array/tuple elements, and
 * object/interface properties (with extends) when the value is inspectable.
 * Nullish (C NULL, afw_value_undefined, Adaptive null) follows unit/process
 * strictNullChecks policy when checking is active.
 */
AFW_DEFINE(afw_boolean_t)
afw_value_type_is_assignable(
    const afw_value_type_t *expected,
    const afw_value_t *value,
    const afw_compile_value_contextual_t *contextual,
    afw_xctx_t *xctx);

/**
 * @brief Throw if value is not assignable (when runtime checking on).
 * @param contextual unit link (NULL => process flags).
 */
AFW_DEFINE(void)
afw_value_type_check_assignable(
    const afw_value_type_t *expected,
    const afw_value_t *value,
    const afw_utf8_z_t *what,
    const afw_compile_value_contextual_t *contextual,
    afw_xctx_t *xctx);

/**
 * @brief Compile-time check when RHS type is known (syntax errors).
 * @param contextual unit link (NULL => process flags).
 */
AFW_DEFINE(void)
afw_value_type_check_compile_assignable(
    const afw_value_type_t *expected,
    const afw_value_t *value,
    const afw_utf8_z_t *what,
    const afw_compile_value_contextual_t *contextual,
    afw_xctx_t *xctx);

/**
 * @brief Excess-property check for object-literal call arguments.
 * @param contextual unit link (NULL => process flags).
 */
AFW_DEFINE(void)
afw_value_type_check_call_arg_object_literal(
    const afw_value_type_t *expected,
    const afw_value_t *value,
    const afw_utf8_z_t *what,
    const afw_compile_value_contextual_t *contextual,
    afw_xctx_t *xctx);

/**
 * @brief Compile-time type check for a known adaptive (built-in) function call.
 * @param contextual call site (NULL => process flags).
 *
 * No-op unless compile type checking is enabled for that unit/flags.
 * See designs/adaptive-function-compile-typecheck.md.
 */
AFW_DEFINE(void)
afw_value_type_check_adaptive_function_call(
    const afw_value_function_definition_t *function,
    afw_size_t argc,
    const afw_value_t *const *argv,
    const afw_compile_value_contextual_t *contextual,
    afw_xctx_t *xctx);

/**
 * @brief Decompile a type as Adaptive Type surface text (no leading ':').
 * @param type to decompile; NULL or "any" writes nothing (caller skips ':').
 * @param writer
 * @param xctx of caller.
 * @return true if anything was written (type is present and not bare any).
 *
 * Used for assignment targets, script_function params/returns, and future
 * type-check work. Prefer TS-like surface forms (e.g. `integer`, `integer[]`).
 */
AFW_DEFINE(afw_boolean_t)
afw_value_decompile_type(
    const afw_value_type_t *type,
    const afw_writer_t *writer,
    afw_xctx_t *xctx);


/**
 * @brief Write ": Type" when type is present and not bare any.
 * @param type optional type.
 * @param writer
 * @param xctx of caller.
 */
AFW_DEFINE(void)
afw_value_decompile_optional_type(
    const afw_value_type_t *type,
    const afw_writer_t *writer,
    afw_xctx_t *xctx);


/**
 * @brief Decompile a list/object Pattern (or symbol) without #assignment_target.
 * @param instance assignment_target value or symbol_reference.
 * @param writer
 * @param xctx of caller.
 *
 * Used for script function parameter Patterns so decompile stays surface-like
 * (`function f({a,b})` / `#script_function({a,b}, body)`).
 */
AFW_DEFINE(void)
afw_value_decompile_assignment_pattern(
    const afw_value_t *instance,
    const afw_writer_t *writer,
    afw_xctx_t *xctx);


/**
 * @internal
 * @brief  Register core value infs.
 * @param xctx of caller.
 *
 * Called in afw_environment_register_core.c.
 */
AFW_DECLARE(void)
afw_value_register_core_value_infs(afw_xctx_t *xctx);

AFW_END_DECLARES

/** @} */  // end of @addtogroup @addtogroup

#endif /* __AFW_VALUE_H__ */
