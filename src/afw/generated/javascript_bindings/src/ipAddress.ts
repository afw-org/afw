// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for ipAddress
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file ipAddress.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the ipAddress category.
 */

interface IAnyObject {
    [prop : string] : any;
}

/**
 * Takes any number of ipAddress values and returns an array of array.
 * 
 * @param {array} values -
 * 
 * @returns {array}
 */
export function afwBagIpAddress(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<ipAddress>";

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
export function afwBagSizeIpAddress(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size<ipAddress>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Determine if ipAddress arg1 is equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'eqx' ('===') instead
 * if you want false to be returned if arg1 and arg2's data type don't match.
 * 
 * @param {ipAddress} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqIpAddress(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<ipAddress>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for ipAddress arg1 is equal to the value and data type of arg2
 * then return the boolean result. Use 'eq' ('==') instead if you want arg2 to
 * be converted to the data type of arg1 before comparison.
 * 
 * @param {ipAddress} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxIpAddress(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<ipAddress>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for ipAddress arg1 is greater than or equal to ipAddress arg2 and
 * return the boolean result.
 * 
 * @param {ipAddress} arg1 -
 * 
 * @param {ipAddress} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeIpAddress(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<ipAddress>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for ipAddress arg1 is greater than ipAddress arg2 and return the
 * boolean result.
 * 
 * @param {ipAddress} arg1 -
 * 
 * @param {ipAddress} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtIpAddress(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<ipAddress>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Converts value to data type ipAddress returning ipAddress result.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {ipAddress} Converted value
 */
export function afwIpAddress(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ipAddress";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks whether ipAddress value is in array of ipAddress array and returns
 * the boolean result.
 * 
 * @param {ipAddress} value -
 * 
 * @param {array} array -
 * 
 * @returns {boolean}
 */
export function afwIsInIpAddress(client : any, value : any, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_in<ipAddress>";
    _action["value"] = value;
    _action["array"] = array;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType ipAddress and return the boolean result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsIpAddress(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is<ipAddress>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks for ipAddress arg1 is less than or equal to ipAddress arg2 and
 * return the boolean result.
 * 
 * @param {ipAddress} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeIpAddress(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<ipAddress>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for ipAddress arg1 is less that ipAddress arg2 and return the
 * boolean result.
 * 
 * @param {ipAddress} arg1 -
 * 
 * @param {ipAddress} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtIpAddress(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<ipAddress>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if ipAddress arg1 is not equal to the value of arg2 converted to
 * the data type of arg1 then return the boolean result. Use 'nex' ('!==')
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 * 
 * @param {ipAddress} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeIpAddress(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<ipAddress>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for ipAddress arg1 is not equal to the value or data type of
 * arg2 then return the boolean result. Use 'ne' ('!=') instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {ipAddress} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexIpAddress(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<ipAddress>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * This converts an array of ipAddress values that contains one value to a
 * single ipAddress value.
 * 
 * @param {array} array -
 * 
 * @returns {ipAddress}
 */
export function afwOneAndOnlyIpAddress(client : any, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "one_and_only<ipAddress>";
    _action["array"] = array;

    return client.perform(_action);
}

/**
 * Checks whether ipAddress value matches the regular expression regexp and
 * return the boolean result.
 * 
 * @param {ipAddress} value -
 * 
 * @param {string} regexp -
 * 
 * @returns {boolean}
 */
export function afwRegexpMatchIpAddress(client : any, value : any, regexp : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "regexp_match<ipAddress>";
    _action["value"] = value;
    _action["regexp"] = regexp;

    return client.perform(_action);
}

/**
 * Converts ipAddress value to string. For array values, the to_string() value
 * for each entry is returned separated with commas.
 * 
 * @param {ipAddress} value - A ipAddress value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringIpAddress(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<ipAddress>";
    _action["value"] = value;

    return client.perform(_action);
}

