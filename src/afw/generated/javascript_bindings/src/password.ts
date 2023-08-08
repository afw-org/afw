// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for password
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Takes any number of password values and returns a list of list.
 * 
 * @param {array} values -
 * 
 * @returns {array}
 */
export function afwBagPassword(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<password>";

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
export function afwBagSizePassword(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size<password>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Determine if password arg1 is equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'eqx' ('===')
 * instead if you want false to be returned if arg1 and arg2's data type
 * don't match.
 * 
 * @param {password} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqPassword(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<password>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for password arg1 is equal to the value and data type of arg2
 * then return the boolean result. Use 'eq' ('==') instead if you want arg2
 * to be converted to the data type of arg1 before comparison.
 * 
 * @param {password} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxPassword(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<password>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for password arg1 is greater than or equal to password arg2 and
 * return the boolean result.
 * 
 * @param {password} arg1 -
 * 
 * @param {password} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGePassword(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<password>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for password arg1 is greater than password arg2 and return the
 * boolean result.
 * 
 * @param {password} arg1 -
 * 
 * @param {password} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtPassword(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<password>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType password and return the boolean result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsPassword(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is<password>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks for password arg1 is less than or equal to password arg2 and return
 * the boolean result.
 * 
 * @param {password} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLePassword(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<password>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for password arg1 is less that password arg2 and return the boolean
 * result.
 * 
 * @param {password} arg1 -
 * 
 * @param {password} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtPassword(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<password>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if password arg1 is not equal to the value of arg2 converted to
 * the data type of arg1 then return the boolean result. Use 'nex' ('!==')
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 * 
 * @param {password} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNePassword(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<password>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for password arg1 is not equal to the value or data type of
 * arg2 then return the boolean result. Use 'ne' ('!=') instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {password} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexPassword(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<password>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Converts value to data type password returning password result.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {password} Converted value
 */
export function afwPassword(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "password";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts password value to string. For list values, the to_string() value
 * for each entry is returned separated with commas.
 * 
 * @param {password} value - A password value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringPassword(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<password>";
    _action["value"] = value;

    return client.perform(_action);
}

