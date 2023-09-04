// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for miscellaneous
 *
 * Copyright (c) 2010-2023 Clemson University
 *
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
 *     /adaptorId/objectType/objectId.propertyNames).
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
 *     `<undefined>`.
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
 * Test value returning boolean True if it is not undefined.
 * 
 * @param {} value - Value to check
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
 * Test value returning boolean True if it is null or undefined.
 * 
 * @param {} value - Value to check
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
 *     /adaptorId/objectType/objectId.propertyNames)
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
 * Write a value to a trace log.
 * 
 * @param {} value - This is the value that will be converted to its string
 *     representation and written the trace log. An undefined value is
 *     represented by `<undefined>`.
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
 * Return the true if the named variable exists.
 * 
 * @param {string} name - Name of variable to check. The name can optionally
 *     be preceded with a qualifier followed by '::'.
 * 
 * @returns {boolean} True if variable exists.
 */
export function afwVariableExists(client : any, name : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "variable_exists";
    _action["name"] = name;

    return client.perform(_action);
}

/**
 * Return the value of a variable. If variable is not available, return a
 * default or null value.
 * 
 * @param {string} name - Name of variable to get. The name can optionally be
 *     preceded with a qualifier followed by '::'.
 * 
 * @param {} defaultValue - The default value of variable if it does not exist
 *     in object. If not specified, null value is the default.
 * 
 * @returns {} Evaluated variable value or default.
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
 * Return the true if the named variable exists and is not null.
 * 
 * @param {string} name - Name of variable to check. The name can optionally
 *     be preceded with a qualifier followed by '::'.
 * 
 * @returns {boolean} True if variable exists and is not null.
 */
export function afwVariableIsNotNull(client : any, name : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "variable_is_not_null";
    _action["name"] = name;

    return client.perform(_action);
}

