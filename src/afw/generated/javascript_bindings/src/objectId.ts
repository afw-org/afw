// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for objectId
 *
 * Copyright (c) 2010-2024 Clemson University
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
 * Construct a new array of objectId from the given values, the same as
 * array() except that the new array is typed: each value, and each entry of a
 * spread array, must be dataType objectId, and adding a value of another data
 * type to the array later throws. A value of another data type is an error,
 * never converted. This is usually written `array<objectId>(`...values).
 * 
 * @param {} values - A value of dataType objectId, or ... followed by an
 *     expression that results in an array, every entry of which is included.
 * 
 * @returns {array} The constructed array of objectId.
 */
export function afwArrayObjectId(client : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "array_objectId";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Returns value unchanged if it is dataType objectId. Otherwise, including
 * when value is undefined, throws an error. It never converts: to get a
 * objectId from a value of another data type, write a typed literal or call a
 * to_T conversion function where one exists. This is usually written value as
 * objectId.
 * 
 * @param {} value - Value to assert
 * 
 * @returns {objectId} value, which is dataType objectId
 */
export function afwAsObjectId(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "as_objectId";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Takes any number of objectId values and returns an array of objectId.
 * 
 * @param {objectId} values -
 * 
 * @returns {array}
 */
export function afwBagObjectId(client : any, values? : any) : any {

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
 * Checks whether value is dataType objectId and returns the boolean result.
 * It is false for undefined. This can also be written `is<objectId>(value`).
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsObjectId(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_objectId";
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

