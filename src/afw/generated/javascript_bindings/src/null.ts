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
 * Returns value unchanged if it is dataType null. Otherwise, including when
 * value is undefined, throws an error. It never converts: to get a null from
 * a value of another data type, write a typed literal or call a to_T
 * conversion function where one exists. This is usually written value as
 * null.
 * 
 * @param {} value - Value to assert
 * 
 * @returns {null} value, which is dataType null
 */
export function afwAsNull(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "as_null";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Takes any number of null values and returns an array of null.
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
 * Checks whether value is dataType null and returns the boolean result. It is
 * false for undefined. This can also be written `is<null>(value`).
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsNull(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_null";
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

