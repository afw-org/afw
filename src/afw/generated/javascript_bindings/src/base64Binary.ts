// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for base64Binary
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Returns boolean true if at least one value in base64Binary list1 is in
 * base64Binary list2.
 * 
 * @param {list} list1 - The first list.
 * 
 * @param {list} list2 - The second list.
 * 
 * @returns {boolean}
 */
export function afwAtLeastOneMemberOfBase64Binary(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "at_least_one_member_of<base64Binary>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Takes any number of base64Binary values and returns a list of list.
 * 
 * @param {list} values -
 * 
 * @returns {list}
 */
export function afwBagBase64Binary(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<base64Binary>";

    if (values !== undefined)
        _action["values"] = values;

    return client.perform(_action);
}

/**
 * This returns the integer number of values in list.
 * 
 * @param {list} value -
 * 
 * @returns {integer}
 */
export function afwBagSizeBase64Binary(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size<base64Binary>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts value to data type base64Binary returning base64Binary result.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {base64Binary} Converted value
 */
export function afwBase64Binary(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "base64Binary";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Decode base64Binary value to string. An error is thrown if decoded value
 * is not valid UTF-8.
 * 
 * @param {base64Binary} value - The base64Binary value to decode.
 * 
 * @returns {string} The decoded string.
 */
export function afwDecodeToStringBase64Binary(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "decode_to_string<base64Binary>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Determine if base64Binary arg1 is equal to the value of arg2 converted to
 * the data type of arg1 then return the boolean result. Use "eqx" ("===")
 * instead if you want false to be returned if arg1 and arg2's data type
 * don't match.
 * 
 * @param {base64Binary} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqBase64Binary(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<base64Binary>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for base64Binary arg1 is equal to the value and data type of
 * arg2 then return the boolean result. Use "eq" ("==") instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {base64Binary} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxBase64Binary(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<base64Binary>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for base64Binary arg1 is greater than or equal to base64Binary arg2
 * and return the boolean result.
 * 
 * @param {base64Binary} arg1 -
 * 
 * @param {base64Binary} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeBase64Binary(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<base64Binary>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for base64Binary arg1 is greater than base64Binary arg2 and return
 * the boolean result.
 * 
 * @param {base64Binary} arg1 -
 * 
 * @param {base64Binary} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtBase64Binary(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<base64Binary>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Returns a list of base64Binary with the values that are common to both
 * list of base64Binary list1 and list2.
 * 
 * @param {list} list1 - The first list.
 * 
 * @param {list} list2 - The second list.
 * 
 * @returns {list}
 */
export function afwIntersectionBase64Binary(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "intersection<base64Binary>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType base64Binary and return the boolean
 * result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsBase64Binary(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is<base64Binary>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks whether base64Binary value is in list of base64Binary list and
 * returns the boolean result.
 * 
 * @param {base64Binary} value -
 * 
 * @param {list} list -
 * 
 * @returns {boolean}
 */
export function afwIsInBase64Binary(client : any, value : any, list : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_in<base64Binary>";
    _action["value"] = value;
    _action["list"] = list;

    return client.perform(_action);
}

/**
 * Checks for base64Binary arg1 is less than or equal to base64Binary arg2
 * and return the boolean result.
 * 
 * @param {base64Binary} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeBase64Binary(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<base64Binary>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for base64Binary arg1 is less that base64Binary arg2 and return the
 * boolean result.
 * 
 * @param {base64Binary} arg1 -
 * 
 * @param {base64Binary} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtBase64Binary(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<base64Binary>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if base64Binary arg1 is not equal to the value of arg2 converted
 * to the data type of arg1 then return the boolean result. Use "nex" ("!==")
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 * 
 * @param {base64Binary} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeBase64Binary(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<base64Binary>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for base64Binary arg1 is not equal to the value or data type
 * of arg2 then return the boolean result. Use "ne" ("!=") instead if you
 * want arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {base64Binary} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexBase64Binary(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<base64Binary>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * This converts a list of base64Binary values that contains one value to a
 * single base64Binary value.
 * 
 * @param {list} list -
 * 
 * @returns {base64Binary}
 */
export function afwOneAndOnlyBase64Binary(client : any, list : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "one_and_only<base64Binary>";
    _action["list"] = list;

    return client.perform(_action);
}

/**
 * Returns boolean true if base64Binary list1 and base64Binary list2 are
 * subsets of each other and return the boolean result.
 * 
 * @param {list} list1 -
 * 
 * @param {list} list2 -
 * 
 * @returns {boolean}
 */
export function afwSetEqualsBase64Binary(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "set_equals<base64Binary>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Returns boolean true if the unique values in base64Binary list1 are all in
 * base64Binary list2.
 * 
 * @param {list} list1 - The first list.
 * 
 * @param {list} list2 - The second list.
 * 
 * @returns {boolean}
 */
export function afwSubsetBase64Binary(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "subset<base64Binary>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Converts base64Binary value to string. For list values, the to_string()
 * value for each entry is returned separated with commas.
 * 
 * @param {base64Binary} value - A base64Binary value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringBase64Binary(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<base64Binary>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Returns a list of base64Binary contains all of the unique values in two or
 * more list of base64Binary values.
 * 
 * @param {list} lists - Two or more lists.
 * 
 * @returns {list}
 */
export function afwUnionBase64Binary(client : any, lists : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "union<base64Binary>";
    _action["lists"] = lists;

    return client.perform(_action);
}

