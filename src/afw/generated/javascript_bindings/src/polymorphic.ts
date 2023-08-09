// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for polymorphic
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Compute the absolute value of the `<dataType>` value and return the
 * `<dataType>` result.
 * 
 * @param {} value -
 * 
 * @returns {}
 */
export function afwAbs(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "abs";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Add 2 or more `<dataType>` values and return the `<dataType>` result.
 * 
 * @param {} values -
 * 
 * @returns {}
 */
export function afwAdd(client : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "add";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Add `<dataType>` arg1 to dayTimeDuration arg2 and return the `<dataType>`
 * result.
 * 
 * @param {} arg1 -
 * 
 * @param {dayTimeDuration} arg2 -
 * 
 * @returns {}
 */
export function afwAddDayTimeDuration(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "add_dayTimeDuration";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Add `<dataType>` arg1 to yearMonthDuration arg2 and return the
 * `<dataType>` result.
 * 
 * @param {} arg1 -
 * 
 * @param {yearMonthDuration} arg2 -
 * 
 * @returns {}
 */
export function afwAddYearMonthDuration(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "add_yearMonthDuration";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Returns boolean true if at least one value in `<dataType>` array1 is in
 * `<dataType>` array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {boolean}
 */
export function afwAtLeastOneMemberOf(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "at_least_one_member_of";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Takes any number of `<dataType>` values and returns an array of array.
 * 
 * @param {array} values -
 * 
 * @returns {array}
 */
export function afwBag(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag";

    if (values !== undefined)
        _action["values"] = values;

    return client.perform(_action);
}

/**
 * This returns the integer number of values in array.
 * 
 * @param {array} value -
 * 
 * @returns {integer}
 */
export function afwBagSize(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Determine the smallest integer that is greater then or equal to the
 * `<dataType>` value and return the `<dataType>` result.
 * 
 * @param {} value -
 * 
 * @returns {}
 */
export function afwCeil(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ceil";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Deep clone a `<dataType>` value.
 * 
 * @param {} value - The `<dataType>` value to clone.
 * 
 * @returns {} The cloned `<dataType>` value.
 */
export function afwClone(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "clone";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Compile `<dataType>` value and return either an unevaluated adaptive value
 * or a string containing the compiler listing.
 * 
 * @param {} source - `<dataType>` string to compile
 * 
 * @param {} listing - If specified, a compiler listing is produced instead
 *     of an unevaluated expression value.
 *     
 *     This parameter can be an integer between 0 and 10 of a string that is
 *     used for indentation. If 0 is specified, no whitespace is added to the
 *     resulting string. If 1 through 10 is specified, that number of spaces
 *     is used.
 * 
 * @returns {unevaluated}
 */
export function afwCompile(client : any, source : any, listing? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "compile";
    _action["source"] = source;

    if (listing !== undefined)
        _action["listing"] = listing;

    return client.perform(_action);
}

/**
 * Decode `<dataType>` value to string. An error is thrown if decoded value
 * is not valid UTF-8.
 * 
 * @param {} value - The `<dataType>` value to decode.
 * 
 * @returns {string} The decoded string.
 */
export function afwDecodeToString(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "decode_to_string";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Divide `<dataType>` dividend by `<dataType>` divisor and return the
 * `<dataType>` quotient.
 * 
 * @param {} dividend -
 * 
 * @param {} divisor -
 * 
 * @returns {}
 */
export function afwDivide(client : any, dividend : any, divisor : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "divide";
    _action["dividend"] = dividend;
    _action["divisor"] = divisor;

    return client.perform(_action);
}

/**
 * Encode a value as a base64Binary. The effect is to create a base64Binary
 * value with an internal value of the value passed.
 * 
 * @param {} value - The `<dataType>` value to encode.
 * 
 * @returns {base64Binary} A base64Binary value.
 */
export function afwEncodeAsBase64Binary(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "encode_as_base64Binary";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Encode a value as a hexBinary. The effect is to create a hexBinary value
 * with an internal value of the value passed.
 * 
 * @param {} value - The `<dataType>` value to encode.
 * 
 * @returns {hexBinary} A hexBinary value.
 */
export function afwEncodeAsHexBinary(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "encode_as_hexBinary";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks whether `<dataType>` value ends with a `<dataType>` and return the
 * boolean result.
 * 
 * @param {} value -
 * 
 * @param {string} subString -
 * 
 * @returns {boolean}
 */
export function afwEndsWith(client : any, value : any, subString : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ends_with";
    _action["value"] = value;
    _action["subString"] = subString;

    return client.perform(_action);
}

/**
 * Determine if `<dataType>` arg1 is equal to the value of arg2 converted to
 * the data type of arg1 then return the boolean result. Use 'eqx' ('===')
 * instead if you want false to be returned if arg1 and arg2's data type
 * don't match.
 * 
 * @param {} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEq(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for `<dataType>` arg1 is equal to `<dataType>` arg2 ignoring case
 * and return the boolean result.
 * 
 * @param {} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqIgnoreCase(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq_ignore_case";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for `<dataType>` arg1 is equal to the value and data type of
 * arg2 then return the boolean result. Use 'eq' ('==') instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqx(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Compile and evaluate `<dataType>` value.
 * 
 * @param {} source - `<dataType>` string to compile and evaluate
 * 
 * @param {object} additionalUntrustedQualifiedVariables - This parameter
 *     supplies additional qualified variables that can be accessed during
 *     evaluation. These variables will not be used by anything that needs to
 *     ensure its qualified variables must come from a trusted source, such
 *     as authorization. This parameter is intended to be used for testing
 *     only and should not be used for anything running in production.
 * 
 * @returns {unevaluated}
 */
export function afwEvaluate(client : any, source : any, additionalUntrustedQualifiedVariables? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "evaluate";
    _action["source"] = source;

    if (additionalUntrustedQualifiedVariables !== undefined)
        _action["additionalUntrustedQualifiedVariables"] = additionalUntrustedQualifiedVariables;

    return client.perform(_action);
}

/**
 * Determine the largest integer that is smaller then or equal to the
 * `<dataType>` value and return the `<dataType>` result.
 * 
 * @param {} number -
 * 
 * @returns {}
 */
export function afwFloor(client : any, number : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "floor";
    _action["number"] = number;

    return client.perform(_action);
}

/**
 * Checks for `<dataType>` arg1 is greater than or equal to `<dataType>` arg2
 * and return the boolean result.
 * 
 * @param {} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGe(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for `<dataType>` arg1 is greater than `<dataType>` arg2 and return
 * the boolean result.
 * 
 * @param {} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGt(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks if `<dataType>` is between startTime and endTime, inclusive.
 * Regardless of endTime value, it is always considered to be equal to, but
 * less than 24 hours greater than startTime. If no `<dataType>` zone is
 * specified for `<dataType>,` the default `<dataType>` zone is used. If no
 * `<dataType>` zone is specified for startTime or endTime, the `<dataType>`
 * zone of `<dataType>` is used.
 * 
 * @param {} time -
 * 
 * @param {} startTime -
 * 
 * @param {} endTime -
 * 
 * @returns {boolean}
 */
export function afwInRange(client : any, time : any, startTime : any, endTime : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "in_range";
    _action["time"] = time;
    _action["startTime"] = startTime;
    _action["endTime"] = endTime;

    return client.perform(_action);
}

/**
 * Checks whether the `<dataType>` value includes a string and return the
 * boolean result.
 * 
 * @param {} searchString - The `<dataType>` to search.
 * 
 * @param {string} subString - Substring to find.
 * 
 * @param {integer} position - Zero-based position in the search string to
 *     start search.
 * 
 * @returns {boolean} Indicates if the substring is contained in the search
 *     string.
 */
export function afwIncludes(client : any, searchString : any, subString : string, position? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "includes";
    _action["searchString"] = searchString;
    _action["subString"] = subString;

    if (position !== undefined)
        _action["position"] = position;

    return client.perform(_action);
}

/**
 * Returns the zero-based index into `<dataType>` value of subString. If
 * subString is not found, -1 is returned.
 * 
 * @param {} value - The `<dataType>` value to search.
 * 
 * @param {string} subString - Substring to search for.
 * 
 * @param {integer} startIndex - Optional start index for search if different
 *     than start of string.
 * 
 * @returns {integer} Zero-based index of subString or -1 if not found.
 */
export function afwIndexOf(client : any, value : any, subString : string, startIndex? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "index_of";
    _action["value"] = value;
    _action["subString"] = subString;

    if (startIndex !== undefined)
        _action["startIndex"] = startIndex;

    return client.perform(_action);
}

/**
 * Returns an array of `<dataType>` with the values that are common to both
 * array of `<dataType>` array1 and array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {array}
 */
export function afwIntersection(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "intersection";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType `<dataType>` and return the boolean
 * result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIs(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks whether `<dataType>` value is in array of `<dataType>` array and
 * returns the boolean result.
 * 
 * @param {} value -
 * 
 * @param {array} array -
 * 
 * @returns {boolean}
 */
export function afwIsIn(client : any, value : any, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_in";
    _action["value"] = value;
    _action["array"] = array;

    return client.perform(_action);
}

/**
 * Returns the zero-based index into `<dataType>` value of the last
 * occurrence of a subString. If subString is not found, -1 is returned.
 * 
 * @param {} value - The `<dataType>` value to search.
 * 
 * @param {string} subString - Substring to search for.
 * 
 * @param {integer} startIndex - Optional start index for search if different
 *     than start of string.
 * 
 * @returns {integer} Zero-based index of subString or -1 if not found.
 */
export function afwLastIndexOf(client : any, value : any, subString : string, startIndex? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "last_index_of";
    _action["value"] = value;
    _action["subString"] = subString;

    if (startIndex !== undefined)
        _action["startIndex"] = startIndex;

    return client.perform(_action);
}

/**
 * Checks for `<dataType>` arg1 is less than or equal to `<dataType>` arg2
 * and return the boolean result.
 * 
 * @param {} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLe(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * This is a polymorphic function where `<dataType>` can be any of the
 * supported data types. Return the integer number of entries in datatype
 * array or codepoints in others.
 * 
 * @param {} value - Returns the number of entries in an array or code points
 *     in others.
 * 
 * @returns {integer}
 */
export function afwLength(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "length";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks for `<dataType>` arg1 is less that `<dataType>` arg2 and return the
 * boolean result.
 * 
 * @param {} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLt(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Returns true if arg1 matches some terminal sequence of RDNs from arg2 when
 * compared using `equal_<dataType>`.
 * 
 * @param {} arg1 -
 * 
 * @param {string} arg2 -
 * 
 * @returns {boolean}
 */
export function afwMatch(client : any, arg1 : any, arg2 : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "match";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Return the `<dataType>` value that is greater than or equal to the others.
 * 
 * @param {} values -
 * 
 * @returns {}
 */
export function afwMax(client : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "max";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Return the `<dataType>` value that is less than or equal to the others.
 * 
 * @param {} values -
 * 
 * @returns {}
 */
export function afwMin(client : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "min";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Divide `<dataType>` dividend by `<dataType>` divisor and return the
 * `<dataType>` remainder.
 * 
 * @param {} dividend -
 * 
 * @param {} divisor -
 * 
 * @returns {}
 */
export function afwMod(client : any, dividend : any, divisor : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "mod";
    _action["dividend"] = dividend;
    _action["divisor"] = divisor;

    return client.perform(_action);
}

/**
 * Multiply 2 or more `<dataType>` values and return the `<dataType>` result.
 * 
 * @param {} values -
 * 
 * @returns {}
 */
export function afwMultiply(client : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "multiply";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Determine if `<dataType>` arg1 is not equal to the value of arg2 converted
 * to the data type of arg1 then return the boolean result. Use 'nex' ('!==')
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 * 
 * @param {} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNe(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Return negative of `<dataType>` value.
 * 
 * @param {} value -
 * 
 * @returns {}
 */
export function afwNegative(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "negative";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Determine if for `<dataType>` arg1 is not equal to the value or data type
 * of arg2 then return the boolean result. Use 'ne' ('!=') instead if you
 * want arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNex(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Remove whitespace from the beginning and end of a `<dataType>` value.
 * 
 * @param {} string -
 * 
 * @returns {}
 */
export function afwNormalizeSpace(client : any, string : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "normalize_space";
    _action["string"] = string;

    return client.perform(_action);
}

/**
 * Normalize `<dataType>` value to lower case and returns `<dataType>`
 * result.
 * 
 * @param {} string -
 * 
 * @returns {}
 */
export function afwNormalizeToLowerCase(client : any, string : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "normalize_to_lower_case";
    _action["string"] = string;

    return client.perform(_action);
}

/**
 * This converts an array of `<dataType>` values that contains one value to a
 * single `<dataType>` value.
 * 
 * @param {array} array -
 * 
 * @returns {}
 */
export function afwOneAndOnly(client : any, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "one_and_only";
    _action["array"] = array;

    return client.perform(_action);
}

/**
 * This returns the value of base raised to a power. Multiple exponents can
 * be specified to raise the previous exponent to the power of the latter
 * exponent.
 * 
 * @param {} base - Base value.
 * 
 * @param {} exponent - Exponent value.
 * 
 * @returns {} Base raised to the power.
 */
export function afwPow(client : any, base : any, exponent : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "pow";
    _action["base"] = base;
    _action["exponent"] = exponent;

    return client.perform(_action);
}

/**
 * Search `<dataType>` value for a regular expression and return index. If
 * not found, -1 is returned.
 * 
 * @param {} value - The `<dataType>` value to search.
 * 
 * @param {string} regexp - A regular expression to use for search.
 * 
 * @returns {integer} Zero-based index of subString or -1 if not found.
 */
export function afwRegexpIndexOf(client : any, value : any, regexp : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "regexp_index_of";
    _action["value"] = value;
    _action["regexp"] = regexp;

    return client.perform(_action);
}

/**
 * Checks whether `<dataType>` value matches the regular expression regexp
 * and return the boolean result.
 * 
 * @param {} value -
 * 
 * @param {string} regexp -
 * 
 * @returns {boolean}
 */
export function afwRegexpMatch(client : any, value : any, regexp : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "regexp_match";
    _action["value"] = value;
    _action["regexp"] = regexp;

    return client.perform(_action);
}

/**
 * Replace matched values for a regular expression in a `<dataType>` value.
 * 
 * @param {} value - The original `<dataType>` value.
 * 
 * @param {string} regexp - A regular expression to use for search.
 * 
 * @param {string} replacement - The replacement string.
 * 
 * @param {integer} limit - This is the maximum times to replace. The default
 *     is 1. Specify -1 to replace all occurrences.
 * 
 * @returns {} A `<dataType>` value with the matched string(s) replaced.
 */
export function afwRegexpReplace(client : any, value : any, regexp : string, replacement : string, limit? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "regexp_replace";
    _action["value"] = value;
    _action["regexp"] = regexp;
    _action["replacement"] = replacement;

    if (limit !== undefined)
        _action["limit"] = limit;

    return client.perform(_action);
}

/**
 * Repeat a `<dataType>` value a specified number of times.
 * 
 * @param {} value - The `<dataType>` value to repeat.
 * 
 * @param {integer} times - The number of times to repeat the value.
 * 
 * @returns {} The repeated `<dataType>` value.
 */
export function afwRepeat(client : any, value : any, times : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "repeat";
    _action["value"] = value;
    _action["times"] = times;

    return client.perform(_action);
}

/**
 * Replace string(s) in a `<dataType>` value.
 * 
 * @param {} value - The original `<dataType>` value.
 * 
 * @param {string} match - The string to replace.
 * 
 * @param {string} replacement - The replacement string.
 * 
 * @param {integer} limit - This is the maximum times to replace. The default
 *     is 1. Specify -1 to replace all occurrences.
 * 
 * @returns {} A `<dataType>` value with the matched string(s) replaced.
 */
export function afwReplace(client : any, value : any, match : string, replacement : string, limit? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "replace";
    _action["value"] = value;
    _action["match"] = match;
    _action["replacement"] = replacement;

    if (limit !== undefined)
        _action["limit"] = limit;

    return client.perform(_action);
}

/**
 * Determine the integer closest to `<dataType>` value and return the
 * `<dataType>` result.
 * 
 * @param {} number -
 * 
 * @returns {}
 */
export function afwRound(client : any, number : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "round";
    _action["number"] = number;

    return client.perform(_action);
}

/**
 * Returns boolean true if `<dataType>` array1 and `<dataType>` array2 are
 * subsets of each other and return the boolean result.
 * 
 * @param {array} array1 -
 * 
 * @param {array} array2 -
 * 
 * @returns {boolean}
 */
export function afwSetEquals(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "set_equals";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Split `<dataType>` value into an array of strings using a separator.
 * 
 * @param {} value - The `<dataType>` value to split.
 * 
 * @param {string} separator - The separator to use. If this is an empty
 *     string or separator is not specified, the value is split between
 *     characters.
 * 
 * @param {integer} limit - This is the maximum number of splits. Any
 *     remaining part of value is ignored.
 * 
 * @returns {array} An array of strings.
 */
export function afwSplit(client : any, value : any, separator? : string, limit? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "split";
    _action["value"] = value;

    if (separator !== undefined)
        _action["separator"] = separator;

    if (limit !== undefined)
        _action["limit"] = limit;

    return client.perform(_action);
}

/**
 * Checks whether `<dataType>` value starts with a subString and return the
 * boolean result.
 * 
 * @param {} value -
 * 
 * @param {string} subString -
 * 
 * @returns {boolean}
 */
export function afwStartsWith(client : any, value : any, subString : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "starts_with";
    _action["value"] = value;
    _action["subString"] = subString;

    return client.perform(_action);
}

/**
 * Returns boolean true if the unique values in `<dataType>` array1 are all
 * in `<dataType>` array2.
 * 
 * @param {array} array1 - The first array.
 * 
 * @param {array} array2 - The second array.
 * 
 * @returns {boolean}
 */
export function afwSubset(client : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "subset";
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Returns the `<dataType>` substring of value beginning at zero-based
 * position integer startIndex and ending at the position before integer
 * endIndex. Specify -1 or omitting endIndex to return up to end of
 * `<dataType>`.
 * 
 * @param {} string -
 * 
 * @param {integer} startIndex -
 * 
 * @param {integer} endIndex -
 * 
 * @returns {}
 */
export function afwSubstring(client : any, string : any, startIndex : number, endIndex? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "substring";
    _action["string"] = string;
    _action["startIndex"] = startIndex;

    if (endIndex !== undefined)
        _action["endIndex"] = endIndex;

    return client.perform(_action);
}

/**
 * Subtract `<dataType>` arg2 from `<dataType>` arg1 and return the
 * `<dataType>` result.
 * 
 * @param {} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {}
 */
export function afwSubtract(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "subtract";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Subtract dayTimeDuration arg2 from `<dataType>` arg1 and return the
 * `<dataType>` result.
 * 
 * @param {} arg1 -
 * 
 * @param {dayTimeDuration} arg2 -
 * 
 * @returns {}
 */
export function afwSubtractDayTimeDuration(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "subtract_dayTimeDuration";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Subtract yearMonthDuration arg2 from `<dataType>` arg1 and return the
 * `<dataType>` result.
 * 
 * @param {} arg1 -
 * 
 * @param {yearMonthDuration} arg2 -
 * 
 * @returns {}
 */
export function afwSubtractYearMonthDuration(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "subtract_yearMonthDuration";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Converts `<dataType>` value to anyURI.
 * 
 * @param {} value -
 * 
 * @returns {anyURI}
 */
export function afwToAnyURI(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_anyURI";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts `<dataType>` value to boolean.
 * 
 * @param {} value -
 * 
 * @returns {boolean}
 */
export function afwToBoolean(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_boolean";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts `<dataType>` value to date.
 * 
 * @param {} value -
 * 
 * @returns {date}
 */
export function afwToDate(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_date";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts `<dataType>` value to dateTime.
 * 
 * @param {} value -
 * 
 * @returns {dateTime}
 */
export function afwToDateTime(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_dateTime";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts `<dataType>` value to dayTimeDuration.
 * 
 * @param {} value -
 * 
 * @returns {dayTimeDuration}
 */
export function afwToDayTimeDuration(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_dayTimeDuration";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts `<dataType>` value to dnsName.
 * 
 * @param {} value -
 * 
 * @returns {dnsName}
 */
export function afwToDnsName(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_dnsName";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts `<dataType>` value to double and returns double result.
 * 
 * @param {} value -
 * 
 * @returns {double}
 */
export function afwToDouble(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_double";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Truncate `<dataType>` value to a whole number and returns integer result.
 * 
 * @param {} value -
 * 
 * @returns {integer}
 */
export function afwToInteger(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_integer";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts `<dataType>` value to ipAddress.
 * 
 * @param {} value -
 * 
 * @returns {ipAddress}
 */
export function afwToIpAddress(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_ipAddress";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts `<dataType>` value to rfc822Name.
 * 
 * @param {} value -
 * 
 * @returns {rfc822Name}
 */
export function afwToRfc822Name(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_rfc822Name";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts `<dataType>` value to string. For array values, the to_string()
 * value for each entry is returned separated with commas.
 * 
 * @param {} value - A `<dataType>` value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToString(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts `<dataType>` value to time.
 * 
 * @param {} value -
 * 
 * @returns {time}
 */
export function afwToTime(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_time";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts `<dataType>` value to x500Name.
 * 
 * @param {} value -
 * 
 * @returns {x500Name}
 */
export function afwToX500Name(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_x500Name";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts `<dataType>` value to yearMonthDuration.
 * 
 * @param {} value -
 * 
 * @returns {yearMonthDuration}
 */
export function afwToYearMonthDuration(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_yearMonthDuration";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Returns an array of `<dataType>` contains all of the unique values in two
 * or more array of `<dataType>` values.
 * 
 * @param {array} arrays - Two or more arrays.
 * 
 * @returns {array}
 */
export function afwUnion(client : any, arrays : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "union";
    _action["arrays"] = arrays;

    return client.perform(_action);
}

/**
 * URL encode a value or bag of values.
 * 
 * @param {} unencoded - URL encode a single value. See the url_encode method
 *     for the data type of more details.
 * 
 * @returns {string} URI encoded string.
 */
export function afwUrlEncode(client : any, unencoded : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "url_encode";
    _action["unencoded"] = unencoded;

    return client.perform(_action);
}

