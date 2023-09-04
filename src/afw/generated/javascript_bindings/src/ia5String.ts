// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for ia5String
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Takes any number of ia5String values and returns an array of array.
 * 
 * @param {array} values -
 * 
 * @returns {array}
 */
export function afwBagIa5String(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<ia5String>";

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
export function afwBagSizeIa5String(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size<ia5String>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Determine if ia5String arg1 is equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'eqx' ('===') instead
 * if you want false to be returned if arg1 and arg2's data type don't match.
 * 
 * @param {ia5String} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqIa5String(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<ia5String>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for ia5String arg1 is equal to the value and data type of arg2
 * then return the boolean result. Use 'eq' ('==') instead if you want arg2 to
 * be converted to the data type of arg1 before comparison.
 * 
 * @param {ia5String} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxIa5String(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<ia5String>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for ia5String arg1 is greater than or equal to ia5String arg2 and
 * return the boolean result.
 * 
 * @param {ia5String} arg1 -
 * 
 * @param {ia5String} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeIa5String(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<ia5String>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for ia5String arg1 is greater than ia5String arg2 and return the
 * boolean result.
 * 
 * @param {ia5String} arg1 -
 * 
 * @param {ia5String} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtIa5String(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<ia5String>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Converts value to data type ia5String returning ia5String result.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {ia5String} Converted value
 */
export function afwIa5String(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ia5String";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType ia5String and return the boolean result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsIa5String(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is<ia5String>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks for ia5String arg1 is less than or equal to ia5String arg2 and
 * return the boolean result.
 * 
 * @param {ia5String} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeIa5String(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<ia5String>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for ia5String arg1 is less that ia5String arg2 and return the
 * boolean result.
 * 
 * @param {ia5String} arg1 -
 * 
 * @param {ia5String} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtIa5String(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<ia5String>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if ia5String arg1 is not equal to the value of arg2 converted to
 * the data type of arg1 then return the boolean result. Use 'nex' ('!==')
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 * 
 * @param {ia5String} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeIa5String(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<ia5String>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for ia5String arg1 is not equal to the value or data type of
 * arg2 then return the boolean result. Use 'ne' ('!=') instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {ia5String} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexIa5String(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<ia5String>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Converts ia5String value to string. For array values, the to_string() value
 * for each entry is returned separated with commas.
 * 
 * @param {ia5String} value - A ia5String value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringIa5String(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<ia5String>";
    _action["value"] = value;

    return client.perform(_action);
}

