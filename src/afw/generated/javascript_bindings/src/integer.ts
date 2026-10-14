// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for integer
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file integer.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the integer category.
 */

interface IAnyObject {
    [prop : string] : any;
}

/**
 * Compute the absolute value of the integer value and return the integer
 * result.
 * 
 * @param {integer} value -
 * 
 * @returns {integer}
 */
export function afwAbsInteger(client : any, value : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "abs<integer>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Add 2 or more integer values and return the integer result.
 * 
 * @param {integer} values -
 * 
 * @returns {integer}
 */
export function afwAddInteger(client : any, values : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "add<integer>";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Construct a new array of integer from the given values, the same as array()
 * except that the new array is typed: each value, and each entry of a spread
 * array, must be dataType integer, and adding a value of another data type to
 * the array later throws. A value of another data type is an error, never
 * converted. This is usually written `array<integer>(`...values).
 * 
 * @param {} values - A value of dataType integer, or ... followed by an
 *     expression that results in an array, every entry of which is included.
 * 
 * @returns {array} The constructed array of integer.
 */
export function afwArrayInteger(client : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "array_integer";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Returns value unchanged if it is dataType integer. Otherwise, including
 * when value is undefined, throws an error. It never converts: to get a
 * integer from a value of another data type, write a typed literal or call a
 * to_T conversion function where one exists. This is usually written value as
 * integer.
 * 
 * @param {} value - Value to assert
 * 
 * @returns {integer} value, which is dataType integer
 */
export function afwAsInteger(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "as_integer";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Returns boolean true if at least one value in integer array1 is in integer
 * array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {boolean}
 */
export function afwAtLeastOneMemberOfInteger(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "at_least_one_member_of<integer>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Divide integer dividend by integer divisor and return the integer quotient.
 * 
 * @param {integer} dividend -
 * 
 * @param {integer} divisor -
 * 
 * @returns {integer}
 */
export function afwDivideInteger(client : any, dividend : number, divisor : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "divide<integer>";
    _action["dividend"] = dividend;
    _action["divisor"] = divisor;

    return client.perform(_action);
}

/**
 * Determine if integer arg1 is equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'eqx' ('===') instead
 * if you want false to be returned if arg1 and arg2's data type don't match.
 * 
 * @param {integer} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqInteger(client : any, arg1 : number, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<integer>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for integer arg1 is equal to the value and data type of arg2
 * then return the boolean result. Use 'eq' ('==') instead if you want arg2 to
 * be converted to the data type of arg1 before comparison.
 * 
 * @param {integer} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxInteger(client : any, arg1 : number, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<integer>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for integer arg1 is greater than or equal to integer arg2 and return
 * the boolean result.
 * 
 * @param {integer} arg1 -
 * 
 * @param {integer} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeInteger(client : any, arg1 : number, arg2 : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<integer>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for integer arg1 is greater than integer arg2 and return the boolean
 * result.
 * 
 * @param {integer} arg1 -
 * 
 * @param {integer} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtInteger(client : any, arg1 : number, arg2 : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<integer>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Returns an array of integer with the values that are common to both array
 * of integer array1 and array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {array}
 */
export function afwIntersectionInteger(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "intersection<integer>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Checks whether integer value is in array of integer array and returns the
 * boolean result. Empty array yields false.
 * 
 * @param {integer} value -
 * 
 * @param {array} array -
 * 
 * @returns {boolean}
 */
export function afwIsInInteger(client : any, value : number, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_in<integer>";
    _action["value"] = value;
    _action["array"] = array;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType integer and returns the boolean result. It
 * is false for undefined. This can also be written `is<integer>(value`).
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsInteger(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_integer";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks for integer arg1 is less than or equal to integer arg2 and return
 * the boolean result.
 * 
 * @param {integer} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeInteger(client : any, arg1 : number, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<integer>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for integer arg1 is less that integer arg2 and return the boolean
 * result.
 * 
 * @param {integer} arg1 -
 * 
 * @param {integer} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtInteger(client : any, arg1 : number, arg2 : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<integer>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Return the integer value that is greater than or equal to the others.
 * 
 * @param {integer} values -
 * 
 * @returns {integer}
 */
export function afwMaxInteger(client : any, values : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "max<integer>";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Return the integer value that is less than or equal to the others.
 * 
 * @param {integer} values -
 * 
 * @returns {integer}
 */
export function afwMinInteger(client : any, values : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "min<integer>";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Divide integer dividend by integer divisor and return the integer
 * remainder.
 * 
 * @param {integer} dividend -
 * 
 * @param {integer} divisor -
 * 
 * @returns {integer}
 */
export function afwModInteger(client : any, dividend : number, divisor : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "mod<integer>";
    _action["dividend"] = dividend;
    _action["divisor"] = divisor;

    return client.perform(_action);
}

/**
 * Multiply 2 or more integer values and return the integer result.
 * 
 * @param {integer} values -
 * 
 * @returns {integer}
 */
export function afwMultiplyInteger(client : any, values : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "multiply<integer>";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Determine if integer arg1 is not equal to the value of arg2 converted to
 * the data type of arg1 then return the boolean result. Use 'nex' ('!==')
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 * 
 * @param {integer} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeInteger(client : any, arg1 : number, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<integer>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Return negative of integer value.
 * 
 * @param {integer} value -
 * 
 * @returns {integer}
 */
export function afwNegativeInteger(client : any, value : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "negative<integer>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Determine if for integer arg1 is not equal to the value or data type of
 * arg2 then return the boolean result. Use 'ne' ('!=') instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {integer} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexInteger(client : any, arg1 : number, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<integer>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * This converts an array of integer values that contains one value to a
 * single integer value.
 * 
 * @param {array} array -
 * 
 * @returns {integer}
 */
export function afwOneAndOnlyInteger(client : any, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "one_and_only<integer>";
    _action["array"] = array;

    return client.perform(_action);
}

/**
 * Returns boolean true if integer array1 and integer array2 are subsets of
 * each other and return the boolean result.
 * 
 * @param {array} array1 -
 * 
 * @param {array} array2 -
 * 
 * @returns {boolean}
 */
export function afwSetEqualsInteger(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "set_equals<integer>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Return the sign of the integer value, as ECMAScript Math.sign does: -1, 0
 * or 1 of the same data type (`sign<double>` keeps the sign of a zero:
 * sign(-0.0) is -0.0). A NaN argument is an error.
 * 
 * @param {integer} value -
 * 
 * @returns {integer}
 */
export function afwSignInteger(client : any, value : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "sign<integer>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Returns boolean true if the unique values in integer array1 are all in
 * integer array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {boolean}
 */
export function afwSubsetInteger(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "subset<integer>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Subtract integer arg2 from integer arg1 and return the integer result.
 * 
 * @param {integer} arg1 -
 * 
 * @param {integer} arg2 -
 * 
 * @returns {integer}
 */
export function afwSubtractInteger(client : any, arg1 : number, arg2 : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "subtract<integer>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Converts integer value to double. A string is read as XML Schema double
 * text: digits with an optional sign, decimal point, and exponent ("5", ".5",
 * "5.", "007.5", "1e3"), INF, +INF, -INF, or NaN. Other text, including
 * leading or trailing spaces, is an error. Only text is read this way: an
 * integer value is never silently a double.
 * 
 * @param {integer} value -
 * 
 * @returns {double}
 */
export function afwToDoubleInteger(client : any, value : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_double<integer>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts integer value to string. For array values, the to_string() value
 * for each entry is returned separated with commas.
 * 
 * @param {integer} value - A integer value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringInteger(client : any, value : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<integer>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Returns an array of integer contains all of the unique values in two or
 * more array of integer values.
 * 
 * @param {array} arrays - Two or more arrays.
 * 
 * @returns {array}
 */
export function afwUnionInteger(client : any, arrays : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "union<integer>";
    _action["arrays"] = arrays;

    return client.perform(_action);
}

