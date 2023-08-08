// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for expression
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Takes any number of expression values and returns a list of list.
 * 
 * @param {list} values -
 * 
 * @returns {list}
 */
export function afwBagExpression(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<expression>";

    if (values !== undefined)
        _action["values"] = values;

    return client.perform(_action);
}

/**
 * This returns the integer number of values in list.
 * 
 * @param {list} value -
 * 
 * @returns {integer}
 */
export function afwBagSizeExpression(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size<expression>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Compile expression value and return either an unevaluated adaptive value
 * or a string containing the compiler listing.
 * 
 * @param {expression} source - expression string to compile
 * 
 * @param {} listing - If specified, a compiler listing is produced instead
 *     of an unevaluated expression value.
 *     
 *     This parameter can be an integer between 0 and 10 of a string that is
 *     used for indentation. If 0 is specified, no whitespace is added to the
 *     resulting string. If 1 through 10 is specified, that number of spaces
 *     is used.
 * 
 * @returns {unevaluated}
 */
export function afwCompileExpression(client : any, source : any, listing? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "compile<expression>";
    _action["source"] = source;

    if (listing !== undefined)
        _action["listing"] = listing;

    return client.perform(_action);
}

/**
 * Determine if expression arg1 is equal to the value of arg2 converted to
 * the data type of arg1 then return the boolean result. Use 'eqx' ('===')
 * instead if you want false to be returned if arg1 and arg2's data type
 * don't match.
 * 
 * @param {expression} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqExpression(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<expression>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for expression arg1 is equal to the value and data type of
 * arg2 then return the boolean result. Use 'eq' ('==') instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {expression} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxExpression(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<expression>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Compile and evaluate expression value.
 * 
 * @param {expression} source - expression string to compile and evaluate
 * 
 * @param {object} additionalUntrustedQualifiedVariables - This parameter
 *     supplies additional qualified variables that can be accessed during
 *     evaluation. These variables will not be used by anything that needs to
 *     ensure its qualified variables must come from a trusted source, such
 *     as authorization. This parameter is intended to be used for testing
 *     only and should not be used for anything running in production.
 * 
 * @returns {unevaluated}
 */
export function afwEvaluateExpression(client : any, source : any, additionalUntrustedQualifiedVariables? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "evaluate<expression>";
    _action["source"] = source;

    if (additionalUntrustedQualifiedVariables !== undefined)
        _action["additionalUntrustedQualifiedVariables"] = additionalUntrustedQualifiedVariables;

    return client.perform(_action);
}

/**
 * Converts value to data type expression returning expression result.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {expression} Converted value
 */
export function afwExpression(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "expression";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks for expression arg1 is greater than or equal to expression arg2 and
 * return the boolean result.
 * 
 * @param {expression} arg1 -
 * 
 * @param {expression} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeExpression(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<expression>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for expression arg1 is greater than expression arg2 and return the
 * boolean result.
 * 
 * @param {expression} arg1 -
 * 
 * @param {expression} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtExpression(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<expression>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType expression and return the boolean result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsExpression(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is<expression>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks for expression arg1 is less than or equal to expression arg2 and
 * return the boolean result.
 * 
 * @param {expression} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeExpression(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<expression>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for expression arg1 is less that expression arg2 and return the
 * boolean result.
 * 
 * @param {expression} arg1 -
 * 
 * @param {expression} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtExpression(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<expression>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if expression arg1 is not equal to the value of arg2 converted
 * to the data type of arg1 then return the boolean result. Use 'nex' ('!==')
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 * 
 * @param {expression} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeExpression(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<expression>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for expression arg1 is not equal to the value or data type of
 * arg2 then return the boolean result. Use 'ne' ('!=') instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {expression} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexExpression(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<expression>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Converts expression value to string. For list values, the to_string()
 * value for each entry is returned separated with commas.
 * 
 * @param {expression} value - A expression value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringExpression(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<expression>";
    _action["value"] = value;

    return client.perform(_action);
}

