// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for template
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file template.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the template category.
 */

interface IAnyObject {
    [prop : string] : any;
}

/**
 * Construct a new array of template from the given values, the same as
 * array() except that the new array is typed: each value, and each entry of a
 * spread array, must be dataType template, and adding a value of another data
 * type to the array later throws. A value of another data type is an error,
 * never converted. This is usually written `array<template>(`...values).
 * 
 * @param {} values - A value of dataType template, or ... followed by an
 *     expression that results in an array, every entry of which is included.
 * 
 * @returns {array} The constructed array of template.
 */
export function afwArrayTemplate(client : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "array_template";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Returns value unchanged if it is dataType template. Otherwise, including
 * when value is undefined, throws an error. It never converts: to get a
 * template from a value of another data type, write a typed literal or call a
 * to_T conversion function where one exists. This is usually written value as
 * template.
 * 
 * @param {} value - Value to assert
 * 
 * @returns {template} value, which is dataType template
 */
export function afwAsTemplate(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "as_template";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Compile template value and return either an unevaluated adaptive value or a
 * string containing the compiler listing. The listing is a human-oriented
 * dump (value tree interleaved with source, plus ---Symbols tables) for
 * Fiddle and debugging — not pure JSON (use stringify) and not Adaptive
 * compiled-form text (use decompile).
 * 
 * @param {template} source - template string to compile
 * 
 * @param {} listing - If specified, a human compiler listing is produced
 *     instead of an unevaluated compiled value (tree + ---Symbols; not
 *     recompilable). Use decompile() for Adaptive compiled-form text and
 *     stringify() for pure JSON of evaluated data.
 * 
 *     This parameter can be an integer between 0 and 10 or a string that is
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
 * @param {template} source - template to compile and evaluate
 * 
 * @param {object} additionalUntrustedQualifiedVariables - This parameter
 *     supplies additional qualified variables that can be accessed during
 *     evaluation. These variables will not be used by anything that needs to
 *     ensure its qualified variables must come from a trusted source, such as
 *     authorization. This parameter is intended to be used for testing only
 *     and should not be used for anything running in production.
 * 
 * @returns {any}
 */
export function afwEvalTemplate(client : any, source : any, additionalUntrustedQualifiedVariables? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eval<template>";
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
 * Checks whether value is dataType template and returns the boolean result.
 * It is false for undefined. This can also be written `is<template>(value`).
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsTemplate(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_template";
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
 * Converts value to data type template returning template result. Holds
 * template source text as a template value (does not compile). Use
 * polymorphic compile to compile the source.
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

