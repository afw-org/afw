// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for relaxed_json
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_relaxed_json.c
 * @brief afw_function_execute_* functions for relaxed_json.
 */

#include "afw.h"



/*
 * Adaptive function: compile<relaxed_json>
 *
 * afw_function_execute_compile_relaxed_json
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Compile relaxed_json value and return either an unevaluated adaptive value or
 * a string containing the compiler listing. The listing is a human-oriented
 * dump (value tree interleaved with source, plus ---Symbols tables) for Fiddle
 * and debugging — not pure JSON (use stringify) and not Adaptive compiled-form
 * text (use decompile).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function compile<relaxed_json>(
 *       source: relaxed_json,
 *       listing?: any
 *   ): unevaluated;
 * ```
 *
 * Parameters:
 *
 *   source - (relaxed_json) relaxed_json string to compile.
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
afw_function_execute_compile_relaxed_json(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
