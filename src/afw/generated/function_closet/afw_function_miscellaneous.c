// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for miscellaneous
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_miscellaneous.c
 * @brief afw_function_execute_* functions for miscellaneous.
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
 * Adaptive function: compare_uri
 *
 * afw_function_execute_compare_uri
 *
 * See afw_function_bindings.h for more information.
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
 *   uri1 - (string) First URI for compare. This URI can not contain an
 *       asterisk ("*").
 *
 *   uri2 - (string) Second URI for compare. This URI can contain asterisk
 *       ("*") for substitution if isValuePath is true and currentPath2 is
 *       specified.
 *
 *   isValuePath - (optional boolean) The URIs are adaptive value paths. If one
 *       of the URIs begins with a single slash "/", both must, and each URI
 *       will be parsed as an adaptive value path (example:
 *       /adaptorId/objectType/objectId.propertyNames).
 *
 *   currentPath - (optional string) If isValuePath is true, this is the
 *       current path that is used to resolve a relative path in the URIs. If
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
 * See afw_function_bindings.h for more information.
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
 *   ): null;
 * ```
 *
 * Parameters:
 *
 *   value - (any dataType) This is the value that will be converted to its
 *       string representation and written. An undefined value is represented
 *       by `<undefined>`.
 *
 *   detail - (optional boolean) If true, the string will only written if the
 *       debug:function_active:detail flag is on. If false or not specified,
 *       the string will only written if the debug:function_active flag is on.
 *
 * Returns:
 *
 *   (null)
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
 * See afw_function_bindings.h for more information.
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
 *   
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
 * See afw_function_bindings.h for more information.
 *
 * Return UTC dateTime when the execution context was created. This will
 * usually be the start of request time.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function execution_start_time_utc(
 *   
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
 * See afw_function_bindings.h for more information.
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
 *   
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
 * See afw_function_bindings.h for more information.
 *
 * Test value returning boolean True if it is not undefined.
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
 *   value - (any dataType) Value to check.
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
 * See afw_function_bindings.h for more information.
 *
 * Test value returning boolean True if it is null or undefined.
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
 *   value - (any dataType) Value to check.
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
 * See afw_function_bindings.h for more information.
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
 *   ): null;
 * ```
 *
 * Parameters:
 *
 *   value - (any dataType) Value to log.
 *
 * Returns:
 *
 *   (null)
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
 * See afw_function_bindings.h for more information.
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
 *   
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
 * See afw_function_bindings.h for more information.
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
 *   
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
 *       ...values_rest: (list of any)
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
 * Adaptive function: parse_uri
 *
 * afw_function_execute_parse_uri
 *
 * See afw_function_bindings.h for more information.
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
 *   ): (object _AdaptiveParsedURI_);
 * ```
 *
 * Parameters:
 *
 *   uri - (string) URI to parse.
 *
 *   isValuePath - (optional boolean) The URI is an adaptive value path. If the
 *       path begins with a single slash "/", the URI will be parsed as an
 *       adaptive value path (example:
 *       /adaptorId/objectType/objectId.propertyNames).
 *
 *   currentPath - (optional string) If isValuePath is true, this is the
 *       current path that is used to resolve relative paths. If isValuePath is
 *       not true, this parameter is ignored.
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
 * See afw_function_bindings.h for more information.
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
 *       request: (object _AdaptiveActions_)
 *   ): (object _AdaptiveResponse_);
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
 * Adaptive function: trace
 *
 * afw_function_execute_trace
 *
 * See afw_function_bindings.h for more information.
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
 *   ): null;
 * ```
 *
 * Parameters:
 *
 *   value - (any dataType) This is the value that will be converted to its
 *       string representation and written the trace log. An undefined value is
 *       represented by `<undefined>`.
 *
 *   filter - (optional boolean) If this optional filter is false, nothing will
 *       be written to the trace log. The default is true.
 *
 *   number - (optional integer) This is an optional number between 1 and 8
 *       that is appended to "trace" to identify the trace log. The default is
 *       1.
 *
 * Returns:
 *
 *   (null)
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
 * See afw_function_bindings.h for more information.
 *
 * Return the true if the named variable exists.
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
 *   name - (string) Name of variable to check. The name can optionally be
 *       preceded with a qualifier followed by "::".
 *
 * Returns:
 *
 *   (boolean) True if variable exists.
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
 * See afw_function_bindings.h for more information.
 *
 * Return the value of a variable. If variable is not available, return a
 * default or null value.
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
 *   name - (string) Name of variable to get. The name can optionally be
 *       preceded with a qualifier followed by "::".
 *
 *   defaultValue - (optional any dataType) The default value of variable if it
 *       does not exist in object. If not specified, null value is the default.
 *
 * Returns:
 *
 *   (any dataType) Evaluated variable value or default.
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
 * See afw_function_bindings.h for more information.
 *
 * Return the true if the named variable exists and is not null.
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
 *   name - (string) Name of variable to check. The name can optionally be
 *       preceded with a qualifier followed by "::".
 *
 * Returns:
 *
 *   (boolean) True if variable exists and is not null.
 */
const afw_value_t *
afw_function_execute_variable_is_not_null(
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
