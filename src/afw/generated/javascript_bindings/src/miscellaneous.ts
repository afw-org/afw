// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for miscellaneous
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file miscellaneous.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the miscellaneous category.
 */

interface IAnyObject {
    [prop : string] : any;
}

/**
 * Compare two URIs.
 * 
 * @param {string} uri1 - First URI for compare. This URI can not contain an
 *     asterisk ('*')
 * 
 * @param {string} uri2 - Second URI for compare. This URI can contain
 *     asterisk ('*') for substitution if isValuePath is true and currentPath2
 *     is specified.
 * 
 * @param {boolean} isValuePath - The URIs are adaptive value paths. If one of
 *     the URIs begins with a single slash '/', both must, and each URI will
 *     be parsed as an adaptive value path (example:
 *     /adapterId/objectType/objectId.propertyNames).
 * 
 * @param {string} currentPath - If isValuePath is true, this is the current
 *     path that is used to resolve a relative path in the URIs. If
 *     isValuePath is not true, this parameter is ignored.
 * 
 * @returns {boolean} Result of comparison.
 */
export function afwCompareUri(client : any, uri1 : string, uri2 : string, isValuePath? : boolean, currentPath? : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "compare_uri";
    _action["uri1"] = uri1;
    _action["uri2"] = uri2;

    if (isValuePath !== undefined)
        _action["isValuePath"] = isValuePath;

    if (currentPath !== undefined)
        _action["currentPath"] = currentPath;

    return client.perform(_action);
}

/**
 * Conditionally, based on the detail parameter, write a value as a string to
 * the debug file descriptor (usually stderr).
 * 
 * @param {} value - This is the value that will be converted to its string
 *     representation and written. An undefined value is represented by
 *     'undefined'.
 * 
 * @param {boolean} detail - If true, the string will only written if the
 *     debug:function_active:detail flag is on. If false or not specified, the
 *     string will only written if the debug:function_active flag is on.
 * 
 * @returns {void}
 */
export function afwDebug(client : any, value : any, detail? : boolean) : any {

    let _action : IAnyObject = {};

    _action["function"] = "debug";
    _action["value"] = value;

    if (detail !== undefined)
        _action["detail"] = detail;

    return client.perform(_action);
}

/**
 * Return local dateTime when the execution context was created. This will
 * usually be the start of request time.
 * 
 * @returns {dateTime}
 */
export function afwExecutionStartTimeLocal(client : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "execution_start_time_local";

    return client.perform(_action);
}

/**
 * Return UTC dateTime when the execution context was created. This will
 * usually be the start of request time.
 * 
 * @returns {dateTime}
 */
export function afwExecutionStartTimeUtc(client : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "execution_start_time_utc";

    return client.perform(_action);
}

/**
 * Generate a UUID.
 * 
 * @returns {string}
 */
export function afwGenerateUuid(client : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "generate_uuid";

    return client.perform(_action);
}

/**
 * Return true if the value is not undefined. Does not check whether a
 * variable name is bound — use variable_exists for that. null is defined.
 * 
 * @param {} value - Value to check.
 * 
 * @returns {boolean} True if value is not undefined.
 */
export function afwIsDefined(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_defined";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Return true if the value is null or undefined. Does not check whether a
 * variable name is bound — use variable_exists for that.
 * 
 * @param {} value - Value to check.
 * 
 * @returns {boolean} True if value is null or undefined.
 */
export function afwIsNullish(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_nullish";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Evaluate and convert value to String and log it.
 * 
 * @param {} value - Value to log.
 * 
 * @returns {void}
 */
export function afwLog(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "log";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Return current local dateTime.
 * 
 * @returns {dateTime}
 */
export function afwNowLocal(client : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "now_local";

    return client.perform(_action);
}

/**
 * Return current UTC dateTime.
 * 
 * @returns {dateTime}
 */
export function afwNowUtc(client : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "now_utc";

    return client.perform(_action);
}

/**
 * Parse a URI.
 * 
 * @param {string} uri - URI to parse
 * 
 * @param {boolean} isValuePath - The URI is an adaptive value path. If the
 *     path begins with a single slash '/', the URI will be parsed as an
 *     adaptive value path (example:
 *     /adapterId/objectType/objectId.propertyNames)
 * 
 * @param {string} currentPath - If isValuePath is true, this is the current
 *     path that is used to resolve relative paths. If isValuePath is not
 *     true, this parameter is ignored.
 * 
 * @returns {object} Object with results of parse.
 */
export function afwParseUri(client : any, uri : string, isValuePath? : boolean, currentPath? : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "parse_uri";
    _action["uri"] = uri;

    if (isValuePath !== undefined)
        _action["isValuePath"] = isValuePath;

    if (currentPath !== undefined)
        _action["currentPath"] = currentPath;

    return client.perform(_action);
}

/**
 * Perform actions right away.
 * 
 * @param {object} request - See /afw/_AdaptiveObjectType_/_AdaptiveActions_
 *     for more information.
 * 
 * @returns {object} Response object.
 */
export function afwPerform(client : any, request : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "perform";
    _action["request"] = request;

    return client.perform(_action);
}

/**
 * Return the environment running total of outstanding bytes AFW malloc/calloc
 * asked for and has not yet given back (heap/tracker free, or pool destroy).
 * Rounding and prefixes included. Not APR's private usage; see process_rss()
 * for current process RSS.
 * 
 * @returns {integer} Sum of all pools' bytes_allocated.
 */
export function afwPoolBytesInUse(client : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "pool_bytes_in_use";

    return client.perform(_action);
}

/**
 * Return the current resident set size of this process in kilobytes (Linux
 * /proc/self/statm). A hint at APR and OS usage; APR does not return pages to
 * the OS. Compare with pool_bytes_in_use() for AFW asked-for vs process RSS.
 * 
 * @returns {integer} Current RSS in kilobytes, or 0 if unavailable.
 */
export function afwProcessRss(client : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "process_rss";

    return client.perform(_action);
}

/**
 * Write a value to a trace log.
 * 
 * @param {} value - This is the value that will be converted to its string
 *     representation and written the trace log. An undefined value is
 *     represented by 'undefined'.
 * 
 * @param {boolean} filter - If this optional filter is false, nothing will be
 *     written to the trace log. The default is true.
 * 
 * @param {integer} number - This is an optional number between 1 and 8 that
 *     is appended to 'trace' to identify the trace log. The default is 1.
 * 
 * @returns {void}
 */
export function afwTrace(client : any, value : any, filter? : boolean, number? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "trace";
    _action["value"] = value;

    if (filter !== undefined)
        _action["filter"] = filter;

    if (number !== undefined)
        _action["number"] = number;

    return client.perform(_action);
}

/**
 * Return true if the named variable is bound: a lexical symbol in the current
 * scope chain, or a name defined on a visible qualifier frame. Still true
 * when the value is undefined (including an uninitialized let) or null. False
 * only when the name is not bound. Use is_defined / is_nullish for the value.
 * 
 * @param {string} name - Name of variable to check. Optionally
 *     qualifier::name.
 * 
 * @returns {boolean} True if the name is bound.
 */
export function afwVariableExists(client : any, name : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "variable_exists";
    _action["name"] = name;

    return client.perform(_action);
}

/**
 * Return the value of a bound variable. Optional default applies only when
 * the name is not bound — not when the value is undefined. If unbound and no
 * default is given, the result is undefined. The default is the evaluated
 * value at that moment (identity).
 * 
 * @param {string} name - Name of variable to get. Optionally qualifier::name.
 * 
 * @param {} defaultValue - Value to return only if the name is not bound. The
 *     evaluated value at that moment (identity).
 * 
 * @returns {} Bound variable value, or default / undefined if unbound.
 */
export function afwVariableGet(client : any, name : string, defaultValue? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "variable_get";
    _action["name"] = name;

    if (defaultValue !== undefined)
        _action["defaultValue"] = defaultValue;

    return client.perform(_action);
}

/**
 * Return true if the named variable is bound and its value is not Adaptive
 * null. Undefined (including an uninitialized let) counts as not null. False
 * if the name is not bound or the value is null. This is not the same as
 * is_defined or not is_nullish.
 * 
 * @param {string} name - Name of variable to check. Optionally
 *     qualifier::name.
 * 
 * @returns {boolean} True if bound and value is not Adaptive null.
 */
export function afwVariableIsNotNull(client : any, name : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "variable_is_not_null";
    _action["name"] = name;

    return client.perform(_action);
}

