// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Basic Function Support.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_FUNCTION_H__
#define __AFW_FUNCTION_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_function
 * @{
 */

/**
 * @file afw_function.h
 * @brief Built-in Adaptive function execute support.
 *
 * See @ref afw_function (defined centrally in afw_doxygen.h).
 *
 * This header is the surface for **built-in Adaptive function** `execute_*`
 * implementations under `function/afw_function_*.c`:
 * - `AFW_FUNCTION_EVALUATE_*` macros and `afw_function_evaluate_parameter*`
 *   use the Adaptive function **definition** (leaf data types, requiredness)
 *   and optional convert-to-requested-type. Short hot path; no script type
 *   graph / unit typeCheck branching here.
 * - Polymorphic dispatch uses dataTypeMethod and execute == NULL.
 * - Operator functions are prepared in
 *   `afw_function_internal_prepare_environment`.
 *
 * Script language IR (const/let/assign/return, script formals, Patterns) lives
 * in `afw_function_compiler_internal.c` and `afw_value_call_script_function.c`,
 * not in these evaluate helpers. All Adaptive functions are still registered
 * via the environment; see `afw_generated_register`.
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
 * @brief Argument block for a built-in Adaptive function execute_* call.
 *
 * Filled by afw_value_call_built_in_function and passed as `x` to execute
 * bodies. Script language calls use call_script_function instead.
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
     * @brief Call argv (function + parameters).
     *
     * Call layout is the same for built-in adaptive functions and script
     * functions (see also `afw_value_call_args_t`):
     *
     *   argv[0]       — the function value (definition / polymorphic def /
     *                   script function / closure). Not a user parameter.
     *   argv[1..argc] — user parameters 1..argc (1-based parameter numbers).
     *   argc          — number of user parameters only (does not include
     *                   argv[0]). For f(a,b), argc is 2.
     *
     * AFW_FUNCTION_ARGV(n) and afw_function_evaluate_parameter(..., n, ...)
     * use that 1-based n. Do not treat argv like a pure C 0-based param list.
     *
     * For a polymorphic call, argv[0] is the original polymorphic definition;
     * x->function is the resolved per-type implementation.
     */
    const afw_value_t * const * argv;

    /**
     * @brief Number of user parameters (not counting argv[0]).
     *
     * Valid parameter numbers are 1..argc. argv[argc] is the last parameter.
     */
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
 * @brief Get the unevaluated argv entry for a user parameter, or NULL.
 * @param A_N 1-based parameter number (first user param is 1, not 0).
 * @return x->argv[A_N], or NULL if A_N > x->argc.
 *
 * argv[0] is the function, not a parameter — use AFW_FUNCTION_ARGV(1) for the
 * first parameter. Same numbering as error text "Parameter N" and as
 * afw_function_evaluate_parameter(..., N, ...).
 *
 * Used in adaptive function execute bodies. Like all AFW_FUNCTION_* macros,
 * "x" must be the name of the function execute struct pointer.
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
    afw_s_afw_utf8_t)) \
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
    (AFW_FUNCTION_ARGV(A_N) != NULL && \
    AFW_FUNCTION_ARGV(A_N) != afw_value_undefined)



/**
 * @brief Evaluate an optional parameter as data type A_TYPE.
 * @param A_RESULT const `afw_value_<A_TYPE>_t` * variable for the result.
 * @param A_N 1-based parameter number in argv.
 * @param A_TYPE unquoted data type id (e.g. string, object).
 *
 * Evaluates the argument, then yields a cast-safe
 * `const afw_value_<A_TYPE>_t *` (or NULL if optional/undefined). This is
 * evaluate-then-typed-pointer — not a produce-type probe like
 * `AFW_VALUE_EVALUATES_TO_DATA_TYPE`. Like other AFW_FUNCTION_* macros, "x"
 * is the function execute struct pointer.
 *
 * It is up to the implementation to handle a NULL A_RESULT.
 */
#define AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(A_RESULT, A_N, A_TYPE) \
A_RESULT = (const afw_value_##A_TYPE##_t *) \
    afw_function_evaluate_parameter(x, A_N, afw_data_type_##A_TYPE)



/**
 * @brief Evaluate an optional parameter (any type).
 * @param A_RESULT const afw_value_t * variable for the result.
 * @param A_N 1-based parameter number in argv.
 *
 * Result stays `const afw_value_t *` until you check with
 * `AFW_VALUE_IS_DATA_TYPE` / `afw_value_is_*` before a typed cast. "x" is the
 * function execute struct pointer.
 *
 * It is up to the implementation to handle a NULL A_RESULT.
 */
#define AFW_FUNCTION_EVALUATE_PARAMETER(A_RESULT, A_N) \
A_RESULT = afw_function_evaluate_parameter(x, A_N, NULL)



/**
 * @brief Evaluate a required parameter (any type).
 * @param A_RESULT const afw_value_t * variable for the result.
 * @param A_N 1-based parameter number in argv.
 *
 * Throws if the result is undefined/NULL. Result is still
 * `const afw_value_t *` until a cast-safe `is_*` / `IS_DATA_TYPE` check. "x"
 * is the function execute struct pointer.
 */
#define AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(A_RESULT, A_N) \
A_RESULT = afw_function_evaluate_required_parameter(x, A_N, NULL);



/**
 * @brief Evaluate a required boolean condition parameter.
 * @param A_RESULT const afw_value_boolean_t * variable for the result.
 * @param A_N 1-based parameter number in argv.
 *
 * Evaluates the arg, requires evaluated boolean (cast-safe
 * `const afw_value_boolean_t *`), else throws arg_error. "x" is the function
 * execute struct pointer.
 */
#define AFW_FUNCTION_EVALUATE_REQUIRED_CONDITION_PARAMETER(A_RESULT, A_N) \
A_RESULT = (const afw_value_boolean_t *) \
    afw_function_evaluate_required_parameter(x, A_N, NULL); \
    if (!afw_value_is_boolean(A_RESULT)) \
        AFW_THROW_ERROR_FZ(arg_error, xctx, \
            "Condition must be boolean (parameter %d)", (A_N))



/**
 * @brief Evaluate a required parameter as data type A_TYPE.
 * @param A_RESULT const `afw_value_<A_TYPE>_t` * variable for the result.
 * @param A_N 1-based parameter number in argv.
 * @param A_TYPE unquoted data type id (e.g. string, object).
 *
 * Evaluates the argument and requires data type A_TYPE, then yields a
 * cast-safe `const afw_value_<A_TYPE>_t *`. Throws if missing or wrong type.
 * Not a produce-type-only check. "x" is the function execute struct pointer.
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


/**
 * @brief Call-site contextual for this built-in invocation.
 * @param x Function execute struct pointer (`execute_*` argument).
 * @return Unit contextual from the call, or NULL if no call value.
 *
 * Use for nested afw_value_call_create (and similar) from execute_* so the
 * new call shares this invocation's unit link / source attribution. Prefer
 * this over NULL: NULL means type-check helpers use process flags only.
 * Safe from core and extensions (call_built_in layout stays opaque via
 * afw.h). See designs/compile-contextual-audit.md.
 */
AFW_DECLARE(const afw_compile_value_contextual_t *)
afw_function_execute_contextual(const afw_function_execute_t *x);


/**
 * @brief Execute function if caller has 'execute' access.
 * 
 * This is an execute function that wraps the actual function implementation
 * when the caller is required to have 'execute' access.
 */
extern const afw_value_t *
afw_function_execute_requiresExecuteAccess_wrapper(
    afw_function_execute_t *x);


/**
 * @brief Evaluate function parameter.
 * @param function_arg function value or definition.
 * @param p Pool to use for result.
 * @param xctx of caller.
 * @return function.
 */
AFW_DECLARE(const afw_value_t *)
afw_function_evaluate_function_parameter(
    const afw_value_t *function_arg,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
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
 * @brief Evaluate a built-in formal and convert if the caller requests a type.
 * @param x function execute struct pointer.
 * @param parameter_number 1-based (first user parameter is 1 → x->argv[1]).
 * @param data_type convert result to this leaf type if needed, or NULL.
 * @return value of parameter or undefined (NULL).
 *
 * Built-in Adaptive formals only. Reads `x->function->parameters[]` for
 * required/optional and declared leaf data type; optional convert uses the
 * `data_type` argument from the implementer (via AFW_FUNCTION_EVALUATE_*).
 * Does not apply script typeCheck policy.
 *
 * Uses the same 1-based numbering as AFW_FUNCTION_ARGV and "Parameter N"
 * errors. Does not count argv[0] (the function value). Pushes parameter
 * number on the evaluation stack when evaluating/converting, then pops.
 *
 * Caller must handle NULL (undefined). Prefer
 * afw_function_evaluate_required_parameter() when a non-NULL result is
 * required. Throws if data_type is set and convert fails.
 */
AFW_DECLARE(const afw_value_t *)
afw_function_evaluate_parameter(
    afw_function_execute_t *x,
    afw_size_t parameter_number,
    const afw_data_type_t *data_type);



/**
 * @brief Like afw_function_evaluate_parameter, but throws if undefined.
 * @param x function execute struct pointer.
 * @param parameter_number starting at 1.
 * @param data_type convert result to this leaf type if needed, or NULL.
 * @return non-NULL parameter value.
 *
 * Built-in Adaptive formals only (same rules as evaluate_parameter).
 * Throws if the result is undefined or if data_type convert fails.
 */
AFW_DEFINE(const afw_value_t *)
afw_function_evaluate_required_parameter(
    afw_function_execute_t *x,
    afw_size_t parameter_number,
    const afw_data_type_t *data_type);



AFW_END_DECLARES

/** @} */

#endif /* __AFW_FUNCTION_H__ */
