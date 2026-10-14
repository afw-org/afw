// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_common polymorphic function_execute_* functions
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_polymorphic.c
 * @brief afw_common polymorphic function_execute_* functions.
 */

#include "afw.h"



/*
 * Common polymorphic function for at_least_one_member_of
 *
 * afw_function_execute_at_least_one_member_of
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Returns boolean true if at least one value in `<dataType>` array1 is in
 * `<dataType>` array2.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, base64Binary, boolean, date, dateTime, dayTimeDuration, double,
 *   hexBinary, integer, rfc822Name, string, time, x500Name, yearMonthDuration.
 *
 * Declaration:
 *
 * ```
 *   function at_least_one_member_of <dataType>(
 *       array1: array,
 *       array2: array
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   array1 - (``<Type>`[]`) The first array.
 *
 *   array2 - (``<Type>`[]`) The second array.
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_at_least_one_member_of(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for clone
 *
 * afw_function_execute_clone
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Deep clone a `<dataType>` value.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   array, object.
 *
 * Declaration:
 *
 * ```
 *   function clone <dataType>(
 *       value: dataType
 *   ): dataType;
 * ```
 *
 * Parameters:
 *
 *   value - (``<Type>``) The `<dataType>` value to clone.
 *
 * Returns:
 *
 *   (``<Type>``) The cloned `<dataType>` value.
 */
const afw_value_t *
afw_function_execute_clone(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for code_point_at
 *
 * afw_function_execute_code_point_at
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the code point at a zero-based index of the `<dataType>` value as an
 * integer, as ECMAScript s.codePointAt(i) does. Indexes count code points, as
 * everywhere in Adaptive Script (not UTF-16 units). Negative indexes count from
 * the end, as at does. If the index is out of range, the result is undefined,
 * as for at.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, string.
 *
 * Declaration:
 *
 * ```
 *   function code_point_at <dataType>(
 *       value: dataType,
 *       index: integer
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   value - (``<Type>``) The `<dataType>` value.
 *
 *   index - (integer) Zero-based code point index, or negative from the end.
 *
 * Returns:
 *
 *   (integer) The code point, or undefined if index is out of range.
 */
const afw_value_t *
afw_function_execute_code_point_at(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for encode_as_base64Binary
 *
 * afw_function_execute_encode_as_base64Binary
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Encode a value as a base64Binary. The effect is to create a base64Binary
 * value with an internal value of the value passed.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   string.
 *
 * Declaration:
 *
 * ```
 *   function encode_as_base64Binary <dataType>(
 *       value: dataType
 *   ): base64Binary;
 * ```
 *
 * Parameters:
 *
 *   value - (``<Type>``) The `<dataType>` value to encode.
 *
 * Returns:
 *
 *   (base64Binary) A base64Binary value.
 */
const afw_value_t *
afw_function_execute_encode_as_base64Binary(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for encode_as_hexBinary
 *
 * afw_function_execute_encode_as_hexBinary
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Encode a value as a hexBinary. The effect is to create a hexBinary value with
 * an internal value of the value passed.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   string.
 *
 * Declaration:
 *
 * ```
 *   function encode_as_hexBinary <dataType>(
 *       value: dataType
 *   ): hexBinary;
 * ```
 *
 * Parameters:
 *
 *   value - (``<Type>``) The `<dataType>` value to encode.
 *
 * Returns:
 *
 *   (hexBinary) A hexBinary value.
 */
const afw_value_t *
afw_function_execute_encode_as_hexBinary(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for ends_with
 *
 * afw_function_execute_ends_with
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Checks whether `<dataType>` value ends with a `<dataType>` and return the
 * boolean result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, string.
 *
 * Declaration:
 *
 * ```
 *   function ends_with <dataType>(
 *       value: dataType,
 *       subString: string
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   value - (``<Type>``)
 *
 *   subString - (string)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_ends_with(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for eq
 *
 * afw_function_execute_eq
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Determine if `<dataType>` arg1 is equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'eqx' ('===') instead
 * if you want false to be returned if arg1 and arg2's data type don't match.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, array, base64Binary, boolean, date, dateTime, dayTimeDuration,
 *   dnsName, double, function, hexBinary, ia5String, integer, ipAddress,
 *   object, objectId, objectPath, password, regexp, rfc822Name, script, string,
 *   template, time, x500Name, xpathExpression, yearMonthDuration.
 *
 * Declaration:
 *
 * ```
 *   function eq <dataType>(
 *       arg1: dataType,
 *       arg2: any
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   arg1 - (``<Type>``)
 *
 *   arg2 - (any)
 *
 * Returns:
 *
 *   (boolean)
 *
 * Errors thrown:
 *
 *   conversion_error - arg2 cannot be converted to the data type of arg1.
 */
const afw_value_t *
afw_function_execute_eq(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for eqx
 *
 * afw_function_execute_eqx
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Determine if for `<dataType>` arg1 is equal to the value and data type of
 * arg2 then return the boolean result. Use 'eq' ('==') instead if you want arg2
 * to be converted to the data type of arg1 before comparison.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, array, base64Binary, boolean, date, dateTime, dayTimeDuration,
 *   dnsName, double, function, hexBinary, ia5String, integer, ipAddress,
 *   object, objectId, objectPath, password, regexp, rfc822Name, script, string,
 *   template, time, x500Name, xpathExpression, yearMonthDuration.
 *
 * Declaration:
 *
 * ```
 *   function eqx <dataType>(
 *       arg1: dataType,
 *       arg2: any
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   arg1 - (``<Type>``)
 *
 *   arg2 - (any)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_eqx(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for freeze
 *
 * afw_function_execute_freeze
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Set a `<dataType>` value immutable so further mutation throws. If already
 * immutable, has no effect. Returns the same value.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Supported `<dataType>`:
 *
 *   array, object.
 *
 * Declaration:
 *
 * ```
 *   function freeze <dataType>(
 *       value: dataType
 *   ): dataType;
 * ```
 *
 * Parameters:
 *
 *   value - (``<Type>``) The `<dataType>` value to freeze.
 *
 * Returns:
 *
 *   (``<Type>``) The same value, now immutable.
 */
const afw_value_t *
afw_function_execute_freeze(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for ge
 *
 * afw_function_execute_ge
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Checks for `<dataType>` arg1 is greater than or equal to `<dataType>` arg2
 * and return the boolean result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, array, base64Binary, boolean, date, dateTime, dayTimeDuration,
 *   double, function, hexBinary, ia5String, integer, object, objectId,
 *   objectPath, password, regexp, script, string, template, time,
 *   xpathExpression, yearMonthDuration.
 *
 * Declaration:
 *
 * ```
 *   function ge <dataType>(
 *       arg1: dataType,
 *       arg2: dataType
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   arg1 - (``<Type>``)
 *
 *   arg2 - (``<Type>``)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_ge(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for gt
 *
 * afw_function_execute_gt
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Checks for `<dataType>` arg1 is greater than `<dataType>` arg2 and return the
 * boolean result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, array, base64Binary, boolean, date, dateTime, dayTimeDuration,
 *   double, function, hexBinary, ia5String, integer, object, objectId,
 *   objectPath, password, regexp, script, string, template, time,
 *   xpathExpression, yearMonthDuration.
 *
 * Declaration:
 *
 * ```
 *   function gt <dataType>(
 *       arg1: dataType,
 *       arg2: dataType
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   arg1 - (``<Type>``)
 *
 *   arg2 - (``<Type>``)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_gt(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for includes
 *
 * afw_function_execute_includes
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Checks whether the `<dataType>` value includes a string and return the
 * boolean result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, array, string.
 *
 * Declaration:
 *
 * ```
 *   function includes <dataType>(
 *       searchString: dataType,
 *       subString: string,
 *       position?: integer
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   searchString - (``<Type>``) The `<dataType>` to search.
 *
 *   subString - (string) Substring to find.
 *
 *   position - (optional integer) Zero-based position in the search string to
 *       start search.
 *
 * Returns:
 *
 *   (boolean) Indicates if the substring is contained in the search string.
 */
const afw_value_t *
afw_function_execute_includes(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for index_of
 *
 * afw_function_execute_index_of
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Returns the zero-based index into `<dataType>` value of subString. If
 * subString is not found, -1 is returned.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, array, string.
 *
 * Declaration:
 *
 * ```
 *   function index_of <dataType>(
 *       value: dataType,
 *       subString: string,
 *       startIndex?: integer
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   value - (``<Type>``) The `<dataType>` value to search.
 *
 *   subString - (string) Substring to search for.
 *
 *   startIndex - (optional integer) Optional start index for search if
 *       different than start of string.
 *
 * Returns:
 *
 *   (integer) Zero-based index of subString or -1 if not found.
 */
const afw_value_t *
afw_function_execute_index_of(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for intersection
 *
 * afw_function_execute_intersection
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Returns an array of `<dataType>` with the values that are common to both
 * array of `<dataType>` array1 and array2.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, base64Binary, boolean, date, dateTime, dayTimeDuration, double,
 *   hexBinary, integer, rfc822Name, string, time, x500Name, yearMonthDuration.
 *
 * Declaration:
 *
 * ```
 *   function intersection <dataType>(
 *       array1: array,
 *       array2: array
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   array1 - (``<Type>`[]`) The first array.
 *
 *   array2 - (``<Type>`[]`) The second array.
 *
 * Returns:
 *
 *   (``<Type>`[]`)
 */
const afw_value_t *
afw_function_execute_intersection(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for is_in
 *
 * afw_function_execute_is_in
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Checks whether `<dataType>` value is in array of `<dataType>` array and
 * returns the boolean result. Empty array yields false.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, base64Binary, boolean, date, dateTime, dayTimeDuration, dnsName,
 *   double, hexBinary, integer, ipAddress, rfc822Name, string, time, x500Name,
 *   yearMonthDuration.
 *
 * Declaration:
 *
 * ```
 *   function is_in <dataType>(
 *       value: dataType,
 *       array: array
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   value - (``<Type>``)
 *
 *   array - (``<Type>`[]`)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_is_in(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for last_index_of
 *
 * afw_function_execute_last_index_of
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Returns the zero-based index into `<dataType>` value of the last occurrence
 * of a subString. If subString is not found, -1 is returned.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, array, string.
 *
 * Declaration:
 *
 * ```
 *   function last_index_of <dataType>(
 *       value: dataType,
 *       subString: string,
 *       startIndex?: integer
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   value - (``<Type>``) The `<dataType>` value to search.
 *
 *   subString - (string) Substring to search for.
 *
 *   startIndex - (optional integer) Optional start index for search if
 *       different than start of string.
 *
 * Returns:
 *
 *   (integer) Zero-based index of subString or -1 if not found.
 */
const afw_value_t *
afw_function_execute_last_index_of(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for le
 *
 * afw_function_execute_le
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Checks for `<dataType>` arg1 is less than or equal to `<dataType>` arg2 and
 * return the boolean result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, array, base64Binary, boolean, date, dateTime, dayTimeDuration,
 *   double, function, hexBinary, ia5String, integer, object, objectId,
 *   objectPath, password, regexp, script, string, template, time,
 *   xpathExpression, yearMonthDuration.
 *
 * Declaration:
 *
 * ```
 *   function le <dataType>(
 *       arg1: dataType,
 *       arg2: any
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   arg1 - (``<Type>``)
 *
 *   arg2 - (any)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_le(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for length
 *
 * afw_function_execute_length
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the integer number of entries in a `<dataType>` array or the number of
 * codepoints in a `<dataType>` anyURI or string.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, array, string.
 *
 * Declaration:
 *
 * ```
 *   function length <dataType>(
 *       value: dataType
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   value - (``<Type>``) Returns the number of entries in an array or code
 *       points in others.
 *
 * Returns:
 *
 *   (integer)
 */
const afw_value_t *
afw_function_execute_length(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for lt
 *
 * afw_function_execute_lt
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Checks for `<dataType>` arg1 is less that `<dataType>` arg2 and return the
 * boolean result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, array, base64Binary, boolean, date, dateTime, dayTimeDuration,
 *   double, function, hexBinary, ia5String, integer, object, objectId,
 *   objectPath, password, regexp, script, string, template, time,
 *   xpathExpression, yearMonthDuration.
 *
 * Declaration:
 *
 * ```
 *   function lt <dataType>(
 *       arg1: dataType,
 *       arg2: dataType
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   arg1 - (``<Type>``)
 *
 *   arg2 - (``<Type>``)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_lt(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for match
 *
 * afw_function_execute_match
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Searches `<dataType>` value for the regular expression and returns its first
 * match with the text of its groups, or undefined if there is none, as
 * ECMAScript's s.match(re) does without the g flag. Use test for a boolean
 * result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, string.
 *
 * Declaration:
 *
 * ```
 *   function match <dataType>(
 *       value: dataType,
 *       regexp: regexp
 *   ): object; // _AdaptiveRegexpMatch_
 * ```
 *
 * Parameters:
 *
 *   value - (``<Type>``) The `<dataType>` value to search.
 *
 *   regexp - (regexp) The regular expression. A regexp literal (/a+b/ or
 *       "a+b"re), to_regexp(s), or `compile<regexp>(r`) is already compiled;
 *       any other regexp value is compiled for this call.
 *
 * Returns:
 *
 *   (object _AdaptiveRegexpMatch_) The first match, or undefined if there is
 *       none.
 */
const afw_value_t *
afw_function_execute_match(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for match_all
 *
 * afw_function_execute_match_all
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Searches `<dataType>` value for the regular expression and returns each match
 * with the text of its groups, left to right, as ECMAScript's s.matchAll(re)
 * does. Matches do not overlap; after an empty match the next search starts one
 * code point later. length(match_all(s, re)) counts the matches.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, string.
 *
 * Declaration:
 *
 * ```
 *   function match_all <dataType>(
 *       value: dataType,
 *       regexp: regexp,
 *       limit?: integer
 *   ): object[]; // _AdaptiveRegexpMatch_
 * ```
 *
 * Parameters:
 *
 *   value - (``<Type>``) The `<dataType>` value to search.
 *
 *   regexp - (regexp) The regular expression. A regexp literal (/a+b/ or
 *       "a+b"re), to_regexp(s), or `compile<regexp>(r`) is already compiled;
 *       any other regexp value is compiled for this call.
 *
 *   limit - (optional integer) The maximum number of matches to return. The
 *       default, or any negative limit, returns every match.
 *
 * Returns:
 *
 *   (object[] _AdaptiveRegexpMatch_) The matches, empty if there are none.
 */
const afw_value_t *
afw_function_execute_match_all(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for max
 *
 * afw_function_execute_max
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the `<dataType>` value that is greater than or equal to the others.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   date, dateTime, double, integer, string, time.
 *
 * Declaration:
 *
 * ```
 *   function max <dataType>(
 *       values_1: dataType,
 *       ...values_rest: dataType[]
 *   ): dataType;
 * ```
 *
 * Parameters:
 *
 *   values - (1 or more ``<Type>``)
 *
 * Returns:
 *
 *   (``<Type>``)
 */
const afw_value_t *
afw_function_execute_max(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for min
 *
 * afw_function_execute_min
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the `<dataType>` value that is less than or equal to the others.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   date, dateTime, double, integer, string, time.
 *
 * Declaration:
 *
 * ```
 *   function min <dataType>(
 *       values_1: dataType,
 *       ...values_rest: dataType[]
 *   ): dataType;
 * ```
 *
 * Parameters:
 *
 *   values - (1 or more ``<Type>``)
 *
 * Returns:
 *
 *   (``<Type>``)
 */
const afw_value_t *
afw_function_execute_min(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for ne
 *
 * afw_function_execute_ne
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Determine if `<dataType>` arg1 is not equal to the value of arg2 converted to
 * the data type of arg1 then return the boolean result. Use 'nex' ('!==')
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, array, base64Binary, boolean, date, dateTime, dayTimeDuration,
 *   dnsName, double, function, hexBinary, ia5String, integer, ipAddress,
 *   object, objectId, objectPath, password, regexp, rfc822Name, script, string,
 *   template, time, x500Name, xpathExpression, yearMonthDuration.
 *
 * Declaration:
 *
 * ```
 *   function ne <dataType>(
 *       arg1: dataType,
 *       arg2: any
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   arg1 - (``<Type>``)
 *
 *   arg2 - (any)
 *
 * Returns:
 *
 *   (boolean)
 *
 * Errors thrown:
 *
 *   conversion_error - arg2 cannot be converted to the data type of arg1.
 */
const afw_value_t *
afw_function_execute_ne(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for nex
 *
 * afw_function_execute_nex
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Determine if for `<dataType>` arg1 is not equal to the value or data type of
 * arg2 then return the boolean result. Use 'ne' ('!=') instead if you want arg2
 * to be converted to the data type of arg1 before comparison.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, array, base64Binary, boolean, date, dateTime, dayTimeDuration,
 *   dnsName, double, function, hexBinary, ia5String, integer, ipAddress,
 *   object, objectId, objectPath, password, rfc822Name, regexp, script, string,
 *   template, time, x500Name, xpathExpression, yearMonthDuration.
 *
 * Declaration:
 *
 * ```
 *   function nex <dataType>(
 *       arg1: dataType,
 *       arg2: any
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   arg1 - (``<Type>``)
 *
 *   arg2 - (any)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_nex(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for normalize
 *
 * afw_function_execute_normalize
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the `<dataType>` value in a Unicode normalization form, as ECMAScript
 * s.normalize(form) does. Every Adaptive string is already NFC, so form "NFC"
 * (the default) returns it as a string; "NFKC" also replaces compatibility
 * characters ("ﬁ" becomes "fi", "①" becomes "1"). "NFD" and "NFKD" are errors:
 * a string in Adaptive Script is always NFC, so a decomposed result can not be
 * a string.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, string.
 *
 * Declaration:
 *
 * ```
 *   function normalize <dataType>(
 *       value: dataType,
 *       form?: string
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   value - (``<Type>``) The `<dataType>` value.
 *
 *   form - (optional string) "NFC" (the default) or "NFKC".
 *
 * Returns:
 *
 *   (string) The normalized value, as a string.
 *
 * Errors thrown:
 *
 *   argument_error - form is not NFC or NFKC
 */
const afw_value_t *
afw_function_execute_normalize(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for one_and_only
 *
 * afw_function_execute_one_and_only
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * This converts an array of `<dataType>` values that contains one value to a
 * single `<dataType>` value.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, base64Binary, boolean, date, dateTime, dayTimeDuration, dnsName,
 *   double, hexBinary, integer, ipAddress, rfc822Name, string, time, x500Name,
 *   yearMonthDuration.
 *
 * Declaration:
 *
 * ```
 *   function one_and_only <dataType>(
 *       array: array[]
 *   ): dataType;
 * ```
 *
 * Parameters:
 *
 *   array - (array[])
 *
 * Returns:
 *
 *   (``<Type>``)
 *
 * Errors thrown:
 *
 *   argument_error - array does not contain exactly one value
 */
const afw_value_t *
afw_function_execute_one_and_only(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for repeat
 *
 * afw_function_execute_repeat
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Repeat a `<dataType>` value a specified number of times.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, string.
 *
 * Declaration:
 *
 * ```
 *   function repeat <dataType>(
 *       value: dataType,
 *       times: integer
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   value - (``<Type>``) The `<dataType>` value to repeat.
 *
 *   times - (integer) The number of times to repeat the value.
 *
 * Returns:
 *
 *   (string) Repeated text as string (not re-typed as the input data type).
 */
const afw_value_t *
afw_function_execute_repeat(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for replace
 *
 * afw_function_execute_replace
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Replace matches in a `<dataType>` value, left to right, up to limit times, as
 * ECMAScript's s.replace(search, r) does. search is a string, which is literal
 * text, or a regexp, which is a pattern. The replacement is a string, or a
 * function called with each match's _AdaptiveRegexpMatch_ object that returns
 * the string to put in. An empty match is a match at a code-point boundary. The
 * default limit replaces once (an empty string inserts at the start). Specify
 * limit -1 to replace every match.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, string.
 *
 * Declaration:
 *
 * ```
 *   function replace <dataType>(
 *       value: dataType,
 *       search: any,
 *       replacement: any,
 *       limit?: integer
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   value - (``<Type>``) The original `<dataType>` value.
 *
 *   search - (any) What to replace: a string is literal text, and an empty
 *       string matches at each code-point boundary, including the start and the
 *       end; a regexp is a pattern. A regexp literal (/a+b/ or "a+b"re),
 *       to_regexp(s), or `compile<regexp>(r`) is already compiled; any other
 *       regexp value is compiled for this call.
 *
 *   replacement - (any) The replacement for each match: a string, or a
 *       function. A function is called with one argument, the match's
 *       _AdaptiveRegexpMatch_ object ({match, index, captures, groups}), and
 *       must return a string. A string replacement is literal text when search
 *       is a string. When search is a regexp, $n is the text of group n ($0 the
 *       whole match), ${name} the text of a named group, and a backslash makes
 *       the next character literal (\$ for a dollar sign); any other $ is an
 *       error.
 *
 *   limit - (optional integer) The maximum number of matches to replace. The
 *       default is 1. Specify -1 (or any negative) to replace every match.
 *
 * Returns:
 *
 *   (string) Result text as string (not re-typed as the input data type).
 */
const afw_value_t *
afw_function_execute_replace(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for replace_all
 *
 * afw_function_execute_replace_all
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Replace every match in a `<dataType>` value, left to right, as ECMAScript
 * s.replaceAll(search, r) does; the same as replace(value, search, replacement,
 * -1). search is a string, which is literal text, or a regexp, which is a
 * pattern (no g flag is needed). The replacement is a string, or a function
 * called with each match's _AdaptiveRegexpMatch_ object that returns the string
 * to put in.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, string.
 *
 * Declaration:
 *
 * ```
 *   function replace_all <dataType>(
 *       value: dataType,
 *       search: any,
 *       replacement: any
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   value - (``<Type>``) The `<dataType>` value.
 *
 *   search - (any) What to replace: a string is literal text, and an empty
 *       string matches at each code-point boundary, including the start and the
 *       end; a regexp is a pattern. A regexp literal (/a+b/ or "a+b"re),
 *       to_regexp(s), or `compile<regexp>(r`) is already compiled; any other
 *       regexp value is compiled for this call.
 *
 *   replacement - (any) The replacement for each match: a string, or a
 *       function. A function is called with one argument, the match's
 *       _AdaptiveRegexpMatch_ object ({match, index, captures, groups}), and
 *       must return a string. A string replacement is literal text when search
 *       is a string. When search is a regexp, $n is the text of group n ($0 the
 *       whole match), ${name} the text of a named group, and a backslash makes
 *       the next character literal (\$ for a dollar sign); any other $ is an
 *       error.
 *
 * Returns:
 *
 *   (string) The value with every match replaced, as a string.
 */
const afw_value_t *
afw_function_execute_replace_all(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for search
 *
 * afw_function_execute_search
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Searches `<dataType>` value for the regular expression and returns the code
 * point index of the first match, or -1 if there is none, as ECMAScript's
 * s.search(re) does. Use index_of to find literal text.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, string.
 *
 * Declaration:
 *
 * ```
 *   function search <dataType>(
 *       value: dataType,
 *       regexp: regexp
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   value - (``<Type>``) The `<dataType>` value to search.
 *
 *   regexp - (regexp) The regular expression. A regexp literal (/a+b/ or
 *       "a+b"re), to_regexp(s), or `compile<regexp>(r`) is already compiled;
 *       any other regexp value is compiled for this call.
 *
 * Returns:
 *
 *   (integer) Zero-based code point index of the first match, or -1.
 */
const afw_value_t *
afw_function_execute_search(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for set_equals
 *
 * afw_function_execute_set_equals
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Returns boolean true if `<dataType>` array1 and `<dataType>` array2 are
 * subsets of each other and return the boolean result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, base64Binary, boolean, date, dateTime, dayTimeDuration, double,
 *   hexBinary, integer, rfc822Name, string, time, x500Name, yearMonthDuration.
 *
 * Declaration:
 *
 * ```
 *   function set_equals <dataType>(
 *       array1: array,
 *       array2: array
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   array1 - (``<Type>`[]`)
 *
 *   array2 - (``<Type>`[]`)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_set_equals(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for split
 *
 * afw_function_execute_split
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Split `<dataType>` value into an array of strings, as ECMAScript's
 * s.split(separator) does. separator is a string, which is literal text, or a
 * regexp, which is a pattern. With a regexp, the text of each group of a match
 * is put in the array after the piece before it (undefined when the group did
 * not take part), and an empty match splits between code points but not at the
 * start or end of the value.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, string.
 *
 * Declaration:
 *
 * ```
 *   function split <dataType>(
 *       value: dataType,
 *       separator?: any,
 *       limit?: integer
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   value - (``<Type>``) The `<dataType>` value to split.
 *
 *   separator - (optional any) Where to split: a string is literal text, a
 *       regexp is a pattern. If this is an empty string or separator is not
 *       specified, the value is split between characters. A regexp literal
 *       (/a+b/ or "a+b"re), to_regexp(s), or `compile<regexp>(r`) is already
 *       compiled; any other regexp value is compiled for this call.
 *
 *   limit - (optional integer) The maximum number of entries in the result. Any
 *       remaining part of value is ignored. The default, or any negative limit,
 *       is no limit.
 *
 * Returns:
 *
 *   (array) An array of strings.
 */
const afw_value_t *
afw_function_execute_split(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for starts_with
 *
 * afw_function_execute_starts_with
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Checks whether `<dataType>` value starts with a subString and return the
 * boolean result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, string.
 *
 * Declaration:
 *
 * ```
 *   function starts_with <dataType>(
 *       value: dataType,
 *       subString: string
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   value - (``<Type>``)
 *
 *   subString - (string)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_starts_with(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for subset
 *
 * afw_function_execute_subset
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Returns boolean true if the unique values in `<dataType>` array1 are all in
 * `<dataType>` array2.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, base64Binary, boolean, date, dateTime, dayTimeDuration, double,
 *   hexBinary, integer, rfc822Name, string, time, x500Name, yearMonthDuration.
 *
 * Declaration:
 *
 * ```
 *   function subset <dataType>(
 *       array1: array,
 *       array2: array
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   array1 - (``<Type>`[]`) The first array.
 *
 *   array2 - (``<Type>`[]`) The second array.
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_subset(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for substring
 *
 * afw_function_execute_substring
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Returns the string substring of value from zero-based code point position
 * integer startIndex up to, but not including, endIndex, as ECMAScript
 * substring does. A negative index is 0 and an index past the end is the end of
 * value. Omit endIndex to return through the end of value. If endIndex is less
 * than startIndex, the two are swapped, and equal indexes return an empty
 * string. Use slice to count from the end. The result is always string (a slice
 * of anyURI is not an anyURI).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, string.
 *
 * Declaration:
 *
 * ```
 *   function substring <dataType>(
 *       string: dataType,
 *       startIndex: integer,
 *       endIndex?: integer
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   string - (``<Type>``)
 *
 *   startIndex - (integer)
 *
 *   endIndex - (optional integer)
 *
 * Returns:
 *
 *   (string) Substring as string (not re-typed as the input data type).
 */
const afw_value_t *
afw_function_execute_substring(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for to_regexp
 *
 * afw_function_execute_to_regexp
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Converts `<dataType>` value to a compiled regexp, the same value as a regexp
 * literal. A pattern ICU does not accept is a conversion_error.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   string.
 *
 * Declaration:
 *
 * ```
 *   function to_regexp <dataType>(
 *       value: dataType
 *   ): regexp;
 * ```
 *
 * Parameters:
 *
 *   value - (``<Type>``)
 *
 * Returns:
 *
 *   (regexp)
 *
 * Errors thrown:
 *
 *   conversion_error - the pattern does not compile
 */
const afw_value_t *
afw_function_execute_to_regexp(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for trim
 *
 * afw_function_execute_trim
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Remove white space and line terminators as ECMAScript counts them (tab,
 * vertical tab, form feed, space, no-break space, U+FEFF, every Unicode space
 * separator, LF, CR, U+2028 and U+2029) from the beginning and end of the
 * `<dataType>` value, as ECMAScript s.trim() does. White space inside the value
 * is kept; normalize_space also collapses it. The result is a string.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, string.
 *
 * Declaration:
 *
 * ```
 *   function trim <dataType>(
 *       value: dataType
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   value - (``<Type>``) The `<dataType>` value.
 *
 * Returns:
 *
 *   (string) The value without leading and trailing white space, as a string.
 */
const afw_value_t *
afw_function_execute_trim(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for union
 *
 * afw_function_execute_union
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Returns an array of `<dataType>` contains all of the unique values in two or
 * more array of `<dataType>` values.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, base64Binary, boolean, date, dateTime, dayTimeDuration, double,
 *   hexBinary, integer, rfc822Name, string, time, x500Name, yearMonthDuration.
 *
 * Declaration:
 *
 * ```
 *   function union <dataType>(
 *       arrays_1: array,
 *       arrays_2: array,
 *       ...arrays_rest: array
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   arrays - (2 or more ``<Type>`[]`) Two or more arrays.
 *
 * Returns:
 *
 *   (``<Type>`[]`)
 */
const afw_value_t *
afw_function_execute_union(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for url_encode
 *
 * afw_function_execute_url_encode
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * URL encode a `<dataType>` value and return the string result, as ECMAScript
 * encodeURI does: every character is percent-encoded as UTF-8 except A-Z a-z
 * 0-9 ; , / ? : @ & = + $ - _ . ! ~ * ' ( ) #. Characters that give a URI its
 * structure are kept, so this encodes a whole URI, not a single query value or
 * path segment.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   string, anyURI.
 *
 * Declaration:
 *
 * ```
 *   function url_encode <dataType>(
 *       unencoded: dataType
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   unencoded - (``<Type>``) The `<dataType>` value to URL encode.
 *
 * Returns:
 *
 *   (string) URI encoded string.
 */
const afw_value_t *
afw_function_execute_url_encode(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
