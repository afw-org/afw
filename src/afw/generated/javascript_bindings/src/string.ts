// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for string
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file string.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the string category.
 */

interface IAnyObject {
    [prop : string] : any;
}

/**
 * Add (concatenate) 2 or more string values and return the string result.
 * Every value must be a string, the same as the + operator: to convert values
 * while concatenating, use concat() or string().
 * 
 * @param {string} values -
 * 
 * @returns {string}
 */
export function afwAddString(client : any, values : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "add<string>";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Construct a new array of string from the given values, the same as array()
 * except that the new array is typed: each value, and each entry of a spread
 * array, must be dataType string, and adding a value of another data type to
 * the array later throws. A value of another data type is an error, never
 * converted. This is usually written `array<string>(`...values).
 * 
 * @param {} values - A value of dataType string, or ... followed by an
 *     expression that results in an array, every entry of which is included.
 * 
 * @returns {array} The constructed array of string.
 */
export function afwArrayString(client : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "array_string";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Returns value unchanged if it is dataType string. Otherwise, including when
 * value is undefined, throws an error. It never converts: to get a string
 * from a value of another data type, write a typed literal or call a to_T
 * conversion function where one exists. This is usually written value as
 * string.
 * 
 * @param {} value - Value to assert
 * 
 * @returns {string} value, which is dataType string
 */
export function afwAsString(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "as_string";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Returns boolean true if at least one value in string array1 is in string
 * array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {boolean}
 */
export function afwAtLeastOneMemberOfString(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "at_least_one_member_of<string>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Return the code point at a zero-based index of the string value as an
 * integer, as ECMAScript s.codePointAt(i) does. Indexes count code points, as
 * everywhere in Adaptive Script (not UTF-16 units). Negative indexes count
 * from the end, as at does. If the index is out of range, the result is
 * undefined, as for at.
 * 
 * @param {string} value - The string value.
 * 
 * @param {integer} index - Zero-based code point index, or negative from the
 *     end.
 * 
 * @returns {integer} The code point, or undefined if index is out of range.
 */
export function afwCodePointAtString(client : any, value : string, index : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "code_point_at<string>";
    _action["value"] = value;
    _action["index"] = index;

    return client.perform(_action);
}

/**
 * concat follows its first value. If it is an array, return a new array of
 * the entries of every value in order, as ECMAScript array concat does,
 * except that every value must be an array (write [...a, v] or push(a, v) to
 * add a single value). Otherwise convert every value of any data type to
 * string and return the concatenated result; a value with an undefined value
 * is represented by 'undefined'.
 * 
 * @param {} values - Arrays to concatenate, or values to convert to strings
 *     and concatenate.
 * 
 * @returns {} A new array if the first value is an array, otherwise the
 *     concatenated string.
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
 * Checks for string arg1 is equal to string arg2 ignoring case and return the
 * boolean result.
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
 * data type of arg1 then return the boolean result. Use 'eqx' ('===') instead
 * if you want false to be returned if arg1 and arg2's data type don't match.
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
 * then return the boolean result. Use 'eq' ('==') instead if you want arg2 to
 * be converted to the data type of arg1 before comparison.
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
 * Compile and evaluate string value as adaptive script.
 * 
 * @param {string} source - string to compile and evaluate
 * 
 * @param {object} additionalUntrustedQualifiedVariables - This parameter
 *     supplies additional qualified variables that can be accessed during
 *     evaluation. These variables will not be used by anything that needs to
 *     ensure its qualified variables must come from a trusted source, such as
 *     authorization. This parameter is intended to be used for testing only
 *     and should not be used for anything running in production.
 * 
 * @returns {any}
 */
export function afwEvalString(client : any, source : string, additionalUntrustedQualifiedVariables? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eval<string>";
    _action["source"] = source;

    if (additionalUntrustedQualifiedVariables !== undefined)
        _action["additionalUntrustedQualifiedVariables"] = additionalUntrustedQualifiedVariables;

    return client.perform(_action);
}

/**
 * Return a string of the code points passed, in order, as ECMAScript
 * String.fromCodePoint(...) does. A code point must be from 0 to 0x10FFFF and
 * not a surrogate (U+D800 to U+DFFF): a string can not hold a lone surrogate.
 * The result is NFC, like every string.
 * 
 * @param {integer} codePoints - The code points.
 * 
 * @returns {string} The string of those code points.
 */
export function afwFromCodePoint(client : any, codePoints : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "from_code_point";
    _action["codePoints"] = codePoints;

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
 * Returns an array of string with the values that are common to both array of
 * string array1 and array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {array}
 */
export function afwIntersectionString(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "intersection<string>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Checks whether string value is in array of string array and returns the
 * boolean result. Empty array yields false.
 * 
 * @param {string} value -
 * 
 * @param {array} array -
 * 
 * @returns {boolean}
 */
export function afwIsInString(client : any, value : string, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_in<string>";
    _action["value"] = value;
    _action["array"] = array;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType string and returns the boolean result. It
 * is false for undefined. This can also be written `is<string>(value`).
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsString(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_string";
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
 * Return the integer number of entries in a string array or the number of
 * codepoints in a string anyURI or string.
 * 
 * @param {string} value - Returns the number of entries in an array or code
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
 * Searches string value for the regular expression and returns each match
 * with the text of its groups, left to right, as ECMAScript's s.matchAll(re)
 * does. Matches do not overlap; after an empty match the next search starts
 * one code point later. length(match_all(s, re)) counts the matches.
 * 
 * @param {string} value - The string value to search.
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
export function afwMatchAllString(client : any, value : string, regexp : any, limit? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "match_all<string>";
    _action["value"] = value;
    _action["regexp"] = regexp;

    if (limit !== undefined)
        _action["limit"] = limit;

    return client.perform(_action);
}

/**
 * Searches string value for the regular expression and returns its first
 * match with the text of its groups, or undefined if there is none, as
 * ECMAScript's s.match(re) does without the g flag. Use test for a boolean
 * result.
 * 
 * @param {string} value - The string value to search.
 * 
 * @param {regexp} regexp - The regular expression. A regexp literal (/a+b/ or
 *     "a+b"re), to_regexp(s), or `compile<regexp>(r`) is already compiled;
 *     any other regexp value is compiled for this call.
 * 
 * @returns {object} The first match, or undefined if there is none.
 */
export function afwMatchString(client : any, value : string, regexp : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "match<string>";
    _action["value"] = value;
    _action["regexp"] = regexp;

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
 * Determine if string arg1 is not equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'nex' ('!==') instead
 * if you want true to be returned if arg1 and arg2's data type don't match.
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
 * Determine if for string arg1 is not equal to the value or data type of arg2
 * then return the boolean result. Use 'ne' ('!=') instead if you want arg2 to
 * be converted to the data type of arg1 before comparison.
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
 * Return the string value in a Unicode normalization form, as ECMAScript
 * s.normalize(form) does. Every Adaptive string is already NFC, so form "NFC"
 * (the default) returns it as a string; "NFKC" also replaces compatibility
 * characters ("ﬁ" becomes "fi", "①" becomes "1"). "NFD" and "NFKD" are
 * errors: a string in Adaptive Script is always NFC, so a decomposed result
 * can not be a string.
 * 
 * @param {string} value - The string value.
 * 
 * @param {string} form - "NFC" (the default) or "NFKC".
 * 
 * @returns {string} The normalized value, as a string.
 */
export function afwNormalizeString(client : any, value : string, form? : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "normalize<string>";
    _action["value"] = value;

    if (form !== undefined)
        _action["form"] = form;

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
 * This converts an array of string values that contains one value to a single
 * string value.
 * 
 * @param {array} array -
 * 
 * @returns {string}
 */
export function afwOneAndOnlyString(client : any, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "one_and_only<string>";
    _action["array"] = array;

    return client.perform(_action);
}

/**
 * Pad the string value at the end with fillString until it is maxLength code
 * points long, as ECMAScript s.padEnd() does. Lengths are code points, not
 * UTF-16 units. Unlike ECMAScript, an empty fillString when padding is needed
 * is an error, not a quiet return of a shorter value.
 * 
 * @param {string} value - The string value.
 * 
 * @param {integer} maxLength - The length of the result in code points, at
 *     most 1,000,000. If it is not more than the length of value, value is
 *     returned (as a string).
 * 
 * @param {string} fillString - The text to pad with, repeated and cut to fit
 *     (by code point). The default is one space. An empty fillString is an
 *     error when padding is needed.
 * 
 * @returns {string} The padded value, as a string.
 */
export function afwPadEndString(client : any, value : string, maxLength : number, fillString? : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "pad_end<string>";
    _action["value"] = value;
    _action["maxLength"] = maxLength;

    if (fillString !== undefined)
        _action["fillString"] = fillString;

    return client.perform(_action);
}

/**
 * Pad the string value at the start with fillString until it is maxLength
 * code points long, as ECMAScript s.padStart() does. Lengths are code points,
 * not UTF-16 units. Unlike ECMAScript, an empty fillString when padding is
 * needed is an error, not a quiet return of a shorter value.
 * 
 * @param {string} value - The string value.
 * 
 * @param {integer} maxLength - The length of the result in code points, at
 *     most 1,000,000. If it is not more than the length of value, value is
 *     returned (as a string).
 * 
 * @param {string} fillString - The text to pad with, repeated and cut to fit
 *     (by code point). The default is one space. An empty fillString is an
 *     error when padding is needed.
 * 
 * @returns {string} The padded value, as a string.
 */
export function afwPadStartString(client : any, value : string, maxLength : number, fillString? : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "pad_start<string>";
    _action["value"] = value;
    _action["maxLength"] = maxLength;

    if (fillString !== undefined)
        _action["fillString"] = fillString;

    return client.perform(_action);
}

/**
 * Repeat a string value a specified number of times.
 * 
 * @param {string} value - The string value to repeat.
 * 
 * @param {integer} times - The number of times to repeat the value.
 * 
 * @returns {string} Repeated text as string (not re-typed as the input data
 *     type).
 */
export function afwRepeatString(client : any, value : string, times : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "repeat<string>";
    _action["value"] = value;
    _action["times"] = times;

    return client.perform(_action);
}

/**
 * Replace every match in a string value, left to right, as ECMAScript
 * s.replaceAll(search, r) does; the same as replace(value, search,
 * replacement, -1). search is a string, which is literal text, or a regexp,
 * which is a pattern (no g flag is needed). The replacement is a string, or a
 * function called with each match's _AdaptiveRegexpMatch_ object that returns
 * the string to put in.
 * 
 * @param {string} value - The string value.
 * 
 * @param {} search - What to replace: a string is literal text, and an empty
 *     string matches at each code-point boundary, including the start and the
 *     end; a regexp is a pattern. A regexp literal (/a+b/ or "a+b"re),
 *     to_regexp(s), or `compile<regexp>(r`) is already compiled; any other
 *     regexp value is compiled for this call.
 * 
 * @param {} replacement - The replacement for each match: a string, or a
 *     function. A function is called with one argument, the match's
 *     _AdaptiveRegexpMatch_ object ({match, index, captures, groups}), and
 *     must return a string. A string replacement is literal text when search
 *     is a string. When search is a regexp, $n is the text of group n ($0 the
 *     whole match), ${name} the text of a named group, and a backslash makes
 *     the next character literal (\$ for a dollar sign); any other $ is an
 *     error.
 * 
 * @returns {string} The value with every match replaced, as a string.
 */
export function afwReplaceAllString(client : any, value : string, search : any, replacement : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "replace_all<string>";
    _action["value"] = value;
    _action["search"] = search;
    _action["replacement"] = replacement;

    return client.perform(_action);
}

/**
 * Replace matches in a string value, left to right, up to limit times, as
 * ECMAScript's s.replace(search, r) does. search is a string, which is
 * literal text, or a regexp, which is a pattern. The replacement is a string,
 * or a function called with each match's _AdaptiveRegexpMatch_ object that
 * returns the string to put in. An empty match is a match at a code-point
 * boundary. The default limit replaces once (an empty string inserts at the
 * start). Specify limit -1 to replace every match.
 * 
 * @param {string} value - The original string value.
 * 
 * @param {} search - What to replace: a string is literal text, and an empty
 *     string matches at each code-point boundary, including the start and the
 *     end; a regexp is a pattern. A regexp literal (/a+b/ or "a+b"re),
 *     to_regexp(s), or `compile<regexp>(r`) is already compiled; any other
 *     regexp value is compiled for this call.
 * 
 * @param {} replacement - The replacement for each match: a string, or a
 *     function. A function is called with one argument, the match's
 *     _AdaptiveRegexpMatch_ object ({match, index, captures, groups}), and
 *     must return a string. A string replacement is literal text when search
 *     is a string. When search is a regexp, $n is the text of group n ($0 the
 *     whole match), ${name} the text of a named group, and a backslash makes
 *     the next character literal (\$ for a dollar sign); any other $ is an
 *     error.
 * 
 * @param {integer} limit - The maximum number of matches to replace. The
 *     default is 1. Specify -1 (or any negative) to replace every match.
 * 
 * @returns {string} Result text as string (not re-typed as the input data
 *     type).
 */
export function afwReplaceString(client : any, value : string, search : any, replacement : any, limit? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "replace<string>";
    _action["value"] = value;
    _action["search"] = search;
    _action["replacement"] = replacement;

    if (limit !== undefined)
        _action["limit"] = limit;

    return client.perform(_action);
}

/**
 * Searches string value for the regular expression and returns the code point
 * index of the first match, or -1 if there is none, as ECMAScript's
 * s.search(re) does. Use index_of to find literal text.
 * 
 * @param {string} value - The string value to search.
 * 
 * @param {regexp} regexp - The regular expression. A regexp literal (/a+b/ or
 *     "a+b"re), to_regexp(s), or `compile<regexp>(r`) is already compiled;
 *     any other regexp value is compiled for this call.
 * 
 * @returns {integer} Zero-based code point index of the first match, or -1.
 */
export function afwSearchString(client : any, value : string, regexp : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "search<string>";
    _action["value"] = value;
    _action["regexp"] = regexp;

    return client.perform(_action);
}

/**
 * Returns boolean true if string array1 and string array2 are subsets of each
 * other and return the boolean result.
 * 
 * @param {array} array1 -
 * 
 * @param {array} array2 -
 * 
 * @returns {boolean}
 */
export function afwSetEqualsString(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "set_equals<string>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Split string value into an array of strings, as ECMAScript's
 * s.split(separator) does. separator is a string, which is literal text, or a
 * regexp, which is a pattern. With a regexp, the text of each group of a
 * match is put in the array after the piece before it (undefined when the
 * group did not take part), and an empty match splits between code points but
 * not at the start or end of the value.
 * 
 * @param {string} value - The string value to split.
 * 
 * @param {} separator - Where to split: a string is literal text, a regexp is
 *     a pattern. If this is an empty string or separator is not specified,
 *     the value is split between characters. A regexp literal (/a+b/ or
 *     "a+b"re), to_regexp(s), or `compile<regexp>(r`) is already compiled;
 *     any other regexp value is compiled for this call.
 * 
 * @param {integer} limit - The maximum number of entries in the result. Any
 *     remaining part of value is ignored. The default, or any negative limit,
 *     is no limit.
 * 
 * @returns {array} An array of strings.
 */
export function afwSplitString(client : any, value : string, separator? : any, limit? : number) : any {

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
 * Conversion function for string: convert one or more values of any data type
 * to string and return the concatenated result. With one argument this is
 * convert-to-string; with more arguments each is converted then concatenated
 * in order (no separator). A value with an undefined value is represented by
 * 'undefined'.
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
 * Returns boolean true if the unique values in string array1 are all in
 * string array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {boolean}
 */
export function afwSubsetString(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "subset<string>";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Returns the string substring of value from zero-based code point position
 * integer startIndex up to, but not including, endIndex, as ECMAScript
 * substring does. A negative index is 0 and an index past the end is the end
 * of value. Omit endIndex to return through the end of value. If endIndex is
 * less than startIndex, the two are swapped, and equal indexes return an
 * empty string. Use slice to count from the end. The result is always string
 * (a slice of anyURI is not an anyURI).
 * 
 * @param {string} string -
 * 
 * @param {integer} startIndex -
 * 
 * @param {integer} endIndex -
 * 
 * @returns {string} Substring as string (not re-typed as the input data
 *     type).
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
 * Converts string value to anyURI. A string is read with the anyURI parser,
 * the same parser the typed literal "..."anyURI uses, and text the parser
 * rejects is an error.
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
 * Converts string value to base64Binary. A string is read with the
 * base64Binary parser, the same parser the typed literal "..."base64Binary
 * uses, and text the parser rejects is an error.
 * 
 * @param {string} value -
 * 
 * @returns {base64Binary}
 */
export function afwToBase64BinaryString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_base64Binary<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts string value to dateTime. A string is read with the dateTime
 * parser, the same parser the typed literal "..."dateTime uses, and text the
 * parser rejects is an error.
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
 * Converts string value to date. A string is read with the date parser, the
 * same parser the typed literal "..."date uses, and text the parser rejects
 * is an error.
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
 * Converts string value to dayTimeDuration. A string is read with the
 * dayTimeDuration parser, the same parser the typed literal
 * "..."dayTimeDuration uses, and text the parser rejects is an error.
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
 * Converts string value to dnsName. A string is read with the dnsName parser,
 * the same parser the typed literal "..."dnsName uses, and text the parser
 * rejects is an error.
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
 * Converts string value to double. A string is read as double text, the same
 * text a double literal accepts, and other text is an error: integer text
 * such as "5" is not a double, so use to_double(to_integer(s)) for it.
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
 * Converts string value to hexBinary. A string is read with the hexBinary
 * parser, the same parser the typed literal "..."hexBinary uses, and text the
 * parser rejects is an error.
 * 
 * @param {string} value -
 * 
 * @returns {hexBinary}
 */
export function afwToHexBinaryString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_hexBinary<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts string value to ia5String. A string is read with the ia5String
 * parser, the same parser the typed literal "..."ia5String uses, and text the
 * parser rejects is an error.
 * 
 * @param {string} value -
 * 
 * @returns {ia5String}
 */
export function afwToIa5StringString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_ia5String<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts string value to integer. A double is truncated toward zero (1.9 is
 * 1 and -1.9 is -1); use round(), floor(), or ceil() first for another
 * choice. A double that is NaN, infinite, or outside the integer range is an
 * error. A string is read as integer text, and text that is not an integer
 * ("1.5", " 1", "1x") is an error.
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
 * Converts string value to ipAddress. A string is read with the ipAddress
 * parser, the same parser the typed literal "..."ipAddress uses, and text the
 * parser rejects is an error.
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
 * Return the string value in lower case, as ECMAScript s.toLowerCase() does:
 * Unicode full case mapping, the same in every locale ("İ" becomes "i̇"). The
 * result is a string. normalize_to_lower_case maps one code point at a time.
 * 
 * @param {string} value - The string value.
 * 
 * @returns {string} The value in lower case, as a string.
 */
export function afwToLowerCaseString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_lower_case<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts string value to objectId. A string is read with the objectId
 * parser, and text the parser rejects is an error.
 * 
 * @param {string} value -
 * 
 * @returns {objectId}
 */
export function afwToObjectIdString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_objectId<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts string value to objectPath. A string is read with the objectPath
 * parser, and text the parser rejects is an error.
 * 
 * @param {string} value -
 * 
 * @returns {objectPath}
 */
export function afwToObjectPathString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_objectPath<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts string value to password. A string is read with the password
 * parser, the same parser the typed literal "..."password uses, and text the
 * parser rejects is an error.
 * 
 * @param {string} value -
 * 
 * @returns {password}
 */
export function afwToPasswordString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_password<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts string value to a compiled regexp, the same value as a regexp
 * literal. A pattern ICU does not accept is a conversion_error.
 * 
 * @param {string} value -
 * 
 * @returns {regexp}
 */
export function afwToRegexpString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_regexp<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts string value to rfc822Name. A string is read with the rfc822Name
 * parser, the same parser the typed literal "..."rfc822Name uses, and text
 * the parser rejects is an error.
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
 * Converts string value to string. For array values, the to_string() value
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
 * Converts string value to time. A string is read with the time parser, the
 * same parser the typed literal "..."time uses, and text the parser rejects
 * is an error.
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
 * Return the string value in upper case, as ECMAScript s.toUpperCase() does:
 * Unicode full case mapping, the same in every locale, so one code point can
 * become more than one ("ß" becomes "SS"). The result is a string.
 * 
 * @param {string} value - The string value.
 * 
 * @returns {string} The value in upper case, as a string.
 */
export function afwToUpperCaseString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_upper_case<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts string value to x500Name. A string is read with the x500Name
 * parser, the same parser the typed literal "..."x500Name uses, and text the
 * parser rejects is an error.
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
 * Converts string value to yearMonthDuration. A string is read with the
 * yearMonthDuration parser, the same parser the typed literal
 * "..."yearMonthDuration uses, and text the parser rejects is an error.
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
 * Remove white space and line terminators as ECMAScript counts them (tab,
 * vertical tab, form feed, space, no-break space, U+FEFF, every Unicode space
 * separator, LF, CR, U+2028 and U+2029) from the end of the string value, as
 * ECMAScript s.trimEnd() does. The result is a string.
 * 
 * @param {string} value - The string value.
 * 
 * @returns {string} The value without trailing white space, as a string.
 */
export function afwTrimEndString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "trim_end<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Remove white space and line terminators as ECMAScript counts them (tab,
 * vertical tab, form feed, space, no-break space, U+FEFF, every Unicode space
 * separator, LF, CR, U+2028 and U+2029) from the beginning of the string
 * value, as ECMAScript s.trimStart() does. The result is a string.
 * 
 * @param {string} value - The string value.
 * 
 * @returns {string} The value without leading white space, as a string.
 */
export function afwTrimStartString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "trim_start<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Remove white space and line terminators as ECMAScript counts them (tab,
 * vertical tab, form feed, space, no-break space, U+FEFF, every Unicode space
 * separator, LF, CR, U+2028 and U+2029) from the beginning and end of the
 * string value, as ECMAScript s.trim() does. White space inside the value is
 * kept; normalize_space also collapses it. The result is a string.
 * 
 * @param {string} value - The string value.
 * 
 * @returns {string} The value without leading and trailing white space, as a
 *     string.
 */
export function afwTrimString(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "trim<string>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Returns an array of string contains all of the unique values in two or more
 * array of string values.
 * 
 * @param {array} arrays - Two or more arrays.
 * 
 * @returns {array}
 */
export function afwUnionString(client : any, arrays : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "union<string>";
    _action["arrays"] = arrays;

    return client.perform(_action);
}

/**
 * URL decode a string.
 * 
 * @param {string} encoded - String to URL decode.
 * 
 * @returns {string} The URL-decoded string.
 */
export function afwUrlDecode(client : any, encoded : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "url_decode";
    _action["encoded"] = encoded;

    return client.perform(_action);
}

/**
 * URL encode a string value and return the string result.
 * 
 * @param {string} unencoded - The string value to URL encode.
 * 
 * @returns {string} URI encoded string.
 */
export function afwUrlEncodeString(client : any, unencoded : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "url_encode<string>";
    _action["unencoded"] = unencoded;

    return client.perform(_action);
}

