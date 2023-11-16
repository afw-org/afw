// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for objectId
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file objectId.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the objectId category.
 */

interface IAnyObject {
    [prop : string] : any;
}

/**
 * Takes any number of objectId values and returns an array of array.
 * 
 * @param {array} values -
 * 
 * @returns {array}
 */
export function afwBagObjectId(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<objectId>";

    if (values !== undefined)
        _action["values"] = values;

    return client.perform(_action);
}

/**
 * This returns the integer number of values in array.
 * 
 * @param {array} value -
 * 
 * @returns {integer}
 */
export function afwBagSizeObjectId(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size<objectId>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Determine if objectId arg1 is equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'eqx' ('===') instead
 * if you want false to be returned if arg1 and arg2's data type don't match.
 * 
 * @param {objectId} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqObjectId(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<objectId>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for objectId arg1 is equal to the value and data type of arg2
 * then return the boolean result. Use 'eq' ('==') instead if you want arg2 to
 * be converted to the data type of arg1 before comparison.
 * 
 * @param {objectId} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxObjectId(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<objectId>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for objectId arg1 is greater than or equal to objectId arg2 and
 * return the boolean result.
 * 
 * @param {objectId} arg1 -
 * 
 * @param {objectId} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeObjectId(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<objectId>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for objectId arg1 is greater than objectId arg2 and return the
 * boolean result.
 * 
 * @param {objectId} arg1 -
 * 
 * @param {objectId} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtObjectId(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<objectId>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType objectId and return the boolean result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsObjectId(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is<objectId>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks for objectId arg1 is less than or equal to objectId arg2 and return
 * the boolean result.
 * 
 * @param {objectId} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeObjectId(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<objectId>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for objectId arg1 is less that objectId arg2 and return the boolean
 * result.
 * 
 * @param {objectId} arg1 -
 * 
 * @param {objectId} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtObjectId(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<objectId>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if objectId arg1 is not equal to the value of arg2 converted to
 * the data type of arg1 then return the boolean result. Use 'nex' ('!==')
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 * 
 * @param {objectId} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeObjectId(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<objectId>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for objectId arg1 is not equal to the value or data type of
 * arg2 then return the boolean result. Use 'ne' ('!=') instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {objectId} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexObjectId(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<objectId>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Converts value to data type objectId returning objectId result.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {objectId} Converted value
 */
export function afwObjectId(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "objectId";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts objectId value to string. For array values, the to_string() value
 * for each entry is returned separated with commas.
 * 
 * @param {objectId} value - A objectId value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringObjectId(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<objectId>";
    _action["value"] = value;

    return client.perform(_action);
}

