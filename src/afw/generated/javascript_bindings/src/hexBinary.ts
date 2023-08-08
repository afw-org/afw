// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for hexBinary
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Returns boolean true if at least one value in hexBinary list1 is in
 * hexBinary list2.
 * 
 * @param {array} list1 - The first list.
 * 
 * @param {array} list2 - The second list.
 * 
 * @returns {boolean}
 */
export function afwAtLeastOneMemberOfHexBinary(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "at_least_one_member_of<hexBinary>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Takes any number of hexBinary values and returns a list of list.
 * 
 * @param {array} values -
 * 
 * @returns {array}
 */
export function afwBagHexBinary(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<hexBinary>";

    if (values !== undefined)
        _action["values"] = values;

    return client.perform(_action);
}

/**
 * This returns the integer number of values in list.
 * 
 * @param {array} value -
 * 
 * @returns {integer}
 */
export function afwBagSizeHexBinary(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size<hexBinary>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Decode hexBinary value to string. An error is thrown if decoded value is
 * not valid UTF-8.
 * 
 * @param {hexBinary} value - The hexBinary value to decode.
 * 
 * @returns {string} The decoded string.
 */
export function afwDecodeToStringHexBinary(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "decode_to_string<hexBinary>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Determine if hexBinary arg1 is equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'eqx' ('===')
 * instead if you want false to be returned if arg1 and arg2's data type
 * don't match.
 * 
 * @param {hexBinary} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqHexBinary(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<hexBinary>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for hexBinary arg1 is equal to the value and data type of
 * arg2 then return the boolean result. Use 'eq' ('==') instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {hexBinary} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxHexBinary(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<hexBinary>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for hexBinary arg1 is greater than or equal to hexBinary arg2 and
 * return the boolean result.
 * 
 * @param {hexBinary} arg1 -
 * 
 * @param {hexBinary} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeHexBinary(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<hexBinary>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for hexBinary arg1 is greater than hexBinary arg2 and return the
 * boolean result.
 * 
 * @param {hexBinary} arg1 -
 * 
 * @param {hexBinary} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtHexBinary(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<hexBinary>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Converts value to data type hexBinary returning hexBinary result.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {hexBinary} Converted value
 */
export function afwHexBinary(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "hexBinary";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Returns a list of hexBinary with the values that are common to both list
 * of hexBinary list1 and list2.
 * 
 * @param {array} list1 - The first list.
 * 
 * @param {array} list2 - The second list.
 * 
 * @returns {array}
 */
export function afwIntersectionHexBinary(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "intersection<hexBinary>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType hexBinary and return the boolean result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsHexBinary(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is<hexBinary>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks whether hexBinary value is in list of hexBinary list and returns
 * the boolean result.
 * 
 * @param {hexBinary} value -
 * 
 * @param {array} array -
 * 
 * @returns {boolean}
 */
export function afwIsInHexBinary(client : any, value : any, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_in<hexBinary>";
    _action["value"] = value;
    _action["array"] = array;

    return client.perform(_action);
}

/**
 * Checks for hexBinary arg1 is less than or equal to hexBinary arg2 and
 * return the boolean result.
 * 
 * @param {hexBinary} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeHexBinary(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<hexBinary>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for hexBinary arg1 is less that hexBinary arg2 and return the
 * boolean result.
 * 
 * @param {hexBinary} arg1 -
 * 
 * @param {hexBinary} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtHexBinary(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<hexBinary>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if hexBinary arg1 is not equal to the value of arg2 converted to
 * the data type of arg1 then return the boolean result. Use 'nex' ('!==')
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 * 
 * @param {hexBinary} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeHexBinary(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<hexBinary>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for hexBinary arg1 is not equal to the value or data type of
 * arg2 then return the boolean result. Use 'ne' ('!=') instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {hexBinary} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexHexBinary(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<hexBinary>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * This converts a list of hexBinary values that contains one value to a
 * single hexBinary value.
 * 
 * @param {array} array -
 * 
 * @returns {hexBinary}
 */
export function afwOneAndOnlyHexBinary(client : any, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "one_and_only<hexBinary>";
    _action["array"] = array;

    return client.perform(_action);
}

/**
 * Returns boolean true if hexBinary list1 and hexBinary list2 are subsets of
 * each other and return the boolean result.
 * 
 * @param {array} list1 -
 * 
 * @param {array} list2 -
 * 
 * @returns {boolean}
 */
export function afwSetEqualsHexBinary(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "set_equals<hexBinary>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Returns boolean true if the unique values in hexBinary list1 are all in
 * hexBinary list2.
 * 
 * @param {array} list1 - The first list.
 * 
 * @param {array} list2 - The second list.
 * 
 * @returns {boolean}
 */
export function afwSubsetHexBinary(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "subset<hexBinary>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Converts hexBinary value to string. For list values, the to_string() value
 * for each entry is returned separated with commas.
 * 
 * @param {hexBinary} value - A hexBinary value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringHexBinary(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<hexBinary>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Returns a list of hexBinary contains all of the unique values in two or
 * more list of hexBinary values.
 * 
 * @param {array} lists - Two or more lists.
 * 
 * @returns {array}
 */
export function afwUnionHexBinary(client : any, lists : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "union<hexBinary>";
    _action["lists"] = lists;

    return client.perform(_action);
}

