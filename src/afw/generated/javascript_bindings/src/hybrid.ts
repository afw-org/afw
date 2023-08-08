// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for hybrid
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Takes any number of hybrid values and returns a list of list.
 * 
 * @param {list} values -
 * 
 * @returns {list}
 */
export function afwBagHybrid(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<hybrid>";

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
export function afwBagSizeHybrid(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size<hybrid>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Compile hybrid value and return either an unevaluated adaptive value or a
 * string containing the compiler listing.
 * 
 * @param {hybrid} source - hybrid string to compile
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
export function afwCompileHybrid(client : any, source : any, listing? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "compile<hybrid>";
    _action["source"] = source;

    if (listing !== undefined)
        _action["listing"] = listing;

    return client.perform(_action);
}

/**
 * Determine if hybrid arg1 is equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'eqx' ('===')
 * instead if you want false to be returned if arg1 and arg2's data type
 * don't match.
 * 
 * @param {hybrid} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqHybrid(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<hybrid>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for hybrid arg1 is equal to the value and data type of arg2
 * then return the boolean result. Use 'eq' ('==') instead if you want arg2
 * to be converted to the data type of arg1 before comparison.
 * 
 * @param {hybrid} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxHybrid(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<hybrid>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Compile and evaluate hybrid value.
 * 
 * @param {hybrid} source - hybrid string to compile and evaluate
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
export function afwEvaluateHybrid(client : any, source : any, additionalUntrustedQualifiedVariables? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "evaluate<hybrid>";
    _action["source"] = source;

    if (additionalUntrustedQualifiedVariables !== undefined)
        _action["additionalUntrustedQualifiedVariables"] = additionalUntrustedQualifiedVariables;

    return client.perform(_action);
}

/**
 * Checks for hybrid arg1 is greater than or equal to hybrid arg2 and return
 * the boolean result.
 * 
 * @param {hybrid} arg1 -
 * 
 * @param {hybrid} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeHybrid(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<hybrid>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for hybrid arg1 is greater than hybrid arg2 and return the boolean
 * result.
 * 
 * @param {hybrid} arg1 -
 * 
 * @param {hybrid} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtHybrid(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<hybrid>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Converts value to data type hybrid returning hybrid result.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {hybrid} Converted value
 */
export function afwHybrid(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "hybrid";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType hybrid and return the boolean result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsHybrid(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is<hybrid>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks for hybrid arg1 is less than or equal to hybrid arg2 and return the
 * boolean result.
 * 
 * @param {hybrid} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeHybrid(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<hybrid>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for hybrid arg1 is less that hybrid arg2 and return the boolean
 * result.
 * 
 * @param {hybrid} arg1 -
 * 
 * @param {hybrid} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtHybrid(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<hybrid>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if hybrid arg1 is not equal to the value of arg2 converted to
 * the data type of arg1 then return the boolean result. Use 'nex' ('!==')
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 * 
 * @param {hybrid} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeHybrid(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<hybrid>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for hybrid arg1 is not equal to the value or data type of
 * arg2 then return the boolean result. Use 'ne' ('!=') instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {hybrid} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexHybrid(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<hybrid>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Converts hybrid value to string. For list values, the to_string() value
 * for each entry is returned separated with commas.
 * 
 * @param {hybrid} value - A hybrid value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringHybrid(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<hybrid>";
    _action["value"] = value;

    return client.perform(_action);
}

