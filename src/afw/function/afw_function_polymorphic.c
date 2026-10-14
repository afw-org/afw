// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_common polymorphic function_execute_* functions
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_polymorphic.c
 * @brief Common polymorphic function_execute_* implementations (shared across data-type methods).
 */

#include "afw_internal.h"


/* IEEE 754 signaling comparison of a NaN, under strictDouble (#628). */
static void
impl_signaling_comparison_of_NaN(afw_function_execute_t *x)
{
    if (AFW_VALUE_STRICT_DOUBLE(afw_function_execute_contextual(x), x->xctx))
    {
        AFW_THROW_ERROR_FZ(argument_error, x->xctx,
            "%ku of NaN is an IEEE 754 invalid operation: <, <=, > and >="
            " are signaling comparisons (#compile strictDouble; is_NaN(v)"
            " tests for NaN)",
            &x->function->functionId->internal);
    }
}



/* Number of code points in s. */
static afw_integer_t
impl_code_point_count(const afw_utf8_t *s, afw_xctx_t *xctx)
{
    afw_size_t offset;
    afw_integer_t count;

    for (offset = 0, count = 0;
        afw_utf8_next_code_point(s->s, &offset, s->len, xctx) >= 0;
        count++);
    return count;
}


/*
 * s as a string value: value itself when it is a string and s is its
 * text, otherwise a new string (an anyURI's text is a string).
 */
static const afw_value_t *
impl_utf8_as_string(const afw_value_t *value, const afw_utf8_t *s,
    afw_function_execute_t *x)
{
    if (afw_value_is_string(value) &&
        s == &((const afw_value_string_t *)value)->internal)
    {
        return value;
    }
    return afw_value_create_unmanaged_string(s, x->p, x->xctx);
}


static afw_boolean_t
impl_is_in_array(
    const afw_value_t *value,
    const afw_array_t *array,
    afw_xctx_t *xctx)
{
    const afw_iterator_old_t *iterator;
    const afw_value_t *entry;

    for (iterator = NULL;;) {
        entry = afw_array_get_next_value(array, &iterator, xctx);
        if (!entry) {
            return false;
        }
        if (afw_value_equal(value, entry, xctx)) {
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
    const afw_iterator_old_t *iterator;
    const afw_value_t *value;

    for (iterator = NULL;;) {
        value = afw_array_get_next_value(array1, &iterator, xctx);
        if (!value) {
            return true;
        }
        if (!impl_is_in_array(value, array2, xctx)) {
            return false;
        }
    }
}


/*
 * The data type of a set function's array. An empty array with no data
 * type (the literal []) is an empty set of the function's data type
 * (subset<string>([], ["a"]) threw "must have a data type that matches").
 */
static const afw_data_type_t *
impl_set_array_data_type(
    const afw_array_t *array,
    afw_function_execute_t *x)
{
    const afw_data_type_t *data_type;

    data_type = afw_array_get_data_type(array, x->xctx);
    if (!data_type && afw_array_get_count(array, x->xctx) == 0) {
        data_type = x->data_type;
    }
    return data_type;
}


/* Add non-duplicates. */
static void
impl_add_nondups_to_array(
    const afw_data_type_t *data_type,
    const afw_array_t *from,
    const afw_array_t *to,
    afw_xctx_t *xctx)
{
    const afw_iterator_old_t *iterator;
    const afw_value_t *value;

    (void)data_type;

    for (iterator = NULL;;) {
        value = afw_array_get_next_value(from, &iterator, xctx);
        if (!value) {
            break;
        }
        if (!impl_is_in_array(value, to, xctx)) {
            afw_array_push_value(to, value, xctx);
        }
    }
}



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
    const afw_value_array_t *array1;
    const afw_value_array_t *array2;
    const afw_iterator_old_t *iterator;
    const afw_value_t *value;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array1, 1, array);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array2, 2, array);

    for (iterator = NULL;;) {
        value = afw_array_get_next_value(array1->internal, &iterator, x->xctx);
        if (!value) {
            return afw_boolean_v_false;
        }
        if (impl_is_in_array(value, array2->internal, x->xctx)) {
            return afw_boolean_v_true;
        }
    }
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
    const afw_value_t *value;
    const afw_value_t *result;

    AFW_FUNCTION_EVALUATE_PARAMETER(value, 1);
    result = afw_value_clone(value, x->p, x->xctx);
    return afw_pool_scope_release_value_at_cleanup(result, x->p, x->xctx);
}



/*
 * Common polymorphic function for compile
 *
 * afw_function_execute_compile
 *
 * See afw_function_bindings_internal.h for more information.
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
    const afw_value_t *compiled;
    const afw_utf8_t *listing;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(source, 1);

    /** @fixme Need a way to get source location */
    result = afw_value_compile(
        source, afw_s_a_empty_string, x->p, x->xctx);
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        listing = afw_function_evaluate_whitespace_parameter(x, 2);
        compiled = result;
        result = afw_value_create_unmanaged_string(
            afw_value_compiler_listing_to_string(compiled, listing,
                x->p, x->xctx),
            x->p, x->xctx);
        /* Listing is a copy in dest p. The unit lasts for dest p. */
        return result;
    }

    return result;
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
        ? afw_boolean_v_true
        : afw_boolean_v_false;
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
            ? afw_boolean_v_true
            : afw_boolean_v_false;
    }
    if (afw_value_is_undefined(arg2) || afw_value_is_null(arg2)) {
        return afw_boolean_v_false;
    }
    if (afw_value_is_void(arg1)) {
        return afw_value_is_void(arg2)
            ? afw_boolean_v_true
            : afw_boolean_v_false;
    }
    if (afw_value_is_void(arg2)) {
        return afw_boolean_v_false;
    }

    arg1_data_type = afw_value_get_data_type(arg1, x->xctx);
    arg2_data_type = afw_value_get_data_type(arg2, x->xctx);

    if (arg1_data_type != arg2_data_type) {
        AFW_THROW_ERROR_Z(argument_error, "Data types do not match", x->xctx);
    }

    /* If either arg is NaN, return false. */
    if (afw_data_type_is_double(arg1_data_type) &&
        (
            afw_number_is_NaN(((afw_value_double_t *)arg1)->internal) ||
            afw_number_is_NaN(((afw_value_double_t *)arg2)->internal)
        ))
    {
        return afw_boolean_v_false;
    }

    cmp = afw_data_type_compare_internal(arg1_data_type,
        AFW_VALUE_INTERNAL(arg1), AFW_VALUE_INTERNAL(arg2), x->xctx);

    return (cmp == 0) ? afw_boolean_v_true : afw_boolean_v_false;
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
            ? afw_boolean_v_true
            : afw_boolean_v_false;
    }
    if (afw_value_is_undefined(arg2)) {
        return afw_boolean_v_false;
    }
    if (afw_value_is_void(arg1)) {
        return afw_value_is_void(arg2)
            ? afw_boolean_v_true
            : afw_boolean_v_false;
    }
    if (afw_value_is_void(arg2)) {
        return afw_boolean_v_false;
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
        return afw_boolean_v_false;
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

    return (cmp == 0) ? afw_boolean_v_true : afw_boolean_v_false;
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
        AFW_THROW_ERROR_Z(argument_error, "Data types do not match", x->xctx);
    }

    /*
     * NaN is unordered: false, as in ECMAScript (IEEE 754's quiet
     * comparison). Under #compile strictDouble, <, <=, >, >= are IEEE 754's
     * signaling comparisons, an invalid operation for a NaN (#628).
     */
    if (afw_data_type_is_double(arg1_data_type) &&
        (
            afw_number_is_NaN(((afw_value_double_t *)arg1)->internal) ||
            afw_number_is_NaN(((afw_value_double_t *)arg2)->internal)
        ))
    {
        impl_signaling_comparison_of_NaN(x);
        return afw_boolean_v_false;
    }

    cmp = afw_data_type_compare_internal(
        arg1_data_type,
        AFW_VALUE_INTERNAL(arg1), AFW_VALUE_INTERNAL(arg2), x->xctx);

    return (cmp >= 0) ? afw_boolean_v_true : afw_boolean_v_false;
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
        AFW_THROW_ERROR_Z(argument_error, "Data types do not match", x->xctx);
    }

    /*
     * NaN is unordered: false, as in ECMAScript (IEEE 754's quiet
     * comparison). Under #compile strictDouble, <, <=, >, >= are IEEE 754's
     * signaling comparisons, an invalid operation for a NaN (#628).
     */
    if (afw_data_type_is_double(arg1_data_type) &&
        (
            afw_number_is_NaN(((afw_value_double_t *)arg1)->internal) ||
            afw_number_is_NaN(((afw_value_double_t *)arg2)->internal)
        ))
    {
        impl_signaling_comparison_of_NaN(x);
        return afw_boolean_v_false;
    }

    cmp = afw_data_type_compare_internal(arg1_data_type,
        AFW_VALUE_INTERNAL(arg1), AFW_VALUE_INTERNAL(arg2), x->xctx);

    return (cmp > 0) ? afw_boolean_v_true : afw_boolean_v_false;
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
    const afw_value_t *arg1;
    const afw_value_t *arg2;
    const afw_value_string_t *a1;
    const afw_value_string_t *a2;
    const afw_value_integer_t *position;
    afw_size_t index;
    afw_size_t offset;
    afw_size_t count;
    afw_integer_t start_cp;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg1, 1);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg2, 2);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(position, 3, integer);
    
    AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(arg1);
    AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(arg2);

    a1 = (const afw_value_string_t *)arg1;
    a2 = (const afw_value_string_t *)arg2;

    /*
     * Search at code-point boundaries only (#153). Position is a zero-based
     * code-point index (negative counts from end), not a byte offset.
     */
    index = 0;
    if (position) {
        start_cp = position->internal;
        if (start_cp < 0) {
            count = 0;
            for (offset = 0;
                afw_utf8_next_code_point(a1->internal.s, &offset,
                    a1->internal.len, x->xctx) >= 0; )
            {
                count++;
            }
            /* Unsigned: -#integerMin does not fit. */
            if ((afw_size_t)0 - (afw_size_t)start_cp > count) {
                return afw_boolean_v_false;
            }
            index = count - ((afw_size_t)0 - (afw_size_t)start_cp);
        }
        else {
            index = (afw_size_t)start_cp;
        }
    }

    for (offset = 0; offset < index; ) {
        if (afw_utf8_next_code_point(a1->internal.s, &offset,
            a1->internal.len, x->xctx) < 0)
        {
            return afw_boolean_v_false;
        }
    }

    for (;;) {
        if (offset + a2->internal.len > a1->internal.len) {
            break;
        }
        if (memcmp(a1->internal.s + offset,
            a2->internal.s, a2->internal.len) == 0)
        {
            return afw_boolean_v_true;
        }
        if (afw_utf8_next_code_point(a1->internal.s, &offset,
            a1->internal.len, x->xctx) < 0)
        {
            break;
        }
    }

    return afw_boolean_v_false;
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
    return afw_value_create_unmanaged_integer(result, x->p, x->xctx);
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
    const afw_value_array_t *array1;
    const afw_value_array_t *array2;
    const afw_iterator_old_t *iterator;
    const afw_data_type_t *data_type;
    const afw_value_t *value;
    const afw_value_array_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array1, 1, array);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array2, 2, array);

    data_type = impl_set_array_data_type(array1->internal, x);
    if (!data_type ||
        data_type != impl_set_array_data_type(array2->internal, x))
    {
        AFW_THROW_ERROR_Z(argument_error,
            "array1 and array2 must have a data type of the same type",
            x->xctx);
    }
    result = (const afw_value_array_t *)
        afw_pool_scope_release_value_at_cleanup(
            afw_array_create_managed(data_type, x->p, x->xctx)->value,
            x->p, x->xctx);

    for (iterator = NULL;;) {
        value = afw_array_get_next_value(array1->internal, &iterator, x->xctx);
        if (!value) {
            break;
        }
        if (impl_is_in_array(value, array2->internal, x->xctx)) {
            if (!impl_is_in_array(value, result->internal, x->xctx)) {
                afw_array_push_value(result->internal, value, x->xctx);
            }
        }
    }

    return &result->pub;
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
    const afw_value_t *value;
    const afw_value_array_t *array;
    const afw_data_type_t *data_type;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(value, 1);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 2, array);

    if (afw_array_get_count(array->internal, x->xctx) == 0) {
        return afw_boolean_v_false;
    }

    data_type = afw_array_get_data_type(array->internal, x->xctx);
    if (!data_type || afw_value_get_data_type(value, x->xctx) != data_type) {
        AFW_THROW_ERROR_Z(argument_error,
            "array must be array of value's data type",
            x->xctx);
    }

    return impl_is_in_array(value, array->internal, x->xctx)
        ? afw_boolean_v_true
        : afw_boolean_v_false;
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
        AFW_THROW_ERROR_Z(argument_error, "Data types do not match", x->xctx);
    }

    /*
     * NaN is unordered: false, as in ECMAScript (IEEE 754's quiet
     * comparison). Under #compile strictDouble, <, <=, >, >= are IEEE 754's
     * signaling comparisons, an invalid operation for a NaN (#628).
     */
    if (afw_data_type_is_double(arg1_data_type) &&
        (
            afw_number_is_NaN(((afw_value_double_t *)arg1)->internal) ||
            afw_number_is_NaN(((afw_value_double_t *)arg2)->internal)
        ))
    {
        impl_signaling_comparison_of_NaN(x);
        return afw_boolean_v_false;
    }

    cmp = afw_data_type_compare_internal(arg1_data_type,
        AFW_VALUE_INTERNAL(arg1), AFW_VALUE_INTERNAL(arg2), x->xctx);

    return (cmp <= 0) ? afw_boolean_v_true : afw_boolean_v_false;
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

    return afw_value_create_unmanaged_integer(length, x->p, x->xctx);
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
        AFW_THROW_ERROR_Z(argument_error, "Data types do not match", x->xctx);
    }

    /*
     * NaN is unordered: false, as in ECMAScript (IEEE 754's quiet
     * comparison). Under #compile strictDouble, <, <=, >, >= are IEEE 754's
     * signaling comparisons, an invalid operation for a NaN (#628).
     */
    if (afw_data_type_is_double(arg1_data_type) &&
        (
            afw_number_is_NaN(((afw_value_double_t *)arg1)->internal) ||
            afw_number_is_NaN(((afw_value_double_t *)arg2)->internal)
        ))
    {
        impl_signaling_comparison_of_NaN(x);
        return afw_boolean_v_false;
    }

    cmp = afw_data_type_compare_internal(arg1_data_type,
        AFW_VALUE_INTERNAL(arg1), AFW_VALUE_INTERNAL(arg2), x->xctx);

    return (cmp < 0) ? afw_boolean_v_true : afw_boolean_v_false;
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
    const afw_value_t *arg_max;
    const afw_value_t *arg;
    const afw_data_type_t *data_type;
    afw_size_t i;
    int cmp;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg_max, 1);
    data_type = afw_value_get_data_type(arg_max, x->xctx);
    if (afw_data_type_is_double(data_type) &&
        afw_number_is_NaN(((const afw_value_double_t *)arg_max)->internal))
    {
        return arg_max;
    }

    for (i = 2; i <= x->argc; i++) {
        /* The parameter check already requires data_type (#583). */
        AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg, i);
        /* Any NaN makes the result NaN, as ECMAScript Math.max does. */
        if (afw_data_type_is_double(data_type) &&
            afw_number_is_NaN(((const afw_value_double_t *)arg)->internal))
        {
            return arg;
        }
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
    const afw_value_t *arg_min;
    const afw_value_t *arg;
    const afw_data_type_t *data_type;
    afw_size_t i;
    int cmp;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg_min, 1);
    data_type = afw_value_get_data_type(arg_min, x->xctx);
    if (afw_data_type_is_double(data_type) &&
        afw_number_is_NaN(((const afw_value_double_t *)arg_min)->internal))
    {
        return arg_min;
    }

    for (i = 2; i <= x->argc; i++) {
        /* The parameter check already requires data_type (#583). */
        AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg, i);
        /* Any NaN makes the result NaN, as ECMAScript Math.min does. */
        if (afw_data_type_is_double(data_type) &&
            afw_number_is_NaN(((const afw_value_double_t *)arg)->internal))
        {
            return arg;
        }
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
            ? afw_boolean_v_false
            : afw_boolean_v_true;
    }
    if (afw_value_is_undefined(arg2) || afw_value_is_null(arg2)) {
        return afw_boolean_v_true;
    }
    if (afw_value_is_void(arg1)) {
        return afw_value_is_void(arg2)
            ? afw_boolean_v_false
            : afw_boolean_v_true;
    }
    if (afw_value_is_void(arg2)) {
        return afw_boolean_v_true;
    }

    arg1_data_type = afw_value_get_data_type(arg1, x->xctx);
    arg2_data_type = afw_value_get_data_type(arg2, x->xctx);

    if (arg1_data_type != arg2_data_type) {
        AFW_THROW_ERROR_Z(argument_error, "Data types do not match", x->xctx);
    }

    /* If either arg is NaN, return true. */
    if (afw_data_type_is_double(arg1_data_type) &&
        (
            afw_number_is_NaN(((afw_value_double_t *)arg1)->internal) ||
            afw_number_is_NaN(((afw_value_double_t *)arg2)->internal)
        ))
    {
        return afw_boolean_v_true;
    }

    cmp = afw_data_type_compare_internal(arg1_data_type,
        AFW_VALUE_INTERNAL(arg1), AFW_VALUE_INTERNAL(arg2), x->xctx);

    return (cmp == 0)
        ? afw_boolean_v_false
        : afw_boolean_v_true;
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
            ? afw_boolean_v_false
            : afw_boolean_v_true;
    }
    if (afw_value_is_undefined(arg2)) {
        return afw_boolean_v_true;
    }
    if (afw_value_is_void(arg1)) {
        return afw_value_is_void(arg2)
            ? afw_boolean_v_false
            : afw_boolean_v_true;
    }
    if (afw_value_is_void(arg2)) {
        return afw_boolean_v_true;
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
        return afw_boolean_v_true;
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

    return (cmp == 0) ? afw_boolean_v_false : afw_boolean_v_true;
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
    const afw_value_array_t *array;
    const afw_iterator_old_t *iterator;
    const afw_value_t *value;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 1, array);
    if (afw_array_get_count(array->internal, x->xctx) != 1) {
        AFW_THROW_ERROR_Z(argument_error,
            "arg must have exactly one value", x->xctx);
    }

    iterator = NULL;
    value = afw_array_get_next_value(array->internal, &iterator, x->xctx);
    if (afw_value_get_data_type(value, x->xctx) != x->data_type) {
        AFW_THROW_ERROR_FZ(argument_error, x->xctx,
            "The value in the array must be data type %ku",
            &x->data_type->data_type_id);
    }
    return value;
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
        return afw_v_a_empty_string;
    }

    /** @fixme How should this limit be handled? 1000 was arbitrary. */
    if (times->internal > 1000) {
        AFW_THROW_ERROR_Z(argument_error, "Parameter times exceeds 1000", x->xctx);
    }

    result = afw_value_allocate_unmanaged_string(x->p, x->xctx);
    result->internal.len = (afw_size_t)times->internal * value->internal.len;
    result->internal.s = s = afw_pool_malloc(x->p,
        result->internal.len, x->xctx);
    for (count = times->internal; count > 0; count--) {
        memcpy(s, value->internal.s, value->internal.len);
        s += value->internal.len;
    }

    return &result->pub;
}



/* Defined with the match functions below. */
static const afw_value_t *
impl_replace_with_function(
    afw_function_execute_t *x,
    const afw_value_t *value,
    const afw_utf8_t *search,
    const afw_utf8_regexp_t *regexp,
    const afw_value_t *function,
    afw_size_t limit);



/*
 * Evaluate a parameter that is a string (literal text) or a regexp (a
 * pattern), as the separator of split and the search of replace. Sets
 * one of *string and *regexp; an undefined value is an error, as for
 * any required parameter.
 */
static void
impl_evaluate_string_or_regexp(
    afw_function_execute_t *x,
    afw_size_t parameter_number,
    const afw_utf8_t **string,
    const afw_utf8_regexp_t **regexp)
{
    const afw_value_t *value;

    *string = NULL;
    *regexp = NULL;
    value = afw_function_evaluate_required_parameter(x, parameter_number,
        NULL);
    if (afw_value_is_string(value)) {
        *string = &((const afw_value_string_t *)value)->internal;
    }
    else if (afw_value_is_regexp(value)) {
        *regexp = afw_function_regexp_parameter_compiled(x,
            parameter_number, value);
    }
    else {
        AFW_THROW_ERROR_FZ(argument_error, x->xctx,
            "Parameter " AFW_SIZE_T_FMT
            " of function '%ku' must evaluate to data type 'string' or "
            "'regexp' but evaluated to be '%ku'",
            parameter_number,
            &x->function->functionId->internal,
            &afw_value_get_data_type(value, x->xctx)->data_type_id);
    }
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

/*
 * Empty match: insert at code-point boundaries, including start and end.
 * Default limit 1 → insert at start only. limit -1 → every boundary.
 * Do not consume the next code point (it is not the match).
 */
static const afw_value_t *
impl_replace_empty_match(
    const afw_value_t *value,
    const afw_utf8_t *src,
    const afw_utf8_t *replacement,
    afw_size_t limit,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_utf8_t remaining;
    afw_value_string_t *result;
    afw_utf8_octet_t *s;
    afw_size_t count;
    afw_size_t made;
    afw_size_t len;
    afw_size_t cp_len;

    /* Inserting nothing at every boundary is the original value. */
    if (replacement->len == 0) {
        return value;
    }

    afw_memory_copy(&remaining, src);
    count = 0;
    while (count < limit) {
        count++;
        if (remaining.len == 0) {
            break;
        }
        cp_len = 0;
        if (afw_utf8_next_code_point(remaining.s, &cp_len,
            remaining.len, xctx) < 0)
        {
            AFW_THROW_ERROR_Z(general,
                "Invalid UTF-8 in replace", xctx);
        }
        remaining.s += cp_len;
        remaining.len -= cp_len;
    }

    len = src->len + (count * replacement->len);
    s = afw_pool_malloc(p, len, xctx);
    result = afw_value_allocate_unmanaged_string(p, xctx);
    result->internal.len = len;
    result->internal.s = s;

    afw_memory_copy(&remaining, src);
    for (made = 0; made < count; made++) {
        memcpy(s, replacement->s, replacement->len);
        s += replacement->len;
        if (remaining.len == 0) {
            break;
        }
        if (made + 1 == count) {
            memcpy(s, remaining.s, remaining.len);
            break;
        }
        cp_len = 0;
        if (afw_utf8_next_code_point(remaining.s, &cp_len,
            remaining.len, xctx) < 0)
        {
            AFW_THROW_ERROR_Z(general,
                "Invalid UTF-8 in replace", xctx);
        }
        memcpy(s, remaining.s, cp_len);
        s += cp_len;
        remaining.s += cp_len;
        remaining.len -= cp_len;
    }

    return &result->pub;
}


/* replace, and replace_all (all: every match, no limit parameter). */
static const afw_value_t *
impl_replace(
    afw_function_execute_t *x,
    afw_boolean_t all)
{
    const afw_value_t *value;
    const afw_utf8_t *match;
    const afw_utf8_regexp_t *regexp;
    const afw_value_string_t *replacement;
    const afw_value_t *replacement_value;
    const afw_value_integer_t *limit_value;
    afw_value_string_t *result;
    afw_size_t count;
    afw_size_t limit;
    afw_utf8_t remaining;
    afw_utf8_octet_t *s;
    afw_size_t len;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(value, 1);
    AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(value);
    impl_evaluate_string_or_regexp(x, 2, &match, &regexp);

    /* The replacement is a string, or a function that returns one. */
    replacement = NULL;
    replacement_value = afw_function_evaluate_required_parameter(x, 3, NULL);
    if (afw_value_is_string(replacement_value)) {
        replacement = (const afw_value_string_t *)replacement_value;
    }
    else if (!afw_value_is_script_function_definition(replacement_value) &&
        !afw_value_is_closure_binding(replacement_value) &&
        !afw_value_is_function_definition(replacement_value) &&
        !afw_value_is_function_thunk(replacement_value))
    {
        AFW_THROW_ERROR_FZ(argument_error, x->xctx,
            "Parameter 3 of function '%ku' must evaluate to data type "
            "'string' or 'function' but evaluated to be '%ku'",
            &x->function->functionId->internal,
            &afw_value_get_data_type(replacement_value, x->xctx)->
                data_type_id);
    }

    limit = all ? AFW_SIZE_T_MAX : 1;
    if (!all && AFW_FUNCTION_PARAMETER_IS_PRESENT(4))
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

    /* A function is called with each match's _AdaptiveRegexpMatch_. */
    if (!replacement) {
        return impl_replace_with_function(x, value, match, regexp,
            replacement_value, limit);
    }

    /* A regexp is a pattern, and its replacement can use its groups. */
    if (!match) {
        return afw_value_create_unmanaged_string(
            afw_utf8_regexp_replace(regexp,
                (const afw_utf8_t *)AFW_VALUE_INTERNAL(value),
                &replacement->internal, limit, x->p, x->xctx),
            x->p, x->xctx);
    }

    /* Empty match: insert at code-point boundaries (#190). */
    if (match->len == 0) {
        return impl_replace_empty_match(value,
            &(((const afw_value_string_t *)value)->internal),
            &replacement->internal, limit, x->p, x->xctx);
    }

    /* Count number of replacement that will be made. */
    afw_memory_copy(&remaining, &(((afw_value_string_t *)value)->internal));
    if (remaining.len == 0) {
        return afw_v_a_empty_string;
    }
    /*
     * Advance search by code point only (#153) so matches are not attempted
     * mid multi-byte sequence.
     */
    for (count = 0; count < limit && remaining.len >= match->len; )
    {
        if (memcmp(remaining.s, match->s, match->len) == 0)
        {
            remaining.s += match->len;
            remaining.len -= match->len;
            count++;
        }
        else {
            afw_size_t cp_len;

            cp_len = 0;
            if (afw_utf8_next_code_point(remaining.s, &cp_len,
                remaining.len, x->xctx) < 0)
            {
                break;
            }
            remaining.s += cp_len;
            remaining.len -= cp_len;
        }
    }

    /* If no replacements, just return value. */
    if (count == 0) {
        return value;
    }

    /* Make a result string large enough to hold replacement. */
    afw_memory_copy(&remaining, &(((afw_value_string_t *)value)->internal));
    len = remaining.len - (count * match->len) +
        (count * replacement->internal.len);
    s = afw_pool_malloc(x->p, len, x->xctx);
    result = afw_value_allocate_unmanaged_string(x->p, x->xctx);
    result->internal.len = len;
    result->internal.s = s;

    /* Do replaces and return result (same CP-step search). */
    for (; count > 0; count--) {
        for (;;) {
            if (remaining.len >= match->len &&
                memcmp(remaining.s, match->s, match->len)
                    == 0)
            {
                memcpy(s, replacement->internal.s, replacement->internal.len);
                s += replacement->internal.len;
                remaining.s += match->len;
                remaining.len -= match->len;
                break;
            }
            else {
                afw_size_t cp_len;

                cp_len = 0;
                if (afw_utf8_next_code_point(remaining.s, &cp_len,
                    remaining.len, x->xctx) < 0)
                {
                    AFW_THROW_ERROR_Z(general,
                        "Invalid UTF-8 in replace", x->xctx);
                }
                memcpy(s, remaining.s, cp_len);
                s += cp_len;
                remaining.s += cp_len;
                remaining.len -= cp_len;
            }
        }
    }
    if (remaining.len > 0) {
        memcpy(s, remaining.s, remaining.len);
    }
    return &result->pub;
}


const afw_value_t *
afw_function_execute_replace(
    afw_function_execute_t *x)
{
    return impl_replace(x, false);
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
    const afw_value_array_t *array1;
    const afw_value_array_t *array2;
    const afw_data_type_t *data_type_1;
    const afw_data_type_t *data_type_2;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array1, 1, array);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array2, 2, array);

    data_type_1 = impl_set_array_data_type(array1->internal, x);
    data_type_2 = impl_set_array_data_type(array2->internal, x);
    if (!data_type_1 || data_type_1 != data_type_2) {
        AFW_THROW_ERROR_Z(argument_error,
            "array1 and array2 must have a data type that matches", x->xctx);
    }

    return (impl_is_subset_array(array1->internal, array2->internal, x->xctx) &&
        impl_is_subset_array(array2->internal, array1->internal, x->xctx))
        ? afw_boolean_v_true
        : afw_boolean_v_false;
}



/* Context for impl_split_cb(). */
typedef struct {
    const afw_array_t *array;
    afw_integer_t limit;
    afw_integer_t length;

    /* Byte offset where the current piece starts. */
    afw_integer_t last;

    /* True when the array has limit entries. */
    afw_boolean_t full;
} impl_split_ctx_t;


/* Push one entry of a regexp split, or undefined; false when full. */
static afw_boolean_t
impl_split_push(
    impl_split_ctx_t *ctx,
    const afw_utf8_t *subject,
    afw_integer_t start,
    afw_integer_t end,
    afw_xctx_t *xctx)
{
    afw_utf8_t piece;

    if (start < 0) {
        afw_array_push_value(ctx->array, afw_value_undefined, xctx);
    }
    else {
        piece.s = subject->s + start;
        piece.len = (afw_size_t)(end - start);
        afw_array_of_string_add_internal(ctx->array, &piece, xctx);
    }
    ctx->length++;
    if (ctx->length >= ctx->limit) {
        ctx->full = true;
    }
    return !ctx->full;
}


/*
 * Split at a match, as ECMAScript's split does with a regexp: the piece
 * before the match, then the text of each group (undefined when it did
 * not take part). An empty match where the current piece starts, or at
 * the end of the value, does not split.
 */
static afw_boolean_t
impl_split_cb(
    const afw_utf8_regexp_match_t *match,
    void *data,
    afw_xctx_t *xctx)
{
    impl_split_ctx_t *ctx = data;
    afw_size_t group;

    if (match->starts[0] >= (afw_integer_t)match->subject->len) {
        return false;
    }
    if (match->ends[0] == ctx->last) {
        return true;
    }
    if (!impl_split_push(ctx, match->subject, ctx->last, match->starts[0],
        xctx))
    {
        return false;
    }
    for (group = 1; group <= match->group_count; group++) {
        if (!impl_split_push(ctx, match->subject, match->starts[group],
            match->ends[group], xctx))
        {
            return false;
        }
    }
    ctx->last = match->ends[0];
    return true;
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
    const afw_value_t *value;
    const afw_value_integer_t *limit_value;
    const afw_array_t *array;
    const afw_value_t *result;
    const afw_utf8_t *separator;
    const afw_utf8_regexp_t *regexp;
    afw_integer_t count;
    afw_integer_t limit;
    afw_utf8_t remaining;
    afw_utf8_t split;
    afw_boolean_t more;
    impl_split_ctx_t ctx;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(value, 1);
    AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(value);

    separator = NULL;
    regexp = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        impl_evaluate_string_or_regexp(x, 2, &separator, &regexp);
        if (separator && separator->len == 0) {
            separator = NULL;
        }
    }

    limit = AFW_INTEGER_MAX;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(limit_value,
            3, integer);
        /* As replace and match_all: a negative limit is no limit. */
        if (limit_value->internal >= 0) {
            limit = limit_value->internal;
        }
    }

    result = afw_pool_scope_release_value_at_cleanup(
        afw_array_create_managed(afw_data_type_string, x->p, x->xctx)->value,
        x->p, x->xctx);
    array = ((const afw_value_array_t *)result)->internal;
    afw_memory_copy(&remaining, &(((afw_value_string_t *)value)->internal));

    if (regexp) {
        if (limit <= 0) {
            return result;
        }

        /* An empty value is [] if the regexp matches it, else [""]. */
        if (remaining.len == 0) {
            if (!afw_utf8_regexp_find(regexp, &remaining, x->xctx)) {
                afw_array_of_string_add_internal(array, &remaining,
                    x->xctx);
            }
            return result;
        }

        afw_memory_clear(&ctx);
        ctx.array = array;
        ctx.limit = limit;
        afw_utf8_regexp_for_each_match(regexp, &remaining, AFW_SIZE_T_MAX,
            impl_split_cb, &ctx, x->p, x->xctx);
        if (!ctx.full) {
            impl_split_push(&ctx, &remaining, ctx.last,
                (afw_integer_t)remaining.len, x->xctx);
        }
    }

    else if (separator) {
        /*
         * Find separator only at code-point boundaries (#153). There is one
         * more piece than separators, as in ECMAScript: "a," is ["a", ""]
         * and "" is [""] (the piece after a trailing separator was lost).
         */
        more = true;
        for (count = 0; count < limit && more; count++) {
            more = false;
            split.s = remaining.s;
            split.len = remaining.len;
            for (;;) {
                if (remaining.len < separator->len) {
                    remaining.len = 0;
                    break;
                }
                if (memcmp(remaining.s, separator->s, separator->len) == 0) {
                    split.len = remaining.s - split.s;
                    remaining.s += separator->len;
                    remaining.len -= separator->len;
                    more = true;
                    break;
                }
                {
                    afw_size_t cp_len;

                    cp_len = 0;
                    if (afw_utf8_next_code_point(remaining.s, &cp_len,
                        remaining.len, x->xctx) < 0)
                    {
                        AFW_THROW_ERROR_Z(general,
                            "Invalid UTF-8 in split", x->xctx);
                    }
                    remaining.s += cp_len;
                    remaining.len -= cp_len;
                }
            }
            afw_array_of_string_add_internal(array, &split, x->xctx);
        }
    }

    else {
        /*
         * Empty / omitted separator: one entry per Unicode code point.
         * afw_utf8_t is always valid UTF-8; never split mid-sequence by octet.
         */
        for (count = 0; count < limit && remaining.len > 0; count++) {
            afw_size_t offset;

            offset = 0;
            if (afw_utf8_next_code_point(remaining.s, &offset,
                remaining.len, x->xctx) < 0)
            {
                AFW_THROW_ERROR_Z(general,
                    "Invalid UTF-8 in split (should be unreachable for "
                    "afw_utf8_t)",
                    x->xctx);
            }
            split.s = remaining.s;
            split.len = offset;
            remaining.s += offset;
            remaining.len -= offset;
            afw_array_of_string_add_internal(array, &split, x->xctx);
        }
    }

    return result;
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
        ? afw_boolean_v_true
        : afw_boolean_v_false;
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
    const afw_value_array_t *array1;
    const afw_value_array_t *array2;
    const afw_data_type_t *data_type_1;
    const afw_data_type_t *data_type_2;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array1, 1, array);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array2, 2, array);

    data_type_1 = impl_set_array_data_type(array1->internal, x);
    data_type_2 = impl_set_array_data_type(array2->internal, x);
    if (!data_type_1 || data_type_1 != data_type_2) {
        AFW_THROW_ERROR_Z(argument_error,
            "array1 and array2 must have a data type that matches", x->xctx);
    }

    return impl_is_subset_array(array1->internal, array2->internal, x->xctx)
        ? afw_boolean_v_true
        : afw_boolean_v_false;
}



/*
 * Common polymorphic function for substring
 *
 * afw_function_execute_substring
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Returns the string substring of value beginning at zero-based position
 * integer startIndex and ending at the position before integer endIndex.
 * Specify -1 or omit endIndex to return through the end of value. The result is
 * always string (a slice of anyURI is not an anyURI).
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
 *
 * Errors thrown:
 *
 *   argument_error - startIndex or endIndex is out of range
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
        AFW_THROW_ERROR_Z(argument_error, "startIndex is out of bounds", x->xctx);
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
            AFW_THROW_ERROR_Z(argument_error, "endIndex is out of bounds", x->xctx);
        }
        end = afw_safe_cast_integer_to_size(endIndex->internal, x->xctx);
    }

    /* Set substring.s */
    memset(&substring, 0, sizeof(afw_utf8_t));
    for (offset = 0, pos = 0; pos < start; pos++)
    {
        if (afw_utf8_next_code_point(s->s, &offset, s->len, x->xctx) < 0) {
            AFW_THROW_ERROR_Z(argument_error,
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
                AFW_THROW_ERROR_Z(argument_error,
                    "startIndex and/or endIndex is out of bounds", x->xctx);
            }
        }
        substring.len = s->s + offset - substring.s;
    }

    /* Return substring. */
    return afw_value_create_unmanaged_string(&substring, x->p, x->xctx);
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
    const afw_value_array_t *array1;
    const afw_value_array_t *arrayn;
    const afw_value_array_t *result;
    const afw_data_type_t *data_type;
    afw_size_t i;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array1, 1, array);

    data_type = impl_set_array_data_type(array1->internal, x);
    if (!data_type)
    {
        AFW_THROW_ERROR_Z(argument_error,
            "all arrays must have the same data type",
            x->xctx);
    }

    result = (const afw_value_array_t *)
        afw_pool_scope_release_value_at_cleanup(
            afw_array_create_managed(data_type, x->p, x->xctx)->value,
            x->p, x->xctx);
    impl_add_nondups_to_array(data_type, array1->internal,
        result->internal, x->xctx);
    for (i = 2; i <= x->argc; i++) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arrayn, i, array);
        if (impl_set_array_data_type(arrayn->internal, x) != data_type) {
            AFW_THROW_ERROR_Z(argument_error,
                "all arrays must have the same data type",
                x->xctx);
        }
        impl_add_nondups_to_array(data_type, arrayn->internal,
            result->internal, x->xctx);
    }

    return &result->pub;
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
    return afw_value_create_unmanaged_integer(result, x->p, x->xctx);
}



/*
 * Common polymorphic function for url_encode
 *
 * afw_function_execute_url_encode
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * URL encode a `<dataType>` value and return the string result.
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
    const afw_value_anyURI_t *unencoded;
    const afw_utf8_t *s;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(unencoded, 1, anyURI);

    s = afw_uri_encode(&unencoded->internal,
        AFW_URI_OCTET_ENCODE_URI, x->p, x->xctx);
    
    /* Return String value. */
    return afw_value_create_unmanaged_string(s, x->p, x->xctx);
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
    afw_value_base64Binary_t *result;
    const afw_value_string_t *value;

    /* Only string supported at the moment. */
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(value, 1, string);

    result = afw_value_allocate_unmanaged_base64Binary(x->p, x->xctx);
    result->internal.ptr = (const afw_octet_t *)value->internal.s;
    result->internal.size = value->internal.len;

    return &result->pub;
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
    afw_value_hexBinary_t *result;
    const afw_value_string_t *value;

    /* Only string supported at the moment. */
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(value, 1, string);

    result = afw_value_allocate_unmanaged_hexBinary(x->p, x->xctx);
    result->internal.ptr = (const afw_octet_t *)value->internal.s;
    result->internal.size = value->internal.len;

    return &result->pub;
}



/*
 * Common polymorphic function for is
 *
 * afw_function_execute_is
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Checks whether value is dataType `<dataType>` and returns the boolean
 * result. There is no is function: each is_<dataType> is a plain function
 * (dataTypeRequired), also written is<dataType>.
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
 *   function is_<dataType>(
 *       value: any
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   value - (any) Value to check.
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
        return afw_boolean_v_false;
    }

    data_type = afw_value_get_data_type(arg, x->xctx);
    return (x->data_type == data_type)
        ? afw_boolean_v_true
        : afw_boolean_v_false;
}



/*
 * Common polymorphic function for as
 *
 * afw_function_execute_as
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Returns value unchanged if it is dataType `<dataType>`. Otherwise,
 * including when value is undefined, throws an error. It never converts.
 * There is no as function: each as_<dataType> is a plain function
 * (dataTypeRequired), usually written value as <dataType>.
 *
 * The parameter is any, not `<dataType>`, so the built-in parameter check
 * does not throw first: this error says how to get a `<dataType>` instead
 * (afw_value_type_assertion_mismatch_z), the same words as the compile time
 * check under typeCheck.
 *
 * Supported `<dataType>`: the same as is_<dataType>.
 *
 * Declaration:
 *
 * ```
 *   function as_<dataType>(
 *       value: any
 *   ): <dataType>;
 * ```
 */
const afw_value_t *
afw_function_execute_as(
    afw_function_execute_t *x)
{
    const afw_value_t *arg;
    const afw_data_type_t *data_type;

    AFW_FUNCTION_EVALUATE_PARAMETER(arg, 1);

    data_type = (arg) ? afw_value_get_data_type(arg, x->xctx) : NULL;
    if (data_type != x->data_type) {
        AFW_THROW_ERROR_FZ(argument_error, x->xctx,
            "Type assertion failed: %s",
            afw_value_type_assertion_mismatch_z(x->data_type, data_type, arg,
                x->p, x->xctx));
    }

    return arg;
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
    const afw_value_t *value;
    const afw_value_object_t *object;
    const afw_value_array_t *array;

    AFW_FUNCTION_EVALUATE_PARAMETER(value, 1);
    if (!value || afw_value_is_undefined(value)) {
        AFW_THROW_ERROR_Z(general,
            "freeze requires an object or array", x->xctx);
    }

    /*
     * Hold the input first (get_assignable; last-release of that
     * hold on dest p), then freeze that handle. Not a fresh
     * create_managed. Assign later bumps the frozen face instead of
     * wrapping a raw immutable instance into a mutable overlay.
     */
    value = afw_pool_scope_get_assignable_for_scope_lifetime(
        value, x->p, x->xctx);

    if (afw_value_is_object(value)) {
        object = (const afw_value_object_t *)value;
        afw_object_set_immutable(object->internal, x->xctx);
        return value;
    }

    if (afw_value_is_array(value)) {
        array = (const afw_value_array_t *)value;
        afw_array_set_immutable(array->internal, x->xctx);
        return value;
    }

    AFW_THROW_ERROR_Z(general,
        "freeze requires an object or array", x->xctx);
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
    const afw_value_string_t *value;
    const afw_value_t *result;
    const afw_utf8_t *error_message;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(value, 1, string);

    /* Pooled in x->p (caller does not release). */
    result = afw_value_compiled_regexp_create(&value->internal,
        &error_message, x->p, x->xctx);
    if (!result) {
        AFW_THROW_ERROR_FZ(conversion_error, x->xctx,
            "Invalid regular expression: %ku",
            error_message);
    }

    return result;
}



/* Context for impl_match_object_cb(). */
typedef struct {
    afw_function_execute_t *x;
    const afw_utf8_regexp_group_name_t *names;
    afw_size_t names_count;

    /* Set to the match object for match(). */
    const afw_value_t *result;

    /* Each match object is pushed here for match_all(). */
    const afw_array_t *array;
} impl_match_object_ctx_t;


/* Text of a group of match as a string value, or undefined. */
static const afw_value_t *
impl_match_group_value(
    const afw_utf8_regexp_match_t *match,
    afw_size_t group,
    afw_function_execute_t *x)
{
    afw_utf8_t text;

    if (match->starts[group] < 0) {
        return afw_value_undefined;
    }
    text.s = match->subject->s + match->starts[group];
    text.len = (afw_size_t)(match->ends[group] - match->starts[group]);

    /* A copy: the value searched may not live as long as dest p. */
    return afw_value_create_unmanaged_string(
        afw_utf8_clone(&text, x->p, x->xctx), x->p, x->xctx);
}


/* An _AdaptiveRegexpMatch_ object for match, last-released with x->p. */
static const afw_value_t *
impl_match_object(
    const afw_utf8_regexp_match_t *match,
    const afw_utf8_regexp_group_name_t *names,
    afw_size_t names_count,
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_object_t *object;
    const afw_object_t *groups;
    const afw_array_t *captures;
    const afw_value_t *value;
    const afw_value_t *result;
    afw_size_t group;
    afw_size_t i;

    /* Each create_managed is RC 1; last-release of that hold on dest p. */
    object = afw_object_create_managed(x->p, xctx);
    result = afw_pool_scope_release_value_at_cleanup(object->value,
        x->p, xctx);
    afw_object_meta_set_object_type_id(object,
        afw_s__AdaptiveRegexpMatch_, xctx);

    captures = ((const afw_value_array_t *)
        afw_pool_scope_release_value_at_cleanup(
            afw_array_create_managed(NULL, x->p, xctx)->value,
            x->p, xctx))->internal;
    for (group = 0; group <= match->group_count; group++) {
        afw_array_push_value(captures,
            impl_match_group_value(match, group, x), xctx);
    }

    afw_object_set_property(object, afw_v_match,
        afw_array_get_entry_value(captures, 0, xctx), xctx);
    afw_object_set_property(object, afw_v_index,
        afw_value_create_unmanaged_integer(match->index, x->p, xctx),
        xctx);
    afw_object_set_property(object, afw_v_captures, captures->value, xctx);

    if (names_count > 0) {
        groups = afw_object_create_managed(x->p, xctx);
        afw_pool_scope_release_value_at_cleanup(groups->value, x->p, xctx);
        for (i = 0; i < names_count; i++) {
            value = impl_match_group_value(match, names[i].number, x);
            if (value != afw_value_undefined) {
                afw_object_set_property(groups,
                    afw_value_create_unmanaged_string(&names[i].name,
                        x->p, xctx),
                    value, xctx);
            }
        }
        afw_object_set_property(object, afw_v_groups, groups->value, xctx);
    }

    return result;
}


/* Make an _AdaptiveRegexpMatch_ object for match() or match_all(). */
static afw_boolean_t
impl_match_object_cb(
    const afw_utf8_regexp_match_t *match,
    void *data,
    afw_xctx_t *xctx)
{
    impl_match_object_ctx_t *ctx = data;
    const afw_value_t *result;

    result = impl_match_object(match, ctx->names, ctx->names_count, ctx->x);
    if (ctx->array) {
        afw_array_push_value(ctx->array, result, xctx);
        return true;
    }

    ctx->result = result;
    return false;
}



/* Context for impl_replace_with_function_cb(). */
typedef struct {
    afw_function_execute_t *x;
    const afw_utf8_regexp_group_name_t *names;
    afw_size_t names_count;
    const afw_writer_t *writer;

    /* The call of the function; args[1] is set to each match object. */
    const afw_value_t *call;
    const afw_value_t *args[2];

    /* Byte offset just past the last match. */
    afw_integer_t last;
} impl_replace_with_function_ctx_t;


/* Write the text before a match and what the function returns for it. */
static afw_boolean_t
impl_replace_with_function_cb(
    const afw_utf8_regexp_match_t *match,
    void *data,
    afw_xctx_t *xctx)
{
    impl_replace_with_function_ctx_t *ctx = data;
    afw_function_execute_t *x = ctx->x;
    const afw_value_t *result;
    const afw_data_type_t *data_type;

    afw_writer_write(ctx->writer, match->subject->s + ctx->last,
        (afw_size_t)(match->starts[0] - ctx->last), xctx);

    ctx->args[1] = impl_match_object(match, ctx->names, ctx->names_count, x);
    result = afw_value_evaluate(ctx->call, x->p, xctx);
    if (!afw_value_is_string(result)) {
        data_type = afw_value_is_undefined(result)
            ? NULL
            : afw_value_get_data_type(result, xctx);
        AFW_THROW_ERROR_FZ(argument_error, xctx,
            "The replacement function of '%ku' must return a string, but "
            "returned %s%ku%s",
            &x->function->functionId->internal,
            data_type ? "data type '" : "",
            data_type ? &data_type->data_type_id : afw_s_undefined,
            data_type ? "'" : "");
    }
    afw_writer_write(ctx->writer,
        ((const afw_value_string_t *)result)->internal.s,
        ((const afw_value_string_t *)result)->internal.len, xctx);

    ctx->last = match->ends[0];
    return true;
}


/*
 * replace() with a function as the replacement. A string search is
 * literal text, so it is matched as its escaped regexp: an empty string
 * then matches at each code point boundary, as replace() with a string
 * replacement does.
 */
static const afw_value_t *
impl_replace_with_function(
    afw_function_execute_t *x,
    const afw_value_t *value,
    const afw_utf8_t *search,
    const afw_utf8_regexp_t *regexp,
    const afw_value_t *function,
    afw_size_t limit)
{
    afw_xctx_t *xctx = x->xctx;
    impl_replace_with_function_ctx_t ctx;
    const afw_utf8_t *subject;
    const afw_utf8_t *error_message;
    const afw_utf8_t * volatile result;
    afw_utf8_t current;

    if (!regexp) {
        regexp = afw_utf8_regexp_compile(
            afw_utf8_regexp_escape(search, x->p, x->xctx), 0,
            &error_message, x->p, x->xctx);
        if (!regexp) {
            AFW_THROW_ERROR_FZ(general, x->xctx,
                "The escaped search text did not compile: %ku",
                error_message);
        }
    }

    subject = (const afw_utf8_t *)AFW_VALUE_INTERNAL(value);
    afw_memory_clear(&ctx);
    ctx.x = x;
    afw_utf8_regexp_groups(regexp, &ctx.names, &ctx.names_count);
    ctx.args[0] = function;
    ctx.call = afw_value_call_create(afw_function_execute_contextual(x),
        1, &ctx.args[0], false, x->p, x->xctx);
    ctx.writer = afw_utf8_writer_create(NULL, x->p, x->xctx);

    result = NULL;
    AFW_TRY {
        afw_utf8_regexp_for_each_match(regexp, subject, limit,
            impl_replace_with_function_cb, &ctx, x->p, x->xctx);
        afw_writer_write(ctx.writer, subject->s + ctx.last,
            subject->len - (afw_size_t)ctx.last, x->xctx);

        /* The current string is a view into the writer's pool. */
        afw_utf8_writer_current_string(ctx.writer, &current, x->xctx);
        result = afw_utf8_clone(&current, x->p, x->xctx);
    }
    AFW_FINALLY {
        afw_writer_release(ctx.writer, x->xctx);
    }
    AFW_ENDTRY;

    return afw_value_create_unmanaged_string(result, x->p, x->xctx);
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
    const afw_value_t *value;
    const afw_utf8_regexp_t *regexp;
    impl_match_object_ctx_t ctx;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(value, 1);
    AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(value);

    afw_memory_clear(&ctx);
    ctx.x = x;
    regexp = afw_function_evaluate_regexp_parameter(x, 2);
    afw_utf8_regexp_groups(regexp, &ctx.names, &ctx.names_count);

    afw_utf8_regexp_for_each_match(regexp,
        (const afw_utf8_t *)AFW_VALUE_INTERNAL(value), 1,
        impl_match_object_cb, &ctx, x->p, x->xctx);

    return ctx.result ? ctx.result : afw_value_undefined;
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
    const afw_value_t *value;
    const afw_utf8_regexp_t *regexp;
    const afw_value_integer_t *limit_value;
    const afw_value_t *result;
    impl_match_object_ctx_t ctx;
    afw_size_t limit;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(value, 1);
    AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(value);
    regexp = afw_function_evaluate_regexp_parameter(x, 2);

    /* As replace: a negative limit is every match. */
    limit = AFW_SIZE_T_MAX;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(
            limit_value, 3, integer);
        if (limit_value->internal >= 0 &&
            limit_value->internal < AFW_SIZE_T_MAX)
        {
            limit = (afw_size_t)limit_value->internal;
        }
    }

    /* New array: create_managed RC 1. Last-release of that hold on dest p. */
    result = afw_pool_scope_release_value_at_cleanup(
        afw_array_create_managed(NULL, x->p, x->xctx)->value,
        x->p, x->xctx);

    afw_memory_clear(&ctx);
    ctx.x = x;
    ctx.array = ((const afw_value_array_t *)result)->internal;
    afw_utf8_regexp_groups(regexp, &ctx.names, &ctx.names_count);

    afw_utf8_regexp_for_each_match(regexp,
        (const afw_utf8_t *)AFW_VALUE_INTERNAL(value), limit,
        impl_match_object_cb, &ctx, x->p, x->xctx);

    return result;
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
    const afw_value_t *value;
    const afw_utf8_regexp_t *regexp;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(value, 1);
    AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(value);
    regexp = afw_function_evaluate_regexp_parameter(x, 2);

    return afw_value_create_unmanaged_integer(
        afw_utf8_regexp_index_of(regexp,
            (const afw_utf8_t *)AFW_VALUE_INTERNAL(value), x->xctx),
        x->p, x->xctx);
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
    const afw_value_t *value;
    const afw_value_integer_t *index;
    const afw_utf8_t *s;
    afw_integer_t i;
    afw_integer_t count;
    afw_size_t offset;
    afw_code_point_t cp;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(value, 1);
    AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(value);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(index, 2, integer);
    s = AFW_VALUE_INTERNAL(value);

    /* Negative from the end, and out of range is undefined, as at. */
    i = index->internal;
    if (i < 0) {
        count = impl_code_point_count(s, x->xctx);
        i = count + i;
        if (i < 0) {
            return afw_value_undefined;
        }
    }
    for (offset = 0; ; i--) {
        cp = afw_utf8_next_code_point(s->s, &offset, s->len, x->xctx);
        if (cp < 0) {
            return afw_value_undefined;
        }
        if (i == 0) {
            return afw_value_create_unmanaged_integer(cp, x->p, x->xctx);
        }
    }
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
    const afw_value_t *value;
    const afw_value_string_t *form;
    const afw_utf8_t *s;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(value, 1);
    AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(value);
    s = AFW_VALUE_INTERNAL(value);

    /* Every string is already NFC. */
    if (!AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        return impl_utf8_as_string(value, s, x);
    }
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(form, 2, string);
    if (afw_utf8_equal_utf8_z(&form->internal, "NFC")) {
        return impl_utf8_as_string(value, s, x);
    }
    if (afw_utf8_equal_utf8_z(&form->internal, "NFKC")) {
        return impl_utf8_as_string(value,
            afw_utf8_nfkc(s, x->p, x->xctx), x);
    }
    if (afw_utf8_equal_utf8_z(&form->internal, "NFD") ||
        afw_utf8_equal_utf8_z(&form->internal, "NFKD"))
    {
        AFW_THROW_ERROR_FZ(argument_error, x->xctx,
            "normalize form \"%ku\": a string in Adaptive Script is"
            " always NFC, so a decomposed string can not be made; use"
            " \"NFC\" or \"NFKC\"",
            &form->internal);
    }
    AFW_THROW_ERROR_FZ(argument_error, x->xctx,
        "normalize form must be \"NFC\" or \"NFKC\", not \"%ku\"",
        &form->internal);
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
    return impl_replace(x, true);
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
    const afw_value_t *value;
    const afw_utf8_t *s;
    afw_utf8_t trimmed;
    afw_size_t offset;
    afw_size_t cp_start;
    afw_size_t start;
    afw_size_t end;
    afw_code_point_t cp;
    afw_boolean_t trim_start;
    afw_boolean_t trim_end;
    afw_boolean_t found;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(value, 1);
    AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(value);
    s = AFW_VALUE_INTERNAL(value);

    /* trim, trim_start and trim_end. */
    trim_start = !afw_utf8_equal(&x->function->untypedFunctionId->internal,
        afw_s_trim_end);
    trim_end = !afw_utf8_equal(&x->function->untypedFunctionId->internal,
        afw_s_trim_start);

    /* ECMAScript's WhiteSpace and LineTerminator. */
    start = 0;
    end = s->len;
    for (offset = 0, found = false; ; ) {
        cp_start = offset;
        cp = afw_utf8_next_code_point(s->s, &offset, s->len, x->xctx);
        if (cp < 0) {
            break;
        }
        if (afw_code_point_is_whitespace_or_eol(cp)) {
            continue;
        }
        if (!found) {
            found = true;
            if (trim_start) {
                start = cp_start;
            }
        }
        if (trim_end) {
            end = offset;
        }
    }
    if (!found) {
        return afw_v_a_empty_string;
    }

    trimmed.s = s->s + start;
    trimmed.len = end - start;
    return impl_utf8_as_string(value,
        (trimmed.len == s->len) ? s : &trimmed, x);
}



/*
 * Common polymorphic function for to_upper_case and to_lower_case
 *
 * afw_function_execute_to_case
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Unicode full case mapping, the same in every locale, as ECMAScript
 * toUpperCase / toLowerCase do. The result is a string.
 */
const afw_value_t *
afw_function_execute_to_case(
    afw_function_execute_t *x)
{
    const afw_value_t *value;
    const afw_utf8_t *s;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(value, 1);
    AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(value);
    s = AFW_VALUE_INTERNAL(value);

    return impl_utf8_as_string(value,
        afw_utf8_case_map(s,
            afw_utf8_equal(&x->function->untypedFunctionId->internal,
                afw_s_to_upper_case),
            x->p, x->xctx),
        x);
}



/* Most code points pad_start / pad_end make, as create_array's length. */
#define IMPL_PAD_MAX_LENGTH 1000000

/* The default fillString. */
static const afw_utf8_t impl_pad_space =
    AFW_UTF8_LITERAL(" ");

/*
 * Common polymorphic function for pad_start and pad_end
 *
 * afw_function_execute_pad
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Pad the value with fillString, repeated and cut by code point, until it
 * is maxLength code points long, as ECMAScript padStart / padEnd do. An
 * empty fillString when padding is needed is an error (ECMAScript returns
 * the value, shorter than asked). The result is a string.
 */
const afw_value_t *
afw_function_execute_pad(
    afw_function_execute_t *x)
{
    const afw_value_t *value;
    const afw_value_integer_t *max_length;
    const afw_value_string_t *fill_value;
    const afw_utf8_t *s;
    const afw_utf8_t *fill;
    afw_value_string_t *result;
    afw_utf8_octet_t *c;
    afw_integer_t count;
    afw_integer_t need;
    afw_integer_t fill_count;
    afw_integer_t i;
    afw_size_t rest_len;
    afw_size_t pad_len;
    afw_size_t offset;
    afw_boolean_t at_start;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(value, 1);
    AFW_FUNCTION_ASSERT_VALUE_IS_UTF8(value);
    s = AFW_VALUE_INTERNAL(value);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(max_length, 2, integer);
    fill = &impl_pad_space;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(fill_value, 3,
            string);
        fill = &fill_value->internal;
    }
    at_start = afw_utf8_equal(&x->function->untypedFunctionId->internal,
        afw_s_pad_start);

    /* Already long enough: the value, as a string. */
    count = impl_code_point_count(s, x->xctx);
    if (max_length->internal <= count) {
        return impl_utf8_as_string(value, s, x);
    }
    if (max_length->internal > IMPL_PAD_MAX_LENGTH) {
        AFW_THROW_ERROR_FZ(argument_error, x->xctx,
            "%ku maxLength must not exceed %d",
            &x->function->functionId->internal, IMPL_PAD_MAX_LENGTH);
    }
    if (fill->len == 0) {
        AFW_THROW_ERROR_FZ(argument_error, x->xctx,
            "%ku with an empty fillString can not make the value "
            AFW_INTEGER_FMT " code points long",
            &x->function->functionId->internal, max_length->internal);
    }

    /* Whole copies of fill, then the first code points of one more. */
    need = max_length->internal - count;
    fill_count = impl_code_point_count(fill, x->xctx);
    for (offset = 0, i = need % fill_count; i > 0; i--) {
        afw_utf8_next_code_point(fill->s, &offset, fill->len, x->xctx);
    }
    rest_len = offset;
    pad_len = (afw_size_t)(need / fill_count) * fill->len + rest_len;

    result = afw_value_allocate_unmanaged_string(x->p, x->xctx);
    result->internal.len = s->len + pad_len;
    result->internal.s = c = afw_pool_malloc(x->p, result->internal.len,
        x->xctx);
    if (!at_start) {
        memcpy(c, s->s, s->len);
        c += s->len;
    }
    for (i = need / fill_count; i > 0; i--) {
        memcpy(c, fill->s, fill->len);
        c += fill->len;
    }
    memcpy(c, fill->s, rest_len);
    c += rest_len;
    if (at_start) {
        memcpy(c, s->s, s->len);
    }

    return &result->pub;
}
