// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for date
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Add date arg1 to yearMonthDuration arg2 and return the date result.
 * 
 * @param {date} arg1 -
 * 
 * @param {yearMonthDuration} arg2 -
 * 
 * @returns {date}
 */
export function afwAddYearMonthDurationDate(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "add_yearMonthDuration<date>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Returns boolean true if at least one value in date list1 is in date list2.
 * 
 * @param {list} list1 - The first list.
 * 
 * @param {list} list2 - The second list.
 * 
 * @returns {boolean}
 */
export function afwAtLeastOneMemberOfDate(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "at_least_one_member_of<date>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Takes any number of date values and returns a list of list.
 * 
 * @param {list} values -
 * 
 * @returns {list}
 */
export function afwBagDate(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<date>";

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
export function afwBagSizeDate(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size<date>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts value to data type date returning date result.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {date} Converted value
 */
export function afwDate(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "date";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Determine if date arg1 is equal to the value of arg2 converted to the data
 * type of arg1 then return the boolean result. Use "eqx" ("===") instead if
 * you want false to be returned if arg1 and arg2's data type don't match.
 * 
 * @param {date} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqDate(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<date>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for date arg1 is equal to the value and data type of arg2
 * then return the boolean result. Use "eq" ("==") instead if you want arg2
 * to be converted to the data type of arg1 before comparison.
 * 
 * @param {date} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxDate(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<date>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for date arg1 is greater than or equal to date arg2 and return the
 * boolean result.
 * 
 * If a date value does not include a time-zone value, then the local
 * time-zone value will be assigned.
 * 
 * @param {date} arg1 -
 * 
 * @param {date} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeDate(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<date>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for date arg1 is greater than date arg2 and return the boolean
 * result.
 * 
 * If a date value does not include a time-zone value, then the local
 * time-zone value will be assigned.
 * 
 * @param {date} arg1 -
 * 
 * @param {date} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtDate(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<date>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Returns a list of date with the values that are common to both list of
 * date list1 and list2.
 * 
 * @param {list} list1 - The first list.
 * 
 * @param {list} list2 - The second list.
 * 
 * @returns {list}
 */
export function afwIntersectionDate(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "intersection<date>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType date and return the boolean result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsDate(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is<date>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks whether date value is in list of date list and returns the boolean
 * result.
 * 
 * @param {date} value -
 * 
 * @param {list} list -
 * 
 * @returns {boolean}
 */
export function afwIsInDate(client : any, value : any, list : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_in<date>";
    _action["value"] = value;
    _action["list"] = list;

    return client.perform(_action);
}

/**
 * Checks for date arg1 is less than or equal to date arg2 and return the
 * boolean result.
 * 
 * If a date value does not include a time-zone value, then the local
 * time-zone value will be assigned.
 * 
 * @param {date} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeDate(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<date>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for date arg1 is less that date arg2 and return the boolean result.
 * 
 * If a date value does not include a time-zone value, then the local
 * time-zone value will be assigned.
 * 
 * @param {date} arg1 -
 * 
 * @param {date} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtDate(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<date>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Return the date value that is greater than or equal to the others..
 * 
 * If a date value does not include a time-zone value, then the local
 * time-zone value will be assigned.
 * 
 * @param {date} values -
 * 
 * @returns {date}
 */
export function afwMaxDate(client : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "max<date>";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Return the date value that is less than or equal to the others..
 * 
 * If a date value does not include a time-zone value, then the local
 * time-zone value will be assigned.
 * 
 * @param {date} values -
 * 
 * @returns {date}
 */
export function afwMinDate(client : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "min<date>";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Determine if date arg1 is not equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use "nex" ("!==")
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 * 
 * @param {date} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeDate(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<date>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for date arg1 is not equal to the value or data type of arg2
 * then return the boolean result. Use "ne" ("!=") instead if you want arg2
 * to be converted to the data type of arg1 before comparison.
 * 
 * @param {date} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexDate(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<date>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * This converts a list of date values that contains one value to a single
 * date value.
 * 
 * @param {list} list -
 * 
 * @returns {date}
 */
export function afwOneAndOnlyDate(client : any, list : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "one_and_only<date>";
    _action["list"] = list;

    return client.perform(_action);
}

/**
 * Returns boolean true if date list1 and date list2 are subsets of each
 * other and return the boolean result.
 * 
 * @param {list} list1 -
 * 
 * @param {list} list2 -
 * 
 * @returns {boolean}
 */
export function afwSetEqualsDate(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "set_equals<date>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Returns boolean true if the unique values in date list1 are all in date
 * list2.
 * 
 * @param {list} list1 - The first list.
 * 
 * @param {list} list2 - The second list.
 * 
 * @returns {boolean}
 */
export function afwSubsetDate(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "subset<date>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Subtract yearMonthDuration arg2 from date arg1 and return the date result.
 * 
 * @param {date} arg1 -
 * 
 * @param {yearMonthDuration} arg2 -
 * 
 * @returns {date}
 */
export function afwSubtractYearMonthDurationDate(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "subtract_yearMonthDuration<date>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Converts date value to string. For list values, the to_string() value for
 * each entry is returned separated with commas.
 * 
 * @param {date} value - A date value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringDate(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<date>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Returns a list of date contains all of the unique values in two or more
 * list of date values.
 * 
 * @param {list} lists - Two or more lists.
 * 
 * @returns {list}
 */
export function afwUnionDate(client : any, lists : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "union<date>";
    _action["lists"] = lists;

    return client.perform(_action);
}

