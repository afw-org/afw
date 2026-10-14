// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for regexp
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file regexp.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the regexp category.
 */

interface IAnyObject {
    [prop : string] : any;
}

/**
 * Construct a new array of regexp from the given values, the same as array()
 * except that the new array is typed: each value, and each entry of a spread
 * array, must be dataType regexp, and adding a value of another data type to
 * the array later throws. A value of another data type is an error, never
 * converted. This is usually written `array<regexp>(`...values).
 * 
 * @param {} values - A value of dataType regexp, or ... followed by an
 *     expression that results in an array, every entry of which is included.
 * 
 * @returns {array} The constructed array of regexp.
 */
export function afwArrayRegexp(client : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "array_regexp";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Returns value unchanged if it is dataType regexp. Otherwise, including when
 * value is undefined, throws an error. It never converts: to get a regexp
 * from a value of another data type, write a typed literal or call a to_T
 * conversion function where one exists. This is usually written value as
 * regexp.
 * 
 * @param {} value - Value to assert
 * 
 * @returns {regexp} value, which is dataType regexp
 */
export function afwAsRegexp(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "as_regexp";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Compile regexp value and return either an unevaluated adaptive value or a
 * string containing the compiler listing. The listing is a human-oriented
 * dump (value tree interleaved with source, plus ---Symbols tables) for
 * Fiddle and debugging — not pure JSON (use stringify) and not Adaptive
 * compiled-form text (use decompile).
 * 
 * @param {regexp} source - regexp string to compile
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
export function afwCompileRegexp(client : any, source : any, listing? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "compile<regexp>";
    _action["source"] = source;

    if (listing !== undefined)
        _action["listing"] = listing;

    return client.perform(_action);
}

/**
 * Determine if regexp arg1 is equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'eqx' ('===') instead
 * if you want false to be returned if arg1 and arg2's data type don't match.
 * 
 * @param {regexp} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqRegexp(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<regexp>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for regexp arg1 is equal to the value and data type of arg2
 * then return the boolean result. Use 'eq' ('==') instead if you want arg2 to
 * be converted to the data type of arg1 before comparison.
 * 
 * @param {regexp} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxRegexp(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<regexp>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for regexp arg1 is greater than or equal to regexp arg2 and return
 * the boolean result.
 * 
 * @param {regexp} arg1 -
 * 
 * @param {regexp} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeRegexp(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<regexp>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for regexp arg1 is greater than regexp arg2 and return the boolean
 * result.
 * 
 * @param {regexp} arg1 -
 * 
 * @param {regexp} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtRegexp(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<regexp>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType regexp and returns the boolean result. It
 * is false for undefined. This can also be written `is<regexp>(value`).
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsRegexp(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_regexp";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks for regexp arg1 is less than or equal to regexp arg2 and return the
 * boolean result.
 * 
 * @param {regexp} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeRegexp(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<regexp>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for regexp arg1 is less that regexp arg2 and return the boolean
 * result.
 * 
 * @param {regexp} arg1 -
 * 
 * @param {regexp} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtRegexp(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<regexp>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if regexp arg1 is not equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'nex' ('!==') instead
 * if you want true to be returned if arg1 and arg2's data type don't match.
 * 
 * @param {regexp} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeRegexp(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<regexp>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for regexp arg1 is not equal to the value or data type of arg2
 * then return the boolean result. Use 'ne' ('!=') instead if you want arg2 to
 * be converted to the data type of arg1 before comparison.
 * 
 * @param {regexp} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexRegexp(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<regexp>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Converts value to data type regexp returning regexp result. Holds a regular
 * expression source string as a regexp value (does not compile the pattern
 * for matching by itself). Use polymorphic compile when a compiled form is
 * required.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {regexp} Converted value
 */
export function afwRegexp(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "regexp";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Returns value with every character that could have a meaning in a regular
 * expression escaped, so that the result matches value itself, as
 * ECMAScript's RegExp.escape(s) does. Use it to put data in a pattern:
 * to_regexp("^" + regexp_escape(name) + "\\z"). ASCII letters, digits, _ and
 * characters above ASCII are kept, except a leading digit and white space,
 * which are written as \x{hh}, as are control characters; every other ASCII
 * character gets a backslash. The result means the same with any flags.
 * 
 * @param {string} value - The text to escape.
 * 
 * @returns {string} The escaped text.
 */
export function afwRegexpEscape(client : any, value : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "regexp_escape";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks whether the regular expression matches anywhere in value and returns
 * the boolean result, as ECMAScript's re.test(s) does: re->test(s). Anchor
 * with ^ and \z to match the whole value. value can be any value whose data
 * type holds text, such as string, anyURI, dnsName, ipAddress, rfc822Name and
 * x500Name.
 * 
 * @param {regexp} regexp - The regular expression. A regexp literal (/a+b/ or
 *     "a+b"re), to_regexp(s), or `compile<regexp>(r`) is already compiled;
 *     any other regexp value is compiled for this call.
 * 
 * @param {} value - The value to search. Its data type must hold text.
 * 
 * @returns {boolean} True if the regular expression matches anywhere in
 *     value.
 */
export function afwTest(client : any, regexp : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "test";
    _action["regexp"] = regexp;
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts regexp value to string. For array values, the to_string() value
 * for each entry is returned separated with commas.
 * 
 * @param {regexp} value - A regexp value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringRegexp(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<regexp>";
    _action["value"] = value;

    return client.perform(_action);
}

