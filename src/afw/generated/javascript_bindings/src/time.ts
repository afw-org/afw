// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for time
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Returns boolean true if at least one value in time array1 is in time
 * array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {boolean}
 */
export function afwAtLeastOneMemberOfTime(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "at_least_one_member_of<time>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * This returns the integer number of values in array.
 * 
 * @param {array} value -
 * 
 * @returns {integer}
 */
export function afwBagSizeTime(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size<time>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Takes any number of time values and returns an array of array.
 * 
 * @param {array} values -
 * 
 * @returns {array}
 */
export function afwBagTime(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<time>";

    if (values !== undefined)
        _action["values"] = values;

    return client.perform(_action);
}

/**
 * Determine if time arg1 is equal to the value of arg2 converted to the data
 * type of arg1 then return the boolean result. Use 'eqx' ('===') instead if
 * you want false to be returned if arg1 and arg2's data type don't match.
 * 
 * @param {time} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqTime(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<time>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for time arg1 is equal to the value and data type of arg2
 * then return the boolean result. Use 'eq' ('==') instead if you want arg2
 * to be converted to the data type of arg1 before comparison.
 * 
 * @param {time} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxTime(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<time>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for time arg1 is greater than or equal to time arg2 and return the
 * boolean result.
 * 
 * It is illegal to compare a time that includes a time-zone value with one
 * that does not. In such cases, the `in_range<time>` function should be
 * used.
 * 
 * @param {time} arg1 -
 * 
 * @param {time} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeTime(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<time>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for time arg1 is greater than time arg2 and return the boolean
 * result.
 * 
 * It is illegal to compare a time that includes a time-zone value with one
 * that does not. In such cases, the `in_range<time>` function should be
 * used.
 * 
 * @param {time} arg1 -
 * 
 * @param {time} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtTime(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<time>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks if time is between startTime and endTime, inclusive. Regardless of
 * endTime value, it is always considered to be equal to, but less than 24
 * hours greater than startTime. If no time zone is specified for time, the
 * default time zone is used. If no time zone is specified for startTime or
 * endTime, the time zone of time is used.
 * 
 * @param {time} time -
 * 
 * @param {time} startTime -
 * 
 * @param {time} endTime -
 * 
 * @returns {boolean}
 */
export function afwInRangeTime(client : any, time : any, startTime : any, endTime : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "in_range<time>";
    _action["time"] = time;
    _action["startTime"] = startTime;
    _action["endTime"] = endTime;

    return client.perform(_action);
}

/**
 * Returns an array of time with the values that are common to both array of
 * time array1 and array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {array}
 */
export function afwIntersectionTime(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "intersection<time>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Checks whether time value is in array of time array and returns the
 * boolean result.
 * 
 * @param {time} value -
 * 
 * @param {array} array -
 * 
 * @returns {boolean}
 */
export function afwIsInTime(client : any, value : any, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_in<time>";
    _action["value"] = value;
    _action["array"] = array;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType time and return the boolean result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsTime(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is<time>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks for time arg1 is less than or equal to time arg2 and return the
 * boolean result.
 * 
 * It is illegal to compare a time that includes a time-zone value with one
 * that does not. In such cases, the `in_range<time>` function should be
 * used.
 * 
 * @param {time} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeTime(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<time>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for time arg1 is less that time arg2 and return the boolean result.
 * 
 * It is illegal to compare a time that includes a time-zone value with one
 * that does not. In such cases, the `in_range<time>` function should be
 * used.
 * 
 * @param {time} arg1 -
 * 
 * @param {time} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtTime(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<time>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Return the time value that is greater than or equal to the others.
 * 
 * It is illegal to compare a time that includes a time-zone value with one
 * that does not. In such cases, the `in_range<time>` function should be
 * used.
 * 
 * @param {time} values -
 * 
 * @returns {time}
 */
export function afwMaxTime(client : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "max<time>";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Return the time value that is less than or equal to the others.
 * 
 * It is illegal to compare a time that includes a time-zone value with one
 * that does not. In such cases, the `in_range<time>` function should be
 * used.
 * 
 * @param {time} values -
 * 
 * @returns {time}
 */
export function afwMinTime(client : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "min<time>";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Determine if time arg1 is not equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'nex' ('!==')
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 * 
 * @param {time} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeTime(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<time>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for time arg1 is not equal to the value or data type of arg2
 * then return the boolean result. Use 'ne' ('!=') instead if you want arg2
 * to be converted to the data type of arg1 before comparison.
 * 
 * @param {time} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexTime(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<time>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * This converts an array of time values that contains one value to a single
 * time value.
 * 
 * @param {array} array -
 * 
 * @returns {time}
 */
export function afwOneAndOnlyTime(client : any, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "one_and_only<time>";
    _action["array"] = array;

    return client.perform(_action);
}

/**
 * Returns boolean true if time array1 and time array2 are subsets of each
 * other and return the boolean result.
 * 
 * @param {array} array1 -
 * 
 * @param {array} array2 -
 * 
 * @returns {boolean}
 */
export function afwSetEqualsTime(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "set_equals<time>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Returns boolean true if the unique values in time array1 are all in time
 * array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {boolean}
 */
export function afwSubsetTime(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "subset<time>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Converts value to data type time returning time result.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {time} Converted value
 */
export function afwTime(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "time";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts time value to string. For array values, the to_string() value for
 * each entry is returned separated with commas.
 * 
 * @param {time} value - A time value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringTime(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<time>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Returns an array of time contains all of the unique values in two or more
 * array of time values.
 * 
 * @param {array} arrays - Two or more arrays.
 * 
 * @returns {array}
 */
export function afwUnionTime(client : any, arrays : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "union<time>";
    _action["arrays"] = arrays;

    return client.perform(_action);
}

