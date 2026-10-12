// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for objectPath
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file objectPath.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the objectPath category.
 */

interface IAnyObject {
    [prop : string] : any;
}

/**
 * Construct a new array of objectPath from the given values, the same as
 * array() except that the new array is typed: each value, and each entry of a
 * spread array, must be dataType objectPath, and adding a value of another
 * data type to the array later throws. A value of another data type is an
 * error, never converted. This is usually written
 * `array<objectPath>(`...values).
 * 
 * @param {} values - A value of dataType objectPath, or ... followed by an
 *     expression that results in an array, every entry of which is included.
 * 
 * @returns {array} The constructed array of objectPath.
 */
export function afwArrayObjectPath(client : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "array_objectPath";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Returns value unchanged if it is dataType objectPath. Otherwise, including
 * when value is undefined, throws an error. It never converts: to get a
 * objectPath from a value of another data type, write a typed literal or call
 * a to_T conversion function where one exists. This is usually written value
 * as objectPath.
 * 
 * @param {} value - Value to assert
 * 
 * @returns {objectPath} value, which is dataType objectPath
 */
export function afwAsObjectPath(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "as_objectPath";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Takes any number of objectPath values and returns an array of objectPath.
 * 
 * @param {objectPath} values -
 * 
 * @returns {array}
 */
export function afwBagObjectPath(client : any, values? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<objectPath>";

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
export function afwBagSizeObjectPath(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size<objectPath>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Determine if objectPath arg1 is equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'eqx' ('===') instead
 * if you want false to be returned if arg1 and arg2's data type don't match.
 * 
 * @param {objectPath} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqObjectPath(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<objectPath>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for objectPath arg1 is equal to the value and data type of
 * arg2 then return the boolean result. Use 'eq' ('==') instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {objectPath} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxObjectPath(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<objectPath>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for objectPath arg1 is greater than or equal to objectPath arg2 and
 * return the boolean result.
 * 
 * @param {objectPath} arg1 -
 * 
 * @param {objectPath} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeObjectPath(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<objectPath>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for objectPath arg1 is greater than objectPath arg2 and return the
 * boolean result.
 * 
 * @param {objectPath} arg1 -
 * 
 * @param {objectPath} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtObjectPath(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<objectPath>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType objectPath and returns the boolean result.
 * It is false for undefined. This can also be written
 * `is<objectPath>(value`).
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsObjectPath(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_objectPath";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks for objectPath arg1 is less than or equal to objectPath arg2 and
 * return the boolean result.
 * 
 * @param {objectPath} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeObjectPath(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<objectPath>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for objectPath arg1 is less that objectPath arg2 and return the
 * boolean result.
 * 
 * @param {objectPath} arg1 -
 * 
 * @param {objectPath} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtObjectPath(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<objectPath>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if objectPath arg1 is not equal to the value of arg2 converted to
 * the data type of arg1 then return the boolean result. Use 'nex' ('!==')
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 * 
 * @param {objectPath} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeObjectPath(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<objectPath>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for objectPath arg1 is not equal to the value or data type of
 * arg2 then return the boolean result. Use 'ne' ('!=') instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {objectPath} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexObjectPath(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<objectPath>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Converts objectPath value to string. For array values, the to_string()
 * value for each entry is returned separated with commas.
 * 
 * @param {objectPath} value - A objectPath value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringObjectPath(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<objectPath>";
    _action["value"] = value;

    return client.perform(_action);
}

