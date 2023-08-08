// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for dnsName
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Takes any number of dnsName values and returns a list of list.
 * 
 * @param {list} values -
 * 
 * @returns {list}
 */
export function afwBagDnsName(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<dnsName>";

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
export function afwBagSizeDnsName(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size<dnsName>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts value to data type dnsName returning dnsName result.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {dnsName} Converted value
 */
export function afwDnsName(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "dnsName";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Determine if dnsName arg1 is equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'eqx' ('===')
 * instead if you want false to be returned if arg1 and arg2's data type
 * don't match.
 * 
 * @param {dnsName} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqDnsName(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<dnsName>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for dnsName arg1 is equal to the value and data type of arg2
 * then return the boolean result. Use 'eq' ('==') instead if you want arg2
 * to be converted to the data type of arg1 before comparison.
 * 
 * @param {dnsName} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxDnsName(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<dnsName>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for dnsName arg1 is greater than or equal to dnsName arg2 and
 * return the boolean result.
 * 
 * @param {dnsName} arg1 -
 * 
 * @param {dnsName} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeDnsName(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<dnsName>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for dnsName arg1 is greater than dnsName arg2 and return the
 * boolean result.
 * 
 * @param {dnsName} arg1 -
 * 
 * @param {dnsName} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtDnsName(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<dnsName>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType dnsName and return the boolean result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsDnsName(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is<dnsName>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks whether dnsName value is in list of dnsName list and returns the
 * boolean result.
 * 
 * @param {dnsName} value -
 * 
 * @param {list} list -
 * 
 * @returns {boolean}
 */
export function afwIsInDnsName(client : any, value : any, list : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_in<dnsName>";
    _action["value"] = value;
    _action["list"] = list;

    return client.perform(_action);
}

/**
 * Checks for dnsName arg1 is less than or equal to dnsName arg2 and return
 * the boolean result.
 * 
 * @param {dnsName} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeDnsName(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<dnsName>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for dnsName arg1 is less that dnsName arg2 and return the boolean
 * result.
 * 
 * @param {dnsName} arg1 -
 * 
 * @param {dnsName} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtDnsName(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<dnsName>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if dnsName arg1 is not equal to the value of arg2 converted to
 * the data type of arg1 then return the boolean result. Use 'nex' ('!==')
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 * 
 * @param {dnsName} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeDnsName(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<dnsName>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for dnsName arg1 is not equal to the value or data type of
 * arg2 then return the boolean result. Use 'ne' ('!=') instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {dnsName} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexDnsName(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<dnsName>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * This converts a list of dnsName values that contains one value to a single
 * dnsName value.
 * 
 * @param {list} list -
 * 
 * @returns {dnsName}
 */
export function afwOneAndOnlyDnsName(client : any, list : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "one_and_only<dnsName>";
    _action["list"] = list;

    return client.perform(_action);
}

/**
 * Checks whether dnsName value matches the regular expression regexp and
 * return the boolean result.
 * 
 * @param {dnsName} value -
 * 
 * @param {string} regexp -
 * 
 * @returns {boolean}
 */
export function afwRegexpMatchDnsName(client : any, value : any, regexp : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "regexp_match<dnsName>";
    _action["value"] = value;
    _action["regexp"] = regexp;

    return client.perform(_action);
}

/**
 * Converts dnsName value to string. For list values, the to_string() value
 * for each entry is returned separated with commas.
 * 
 * @param {dnsName} value - A dnsName value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringDnsName(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<dnsName>";
    _action["value"] = value;

    return client.perform(_action);
}

