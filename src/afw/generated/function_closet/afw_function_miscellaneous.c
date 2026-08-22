// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for miscellaneous
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_miscellaneous.c
 * @brief afw_function_execute_* functions for miscellaneous.
 */

#include "afw.h"



/*
 * Adaptive function: compare_uri
 *
 * afw_function_execute_compare_uri
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Compare two URIs.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function compare_uri(
 *       uri1: string,
 *       uri2: string,
 *       isValuePath?: boolean,
 *       currentPath?: string
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   uri1 - (string) First URI for compare. This URI can not contain an asterisk
 *       ('*').
 *
 *   uri2 - (string) Second URI for compare. This URI can contain asterisk ('*')
 *       for substitution if isValuePath is true and currentPath2 is specified.
 *
 *   isValuePath - (optional boolean) The URIs are adaptive value paths. If one
 *       of the URIs begins with a single slash '/', both must, and each URI
 *       will be parsed as an adaptive value path (example:
 *       /adapterId/objectType/objectId.propertyNames).
 *
 *   currentPath - (optional string) If isValuePath is true, this is the current
 *       path that is used to resolve a relative path in the URIs. If
 *       isValuePath is not true, this parameter is ignored.
 *
 * Returns:
 *
 *   (boolean) Result of comparison.
 */
const afw_value_t *
afw_function_execute_compare_uri(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: debug
 *
 * afw_function_execute_debug
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Conditionally, based on the detail parameter, write a value as a string to
 * the debug file descriptor (usually stderr).
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function debug(
 *       value: any,
 *       detail?: boolean
 *   ): void;
 * ```
 *
 * Parameters:
 *
 *   value - (any) This is the value that will be converted to its string
 *       representation and written. An undefined value is represented by
 *       'undefined'.
 *
 *   detail - (optional boolean) If true, the string will only written if the
 *       debug:function_active:detail flag is on. If false or not specified, the
 *       string will only written if the debug:function_active flag is on.
 *
 * Returns:
 *
 *   (void)
 */
const afw_value_t *
afw_function_execute_debug(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: execution_start_time_local
 *
 * afw_function_execute_execution_start_time_local
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return local dateTime when the execution context was created. This will
 * usually be the start of request time.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function execution_start_time_local(
 *   ): dateTime;
 * ```
 *
 * Parameters:
 *
 * Returns:
 *
 *   (dateTime)
 */
const afw_value_t *
afw_function_execute_execution_start_time_local(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: execution_start_time_utc
 *
 * afw_function_execute_execution_start_time_utc
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return UTC dateTime when the execution context was created. This will usually
 * be the start of request time.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function execution_start_time_utc(
 *   ): dateTime;
 * ```
 *
 * Parameters:
 *
 * Returns:
 *
 *   (dateTime)
 */
const afw_value_t *
afw_function_execute_execution_start_time_utc(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: generate_uuid
 *
 * afw_function_execute_generate_uuid
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Generate a UUID.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function generate_uuid(
 *   ): string;
 * ```
 *
 * Parameters:
 *
 * Returns:
 *
 *   (string)
 */
const afw_value_t *
afw_function_execute_generate_uuid(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: is_defined
 *
 * afw_function_execute_is_defined
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return true if the value is not undefined. Does not check whether a variable
 * name is bound — use variable_exists for that. null is defined.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function is_defined(
 *       value: any
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   value - (any) Value to check.
 *
 * Returns:
 *
 *   (boolean) True if value is not undefined.
 */
const afw_value_t *
afw_function_execute_is_defined(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: is_nullish
 *
 * afw_function_execute_is_nullish
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return true if the value is null or undefined. Does not check whether a
 * variable name is bound — use variable_exists for that.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function is_nullish(
 *       value: any
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   value - (any) Value to check.
 *
 * Returns:
 *
 *   (boolean) True if value is null or undefined.
 */
const afw_value_t *
afw_function_execute_is_nullish(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: log
 *
 * afw_function_execute_log
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Evaluate and convert value to String and log it.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function log(
 *       value: any
 *   ): void;
 * ```
 *
 * Parameters:
 *
 *   value - (any) Value to log.
 *
 * Returns:
 *
 *   (void)
 */
const afw_value_t *
afw_function_execute_log(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: now_local
 *
 * afw_function_execute_now_local
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return current local dateTime.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function now_local(
 *   ): dateTime;
 * ```
 *
 * Parameters:
 *
 * Returns:
 *
 *   (dateTime)
 */
const afw_value_t *
afw_function_execute_now_local(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: now_utc
 *
 * afw_function_execute_now_utc
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return current UTC dateTime.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function now_utc(
 *   ): dateTime;
 * ```
 *
 * Parameters:
 *
 * Returns:
 *
 *   (dateTime)
 */
const afw_value_t *
afw_function_execute_now_utc(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: parse_uri
 *
 * afw_function_execute_parse_uri
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Parse a URI.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function parse_uri(
 *       uri: string,
 *       isValuePath?: boolean,
 *       currentPath?: string
 *   ): object; // _AdaptiveParsedURI_
 * ```
 *
 * Parameters:
 *
 *   uri - (string) URI to parse.
 *
 *   isValuePath - (optional boolean) The URI is an adaptive value path. If the
 *       path begins with a single slash '/', the URI will be parsed as an
 *       adaptive value path (example:
 *       /adapterId/objectType/objectId.propertyNames).
 *
 *   currentPath - (optional string) If isValuePath is true, this is the current
 *       path that is used to resolve relative paths. If isValuePath is not
 *       true, this parameter is ignored.
 *
 * Returns:
 *
 *   (object _AdaptiveParsedURI_) Object with results of parse.
 */
const afw_value_t *
afw_function_execute_parse_uri(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: perform
 *
 * afw_function_execute_perform
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Perform actions right away.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function perform(
 *       request: object // _AdaptiveActions_
 *   ): object; // _AdaptiveResponse_
 * ```
 *
 * Parameters:
 *
 *   request - (object _AdaptiveActions_) See
 *       /afw/_AdaptiveObjectType_/_AdaptiveActions_ for more information.
 *
 * Returns:
 *
 *   (object _AdaptiveResponse_) Response object.
 */
const afw_value_t *
afw_function_execute_perform(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: pool_bytes_in_use
 *
 * afw_function_execute_pool_bytes_in_use
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the environment running total of outstanding bytes AFW malloc/calloc
 * asked for and has not yet given back (heap/tracker free, or pool destroy).
 * Rounding and prefixes included. Not APR's private usage; see process_rss()
 * for current process RSS.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function pool_bytes_in_use(
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 * Returns:
 *
 *   (integer) Sum of all pools' bytes_allocated.
 */
const afw_value_t *
afw_function_execute_pool_bytes_in_use(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: process_rss
 *
 * afw_function_execute_process_rss
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the current resident set size of this process in kilobytes (Linux
 * /proc/self/statm). A hint at APR and OS usage; APR does not return pages to
 * the OS. Compare with pool_bytes_in_use() for AFW asked-for vs process RSS.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function process_rss(
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 * Returns:
 *
 *   (integer) Current RSS in kilobytes, or 0 if unavailable.
 */
const afw_value_t *
afw_function_execute_process_rss(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: trace
 *
 * afw_function_execute_trace
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Write a value to a trace log.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function trace(
 *       value: any,
 *       filter?: boolean,
 *       number?: integer
 *   ): void;
 * ```
 *
 * Parameters:
 *
 *   value - (any) This is the value that will be converted to its string
 *       representation and written the trace log. An undefined value is
 *       represented by 'undefined'.
 *
 *   filter - (optional boolean) If this optional filter is false, nothing will
 *       be written to the trace log. The default is true.
 *
 *   number - (optional integer) This is an optional number between 1 and 8 that
 *       is appended to 'trace' to identify the trace log. The default is 1.
 *
 * Returns:
 *
 *   (void)
 */
const afw_value_t *
afw_function_execute_trace(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: variable_exists
 *
 * afw_function_execute_variable_exists
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return true if the named variable is bound: a lexical symbol in the current
 * scope chain, or a name defined on a visible qualifier frame. Still true when
 * the value is undefined (including an uninitialized let) or null. False only
 * when the name is not bound. Use is_defined / is_nullish for the value.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function variable_exists(
 *       name: string
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   name - (string) Name of variable to check. Optionally qualifier::name.
 *
 * Returns:
 *
 *   (boolean) True if the name is bound.
 */
const afw_value_t *
afw_function_execute_variable_exists(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: variable_get
 *
 * afw_function_execute_variable_get
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the value of a bound variable. Optional default applies only when the
 * name is not bound — not when the value is undefined. If unbound and no
 * default is given, the result is undefined. Object/array defaults get a
 * mutable memory face (issues #110 / #17); other defaults are cloned.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function variable_get(
 *       name: string,
 *       defaultValue?: any
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   name - (string) Name of variable to get. Optionally qualifier::name.
 *
 *   defaultValue - (optional any) Value to return only if the name is not
 *       bound. Isolated when used (object/array face; otherwise clone).
 *
 * Returns:
 *
 *   (any) Bound variable value, or default / undefined if unbound.
 */
const afw_value_t *
afw_function_execute_variable_get(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: variable_is_not_null
 *
 * afw_function_execute_variable_is_not_null
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return true if the named variable is bound and its value is not Adaptive
 * null. Undefined (including an uninitialized let) counts as not null. False if
 * the name is not bound or the value is null. This is not the same as
 * is_defined or not is_nullish.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function variable_is_not_null(
 *       name: string
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   name - (string) Name of variable to check. Optionally qualifier::name.
 *
 * Returns:
 *
 *   (boolean) True if bound and value is not Adaptive null.
 */
const afw_value_t *
afw_function_execute_variable_is_not_null(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
