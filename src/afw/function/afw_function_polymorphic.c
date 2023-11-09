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

#include "afw_internal.h"
#include <libxml/xmlregexp.h>


static afw_boolean_t
impl_is_in_array(
    const afw_data_type_t *data_type,
    const void *internal,
    const afw_array_t *array,
    afw_xctx_t *xctx)
{
    const afw_iterator_t *iterator;
    const void *entry_internal;
    const afw_data_type_t *entry_data_type;

    if (!data_type) {
        AFW_THROW_ERROR_Z(arg_error,
            "impl_is_in_array() called with NULL data_type",
            xctx);
    }

    for (iterator = NULL;;) {
        afw_array_get_next_internal(array, &iterator,
            &entry_data_type, &entry_internal, xctx);
        if (!entry_internal) {
            return false;
        }
        if (data_type != entry_data_type) {
            continue;
        }
        if (afw_data_type_compare_internal(data_type,
            internal, entry_internal, xctx) == 0)
        {
            return true;
        }
    }
}



static afw_boolean_t
impl_is_subset_array(
    const afw_array_t *array1,
    const afw_array_t *array2,
    afw_xctx_t *xctx)
{
    const afw_iterator_t *iterator;
    const void *internal;
    const afw_data_type_t *data_type;

    for (iterator = NULL;;) {
        afw_array_get_next_internal(array1, &iterator,
            &data_type, &internal, xctx);
        if (!internal) {
            return true;
        }
        if (!impl_is_in_array(data_type, internal, array2, xctx)) {
            return false;
        }
    }
}


/* Add non-duplicates. */
static void
impl_add_nondups_to_array(
    const afw_data_type_t *data_type,
    const afw_array_t *from,
    const afw_array_t *to,
    afw_xctx_t *xctx)
{
    const afw_iterator_t *iterator;
    const void *internal;

    for (iterator = NULL;;) {
        afw_array_get_next_internal(from, &iterator,
            NULL, &internal, xctx);
        if (!internal) {
            break;
        }
        if (!impl_is_in_array(data_type, internal, to, xctx)) {
            afw_array_add_internal(to, data_type, internal, xctx);
        }
    }
}



/*
 * Common polymorphic function for at_least_one_member_of
 *
 * afw_function_execute_at_least_one_member_of
 *
 * See afw_function_bindings.h for more information.
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
 *   array1 - (array ``<Type>``) The first array.
 *
 *   array2 - (array ``<Type>``) The second array.
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_at_least_one_member_of(
    afw_function_execute_t *x)
{
    const afw_value_array_t *array1;
    const afw_value_array_t *array2;
    const afw_iterator_t *iterator;
    const afw_data_type_t *data_type;
    const void *internal;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array1, 1, array);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array2, 2, array);

    for (iterator = NULL;;) {
        afw_array_get_next_internal(array1->internal, &iterator,
            &data_type, &internal, x->xctx);
        if (!internal) {
            return afw_value_false;
        }
        if (impl_is_in_array(data_type, internal, array2->internal, x->xctx)) {
            return afw_value_true;
        }
    }
}



/*
 * Common polymorphic function for bag
 *
 * afw_function_execute_bag
 *
 * See afw_function_bindings.h for more information.
 *
 * Takes any number of `<dataType>` values and returns an array of array.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, array, base64Binary, boolean, date, dateTime, dayTimeDuration,
 *   dnsName, double, function, hexBinary, ia5String, integer, ipAddress, null,
 *   object, objectId, objectPath, password, rfc822Name, script, string,
 *   template, time, x500Name, xpathExpression, yearMonthDuration.
 *
 * Declaration:
 *
 * ```
 *   function bag <dataType>(
 *       ...values: (array of array)
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   values - (0 or more array ``<Type>``)
 *
 * Returns:
 *
 *   (array ``<Type>``)
 */
const afw_value_t *
afw_function_execute_bag(
    afw_function_execute_t *x)
{
    const afw_array_t *array;
    const afw_value_t *value;
    afw_size_t i;

    if (x->argc == 0) {
        return x->data_type->empty_array_value;
    }

    array = afw_array_of_create(x->data_type, x->p, x->xctx);

    for (i = 1; i <= x->argc; i++) {
        value = afw_function_evaluate_required_parameter(x, i, x->data_type);
        afw_array_add_internal(array, x->data_type,
            AFW_VALUE_INTERNAL(value), x->xctx);
    }

    return afw_value_create_array_unmanaged(array, x->p, x->xctx);
}



/*
 * Common polymorphic function for bag_size
 *
 * afw_function_execute_bag_size
 *
 * See afw_function_bindings.h for more information.
 *
 * This returns the integer number of values in array.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, array, base64Binary, boolean, date, dateTime, dayTimeDuration,
 *   dnsName, double, function, hexBinary, ia5String, integer, ipAddress, null,
 *   object, objectId, objectPath, password, rfc822Name, script, string,
 *   template, time, x500Name, xpathExpression, yearMonthDuration.
 *
 * Declaration:
 *
 * ```
 *   function bag_size <dataType>(
 *       value: array
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   value - (array ``<Type>``)
 *
 * Returns:
 *
 *   (integer)
 */
const afw_value_t *
afw_function_execute_bag_size(
    afw_function_execute_t *x)
{
    const afw_value_array_t *arg;
    afw_size_t count;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, 1, array);

    count = afw_array_get_count(arg->internal, x->xctx);

    return afw_value_create_integer_unmanaged((afw_integer_t)count, x->p, x->xctx);
}



/*
 * Common polymorphic function for clone
 *
 * afw_function_execute_clone
 *
 * See afw_function_bindings.h for more information.
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
    const afw_value_t *value;
    const afw_value_t *result;

    AFW_FUNCTION_EVALUATE_PARAMETER(value, 1);

    result = afw_value_clone(value, x->p, x->xctx);

    return result;
}



/*
 * Common polymorphic function for compile
 *
 * afw_function_execute_compile
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile <Type> value and return either an unevaluated adaptive value or a
 * string containing the compiler listing.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported <Type>:
 *
 * function, script, template, xpathExpression.
 *
 * Parameters:
 *     source - (<Type>) <Type> string to compile.
 *     listing - (any dataType) If specified, a compiler listing is produced
 *         instead of an unevaluated expression value.
 *         
 *         This parameter can be an integer between 0 and 10 of a string that
 *         is used for indentation.  If 0 is specified, no whitespace is added
 *         to the resulting string.  If 1 through 10 is specified, that number
 *         of spaces is used.
 *
 * Returns:
 *     (unevaluated)
 */
const afw_value_t *
afw_function_execute_compile(
    afw_function_execute_t *x)
{
    const afw_value_t *source;
    const afw_value_t *result;
    const afw_utf8_t *listing;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(source, 1);

    /** @fixme Need a way to get source location */
    result = afw_value_compile(
        source, afw_s_a_empty_string, x->p, x->xctx);
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        listing = afw_function_evaluate_whitespace_parameter(x, 2);
        result = afw_value_create_string_unmanaged(
            afw_value_compiler_listing_to_string(result, listing,
                x->p, x->xctx),
            x->p, x->xctx);
    }

    return result;
}



/*
 * Common polymorphic function for ends_with
 *
 * afw_function_execute_ends_with
 *
 * See afw_function_bindings.h for more information.
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
    const afw_value_t *arg1;
    const afw_value_t *arg2;
    const afw_value_string_t *a1;
    const afw_value_string_t *a2;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg1, 1);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg2, 2);
    AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(arg1);
    AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(arg2);

    a1 = (const afw_value_string_t *)arg1;
    a2 = (const afw_value_string_t *)arg2;

    return
        (
            a1->internal.len >= a2->internal.len &&
            memcmp(a1->internal.s + (a1->internal.len - a2->internal.len),
                a2->internal.s, a2->internal.len) == 0
        )
        ? afw_value_true
        : afw_value_false;
}



/*
 * Common polymorphic function for eq
 *
 * afw_function_execute_eq
 *
 * See afw_function_bindings.h for more information.
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
    const afw_value_t *arg1;
    const afw_value_t *arg2;
    const afw_data_type_t *arg1_data_type;
    const afw_data_type_t *arg2_data_type;
    int cmp;

    AFW_FUNCTION_EVALUATE_PARAMETER(arg1, 1);
    AFW_FUNCTION_EVALUATE_PARAMETER(arg2, 2);

    /* Handle undefined/null special case. */
    if (afw_value_is_undefined(arg1) || afw_value_is_null(arg1)) {
        return (afw_value_is_undefined(arg2) || afw_value_is_null(arg2))
            ? afw_value_true
            : afw_value_false;
    }
    if (afw_value_is_undefined(arg2) || afw_value_is_null(arg2)) {
        return afw_value_false;
    }

    arg1_data_type = afw_value_get_data_type(arg1, x->xctx);
    arg2_data_type = afw_value_get_data_type(arg2, x->xctx);

    if (arg1_data_type != arg2_data_type) {
        AFW_THROW_ERROR_Z(arg_error, "Data types do not match", x->xctx);
    }

    /* If either arg is NaN, return false. */
    if (afw_data_type_is_double(arg1_data_type) &&
        (
            afw_number_is_NaN(((afw_value_double_t *)arg1)->internal) ||
            afw_number_is_NaN(((afw_value_double_t *)arg2)->internal)
        ))
    {
        return afw_value_false;
    }

    cmp = afw_data_type_compare_internal(arg1_data_type,
        AFW_VALUE_INTERNAL(arg1), AFW_VALUE_INTERNAL(arg2), x->xctx);

    return (cmp == 0) ? afw_value_true : afw_value_false;
}



/*
 * Common polymorphic function for eqx
 *
 * afw_function_execute_eqx
 *
 * See afw_function_bindings.h for more information.
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
    const afw_value_t *arg1;
    const afw_value_t *arg2;
    const afw_data_type_t *arg1_data_type;
    const afw_data_type_t *arg2_data_type;
    int cmp;

    AFW_FUNCTION_EVALUATE_PARAMETER(arg1, 1);
    AFW_FUNCTION_EVALUATE_PARAMETER(arg2, 2);

    /* Handle undefined/null special case. */
    if (afw_value_is_undefined(arg1)) {
        return (afw_value_is_undefined(arg2))
            ? afw_value_true
            : afw_value_false;
    }
    if (afw_value_is_undefined(arg2)) {
        return afw_value_false;
    }

    /* If either arg is NaN, return false. */
    if (
        (afw_value_is_double(arg1) &&
        afw_number_is_NaN(((afw_value_double_t *)arg1)->internal))
        ||
        (afw_value_is_double(arg2) &&
        afw_number_is_NaN(((afw_value_double_t *)arg2)->internal))
        )
    {
        return afw_value_false;
    }

    arg1_data_type = afw_value_get_data_type(arg1, x->xctx);
    arg2_data_type = afw_value_get_data_type(arg2, x->xctx);

    if (arg1_data_type != arg2_data_type) {
        cmp = 1;
    }

    else {
        cmp = afw_data_type_compare_internal(arg1_data_type,
            AFW_VALUE_INTERNAL(arg1), AFW_VALUE_INTERNAL(arg2), x->xctx);
    }

    return (cmp == 0) ? afw_value_true : afw_value_false;
}



/*
 * Common polymorphic function for ge
 *
 * afw_function_execute_ge
 *
 * See afw_function_bindings.h for more information.
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
 *   dnsName, double, function, hexBinary, ia5String, integer, ipAddress,
 *   object, objectId, objectPath, password, regexp, rfc822Name, script, string,
 *   template, time, x500Name, xpathExpression, yearMonthDuration.
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
    const afw_value_t *arg1;
    const afw_value_t *arg2;
    const afw_data_type_t *arg1_data_type;
    const afw_data_type_t *arg2_data_type;
    int cmp;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg1, 1);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg2, 2);

    arg1_data_type = afw_value_get_data_type(arg1, x->xctx);
    arg2_data_type = afw_value_get_data_type(arg2, x->xctx);

    if (arg1_data_type != arg2_data_type) {
        AFW_THROW_ERROR_Z(arg_error, "Data types do not match", x->xctx);
    }

    /* If either arg is NaN, return false. */
    if (afw_data_type_is_double(arg1_data_type) &&
        (
            afw_number_is_NaN(((afw_value_double_t *)arg1)->internal) ||
            afw_number_is_NaN(((afw_value_double_t *)arg2)->internal)
        ))
    {
        return afw_value_false;
    }

    cmp = afw_data_type_compare_internal(
        arg1_data_type,
        AFW_VALUE_INTERNAL(arg1), AFW_VALUE_INTERNAL(arg2), x->xctx);

    return (cmp >= 0) ? afw_value_true : afw_value_false;
}



/*
 * Common polymorphic function for gt
 *
 * afw_function_execute_gt
 *
 * See afw_function_bindings.h for more information.
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
 *   dnsName, double, function, hexBinary, ia5String, integer, ipAddress,
 *   object, objectId, objectPath, password, regexp, rfc822Name, script, string,
 *   template, time, x500Name, xpathExpression, yearMonthDuration.
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
    const afw_value_t *arg1;
    const afw_value_t *arg2;
    const afw_data_type_t *arg1_data_type;
    const afw_data_type_t *arg2_data_type;
    int cmp;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg1, 1);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg2, 2);

    arg1_data_type = afw_value_get_data_type(arg1, x->xctx);
    arg2_data_type = afw_value_get_data_type(arg2, x->xctx);

    if (arg1_data_type != arg2_data_type) {
        AFW_THROW_ERROR_Z(arg_error, "Data types do not match", x->xctx);
    }

    /* If either arg is NaN, return false. */
    if (afw_data_type_is_double(arg1_data_type) &&
        (
            afw_number_is_NaN(((afw_value_double_t *)arg1)->internal) ||
            afw_number_is_NaN(((afw_value_double_t *)arg2)->internal)
        ))
    {
        return afw_value_false;
    }

    cmp = afw_data_type_compare_internal(arg1_data_type,
        AFW_VALUE_INTERNAL(arg1), AFW_VALUE_INTERNAL(arg2), x->xctx);

    return (cmp > 0) ? afw_value_true : afw_value_false;
}



/*
 * Common polymorphic function for includes
 *
 * afw_function_execute_includes
 *
 * See afw_function_bindings.h for more information.
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
    const afw_value_t *arg1;
    const afw_value_t *arg2;
    const afw_value_string_t *a1;
    const afw_value_string_t *a2;
    const afw_value_integer_t *position;
    const afw_utf8_octet_t *c;
    afw_integer_t start;
    afw_size_t len;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg1, 1);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg2, 2);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(position, 3, integer);
    
    AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(arg1);
    AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(arg2);

    a1 = (const afw_value_string_t *)arg1;
    a2 = (const afw_value_string_t *)arg2;
    
    c = a1->internal.s;
    len = a1->internal.len;
    
    if (position) {
        start = abs((int)position->internal) % a1->internal.len;

        if (position->internal < 0 && start != 0) {
            start = a1->internal.len - start;
        }

        c += start;
        len -= start;
    }

    for ( ; a2->internal.len <= len; c++, len--) {
        if (memcmp(c, a2->internal.s, a2->internal.len) == 0) {
            return afw_value_true;
        }
    }

    return afw_value_false;
}



/*
 * Common polymorphic function for index_of
 *
 * afw_function_execute_index_of
 *
 * See afw_function_bindings.h for more information.
 *
 * Returns the zero-based index into `<dataType>` value of subString. If
 * subString is not found, -1 is returned.
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
    const afw_value_t *arg1;
    const afw_value_string_t *value;
    const afw_value_string_t *subString;
    const afw_value_integer_t *startIndex;
    afw_size_t index;
    afw_integer_t result;
    afw_size_t offset;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg1, 1);
    AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(arg1);
    value = (const afw_value_string_t *)arg1;
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(subString, 2, string);
    index = 0;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(startIndex,
            3, integer);
        index = (afw_size_t)startIndex->internal;
    }

    result = -1;

    for (offset = 0; offset < index; ) {
        if (afw_utf8_next_code_point(value->internal.s, &offset,
            value->internal.len, x->xctx) < 0)
        {
            goto return_result;
        }
    }

    for (;;) {

        if (offset + subString->internal.len > value->internal.len) {
            break;
        }

        if (memcmp(value->internal.s + offset,
            subString->internal.s, subString->internal.len) == 0)
        {
            result = index;
            break;
        }

        index++;

        if (afw_utf8_next_code_point(value->internal.s, &offset,
            value->internal.len, x->xctx) < 0)
        {
            break;
        }
    }

return_result:
    return afw_value_create_integer_unmanaged(result, x->p, x->xctx);
}



/*
 * Common polymorphic function for intersection
 *
 * afw_function_execute_intersection
 *
 * See afw_function_bindings.h for more information.
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
 *   array1 - (array ``<Type>``) The first array.
 *
 *   array2 - (array ``<Type>``) The second array.
 *
 * Returns:
 *
 *   (array ``<Type>``)
 */
const afw_value_t *
afw_function_execute_intersection(
    afw_function_execute_t *x)
{
    const afw_value_array_t *array1;
    const afw_value_array_t *array2;
    const afw_iterator_t *iterator;
    const afw_data_type_t *data_type;
    const void *internal;
    const afw_array_t *array;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array1, 1, array);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array2, 2, array);

    data_type = afw_array_get_data_type(array1->internal, x->xctx);
    if (!data_type ||
        data_type != afw_array_get_data_type(array2->internal, x->xctx))
    {
        AFW_THROW_ERROR_Z(arg_error,
            "array1 and array2 must have a data type of the same type",
            x->xctx);
    }
    array = afw_array_of_create(data_type, x->p, x->xctx);

    for (iterator = NULL;;) {
        afw_array_get_next_internal(array1->internal, &iterator, NULL,
            &internal, x->xctx);
        if (!internal) {
            break;
        }
        if (impl_is_in_array(data_type, internal, array2->internal, x->xctx)) {
            if (!impl_is_in_array(data_type, internal, array, x->xctx)) {
                afw_array_add_internal(array, data_type, internal, x->xctx);
            }
        }
    }

    return afw_value_create_array_unmanaged(array, x->p, x->xctx);
}



/*
 * Common polymorphic function for is_in
 *
 * afw_function_execute_is_in
 *
 * See afw_function_bindings.h for more information.
 *
 * Checks whether `<dataType>` value is in array of `<dataType>` array and
 * returns the boolean result.
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
 *   array - (array ``<Type>``)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_is_in(
    afw_function_execute_t *x)
{
    const afw_value_t *value;
    const afw_value_array_t *array;
    const afw_data_type_t *data_type;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(value, 1);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 2, array);

    data_type = afw_array_get_data_type(array->internal, x->xctx);
    if (!data_type || afw_value_get_data_type(value, x->xctx) != data_type) {
        AFW_THROW_ERROR_Z(arg_error,
            "array must be array of value's data type",
            x->xctx);
    }

    return impl_is_in_array(data_type, AFW_VALUE_INTERNAL(value),
        array->internal, x->xctx)
        ? afw_value_true
        : afw_value_false;
}



/*
 * Common polymorphic function for le
 *
 * afw_function_execute_le
 *
 * See afw_function_bindings.h for more information.
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
 *   dnsName, double, function, hexBinary, ia5String, integer, ipAddress,
 *   object, objectId, objectPath, password, regexp, rfc822Name, script, string,
 *   template, time, x500Name, xpathExpression, yearMonthDuration.
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
    const afw_value_t *arg1;
    const afw_value_t *arg2;
    const afw_data_type_t *arg1_data_type;
    const afw_data_type_t *arg2_data_type;
    int cmp;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg1, 1);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg2, 2);

    arg1_data_type = afw_value_get_data_type(arg1, x->xctx);
    arg2_data_type = afw_value_get_data_type(arg2, x->xctx);

    if (arg1_data_type != arg2_data_type) {
        AFW_THROW_ERROR_Z(arg_error, "Data types do not match", x->xctx);
    }

    /* If either arg is NaN, return false. */
    if (afw_data_type_is_double(arg1_data_type) &&
        (
            afw_number_is_NaN(((afw_value_double_t *)arg1)->internal) ||
            afw_number_is_NaN(((afw_value_double_t *)arg2)->internal)
        ))
    {
        return afw_value_false;
    }

    cmp = afw_data_type_compare_internal(arg1_data_type,
        AFW_VALUE_INTERNAL(arg1), AFW_VALUE_INTERNAL(arg2), x->xctx);

    return (cmp <= 0) ? afw_value_true : afw_value_false;
}



/*
 * Common polymorphic function for length
 *
 * afw_function_execute_length
 *
 * See afw_function_bindings.h for more information.
 *
 * This is a polymorphic function where `<dataType>` can be any of the supported
 * data types. Return the integer number of entries in datatype array or
 * codepoints in others.
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
    const afw_value_t *single;
    const afw_utf8_t *s;
    afw_size_t offset;
    afw_integer_t length;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(single, 1);

    if (afw_value_is_array(single)) {
        offset = afw_array_get_count(
            ((const afw_value_array_t *)single)->internal, x->xctx);
        length = afw_safe_cast_size_to_integer(offset, x->xctx);
    }

    else {
        AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(single);
        s = &((const afw_value_string_t *)single)->internal;

        for (length = 0, offset = 0;
            afw_utf8_next_code_point(s->s, &offset, s->len, x->xctx) >= 0;
            length++)
            ;
    }

    return afw_value_create_integer_unmanaged(length, x->p, x->xctx);
}



/*
 * Common polymorphic function for lt
 *
 * afw_function_execute_lt
 *
 * See afw_function_bindings.h for more information.
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
 *   dnsName, double, function, hexBinary, ia5String, integer, ipAddress,
 *   object, objectId, objectPath, password, regexp, rfc822Name, script, string,
 *   template, time, x500Name, xpathExpression, yearMonthDuration.
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
    const afw_value_t *arg1;
    const afw_value_t *arg2;
    const afw_data_type_t *arg1_data_type;
    const afw_data_type_t *arg2_data_type;
    int cmp;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg1, 1);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg2, 2);

    arg1_data_type = afw_value_get_data_type(arg1, x->xctx);
    arg2_data_type = afw_value_get_data_type(arg2, x->xctx);

    if (arg1_data_type != arg2_data_type) {
        AFW_THROW_ERROR_Z(arg_error, "Data types do not match", x->xctx);
    }

    /* If either arg is NaN, return false. */
    if (afw_data_type_is_double(arg1_data_type) &&
        (
            afw_number_is_NaN(((afw_value_double_t *)arg1)->internal) ||
            afw_number_is_NaN(((afw_value_double_t *)arg2)->internal)
        ))
    {
        return afw_value_false;
    }

    cmp = afw_data_type_compare_internal(arg1_data_type,
        AFW_VALUE_INTERNAL(arg1), AFW_VALUE_INTERNAL(arg2), x->xctx);

    return (cmp < 0) ? afw_value_true : afw_value_false;
}



/*
 * Common polymorphic function for max
 *
 * afw_function_execute_max
 *
 * See afw_function_bindings.h for more information.
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
 *       ...values_rest: (array of dataType)
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
    const afw_value_t *arg_max;
    const afw_value_t *arg;
    const afw_data_type_t *data_type;
    afw_size_t i;
    int cmp;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg_max, 1);
    data_type = afw_value_get_data_type(arg_max, x->xctx);

    for (i = 2; i <= x->argc; i++) {
        AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg, i);
        arg = afw_value_convert(arg, data_type, true, x->p, x->xctx);
        cmp = afw_data_type_compare_internal(data_type,
            AFW_VALUE_INTERNAL(arg_max), AFW_VALUE_INTERNAL(arg), x->xctx);
        if (cmp < 0) {
            arg_max = arg;
        }
    }

    return arg_max;
}



/*
 * Common polymorphic function for min
 *
 * afw_function_execute_min
 *
 * See afw_function_bindings.h for more information.
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
 *       ...values_rest: (array of dataType)
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
    const afw_value_t *arg_min;
    const afw_value_t *arg;
    const afw_data_type_t *data_type;
    afw_size_t i;
    int cmp;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg_min, 1);
    data_type = afw_value_get_data_type(arg_min, x->xctx);

    for (i = 2; i <= x->argc; i++) {
        AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg, i);
        arg = afw_value_convert(arg, data_type, true, x->p, x->xctx);
        cmp = afw_data_type_compare_internal(data_type,
            AFW_VALUE_INTERNAL(arg_min), AFW_VALUE_INTERNAL(arg), x->xctx);
        if (cmp > 0) {
            arg_min = arg;
        }
    }

    return arg_min;
}



/*
 * Common polymorphic function for ne
 *
 * afw_function_execute_ne
 *
 * See afw_function_bindings.h for more information.
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
    const afw_value_t *arg1;
    const afw_value_t *arg2;
    const afw_data_type_t *arg1_data_type;
    const afw_data_type_t *arg2_data_type;
    int cmp;

    AFW_FUNCTION_EVALUATE_PARAMETER(arg1, 1);
    AFW_FUNCTION_EVALUATE_PARAMETER(arg2, 2);

    /* Handle undefined/null special case. */
    if (afw_value_is_undefined(arg1) || afw_value_is_null(arg1)) {
        return (afw_value_is_undefined(arg2) || afw_value_is_null(arg2))
            ? afw_value_false
            : afw_value_true;
    }
    if (afw_value_is_undefined(arg2) || afw_value_is_null(arg2)) {
        return afw_value_true;
    }

    arg1_data_type = afw_value_get_data_type(arg1, x->xctx);
    arg2_data_type = afw_value_get_data_type(arg2, x->xctx);

    if (arg1_data_type != arg2_data_type) {
        AFW_THROW_ERROR_Z(arg_error, "Data types do not match", x->xctx);
    }

    /* If either arg is NaN, return true. */
    if (afw_data_type_is_double(arg1_data_type) &&
        (
            afw_number_is_NaN(((afw_value_double_t *)arg1)->internal) ||
            afw_number_is_NaN(((afw_value_double_t *)arg2)->internal)
        ))
    {
        return afw_value_true;
    }

    cmp = afw_data_type_compare_internal(arg1_data_type,
        AFW_VALUE_INTERNAL(arg1), AFW_VALUE_INTERNAL(arg2), x->xctx);

    return (cmp == 0)
        ? afw_value_false
        : afw_value_true;
}



/*
 * Common polymorphic function for nex
 *
 * afw_function_execute_nex
 *
 * See afw_function_bindings.h for more information.
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
    const afw_value_t *arg1;
    const afw_value_t *arg2;
    const afw_data_type_t *arg1_data_type;
    const afw_data_type_t *arg2_data_type;
    int cmp;

    AFW_FUNCTION_EVALUATE_PARAMETER(arg1, 1);
    AFW_FUNCTION_EVALUATE_PARAMETER(arg2, 2);

    /* Handle undefined/null special case. */
    if (afw_value_is_undefined(arg1)) {
        return (afw_value_is_undefined(arg2))
            ? afw_value_false
            : afw_value_true;
    }
    if (afw_value_is_undefined(arg2)) {
        return afw_value_true;
    }

    /* If either arg is NaN, return false. */
    if (
        (afw_value_is_double(arg1) &&
        afw_number_is_NaN(((afw_value_double_t *)arg1)->internal))
        ||
        (afw_value_is_double(arg2) &&
        afw_number_is_NaN(((afw_value_double_t *)arg2)->internal))
        )
    {
        return afw_value_true;
    }

    arg1_data_type = afw_value_get_data_type(arg1, x->xctx);
    arg2_data_type = afw_value_get_data_type(arg2, x->xctx);

    if (arg1_data_type != arg2_data_type) {
        cmp = 1;
    }

    else {
        cmp = afw_data_type_compare_internal(arg1_data_type,
            AFW_VALUE_INTERNAL(arg1), AFW_VALUE_INTERNAL(arg2), x->xctx);
    }

    return (cmp == 0) ? afw_value_false : afw_value_true;
}



/*
 * Common polymorphic function for one_and_only
 *
 * afw_function_execute_one_and_only
 *
 * See afw_function_bindings.h for more information.
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
 *       array: (array array)
 *   ): dataType;
 * ```
 *
 * Parameters:
 *
 *   array - (array array)
 *
 * Returns:
 *
 *   (``<Type>``)
 *
 * Errors thrown:
 *
 *   arg_error - array does not contain exactly one value
 */
const afw_value_t *
afw_function_execute_one_and_only(
    afw_function_execute_t *x)
{
    const afw_value_array_t *array;
    const afw_data_type_t *data_type;
    const void *internal;
    const afw_iterator_t *iterator;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 1, array);
    if (afw_array_get_count(array->internal, x->xctx) != 1) {
        AFW_THROW_ERROR_Z(arg_error,
            "arg must have exactly one value", x->xctx);
    }

    iterator = NULL;
    afw_array_get_next_internal(array->internal,
        &iterator, &data_type, &internal, x->xctx);

    return afw_value_common_create(internal, data_type, x->p, x->xctx);
}



/*
 * Common polymorphic function for regexp_match
 *
 * afw_function_execute_regexp_match
 *
 * See afw_function_bindings.h for more information.
 *
 * Checks whether `<dataType>` value matches the regular expression regexp and
 * return the boolean result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, dnsName, ipAddress, rfc822Name, string, x500Name.
 *
 * Declaration:
 *
 * ```
 *   function regexp_match <dataType>(
 *       value: dataType,
 *       regexp: string
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   value - (``<Type>``)
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
    xmlRegexpPtr rx = NULL;
    const afw_value_t *arg1;
    const afw_value_string_t *arg2;
    const xmlChar * s_z;
    const xmlChar * regexp_z;
    const afw_utf8_t *err_message;
    xmlErrorPtr err;
    const afw_value_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg1, 1);
    AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(arg1);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg2, 2, string);

    s_z = BAD_CAST afw_utf8_to_utf8_z(
        (const afw_utf8_t *)AFW_VALUE_INTERNAL(arg1), x->p, x->xctx);
    regexp_z = BAD_CAST afw_utf8_to_utf8_z(&arg2->internal, x->p, x->xctx);
    rx = xmlRegexpCompile(regexp_z);
    if (rx == NULL) {
        err = xmlGetLastError();
        err_message = afw_utf8_create_copy(err->message, AFW_UTF8_Z_LEN,
            x->p, x->xctx);
        xmlResetError(err);
        xmlRegFreeRegexp(rx);
        AFW_THROW_ERROR_FZ(arg_error, x->xctx,
            AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(err_message));
    }

    /* xmlRegexpExec returns: 1 if it matches, 0 if not */
    result = xmlRegexpExec(rx, s_z) == 0 ? afw_value_false : afw_value_true;
    xmlRegFreeRegexp(rx);

    return result;
}



/*
 * Common polymorphic function for repeat
 *
 * afw_function_execute_repeat
 *
 * See afw_function_bindings.h for more information.
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
 *   ): dataType;
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
 *   (``<Type>``) The repeated `<dataType>` value.
 */
const afw_value_t *
afw_function_execute_repeat(
    afw_function_execute_t *x)
{
    const afw_value_t *single;
    const afw_value_string_t *value;
    const afw_value_integer_t *times;
    afw_value_string_t *result;
    afw_utf8_octet_t *s;
    afw_integer_t count;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(single, 1);
    AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(single);
    value = (const afw_value_string_t *)single;
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(times, 2, integer);

    if (times->internal <= 0) {
        return afw_value_empty_string;
    }

    /** @fixme How should this limit be handled? 1000 was arbitrary. */
    if (times->internal > 1000) {
        AFW_THROW_ERROR_Z(arg_error, "Parameter times exceeds 1000", x->xctx);
    }

    result = afw_value_allocate_string(x->p, x->xctx);
    result->internal.len = (afw_size_t)times->internal * value->internal.len;
    result->internal.s = s = afw_pool_malloc(x->p,
        result->internal.len, x->xctx);
    for (count = times->internal; count > 0; count--) {
        memcpy(s, value->internal.s, value->internal.len);
        s += value->internal.len;
    }

    return &result->pub;
}



/*
 * Common polymorphic function for replace
 *
 * afw_function_execute_replace
 *
 * See afw_function_bindings.h for more information.
 *
 * Replace string(s) in a `<dataType>` value.
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
 *       match: string,
 *       replacement: string,
 *       limit?: integer
 *   ): dataType;
 * ```
 *
 * Parameters:
 *
 *   value - (``<Type>``) The original `<dataType>` value.
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
 *   (``<Type>``) A `<dataType>` value with the matched string(s) replaced.
 */
const afw_value_t *
afw_function_execute_replace(
    afw_function_execute_t *x)
{
    const afw_value_t *value;
    const afw_value_string_t *match;
    const afw_value_string_t *replacement;
    const afw_value_integer_t *limit_value;
    afw_value_string_t *result;
    afw_size_t count;
    afw_size_t limit;
    afw_utf8_t remaining;
    afw_utf8_octet_t *s;
    afw_size_t len;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(value, 1);
    AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(value);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(match, 2, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(replacement, 3, string);

    limit = 1;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(4))
    {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(
            limit_value, 4, integer);
        limit = (limit_value->internal < 0 ||
            limit_value->internal > AFW_SIZE_T_MAX)
            ? AFW_SIZE_T_MAX
            : (afw_size_t)limit_value->internal;
    }

    /* If limit is 0, return value. */
    if (limit == 0) {
        return value;
    }

    /* Count number of replacement that will be made. */
    afw_memory_copy(&remaining, &(((afw_value_string_t *)value)->internal));
    if (remaining.len == 0) {
        return afw_value_empty_string;
    }
    for (count = 0; count < limit && remaining.len > match->internal.len; )
    {
        for (;
            remaining.len >= match->internal.len;
            remaining.s++, remaining.len--)
        {
            if (memcmp(remaining.s, match->internal.s, match->internal.len)
                == 0)
            {
                remaining.s += match->internal.len;
                remaining.len -= match->internal.len;
                count++;
                break;
            }
        }
    }

    /* If no replacements, just return value. */
    if (count == 0) {
        return value;
    }

    /* Make a result string large enough to hold replacement. */
    afw_memory_copy(&remaining, &(((afw_value_string_t *)value)->internal));
    len = remaining.len - (count * match->internal.len) +
        (count * replacement->internal.len);
    s = afw_pool_malloc(x->p, len, x->xctx);
    result = afw_value_allocate_string(x->p, x->xctx);
    result->internal.len = len;
    result->internal.s = s;

    /* Do replaces and return result. */
    for (; count > 0; count--) {
        for (;;) {
            if (memcmp(remaining.s, match->internal.s, match->internal.len) == 0)
            {
                memcpy(s, replacement->internal.s, replacement->internal.len);
                s += replacement->internal.len;
                remaining.s += match->internal.len;
                remaining.len -= match->internal.len;
                break;
            }
            else {
                *s++ = *remaining.s++;
                remaining.len--;
            }
        }            
    }
    if (remaining.len > 0) {
        memcpy(s, remaining.s, remaining.len);
    }
    return &result->pub;
}



/*
 * Common polymorphic function for set_equals
 *
 * afw_function_execute_set_equals
 *
 * See afw_function_bindings.h for more information.
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
 *   array1 - (array ``<Type>``)
 *
 *   array2 - (array ``<Type>``)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_set_equals(
    afw_function_execute_t *x)
{
    const afw_value_array_t *array1;
    const afw_value_array_t *array2;
    const afw_data_type_t *data_type_1;
    const afw_data_type_t *data_type_2;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array1, 1, array);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array2, 2, array);

    data_type_1 = afw_array_get_data_type(array1->internal, x->xctx);
    data_type_2 = afw_array_get_data_type(array2->internal, x->xctx);
    if (!data_type_1 || data_type_1 != data_type_2) {
        AFW_THROW_ERROR_Z(arg_error,
            "array1 and array2 must have a data type that matches", x->xctx);
    }

    return (impl_is_subset_array(array1->internal, array2->internal, x->xctx) &&
        impl_is_subset_array(array2->internal, array1->internal, x->xctx))
        ? afw_value_true
        : afw_value_false;
}



/*
 * Common polymorphic function for split
 *
 * afw_function_execute_split
 *
 * See afw_function_bindings.h for more information.
 *
 * Split `<dataType>` value into an array of strings using a separator.
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
 *       separator?: string,
 *       limit?: integer
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   value - (``<Type>``) The `<dataType>` value to split.
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
 *   (array) An array of strings.
 */
const afw_value_t *
afw_function_execute_split(
    afw_function_execute_t *x)
{
    const afw_value_t *value;
    const afw_value_string_t *separator_value;
    const afw_value_integer_t *limit_value;
    const afw_array_t *array;
    const afw_value_t *result;
    const afw_utf8_t *separator;
    afw_integer_t count;
    afw_integer_t limit;
    afw_utf8_t remaining;
    afw_utf8_t split;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(value, 1);
    AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(value);

    separator = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(separator_value,
            2, string);
        separator = &separator_value->internal;
        if (separator->len == 0) {
            separator = NULL;
        }
    }

    limit = AFW_INTEGER_MAX;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(limit_value,
            3, integer);
        limit = limit_value->internal;
    }

    array = afw_array_create_with_options(0, afw_data_type_string,
        x->p, x->xctx);
    result = afw_value_create_array_unmanaged(array, x->p, x->xctx);
    afw_memory_copy(&remaining, &(((afw_value_string_t *)value)->internal));

    if (separator) {
        for (count = 0; count < limit && remaining.len > 0; count++) {
            for (split.s = remaining.s, split.len = remaining.len;
                ;
                remaining.s++, remaining.len--)
            {
                if (remaining.len < separator->len) {
                    remaining.len = 0;
                    break;
                }
                if (memcmp(remaining.s, separator->s, separator->len) == 0) {
                    split.len = remaining.s - split.s;
                    remaining.s += separator->len;
                    remaining.len -= separator->len;
                    break;
                }
            }            
            afw_array_add_internal(array, afw_data_type_string,
                (const void *)&split, x->xctx);
        }
    }

    else {
        for (count = 0; count < limit && remaining.len > 0; count++) {
            split.s = remaining.s;
            split.len = 1;
            remaining.s++;
            remaining.len--;
            afw_array_add_internal(array, afw_data_type_string,
                (const void *)&split, x->xctx);
        }
    }

    return result;
}



/*
 * Common polymorphic function for starts_with
 *
 * afw_function_execute_starts_with
 *
 * See afw_function_bindings.h for more information.
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
    const afw_value_t *arg1;
    const afw_value_t *arg2;
    const afw_value_string_t *a1;
    const afw_value_string_t *a2;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg1, 1);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg2, 2);
    AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(arg1);
    AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(arg2);

    a1 = (const afw_value_string_t *)arg1;
    a2 = (const afw_value_string_t *)arg2;

    return
        (
            a1->internal.len >= a2->internal.len &&
            memcmp(a1->internal.s, a2->internal.s, a2->internal.len) == 0
        )
        ? afw_value_true
        : afw_value_false;
}



/*
 * Common polymorphic function for subset
 *
 * afw_function_execute_subset
 *
 * See afw_function_bindings.h for more information.
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
 *   array1 - (array ``<Type>``) The first array.
 *
 *   array2 - (array ``<Type>``) The second array.
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_subset(
    afw_function_execute_t *x)
{
    const afw_value_array_t *array1;
    const afw_value_array_t *array2;
    const afw_data_type_t *data_type_1;
    const afw_data_type_t *data_type_2;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array1, 1, array);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array2, 2, array);

    data_type_1 = afw_array_get_data_type(array1->internal, x->xctx);
    data_type_2 = afw_array_get_data_type(array2->internal, x->xctx);
    if (!data_type_1 || data_type_1 != data_type_2) {
        AFW_THROW_ERROR_Z(arg_error,
            "array1 and array2 must have a data type that matches", x->xctx);
    }

    return impl_is_subset_array(array1->internal, array2->internal, x->xctx)
        ? afw_value_true
        : afw_value_false;
}



/*
 * Common polymorphic function for substring
 *
 * afw_function_execute_substring
 *
 * See afw_function_bindings.h for more information.
 *
 * Returns the `<dataType>` substring of value beginning at zero-based position
 * integer startIndex and ending at the position before integer endIndex.
 * Specify -1 or omitting endIndex to return up to end of `<dataType>`.
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
 *   ): dataType;
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
 *   (``<Type>``)
 *
 * Errors thrown:
 *
 *   arg_error - startIndex or endIndex is out of range
 */
const afw_value_t *
afw_function_execute_substring(
    afw_function_execute_t *x)
{
    const afw_value_t *string;
    const afw_value_integer_t *startIndex;
    const afw_value_integer_t *endIndex;
    const afw_utf8_t *s;
    afw_size_t offset;
    afw_size_t pos;
    afw_size_t start;
    afw_size_t end;
    afw_utf8_t substring;

    /* string -> s */
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(string, 1);
    AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(string);
    s = AFW_VALUE_INTERNAL(string);

    /* startIndex -> start */
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(startIndex, 2,
        integer);
    if (startIndex->internal < 0) {
        AFW_THROW_ERROR_Z(arg_error, "startIndex is out of bounds", x->xctx);
    }
    start = afw_safe_cast_integer_to_size(startIndex->internal, x->xctx);

    /* endIndex -> end */
    end = -1;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(endIndex, 3,
            integer);
        if (endIndex->internal < -1 ||
            (endIndex->internal != -1 &&
                startIndex->internal >= endIndex->internal))
        {
            AFW_THROW_ERROR_Z(arg_error, "endIndex is out of bounds", x->xctx);
        }
        end = afw_safe_cast_integer_to_size(endIndex->internal, x->xctx);
    }

    /* Set substring.s */
    memset(&substring, 0, sizeof(afw_utf8_t));
    for (offset = 0, pos = 0; pos < start; pos++)
    {
        if (afw_utf8_next_code_point(s->s, &offset, s->len, x->xctx) < 0) {
            AFW_THROW_ERROR_Z(arg_error,
                "startIndex is out of bounds", x->xctx);
        }
    }
    substring.s = s->s + offset;

    /* Set substring.len */
    if (end == -1) {
        substring.len = s->s + s->len - substring.s;
    }
    else {
        for (; pos < end; pos++)
        {
            if (afw_utf8_next_code_point(s->s, &offset, s->len, x->xctx) < 0) {
                AFW_THROW_ERROR_Z(arg_error,
                    "startIndex and/or endIndex is out of bounds", x->xctx);
            }
        }
        substring.len = s->s + offset - substring.s;
    }

    /* Return substring. */
    return afw_value_create_string_unmanaged(&substring, x->p, x->xctx);
}



/*
 * Common polymorphic function for union
 *
 * afw_function_execute_union
 *
 * See afw_function_bindings.h for more information.
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
 *       ...arrays_rest: (array of array)
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   arrays - (2 or more array ``<Type>``) Two or more arrays.
 *
 * Returns:
 *
 *   (array ``<Type>``)
 */
const afw_value_t *
afw_function_execute_union(
    afw_function_execute_t *x)
{
    const afw_value_array_t *array1;
    const afw_value_array_t *arrayn;
    const afw_data_type_t *data_type;
    const afw_array_t *array;
    afw_size_t i;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array1, 1, array);

    data_type = afw_array_get_data_type(array1->internal, x->xctx);
    if (!data_type)
    {
        AFW_THROW_ERROR_Z(arg_error,
            "all arrays must have the same data type",
            x->xctx);
    }

    array = afw_array_of_create(data_type, x->p, x->xctx);
    impl_add_nondups_to_array(data_type, array1->internal, array, x->xctx);
    for (i = 2; i <= x->argc; i++) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arrayn, i, array);
        if (afw_array_get_data_type(arrayn->internal, x->xctx) != data_type) {
            AFW_THROW_ERROR_Z(arg_error,
                "all arrays must have the same data type",
                x->xctx);
        }
        impl_add_nondups_to_array(data_type, arrayn->internal, array, x->xctx);
    }

    return afw_value_create_array_unmanaged(array, x->p, x->xctx);
}



/*
 * Common polymorphic function for last_index_of
 *
 * afw_function_execute_last_index_of
 *
 * See afw_function_bindings.h for more information.
 *
 * Returns the zero-based index into `<dataType>` value of the last occurrence
 * of a subString. If subString is not found, -1 is returned.
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
    const afw_value_t *arg1;
    const afw_value_string_t *value;
    const afw_value_string_t *subString;
    const afw_value_integer_t *startIndex;
    afw_size_t index;
    afw_integer_t result;
    afw_size_t offset;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg1, 1);
    AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(arg1);
    value = (const afw_value_string_t *)arg1;
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(subString,
        2, string);
    index = 0;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(startIndex,
            3, integer);
        index = (afw_size_t)startIndex->internal;
    }

    result = -1;

    for (offset = 0; offset < index; ) {
        if (afw_utf8_next_code_point(value->internal.s, &offset,
            value->internal.len, x->xctx) < 0)
        {
            goto return_result;
        }
    }

    for (;;) {

        if (offset + subString->internal.len > value->internal.len) {
            break;
        }

        if (memcmp(value->internal.s + offset,
            subString->internal.s, subString->internal.len) == 0)
        {
            result = index;
        }

        index++;

        if (afw_utf8_next_code_point(value->internal.s, &offset,
            value->internal.len, x->xctx) < 0)
        {
            break;
        }
    }

return_result:
    return afw_value_create_integer_unmanaged(result, x->p, x->xctx);
}



/*
 * Common polymorphic function for regexp_index_of
 *
 * afw_function_execute_regexp_index_of
 *
 * See afw_function_bindings.h for more information.
 *
 * Search `<dataType>` value for a regular expression and return index. If not
 * found, -1 is returned.
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
 *   function regexp_index_of <dataType>(
 *       value: dataType,
 *       regexp: string
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   value - (``<Type>``) The `<dataType>` value to search.
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
    /** @fixme Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Common polymorphic function for regexp_replace
 *
 * afw_function_execute_regexp_replace
 *
 * See afw_function_bindings.h for more information.
 *
 * Replace matched values for a regular expression in a `<dataType>` value.
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
 *   function regexp_replace <dataType>(
 *       value: dataType,
 *       regexp: string,
 *       replacement: string,
 *       limit?: integer
 *   ): dataType;
 * ```
 *
 * Parameters:
 *
 *   value - (``<Type>``) The original `<dataType>` value.
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
 *   (``<Type>``) A `<dataType>` value with the matched string(s) replaced.
 */
const afw_value_t *
afw_function_execute_regexp_replace(
    afw_function_execute_t *x)
{
    /** @fixme Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
 *   unencoded - (``<Type>``) URL encode a single value. See the url_encode
 *       method for the data type of more details.
 *
 * Returns:
 *
 *   (string) URI encoded string.
 */
const afw_value_t *
afw_function_execute_url_encode(
    afw_function_execute_t *x)
{
    const afw_value_anyURI_t *unencoded;
    const afw_utf8_t *s;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(unencoded, 1, anyURI);

    s = afw_uri_encode(&unencoded->internal,
        AFW_URI_OCTET_ENCODE_URI, x->p, x->xctx);
    
    /* Return String value. */
    return afw_value_create_string_unmanaged(s, x->p, x->xctx);
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
    afw_value_base64Binary_t *result;
    const afw_value_string_t *value;

    /* Only string supported at the moment. */
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(value, 1, string);

    result = afw_value_allocate_base64Binary(x->p, x->xctx);
    result->internal.ptr = (const afw_octet_t *)value->internal.s;
    result->internal.size = value->internal.len;

    return &result->pub;
}



/*
 * Common polymorphic function for encode_as_hexBinary
 *
 * afw_function_execute_encode_as_hexBinary
 *
 * See afw_function_bindings.h for more information.
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
    afw_value_hexBinary_t *result;
    const afw_value_string_t *value;

    /* Only string supported at the moment. */
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(value, 1, string);

    result = afw_value_allocate_hexBinary(x->p, x->xctx);
    result->internal.ptr = (const afw_octet_t *)value->internal.s;
    result->internal.size = value->internal.len;

    return &result->pub;
}



/*
 * Common polymorphic function for is
 *
 * afw_function_execute_is
 *
 * See afw_function_bindings.h for more information.
 *
 * Checks whether value is dataType `<dataType>` and return the boolean result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Supported `<dataType>`:
 *
 *   anyURI, array, base64Binary, boolean, date, dateTime, dayTimeDuration,
 *   dnsName, double, function, hexBinary, ia5String, integer, ipAddress, null,
 *   object, objectId, objectPath, password, rfc822Name, script, string,
 *   template, time, x500Name, xpathExpression, yearMonthDuration, unevaluated.
 *
 * Declaration:
 *
 * ```
 *   function is <dataType>(
 *       value: any
 *   ): boolean;
 * ```
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
    const afw_value_t *arg;
    const afw_data_type_t *data_type;
    
    AFW_FUNCTION_EVALUATE_PARAMETER(arg, 1);

    if (afw_value_is_undefined(arg)) {
        return afw_value_false;
    }

    data_type = afw_value_get_data_type(arg, x->xctx);
    return (x->data_type == data_type)
        ? afw_value_true
        : afw_value_false;
}
