// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for compiler_expression
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_compiler_expression.c
 * @brief afw_function_execute_* functions for compiler_expression.
 */

#include "afw.h"



/*
 * Adaptive function: annotate
 *
 * afw_function_execute_annotate
 *
 * See afw_function_bindings.h for more information.
 *
 * Create an annotated value.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function annotate(
 *       annotation: (object _AdaptiveAnnotation_),
 *       value: any
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   annotation - (object _AdaptiveAnnotation_) Annotation for value.
 *
 *   value - (any dataType) Any value. This value will not be evaluated.
 *
 * Returns:
 *
 *   (any dataType) Unevaluated annotated value ready for use by function
 *       evaluate().
 */
const afw_value_t *
afw_function_execute_annotate(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: nullish_coalescing
 *
 * afw_function_execute_nullish_coalescing
 *
 * See afw_function_bindings.h for more information.
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
 *       ...values_rest: (array of any)
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   values - (2 or more any dataType)
 *
 * Returns:
 *
 *   (any dataType) The first value of values that is not null or undefined.
 */
const afw_value_t *
afw_function_execute_nullish_coalescing(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: optional_chaining
 *
 * afw_function_execute_optional_chaining
 *
 * See afw_function_bindings.h for more information.
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
 *   arg1 - (any dataType)
 *
 *   arg2 - (any dataType)
 *
 * Returns:
 *
 *   (any dataType) Undefined value if arg1 is null or undefined but otherwise
 *       evaluated arg2.
 */
const afw_value_t *
afw_function_execute_optional_chaining(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: void_operator
 *
 * afw_function_execute_void_operator
 *
 * See afw_function_bindings.h for more information.
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
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   value - (any dataType) This is the value to evaluate.
 *
 * Returns:
 *
 *   (any dataType) This always returns undefined.
 */
const afw_value_t *
afw_function_execute_void_operator(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
