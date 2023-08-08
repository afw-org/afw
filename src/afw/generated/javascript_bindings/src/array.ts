// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for array
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Add the entries of one or more lists to another.
 * 
 * @param {array} target - Target list. This list must not be immutable.
 * 
 * @param {array} source - Source list(s).
 * 
 * @returns {array} The modified target list.
 */
export function afwAddEntries(client : any, target : any[], source : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "add_entries";
    _action["target"] = target;
    _action["source"] = source;

    return client.perform(_action);
}

/**
 * Construct an array with 0 or more elements.
 * 
 * @param {} values - A value can refer to any adaptable value belonging to
 *     any data type or an array expression. In the case of an array
 *     expression, indicated by '...' followed by an expression that results
 *     in an array, every element within that array is included in the newly
 *     created array.
 * 
 * @returns {array} The constructed array.
 */
export function afwArray(client : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "array";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Takes any number of array values and returns a list of list.
 * 
 * @param {array} values -
 * 
 * @returns {array}
 */
export function afwBagArray(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<array>";

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
export function afwBagSizeArray(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size<array>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Deep clone a array value.
 * 
 * @param {array} value - The array value to clone.
 * 
 * @returns {array} The cloned array value.
 */
export function afwCloneArray(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "clone<array>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Determine if array arg1 is equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'eqx' ('===')
 * instead if you want false to be returned if arg1 and arg2's data type
 * don't match.
 * 
 * @param {array} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqArray(client : any, arg1 : any[], arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<array>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for array arg1 is equal to the value and data type of arg2
 * then return the boolean result. Use 'eq' ('==') instead if you want arg2
 * to be converted to the data type of arg1 before comparison.
 * 
 * @param {array} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxArray(client : any, arg1 : any[], arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<array>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for array arg1 is greater than or equal to array arg2 and return
 * the boolean result.
 * 
 * @param {array} arg1 -
 * 
 * @param {array} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeArray(client : any, arg1 : any[], arg2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<array>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for array arg1 is greater than array arg2 and return the boolean
 * result.
 * 
 * @param {array} arg1 -
 * 
 * @param {array} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtArray(client : any, arg1 : any[], arg2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<array>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks whether or not a list contains any value.
 * 
 * @param {array} array - The list to search.
 * 
 * @param {} searchElement - Element to find.
 * 
 * @param {integer} fromIndex - Index in the list to start search.
 * 
 * @returns {boolean} Indicates if the element is found in list.
 */
export function afwIncludesArray(client : any, array : any[], searchElement : any, fromIndex? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "includes<array>";
    _action["array"] = array;
    _action["searchElement"] = searchElement;

    if (fromIndex !== undefined)
        _action["fromIndex"] = fromIndex;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType array and return the boolean result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsArray(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is<array>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Concatenate the string values of the elements of a list with a separator.
 * 
 * @param {array} value - A list of values (statements) of any data type.
 * 
 * @param {string} separator - The separator to use. If not specified, a
 *     comma (,) is used.
 * 
 * @returns {string} Joined list values.
 */
export function afwJoin(client : any, value : any[], separator? : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "join";
    _action["value"] = value;

    if (separator !== undefined)
        _action["separator"] = separator;

    return client.perform(_action);
}

/**
 * Checks for array arg1 is less than or equal to array arg2 and return the
 * boolean result.
 * 
 * @param {array} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeArray(client : any, arg1 : any[], arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<array>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * This is a polymorphic function where array can be any of the supported
 * data types. Return the integer number of entries in datatype list or
 * codepoints in others.
 * 
 * @param {array} value - Returns the number of entries in a list or code
 *     points in others.
 * 
 * @returns {integer}
 */
export function afwLengthArray(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "length<array>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks for array arg1 is less that array arg2 and return the boolean
 * result.
 * 
 * @param {array} arg1 -
 * 
 * @param {array} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtArray(client : any, arg1 : any[], arg2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<array>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if array arg1 is not equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'nex' ('!==')
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 * 
 * @param {array} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeArray(client : any, arg1 : any[], arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<array>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for array arg1 is not equal to the value or data type of arg2
 * then return the boolean result. Use 'ne' ('!=') instead if you want arg2
 * to be converted to the data type of arg1 before comparison.
 * 
 * @param {array} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexArray(client : any, arg1 : any[], arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<array>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Reverse the order of the elements in a list. If the list is typed, the
 * resulting list will be the same type.
 * 
 * @param {array} array - A list to reverse.
 * 
 * @returns {array} A list with elements reversed.
 */
export function afwReverse(client : any, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "reverse";
    _action["array"] = array;

    return client.perform(_action);
}

/**
 * This function extracts a consecutive slice of values from a list.
 * 
 * @param {array} array - The list to slice.
 * 
 * @param {integer} startIndex - This is the zero based starting index. Use
 *     negative number to index from the end of the list. If not specified,
 *     an index of 0 is assumed.
 * 
 * @param {integer} endIndex - This is the zero based ending index. If
 *     positive, this is one more than the index of the last value to include
 *     in the list. If negative, the index is from the end of the list. If
 *     not specified, the slice is from startIndex up to and including the
 *     end of the list.
 * 
 * @returns {array} A list containing the selected values. If all of the
 *     values are the same data type, the list will be a list of that data
 *     type.
 */
export function afwSlice(client : any, array : any[], startIndex? : number, endIndex? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "slice";
    _action["array"] = array;

    if (startIndex !== undefined)
        _action["startIndex"] = startIndex;

    if (endIndex !== undefined)
        _action["endIndex"] = endIndex;

    return client.perform(_action);
}

/**
 * Converts array value to string. For list values, the to_string() value for
 * each entry is returned separated with commas.
 * 
 * @param {array} value - A array value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringArray(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<array>";
    _action["value"] = value;

    return client.perform(_action);
}

