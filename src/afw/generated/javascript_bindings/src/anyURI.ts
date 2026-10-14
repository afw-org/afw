// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for anyURI
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file anyURI.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the anyURI category.
 */

interface IAnyObject {
    [prop : string] : any;
}

/**
 * Construct a new array of anyURI from the given values, the same as array()
 * except that the new array is typed: each value, and each entry of a spread
 * array, must be dataType anyURI, and adding a value of another data type to
 * the array later throws. A value of another data type is an error, never
 * converted. This is usually written `array<anyURI>(`...values).
 * 
 * @param {} values - A value of dataType anyURI, or ... followed by an
 *     expression that results in an array, every entry of which is included.
 * 
 * @returns {array} The constructed array of anyURI.
 */
export function afwArrayAnyURI(client : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "array_anyURI";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Returns value unchanged if it is dataType anyURI. Otherwise, including when
 * value is undefined, throws an error. It never converts: to get a anyURI
 * from a value of another data type, write a typed literal or call a to_T
 * conversion function where one exists. This is usually written value as
 * anyURI.
 * 
 * @param {} value - Value to assert
 * 
 * @returns {anyURI} value, which is dataType anyURI
 */
export function afwAsAnyURI(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "as_anyURI";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Returns boolean true if at least one value in anyURI array1 is in anyURI
 * array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {boolean}
 */
export function afwAtLeastOneMemberOfAnyURI(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "at_least_one_member_of<anyURI>";
    _action["array1"] = array1;
    _action["array2"] = array2;

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
 * data type of arg1 then return the boolean result. Use 'eqx' ('===') instead
 * if you want false to be returned if arg1 and arg2's data type don't match.
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
 * then return the boolean result. Use 'eq' ('==') instead if you want arg2 to
 * be converted to the data type of arg1 before comparison.
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
 * Returns an array of anyURI with the values that are common to both array of
 * anyURI array1 and array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {array}
 */
export function afwIntersectionAnyURI(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "intersection<anyURI>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType anyURI and returns the boolean result. It
 * is false for undefined. This can also be written `is<anyURI>(value`).
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsAnyURI(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_anyURI";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks whether anyURI value is in array of anyURI array and returns the
 * boolean result. Empty array yields false.
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
 * Return the integer number of entries in a anyURI array or the number of
 * codepoints in a anyURI anyURI or string.
 * 
 * @param {anyURI} value - Returns the number of entries in an array or code
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
 * Searches anyURI value for the regular expression and returns each match
 * with the text of its groups, left to right, as ECMAScript's s.matchAll(re)
 * does. Matches do not overlap; after an empty match the next search starts
 * one code point later. length(match_all(s, re)) counts the matches.
 * 
 * @param {anyURI} value - The anyURI value to search.
 * 
 * @param {regexp} regexp - The regular expression. A regexp literal (/a+b/ or
 *     "a+b"re), to_regexp(s), or `compile<regexp>(r`) is already compiled;
 *     any other regexp value is compiled for this call.
 * 
 * @param {integer} limit - The maximum number of matches to return. The
 *     default, or any negative limit, returns every match.
 * 
 * @returns {array} The matches, empty if there are none.
 */
export function afwMatchAllAnyURI(client : any, value : any, regexp : any, limit? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "match_all<anyURI>";
    _action["value"] = value;
    _action["regexp"] = regexp;

    if (limit !== undefined)
        _action["limit"] = limit;

    return client.perform(_action);
}

/**
 * Searches anyURI value for the regular expression and returns its first
 * match with the text of its groups, or undefined if there is none, as
 * ECMAScript's s.match(re) does without the g flag. Use test for a boolean
 * result.
 * 
 * @param {anyURI} value - The anyURI value to search.
 * 
 * @param {regexp} regexp - The regular expression. A regexp literal (/a+b/ or
 *     "a+b"re), to_regexp(s), or `compile<regexp>(r`) is already compiled;
 *     any other regexp value is compiled for this call.
 * 
 * @returns {object} The first match, or undefined if there is none.
 */
export function afwMatchAnyURI(client : any, value : any, regexp : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "match<anyURI>";
    _action["value"] = value;
    _action["regexp"] = regexp;

    return client.perform(_action);
}

/**
 * Determine if anyURI arg1 is not equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'nex' ('!==') instead
 * if you want true to be returned if arg1 and arg2's data type don't match.
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
 * Determine if for anyURI arg1 is not equal to the value or data type of arg2
 * then return the boolean result. Use 'ne' ('!=') instead if you want arg2 to
 * be converted to the data type of arg1 before comparison.
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
 * This converts an array of anyURI values that contains one value to a single
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
 * Repeat a anyURI value a specified number of times.
 * 
 * @param {anyURI} value - The anyURI value to repeat.
 * 
 * @param {integer} times - The number of times to repeat the value.
 * 
 * @returns {string} Repeated text as string (not re-typed as the input data
 *     type).
 */
export function afwRepeatAnyURI(client : any, value : any, times : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "repeat<anyURI>";
    _action["value"] = value;
    _action["times"] = times;

    return client.perform(_action);
}

/**
 * Replace matches in a anyURI value, left to right, up to limit times, as
 * ECMAScript's s.replace(search, r) does. search is a string, which is
 * literal text, or a regexp, which is a pattern. An empty match is a match at
 * a code-point boundary. The default limit replaces once (an empty string
 * inserts at the start). Specify limit -1 to replace every match.
 * 
 * @param {anyURI} value - The original anyURI value.
 * 
 * @param {} search - What to replace: a string is literal text, and an empty
 *     string matches at each code-point boundary, including the start and the
 *     end; a regexp is a pattern. A regexp literal (/a+b/ or "a+b"re),
 *     to_regexp(s), or `compile<regexp>(r`) is already compiled; any other
 *     regexp value is compiled for this call.
 * 
 * @param {string} replacement - The replacement for each match. When search
 *     is a string, the replacement is literal text. When search is a regexp,
 *     $n is the text of group n ($0 the whole match), ${name} the text of a
 *     named group, and a backslash makes the next character literal (\$ for a
 *     dollar sign); any other $ is an error.
 * 
 * @param {integer} limit - The maximum number of matches to replace. The
 *     default is 1. Specify -1 (or any negative) to replace every match.
 * 
 * @returns {string} Result text as string (not re-typed as the input data
 *     type).
 */
export function afwReplaceAnyURI(client : any, value : any, search : any, replacement : string, limit? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "replace<anyURI>";
    _action["value"] = value;
    _action["search"] = search;
    _action["replacement"] = replacement;

    if (limit !== undefined)
        _action["limit"] = limit;

    return client.perform(_action);
}

/**
 * Searches anyURI value for the regular expression and returns the code point
 * index of the first match, or -1 if there is none, as ECMAScript's
 * s.search(re) does. Use index_of to find literal text.
 * 
 * @param {anyURI} value - The anyURI value to search.
 * 
 * @param {regexp} regexp - The regular expression. A regexp literal (/a+b/ or
 *     "a+b"re), to_regexp(s), or `compile<regexp>(r`) is already compiled;
 *     any other regexp value is compiled for this call.
 * 
 * @returns {integer} Zero-based code point index of the first match, or -1.
 */
export function afwSearchAnyURI(client : any, value : any, regexp : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "search<anyURI>";
    _action["value"] = value;
    _action["regexp"] = regexp;

    return client.perform(_action);
}

/**
 * Returns boolean true if anyURI array1 and anyURI array2 are subsets of each
 * other and return the boolean result.
 * 
 * @param {array} array1 -
 * 
 * @param {array} array2 -
 * 
 * @returns {boolean}
 */
export function afwSetEqualsAnyURI(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "set_equals<anyURI>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Split anyURI value into an array of strings, as ECMAScript's
 * s.split(separator) does. separator is a string, which is literal text, or a
 * regexp, which is a pattern. With a regexp, the text of each group of a
 * match is put in the array after the piece before it (undefined when the
 * group did not take part), and an empty match splits between code points but
 * not at the start or end of the value.
 * 
 * @param {anyURI} value - The anyURI value to split.
 * 
 * @param {} separator - Where to split: a string is literal text, a regexp is
 *     a pattern. If this is an empty string or separator is not specified,
 *     the value is split between characters. A regexp literal (/a+b/ or
 *     "a+b"re), to_regexp(s), or `compile<regexp>(r`) is already compiled;
 *     any other regexp value is compiled for this call.
 * 
 * @param {integer} limit - The maximum number of entries in the result. Any
 *     remaining part of value is ignored.
 * 
 * @returns {array} An array of strings.
 */
export function afwSplitAnyURI(client : any, value : any, separator? : any, limit? : number) : any {

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
 * Returns boolean true if the unique values in anyURI array1 are all in
 * anyURI array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {boolean}
 */
export function afwSubsetAnyURI(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "subset<anyURI>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Returns the string substring of value beginning at zero-based position
 * integer startIndex and ending at the position before integer endIndex.
 * Specify -1 or omit endIndex to return through the end of value. The result
 * is always string (a slice of anyURI is not an anyURI).
 * 
 * @param {anyURI} string -
 * 
 * @param {integer} startIndex -
 * 
 * @param {integer} endIndex -
 * 
 * @returns {string} Substring as string (not re-typed as the input data
 *     type).
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
 * Converts anyURI value to string. For array values, the to_string() value
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
 * Returns an array of anyURI contains all of the unique values in two or more
 * array of anyURI values.
 * 
 * @param {array} arrays - Two or more arrays.
 * 
 * @returns {array}
 */
export function afwUnionAnyURI(client : any, arrays : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "union<anyURI>";
    _action["arrays"] = arrays;

    return client.perform(_action);
}

/**
 * URL encode a anyURI value and return the string result.
 * 
 * @param {anyURI} unencoded - The anyURI value to URL encode.
 * 
 * @returns {string} URI encoded string.
 */
export function afwUrlEncodeAnyURI(client : any, unencoded : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "url_encode<anyURI>";
    _action["unencoded"] = unencoded;

    return client.perform(_action);
}

