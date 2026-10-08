// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for compiler_expression
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_compiler_expression.c
 * @brief Adaptive function execute implementations for category `compiler_expression`.
 */

#include "afw_internal.h"

 
 
/*
 * Adaptive function: nullish_coalescing
 *
 * afw_function_execute_nullish_coalescing
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Returns the first value of values that is not null or undefined leaving the
 * remaining values unevaluated.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function nullish_coalescing(
 *       values_1: any,
 *       values_2: any,
 *       ...values_rest: any[]
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   values - (2 or more any)
 *
 * Returns:
 *
 *   (any) The first value of values that is not null or undefined.
 */
const afw_value_t *
afw_function_execute_nullish_coalescing(
    afw_function_execute_t *x)
{
    const afw_value_t *arg;
    afw_size_t i;

    arg = NULL;
    for (i = 1; i <= x->argc; i++) {
        AFW_FUNCTION_EVALUATE_PARAMETER(arg, i);
        if (!afw_value_is_nullish(arg)) {
            return arg;
        }
    }

    return arg;
}


/*
 * Adaptive function: optional_chaining
 *
 * afw_function_execute_optional_chaining
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Returns undefined if arg1 is null or undefined without evaluating arg2, but
 * otherwise returns evaluated value of arg2.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function optional_chaining(
 *       arg1: any,
 *       arg2: any
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   arg1 - (any)
 *
 *   arg2 - (any)
 *
 * Returns:
 *
 *   (any) Undefined value if arg1 is null or undefined but otherwise evaluated
 *       arg2.
 */
const afw_value_t *
afw_function_execute_optional_chaining(
    afw_function_execute_t *x)
{
    const afw_value_t *arg1;
    const afw_value_t *arg2;
    const afw_value_reference_by_key_t *ref;
    afw_value_reference_by_key_t *once_ref;
    const afw_value_call_t *call;
    afw_value_call_t *once_call;
    const afw_value_t **argv;

    AFW_FUNCTION_EVALUATE_PARAMETER(arg1, 1);
    if (afw_value_is_nullish(arg1)) {
        return NULL;
    }

    /*
     * The compiler makes arg2 from arg1's tree: base?.key is
     * optional_chaining(base, base[key]) and base?.(args) is
     * optional_chaining(base, base(args)). Use the evaluated arg1 in a
     * copy of arg2 so base is evaluated once, not again for every '?.'
     * in a chain.
     */
    arg2 = x->argv[2];
    if (afw_value_is_reference_by_key(arg2)) {
        ref = (const afw_value_reference_by_key_t *)arg2;
        if (ref->aggregate_value == x->argv[1]) {
            once_ref = afw_pool_malloc_type(x->p,
                afw_value_reference_by_key_t, x->xctx);
            afw_memory_copy(once_ref, ref);
            once_ref->aggregate_value = arg1;
            arg2 = &once_ref->pub;
        }
    }
    else if (afw_value_is_call(arg2)) {
        call = (const afw_value_call_t *)arg2;
        if (call->function_value == x->argv[1]) {
            once_call = afw_pool_malloc_type(x->p,
                afw_value_call_t, x->xctx);
            afw_memory_copy(once_call, call);
            argv = afw_pool_malloc(x->p,
                sizeof(afw_value_t *) * (call->args.argc + 1), x->xctx);
            memcpy(argv, call->args.argv,
                sizeof(afw_value_t *) * (call->args.argc + 1));
            argv[0] = arg1;
            once_call->args.argv = argv;
            once_call->function_value = arg1;
            once_call->optimized_value = &once_call->pub;
            arg2 = &once_call->pub;
        }
    }

    arg2 = afw_value_evaluate(arg2, x->p, x->xctx);
    if (afw_value_is_undefined(arg2)) {
        arg2 = NULL;
    }
    return arg2;
}



/*
 * Adaptive function: void_operator
 *
 * afw_function_execute_void_operator
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * This is the support function for the void operator which evaluates value and
 * returns undefined.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function void_operator(
 *       value: any
 *   ): undefined;
 * ```
 *
 * Parameters:
 *
 *   value - (any) This is the value to evaluate (including undefined).
 *       Evaluated for side effects only.
 *
 * Returns:
 *
 *   (undefined) This always returns undefined.
 */
const afw_value_t *
afw_function_execute_void_operator(
    afw_function_execute_t *x)
{
    const afw_value_t *result;

    /*
     * Evaluate operand for side effects. Operand may be undefined
     * (void undefined === undefined in ES and Adaptive).
     */
    AFW_FUNCTION_EVALUATE_PARAMETER(result, 1);
    (void)result;

    return afw_value_undefined;
}
