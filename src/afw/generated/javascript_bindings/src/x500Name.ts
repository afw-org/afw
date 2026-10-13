// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for x500Name
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file x500Name.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the x500Name category.
 */

interface IAnyObject {
    [prop : string] : any;
}

/**
 * Construct a new array of x500Name from the given values, the same as
 * array() except that the new array is typed: each value, and each entry of a
 * spread array, must be dataType x500Name, and adding a value of another data
 * type to the array later throws. A value of another data type is an error,
 * never converted. This is usually written `array<x500Name>(`...values).
 * 
 * @param {} values - A value of dataType x500Name, or ... followed by an
 *     expression that results in an array, every entry of which is included.
 * 
 * @returns {array} The constructed array of x500Name.
 */
export function afwArrayX500Name(client : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "array_x500Name";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Returns value unchanged if it is dataType x500Name. Otherwise, including
 * when value is undefined, throws an error. It never converts: to get a
 * x500Name from a value of another data type, write a typed literal or call a
 * to_T conversion function where one exists. This is usually written value as
 * x500Name.
 * 
 * @param {} value - Value to assert
 * 
 * @returns {x500Name} value, which is dataType x500Name
 */
export function afwAsX500Name(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "as_x500Name";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Returns boolean true if at least one value in x500Name array1 is in
 * x500Name array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {boolean}
 */
export function afwAtLeastOneMemberOfX500Name(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "at_least_one_member_of<x500Name>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Determine if x500Name arg1 is equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'eqx' ('===') instead
 * if you want false to be returned if arg1 and arg2's data type don't match.
 * 
 * @param {x500Name} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqX500Name(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<x500Name>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for x500Name arg1 is equal to the value and data type of arg2
 * then return the boolean result. Use 'eq' ('==') instead if you want arg2 to
 * be converted to the data type of arg1 before comparison.
 * 
 * @param {x500Name} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxX500Name(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<x500Name>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Returns an array of x500Name with the values that are common to both array
 * of x500Name array1 and array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {array}
 */
export function afwIntersectionX500Name(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "intersection<x500Name>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Checks whether x500Name value is in array of x500Name array and returns the
 * boolean result. Empty array yields false.
 * 
 * @param {x500Name} value -
 * 
 * @param {array} array -
 * 
 * @returns {boolean}
 */
export function afwIsInX500Name(client : any, value : any, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_in<x500Name>";
    _action["value"] = value;
    _action["array"] = array;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType x500Name and returns the boolean result.
 * It is false for undefined. This can also be written `is<x500Name>(value`).
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsX500Name(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_x500Name";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Determine if x500Name arg1 is not equal to the value of arg2 converted to
 * the data type of arg1 then return the boolean result. Use 'nex' ('!==')
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 * 
 * @param {x500Name} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeX500Name(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<x500Name>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for x500Name arg1 is not equal to the value or data type of
 * arg2 then return the boolean result. Use 'ne' ('!=') instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {x500Name} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexX500Name(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<x500Name>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * This converts an array of x500Name values that contains one value to a
 * single x500Name value.
 * 
 * @param {array} array -
 * 
 * @returns {x500Name}
 */
export function afwOneAndOnlyX500Name(client : any, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "one_and_only<x500Name>";
    _action["array"] = array;

    return client.perform(_action);
}

/**
 * Checks whether x500Name value matches the regular expression regexp and
 * return the boolean result.
 * 
 * @param {x500Name} value -
 * 
 * @param {string} regexp -
 * 
 * @returns {boolean}
 */
export function afwRegexpMatchX500Name(client : any, value : any, regexp : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "regexp_match<x500Name>";
    _action["value"] = value;
    _action["regexp"] = regexp;

    return client.perform(_action);
}

/**
 * Returns boolean true if x500Name array1 and x500Name array2 are subsets of
 * each other and return the boolean result.
 * 
 * @param {array} array1 -
 * 
 * @param {array} array2 -
 * 
 * @returns {boolean}
 */
export function afwSetEqualsX500Name(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "set_equals<x500Name>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Returns boolean true if the unique values in x500Name array1 are all in
 * x500Name array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {boolean}
 */
export function afwSubsetX500Name(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "subset<x500Name>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Converts x500Name value to string. For array values, the to_string() value
 * for each entry is returned separated with commas.
 * 
 * @param {x500Name} value - A x500Name value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringX500Name(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<x500Name>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Returns an array of x500Name contains all of the unique values in two or
 * more array of x500Name values.
 * 
 * @param {array} arrays - Two or more arrays.
 * 
 * @returns {array}
 */
export function afwUnionX500Name(client : any, arrays : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "union<x500Name>";
    _action["arrays"] = arrays;

    return client.perform(_action);
}

/**
 * Returns true if the RDNs of pattern are the last RDNs of value, compared as
 * x500Name equality compares them. For example, "O=Medico Corp,C=US"x500Name
 * matches "cn=John Smith,o=Medico Corp, c=US"x500Name.
 * 
 * @param {x500Name} pattern - The RDNs value must end with.
 * 
 * @param {x500Name} value - The x500Name to check.
 * 
 * @returns {boolean} True if value ends with the RDNs of pattern.
 */
export function afwX500NameMatch(client : any, pattern : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "x500Name_match";
    _action["pattern"] = pattern;
    _action["value"] = value;

    return client.perform(_action);
}

