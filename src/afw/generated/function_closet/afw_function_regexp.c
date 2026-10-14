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
 * See afw_function_bindings_internal.h for more information.
 *
 * Compile regexp value and return either an unevaluated adaptive value or a
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
afw_function_execute_compile_regexp(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: regexp_escape
 *
 * afw_function_execute_regexp_escape
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Returns value with every character that could have a meaning in a regular
 * expression escaped, so that the result matches value itself, as ECMAScript's
 * RegExp.escape(s) does. Use it to put data in a pattern: to_regexp("^" +
 * regexp_escape(name) + "\\z"). ASCII letters, digits, _ and characters above
 * ASCII are kept, except a leading digit and white space, which are written as
 * \x{hh}, as are control characters; every other ASCII character gets a
 * backslash. The result means the same with any flags.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function regexp_escape(
 *       value: string
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   value - (string) The text to escape.
 *
 * Returns:
 *
 *   (string) The escaped text.
 */
const afw_value_t *
afw_function_execute_regexp_escape(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: test
 *
 * afw_function_execute_test
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Checks whether the regular expression matches anywhere in value and returns
 * the boolean result, as ECMAScript's re.test(s) does: re->test(s). Anchor with
 * ^ and \z to match the whole value. value can be any value whose data type
 * holds text, such as string, anyURI, dnsName, ipAddress, rfc822Name and
 * x500Name.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function test(
 *       regexp: regexp,
 *       value: any
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   regexp - (regexp) The regular expression. A regexp literal (/a+b/ or
 *       "a+b"re), to_regexp(s), or `compile<regexp>(r`) is already compiled;
 *       any other regexp value is compiled for this call.
 *
 *   value - (any) The value to search. Its data type must hold text.
 *
 * Returns:
 *
 *   (boolean) True if the regular expression matches anywhere in value.
 */
const afw_value_t *
afw_function_execute_test(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
