// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for yearMonthDuration
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file yearMonthDuration.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the yearMonthDuration category.
 */

interface IAnyObject {
    [prop : string] : any;
}

/**
 * Returns boolean true if at least one value in yearMonthDuration array1 is
 * in yearMonthDuration array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {boolean}
 */
export function afwAtLeastOneMemberOfYearMonthDuration(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "at_least_one_member_of<yearMonthDuration>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * This returns the integer number of values in array.
 * 
 * @param {array} value -
 * 
 * @returns {integer}
 */
export function afwBagSizeYearMonthDuration(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size<yearMonthDuration>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Takes any number of yearMonthDuration values and returns an array of array.
 * 
 * @param {array} values -
 * 
 * @returns {array}
 */
export function afwBagYearMonthDuration(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<yearMonthDuration>";

    if (values !== undefined)
        _action["values"] = values;

    return client.perform(_action);
}

/**
 * Determine if yearMonthDuration arg1 is equal to the value of arg2 converted
 * to the data type of arg1 then return the boolean result. Use 'eqx' ('===')
 * instead if you want false to be returned if arg1 and arg2's data type don't
 * match.
 * 
 * @param {yearMonthDuration} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqYearMonthDuration(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<yearMonthDuration>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for yearMonthDuration arg1 is equal to the value and data type
 * of arg2 then return the boolean result. Use 'eq' ('==') instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {yearMonthDuration} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxYearMonthDuration(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<yearMonthDuration>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for yearMonthDuration arg1 is greater than or equal to
 * yearMonthDuration arg2 and return the boolean result.
 * 
 * @param {yearMonthDuration} arg1 -
 * 
 * @param {yearMonthDuration} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeYearMonthDuration(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<yearMonthDuration>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for yearMonthDuration arg1 is greater than yearMonthDuration arg2
 * and return the boolean result.
 * 
 * @param {yearMonthDuration} arg1 -
 * 
 * @param {yearMonthDuration} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtYearMonthDuration(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<yearMonthDuration>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Returns an array of yearMonthDuration with the values that are common to
 * both array of yearMonthDuration array1 and array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {array}
 */
export function afwIntersectionYearMonthDuration(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "intersection<yearMonthDuration>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Checks whether yearMonthDuration value is in array of yearMonthDuration
 * array and returns the boolean result.
 * 
 * @param {yearMonthDuration} value -
 * 
 * @param {array} array -
 * 
 * @returns {boolean}
 */
export function afwIsInYearMonthDuration(client : any, value : any, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_in<yearMonthDuration>";
    _action["value"] = value;
    _action["array"] = array;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType yearMonthDuration and return the boolean
 * result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsYearMonthDuration(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is<yearMonthDuration>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks for yearMonthDuration arg1 is less than or equal to
 * yearMonthDuration arg2 and return the boolean result.
 * 
 * @param {yearMonthDuration} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeYearMonthDuration(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<yearMonthDuration>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for yearMonthDuration arg1 is less that yearMonthDuration arg2 and
 * return the boolean result.
 * 
 * @param {yearMonthDuration} arg1 -
 * 
 * @param {yearMonthDuration} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtYearMonthDuration(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<yearMonthDuration>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if yearMonthDuration arg1 is not equal to the value of arg2
 * converted to the data type of arg1 then return the boolean result. Use
 * 'nex' ('!==') instead if you want true to be returned if arg1 and arg2's
 * data type don't match.
 * 
 * @param {yearMonthDuration} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeYearMonthDuration(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<yearMonthDuration>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for yearMonthDuration arg1 is not equal to the value or data
 * type of arg2 then return the boolean result. Use 'ne' ('!=') instead if you
 * want arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {yearMonthDuration} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexYearMonthDuration(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<yearMonthDuration>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * This converts an array of yearMonthDuration values that contains one value
 * to a single yearMonthDuration value.
 * 
 * @param {array} array -
 * 
 * @returns {yearMonthDuration}
 */
export function afwOneAndOnlyYearMonthDuration(client : any, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "one_and_only<yearMonthDuration>";
    _action["array"] = array;

    return client.perform(_action);
}

/**
 * Returns boolean true if yearMonthDuration array1 and yearMonthDuration
 * array2 are subsets of each other and return the boolean result.
 * 
 * @param {array} array1 -
 * 
 * @param {array} array2 -
 * 
 * @returns {boolean}
 */
export function afwSetEqualsYearMonthDuration(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "set_equals<yearMonthDuration>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Returns boolean true if the unique values in yearMonthDuration array1 are
 * all in yearMonthDuration array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {boolean}
 */
export function afwSubsetYearMonthDuration(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "subset<yearMonthDuration>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Converts yearMonthDuration value to string. For array values, the
 * to_string() value for each entry is returned separated with commas.
 * 
 * @param {yearMonthDuration} value - A yearMonthDuration value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringYearMonthDuration(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<yearMonthDuration>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Returns an array of yearMonthDuration contains all of the unique values in
 * two or more array of yearMonthDuration values.
 * 
 * @param {array} arrays - Two or more arrays.
 * 
 * @returns {array}
 */
export function afwUnionYearMonthDuration(client : any, arrays : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "union<yearMonthDuration>";
    _action["arrays"] = arrays;

    return client.perform(_action);
}

/**
 * Converts value to data type yearMonthDuration returning yearMonthDuration
 * result.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {yearMonthDuration} Converted value
 */
export function afwYearMonthDuration(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "yearMonthDuration";
    _action["value"] = value;

    return client.perform(_action);
}

