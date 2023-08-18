// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Header for Adaptive Values
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_VALUE_H__
#define __AFW_VALUE_H__

#include "afw_interface.h"

/**
 * @defgroup afw_value Value
 * @ingroup afw_c_api_public
 *
 * Adaptive Framework Value
 * @{
 */

/**
 * @file afw_value.h
 * @brief Adaptive Framework header for adaptive values
 * 
 * This is the main header for adaptive values.
 * 
 * An adaptive value is a structure that consists of an inf pointer followed
 * by internal memory specific to an afw_value implementation. There are
 * implementations of afw_value to represent values for all of the adaptive
 * data types as well as implementations that can be evaluated at runtime such
 * as "compiled_value", "block", and "symbol_reference".
 *
 * Adaptive source, such as adaptive expressions, json, and adaptive scripts,
 * can be compiled to produce an adaptive value. Adaptive values can also be
 * produced by calling their corresponding type's afw_value_allocate_* or
 * afw_value_create_* function. 
 * 
 * The afw_value_evaluate() macro can be called to evaluate any adaptive
 * value. For example, afw_value_evaluate() can be called for an adaptive
 * value returned from the adaptive compiler used to compile an adaptive script
 * to "run" the adaptive script and return its adaptive value result.
 * 
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
 * @brief Struct to access internal of all evaluated values.
 */
struct afw_value_evaluated_s {
    const afw_value_inf_t *inf;
    afw_octet_t internal; /* type specific value */
};



/** @brief Struct for adaptive function parameter. */
struct afw_value_function_parameter_s {
    afw_object_t meta;
    const afw_data_type_t *data_type;
    afw_utf8_t dataType;
    afw_utf8_t dataTypeParameter;
    afw_utf8_t name;
    afw_utf8_t brief;
    afw_utf8_t description;

    /*
     * This is the minimum number of values that can be specified for this
     * parameter.  If -1, the parameter can be specified exactly once.  This
     * can only have a value other than -1 on last parameter where it can
     * have a value of -1 to 127.
     */
    afw_integer_t minArgs;

    /* Indicates that parameter is optional and can be undefined (NULL). */
    afw_boolean_t optional;

    /* Indicates that parameter can be undefined (NULL) even if not optional. */
    afw_boolean_t canBeUndefined;

    /* Indicates that dataType is polymorphic. */
    afw_boolean_t polymorphicDataType;

    /* Indicates that dataTypeParameter is polymorphic. */
    afw_boolean_t polymorphicDataTypeParameter;

};



/** @brief Struct for function value. */
struct afw_value_function_definition_s {
    const afw_value_inf_t *inf;

    /** @brief Object representing function. */
    const afw_object_t *object;

    /** @brief Category of function. */
    afw_utf8_t category;

    /** @brief Function id of function. */
    afw_utf8_t functionId;

    /** @brief Function id without `<dataType>`.
     * 
     *  This is only different from functionId if polymorphic.
     */
    afw_utf8_t untypedFunctionId;

    /** @brief Label used internally for function. */
    afw_utf8_t functionLabel;

    /** @brief functionLabel converted to camel case. */
    afw_utf8_t camelCaseFunctionLabel;

    /** @brief functionLabel prefixed with "afw" and converted to camel case. */
    afw_utf8_t afwCamelCaseFunctionLabel;

    /** @brief Brief description. */
    afw_utf8_t brief;

    /** @brief Description of function. */
    afw_utf8_t description;

    /** @brief Function signature. */
    afw_utf8_t functionSignature;

    /** @brief Function declaration. */
    afw_utf8_t functionDeclaration;

    /** @brief Optional operator for function. */
    afw_utf8_t op;

    /** @brief Function called to execute this Adaptive function. */
    afw_function_execute_cb_t execute;

    /** @brief Optional function called to check args. */
    afw_function_arg_check_t arg_check;

    /** @brief The number of required parameters. */
    afw_integer_t numberOfRequiredParameters;

    /** @brief The maximum number of required parameters or -1 is no max. */
    afw_integer_t maximumNumberOfParameters;

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
    afw_utf8_t dataType;

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

    /** @brief See AFW_VALUE_SCRIPT_SUPPORT_NUMBER_* `#define`s. */
    afw_octet_t scriptSupportNumber;

    /** @brief True if this is a polymorphic function. */
    afw_boolean_t polymorphic;

    /** @brief True if polymorphic execute evaluates first parameter. */
    afw_boolean_t polymorphicExecuteFunctionEvaluatesFirstParameter;

    /** @brief This function is deprecated. */
    afw_boolean_t deprecated;

    /**
     * @brief Given the same exact parameter values, this function will always
     *     return the same result an not cause side effects.
     */
    afw_boolean_t pure;

    /**
     * @brief This is only a signature with an unimplemented execute function.
     */
    afw_boolean_t signatureOnly;
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



/** @brief Value call inf. */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_call_built_in_function_inf;



/** @brief Value call inf. */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_compiled_value_inf;



/** @brief Value function inf. */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_function_definition_inf;



/** @brief Value function thunk inf. */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_function_thunk_inf;



/** @brief Value lambda inf. */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_script_function_definition_inf;



/** @brief Value list expression inf. */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_list_expression_inf;



/** @brief Value object expression inf. */
AFW_DECLARE_CONST_DATA(afw_value_inf_t)
afw_value_object_expression_inf;



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
 * @brief Adaptive value null.
 *
 * Note: this is different from c NULL which is represented by undefined.  See
 * afw_value_undefined.
 */
AFW_DECLARE_CONST_DATA(afw_value_t *)
afw_value_null;



/**
 * @brief Adaptive value integer 1.
 */
AFW_DECLARE_CONST_DATA(afw_value_t *)
afw_value_integer_1;



/**
 * @brief An undefined value is represented by c NULL
 *
 * Undefined values are represented by c NULL.  Throughout source NULL and
 * undefined are used interchangeably.
 */
#define afw_value_undefined NULL



/**
 * @brief Unique value used to represent default case for switch().
 *
 * This is a unique value used to make the default case in parameters passed
 * to the switch() runtime support function. The address of this value is
 * used for uniqueness.
 */
AFW_DECLARE_CONST_DATA(afw_value_t *)
afw_value_unique_default_case_value;



/** @brief Adaptive value containing `<undefined>` string. */
AFW_DECLARE_CONST_DATA(afw_value_t *)
afw_value_undefined_as_string;



/** @brief Adaptive value empty string. */
AFW_DECLARE_CONST_DATA(afw_value_t *)
afw_value_empty_string;



/** @brief Adaptive value true. */
AFW_DECLARE_CONST_DATA(afw_value_t *)
afw_value_true;



/** @brief Adaptive value false. */
AFW_DECLARE_CONST_DATA(afw_value_t *)
afw_value_false;



/**
 * @brief Value for boolean variable.
 * @param variable
 * @return afw_value_true or afw_value_false
 */
#define afw_value_for_boolean(variable) (variable) \
    ? afw_value_true \
    : afw_value_false



/**
 * @brief Undecorated value in place.
 * @param value to undecorate.
 *
 * A value "decoration" is annotation and compile_unit values.
 */
#define AFW_VALUE_UNDECORATE(a_value) \
if (a_value) { \
    if ((a_value)->inf == &afw_value_compiled_value_inf) { \
        a_value = ((const afw_value_compiled_value_t *)a_value)->root_value; \
    } \
}



/**
 * @brief Return undecorated value.
 * @param value to undecorate.
 * @return undecorated value.
 *
 * A value "decoration" is annotation and compile_unit values.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_undecorate(
    const afw_value_t *value);

/**
 * @brief Determine if value's undecorated inf is the supplied one.
 * @param value
 * @param inf
 * @return true if undecorated value's inf is the supplied on.
 *
 * A value "decoration" is annotation and compile_unit values.
 */
AFW_DECLARE(afw_boolean_t)
afw_value_undecorated_inf_is(
    const afw_value_t *value,
    const afw_value_inf_t *inf);



/**
 * @brief Determine if value is undefined or null.
 * @param value to test.
 * @param xctx of caller.
 * @return boolean result.
 *
 * NOTE: Undefined values are represented by c NULL which is different from
 * an adaptive null value.
 */
#define afw_value_is_nullish(A_VALUE) \
( \
    (!A_VALUE) || \
    (A_VALUE)->inf->is_evaluated_of_data_type == afw_data_type_null \
)



/**
 * @brief Determine if value is undefined.
 * @param value to test.
 * @param xctx of caller.
 * @return boolean result.
 *
 * NOTE: Undefined values are represented by c NULL.
 */
#define afw_value_is_undefined(A_VALUE) (!A_VALUE)



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
 * @brief Macro to determine if value is defined and evaluated.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_defined_and_evaluated(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf->is_evaluated_of_data_type \
)



/**
 * @brief Macro to determine if value is undefined or evaluated.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_undefined_or_evaluated(A_VALUE) \
( \
    (!A_VALUE) || \
    (A_VALUE)->inf->is_evaluated_of_data_type \
)



/**
 * @brief Determine if value and all of it contained values are evaluated.
 * @param value to test.
 * @param xctx of caller.
 * @return boolean result.
 */
AFW_DECLARE(afw_boolean_t)
afw_value_is_fully_evaluated(
    const afw_value_t *value,
    afw_xctx_t *xctx);



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
 * @brief Macro to determine if value is a call.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_any_call(A_VALUE) \
( \
    (A_VALUE) && \
    ( \
        (A_VALUE)->inf == &afw_value_call_inf || \
        (A_VALUE)->inf == &afw_value_call_built_in_function_inf \
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
 * @brief Macro to determine if value is lambda definition.
 * @param A_VALUE to test.
 * @return boolean result.
 */
#define afw_value_is_script_function(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf == &afw_value_script_function_definition_inf \
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
#define afw_value_is_variable_reference(A_VALUE) \
( \
    (A_VALUE) && \
    (A_VALUE)->inf == &afw_value_symbol_reference_inf \
)

 
/** @brief Throw and error if A_VALUE is not value inf id. */
#define AFW_VALUE_ASSERT_IS(A_VALUE, A_TYPE_ID, A_SCOPE) \
if (!A_VALUE || (A_VALUE)->inf != &afw_value_ ## A_TYPE_ID ## _inf) \
    AFW_THROW_ERROR_Z(cast_error, "Expecting " #A_TYPE_ID, A_SCOPE)


/**
 * @brief Get the easily accessible data type for a value.
 * @param value
 * @return data_type or NULL.
 * 
 * This will return the data type of value if inf->data_type is not NULL.
 * This will be available for all evaluated values and some other values.
 */
#define afw_value_quick_data_type(A_VALUE) \
((A_VALUE)->inf->data_type)


/**
 * @brief Get the easily accessible data type id for a value.
 * @param value
 * 
 * This will return the data type id of value if inf->data_type is not NULL.
 * This will be available for all evaluated values and some other values.  If
 * the data type id is not available, "unknown" is returned.
 */
#define afw_value_get_quick_data_type_id(A_VALUE) \
(((A_VALUE) && (A_VALUE)->inf->data_type) \
? &((A_VALUE)->inf->data_type->data_type_id) \
: &afw_s_unknown )



/** @brief Test that the value in A_VALUE is an evaluated data type A_DATA_TYPE. */
#define AFW_VALUE_IS_DATA_TYPE(A_VALUE,A_DATA_TYPE) \
( \
    (A_VALUE) && (A_VALUE)->inf->is_evaluated_of_data_type && \
    (A_VALUE)->inf->is_evaluated_of_data_type == afw_data_type_ ## A_DATA_TYPE \
)



/** @brief Throw and error if A_VALUE is not evaluated data type A_DATA_TYPE. */
#define AFW_VALUE_ASSERT_IS_DATA_TYPE(A_VALUE, A_DATA_TYPE, A_SCOPE) \
do { \
if (!AFW_VALUE_IS_DATA_TYPE(A_VALUE, A_DATA_TYPE)) \
    AFW_THROW_ERROR_Z(cast_error, "Type safe exception.", A_SCOPE); \
} while (0)



/** @brief Throw and error if A_VALUE is not anyURI or string. */
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
 * @param value
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
 * @param value internal must align with afw_value_evaluated_t *.
 * @return const void * of internal.
 *
 * This should be used with extreme care.  The intended is to access internal
 * of an evaluated value.
 */
#define AFW_VALUE_INTERNAL(_VALUE_) \
((void *)(&((afw_value_evaluated_t *)(_VALUE_))->internal))

 
 
/**
 * @brief Assert that the data type of two adaptive values is the same
 * @param value1 is an adaptive value.
 * @param value2 is an adaptive value.
 * @param xctx of caller
 */
#define AFW_VALUE_DATA_TYPES_EQUAL(value1, value2, xctx) \
(afw_value_get_data_type(value1, xctx) != \
    afw_value_get_data_type(value2, xctx))



/**
 * @brief Assert that the data type of two adaptive values is the same
 * @param value1 is an adaptive value.
 * @param value2 is an adaptive value.
 * @param xctx of caller
 */
#define AFW_VALUE_ASSERT_DATA_TYPES_EQUAL(value1, value2, xctx) \
if (!AFW_VALUE_DATA_TYPES_EQUAL(value1, value2, xctx)) \
{ \
    AFW_THROW_ERROR_Z(cast_error, "Type safe exception.", xctx); \
}



/**
 * @brief Determine if value is boolean true.
 * @param value to test.
 * @param xctx of caller.
 * @return true if value is an evaluated boolean true value.
 */
#define afw_value_is_true(A_VALUE) \
( \
    AFW_VALUE_IS_DATA_TYPE(A_VALUE, boolean) && \
    ((const afw_value_boolean_t *)(A_VALUE))->internal \
)



/**
 * @brief Compile a value.
 * @param value must be a type that can be compiled
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
 * @param compile_type.
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
 * @param compile_type.
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
 * @param to_data_type.
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
 * @param allow_undefined if true returns '<undefined>" if value is NULL. 
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
 * @param argc is number of statements.
 * @param argv is array of statements.
 * @param xctx of caller.
 * @return Finalized block value.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_block_finalize(
    const afw_value_block_t *block,
    afw_size_t argc,
    const afw_value_t * const *argv,
    afw_xctx_t *xctx);




/**
 * @brief Allocate function for an evaluated data type value.
 * @param data_type of value.
 * @param p pool used for value.
 * @param xctx of caller.
 * @return Allocated afw_value_evaluated_t.
 */
AFW_DECLARE(afw_value_evaluated_t *)
afw_value_evaluated_allocate(
    const afw_data_type_t *data_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create function for an evaluated data type value.
 * @param value to copy.
 * @param data_type of value.
 * @param p pool used for value.
 * @param xctx of caller.
 * @return Created afw_value_t.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_evaluated_create(
    const void * value,
    const afw_data_type_t *data_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create function for call value.
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
 * @brief Create function for call_built_in_function value.
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
 * @brief Create function for lambda definition value.
 * @param contextual information for lambda.
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
    const afw_value_script_function_signature_t *signature,
    const afw_value_type_t *returns,
    afw_size_t count,
    const afw_value_script_function_parameter_t **parameters,
    const afw_value_t *body,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create function for list expression value.
 * @param contextual information for lambda.
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
 * @brief Create function for object expression value.
 * @param contextual information for lambda.
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
 * @brief Create function for variable reference value.
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
 * @brief Create function for reference_by_key value.
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
 * @brief Create function for template definition value.
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
 * @brief Create function for symbol reference value.
 * @param contextual information for variable.
 * @param symbol for variable.
 * @param p pool used for value.
 * @param xctx of caller.
 * @return Created afw_value_t.
 */
AFW_DECLARE(const afw_value_t *)
afw_value_symbol_reference_create(
    const afw_compile_value_contextual_t *contextual,
    const afw_value_frame_symbol_t *symbol,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/** Make an afw_value_evaluated_t String using string in specified pool. */
AFW_DECLARE(const afw_value_t *)
afw_value_make_single_string(
    const afw_utf8_octet_t *s,
    afw_size_t len,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/** Make an afw_value_evaluated_t String using copy of string in specified pool. */
AFW_DECLARE(const afw_value_t *)
afw_value_make_string_copy(
    const afw_utf8_octet_t *s,
    afw_size_t len,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/** Make an afw_value_string_t from utf8_z in specified pool. */
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
 * @param value.
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
 * @param value.
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
 * @param writer.
 * @param args to decompile.
 * @param xctx of caller.
 */
AFW_DEFINE(void)
afw_value_decompile_call_args(
    const afw_writer_t *writer,
    afw_size_t first_arg,
    const afw_value_call_args_t *args,
    afw_xctx_t *xctx);



/**
 * @brief Decompile Value::.
 * @param instance to decompile.
 * @param writer.
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
 * @internal
 * @brief  Register core value infs.
 * @param xctx of caller.
 *
 * Called in afw_environment_register_core.c.
 */
AFW_DECLARE(void)
afw_value_register_core_value_infs(afw_xctx_t *xctx);

AFW_END_DECLARES

/** @} */

#endif /* __AFW_VALUE_H__ */
