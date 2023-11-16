// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for regexp
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file regexp.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the regexp category.
 */

interface IAnyObject {
    [prop : string] : any;
}

/**
 * Compile regexp value and return either an unevaluated adaptive value or a
 * string containing the compiler listing.
 * 
 * @param {regexp} source - regexp string to compile
 * 
 * @param {} listing - If specified, a compiler listing is produced instead of
 *     an unevaluated compiled value.
 * 
 *     This parameter can be an integer between 0 and 10 of a string that is
 *     used for indentation. If 0 is specified, no whitespace is added to the
 *     resulting string. If 1 through 10 is specified, that number of spaces
 *     is used.
 * 
 * @returns {unevaluated}
 */
export function afwCompileRegexp(client : any, source : any, listing? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "compile<regexp>";
    _action["source"] = source;

    if (listing !== undefined)
        _action["listing"] = listing;

    return client.perform(_action);
}

/**
 * Determine if regexp arg1 is equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'eqx' ('===') instead
 * if you want false to be returned if arg1 and arg2's data type don't match.
 * 
 * @param {regexp} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqRegexp(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<regexp>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for regexp arg1 is equal to the value and data type of arg2
 * then return the boolean result. Use 'eq' ('==') instead if you want arg2 to
 * be converted to the data type of arg1 before comparison.
 * 
 * @param {regexp} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxRegexp(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<regexp>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for regexp arg1 is greater than or equal to regexp arg2 and return
 * the boolean result.
 * 
 * @param {regexp} arg1 -
 * 
 * @param {regexp} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeRegexp(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<regexp>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for regexp arg1 is greater than regexp arg2 and return the boolean
 * result.
 * 
 * @param {regexp} arg1 -
 * 
 * @param {regexp} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtRegexp(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<regexp>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for regexp arg1 is less than or equal to regexp arg2 and return the
 * boolean result.
 * 
 * @param {regexp} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeRegexp(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<regexp>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for regexp arg1 is less that regexp arg2 and return the boolean
 * result.
 * 
 * @param {regexp} arg1 -
 * 
 * @param {regexp} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtRegexp(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<regexp>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if regexp arg1 is not equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'nex' ('!==') instead
 * if you want true to be returned if arg1 and arg2's data type don't match.
 * 
 * @param {regexp} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeRegexp(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<regexp>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for regexp arg1 is not equal to the value or data type of arg2
 * then return the boolean result. Use 'ne' ('!=') instead if you want arg2 to
 * be converted to the data type of arg1 before comparison.
 * 
 * @param {regexp} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexRegexp(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<regexp>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Converts value to data type regexp returning regexp result.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {regexp} Converted value
 */
export function afwRegexp(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "regexp";
    _action["value"] = value;

    return client.perform(_action);
}

