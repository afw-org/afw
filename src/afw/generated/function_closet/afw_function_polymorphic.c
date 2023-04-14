// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_common polymorphic function_execute_* functions
 *
 * Copyright (c) 2010-2023 Clemson University
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
 * See afw_function_bindings.h for more information.
 *
 * Returns boolean true if at least one value in <dataType> list1 is in
 * <dataType> list2.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, base64Binary, boolean, date, dateTime, dayTimeDuration, double,
 *   hexBinary, integer, rfc822Name, string, time, x500Name, yearMonthDuration.
 *
 * Declaration:
 *
 *   function at_least_one_member_of <dataType>(
 *       list1: list,
 *       list2: list
 *   ): boolean;
 *
 * Parameters:
 *
 *   list1 - (list <Type>) The first list.
 *
 *   list2 - (list <Type>) The second list.
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for bag
 *
 * afw_function_execute_bag
 *
 * See afw_function_bindings.h for more information.
 *
 * Takes any number of <dataType> values and returns a list of list.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, base64Binary, boolean, date, dateTime, dayTimeDuration, dnsName,
 *   double, expression, function, hexBinary, hybrid, ia5String, integer,
 *   ipAddress, list, null, object, objectId, objectPath, password, rfc822Name,
 *   script, string, template, time, x500Name, xpathExpression,
 *   yearMonthDuration.
 *
 * Declaration:
 *
 *   function bag <dataType>(
 *       ...values: (list of list)
 *   ): list;
 *
 * Parameters:
 *
 *   values - (0 or more list <Type>)
 *
 * Returns:
 *
 *   (list <Type>)
 */
const afw_value_t *
afw_function_execute_bag(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for bag_size
 *
 * afw_function_execute_bag_size
 *
 * See afw_function_bindings.h for more information.
 *
 * This returns the integer number of values in list.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, base64Binary, boolean, date, dateTime, dayTimeDuration, dnsName,
 *   double, expression, function, hexBinary, hybrid, ia5String, integer,
 *   ipAddress, list, null, object, objectId, objectPath, password, rfc822Name,
 *   script, string, template, time, x500Name, xpathExpression,
 *   yearMonthDuration.
 *
 * Declaration:
 *
 *   function bag_size <dataType>(
 *       value: list
 *   ): integer;
 *
 * Parameters:
 *
 *   value - (list <Type>)
 *
 * Returns:
 *
 *   (integer)
 */
const afw_value_t *
afw_function_execute_bag_size(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for clone
 *
 * afw_function_execute_clone
 *
 * See afw_function_bindings.h for more information.
 *
 * Deep clone a <dataType> value.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   list, object.
 *
 * Declaration:
 *
 *   function clone <dataType>(
 *       value: dataType
 *   ): dataType;
 *
 * Parameters:
 *
 *   value - (<Type>) The <dataType> value to clone.
 *
 * Returns:
 *
 *   (<Type>) The cloned <dataType> value.
 */
const afw_value_t *
afw_function_execute_clone(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for encode_as_base64Binary
 *
 * afw_function_execute_encode_as_base64Binary
 *
 * See afw_function_bindings.h for more information.
 *
 * Encode a value as a base64Binary. The effect is to create a base64Binary
 * value with an internal value of the value passed.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   string.
 *
 * Declaration:
 *
 *   function encode_as_base64Binary <dataType>(
 *       value: dataType
 *   ): base64Binary;
 *
 * Parameters:
 *
 *   value - (<Type>) The <dataType> value to encode.
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for encode_as_hexBinary
 *
 * afw_function_execute_encode_as_hexBinary
 *
 * See afw_function_bindings.h for more information.
 *
 * Encode a value as a hexBinary. The effect is to create a hexBinary value
 * with an internal value of the value passed.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   string.
 *
 * Declaration:
 *
 *   function encode_as_hexBinary <dataType>(
 *       value: dataType
 *   ): hexBinary;
 *
 * Parameters:
 *
 *   value - (<Type>) The <dataType> value to encode.
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for ends_with
 *
 * afw_function_execute_ends_with
 *
 * See afw_function_bindings.h for more information.
 *
 * Checks whether <dataType> value ends with a <dataType> and return the
 * boolean result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, string.
 *
 * Declaration:
 *
 *   function ends_with <dataType>(
 *       value: dataType,
 *       subString: string
 *   ): boolean;
 *
 * Parameters:
 *
 *   value - (<Type>)
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for eq
 *
 * afw_function_execute_eq
 *
 * See afw_function_bindings.h for more information.
 *
 * Determine if <dataType> arg1 is equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use "eqx" ("===") instead
 * if you want false to be returned if arg1 and arg2's data type don't match.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, base64Binary, boolean, date, dateTime, dayTimeDuration, dnsName,
 *   double, expression, function, hexBinary, hybrid, ia5String, integer,
 *   ipAddress, list, object, objectId, objectPath, password, regexp,
 *   rfc822Name, script, string, template, time, x500Name, xpathExpression,
 *   yearMonthDuration.
 *
 * Declaration:
 *
 *   function eq <dataType>(
 *       arg1: dataType,
 *       arg2: any
 *   ): boolean;
 *
 * Parameters:
 *
 *   arg1 - (<Type>)
 *
 *   arg2 - (any dataType)
 *
 * Returns:
 *
 *   (boolean)
 *
 * Errors thrown:
 *
 *   conversion - arg2 cannot be converted to the data type of arg1.
 */
const afw_value_t *
afw_function_execute_eq(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for eqx
 *
 * afw_function_execute_eqx
 *
 * See afw_function_bindings.h for more information.
 *
 * Determine if for <dataType> arg1 is equal to the value and data type of arg2
 * then return the boolean result. Use "eq" ("==") instead if you want arg2 to
 * be converted to the data type of arg1 before comparison.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, base64Binary, boolean, date, dateTime, dayTimeDuration, dnsName,
 *   double, expression, function, hexBinary, hybrid, ia5String, integer,
 *   ipAddress, list, object, objectId, objectPath, password, regexp,
 *   rfc822Name, script, string, template, time, x500Name, xpathExpression,
 *   yearMonthDuration.
 *
 * Declaration:
 *
 *   function eqx <dataType>(
 *       arg1: dataType,
 *       arg2: any
 *   ): boolean;
 *
 * Parameters:
 *
 *   arg1 - (<Type>)
 *
 *   arg2 - (any dataType)
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for ge
 *
 * afw_function_execute_ge
 *
 * See afw_function_bindings.h for more information.
 *
 * Checks for <dataType> arg1 is greater than or equal to <dataType> arg2 and
 * return the boolean result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, base64Binary, boolean, date, dateTime, dayTimeDuration, dnsName,
 *   double, expression, function, hexBinary, hybrid, ia5String, integer,
 *   ipAddress, list, object, objectId, objectPath, password, regexp,
 *   rfc822Name, script, string, template, time, x500Name, xpathExpression,
 *   yearMonthDuration.
 *
 * Declaration:
 *
 *   function ge <dataType>(
 *       arg1: dataType,
 *       arg2: dataType
 *   ): boolean;
 *
 * Parameters:
 *
 *   arg1 - (<Type>)
 *
 *   arg2 - (<Type>)
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for gt
 *
 * afw_function_execute_gt
 *
 * See afw_function_bindings.h for more information.
 *
 * Checks for <dataType> arg1 is greater than <dataType> arg2 and return the
 * boolean result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, base64Binary, boolean, date, dateTime, dayTimeDuration, dnsName,
 *   double, expression, function, hexBinary, hybrid, ia5String, integer,
 *   ipAddress, list, object, objectId, objectPath, password, regexp,
 *   rfc822Name, script, string, template, time, x500Name, xpathExpression,
 *   yearMonthDuration.
 *
 * Declaration:
 *
 *   function gt <dataType>(
 *       arg1: dataType,
 *       arg2: dataType
 *   ): boolean;
 *
 * Parameters:
 *
 *   arg1 - (<Type>)
 *
 *   arg2 - (<Type>)
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for includes
 *
 * afw_function_execute_includes
 *
 * See afw_function_bindings.h for more information.
 *
 * Checks whether the <dataType> value includes a string and return the boolean
 * result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, list, string.
 *
 * Declaration:
 *
 *   function includes <dataType>(
 *       searchString: dataType,
 *       subString: string,
 *       position?: integer
 *   ): boolean;
 *
 * Parameters:
 *
 *   searchString - (<Type>) The <dataType> to search.
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for index_of
 *
 * afw_function_execute_index_of
 *
 * See afw_function_bindings.h for more information.
 *
 * Returns the zero-based index into <dataType> value of subString. If
 * subString is not found, -1 is returned.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, string.
 *
 * Declaration:
 *
 *   function index_of <dataType>(
 *       value: dataType,
 *       subString: string,
 *       startIndex?: integer
 *   ): integer;
 *
 * Parameters:
 *
 *   value - (<Type>) The <dataType> value to search.
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for intersection
 *
 * afw_function_execute_intersection
 *
 * See afw_function_bindings.h for more information.
 *
 * Returns a list of <dataType> with the values that are common to both list of
 * <dataType> list1 and list2.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, base64Binary, boolean, date, dateTime, dayTimeDuration, double,
 *   hexBinary, integer, rfc822Name, string, time, x500Name, yearMonthDuration.
 *
 * Declaration:
 *
 *   function intersection <dataType>(
 *       list1: list,
 *       list2: list
 *   ): list;
 *
 * Parameters:
 *
 *   list1 - (list <Type>) The first list.
 *
 *   list2 - (list <Type>) The second list.
 *
 * Returns:
 *
 *   (list <Type>)
 */
const afw_value_t *
afw_function_execute_intersection(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for is
 *
 * afw_function_execute_is
 *
 * See afw_function_bindings.h for more information.
 *
 * Checks whether value is dataType <dataType> and return the boolean result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, base64Binary, boolean, date, dateTime, dayTimeDuration, dnsName,
 *   double, expression, function, hexBinary, hybrid, ia5String, integer,
 *   ipAddress, list, null, object, objectId, objectPath, password, rfc822Name,
 *   script, string, template, time, x500Name, xpathExpression,
 *   yearMonthDuration, unevaluated.
 *
 * Declaration:
 *
 *   function is <dataType>(
 *       value: any
 *   ): boolean;
 *
 * Parameters:
 *
 *   value - (any dataType) Value to check.
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_is(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for is_in
 *
 * afw_function_execute_is_in
 *
 * See afw_function_bindings.h for more information.
 *
 * Checks whether <dataType> value is in list of <dataType> list and returns
 * the boolean result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, base64Binary, boolean, date, dateTime, dayTimeDuration, dnsName,
 *   double, hexBinary, integer, ipAddress, rfc822Name, string, time, x500Name,
 *   yearMonthDuration.
 *
 * Declaration:
 *
 *   function is_in <dataType>(
 *       value: dataType,
 *       list: list
 *   ): boolean;
 *
 * Parameters:
 *
 *   value - (<Type>)
 *
 *   list - (list <Type>)
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for last_index_of
 *
 * afw_function_execute_last_index_of
 *
 * See afw_function_bindings.h for more information.
 *
 * Returns the zero-based index into <dataType> value of the last occurrence of
 * a subString. If subString is not found, -1 is returned.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, string.
 *
 * Declaration:
 *
 *   function last_index_of <dataType>(
 *       value: dataType,
 *       subString: string,
 *       startIndex?: integer
 *   ): integer;
 *
 * Parameters:
 *
 *   value - (<Type>) The <dataType> value to search.
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for le
 *
 * afw_function_execute_le
 *
 * See afw_function_bindings.h for more information.
 *
 * Checks for <dataType> arg1 is less than or equal to <dataType> arg2 and
 * return the boolean result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, base64Binary, boolean, date, dateTime, dayTimeDuration, dnsName,
 *   double, expression, function, hexBinary, hybrid, ia5String, integer,
 *   ipAddress, list, object, objectId, objectPath, password, regexp,
 *   rfc822Name, script, string, template, time, x500Name, xpathExpression,
 *   yearMonthDuration.
 *
 * Declaration:
 *
 *   function le <dataType>(
 *       arg1: dataType,
 *       arg2: any
 *   ): boolean;
 *
 * Parameters:
 *
 *   arg1 - (<Type>)
 *
 *   arg2 - (any dataType)
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for length
 *
 * afw_function_execute_length
 *
 * See afw_function_bindings.h for more information.
 *
 * This is a polymorphic function where <dataType> can be any of the supported
 * data types. Return the integer number of entries in datatype list or
 * codepoints in others.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, list, string.
 *
 * Declaration:
 *
 *   function length <dataType>(
 *       value: dataType
 *   ): integer;
 *
 * Parameters:
 *
 *   value - (<Type>) Returns the number of entries in a list or code points in
 *       others.
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for lt
 *
 * afw_function_execute_lt
 *
 * See afw_function_bindings.h for more information.
 *
 * Checks for <dataType> arg1 is less that <dataType> arg2 and return the
 * boolean result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, base64Binary, boolean, date, dateTime, dayTimeDuration, dnsName,
 *   double, expression, function, hexBinary, hybrid, ia5String, integer,
 *   ipAddress, list, object, objectId, objectPath, password, regexp,
 *   rfc822Name, script, string, template, time, x500Name, xpathExpression,
 *   yearMonthDuration.
 *
 * Declaration:
 *
 *   function lt <dataType>(
 *       arg1: dataType,
 *       arg2: dataType
 *   ): boolean;
 *
 * Parameters:
 *
 *   arg1 - (<Type>)
 *
 *   arg2 - (<Type>)
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for max
 *
 * afw_function_execute_max
 *
 * See afw_function_bindings.h for more information.
 *
 * Return the <dataType> value that is greater than or equal to the others.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   date, dateTime, double, integer, string, time.
 *
 * Declaration:
 *
 *   function max <dataType>(
 *       values_1: dataType,
 *       ...values_rest: (list of dataType)
 *   ): dataType;
 *
 * Parameters:
 *
 *   values - (1 or more <Type>)
 *
 * Returns:
 *
 *   (<Type>)
 */
const afw_value_t *
afw_function_execute_max(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for min
 *
 * afw_function_execute_min
 *
 * See afw_function_bindings.h for more information.
 *
 * Return the <dataType> value that is less than or equal to the others.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   date, dateTime, double, integer, string, time.
 *
 * Declaration:
 *
 *   function min <dataType>(
 *       values_1: dataType,
 *       ...values_rest: (list of dataType)
 *   ): dataType;
 *
 * Parameters:
 *
 *   values - (1 or more <Type>)
 *
 * Returns:
 *
 *   (<Type>)
 */
const afw_value_t *
afw_function_execute_min(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for ne
 *
 * afw_function_execute_ne
 *
 * See afw_function_bindings.h for more information.
 *
 * Determine if <dataType> arg1 is not equal to the value of arg2 converted to
 * the data type of arg1 then return the boolean result. Use "nex" ("!==")
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, base64Binary, boolean, date, dateTime, dayTimeDuration, dnsName,
 *   double, expression, function, hexBinary, hybrid, ia5String, integer,
 *   ipAddress, list, object, objectId, objectPath, password, regexp,
 *   rfc822Name, script, string, template, time, x500Name, xpathExpression,
 *   yearMonthDuration.
 *
 * Declaration:
 *
 *   function ne <dataType>(
 *       arg1: dataType,
 *       arg2: any
 *   ): boolean;
 *
 * Parameters:
 *
 *   arg1 - (<Type>)
 *
 *   arg2 - (any dataType)
 *
 * Returns:
 *
 *   (boolean)
 *
 * Errors thrown:
 *
 *   conversion - arg2 cannot be converted to the data type of arg1.
 */
const afw_value_t *
afw_function_execute_ne(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for nex
 *
 * afw_function_execute_nex
 *
 * See afw_function_bindings.h for more information.
 *
 * Determine if for <dataType> arg1 is not equal to the value or data type of
 * arg2 then return the boolean result. Use "ne" ("!=") instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, base64Binary, boolean, date, dateTime, dayTimeDuration, dnsName,
 *   double, expression, function, hexBinary, hybrid, ia5String, integer,
 *   ipAddress, list, object, objectId, objectPath, password, rfc822Name,
 *   regexp, script, string, template, time, x500Name, xpathExpression,
 *   yearMonthDuration.
 *
 * Declaration:
 *
 *   function nex <dataType>(
 *       arg1: dataType,
 *       arg2: any
 *   ): boolean;
 *
 * Parameters:
 *
 *   arg1 - (<Type>)
 *
 *   arg2 - (any dataType)
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for one_and_only
 *
 * afw_function_execute_one_and_only
 *
 * See afw_function_bindings.h for more information.
 *
 * This converts a list of <dataType> values that contains one value to a
 * single <dataType> value.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, base64Binary, boolean, date, dateTime, dayTimeDuration, dnsName,
 *   double, hexBinary, integer, ipAddress, rfc822Name, string, time, x500Name,
 *   yearMonthDuration.
 *
 * Declaration:
 *
 *   function one_and_only <dataType>(
 *       list: (list list)
 *   ): dataType;
 *
 * Parameters:
 *
 *   list - (list list)
 *
 * Returns:
 *
 *   (<Type>)
 *
 * Errors thrown:
 *
 *   arg_error - list does not contain exactly one value
 */
const afw_value_t *
afw_function_execute_one_and_only(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for regexp_index_of
 *
 * afw_function_execute_regexp_index_of
 *
 * See afw_function_bindings.h for more information.
 *
 * Search <dataType> value for a regular expression and return index. If not
 * found, -1 is returned.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, string.
 *
 * Declaration:
 *
 *   function regexp_index_of <dataType>(
 *       value: dataType,
 *       regexp: string
 *   ): integer;
 *
 * Parameters:
 *
 *   value - (<Type>) The <dataType> value to search.
 *
 *   regexp - (string) A regular expression to use for search.
 *
 * Returns:
 *
 *   (integer) Zero-based index of subString or -1 if not found.
 */
const afw_value_t *
afw_function_execute_regexp_index_of(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for regexp_match
 *
 * afw_function_execute_regexp_match
 *
 * See afw_function_bindings.h for more information.
 *
 * Checks whether <dataType> value matches the regular expression regexp and
 * return the boolean result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, dnsName, ipAddress, rfc822Name, string, x500Name.
 *
 * Declaration:
 *
 *   function regexp_match <dataType>(
 *       value: dataType,
 *       regexp: string
 *   ): boolean;
 *
 * Parameters:
 *
 *   value - (<Type>)
 *
 *   regexp - (string)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_regexp_match(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for regexp_replace
 *
 * afw_function_execute_regexp_replace
 *
 * See afw_function_bindings.h for more information.
 *
 * Replace matched values for a regular expression in a <dataType> value.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, string.
 *
 * Declaration:
 *
 *   function regexp_replace <dataType>(
 *       value: dataType,
 *       regexp: string,
 *       replacement: string,
 *       limit?: integer
 *   ): dataType;
 *
 * Parameters:
 *
 *   value - (<Type>) The original <dataType> value.
 *
 *   regexp - (string) A regular expression to use for search.
 *
 *   replacement - (string) The replacement string.
 *
 *   limit - (optional integer) This is the maximum times to replace. The
 *       default is 1. Specify -1 to replace all occurrences.
 *
 * Returns:
 *
 *   (<Type>) A <dataType> value with the matched string(s) replaced.
 */
const afw_value_t *
afw_function_execute_regexp_replace(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for repeat
 *
 * afw_function_execute_repeat
 *
 * See afw_function_bindings.h for more information.
 *
 * Repeat a <dataType> value a specified number of times.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, string.
 *
 * Declaration:
 *
 *   function repeat <dataType>(
 *       value: dataType,
 *       times: integer
 *   ): dataType;
 *
 * Parameters:
 *
 *   value - (<Type>) The <dataType> value to repeat.
 *
 *   times - (integer) The number of times to repeat the value.
 *
 * Returns:
 *
 *   (<Type>) The repeated <dataType> value.
 */
const afw_value_t *
afw_function_execute_repeat(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for replace
 *
 * afw_function_execute_replace
 *
 * See afw_function_bindings.h for more information.
 *
 * Replace string(s) in a <dataType> value.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, string.
 *
 * Declaration:
 *
 *   function replace <dataType>(
 *       value: dataType,
 *       match: string,
 *       replacement: string,
 *       limit?: integer
 *   ): dataType;
 *
 * Parameters:
 *
 *   value - (<Type>) The original <dataType> value.
 *
 *   match - (string) The string to replace.
 *
 *   replacement - (string) The replacement string.
 *
 *   limit - (optional integer) This is the maximum times to replace. The
 *       default is 1. Specify -1 to replace all occurrences.
 *
 * Returns:
 *
 *   (<Type>) A <dataType> value with the matched string(s) replaced.
 */
const afw_value_t *
afw_function_execute_replace(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for set_equals
 *
 * afw_function_execute_set_equals
 *
 * See afw_function_bindings.h for more information.
 *
 * Returns boolean true if <dataType> list1 and <dataType> list2 are subsets of
 * each other and return the boolean result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, base64Binary, boolean, date, dateTime, dayTimeDuration, double,
 *   hexBinary, integer, rfc822Name, string, time, x500Name, yearMonthDuration.
 *
 * Declaration:
 *
 *   function set_equals <dataType>(
 *       list1: list,
 *       list2: list
 *   ): boolean;
 *
 * Parameters:
 *
 *   list1 - (list <Type>)
 *
 *   list2 - (list <Type>)
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for split
 *
 * afw_function_execute_split
 *
 * See afw_function_bindings.h for more information.
 *
 * Split <dataType> value into an list of strings using a separator.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, string.
 *
 * Declaration:
 *
 *   function split <dataType>(
 *       value: dataType,
 *       separator?: string,
 *       limit?: integer
 *   ): list;
 *
 * Parameters:
 *
 *   value - (<Type>) The <dataType> value to split.
 *
 *   separator - (optional string) The separator to use. If this is an empty
 *       string or separator is not specified, the value is split between
 *       characters.
 *
 *   limit - (optional integer) This is the maximum number of splits. Any
 *       remaining part of value is ignored.
 *
 * Returns:
 *
 *   (list) An list of strings.
 */
const afw_value_t *
afw_function_execute_split(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for starts_with
 *
 * afw_function_execute_starts_with
 *
 * See afw_function_bindings.h for more information.
 *
 * Checks whether <dataType> value starts with a subString and return the
 * boolean result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, string.
 *
 * Declaration:
 *
 *   function starts_with <dataType>(
 *       value: dataType,
 *       subString: string
 *   ): boolean;
 *
 * Parameters:
 *
 *   value - (<Type>)
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for subset
 *
 * afw_function_execute_subset
 *
 * See afw_function_bindings.h for more information.
 *
 * Returns boolean true if the unique values in <dataType> list1 are all in
 * <dataType> list2.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, base64Binary, boolean, date, dateTime, dayTimeDuration, double,
 *   hexBinary, integer, rfc822Name, string, time, x500Name, yearMonthDuration.
 *
 * Declaration:
 *
 *   function subset <dataType>(
 *       list1: list,
 *       list2: list
 *   ): boolean;
 *
 * Parameters:
 *
 *   list1 - (list <Type>) The first list.
 *
 *   list2 - (list <Type>) The second list.
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for substring
 *
 * afw_function_execute_substring
 *
 * See afw_function_bindings.h for more information.
 *
 * Returns the <dataType> substring of value beginning at zero-based position
 * integer startIndex and ending at the position before integer endIndex.
 * Specify -1 or omitting endIndex to return up to end of <dataType>.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, string.
 *
 * Declaration:
 *
 *   function substring <dataType>(
 *       string: dataType,
 *       startIndex: integer,
 *       endIndex?: integer
 *   ): dataType;
 *
 * Parameters:
 *
 *   string - (<Type>)
 *
 *   startIndex - (integer)
 *
 *   endIndex - (optional integer)
 *
 * Returns:
 *
 *   (<Type>)
 *
 * Errors thrown:
 *
 *   arg_error - startIndex or endIndex is out of range
 */
const afw_value_t *
afw_function_execute_substring(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for union
 *
 * afw_function_execute_union
 *
 * See afw_function_bindings.h for more information.
 *
 * Returns a list of <dataType> contains all of the unique values in two or
 * more list of <dataType> values.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   anyURI, base64Binary, boolean, date, dateTime, dayTimeDuration, double,
 *   hexBinary, integer, rfc822Name, string, time, x500Name, yearMonthDuration.
 *
 * Declaration:
 *
 *   function union <dataType>(
 *       lists_1: list,
 *       lists_2: list,
 *       ...lists_rest: (list of list)
 *   ): list;
 *
 * Parameters:
 *
 *   lists - (2 or more list <Type>) Two or more lists.
 *
 * Returns:
 *
 *   (list <Type>)
 */
const afw_value_t *
afw_function_execute_union(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Common polymorphic function for url_encode
 *
 * afw_function_execute_url_encode
 *
 * See afw_function_bindings.h for more information.
 *
 * URL encode a value or bag of values.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <dataType>:
 *
 *   string, anyURI.
 *
 * Declaration:
 *
 *   function url_encode <dataType>(
 *       unencoded: dataType
 *   ): string;
 *
 * Parameters:
 *
 *   unencoded - (<Type>) URL encode a single value. See the url_encode method
 *       for the data type of more details.
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
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}
