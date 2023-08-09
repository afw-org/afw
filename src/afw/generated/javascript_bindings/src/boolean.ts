// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for boolean
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Returns boolean true if at least one value in boolean array1 is in boolean
 * array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {boolean}
 */
export function afwAtLeastOneMemberOfBoolean(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "at_least_one_member_of<boolean>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Takes any number of boolean values and returns an array of array.
 * 
 * @param {array} values -
 * 
 * @returns {array}
 */
export function afwBagBoolean(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<boolean>";

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
export function afwBagSizeBoolean(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size<boolean>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts value to data type boolean returning boolean result.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {boolean} Converted value
 */
export function afwBoolean(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "boolean";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Determine if boolean arg1 is equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'eqx' ('===')
 * instead if you want false to be returned if arg1 and arg2's data type
 * don't match.
 * 
 * @param {boolean} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqBoolean(client : any, arg1 : boolean, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<boolean>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for boolean arg1 is equal to the value and data type of arg2
 * then return the boolean result. Use 'eq' ('==') instead if you want arg2
 * to be converted to the data type of arg1 before comparison.
 * 
 * @param {boolean} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxBoolean(client : any, arg1 : boolean, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<boolean>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for boolean arg1 is greater than or equal to boolean arg2 and
 * return the boolean result.
 * 
 * @param {boolean} arg1 -
 * 
 * @param {boolean} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeBoolean(client : any, arg1 : boolean, arg2 : boolean) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<boolean>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for boolean arg1 is greater than boolean arg2 and return the
 * boolean result.
 * 
 * @param {boolean} arg1 -
 * 
 * @param {boolean} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtBoolean(client : any, arg1 : boolean, arg2 : boolean) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<boolean>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Returns an array of boolean with the values that are common to both array
 * of boolean array1 and array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {array}
 */
export function afwIntersectionBoolean(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "intersection<boolean>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType boolean and return the boolean result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsBoolean(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is<boolean>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks whether boolean value is in array of boolean array and returns the
 * boolean result.
 * 
 * @param {boolean} value -
 * 
 * @param {array} array -
 * 
 * @returns {boolean}
 */
export function afwIsInBoolean(client : any, value : boolean, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_in<boolean>";
    _action["value"] = value;
    _action["array"] = array;

    return client.perform(_action);
}

/**
 * Checks for boolean arg1 is less than or equal to boolean arg2 and return
 * the boolean result.
 * 
 * @param {boolean} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeBoolean(client : any, arg1 : boolean, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<boolean>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for boolean arg1 is less that boolean arg2 and return the boolean
 * result.
 * 
 * @param {boolean} arg1 -
 * 
 * @param {boolean} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtBoolean(client : any, arg1 : boolean, arg2 : boolean) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<boolean>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if boolean arg1 is not equal to the value of arg2 converted to
 * the data type of arg1 then return the boolean result. Use 'nex' ('!==')
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 * 
 * @param {boolean} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeBoolean(client : any, arg1 : boolean, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<boolean>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for boolean arg1 is not equal to the value or data type of
 * arg2 then return the boolean result. Use 'ne' ('!=') instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {boolean} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexBoolean(client : any, arg1 : boolean, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<boolean>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * This converts an array of boolean values that contains one value to a
 * single boolean value.
 * 
 * @param {array} array -
 * 
 * @returns {boolean}
 */
export function afwOneAndOnlyBoolean(client : any, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "one_and_only<boolean>";
    _action["array"] = array;

    return client.perform(_action);
}

/**
 * Returns boolean true if boolean array1 and boolean array2 are subsets of
 * each other and return the boolean result.
 * 
 * @param {array} array1 -
 * 
 * @param {array} array2 -
 * 
 * @returns {boolean}
 */
export function afwSetEqualsBoolean(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "set_equals<boolean>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Returns boolean true if the unique values in boolean array1 are all in
 * boolean array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {boolean}
 */
export function afwSubsetBoolean(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "subset<boolean>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Converts boolean value to string. For array values, the to_string() value
 * for each entry is returned separated with commas.
 * 
 * @param {boolean} value - A boolean value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringBoolean(client : any, value : boolean) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<boolean>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Returns an array of boolean contains all of the unique values in two or
 * more array of boolean values.
 * 
 * @param {array} arrays - Two or more arrays.
 * 
 * @returns {array}
 */
export function afwUnionBoolean(client : any, arrays : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "union<boolean>";
    _action["arrays"] = arrays;

    return client.perform(_action);
}

