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
 * Construct a new array of null from the given values, the same as array()
 * except that the new array is typed: each value, and each entry of a spread
 * array, must be dataType null, and adding a value of another data type to
 * the array later throws. A value of another data type is an error, never
 * converted. This is usually written `array<null>(`...values).
 * 
 * @param {} values - A value of dataType null, or ... followed by an
 *     expression that results in an array, every entry of which is included.
 * 
 * @returns {array} The constructed array of null.
 */
export function afwArrayNull(client : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "array_null";
    _action["values"] = values;

    return client.perform(_action);
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

