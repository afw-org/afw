// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for rfc822Name
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Returns boolean true if at least one value in rfc822Name array1 is in
 * rfc822Name array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {boolean}
 */
export function afwAtLeastOneMemberOfRfc822Name(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "at_least_one_member_of<rfc822Name>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Takes any number of rfc822Name values and returns an array of array.
 * 
 * @param {array} values -
 * 
 * @returns {array}
 */
export function afwBagRfc822Name(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<rfc822Name>";

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
export function afwBagSizeRfc822Name(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size<rfc822Name>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Determine if rfc822Name arg1 is equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'eqx' ('===') instead
 * if you want false to be returned if arg1 and arg2's data type don't match.
 * 
 * @param {rfc822Name} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqRfc822Name(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<rfc822Name>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for rfc822Name arg1 is equal to the value and data type of
 * arg2 then return the boolean result. Use 'eq' ('==') instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {rfc822Name} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxRfc822Name(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<rfc822Name>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for rfc822Name arg1 is greater than or equal to rfc822Name arg2 and
 * return the boolean result.
 * 
 * @param {rfc822Name} arg1 -
 * 
 * @param {rfc822Name} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeRfc822Name(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<rfc822Name>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for rfc822Name arg1 is greater than rfc822Name arg2 and return the
 * boolean result.
 * 
 * @param {rfc822Name} arg1 -
 * 
 * @param {rfc822Name} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtRfc822Name(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<rfc822Name>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Returns an array of rfc822Name with the values that are common to both
 * array of rfc822Name array1 and array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {array}
 */
export function afwIntersectionRfc822Name(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "intersection<rfc822Name>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Checks whether rfc822Name value is in array of rfc822Name array and returns
 * the boolean result.
 * 
 * @param {rfc822Name} value -
 * 
 * @param {array} array -
 * 
 * @returns {boolean}
 */
export function afwIsInRfc822Name(client : any, value : any, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_in<rfc822Name>";
    _action["value"] = value;
    _action["array"] = array;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType rfc822Name and return the boolean result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsRfc822Name(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is<rfc822Name>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks for rfc822Name arg1 is less than or equal to rfc822Name arg2 and
 * return the boolean result.
 * 
 * @param {rfc822Name} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeRfc822Name(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<rfc822Name>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for rfc822Name arg1 is less that rfc822Name arg2 and return the
 * boolean result.
 * 
 * @param {rfc822Name} arg1 -
 * 
 * @param {rfc822Name} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtRfc822Name(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<rfc822Name>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Returns true if arg1 matches some terminal sequence of RDNs from arg2 when
 * compared using equal_rfc822Name.
 * 
 * @param {rfc822Name} arg1 -
 * 
 * @param {string} arg2 -
 * 
 * @returns {boolean}
 */
export function afwMatchRfc822Name(client : any, arg1 : any, arg2 : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "match<rfc822Name>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if rfc822Name arg1 is not equal to the value of arg2 converted to
 * the data type of arg1 then return the boolean result. Use 'nex' ('!==')
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 * 
 * @param {rfc822Name} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeRfc822Name(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<rfc822Name>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for rfc822Name arg1 is not equal to the value or data type of
 * arg2 then return the boolean result. Use 'ne' ('!=') instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {rfc822Name} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexRfc822Name(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<rfc822Name>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * This converts an array of rfc822Name values that contains one value to a
 * single rfc822Name value.
 * 
 * @param {array} array -
 * 
 * @returns {rfc822Name}
 */
export function afwOneAndOnlyRfc822Name(client : any, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "one_and_only<rfc822Name>";
    _action["array"] = array;

    return client.perform(_action);
}

/**
 * Checks whether rfc822Name value matches the regular expression regexp and
 * return the boolean result.
 * 
 * @param {rfc822Name} value -
 * 
 * @param {string} regexp -
 * 
 * @returns {boolean}
 */
export function afwRegexpMatchRfc822Name(client : any, value : any, regexp : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "regexp_match<rfc822Name>";
    _action["value"] = value;
    _action["regexp"] = regexp;

    return client.perform(_action);
}

/**
 * Converts value to data type rfc822Name returning rfc822Name result.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {rfc822Name} Converted value
 */
export function afwRfc822Name(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "rfc822Name";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Returns boolean true if rfc822Name array1 and rfc822Name array2 are subsets
 * of each other and return the boolean result.
 * 
 * @param {array} array1 -
 * 
 * @param {array} array2 -
 * 
 * @returns {boolean}
 */
export function afwSetEqualsRfc822Name(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "set_equals<rfc822Name>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Returns boolean true if the unique values in rfc822Name array1 are all in
 * rfc822Name array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {boolean}
 */
export function afwSubsetRfc822Name(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "subset<rfc822Name>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Converts rfc822Name value to string. For array values, the to_string()
 * value for each entry is returned separated with commas.
 * 
 * @param {rfc822Name} value - A rfc822Name value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringRfc822Name(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<rfc822Name>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Returns an array of rfc822Name contains all of the unique values in two or
 * more array of rfc822Name values.
 * 
 * @param {array} arrays - Two or more arrays.
 * 
 * @returns {array}
 */
export function afwUnionRfc822Name(client : any, arrays : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "union<rfc822Name>";
    _action["arrays"] = arrays;

    return client.perform(_action);
}

