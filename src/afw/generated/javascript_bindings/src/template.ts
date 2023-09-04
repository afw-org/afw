// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for template
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * This returns the integer number of values in array.
 * 
 * @param {array} value -
 * 
 * @returns {integer}
 */
export function afwBagSizeTemplate(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size<template>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Takes any number of template values and returns an array of array.
 * 
 * @param {array} values -
 * 
 * @returns {array}
 */
export function afwBagTemplate(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<template>";

    if (values !== undefined)
        _action["values"] = values;

    return client.perform(_action);
}

/**
 * Compile template value and return either an unevaluated adaptive value or a
 * string containing the compiler listing.
 * 
 * @param {template} source - template string to compile
 * 
 * @param {} listing - If specified, a compiler listing is produced instead of
 *     an unevaluated expression value.
 * 
 *     This parameter can be an integer between 0 and 10 of a string that is
 *     used for indentation. If 0 is specified, no whitespace is added to the
 *     resulting string. If 1 through 10 is specified, that number of spaces
 *     is used.
 * 
 * @returns {unevaluated}
 */
export function afwCompileTemplate(client : any, source : any, listing? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "compile<template>";
    _action["source"] = source;

    if (listing !== undefined)
        _action["listing"] = listing;

    return client.perform(_action);
}

/**
 * Determine if template arg1 is equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'eqx' ('===') instead
 * if you want false to be returned if arg1 and arg2's data type don't match.
 * 
 * @param {template} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqTemplate(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<template>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for template arg1 is equal to the value and data type of arg2
 * then return the boolean result. Use 'eq' ('==') instead if you want arg2 to
 * be converted to the data type of arg1 before comparison.
 * 
 * @param {template} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxTemplate(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<template>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Compile and evaluate template value.
 * 
 * @param {template} source - template string to compile and evaluate
 * 
 * @param {object} additionalUntrustedQualifiedVariables - This parameter
 *     supplies additional qualified variables that can be accessed during
 *     evaluation. These variables will not be used by anything that needs to
 *     ensure its qualified variables must come from a trusted source, such as
 *     authorization. This parameter is intended to be used for testing only
 *     and should not be used for anything running in production.
 * 
 * @returns {unevaluated}
 */
export function afwEvaluateTemplate(client : any, source : any, additionalUntrustedQualifiedVariables? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "evaluate<template>";
    _action["source"] = source;

    if (additionalUntrustedQualifiedVariables !== undefined)
        _action["additionalUntrustedQualifiedVariables"] = additionalUntrustedQualifiedVariables;

    return client.perform(_action);
}

/**
 * Checks for template arg1 is greater than or equal to template arg2 and
 * return the boolean result.
 * 
 * @param {template} arg1 -
 * 
 * @param {template} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeTemplate(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<template>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for template arg1 is greater than template arg2 and return the
 * boolean result.
 * 
 * @param {template} arg1 -
 * 
 * @param {template} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtTemplate(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<template>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType template and return the boolean result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsTemplate(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is<template>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks for template arg1 is less than or equal to template arg2 and return
 * the boolean result.
 * 
 * @param {template} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeTemplate(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<template>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for template arg1 is less that template arg2 and return the boolean
 * result.
 * 
 * @param {template} arg1 -
 * 
 * @param {template} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtTemplate(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<template>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if template arg1 is not equal to the value of arg2 converted to
 * the data type of arg1 then return the boolean result. Use 'nex' ('!==')
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 * 
 * @param {template} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeTemplate(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<template>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for template arg1 is not equal to the value or data type of
 * arg2 then return the boolean result. Use 'ne' ('!=') instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {template} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexTemplate(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<template>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Converts value to data type template returning template result.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {template} Converted value
 */
export function afwTemplate(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "template";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts template value to string. For array values, the to_string() value
 * for each entry is returned separated with commas.
 * 
 * @param {template} value - A template value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringTemplate(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<template>";
    _action["value"] = value;

    return client.perform(_action);
}

