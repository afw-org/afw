// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for null
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file null.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the null category.
 */

interface IAnyObject {
    [prop : string] : any;
}

/**
 * Takes any number of null values and returns an array of array.
 * 
 * @param {array} values -
 * 
 * @returns {array}
 */
export function afwBagNull(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<null>";

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
export function afwBagSizeNull(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size<null>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType null and return the boolean result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsNull(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is<null>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts value to data type null returning null result.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {void} Converted value
 */
export function afwNull(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "null";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts null value to string. For array values, the to_string() value for
 * each entry is returned separated with commas.
 * 
 * @param {null} value - A null value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringNull(client : any, value : null) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<null>";
    _action["value"] = value;

    return client.perform(_action);
}

