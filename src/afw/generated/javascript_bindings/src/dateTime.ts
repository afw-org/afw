// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for dateTime
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Add dateTime arg1 to dayTimeDuration arg2 and return the dateTime result.
 * 
 * @param {dateTime} arg1 -
 * 
 * @param {dayTimeDuration} arg2 -
 * 
 * @returns {dateTime}
 */
export function afwAddDayTimeDurationDateTime(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "add_dayTimeDuration<dateTime>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Add dateTime arg1 to yearMonthDuration arg2 and return the dateTime
 * result.
 * 
 * @param {dateTime} arg1 -
 * 
 * @param {yearMonthDuration} arg2 -
 * 
 * @returns {dateTime}
 */
export function afwAddYearMonthDurationDateTime(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "add_yearMonthDuration<dateTime>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Returns boolean true if at least one value in dateTime list1 is in
 * dateTime list2.
 * 
 * @param {array} list1 - The first list.
 * 
 * @param {array} list2 - The second list.
 * 
 * @returns {boolean}
 */
export function afwAtLeastOneMemberOfDateTime(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "at_least_one_member_of<dateTime>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Takes any number of dateTime values and returns a list of list.
 * 
 * @param {array} values -
 * 
 * @returns {array}
 */
export function afwBagDateTime(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<dateTime>";

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
export function afwBagSizeDateTime(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size<dateTime>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts value to data type dateTime returning dateTime result.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {dateTime} Converted value
 */
export function afwDateTime(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "dateTime";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Determine if dateTime arg1 is equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'eqx' ('===')
 * instead if you want false to be returned if arg1 and arg2's data type
 * don't match.
 * 
 * @param {dateTime} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqDateTime(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<dateTime>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for dateTime arg1 is equal to the value and data type of arg2
 * then return the boolean result. Use 'eq' ('==') instead if you want arg2
 * to be converted to the data type of arg1 before comparison.
 * 
 * @param {dateTime} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxDateTime(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<dateTime>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for dateTime arg1 is greater than or equal to dateTime arg2 and
 * return the boolean result.
 * 
 * If a dateTime value does not include a time-zone value, then the local
 * time-zone value will be assigned.
 * 
 * @param {dateTime} arg1 -
 * 
 * @param {dateTime} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeDateTime(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<dateTime>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for dateTime arg1 is greater than dateTime arg2 and return the
 * boolean result.
 * 
 * If a dateTime value does not include a time-zone value, then the local
 * time-zone value will be assigned.
 * 
 * @param {dateTime} arg1 -
 * 
 * @param {dateTime} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtDateTime(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<dateTime>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Returns a list of dateTime with the values that are common to both list of
 * dateTime list1 and list2.
 * 
 * @param {array} list1 - The first list.
 * 
 * @param {array} list2 - The second list.
 * 
 * @returns {array}
 */
export function afwIntersectionDateTime(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "intersection<dateTime>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType dateTime and return the boolean result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsDateTime(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is<dateTime>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks whether dateTime value is in list of dateTime list and returns the
 * boolean result.
 * 
 * @param {dateTime} value -
 * 
 * @param {array} array -
 * 
 * @returns {boolean}
 */
export function afwIsInDateTime(client : any, value : any, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_in<dateTime>";
    _action["value"] = value;
    _action["array"] = array;

    return client.perform(_action);
}

/**
 * Checks for dateTime arg1 is less than or equal to dateTime arg2 and return
 * the boolean result.
 * 
 * If a dateTime value does not include a time-zone value, then the local
 * time-zone value will be assigned.
 * 
 * @param {dateTime} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeDateTime(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<dateTime>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for dateTime arg1 is less that dateTime arg2 and return the boolean
 * result.
 * 
 * If a dateTime value does not include a time-zone value, then the local
 * time-zone value will be assigned.
 * 
 * @param {dateTime} arg1 -
 * 
 * @param {dateTime} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtDateTime(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<dateTime>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Return the dateTime value that is greater than or equal to the others.
 * 
 * If a dateTime value does not include a time-zone value, then the local
 * time-zone value will be assigned.
 * 
 * @param {dateTime} values -
 * 
 * @returns {dateTime}
 */
export function afwMaxDateTime(client : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "max<dateTime>";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Return the dateTime value that is less than or equal to the others.
 * 
 * If a dateTime value does not include a time-zone value, then the local
 * time-zone value will be assigned.
 * 
 * @param {dateTime} values -
 * 
 * @returns {dateTime}
 */
export function afwMinDateTime(client : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "min<dateTime>";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Determine if dateTime arg1 is not equal to the value of arg2 converted to
 * the data type of arg1 then return the boolean result. Use 'nex' ('!==')
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 * 
 * @param {dateTime} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeDateTime(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<dateTime>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for dateTime arg1 is not equal to the value or data type of
 * arg2 then return the boolean result. Use 'ne' ('!=') instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {dateTime} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexDateTime(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<dateTime>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * This converts a list of dateTime values that contains one value to a
 * single dateTime value.
 * 
 * @param {array} array -
 * 
 * @returns {dateTime}
 */
export function afwOneAndOnlyDateTime(client : any, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "one_and_only<dateTime>";
    _action["array"] = array;

    return client.perform(_action);
}

/**
 * Returns boolean true if dateTime list1 and dateTime list2 are subsets of
 * each other and return the boolean result.
 * 
 * @param {array} list1 -
 * 
 * @param {array} list2 -
 * 
 * @returns {boolean}
 */
export function afwSetEqualsDateTime(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "set_equals<dateTime>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Returns boolean true if the unique values in dateTime list1 are all in
 * dateTime list2.
 * 
 * @param {array} list1 - The first list.
 * 
 * @param {array} list2 - The second list.
 * 
 * @returns {boolean}
 */
export function afwSubsetDateTime(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "subset<dateTime>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Subtract dayTimeDuration arg2 from dateTime arg1 and return the dateTime
 * result.
 * 
 * @param {dateTime} arg1 -
 * 
 * @param {dayTimeDuration} arg2 -
 * 
 * @returns {dateTime}
 */
export function afwSubtractDayTimeDurationDateTime(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "subtract_dayTimeDuration<dateTime>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Subtract yearMonthDuration arg2 from dateTime arg1 and return the dateTime
 * result.
 * 
 * @param {dateTime} arg1 -
 * 
 * @param {yearMonthDuration} arg2 -
 * 
 * @returns {dateTime}
 */
export function afwSubtractYearMonthDurationDateTime(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "subtract_yearMonthDuration<dateTime>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Converts dateTime value to string. For list values, the to_string() value
 * for each entry is returned separated with commas.
 * 
 * @param {dateTime} value - A dateTime value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringDateTime(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<dateTime>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Returns a list of dateTime contains all of the unique values in two or
 * more list of dateTime values.
 * 
 * @param {array} lists - Two or more lists.
 * 
 * @returns {array}
 */
export function afwUnionDateTime(client : any, lists : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "union<dateTime>";
    _action["lists"] = lists;

    return client.perform(_action);
}

