// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for string
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Add (concatenate) a string with 1 or more values of any data type
 * converted to their string value and return the string result.
 * 
 * @param {string} string -
 * 
 * @param {} values -
 * 
 * @returns {string}
 */
export function afwAddString(client : any, string : string, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "add<string>";
    _action["string"] = string;
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Returns boolean true if at least one value in string list1 is in string
 * list2.
 * 
 * @param {list} list1 - The first list.
 * 
 * @param {list} list2 - The second list.
 * 
 * @returns {boolean}
 */
export function afwAtLeastOneMemberOfString(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "at_least_one_member_of<string>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * This returns the integer number of values in list.
 * 
 * @param {list} value -
 * 
 * @returns {integer}
 */
export function afwBagSizeString(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Takes any number of string values and returns a list of list.
 * 
 * @param {list} values -
 * 
 * @returns {list}
 */
export function afwBagString(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<string>";

    if (values !== undefined)
        _action["values"] = values;

    return client.perform(_action);
}

/**
 * Convert two or more values of any data type to string and return the
 * concatenated result. A value with an undefined value is represented by
 * `<undefined>`.
 * 
 * @param {} values - Value to convert
 * 
 * @returns {string} The concatenated string values.
 */
export function afwConcat(client : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "concat";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Encode a value as a base64Binary. The effect is to create a base64Binary
 * value with an internal value of the value passed.
 * 
 * @param {string} value - The string value to encode.
 * 
 * @returns {base64Binary} A base64Binary value.
 */
export function afwEncodeAsBase64BinaryString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "encode_as_base64Binary<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Encode a value as a hexBinary. The effect is to create a hexBinary value
 * with an internal value of the value passed.
 * 
 * @param {string} value - The string value to encode.
 * 
 * @returns {hexBinary} A hexBinary value.
 */
export function afwEncodeAsHexBinaryString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "encode_as_hexBinary<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks whether string value ends with a string and return the boolean
 * result.
 * 
 * @param {string} value -
 * 
 * @param {string} subString -
 * 
 * @returns {boolean}
 */
export function afwEndsWithString(client : any, value : string, subString : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ends_with<string>";
    _action["value"] = value;
    _action["subString"] = subString;

    return client.perform(_action);
}

/**
 * Checks for string arg1 is equal to string arg2 ignoring case and return
 * the boolean result.
 * 
 * @param {string} arg1 -
 * 
 * @param {string} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqIgnoreCaseString(client : any, arg1 : string, arg2 : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq_ignore_case<string>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if string arg1 is equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'eqx' ('===')
 * instead if you want false to be returned if arg1 and arg2's data type
 * don't match.
 * 
 * @param {string} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqString(client : any, arg1 : string, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<string>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for string arg1 is equal to the value and data type of arg2
 * then return the boolean result. Use 'eq' ('==') instead if you want arg2
 * to be converted to the data type of arg1 before comparison.
 * 
 * @param {string} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxString(client : any, arg1 : string, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<string>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for string arg1 is greater than or equal to string arg2 and return
 * the boolean result.
 * 
 * @param {string} arg1 -
 * 
 * @param {string} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeString(client : any, arg1 : string, arg2 : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<string>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for string arg1 is greater than string arg2 and return the boolean
 * result.
 * 
 * @param {string} arg1 -
 * 
 * @param {string} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtString(client : any, arg1 : string, arg2 : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<string>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks whether the string value includes a string and return the boolean
 * result.
 * 
 * @param {string} searchString - The string to search.
 * 
 * @param {string} subString - Substring to find.
 * 
 * @param {integer} position - Zero-based position in the search string to
 *     start search.
 * 
 * @returns {boolean} Indicates if the substring is contained in the search
 *     string.
 */
export function afwIncludesString(client : any, searchString : string, subString : string, position? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "includes<string>";
    _action["searchString"] = searchString;
    _action["subString"] = subString;

    if (position !== undefined)
        _action["position"] = position;

    return client.perform(_action);
}

/**
 * Returns the zero-based index into string value of subString. If subString
 * is not found, -1 is returned.
 * 
 * @param {string} value - The string value to search.
 * 
 * @param {string} subString - Substring to search for.
 * 
 * @param {integer} startIndex - Optional start index for search if different
 *     than start of string.
 * 
 * @returns {integer} Zero-based index of subString or -1 if not found.
 */
export function afwIndexOfString(client : any, value : string, subString : string, startIndex? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "index_of<string>";
    _action["value"] = value;
    _action["subString"] = subString;

    if (startIndex !== undefined)
        _action["startIndex"] = startIndex;

    return client.perform(_action);
}

/**
 * Returns a list of string with the values that are common to both list of
 * string list1 and list2.
 * 
 * @param {list} list1 - The first list.
 * 
 * @param {list} list2 - The second list.
 * 
 * @returns {list}
 */
export function afwIntersectionString(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "intersection<string>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Checks whether string value is in list of string list and returns the
 * boolean result.
 * 
 * @param {string} value -
 * 
 * @param {list} list -
 * 
 * @returns {boolean}
 */
export function afwIsInString(client : any, value : string, list : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_in<string>";
    _action["value"] = value;
    _action["list"] = list;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType string and return the boolean result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsString(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Returns the zero-based index into string value of the last occurrence of a
 * subString. If subString is not found, -1 is returned.
 * 
 * @param {string} value - The string value to search.
 * 
 * @param {string} subString - Substring to search for.
 * 
 * @param {integer} startIndex - Optional start index for search if different
 *     than start of string.
 * 
 * @returns {integer} Zero-based index of subString or -1 if not found.
 */
export function afwLastIndexOfString(client : any, value : string, subString : string, startIndex? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "last_index_of<string>";
    _action["value"] = value;
    _action["subString"] = subString;

    if (startIndex !== undefined)
        _action["startIndex"] = startIndex;

    return client.perform(_action);
}

/**
 * Checks for string arg1 is less than or equal to string arg2 and return the
 * boolean result.
 * 
 * @param {string} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeString(client : any, arg1 : string, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<string>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * This is a polymorphic function where string can be any of the supported
 * data types. Return the integer number of entries in datatype list or
 * codepoints in others.
 * 
 * @param {string} value - Returns the number of entries in a list or code
 *     points in others.
 * 
 * @returns {integer}
 */
export function afwLengthString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "length<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks for string arg1 is less that string arg2 and return the boolean
 * result.
 * 
 * @param {string} arg1 -
 * 
 * @param {string} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtString(client : any, arg1 : string, arg2 : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<string>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Return the string value that is greater than or equal to the others.
 * 
 * @param {string} values -
 * 
 * @returns {string}
 */
export function afwMaxString(client : any, values : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "max<string>";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Return the string value that is less than or equal to the others.
 * 
 * @param {string} values -
 * 
 * @returns {string}
 */
export function afwMinString(client : any, values : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "min<string>";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Determine if string arg1 is not equal to the value of arg2 converted to
 * the data type of arg1 then return the boolean result. Use 'nex' ('!==')
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 * 
 * @param {string} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeString(client : any, arg1 : string, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<string>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for string arg1 is not equal to the value or data type of
 * arg2 then return the boolean result. Use 'ne' ('!=') instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {string} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexString(client : any, arg1 : string, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<string>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Remove whitespace from the beginning and end of a string value.
 * 
 * @param {string} string -
 * 
 * @returns {string}
 */
export function afwNormalizeSpaceString(client : any, string : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "normalize_space<string>";
    _action["string"] = string;

    return client.perform(_action);
}

/**
 * Normalize string value to lower case and returns string result.
 * 
 * @param {string} string -
 * 
 * @returns {string}
 */
export function afwNormalizeToLowerCaseString(client : any, string : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "normalize_to_lower_case<string>";
    _action["string"] = string;

    return client.perform(_action);
}

/**
 * This converts a list of string values that contains one value to a single
 * string value.
 * 
 * @param {list} list -
 * 
 * @returns {string}
 */
export function afwOneAndOnlyString(client : any, list : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "one_and_only<string>";
    _action["list"] = list;

    return client.perform(_action);
}

/**
 * Search string value for a regular expression and return index. If not
 * found, -1 is returned.
 * 
 * @param {string} value - The string value to search.
 * 
 * @param {string} regexp - A regular expression to use for search.
 * 
 * @returns {integer} Zero-based index of subString or -1 if not found.
 */
export function afwRegexpIndexOfString(client : any, value : string, regexp : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "regexp_index_of<string>";
    _action["value"] = value;
    _action["regexp"] = regexp;

    return client.perform(_action);
}

/**
 * Checks whether string value matches the regular expression regexp and
 * return the boolean result.
 * 
 * @param {string} value -
 * 
 * @param {string} regexp -
 * 
 * @returns {boolean}
 */
export function afwRegexpMatchString(client : any, value : string, regexp : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "regexp_match<string>";
    _action["value"] = value;
    _action["regexp"] = regexp;

    return client.perform(_action);
}

/**
 * Replace matched values for a regular expression in a string value.
 * 
 * @param {string} value - The original string value.
 * 
 * @param {string} regexp - A regular expression to use for search.
 * 
 * @param {string} replacement - The replacement string.
 * 
 * @param {integer} limit - This is the maximum times to replace. The default
 *     is 1. Specify -1 to replace all occurrences.
 * 
 * @returns {string} A string value with the matched string(s) replaced.
 */
export function afwRegexpReplaceString(client : any, value : string, regexp : string, replacement : string, limit? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "regexp_replace<string>";
    _action["value"] = value;
    _action["regexp"] = regexp;
    _action["replacement"] = replacement;

    if (limit !== undefined)
        _action["limit"] = limit;

    return client.perform(_action);
}

/**
 * Repeat a string value a specified number of times.
 * 
 * @param {string} value - The string value to repeat.
 * 
 * @param {integer} times - The number of times to repeat the value.
 * 
 * @returns {string} The repeated string value.
 */
export function afwRepeatString(client : any, value : string, times : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "repeat<string>";
    _action["value"] = value;
    _action["times"] = times;

    return client.perform(_action);
}

/**
 * Replace string(s) in a string value.
 * 
 * @param {string} value - The original string value.
 * 
 * @param {string} match - The string to replace.
 * 
 * @param {string} replacement - The replacement string.
 * 
 * @param {integer} limit - This is the maximum times to replace. The default
 *     is 1. Specify -1 to replace all occurrences.
 * 
 * @returns {string} A string value with the matched string(s) replaced.
 */
export function afwReplaceString(client : any, value : string, match : string, replacement : string, limit? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "replace<string>";
    _action["value"] = value;
    _action["match"] = match;
    _action["replacement"] = replacement;

    if (limit !== undefined)
        _action["limit"] = limit;

    return client.perform(_action);
}

/**
 * Returns boolean true if string list1 and string list2 are subsets of each
 * other and return the boolean result.
 * 
 * @param {list} list1 -
 * 
 * @param {list} list2 -
 * 
 * @returns {boolean}
 */
export function afwSetEqualsString(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "set_equals<string>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Split string value into an list of strings using a separator.
 * 
 * @param {string} value - The string value to split.
 * 
 * @param {string} separator - The separator to use. If this is an empty
 *     string or separator is not specified, the value is split between
 *     characters.
 * 
 * @param {integer} limit - This is the maximum number of splits. Any
 *     remaining part of value is ignored.
 * 
 * @returns {list} An list of strings.
 */
export function afwSplitString(client : any, value : string, separator? : string, limit? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "split<string>";
    _action["value"] = value;

    if (separator !== undefined)
        _action["separator"] = separator;

    if (limit !== undefined)
        _action["limit"] = limit;

    return client.perform(_action);
}

/**
 * Checks whether string value starts with a subString and return the boolean
 * result.
 * 
 * @param {string} value -
 * 
 * @param {string} subString -
 * 
 * @returns {boolean}
 */
export function afwStartsWithString(client : any, value : string, subString : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "starts_with<string>";
    _action["value"] = value;
    _action["subString"] = subString;

    return client.perform(_action);
}

/**
 * Convert one or more values of any data type to string and return the
 * concatenated result. A value with an undefined value is represented by
 * `<undefined>`.
 * 
 * @param {} values - Value to convert
 * 
 * @returns {string} The concatenated string values.
 */
export function afwString(client : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "string";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Returns boolean true if the unique values in string list1 are all in
 * string list2.
 * 
 * @param {list} list1 - The first list.
 * 
 * @param {list} list2 - The second list.
 * 
 * @returns {boolean}
 */
export function afwSubsetString(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "subset<string>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Returns the string substring of value beginning at zero-based position
 * integer startIndex and ending at the position before integer endIndex.
 * Specify -1 or omitting endIndex to return up to end of string.
 * 
 * @param {string} string -
 * 
 * @param {integer} startIndex -
 * 
 * @param {integer} endIndex -
 * 
 * @returns {string}
 */
export function afwSubstringString(client : any, string : string, startIndex : number, endIndex? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "substring<string>";
    _action["string"] = string;
    _action["startIndex"] = startIndex;

    if (endIndex !== undefined)
        _action["endIndex"] = endIndex;

    return client.perform(_action);
}

/**
 * Converts string value to anyURI.
 * 
 * @param {string} value -
 * 
 * @returns {anyURI}
 */
export function afwToAnyURIString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_anyURI<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts string value to boolean.
 * 
 * @param {string} value -
 * 
 * @returns {boolean}
 */
export function afwToBooleanString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_boolean<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts string value to dateTime.
 * 
 * @param {string} value -
 * 
 * @returns {dateTime}
 */
export function afwToDateTimeString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_dateTime<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts string value to date.
 * 
 * @param {string} value -
 * 
 * @returns {date}
 */
export function afwToDateString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_date<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts string value to dayTimeDuration.
 * 
 * @param {string} value -
 * 
 * @returns {dayTimeDuration}
 */
export function afwToDayTimeDurationString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_dayTimeDuration<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts string value to dnsName.
 * 
 * @param {string} value -
 * 
 * @returns {dnsName}
 */
export function afwToDnsNameString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_dnsName<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts string value to double and returns double result.
 * 
 * @param {string} value -
 * 
 * @returns {double}
 */
export function afwToDoubleString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_double<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Truncate string value to a whole number and returns integer result.
 * 
 * @param {string} value -
 * 
 * @returns {integer}
 */
export function afwToIntegerString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_integer<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts string value to ipAddress.
 * 
 * @param {string} value -
 * 
 * @returns {ipAddress}
 */
export function afwToIpAddressString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_ipAddress<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts string value to rfc822Name.
 * 
 * @param {string} value -
 * 
 * @returns {rfc822Name}
 */
export function afwToRfc822NameString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_rfc822Name<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts string value to string. For list values, the to_string() value
 * for each entry is returned separated with commas.
 * 
 * @param {string} value - A string value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts string value to time.
 * 
 * @param {string} value -
 * 
 * @returns {time}
 */
export function afwToTimeString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_time<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts string value to x500Name.
 * 
 * @param {string} value -
 * 
 * @returns {x500Name}
 */
export function afwToX500NameString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_x500Name<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts string value to yearMonthDuration.
 * 
 * @param {string} value -
 * 
 * @returns {yearMonthDuration}
 */
export function afwToYearMonthDurationString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_yearMonthDuration<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Returns a list of string contains all of the unique values in two or more
 * list of string values.
 * 
 * @param {list} lists - Two or more lists.
 * 
 * @returns {list}
 */
export function afwUnionString(client : any, lists : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "union<string>";
    _action["lists"] = lists;

    return client.perform(_action);
}

/**
 * URL decode a value or bag of values.
 * 
 * @param {string} encoded - URL decode a single string or a bag of string.
 * 
 * @returns {string} A string or bag of strings.
 */
export function afwUrlDecode(client : any, encoded : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "url_decode";
    _action["encoded"] = encoded;

    return client.perform(_action);
}

/**
 * URL encode a value or bag of values.
 * 
 * @param {string} unencoded - URL encode a single value. See the url_encode
 *     method for the data type of more details.
 * 
 * @returns {string} URI encoded string.
 */
export function afwUrlEncodeString(client : any, unencoded : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "url_encode<string>";
    _action["unencoded"] = unencoded;

    return client.perform(_action);
}

