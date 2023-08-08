// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for anyURI
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Converts value to data type anyURI returning anyURI result.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {anyURI} Converted value
 */
export function afwAnyURI(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "anyURI";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Returns boolean true if at least one value in anyURI list1 is in anyURI
 * list2.
 * 
 * @param {array} list1 - The first list.
 * 
 * @param {array} list2 - The second list.
 * 
 * @returns {boolean}
 */
export function afwAtLeastOneMemberOfAnyURI(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "at_least_one_member_of<anyURI>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Takes any number of anyURI values and returns a list of list.
 * 
 * @param {array} values -
 * 
 * @returns {array}
 */
export function afwBagAnyURI(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<anyURI>";

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
export function afwBagSizeAnyURI(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size<anyURI>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks whether anyURI value ends with a anyURI and return the boolean
 * result.
 * 
 * @param {anyURI} value -
 * 
 * @param {string} subString -
 * 
 * @returns {boolean}
 */
export function afwEndsWithAnyURI(client : any, value : any, subString : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ends_with<anyURI>";
    _action["value"] = value;
    _action["subString"] = subString;

    return client.perform(_action);
}

/**
 * Determine if anyURI arg1 is equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'eqx' ('===')
 * instead if you want false to be returned if arg1 and arg2's data type
 * don't match.
 * 
 * @param {anyURI} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqAnyURI(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<anyURI>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for anyURI arg1 is equal to the value and data type of arg2
 * then return the boolean result. Use 'eq' ('==') instead if you want arg2
 * to be converted to the data type of arg1 before comparison.
 * 
 * @param {anyURI} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxAnyURI(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<anyURI>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for anyURI arg1 is greater than or equal to anyURI arg2 and return
 * the boolean result.
 * 
 * @param {anyURI} arg1 -
 * 
 * @param {anyURI} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeAnyURI(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<anyURI>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for anyURI arg1 is greater than anyURI arg2 and return the boolean
 * result.
 * 
 * @param {anyURI} arg1 -
 * 
 * @param {anyURI} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtAnyURI(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<anyURI>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks whether the anyURI value includes a string and return the boolean
 * result.
 * 
 * @param {anyURI} searchString - The anyURI to search.
 * 
 * @param {string} subString - Substring to find.
 * 
 * @param {integer} position - Zero-based position in the search string to
 *     start search.
 * 
 * @returns {boolean} Indicates if the substring is contained in the search
 *     string.
 */
export function afwIncludesAnyURI(client : any, searchString : any, subString : string, position? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "includes<anyURI>";
    _action["searchString"] = searchString;
    _action["subString"] = subString;

    if (position !== undefined)
        _action["position"] = position;

    return client.perform(_action);
}

/**
 * Returns the zero-based index into anyURI value of subString. If subString
 * is not found, -1 is returned.
 * 
 * @param {anyURI} value - The anyURI value to search.
 * 
 * @param {string} subString - Substring to search for.
 * 
 * @param {integer} startIndex - Optional start index for search if different
 *     than start of string.
 * 
 * @returns {integer} Zero-based index of subString or -1 if not found.
 */
export function afwIndexOfAnyURI(client : any, value : any, subString : string, startIndex? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "index_of<anyURI>";
    _action["value"] = value;
    _action["subString"] = subString;

    if (startIndex !== undefined)
        _action["startIndex"] = startIndex;

    return client.perform(_action);
}

/**
 * Returns a list of anyURI with the values that are common to both list of
 * anyURI list1 and list2.
 * 
 * @param {array} list1 - The first list.
 * 
 * @param {array} list2 - The second list.
 * 
 * @returns {array}
 */
export function afwIntersectionAnyURI(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "intersection<anyURI>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType anyURI and return the boolean result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsAnyURI(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is<anyURI>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks whether anyURI value is in list of anyURI list and returns the
 * boolean result.
 * 
 * @param {anyURI} value -
 * 
 * @param {array} array -
 * 
 * @returns {boolean}
 */
export function afwIsInAnyURI(client : any, value : any, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_in<anyURI>";
    _action["value"] = value;
    _action["array"] = array;

    return client.perform(_action);
}

/**
 * Returns the zero-based index into anyURI value of the last occurrence of a
 * subString. If subString is not found, -1 is returned.
 * 
 * @param {anyURI} value - The anyURI value to search.
 * 
 * @param {string} subString - Substring to search for.
 * 
 * @param {integer} startIndex - Optional start index for search if different
 *     than start of string.
 * 
 * @returns {integer} Zero-based index of subString or -1 if not found.
 */
export function afwLastIndexOfAnyURI(client : any, value : any, subString : string, startIndex? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "last_index_of<anyURI>";
    _action["value"] = value;
    _action["subString"] = subString;

    if (startIndex !== undefined)
        _action["startIndex"] = startIndex;

    return client.perform(_action);
}

/**
 * Checks for anyURI arg1 is less than or equal to anyURI arg2 and return the
 * boolean result.
 * 
 * @param {anyURI} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeAnyURI(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<anyURI>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * This is a polymorphic function where anyURI can be any of the supported
 * data types. Return the integer number of entries in datatype list or
 * codepoints in others.
 * 
 * @param {anyURI} value - Returns the number of entries in a list or code
 *     points in others.
 * 
 * @returns {integer}
 */
export function afwLengthAnyURI(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "length<anyURI>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks for anyURI arg1 is less that anyURI arg2 and return the boolean
 * result.
 * 
 * @param {anyURI} arg1 -
 * 
 * @param {anyURI} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtAnyURI(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<anyURI>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if anyURI arg1 is not equal to the value of arg2 converted to
 * the data type of arg1 then return the boolean result. Use 'nex' ('!==')
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 * 
 * @param {anyURI} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeAnyURI(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<anyURI>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for anyURI arg1 is not equal to the value or data type of
 * arg2 then return the boolean result. Use 'ne' ('!=') instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {anyURI} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexAnyURI(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<anyURI>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * This converts a list of anyURI values that contains one value to a single
 * anyURI value.
 * 
 * @param {array} array -
 * 
 * @returns {anyURI}
 */
export function afwOneAndOnlyAnyURI(client : any, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "one_and_only<anyURI>";
    _action["array"] = array;

    return client.perform(_action);
}

/**
 * Search anyURI value for a regular expression and return index. If not
 * found, -1 is returned.
 * 
 * @param {anyURI} value - The anyURI value to search.
 * 
 * @param {string} regexp - A regular expression to use for search.
 * 
 * @returns {integer} Zero-based index of subString or -1 if not found.
 */
export function afwRegexpIndexOfAnyURI(client : any, value : any, regexp : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "regexp_index_of<anyURI>";
    _action["value"] = value;
    _action["regexp"] = regexp;

    return client.perform(_action);
}

/**
 * Checks whether anyURI value matches the regular expression regexp and
 * return the boolean result.
 * 
 * @param {anyURI} value -
 * 
 * @param {string} regexp -
 * 
 * @returns {boolean}
 */
export function afwRegexpMatchAnyURI(client : any, value : any, regexp : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "regexp_match<anyURI>";
    _action["value"] = value;
    _action["regexp"] = regexp;

    return client.perform(_action);
}

/**
 * Replace matched values for a regular expression in a anyURI value.
 * 
 * @param {anyURI} value - The original anyURI value.
 * 
 * @param {string} regexp - A regular expression to use for search.
 * 
 * @param {string} replacement - The replacement string.
 * 
 * @param {integer} limit - This is the maximum times to replace. The default
 *     is 1. Specify -1 to replace all occurrences.
 * 
 * @returns {anyURI} A anyURI value with the matched string(s) replaced.
 */
export function afwRegexpReplaceAnyURI(client : any, value : any, regexp : string, replacement : string, limit? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "regexp_replace<anyURI>";
    _action["value"] = value;
    _action["regexp"] = regexp;
    _action["replacement"] = replacement;

    if (limit !== undefined)
        _action["limit"] = limit;

    return client.perform(_action);
}

/**
 * Repeat a anyURI value a specified number of times.
 * 
 * @param {anyURI} value - The anyURI value to repeat.
 * 
 * @param {integer} times - The number of times to repeat the value.
 * 
 * @returns {anyURI} The repeated anyURI value.
 */
export function afwRepeatAnyURI(client : any, value : any, times : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "repeat<anyURI>";
    _action["value"] = value;
    _action["times"] = times;

    return client.perform(_action);
}

/**
 * Replace string(s) in a anyURI value.
 * 
 * @param {anyURI} value - The original anyURI value.
 * 
 * @param {string} match - The string to replace.
 * 
 * @param {string} replacement - The replacement string.
 * 
 * @param {integer} limit - This is the maximum times to replace. The default
 *     is 1. Specify -1 to replace all occurrences.
 * 
 * @returns {anyURI} A anyURI value with the matched string(s) replaced.
 */
export function afwReplaceAnyURI(client : any, value : any, match : string, replacement : string, limit? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "replace<anyURI>";
    _action["value"] = value;
    _action["match"] = match;
    _action["replacement"] = replacement;

    if (limit !== undefined)
        _action["limit"] = limit;

    return client.perform(_action);
}

/**
 * Returns boolean true if anyURI list1 and anyURI list2 are subsets of each
 * other and return the boolean result.
 * 
 * @param {array} list1 -
 * 
 * @param {array} list2 -
 * 
 * @returns {boolean}
 */
export function afwSetEqualsAnyURI(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "set_equals<anyURI>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Split anyURI value into an list of strings using a separator.
 * 
 * @param {anyURI} value - The anyURI value to split.
 * 
 * @param {string} separator - The separator to use. If this is an empty
 *     string or separator is not specified, the value is split between
 *     characters.
 * 
 * @param {integer} limit - This is the maximum number of splits. Any
 *     remaining part of value is ignored.
 * 
 * @returns {array} An list of strings.
 */
export function afwSplitAnyURI(client : any, value : any, separator? : string, limit? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "split<anyURI>";
    _action["value"] = value;

    if (separator !== undefined)
        _action["separator"] = separator;

    if (limit !== undefined)
        _action["limit"] = limit;

    return client.perform(_action);
}

/**
 * Checks whether anyURI value starts with a subString and return the boolean
 * result.
 * 
 * @param {anyURI} value -
 * 
 * @param {string} subString -
 * 
 * @returns {boolean}
 */
export function afwStartsWithAnyURI(client : any, value : any, subString : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "starts_with<anyURI>";
    _action["value"] = value;
    _action["subString"] = subString;

    return client.perform(_action);
}

/**
 * Returns boolean true if the unique values in anyURI list1 are all in
 * anyURI list2.
 * 
 * @param {array} list1 - The first list.
 * 
 * @param {array} list2 - The second list.
 * 
 * @returns {boolean}
 */
export function afwSubsetAnyURI(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "subset<anyURI>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Returns the anyURI substring of value beginning at zero-based position
 * integer startIndex and ending at the position before integer endIndex.
 * Specify -1 or omitting endIndex to return up to end of anyURI.
 * 
 * @param {anyURI} string -
 * 
 * @param {integer} startIndex -
 * 
 * @param {integer} endIndex -
 * 
 * @returns {anyURI}
 */
export function afwSubstringAnyURI(client : any, string : any, startIndex : number, endIndex? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "substring<anyURI>";
    _action["string"] = string;
    _action["startIndex"] = startIndex;

    if (endIndex !== undefined)
        _action["endIndex"] = endIndex;

    return client.perform(_action);
}

/**
 * Converts anyURI value to string. For list values, the to_string() value
 * for each entry is returned separated with commas.
 * 
 * @param {anyURI} value - A anyURI value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringAnyURI(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<anyURI>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Returns a list of anyURI contains all of the unique values in two or more
 * list of anyURI values.
 * 
 * @param {array} lists - Two or more lists.
 * 
 * @returns {array}
 */
export function afwUnionAnyURI(client : any, lists : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "union<anyURI>";
    _action["lists"] = lists;

    return client.perform(_action);
}

/**
 * URL encode a value or bag of values.
 * 
 * @param {anyURI} unencoded - URL encode a single value. See the url_encode
 *     method for the data type of more details.
 * 
 * @returns {string} URI encoded string.
 */
export function afwUrlEncodeAnyURI(client : any, unencoded : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "url_encode<anyURI>";
    _action["unencoded"] = unencoded;

    return client.perform(_action);
}

