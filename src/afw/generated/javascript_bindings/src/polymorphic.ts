// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for polymorphic
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file polymorphic.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the polymorphic category.
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
 * Return the arccosine, in radians from 0 to #pi, of the `<dataType>` value,
 * as ECMAScript Math.acos does. A value outside -1.0 to 1.0 is an error. A
 * result that would be NaN, or a NaN argument, is an error. An integer is not
 * accepted: write to_double(v).
 * 
 * @param {} value -
 * 
 * @returns {}
 */
export function afwAcos(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "acos";
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
 * Add `<dataType>` arg1 to yearMonthDuration arg2 and return the `<dataType>`
 * result.
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
 * Return the arcsine, in radians from -#pi/2 to #pi/2, of the `<dataType>`
 * value, as ECMAScript Math.asin does. A value outside -1.0 to 1.0 is an
 * error. A result that would be NaN, or a NaN argument, is an error. An
 * integer is not accepted: write to_double(v).
 * 
 * @param {} value -
 * 
 * @returns {}
 */
export function afwAsin(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "asin";
    _action["value"] = value;

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
 * Return the arctangent, in radians from -#pi/2 to #pi/2, of the `<dataType>`
 * value, as ECMAScript Math.atan does. A result that would be NaN, or a NaN
 * argument, is an error. An integer is not accepted: write to_double(v).
 * 
 * @param {} value -
 * 
 * @returns {}
 */
export function afwAtan(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "atan";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Return the angle, in radians from -#pi to #pi, from the positive x axis to
 * the point (x, y), as ECMAScript Math.atan2(y, x) does; y comes first. A
 * result that would be NaN, or a NaN argument, is an error. An integer is not
 * accepted: write to_double(v).
 * 
 * @param {} y - The y coordinate.
 * 
 * @param {} x - The x coordinate.
 * 
 * @returns {}
 */
export function afwAtan2(client : any, y : any, x : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "atan2";
    _action["y"] = y;
    _action["x"] = x;

    return client.perform(_action);
}

/**
 * Return the cube root of the `<dataType>` value, as ECMAScript Math.cbrt
 * does. A result that would be NaN, or a NaN argument, is an error. An
 * integer is not accepted: write to_double(v).
 * 
 * @param {} value -
 * 
 * @returns {}
 */
export function afwCbrt(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "cbrt";
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
 * Return the code point at a zero-based index of the `<dataType>` value as an
 * integer, as ECMAScript s.codePointAt(i) does. Indexes count code points, as
 * everywhere in Adaptive Script (not UTF-16 units). Negative indexes count
 * from the end, as at does. If the index is out of range, the result is
 * undefined, as for at.
 * 
 * @param {} value - The `<dataType>` value.
 * 
 * @param {integer} index - Zero-based code point index, or negative from the
 *     end.
 * 
 * @returns {integer} The code point, or undefined if index is out of range.
 */
export function afwCodePointAt(client : any, value : any, index : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "code_point_at";
    _action["value"] = value;
    _action["index"] = index;

    return client.perform(_action);
}

/**
 * Compile `<dataType>` value and return either an unevaluated adaptive value
 * or a string containing the compiler listing. The listing is a
 * human-oriented dump (value tree interleaved with source, plus ---Symbols
 * tables) for Fiddle and debugging — not pure JSON (use stringify) and not
 * Adaptive compiled-form text (use decompile).
 * 
 * @param {} source - `<dataType>` string to compile
 * 
 * @param {} listing - If specified, a human compiler listing is produced
 *     instead of an unevaluated compiled value (tree + ---Symbols; not
 *     recompilable). Use decompile() for Adaptive compiled-form text and
 *     stringify() for pure JSON of evaluated data.
 * 
 *     This parameter can be an integer between 0 and 10 or a string that is
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
 * Return the cosine of the `<dataType>` value in radians, as ECMAScript
 * Math.cos does (#pi is π). An infinite value is an error. A result that
 * would be NaN, or a NaN argument, is an error. An integer is not accepted:
 * write to_double(v).
 * 
 * @param {} value -
 * 
 * @returns {}
 */
export function afwCos(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "cos";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Decode `<dataType>` value to string. An error is thrown if decoded value is
 * not valid UTF-8.
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
 * instead if you want false to be returned if arg1 and arg2's data type don't
 * match.
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
 * @param {} source - `<dataType>` to compile and evaluate
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
export function afwEval(client : any, source : any, additionalUntrustedQualifiedVariables? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eval";
    _action["source"] = source;

    if (additionalUntrustedQualifiedVariables !== undefined)
        _action["additionalUntrustedQualifiedVariables"] = additionalUntrustedQualifiedVariables;

    return client.perform(_action);
}

/**
 * Return #e raised to the `<dataType>` value, as ECMAScript Math.exp does. A
 * result that would be NaN, or a NaN argument, is an error. An integer is not
 * accepted: write to_double(v).
 * 
 * @param {} value -
 * 
 * @returns {}
 */
export function afwExp(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "exp";
    _action["value"] = value;

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
 * Set a `<dataType>` value immutable so further mutation throws. If already
 * immutable, has no effect. Returns the same value.
 * 
 * @param {} value - The `<dataType>` value to freeze.
 * 
 * @returns {} The same value, now immutable.
 */
export function afwFreeze(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "freeze";
    _action["value"] = value;

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
 * Return the square root of the sum of the squares of the `<dataType>`
 * values, as ECMAScript Math.hypot does, without overflow or underflow in
 * between. At least one value is required (ECMAScript's hypot() is 0); an
 * infinite value makes the result Infinity. A result that would be NaN, or a
 * NaN argument, is an error. An integer is not accepted: write to_double(v).
 * 
 * @param {} values - The values.
 * 
 * @returns {}
 */
export function afwHypot(client : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "hypot";
    _action["values"] = values;

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
 * Checks whether `<dataType>` value is in array of `<dataType>` array and
 * returns the boolean result. Empty array yields false.
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
 * Returns the zero-based index into `<dataType>` value of the last occurrence
 * of a subString. If subString is not found, -1 is returned.
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
 * Checks for `<dataType>` arg1 is less than or equal to `<dataType>` arg2 and
 * return the boolean result.
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
 * Return the integer number of entries in a `<dataType>` array or the number
 * of codepoints in a `<dataType>` anyURI or string.
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
 * Return the natural logarithm (base #e) of the `<dataType>` value, as
 * ECMAScript Math.log does. log(0.0) is -Infinity; a negative value is an
 * error. A result that would be NaN, or a NaN argument, is an error. An
 * integer is not accepted: write to_double(v).
 * 
 * @param {} value -
 * 
 * @returns {}
 */
export function afwLog(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "log";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Return the base 10 logarithm of the `<dataType>` value, as ECMAScript
 * Math.log10 does. log10(0.0) is -Infinity; a negative value is an error. A
 * result that would be NaN, or a NaN argument, is an error. An integer is not
 * accepted: write to_double(v).
 * 
 * @param {} value -
 * 
 * @returns {}
 */
export function afwLog10(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "log10";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Return the base 2 logarithm of the `<dataType>` value, as ECMAScript
 * Math.log2 does. log2(0.0) is -Infinity; a negative value is an error. A
 * result that would be NaN, or a NaN argument, is an error. An integer is not
 * accepted: write to_double(v).
 * 
 * @param {} value -
 * 
 * @returns {}
 */
export function afwLog2(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "log2";
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
 * Searches `<dataType>` value for the regular expression and returns its
 * first match with the text of its groups, or undefined if there is none, as
 * ECMAScript's s.match(re) does without the g flag. Use test for a boolean
 * result.
 * 
 * @param {} value - The `<dataType>` value to search.
 * 
 * @param {regexp} regexp - The regular expression. A regexp literal (/a+b/ or
 *     "a+b"re), to_regexp(s), or `compile<regexp>(r`) is already compiled;
 *     any other regexp value is compiled for this call.
 * 
 * @returns {object} The first match, or undefined if there is none.
 */
export function afwMatch(client : any, value : any, regexp : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "match";
    _action["value"] = value;
    _action["regexp"] = regexp;

    return client.perform(_action);
}

/**
 * Searches `<dataType>` value for the regular expression and returns each
 * match with the text of its groups, left to right, as ECMAScript's
 * s.matchAll(re) does. Matches do not overlap; after an empty match the next
 * search starts one code point later. length(match_all(s, re)) counts the
 * matches.
 * 
 * @param {} value - The `<dataType>` value to search.
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
export function afwMatchAll(client : any, value : any, regexp : any, limit? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "match_all";
    _action["value"] = value;
    _action["regexp"] = regexp;

    if (limit !== undefined)
        _action["limit"] = limit;

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
 * of arg2 then return the boolean result. Use 'ne' ('!=') instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
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
 * Return the `<dataType>` value in a Unicode normalization form, as
 * ECMAScript s.normalize(form) does. Every Adaptive string is already NFC, so
 * form "NFC" (the default) returns it as a string; "NFKC" also replaces
 * compatibility characters ("ﬁ" becomes "fi", "①" becomes "1"). "NFD" and
 * "NFKD" are errors: a string in Adaptive Script is always NFC, so a
 * decomposed result can not be a string.
 * 
 * @param {} value - The `<dataType>` value.
 * 
 * @param {string} form - "NFC" (the default) or "NFKC".
 * 
 * @returns {string} The normalized value, as a string.
 */
export function afwNormalize(client : any, value : any, form? : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "normalize";
    _action["value"] = value;

    if (form !== undefined)
        _action["form"] = form;

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
 * Normalize `<dataType>` value to lower case and returns `<dataType>` result.
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
 * Pad the `<dataType>` value at the end with fillString until it is maxLength
 * code points long, as ECMAScript s.padEnd() does. Lengths are code points,
 * not UTF-16 units. Unlike ECMAScript, an empty fillString when padding is
 * needed is an error, not a quiet return of a shorter value.
 * 
 * @param {} value - The `<dataType>` value.
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
export function afwPadEnd(client : any, value : any, maxLength : number, fillString? : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "pad_end";
    _action["value"] = value;
    _action["maxLength"] = maxLength;

    if (fillString !== undefined)
        _action["fillString"] = fillString;

    return client.perform(_action);
}

/**
 * Pad the `<dataType>` value at the start with fillString until it is
 * maxLength code points long, as ECMAScript s.padStart() does. Lengths are
 * code points, not UTF-16 units. Unlike ECMAScript, an empty fillString when
 * padding is needed is an error, not a quiet return of a shorter value.
 * 
 * @param {} value - The `<dataType>` value.
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
export function afwPadStart(client : any, value : any, maxLength : number, fillString? : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "pad_start";
    _action["value"] = value;
    _action["maxLength"] = maxLength;

    if (fillString !== undefined)
        _action["fillString"] = fillString;

    return client.perform(_action);
}

/**
 * This returns the value of base raised to a power. Multiple exponents can be
 * specified to raise the previous exponent to the power of the latter
 * exponent. An integer result is exact: a negative exponent or a result too
 * large for an integer is an error.
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
 * Repeat a `<dataType>` value a specified number of times.
 * 
 * @param {} value - The `<dataType>` value to repeat.
 * 
 * @param {integer} times - The number of times to repeat the value.
 * 
 * @returns {string} Repeated text as string (not re-typed as the input data
 *     type).
 */
export function afwRepeat(client : any, value : any, times : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "repeat";
    _action["value"] = value;
    _action["times"] = times;

    return client.perform(_action);
}

/**
 * Replace matches in a `<dataType>` value, left to right, up to limit times,
 * as ECMAScript's s.replace(search, r) does. search is a string, which is
 * literal text, or a regexp, which is a pattern. The replacement is a string,
 * or a function called with each match's _AdaptiveRegexpMatch_ object that
 * returns the string to put in. An empty match is a match at a code-point
 * boundary. The default limit replaces once (an empty string inserts at the
 * start). Specify limit -1 to replace every match.
 * 
 * @param {} value - The original `<dataType>` value.
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
export function afwReplace(client : any, value : any, search : any, replacement : any, limit? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "replace";
    _action["value"] = value;
    _action["search"] = search;
    _action["replacement"] = replacement;

    if (limit !== undefined)
        _action["limit"] = limit;

    return client.perform(_action);
}

/**
 * Replace every match in a `<dataType>` value, left to right, as ECMAScript
 * s.replaceAll(search, r) does; the same as replace(value, search,
 * replacement, -1). search is a string, which is literal text, or a regexp,
 * which is a pattern (no g flag is needed). The replacement is a string, or a
 * function called with each match's _AdaptiveRegexpMatch_ object that returns
 * the string to put in.
 * 
 * @param {} value - The `<dataType>` value.
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
export function afwReplaceAll(client : any, value : any, search : any, replacement : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "replace_all";
    _action["value"] = value;
    _action["search"] = search;
    _action["replacement"] = replacement;

    return client.perform(_action);
}

/**
 * Determine the integer closest to `<dataType>` value and return the
 * `<dataType>` result. A value halfway between two integers rounds toward
 * positive infinity, as ECMAScript Math.round and XPath fn:round do:
 * round(2.5) is 3 and round(-2.5) is -2.
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
 * Searches `<dataType>` value for the regular expression and returns the code
 * point index of the first match, or -1 if there is none, as ECMAScript's
 * s.search(re) does. Use index_of to find literal text.
 * 
 * @param {} value - The `<dataType>` value to search.
 * 
 * @param {regexp} regexp - The regular expression. A regexp literal (/a+b/ or
 *     "a+b"re), to_regexp(s), or `compile<regexp>(r`) is already compiled;
 *     any other regexp value is compiled for this call.
 * 
 * @returns {integer} Zero-based code point index of the first match, or -1.
 */
export function afwSearch(client : any, value : any, regexp : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "search";
    _action["value"] = value;
    _action["regexp"] = regexp;

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
 * Return the sign of the `<dataType>` value, as ECMAScript Math.sign does:
 * -1, 0 or 1 of the same data type (`sign<double>` keeps the sign of a zero:
 * sign(-0.0) is -0.0). A NaN argument is an error.
 * 
 * @param {} value -
 * 
 * @returns {}
 */
export function afwSign(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "sign";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Return the sine of the `<dataType>` value in radians, as ECMAScript
 * Math.sin does (#pi is π). An infinite value is an error. A result that
 * would be NaN, or a NaN argument, is an error. An integer is not accepted:
 * write to_double(v).
 * 
 * @param {} value -
 * 
 * @returns {}
 */
export function afwSin(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "sin";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Split `<dataType>` value into an array of strings, as ECMAScript's
 * s.split(separator) does. separator is a string, which is literal text, or a
 * regexp, which is a pattern. With a regexp, the text of each group of a
 * match is put in the array after the piece before it (undefined when the
 * group did not take part), and an empty match splits between code points but
 * not at the start or end of the value.
 * 
 * @param {} value - The `<dataType>` value to split.
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
export function afwSplit(client : any, value : any, separator? : any, limit? : number) : any {

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
 * Return the square root of the `<dataType>` value, as ECMAScript Math.sqrt
 * does; sqrt(-0.0) is -0.0. A negative value is an error. A result that would
 * be NaN, or a NaN argument, is an error. An integer is not accepted: write
 * to_double(v).
 * 
 * @param {} value -
 * 
 * @returns {}
 */
export function afwSqrt(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "sqrt";
    _action["value"] = value;

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
 * Returns boolean true if the unique values in `<dataType>` array1 are all in
 * `<dataType>` array2.
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
 * Returns the string substring of value from zero-based code point position
 * integer startIndex up to, but not including, endIndex, as ECMAScript
 * substring does. A negative index is 0 and an index past the end is the end
 * of value. Omit endIndex to return through the end of value. If endIndex is
 * less than startIndex, the two are swapped, and equal indexes return an
 * empty string. Use slice to count from the end. The result is always string
 * (a slice of anyURI is not an anyURI).
 * 
 * @param {} string -
 * 
 * @param {integer} startIndex -
 * 
 * @param {integer} endIndex -
 * 
 * @returns {string} Substring as string (not re-typed as the input data
 *     type).
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
 * Return the tangent of the `<dataType>` value in radians, as ECMAScript
 * Math.tan does (#pi is π). An infinite value is an error. A result that
 * would be NaN, or a NaN argument, is an error. An integer is not accepted:
 * write to_double(v).
 * 
 * @param {} value -
 * 
 * @returns {}
 */
export function afwTan(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "tan";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts `<dataType>` value to anyURI. A string is read with the anyURI
 * parser, the same parser the typed literal "..."anyURI uses, and text the
 * parser rejects is an error.
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
 * Converts `<dataType>` value to base64Binary. A string is read with the
 * base64Binary parser, the same parser the typed literal "..."base64Binary
 * uses, and text the parser rejects is an error.
 * 
 * @param {} value -
 * 
 * @returns {base64Binary}
 */
export function afwToBase64Binary(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_base64Binary";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts `<dataType>` value to date. A string is read with the date parser,
 * the same parser the typed literal "..."date uses, and text the parser
 * rejects is an error.
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
 * Converts `<dataType>` value to dateTime. A string is read with the dateTime
 * parser, the same parser the typed literal "..."dateTime uses, and text the
 * parser rejects is an error.
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
 * Converts `<dataType>` value to dayTimeDuration. A string is read with the
 * dayTimeDuration parser, the same parser the typed literal
 * "..."dayTimeDuration uses, and text the parser rejects is an error.
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
 * Converts `<dataType>` value to dnsName. A string is read with the dnsName
 * parser, the same parser the typed literal "..."dnsName uses, and text the
 * parser rejects is an error.
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
 * Converts `<dataType>` value to double. A string is read as XML Schema
 * double text: digits with an optional sign, decimal point, and exponent
 * ("5", ".5", "5.", "007.5", "1e3"), INF, +INF, -INF, or NaN. Other text,
 * including leading or trailing spaces, is an error. Only text is read this
 * way: an integer value is never silently a double.
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
 * Converts `<dataType>` value to hexBinary. A string is read with the
 * hexBinary parser, the same parser the typed literal "..."hexBinary uses,
 * and text the parser rejects is an error.
 * 
 * @param {} value -
 * 
 * @returns {hexBinary}
 */
export function afwToHexBinary(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_hexBinary";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts `<dataType>` value to ia5String. A string is read with the
 * ia5String parser, the same parser the typed literal "..."ia5String uses,
 * and text the parser rejects is an error.
 * 
 * @param {} value -
 * 
 * @returns {ia5String}
 */
export function afwToIa5String(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_ia5String";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts `<dataType>` value to integer. A double is truncated toward zero
 * (1.9 is 1 and -1.9 is -1); use round(), floor(), or ceil() first for
 * another choice. A double that is NaN, infinite, or outside the integer
 * range is an error. A string is read as XML Schema integer text: digits with
 * an optional sign, leading zeros allowed ("007" is 7). Text that is not an
 * integer ("1.5", " 1", "1x") is an error.
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
 * Converts `<dataType>` value to ipAddress. A string is read with the
 * ipAddress parser, the same parser the typed literal "..."ipAddress uses,
 * and text the parser rejects is an error.
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
 * Return the `<dataType>` value in lower case, as ECMAScript s.toLowerCase()
 * does: Unicode full case mapping, the same in every locale ("İ" becomes
 * "i̇"). The result is a string. normalize_to_lower_case maps one code point
 * at a time.
 * 
 * @param {} value - The `<dataType>` value.
 * 
 * @returns {string} The value in lower case, as a string.
 */
export function afwToLowerCase(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_lower_case";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts `<dataType>` value to objectId. A string is read with the objectId
 * parser, and text the parser rejects is an error.
 * 
 * @param {} value -
 * 
 * @returns {objectId}
 */
export function afwToObjectId(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_objectId";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts `<dataType>` value to objectPath. A string is read with the
 * objectPath parser, and text the parser rejects is an error.
 * 
 * @param {} value -
 * 
 * @returns {objectPath}
 */
export function afwToObjectPath(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_objectPath";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts `<dataType>` value to password. A string is read with the password
 * parser, the same parser the typed literal "..."password uses, and text the
 * parser rejects is an error.
 * 
 * @param {} value -
 * 
 * @returns {password}
 */
export function afwToPassword(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_password";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts `<dataType>` value to a compiled regexp, the same value as a
 * regexp literal. A pattern ICU does not accept is a conversion_error.
 * 
 * @param {} value -
 * 
 * @returns {regexp}
 */
export function afwToRegexp(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_regexp";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts `<dataType>` value to rfc822Name. A string is read with the
 * rfc822Name parser, the same parser the typed literal "..."rfc822Name uses,
 * and text the parser rejects is an error.
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
 * Converts `<dataType>` value to time. A string is read with the time parser,
 * the same parser the typed literal "..."time uses, and text the parser
 * rejects is an error.
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
 * Return the `<dataType>` value in upper case, as ECMAScript s.toUpperCase()
 * does: Unicode full case mapping, the same in every locale, so one code
 * point can become more than one ("ß" becomes "SS"). The result is a string.
 * 
 * @param {} value - The `<dataType>` value.
 * 
 * @returns {string} The value in upper case, as a string.
 */
export function afwToUpperCase(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_upper_case";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts `<dataType>` value to x500Name. A string is read with the x500Name
 * parser, the same parser the typed literal "..."x500Name uses, and text the
 * parser rejects is an error.
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
 * Converts `<dataType>` value to yearMonthDuration. A string is read with the
 * yearMonthDuration parser, the same parser the typed literal
 * "..."yearMonthDuration uses, and text the parser rejects is an error.
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
 * Remove white space and line terminators as ECMAScript counts them (tab,
 * vertical tab, form feed, space, no-break space, U+FEFF, every Unicode space
 * separator, LF, CR, U+2028 and U+2029) from the beginning and end of the
 * `<dataType>` value, as ECMAScript s.trim() does. White space inside the
 * value is kept; normalize_space also collapses it. The result is a string.
 * 
 * @param {} value - The `<dataType>` value.
 * 
 * @returns {string} The value without leading and trailing white space, as a
 *     string.
 */
export function afwTrim(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "trim";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Remove white space and line terminators as ECMAScript counts them (tab,
 * vertical tab, form feed, space, no-break space, U+FEFF, every Unicode space
 * separator, LF, CR, U+2028 and U+2029) from the end of the `<dataType>`
 * value, as ECMAScript s.trimEnd() does. The result is a string.
 * 
 * @param {} value - The `<dataType>` value.
 * 
 * @returns {string} The value without trailing white space, as a string.
 */
export function afwTrimEnd(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "trim_end";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Remove white space and line terminators as ECMAScript counts them (tab,
 * vertical tab, form feed, space, no-break space, U+FEFF, every Unicode space
 * separator, LF, CR, U+2028 and U+2029) from the beginning of the
 * `<dataType>` value, as ECMAScript s.trimStart() does. The result is a
 * string.
 * 
 * @param {} value - The `<dataType>` value.
 * 
 * @returns {string} The value without leading white space, as a string.
 */
export function afwTrimStart(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "trim_start";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Return the integer part of the `<dataType>` value, removing any fraction,
 * as ECMAScript Math.trunc does (toward zero: trunc(-1.5) is -1.0). The
 * result is a double: write to_integer(trunc(v)) for an integer. A result
 * that would be NaN, or a NaN argument, is an error. An integer is not
 * accepted: write to_double(v).
 * 
 * @param {} value -
 * 
 * @returns {}
 */
export function afwTrunc(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "trunc";
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
 * URL encode a `<dataType>` value and return the string result, as ECMAScript
 * encodeURI does: every character is percent-encoded as UTF-8 except A-Z a-z
 * 0-9 ; , / ? : @ & = + $ - _ . ! ~ * ' ( ) #. Characters that give a URI its
 * structure are kept, so this encodes a whole URI, not a single query value
 * or path segment.
 * 
 * @param {} unencoded - The `<dataType>` value to URL encode.
 * 
 * @returns {string} URI encoded string.
 */
export function afwUrlEncode(client : any, unencoded : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "url_encode";
    _action["unencoded"] = unencoded;

    return client.perform(_action);
}

