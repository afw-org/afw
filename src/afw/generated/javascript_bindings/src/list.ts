// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for list
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
 * @param {list} target - Target list. This list must not be immutable.
 * 
 * @param {list} source - Source list(s).
 * 
 * @returns {list} The modified target list.
 */
export function afwAddEntries(client : any, target : any[], source : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "add_entries";
    _action["target"] = target;
    _action["source"] = source;

    return client.perform(_action);
}

/**
 * Takes any number of list values and returns a list of list.
 * 
 * @param {list} values -
 * 
 * @returns {list}
 */
export function afwBagList(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<list>";

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
export function afwBagSizeList(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size<list>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Deep clone a list value.
 * 
 * @param {list} value - The list value to clone.
 * 
 * @returns {list} The cloned list value.
 */
export function afwCloneList(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "clone<list>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Determine if list arg1 is equal to the value of arg2 converted to the data
 * type of arg1 then return the boolean result. Use 'eqx' ('===') instead if
 * you want false to be returned if arg1 and arg2's data type don't match.
 * 
 * @param {list} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqList(client : any, arg1 : any[], arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<list>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for list arg1 is equal to the value and data type of arg2
 * then return the boolean result. Use 'eq' ('==') instead if you want arg2
 * to be converted to the data type of arg1 before comparison.
 * 
 * @param {list} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxList(client : any, arg1 : any[], arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<list>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for list arg1 is greater than or equal to list arg2 and return the
 * boolean result.
 * 
 * @param {list} arg1 -
 * 
 * @param {list} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeList(client : any, arg1 : any[], arg2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<list>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for list arg1 is greater than list arg2 and return the boolean
 * result.
 * 
 * @param {list} arg1 -
 * 
 * @param {list} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtList(client : any, arg1 : any[], arg2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<list>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks whether or not a list contains any value.
 * 
 * @param {list} list - The list to search.
 * 
 * @param {} searchElement - Element to find.
 * 
 * @param {integer} fromIndex - Index in the list to start search.
 * 
 * @returns {boolean} Indicates if the element is found in list.
 */
export function afwIncludesList(client : any, list : any[], searchElement : any, fromIndex? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "includes<list>";
    _action["list"] = list;
    _action["searchElement"] = searchElement;

    if (fromIndex !== undefined)
        _action["fromIndex"] = fromIndex;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType list and return the boolean result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsList(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is<list>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Concatenate the string values of the elements of a list with a separator.
 * 
 * @param {list} value - A list of values (statements) of any data type.
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
 * Checks for list arg1 is less than or equal to list arg2 and return the
 * boolean result.
 * 
 * @param {list} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeList(client : any, arg1 : any[], arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<list>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * This is a polymorphic function where list can be any of the supported data
 * types. Return the integer number of entries in datatype list or codepoints
 * in others.
 * 
 * @param {list} value - Returns the number of entries in a list or code
 *     points in others.
 * 
 * @returns {integer}
 */
export function afwLengthList(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "length<list>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Construct a list with 0 or more elements.
 * 
 * @param {} values - A value can refer to any adaptable value belonging to
 *     any data type or a list expression. In the case of a list expression,
 *     indicated by '...' followed by an expression that results in a list,
 *     every element within that list is included in the newly created list.
 * 
 * @returns {list} The constructed list.
 */
export function afwList(client : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "list";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Checks for list arg1 is less that list arg2 and return the boolean result.
 * 
 * @param {list} arg1 -
 * 
 * @param {list} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtList(client : any, arg1 : any[], arg2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<list>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if list arg1 is not equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'nex' ('!==')
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 * 
 * @param {list} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeList(client : any, arg1 : any[], arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<list>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for list arg1 is not equal to the value or data type of arg2
 * then return the boolean result. Use 'ne' ('!=') instead if you want arg2
 * to be converted to the data type of arg1 before comparison.
 * 
 * @param {list} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexList(client : any, arg1 : any[], arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<list>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Reverse the order of the elements in a list. If the list is typed, the
 * resulting list will be the same type.
 * 
 * @param {list} list - A list to reverse.
 * 
 * @returns {list} A list with elements reversed.
 */
export function afwReverse(client : any, list : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "reverse";
    _action["list"] = list;

    return client.perform(_action);
}

/**
 * This function extracts a consecutive slice of values from a list.
 * 
 * @param {list} list - The list to slice.
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
 * @returns {list} A list containing the selected values. If all of the
 *     values are the same data type, the list will be a list of that data
 *     type.
 */
export function afwSlice(client : any, list : any[], startIndex? : number, endIndex? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "slice";
    _action["list"] = list;

    if (startIndex !== undefined)
        _action["startIndex"] = startIndex;

    if (endIndex !== undefined)
        _action["endIndex"] = endIndex;

    return client.perform(_action);
}

/**
 * Converts list value to string. For list values, the to_string() value for
 * each entry is returned separated with commas.
 * 
 * @param {list} value - A list value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringList(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<list>";
    _action["value"] = value;

    return client.perform(_action);
}

