// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for miscellaneous
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_miscellaneous.c
 * @brief Adaptive function execute implementations for category `miscellaneous`.
 */

#include "afw_internal.h"



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
    const afw_value_string_t *uri1;
    const afw_value_string_t *uri2;
    const afw_value_boolean_t *isValuePath;
    const afw_value_string_t *currentPath;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(uri1, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(uri2, 2, string);

    isValuePath = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(isValuePath, 3, boolean);
    }

    currentPath = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(4)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(currentPath, 4, string);
    }

    return
        afw_uri_are_equivalent(
            &uri1->internal,
            &uri2->internal,
            (isValuePath) ? isValuePath->internal: false,
            (currentPath) ? &currentPath->internal: NULL,
            x->p, x->xctx)
        ? afw_boolean_v_true
        : afw_boolean_v_false;
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
    const afw_value_t *value;
    const afw_value_boolean_t *detail;
    const afw_utf8_t *s;
    afw_size_t flag_index;

    value = afw_value_evaluate(x->argv[1], x->p, x->xctx);
    value = afw_value_convert_to_string(value, true, x->p, x->xctx);
    s = AFW_VALUE_INTERNAL(value);

    detail = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(detail, 2, boolean);
    }

    flag_index = (detail && detail->internal)
        ? x->xctx->env->flag_index_debug_function_active_detail
        : x->xctx->env->flag_index_debug_function_active;

    afw_debug(flag_index, NULL, s, x->xctx);

    /* Return undefined for void. */
    return afw_value_undefined;
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
    const afw_utf8_t *uuid;

    uuid = afw_uuid_create_utf8(x->p, x->xctx);
    
    return afw_value_create_unmanaged_string(uuid, x->p, x->xctx);
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
    /** @fixme Add code. */
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
    return afw_value_create_dateTime_now_local(x->p, x->xctx);
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
    return afw_value_create_dateTime_now_utc(x->p, x->xctx);
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
    const afw_value_string_t *uri;
    const afw_object_t *parsed;
    const afw_value_boolean_t *isValuePath;
    const afw_value_string_t *currentPath;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(uri, 1, string);

    isValuePath = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(isValuePath,
            2, boolean);
    }

    currentPath = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(currentPath,
            3, string);
    }

    parsed = afw_uri_to_object(
        &uri->internal,
        (isValuePath) ? isValuePath->internal : false,
        (currentPath) ? &currentPath->internal : NULL,
        x->p, x->xctx);

    return afw_value_create_unmanaged_object(parsed, x->p, x->xctx);
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
    const afw_object_t *response;
    const afw_value_object_t *request;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(request, 1, object);
   
    response = afw_action_perform(request->internal, NULL, NULL,
        x->p, x->xctx);
    return afw_value_create_unmanaged_object(response, x->p, x->xctx);
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
    return afw_value_create_unmanaged_dateTime(
        &x->xctx->local_dateTime_when_created, x->p, x->xctx);
}



/*
 * Adaptive function: execution_start_time_utc
 *
 * afw_function_execute_execution_start_time_utc
 *
 * See afw_function_bindings.h for more information.
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
    return afw_value_create_unmanaged_dateTime(
        &x->xctx->utc_dateTime_when_created, x->p, x->xctx);
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
    const afw_value_t *value;
    const afw_value_boolean_t *filter;
    const afw_value_integer_t *number;
    const afw_utf8_t *s;
    afw_log_priority_t priority;

    /* If parameter 2 is present and false, skip writing value. */
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(filter, 2, boolean);
        if (!filter->internal) {
            return afw_value_undefined;
        }
    }

    /* Determine log priority. */
    priority = afw_log_priority_trace1;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(number, 3, integer);
        if (number->internal < 1 || number->internal > 8) {
            AFW_THROW_ERROR_Z(general,
                "Parameter 3 must be an integer between 1 and 8.", x->xctx);
        }
        priority = (afw_log_priority_t)
            (number->internal - 1 + afw_log_priority_trace1);
    }

    /* Get value as a string. */
    value = afw_value_evaluate(x->argv[1], x->p, x->xctx);
    if (!afw_value_is_string(value)) {
        value = afw_value_convert_to_string(value, true, x->p, x->xctx);
    }
    s = AFW_VALUE_INTERNAL(value);

    /* Write string to trace log. */
    afw_trace_write(priority, NULL, AFW__FILE_LINE__, s, x->xctx);

    /* Return undefined for void. */
    return afw_value_undefined;
}



/*
 * Adaptive function: variable_is_not_null
 *
 * afw_function_execute_variable_is_not_null
 *
 * See afw_function_bindings.h for more information.
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
    const afw_value_string_t *qualified_name;
    afw_utf8_t qualifier;
    afw_utf8_t name;
    const afw_value_t *value;
    const afw_value_t **value_address;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(qualified_name, 1, string);

    afw_compile_split_qualified_name(&qualified_name->internal,
        &qualifier, &name, x->xctx);

    /* Unqualified: symbol must be bound; empty slot is undefined, not null. */
    if (qualifier.len == 0) {
        value_address = afw_xctx_scope_symbol_get_value_address_by_name(
            &name, x->xctx);
        if (!value_address) {
            return afw_boolean_v_false;
        }
        value = *value_address;
        return afw_value_is_null(value)
            ? afw_boolean_v_false
            : afw_boolean_v_true;
    }

    value = afw_xctx_get_optionally_qualified_variable(&qualifier, &name,
        x->xctx);
    /* Qualified: C NULL means not defined on any frame. */
    if (!value) {
        return afw_boolean_v_false;
    }
    return afw_value_is_null(value)
        ? afw_boolean_v_false
        : afw_boolean_v_true;
}



/*
 * Adaptive function: is_nullish
 *
 * afw_function_execute_is_nullish
 *
 * See afw_function_bindings.h for more information.
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
    const afw_value_t *value;

    AFW_FUNCTION_EVALUATE_PARAMETER(value, 1);

    return afw_value_is_nullish(value) ? afw_boolean_v_true : afw_boolean_v_false;
}



/*
 * Adaptive function: is_defined
 *
 * afw_function_execute_is_defined
 *
 * See afw_function_bindings.h for more information.
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
    const afw_value_t *value;

    AFW_FUNCTION_EVALUATE_PARAMETER(value, 1);

    return value ? afw_boolean_v_true : afw_boolean_v_false;
}



/*
 * Adaptive function: variable_exists
 *
 * afw_function_execute_variable_exists
 *
 * See afw_function_bindings.h for more information.
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
    const afw_value_string_t *qualified_name;
    afw_utf8_t qualifier;
    afw_utf8_t name;
    const afw_value_t *value;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(qualified_name, 1, string);

    afw_compile_split_qualified_name(&qualified_name->internal,
        &qualifier, &name, x->xctx);

    /* Unqualified: symbol present, even if slot is still C NULL. */
    if (qualifier.len == 0) {
        return afw_xctx_scope_symbol_exists_by_name(&name, x->xctx)
            ? afw_boolean_v_true
            : afw_boolean_v_false;
    }

    /*
     * Qualified: get_cb non-NULL (including afw_value_undefined / null) means
     * defined on a frame; C NULL means not defined (keep walking / missing).
     */
    value = afw_xctx_get_optionally_qualified_variable(&qualifier, &name,
        x->xctx);
    return value ? afw_boolean_v_true : afw_boolean_v_false;
}



/*
 * Adaptive function: variable_get
 *
 * afw_function_execute_variable_get
 *
 * See afw_function_bindings.h for more information.
 *
 * Return the value of a bound variable. Optional default applies only when the
 * name is not bound — not when the value is undefined. If unbound and no
 * default is given, the result is undefined. Mutable defaults are cloned.
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
 *       bound. Cloned when used.
 *
 * Returns:
 *
 *   (any) Bound variable value, or default / undefined if unbound.
 */
const afw_value_t *
afw_function_execute_variable_get(
    afw_function_execute_t *x)
{
    const afw_value_string_t *qualified_name;
    afw_utf8_t qualifier;
    afw_utf8_t name;
    const afw_value_t *value;
    const afw_value_t **value_address;
    afw_boolean_t bound;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(qualified_name, 1, string);

    afw_compile_split_qualified_name(&qualified_name->internal,
        &qualifier, &name, x->xctx);

    bound = false;
    value = NULL;

    if (qualifier.len == 0) {
        value_address = afw_xctx_scope_symbol_get_value_address_by_name(
            &name, x->xctx);
        if (value_address) {
            bound = true;
            value = *value_address;
            if (!value) {
                value = afw_value_undefined;
            }
        }
    }
    else {
        value = afw_xctx_get_optionally_qualified_variable(
            &qualifier, &name, x->xctx);
        if (value) {
            bound = true;
        }
    }

    if (!bound) {
        value = afw_value_undefined;
        if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
            value = afw_value_evaluate(x->argv[2], x->p, x->xctx);
            value = afw_value_clone(value, x->p, x->xctx);
        }
    }

    return value;
}
