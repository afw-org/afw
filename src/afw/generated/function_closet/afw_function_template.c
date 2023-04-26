// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for template
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_template.c
 * @brief afw_function_execute_* functions for template.
 */

#include "afw.h"



/*
 * Adaptive function: compile<template>
 *
 * afw_function_execute_compile_template
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile template value and return either an unevaluated adaptive value or a
 * string containing the compiler listing.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function compile<template>(
 *       source: template,
 *       listing?: any
 *   ): unevaluated;
 * ```
 *
 * Parameters:
 *
 *   source - (template) template string to compile.
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
afw_function_execute_compile_template(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Adaptive function: evaluate<template>
 *
 * afw_function_execute_evaluate_template
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile and evaluate template value.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function evaluate<template>(
 *       source: template,
 *       additionalUntrustedQualifiedVariables?: (object _AdaptiveHybridPropertiesObjects_)
 *   ): unevaluated;
 * ```
 *
 * Parameters:
 *
 *   source - (template) template string to compile and evaluate.
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
afw_function_execute_evaluate_template(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}
