// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for script script
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_script.c
 * @brief Adaptive function execute implementations for category `script`.
 */

#include "afw_internal.h"



/*
 * Adaptive function: compile<script>
 *
 * afw_function_execute_compile_script
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Compile script value and return either an unevaluated adaptive value or a
 * string containing the compiler listing. The listing is a human-oriented dump
 * (value tree interleaved with source, plus ---Symbols tables) for Fiddle and
 * debugging — not pure JSON (use stringify) and not Adaptive compiled-form text
 * (use decompile).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function compile<script>(
 *       source: script,
 *       listing?: any
 *   ): unevaluated;
 * ```
 *
 * Parameters:
 *
 *   source - (script) script string to compile.
 *
 *   listing - (optional any) If specified, a human compiler listing is produced
 *       instead of an unevaluated compiled value (tree + ---Symbols; not
 *       recompilable). Use decompile() for Adaptive compiled-form text and
 *       stringify() for pure JSON of evaluated data.
 * 
 *       This parameter can be an integer between 0 and 10 or a string that is
 *       used for indentation. If 0 is specified, no whitespace is added to the
 *       resulting string. If 1 through 10 is specified, that number of spaces
 *       is used.
 *
 * Returns:
 *
 *   (unevaluated)
 *
 * Errors thrown:
 *
 *   syntax - source could not be compiled
 */
const afw_value_t *
afw_function_execute_compile_script(
    afw_function_execute_t *x)
{
    const afw_value_script_t *script;
    const afw_value_t *result;
    const afw_value_t *compiled;
    const afw_utf8_t *listing;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(script, 1, script);

    result = afw_compile_to_value(
        &script->internal, AFW_FUNCTION_SOURCE_LOCATION,
        afw_compile_type_script,
        NULL, x->p, x->xctx);

    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        listing = afw_function_evaluate_whitespace_parameter(x, 2);
        compiled = result;
        result = afw_value_create_unmanaged_string(
            afw_value_compiler_listing_to_string(compiled, listing,
                x->p, x->xctx),
            x->p, x->xctx);
        /* Listing is a copy in dest p. The unit lasts for dest p. */
        return result;
    }

    return result;
}



/*
 * Adaptive function: eval<script>
 *
 * afw_function_execute_eval_script
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Compile and evaluate script value.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function eval<script>(
 *       source: script,
 *       additionalUntrustedQualifiedVariables?: object // _AdaptiveTemplatePropertiesObjects_
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   source - (script) script to compile and evaluate.
 *
 *   additionalUntrustedQualifiedVariables - (optional object
 *       _AdaptiveTemplatePropertiesObjects_) This parameter supplies additional
 *       qualified variables that can be accessed during evaluation. These
 *       variables will not be used by anything that needs to ensure its
 *       qualified variables must come from a trusted source, such as
 *       authorization. This parameter is intended to be used for testing only
 *       and should not be used for anything running in production.
 *
 * Returns:
 *
 *   (any)
 */
const afw_value_t *
afw_function_execute_eval_script(
    afw_function_execute_t *x)
{
    const afw_value_script_t *script;
    const afw_value_t *compiled;
    const afw_value_t *value = NULL;
    char unexpected[192];

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(script, 1, script);

    /* compile dest is x->p (last-releases dest p). Evaluate dest is
     * x->p. After evaluate, get_reference the unit onto an escaped
     * function. */
    compiled = afw_compile_to_value(
        &script->internal, AFW_FUNCTION_SOURCE_LOCATION,
        afw_compile_type_script,
        NULL, x->p, x->xctx);

    {
        afw_xctx_t *xctx = x->xctx;

        AFW_TRY {
            if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
                value = afw_value_evaluate_with_additional_untrusted_qualified_variables(
                    compiled, x->argv[2], x->p, xctx);
            }
            else {
                value = afw_value_evaluate(compiled, x->p, xctx);
            }
        }
        AFW_FINALLY {
            value = afw_function_eval_reference_escaped_unit(
                compiled, value, unexpected, sizeof(unexpected),
                "eval<script>", x->p, xctx);
        }
        AFW_ENDTRY;

        if (unexpected[0]) {
            AFW_THROW_ERROR_FZ(general, xctx, "%s", unexpected);
        }
    }

    afw_xctx_statement_flow_reset_all_except_rethrow(x->xctx);
    return value;
}
