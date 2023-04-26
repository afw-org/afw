// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Basic Function Support.
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_FUNCTION_H__
#define __AFW_FUNCTION_H__

#include "afw_interface.h"

/**
 * @defgroup afw_function Adaptive function support
 * @ingroup afw_c_api_public
 *
 * Adaptive Framework runtime function support.
 * @{
 */

/**
 * @file afw_function.h
 * @brief Header file for Adaptive Framework Runtime Basic Function Support.
 */

AFW_BEGIN_DECLARES

/**
 * @brief Function execute is handled by standard polymorphic function handling.
 *
 * NULL in function->execute indicates standard polymorphic function handling
 * is performed in afw_value_call_built_in_function.c function
 * impl_afw_value_optional_evaluate().
 *
 * The reason this is NULL:
 * 
 * AFW_FUNCTION_EXECUTE_STANDARD_POLYMORPHIC_FUNCTION_HANDLING is NULL so
 * it can be used in extensions. The generated *_function_bindings.c source
 * contains a direct reference to the execute function name so if this was
 * a direct reference to a function, there would be linkage problems on some
 * operating systems.
 */
#define AFW_FUNCTION_EXECUTE_STANDARD_POLYMORPHIC_FUNCTION_HANDLING NULL


/**
 * @brief Function execute parameter
 * 
 * This is the parameter sent to implementation of a built-in function
 * during evaluation.
 */
struct afw_function_execute_s {

    /** @brief self of call_built_in_function. */
    const afw_value_call_built_in_function_t *self;

    /** @brief Pool for result. */
    const afw_pool_t *p;

    /** @brief The execution context (xctx) of caller.*/
    afw_xctx_t *xctx;

    /**
     * @brief The evaluated function definition.
     * 
     * For functions that are called polymorphically, this is the resolved
     * function implementation for the specific data type. See argv[0] for
     * the original.
     */
    const afw_value_function_definition_t *function;

    /** @brief This is the `<dataType>` for data type functions or NULL. */
    const afw_data_type_t *data_type;

    /** @brief This is the evaluated first parameter if data type function. */  
    const afw_value_t *first_arg;

    /**
     * @brief This is the function parameters
     * 
     * argv[0] Is the original function definition. For a function called
     * polymorphically, this is the polymorphic function definition.
     * 
     */
    const afw_value_t * const * argv;

    /** @brief This is the argv count not counting argv[0] */
    afw_size_t argc;
    
};



struct afw_function_environment_s {

    /** @brief Add operator function. */
    const afw_value_function_definition_t *add_operator_function;

    /** @brief Subtract operator function. */
    const afw_value_function_definition_t *subtract_operator_function;

    /** @brief Multiply operator function. */
    const afw_value_function_definition_t *multiply_operator_function;

    /** @brief Divide operator function. */
    const afw_value_function_definition_t *divide_operator_function;

    /** @brief Modulus operator function. */
    const afw_value_function_definition_t *modulus_operator_function;

    /** @brief Exponentiation operator function. */
    const afw_value_function_definition_t *exponentiation_operator_function;

    /** @brief Negative operator function. */
    const afw_value_function_definition_t *negative_operator_function;

    /** @brief And operator function. */
    const afw_value_function_definition_t *and_operator_function;

    /** @brief Or operator function. */
    const afw_value_function_definition_t *or_operator_function;

    /** @brief Not operator function. */
    const afw_value_function_definition_t *unary_not_operator_function;

    /** @brief Equal to operator function. */
    const afw_value_function_definition_t *equal_to_operator_function;

    /** @brief Not equal to operator function. */
    const afw_value_function_definition_t *not_equal_to_operator_function;

    /** @brief Less than operator function. */
    const afw_value_function_definition_t *less_than_operator_function;

    /** @brief Less than or equal to operator function. */
    const afw_value_function_definition_t *less_than_or_equal_to_function;

    /** @brief Greater than operator function. */
    const afw_value_function_definition_t *greater_than_operator_function;

    /** @brief Greater than or equal to operator function. */
    const afw_value_function_definition_t *greater_than_or_equal_to_function;
};



/**
 * @brief Get the unevaluated argv value or NULL.
 * @param A_N is the 1 based parameter number of argv to check.
 * @return argv[A_N] or NULL.
 * 
 * This will return NULL if A_N is greater than x->argc.
 *
 * This is used when implementing the body of an adaptive function.  Like all
 * of the AFW_FUNCTION_* macros, "x" must be the name of the function execute
 * struct pointer.
 */
#define AFW_FUNCTION_ARGV(A_N) \
((A_N <= x->argc) ? x->argv[A_N] : NULL)



/**
 * @brief Assert that argc is a specific number
 * @param n is the number to check
 *
 * This is used when implementing the body of an adaptive function.  Like all
 * of the AFW_FUNCTION_* macros, "x" must be the name of the function execute
 * struct pointer.
 */
#define AFW_FUNCTION_ASSERT_PARAMETER_COUNT_IS(n) \
do { \
    if ((x->argc) != (n)) {\
        AFW_THROW_ERROR_Z(general, "Expecting " #n " parameters.", x->xctx);\
    } \
} while (0)



/**
 * @brief Assert that argc is at least a specific number
 * @param n is the number to check
 *
 * This is used when implementing the body of an adaptive function.  Like all
 * of the AFW_FUNCTION_* macros, "x" must be the name of the function execute
 * struct pointer.
 */
#define AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MIN(n) \
do { \
    if (x->argc < (n)) {\
        AFW_THROW_ERROR_Z(general, "Expecting at least " #n " parameters.", x->xctx);\
    } \
} while (0)



/**
 * @brief Assert that argc is no more than specific number
 * @param n is the number to check
 *
 * This is used when implementing the body of an adaptive function.  Like all
 * of the AFW_FUNCTION_* macros, "x" must be the name of the function execute
 * struct pointer.
 */
#define AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(n) \
do { \
    if (x->argc > (n)) {\
        AFW_THROW_ERROR_Z(general, "Expecting no more than " #n " parameters.", x->xctx);\
    } \
} while (0)



/**
 * @brief Assert that a value is at data type with cType of afw_utf8_t.
 * @param A_VALUE to check.
 *
 * This is used when implementing the body of an adaptive function.  Like all
 * of the AFW_FUNCTION_* macros, "x" must be the name of the function execute
 * struct pointer.
 */
#define AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(A_VALUE) \
if (!(A_VALUE) || !afw_utf8_equal( \
    &afw_value_get_data_type(((const afw_value_t *)A_VALUE), x->xctx)->cType, \
    &afw_s_afw_utf8_t)) \
{ \
    AFW_THROW_ERROR_Z(arg_error, \
        "Value's data type not supported for this function", x->xctx); \
}


 
/**
 * @brief Determine if a specific parameter value is present.
 * @param A_N is the 1 based parameter number of argv to check.
 *
 * This is used when implementing the body of an adaptive function.  Like all
 * of the AFW_FUNCTION_* macros, "x" must be the name of the function execute
 * struct pointer.
 */
#define AFW_FUNCTION_PARAMETER_IS_PRESENT(A_N) \
    (AFW_FUNCTION_ARGV(A_N) != NULL)



/**
 * @brief Evaluate an arg for a particular data type
 * @param A_RESULT is a const `afw_value_<A_TYPE>_t` * variable name for result.
 * @param n is the 1 based parameter number in of argv to evaluate.
 * @param A_TYPE is the unquoted dataType.
 * @return the value casted to `afw_value_<A_TYPE>_t` * or NULL (undefined).
 *
 * This is used when implementing the body of an adaptive function.  Like all
 * of the AFW_FUNCTION_* macros, "x" must be the name of the function execute
 * struct pointer.
 * 
 * It is up to the implementation to check if A_RESULT is NULL and the
 * correct data type.
 */
#define AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(A_RESULT, A_N, A_TYPE) \
A_RESULT = (const afw_value_##A_TYPE##_t *) \
    afw_function_evaluate_parameter(x, A_N, afw_data_type_##A_TYPE)



/**
 * @brief Evaluate a parameter.
 * @param A_RESULT is a const afw_value_t * variable name for result.
 * @param A_N is the 1 based parameter number in argv to evaluate.
 *
 * This is used when implementing the body of an adaptive function.  Like all
 * of the AFW_FUNCTION_* macros, "x" must be the name of the function execute
 * struct pointer.
 *
 * It is up to the implementation to check if A_RESULT is NULL and the
 * correct data type.
 */
#define AFW_FUNCTION_EVALUATE_PARAMETER(A_RESULT, A_N) \
A_RESULT = afw_function_evaluate_parameter(x, A_N, NULL)



/**
 * @brief Evaluate an required parameter
 * @param A_RESULT is a const afw_value_t * variable name for result.
 * @param A_N is the 1 based parameter number in argv to evaluate.
 *
 * This is used when implementing the body of an adaptive function.  Like all
 * of the AFW_FUNCTION_* macros, "x" must be the name of the function execute
 * struct pointer.
 *
 * A undefined error is thrown if result is NULL.
 */
#define AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(A_RESULT, A_N) \
A_RESULT = afw_function_evaluate_required_parameter(x, A_N, NULL);



/**
 * @brief Evaluate an arg that is a required condition
 * @param A_RESULT is a const `afw_value_<A_TYPE>_t` * variable name for result.
 * @param n is the 1 based parameter number in of argv to evaluate.
 *
 * This is used when implementing the body of an adaptive function.  Like all
 * of the AFW_FUNCTION_* macros, "x" must be the name of the function execute
 * struct pointer.
 */
#define AFW_FUNCTION_EVALUATE_REQUIRED_CONDITION_PARAMETER(A_RESULT, A_N) \
A_RESULT = (const afw_value_boolean_t *) \
    afw_function_evaluate_required_parameter(x, A_N, NULL); \
    if (!afw_value_is_boolean(A_RESULT)) \
        AFW_THROW_ERROR_FZ(arg_error, xctx, \
            "Condition must be boolean (parameter %d)", (A_N))



/**
 * @brief Evaluate an arg for a particular data type
 * @param A_RESULT is a const `afw_value_<A_TYPE>_t` * variable name for result.
 * @param n is the 1 based parameter number in of argv to evaluate.
 * @param A_TYPE is the unquoted dataType.
 *
 * This is used when implementing the body of an adaptive function.  Like all
 * of the AFW_FUNCTION_* macros, "x" must be the name of the function execute
 * struct pointer.
 */
#define AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(A_RESULT, A_N, A_TYPE) \
A_RESULT = (const afw_value_##A_TYPE##_t *) \
    afw_function_evaluate_required_parameter(x, A_N, afw_data_type_##A_TYPE)



/**
 * @brief Source location of a value.
 * 
 * This is used when implementing the body of an adaptive function.  Like all
 * of the AFW_FUNCTION_* macros, "x" must be the name of the function execute
 * struct pointer.
 */
#define AFW_FUNCTION_SOURCE_LOCATION \
afw_compile_source_location_of_value( \
    (const afw_value_t *)x->self, x->p, x->xctx)



/*
 * @brief Evaluate function parameter.
 * @param function_arg.
 * @param p Pool to use for result.
 * @param xctx of caller.
 * @return function.
 */
AFW_DECLARE(const afw_value_t *)
afw_function_evaluate_function_parameter(
    const afw_value_t *function_arg,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/*
 * @brief Execute whitespace parameter.
 * @param x function execute struct pointer.
 * @param n of whitespace parameter in argv.
 * @return a string containing whitespace to use.
 * 
 * This parameter can be an integer between 0 and 10 or a string that is used
 * for indentation.  If 0 is specified, no whitespace is added to the resulting
 * string.  If 1 through 10 is specified, that number of spaces is returned.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_function_evaluate_whitespace_parameter(
    afw_function_execute_t *x,
    afw_size_t n);


/**
 * @brief Evaluate a parameter and convert if necessary.
 * @param x function execute struct pointer.
 * @param parameter_number starting at 1.
 * @param data_type result will be converted to if needed or NULL.
 * @return value of parameter or undefined (NULL).
 *
 * This function adds the parameter number to the evaluation stack if an
 * evaluation or conversion is needed then removes it if successful.
 *
 * It's up to the caller to check if the returned value is undefined. Use
 * afw_function_evaluate_required_parameter() instead for required parameters.
 *
 * If data_type specified and result can't be converted, an error is thrown.
 */
AFW_DECLARE(const afw_value_t *)
afw_function_evaluate_parameter(
    afw_function_execute_t *x,
    afw_size_t parameter_number,
    const afw_data_type_t *data_type);



/**
 * @brief Evaluate an required parameter and convert if necessary.
 * @param x function execute struct pointer.
 * @param parameter_number starting at 1.
 * @param data_type result will be converted to if needed or NULL.
 * @return value of parameter.
 *
 * This function adds the parameter number to the evaluation stack if an
 * evaluation or conversion is needed then removes it if successful.
 *
 * If result is undefined or data_type is specified and can't be converted, an
 * error is thrown.
 */
AFW_DEFINE(const afw_value_t *)
afw_function_evaluate_required_parameter(
    afw_function_execute_t *x,
    afw_size_t parameter_number,
    const afw_data_type_t *data_type);



/**
 * @brief Evaluate a parameter with dataTypeParameter.
 * @param value to evaluate.
 * @param parameter_number starting at 1.
 * @param type for result or NULL if not converting.
 * @param p Pool
 * @param xctx of caller.
 * @return value or undefined if there is an error.
 *
 * This is called for script function parameters so is different from others
 * for now.
 */
AFW_DECLARE(const afw_value_t *)
afw_function_evaluate_parameter_with_type(
    const afw_value_t *value,
    afw_size_t parameter_number,
    const afw_value_type_t *type,
    const afw_pool_t *p, afw_xctx_t *xctx);



AFW_END_DECLARES

/** @} */

#endif /* __AFW_FUNCTION_H__ */
