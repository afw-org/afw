// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for objectPath
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Takes any number of objectPath values and returns a list of list.
 * 
 * @param {list} values -
 * 
 * @returns {list}
 */
export function afwBagObjectPath(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<objectPath>";

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
export function afwBagSizeObjectPath(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size<objectPath>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Determine if objectPath arg1 is equal to the value of arg2 converted to
 * the data type of arg1 then return the boolean result. Use "eqx" ("===")
 * instead if you want false to be returned if arg1 and arg2's data type
 * don't match.
 * 
 * @param {objectPath} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqObjectPath(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<objectPath>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for objectPath arg1 is equal to the value and data type of
 * arg2 then return the boolean result. Use "eq" ("==") instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {objectPath} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxObjectPath(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<objectPath>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for objectPath arg1 is greater than or equal to objectPath arg2 and
 * return the boolean result.
 * 
 * @param {objectPath} arg1 -
 * 
 * @param {objectPath} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeObjectPath(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<objectPath>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for objectPath arg1 is greater than objectPath arg2 and return the
 * boolean result.
 * 
 * @param {objectPath} arg1 -
 * 
 * @param {objectPath} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtObjectPath(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<objectPath>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType objectPath and return the boolean result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsObjectPath(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is<objectPath>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks for objectPath arg1 is less than or equal to objectPath arg2 and
 * return the boolean result.
 * 
 * @param {objectPath} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeObjectPath(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<objectPath>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for objectPath arg1 is less that objectPath arg2 and return the
 * boolean result.
 * 
 * @param {objectPath} arg1 -
 * 
 * @param {objectPath} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtObjectPath(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<objectPath>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if objectPath arg1 is not equal to the value of arg2 converted
 * to the data type of arg1 then return the boolean result. Use "nex" ("!==")
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 * 
 * @param {objectPath} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeObjectPath(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<objectPath>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for objectPath arg1 is not equal to the value or data type of
 * arg2 then return the boolean result. Use "ne" ("!=") instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {objectPath} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexObjectPath(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<objectPath>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Converts value to data type objectPath returning objectPath result.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {objectPath} Converted value
 */
export function afwObjectPath(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "objectPath";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts objectPath value to string. For list values, the to_string()
 * value for each entry is returned separated with commas.
 * 
 * @param {objectPath} value - A objectPath value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringObjectPath(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<objectPath>";
    _action["value"] = value;

    return client.perform(_action);
}

