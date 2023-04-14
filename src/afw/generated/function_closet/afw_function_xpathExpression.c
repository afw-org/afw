// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for xpathExpression
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_xpathExpression.c
 * @brief afw_function_execute_* functions for xpathExpression.
 */

#include "afw.h"



/*
 * Adaptive function: compile<xpathExpression>
 *
 * afw_function_execute_compile_xpathExpression
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile xpathExpression value and return either an unevaluated adaptive
 * value or a string containing the compiler listing.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 *   function compile<xpathExpression>(
 *       source: xpathExpression,
 *       listing?: any
 *   ): unevaluated;
 *
 * Parameters:
 *
 *   source - (xpathExpression) xpathExpression string to compile.
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
afw_function_execute_compile_xpathExpression(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Adaptive function: evaluate<xpathExpression>
 *
 * afw_function_execute_evaluate_xpathExpression
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile and evaluate xpathExpression value.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 *   function evaluate<xpathExpression>(
 *       source: xpathExpression,
 *       additionalUntrustedQualifiedVariables?: (object _AdaptiveHybridPropertiesObjects_)
 *   ): unevaluated;
 *
 * Parameters:
 *
 *   source - (xpathExpression) xpathExpression string to compile and evaluate.
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
afw_function_execute_evaluate_xpathExpression(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Adaptive function: xpath_node_count
 *
 * afw_function_execute_xpath_node_count
 *
 * See afw_function_bindings.h for more information.
 *
 * The number of nodes in a node-set.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 *   function xpath_node_count(
 *       nodeset: xpathExpression
 *   ): integer;
 *
 * Parameters:
 *
 *   nodeset - (xpathExpression)
 *
 * Returns:
 *
 *   (integer)
 */
const afw_value_t *
afw_function_execute_xpath_node_count(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Adaptive function: xpath_node_eq
 *
 * afw_function_execute_xpath_node_eq
 *
 * See afw_function_bindings.h for more information.
 *
 * Checks for xpathExpression arg1 is equal to xpathExpression arg2 and return
 * the boolean result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 *   function xpath_node_eq(
 *       arg1: xpathExpression,
 *       arg2: xpathExpression
 *   ): boolean;
 *
 * Parameters:
 *
 *   arg1 - (xpathExpression)
 *
 *   arg2 - (xpathExpression)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_xpath_node_eq(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Adaptive function: xpath_node_match
 *
 * afw_function_execute_xpath_node_match
 *
 * See afw_function_bindings.h for more information.
 *
 * Returns true if any of the nodes matched by nodeset1 are equal to any of the
 * nodes matched by nodeset2 or their corresponding children.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 *   function xpath_node_match(
 *       nodeset2: xpathExpression,
 *       arg2: xpathExpression
 *   ): boolean;
 *
 * Parameters:
 *
 *   nodeset2 - (xpathExpression)
 *
 *   arg2 - (xpathExpression)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_xpath_node_match(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}
