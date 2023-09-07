// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for expression
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_expression.c
 * @brief afw_function_execute_* functions for expression.
 */

#include "afw.h"

 
 
/*
 * Adaptive function: compile<expression>
 *
 * afw_function_execute_compile_expression
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile expression value and return either an unevaluated adaptive value or a
 * string containing the compiler listing.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function compile<expression>(
 *       source: expression,
 *       listing?: any
 *   ): unevaluated;
 * ```
 *
 * Parameters:
 *
 *   source - (expression) expression string to compile.
 *
 *   listing - (optional any dataType) If specified, a compiler listing is
 *       produced instead of an unevaluated expression value.
 * 
 *       This parameter can be an integer between 0 and 10 of a string that is
 *       used for indentation. If 0 is specified, no whitespace is added to the
 *       resulting string. If 1 through 10 is specified, that number of spaces
 *       is used.
 *
 * Returns:
 *
 *   (unevaluated)
 */
const afw_value_t *
afw_function_execute_compile_expression(
    afw_function_execute_t *x)
{
    const afw_value_expression_t *source;
    const afw_value_t *result;
    const afw_utf8_t *listing;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(source, 1, expression);

    result = afw_compile_to_value(
        &source->internal, AFW_FUNCTION_SOURCE_LOCATION,
        afw_compile_type_expression,
        NULL, NULL, x->p, x->xctx);

    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        listing = afw_function_evaluate_whitespace_parameter(x, 2);
        result = afw_value_create_string(
            afw_value_compiler_listing_to_string(result, listing,
                x->p, x->xctx),
            x->p, x->xctx);
    }

    return result;
}



/*
 * Adaptive function: evaluate<expression>
 *
 * afw_function_execute_evaluate_expression
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile and evaluate expression value.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function evaluate<expression>(
 *       source: expression,
 *       additionalUntrustedQualifiedVariables?: (object _AdaptiveHybridPropertiesObjects_)
 *   ): unevaluated;
 * ```
 *
 * Parameters:
 *
 *   source - (expression) expression string to compile and evaluate.
 *
 *   additionalUntrustedQualifiedVariables - (optional object
 *       _AdaptiveHybridPropertiesObjects_) This parameter supplies additional
 *       qualified variables that can be accessed during evaluation. These
 *       variables will not be used by anything that needs to ensure its
 *       qualified variables must come from a trusted source, such as
 *       authorization. This parameter is intended to be used for testing only
 *       and should not be used for anything running in production.
 *
 * Returns:
 *
 *   (unevaluated)
 */
const afw_value_t *
afw_function_execute_evaluate_expression(
    afw_function_execute_t *x)
{
    const afw_value_expression_t *source;
    const afw_value_t *compiled;
    const afw_value_t *value;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(source, 1, expression);

    compiled = afw_compile_to_value(
        &source->internal, AFW_FUNCTION_SOURCE_LOCATION,
        afw_compile_type_expression,
        NULL, NULL, x->p, x->xctx);

    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        value = afw_value_evaluate_with_additional_untrusted_qualified_variables(
            compiled, x->argv[2], x->p, x->xctx);
    }
    else {
        value = afw_value_evaluate(compiled, x->p, x->xctx);
    }
  
    afw_xctx_statement_flow_reset_all_except_rethrow(x->xctx);
    return value;
}
