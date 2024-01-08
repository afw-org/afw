// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for regexp
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_regexp.c
 * @brief afw_function_execute_* functions for regexp.
 */

#include "afw.h"



/*
 * Adaptive function: compile<regexp>
 *
 * afw_function_execute_compile_regexp
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile regexp value and return either an unevaluated adaptive value or a
 * string containing the compiler listing.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function compile<regexp>(
 *       source: regexp,
 *       listing?: any
 *   ): unevaluated;
 * ```
 *
 * Parameters:
 *
 *   source - (regexp) regexp string to compile.
 *
 *   listing - (optional any dataType) If specified, a compiler listing is
 *       produced instead of an unevaluated compiled value.
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
afw_function_execute_compile_regexp(
    afw_function_execute_t *x)
{
    const afw_value_string_t *source;
    const afw_value_t *result;
    const afw_utf8_t *listing;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(source, 1, string);

    result = afw_compile_to_value(
        &source->internal, AFW_FUNCTION_SOURCE_LOCATION,
        afw_compile_type_regexp,
        NULL, NULL, x->p, x->xctx);

    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        listing = afw_function_evaluate_whitespace_parameter(x, 2);
        result = afw_value_create_unmanaged_string(
            afw_value_compiler_listing_to_string(result, listing,
                x->p, x->xctx),
            x->p, x->xctx);
    }

    return result;
}
