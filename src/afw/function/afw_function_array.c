// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for array
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_array.c
 * @brief Adaptive function execute implementations for category `array`.
 */

#include "afw_internal.h"



/*
 * Adaptive function: add_entries
 *
 * afw_function_execute_add_entries
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Append every entry of each source array onto the end of target, in order.
 * target must be mutable. Entries are copied by value reference; nested objects
 * and arrays are not deeply cloned.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function add_entries(
 *       target: array,
 *       source_1: array,
 *       ...source_rest: array
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   target - (array) Target array. This array must not be immutable.
 *
 *   source - (1 or more array) Source array(s).
 *
 * Returns:
 *
 *   (array) The modified target array.
 */
const afw_value_t *
afw_function_execute_add_entries(
    afw_function_execute_t *x)
{
    const afw_value_array_t *target;
    const afw_value_array_t *source;
    const afw_iterator_old_t *iterator;
    const afw_value_t *value;
    afw_size_t count;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(target, 1, array);
    /* Mutate-input: get_assignable; last-release of that hold on dest p. */
    target = (const afw_value_array_t *)
        afw_pool_scope_get_assignable_for_scope_lifetime(
            &target->pub, x->p, x->xctx);

    for (count = 2; count <= x->argc; count++) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(source, count, array);
        for (iterator = NULL;;) {
            value = afw_array_get_next_value(
                source->internal, &iterator, x->xctx);
            if (!value) {
                break;
            }            
            afw_array_push_value(target->internal, value, x->xctx);
        }
    }

    return &target->pub;
}



/*
 * Adaptive function: array
 *
 * afw_function_execute_array
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Construct a new array from the given values (not a conversion function). Each
 * argument becomes one element, in order. If a value is written as
 * ...expression and the expression is an array, each of its entries is included
 * in order. An empty call produces an empty array. A non-spread array argument
 * is nested as a single element (array([1,2]) is [[1,2]]); use spread or a list
 * literal to flatten. For a length of undefined slots use create_array(n).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function array(
 *       ...values: any[]
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   values - (0 or more any) A value can refer to any adaptable value belonging
 *       to any data type or an array expression. In the case of an array
 *       expression, indicated by '...' followed by an expression that results
 *       in an array, every element within that array is included in the newly
 *       created array.
 *
 * Returns:
 *
 *   (array) The constructed array.
 */
/* A value for an array of data_type (NULL: any data type). */
static void
impl_push_constructed(
    afw_function_execute_t *x,
    const afw_array_t *array,
    const afw_data_type_t *data_type,
    afw_size_t n,
    const afw_value_t *value)
{
    const afw_data_type_t *got;

    if (data_type && !afw_value_is_undefined(value)) {
        got = afw_value_get_data_type(value, x->xctx);
        if (got != data_type) {
            AFW_THROW_ERROR_FZ(argument_error, x->xctx,
                "Argument " AFW_SIZE_T_FMT " of array<%ku>: %s",
                n, &data_type->data_type_id,
                afw_value_type_assertion_mismatch_z(data_type, got, value,
                    x->p, x->xctx));
        }
    }
    afw_array_push_value(array, value, x->xctx);
}



/* array() and array<dataType>(): data_type NULL makes an untyped array. */
static const afw_value_t *
impl_construct_array(
    afw_function_execute_t *x,
    const afw_data_type_t *data_type)
{
    afw_size_t n;
    const afw_array_t *array;
    const afw_array_t *l;
    const afw_value_t * const * arg;
    const afw_value_t *entry;
    const afw_value_t *value;
    const afw_iterator_old_t *iterator;

    /* Construct a new array with elements passed as arguments. */
    array = afw_array_create_unmanaged_of(data_type, x->p, x->xctx);
    for (n = 1, arg = &x->argv[1]; n <= x->argc; n++, arg++) {
        value = afw_value_evaluate(*arg, x->p, x->xctx);

        /* If array expression, add each element of array. */
        if (afw_value_is_array_expression(*arg)) {
            if (value) {
                l = afw_value_as_array_internal(value, x->p, x->xctx);
                for (iterator = NULL;;) {
                    entry = afw_array_get_next_value(l, &iterator, x->xctx);
                    if (!entry) {
                        break;
                    }
                    entry = afw_value_evaluate(entry, x->p, x->xctx);
                    impl_push_constructed(x, array, data_type, n, entry);
                }
            }
        }

        /*
         * If not an array expression, add evaluated argument as element.
         * A missing value (NULL) is an undefined element: a NULL entry
         * would end iteration of the array early.
         */
        else {
            impl_push_constructed(x, array, data_type, n,
                (value) ? value : afw_value_undefined);
        }
    }

    /* Dual face on the instance; no extra heap header. */
    return array->value;
}



const afw_value_t *
afw_function_execute_array(
    afw_function_execute_t *x)
{
    return impl_construct_array(x, NULL);
}



/*
 * Common polymorphic function for array
 *
 * afw_function_execute_array_of_data_type
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Construct a new array of `<dataType>`, the same as array() except that
 * the new array is typed. There is no polymorphic array function: each
 * array_<dataType> is a plain function (dataTypeRequired), usually written
 * array<dataType>(...values). Its parameter is not typed, so a value of
 * another data type gets this error, which says how to get a `<dataType>`.
 *
 * Declaration:
 *
 * ```
 *   function array_<dataType>(
 *       ...values: any[]
 *   ): <dataType>[];
 * ```
 */
const afw_value_t *
afw_function_execute_array_of_data_type(
    afw_function_execute_t *x)
{
    return impl_construct_array(x, x->data_type);
}



/*
 * includes, index_of and last_index_of of an array: the index of an
 * entry equal to argv[2] (afw_value_equal: by value, NaN found), from
 * argv[3] (fromIndex, as in ECMAScript), or -1.
 */
static afw_integer_t
impl_array_search(
    afw_function_execute_t *x,
    afw_boolean_t backward)
{
    const afw_value_array_t *array;
    const afw_value_t *search;
    const afw_value_integer_t *from;
    afw_integer_t len;
    afw_integer_t i;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 1, array);
    AFW_FUNCTION_EVALUATE_PARAMETER(search, 2);
    if (!search) {
        search = afw_value_undefined;
    }
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(from, 3, integer);

    len = (afw_integer_t)afw_array_get_count(array->internal, x->xctx);
    if (!backward) {
        i = from ? from->internal : 0;
        if (i < 0) {
            i = (i < -len) ? 0 : len + i;
        }
        for (; i < len; i++) {
            if (afw_value_equal(afw_array_get_entry_value(
                array->internal, i, x->xctx), search, x->xctx))
            {
                return i;
            }
        }
    }
    else {
        i = from ? from->internal : len - 1;
        if (i < 0) {
            i = len + i;
        }
        else if (i > len - 1) {
            i = len - 1;
        }
        for (; i >= 0; i--) {
            if (afw_value_equal(afw_array_get_entry_value(
                array->internal, i, x->xctx), search, x->xctx))
            {
                return i;
            }
        }
    }

    return -1;
}



/* Adaptive function includes<array>: index_of<array> >= 0. */
const afw_value_t *
afw_function_execute_includes_array(
    afw_function_execute_t *x)
{
    return impl_array_search(x, false) >= 0
        ? afw_boolean_v_true
        : afw_boolean_v_false;
}



/* Adaptive function index_of<array>. */
const afw_value_t *
afw_function_execute_index_of_array(
    afw_function_execute_t *x)
{
    return afw_value_create_unmanaged_integer(
        impl_array_search(x, false), x->p, x->xctx);
}



/* Adaptive function last_index_of<array>. */
const afw_value_t *
afw_function_execute_last_index_of_array(
    afw_function_execute_t *x)
{
    return afw_value_create_unmanaged_integer(
        impl_array_search(x, true), x->p, x->xctx);
}

/*
 * Adaptive function: create_array
 *
 * afw_function_execute_create_array
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Create a new mutable array of the given length where every entry is
 * undefined. Useful when you want a known length up front before assigning or
 * filling entries. Length must be a non-negative integer and must not exceed
 * 1,000,000. This is a length-based constructor, not a conversion function (see
 * also array(...), which builds from elements).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function create_array(
 *       length: integer
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   length - (integer) Number of undefined elements (0 or more, up to the
 *       maximum).
 *
 * Returns:
 *
 *   (array) A new array of the requested length; each entry is undefined.
 *
 * Errors thrown:
 *
 *   argument_error - length is negative or exceeds the maximum allowed
 */
/* Hard cap so create_array(huge) cannot allocate without bound. */
#define AFW_CREATE_ARRAY_MAX_LENGTH 1000000

const afw_value_t *
afw_function_execute_create_array(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *length;
    const afw_array_t *array;
    afw_integer_t n;
    afw_integer_t i;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(length, 1, integer);
    n = length->internal;

    if (n < 0 || n > AFW_CREATE_ARRAY_MAX_LENGTH) {
        AFW_THROW_ERROR_FZ(argument_error, x->xctx,
            "create_array length must be between 0 and %d inclusive",
            AFW_CREATE_ARRAY_MAX_LENGTH);
    }

    /*
     * A result meant to be modified is managed (the modifiable-result
     * rule, lifetime-principles.md): create_managed RC 1, last-release of
     * that hold on dest p. Keeping it references it, so it stays the
     * same array.
     */
    array = ((const afw_value_array_t *)
        afw_pool_scope_release_value_at_cleanup(
            afw_array_create_managed(NULL, x->p, x->xctx)->value,
            x->p, x->xctx))->internal;
    for (i = 0; i < n; i++) {
        afw_array_push_value(array, afw_value_undefined, x->xctx);
    }

    return array->value;
}

/*
 * Adaptive function: join
 *
 * afw_function_execute_join
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Concatenate the string values of the elements of an array with a separator.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function join(
 *       value: array,
 *       separator?: string
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   value - (array) An array of values (statements) of any data type.
 *
 *   separator - (optional string) The separator to use. If not specified, a
 *       comma (,) is used.
 *
 * Returns:
 *
 *   (string) Joined array values.
 */
const afw_value_t *
afw_function_execute_join(
    afw_function_execute_t *x)
{
    const afw_value_array_t *value;
    const afw_value_string_t *separator;
    const afw_utf8_t *sep;
    const afw_iterator_old_t *iterator;
    const afw_array_t *array_of_string;
    const afw_utf8_t *s;
    afw_size_t len;
    afw_value_string_t *result;
    afw_utf8_octet_t *c;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(value, 1, array);

    sep = afw_s_a_comma;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(separator, 2, string);
        sep = &separator->internal;
    }

    /* Create array of strings and get len of all stings and separators. */
    array_of_string = afw_array_convert_to_array_of_strings(value->internal,
        x->p, x->xctx);
    for (iterator = NULL, len = 0;;) {
        s = afw_array_of_string_get_next_internal(array_of_string, &iterator, x->xctx);
        if (!s) {
            break;
        }
        len += s->len + sep->len;
    }

    /* If len > 0 adjust len for extra separator, make result and return. */
    if (len > 0) {
        len -= sep->len;
        c = afw_pool_malloc(x->p, len, x->xctx);
        result = afw_value_allocate_unmanaged_string(x->p, x->xctx);
        result->internal.len = len;
        result->internal.s = c;
        for (iterator = NULL;;) {
            s = afw_array_of_string_get_next_internal(
                array_of_string, &iterator, x->xctx);
            if (!s) {
                break;
            }

            /* If not first time, add separator. */
            if (c != result->internal.s && sep->len > 0) {
                memcpy(c, sep->s, sep->len);
                c += sep->len;
            }

            /* Add this string. */
            if (s->len > 0) {
                memcpy(c, s->s, s->len);
                c += s->len;
            }
        }
        return &result->pub;
    }

    /* If len is zero, return empty string. */
    else {
        return afw_v_a_empty_string;
    }
}



/*
 * reverse and to_reversed: reverse a mutable array in place. Each entry
 * is held first (the read rule): setting an index releases the entry
 * there, which may be its last reference.
 */
static void
impl_reverse(
    const afw_array_t *array,
    afw_function_execute_t *x)
{
    const afw_value_t **values;
    const afw_iterator_old_t *iterator;
    afw_size_t count;
    afw_size_t i;

    count = afw_array_get_count(array, x->xctx);
    if (count < 2) {
        return;
    }
    values = afw_pool_malloc(x->p, sizeof(const afw_value_t *) * count,
        x->xctx);
    for (iterator = NULL, i = 0; i < count; i++) {
        values[i] = afw_value_hold_read(afw_array_get_next_value(
            array, &iterator, x->xctx), x->p, x->xctx);
    }
    for (i = 0; i < count; i++) {
        afw_array_set_value(array, (afw_integer_t)i,
            values[count - 1 - i], x->xctx);
    }
}



/*
 * Adaptive function: reverse
 *
 * afw_function_execute_reverse
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Reverse the order of the entries of array in place and return the same array,
 * as in ECMAScript. A frozen array is an error. Use to_reversed for a reversed
 * copy.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function reverse(
 *       array: array
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   array - (array) The array to reverse. Must not be frozen.
 *
 * Returns:
 *
 *   (array) The array, reversed.
 */
const afw_value_t *
afw_function_execute_reverse(
    afw_function_execute_t *x)
{
    const afw_value_array_t *array;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 1, array);
    /* Mutate-input: get_assignable; last-release of that hold on dest p. */
    array = (const afw_value_array_t *)
        afw_pool_scope_get_assignable_for_scope_lifetime(
            &array->pub, x->p, x->xctx);
    if (afw_array_is_immutable(array->internal, x->xctx)) {
        AFW_THROW_ERROR_Z(read_only,
            "reverse of a frozen array; to_reversed makes a reversed copy",
            x->xctx);
    }

    impl_reverse(array->internal, x);

    return &array->pub;
}



/*
 * A new array with the entries of from (shared, not cloned), as the
 * ECMAScript copying functions make: to_reversed, to_spliced, with_at.
 * Same declared data type as from. Caller does not release: last-release of the
 * create_managed hold on dest p.
 */
static const afw_array_t *
impl_shallow_copy(
    const afw_array_t *from,
    afw_function_execute_t *x)
{
    const afw_array_t *to;
    const afw_iterator_old_t *iterator;
    const afw_value_t *value;

    to = ((const afw_value_array_t *)
        afw_pool_scope_release_value_at_cleanup(
            afw_array_create_managed(
                afw_array_get_declared_data_type(from, x->xctx),
                x->p, x->xctx)->value,
            x->p, x->xctx))->internal;
    for (iterator = NULL;;) {
        value = afw_array_get_next_value(from, &iterator, x->xctx);
        if (!value) {
            break;
        }
        afw_array_push_value(to, value, x->xctx);
    }
    return to;
}



/*
 * splice and to_spliced: remove argv[3] (deleteCount) values at argv[2]
 * (startIndex) from array, pushing them to removed if not NULL, then
 * insert argv[4...] there.
 */
static void
impl_splice(
    afw_function_execute_t *x,
    const afw_array_t *array,
    const afw_array_t *removed)
{
    const afw_value_integer_t *integer;
    const afw_value_t *value;
    afw_integer_t start;
    afw_integer_t delete_count;
    afw_integer_t count;
    afw_integer_t i;
    afw_size_t arg;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(integer, 2, integer);

    count = (afw_integer_t)afw_array_get_count(array, x->xctx);
    start = integer->internal;
    if (start < 0) {
        start = count + start;
        if (start < 0) {
            start = 0;
        }
    }
    else if (start > count) {
        start = count;
    }

    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(integer, 3, integer);
        delete_count = integer->internal;
        if (delete_count < 0) {
            delete_count = 0;
        }
    }
    else {
        delete_count = count - start;
    }
    /* Not start + delete_count: that overflows for a huge count. */
    if (delete_count > count - start) {
        delete_count = count - start;
    }

    for (i = 0; i < delete_count; i++) {
        if (removed) {
            value = afw_array_get_entry_value(array, start, x->xctx);
            if (value) {
                afw_array_push_value(removed, value, x->xctx);
            }
        }
        /* Managed source last-releases the slot hold. Peek then
         * push then remove: removed holds, source drops. */
        afw_array_remove_value_by_index(array, start, x->xctx);
    }

    for (arg = 4; arg <= x->argc; arg++) {
        AFW_FUNCTION_EVALUATE_PARAMETER(value, arg);
        afw_array_insert_value(array,
            start + (afw_integer_t)(arg - 4), value, x->xctx);
    }
}



/* True if value is stored as UTF-8 (string, anyURI, ...). */
static afw_boolean_t
impl_is_utf8_value(const afw_value_t *value, afw_xctx_t *xctx)
{
    const afw_data_type_t *data_type;

    data_type = afw_value_get_data_type(value, xctx);
    return data_type &&
        afw_utf8_equal(&data_type->cType, afw_s_afw_utf8_t);
}


/* Octet offset of code point index (index past the end is the end). */
static afw_size_t
impl_code_point_offset(
    const afw_utf8_t *s,
    afw_integer_t index,
    afw_xctx_t *xctx)
{
    afw_size_t offset;
    afw_integer_t i;

    for (offset = 0, i = 0; i < index; i++) {
        if (afw_utf8_next_code_point(s->s, &offset, s->len, xctx) < 0) {
            break;
        }
    }
    return offset;
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
 * Adaptive function: slice
 *
 * afw_function_execute_slice
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * This function extracts a consecutive slice of values from an array, or of
 * code points from a string (as ECMAScript slice does for both). For a string,
 * or another value stored as UTF-8, the result is a string.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function slice(
 *       array: any,
 *       startIndex?: integer,
 *       endIndex?: integer
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   array - (any) The array, or the string, to slice.
 *
 *   startIndex - (optional integer) This is the zero based starting index. Use
 *       negative number to index from the end. If not specified, an index of 0
 *       is assumed.
 *
 *   endIndex - (optional integer) This is the zero based ending index. If
 *       positive, this is one more than the index of the last value or code
 *       point to include. If negative, the index is from the end. If not
 *       specified, the slice is from startIndex up to and including the end.
 *
 * Returns:
 *
 *   (any) For an array, a new array containing the selected values; it is an
 *       `array<T>` only if array is one, so values of another data type can be
 *       added to it, as in ECMAScript. For a string, a string containing the
 *       selected code points.
 */
const afw_value_t *
afw_function_execute_slice(
    afw_function_execute_t *x)
{
    const afw_value_t *source;
    const afw_value_array_t *array;
    const afw_value_array_t *result;
    const afw_value_integer_t *integer;
    const afw_iterator_old_t *iterator;
    const afw_value_t *value;
    const afw_data_type_t *data_type;
    const afw_utf8_t *s;
    afw_utf8_t slice;
    afw_size_t start_offset;
    afw_integer_t start;
    afw_integer_t end;
    afw_integer_t count;

    /*
     * An array, or a string (any value stored as UTF-8) sliced by code
     * point into a string, as ECMAScript slice does for both (#583).
     */
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(source, 1);
    s = NULL;
    array = NULL;
    if (afw_value_is_array(source)) {
        array = (const afw_value_array_t *)source;
        count = (afw_integer_t)afw_array_get_count(array->internal, x->xctx);
    }
    else if (impl_is_utf8_value(source, x->xctx)) {
        s = AFW_VALUE_INTERNAL(source);
        count = impl_code_point_count(s, x->xctx);
    }
    else {
        AFW_THROW_ERROR_FZ(argument_error, x->xctx,
            "Parameter 1 of function 'slice' must be an array or a string"
            " but evaluated to be '%ku'",
            &afw_value_get_data_type(source, x->xctx)->data_type_id);
    }

    /* Determine start index. */
    start = 0;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(integer, 2, integer);
        start = integer->internal;
        if (start < 0) {
            start = count + start;
            if (start < 0) {
                start = 0;
            }
        }
    }

    /* Determine end index. */
    end = count;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(integer, 3, integer);
        end = integer->internal;
        if (end < 0) {
            end = count + end;
        }
        else if (end > count) {
            end = count;
        }
    }

    /* String: the code points from start up to end. */
    if (s) {
        if (start >= end) {
            return afw_v_a_empty_string;
        }
        start_offset = impl_code_point_offset(s, start, x->xctx);
        slice.s = s->s + start_offset;
        slice.len = impl_code_point_offset(s, end, x->xctx) - start_offset;
        return afw_value_create_unmanaged_string(&slice, x->p, x->xctx);
    }

    /* New array: create_managed RC 1. Last-release of that hold on dest p. */
    data_type = afw_array_get_declared_data_type(array->internal, x->xctx);
    result = (const afw_value_array_t *)
        afw_pool_scope_release_value_at_cleanup(
            afw_array_create_managed(data_type, x->p, x->xctx)->value,
            x->p, x->xctx);
    for (iterator = NULL, count = 0; count < end; count++) {
        value = afw_array_get_next_value(array->internal, &iterator, x->xctx);
        if (!value) {
            AFW_THROW_ERROR_Z(general, "Expecting a value", x->xctx);
        }
        if (start <= count) {
            afw_array_push_value(result->internal, value, x->xctx);
        }
    }
    return &result->pub;
}



/*
 * Adaptive function: at
 *
 * afw_function_execute_at
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the value at a zero-based index in an array, or the code point at a
 * zero-based index in a string as a one-code-point string (as ECMAScript at
 * does for both). Negative indexes count from the end (-1 is the last). If the
 * index is out of range, the result is undefined. Bracket indexing a[index] and
 * s[index] use the same out-of-range result (undefined).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function at(
 *       array: any,
 *       index: integer
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   array - (any) Array, or string, to index.
 *
 *   index - (integer) Zero-based index, or negative from the end.
 *
 * Returns:
 *
 *   (any) The value at the index, or undefined if out of range.
 */
const afw_value_t *
afw_function_execute_at(
    afw_function_execute_t *x)
{
    const afw_value_t *source;
    const afw_value_integer_t *index;
    const afw_value_t *value;
    const afw_utf8_t *s;
    afw_utf8_t code_point;
    afw_integer_t i;
    afw_integer_t count;
    afw_size_t start_offset;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(source, 1);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(index, 2, integer);

    if (afw_value_is_array(source)) {
        value = afw_array_get_entry_value(
            ((const afw_value_array_t *)source)->internal,
            index->internal, x->xctx);
        return value ? value : afw_value_undefined;
    }

    /*
     * A string (any value stored as UTF-8): the code point at index as a
     * one-code-point string, as ECMAScript at does and s[i] (#583).
     */
    if (!impl_is_utf8_value(source, x->xctx)) {
        AFW_THROW_ERROR_FZ(argument_error, x->xctx,
            "Parameter 1 of function 'at' must be an array or a string"
            " but evaluated to be '%ku'",
            &afw_value_get_data_type(source, x->xctx)->data_type_id);
    }
    s = AFW_VALUE_INTERNAL(source);
    count = impl_code_point_count(s, x->xctx);
    i = index->internal;
    if (i < 0) {
        i = count + i;
    }
    if (i < 0 || i >= count) {
        return afw_value_undefined;
    }
    start_offset = impl_code_point_offset(s, i, x->xctx);
    code_point.s = s->s + start_offset;
    code_point.len = impl_code_point_offset(s, i + 1, x->xctx) - start_offset;
    return afw_value_create_unmanaged_string(&code_point, x->p, x->xctx);
}



/*
 * Adaptive function: pop
 *
 * afw_function_execute_pop
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Remove the last value from a mutable array and return it. If the array is
 * empty, returns undefined.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function pop(
 *       array: array
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   array - (array) Target array. Must not be immutable.
 *
 * Returns:
 *
 *   (any) The removed value, or undefined if the array was empty.
 */
const afw_value_t *
afw_function_execute_pop(
    afw_function_execute_t *x)
{
    const afw_value_array_t *array;
    const afw_value_t *value;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 1, array);
    /* Mutate-input: get_assignable; last-release of that hold on dest p. */
    array = (const afw_value_array_t *)
        afw_pool_scope_get_assignable_for_scope_lifetime(
            &array->pub, x->p, x->xctx);

    value = afw_array_pop_value(array->internal, NULL, x->p, x->xctx);
    if (!value) {
        return afw_value_undefined;
    }
    /* Transfer last-released dest p inside pop_value. */
    return value;
}



/*
 * Adaptive function: push
 *
 * afw_function_execute_push
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Append one or more values to the end of a mutable array. Returns the same
 * array after modification. The array must not be immutable (for example after
 * freeze).
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function push(
 *       array: array,
 *       ...values: any[]
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   array - (array) Target array. Must not be immutable.
 *
 *   values - (0 or more any) Values to append in order.
 *
 * Returns:
 *
 *   (array) The modified array.
 */
const afw_value_t *
afw_function_execute_push(
    afw_function_execute_t *x)
{
    const afw_value_array_t *array;
    const afw_value_t *value;
    afw_size_t i;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 1, array);
    /* Mutate-input: get_assignable; last-release of that hold on dest p. */
    array = (const afw_value_array_t *)
        afw_pool_scope_get_assignable_for_scope_lifetime(
            &array->pub, x->p, x->xctx);

    for (i = 2; i <= x->argc; i++) {
        AFW_FUNCTION_EVALUATE_PARAMETER(value, i);
        afw_array_push_value(array->internal, value, x->xctx);
    }

    return &array->pub;
}



/*
 * Adaptive function: shift
 *
 * afw_function_execute_shift
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Remove the first value from a mutable array and return it. If the array is
 * empty, returns undefined.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function shift(
 *       array: array
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   array - (array) Target array. Must not be immutable.
 *
 * Returns:
 *
 *   (any) The removed value, or undefined if the array was empty.
 */
const afw_value_t *
afw_function_execute_shift(
    afw_function_execute_t *x)
{
    const afw_value_array_t *array;
    const afw_value_t *value;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 1, array);
    /* Mutate-input: get_assignable; last-release of that hold on dest p. */
    array = (const afw_value_array_t *)
        afw_pool_scope_get_assignable_for_scope_lifetime(
            &array->pub, x->p, x->xctx);

    value = afw_array_shift_value(array->internal, NULL, x->p, x->xctx);
    if (!value) {
        return afw_value_undefined;
    }
    /* Transfer last-released dest p inside shift_value. */
    return value;
}



/*
 * Adaptive function: splice
 *
 * afw_function_execute_splice
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Remove zero or more values starting at an index from a mutable array and
 * optionally insert new values at that index. Returns an array of the removed
 * values. Negative startIndex counts from the end. If deleteCount is omitted,
 * all values from startIndex to the end are removed.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function splice(
 *       array: array,
 *       startIndex: integer,
 *       deleteCount?: integer,
 *       ...values: any[]
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   array - (array) Target array. Must not be immutable.
 *
 *   startIndex - (integer) Zero-based start index, or negative from the end.
 *
 *   deleteCount - (optional integer) Number of values to remove. If omitted,
 *       remove through the end of the array. Negative is treated as zero.
 *
 *   values - (0 or more any) Values to insert at startIndex after removals.
 *
 * Returns:
 *
 *   (array) Array of removed values, in original order.
 */
const afw_value_t *
afw_function_execute_splice(
    afw_function_execute_t *x)
{
    const afw_value_array_t *array;
    const afw_array_t *removed;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 1, array);
    /* Mutate-input: get_assignable; last-release of that hold on dest p. */
    array = (const afw_value_array_t *)
        afw_pool_scope_get_assignable_for_scope_lifetime(
            &array->pub, x->p, x->xctx);

    /*
     * New array: create_managed RC 1. Last-release of that hold on
     * dest p. Do not get_assignable. Same as slice.
     */
    removed = ((const afw_value_array_t *)
        afw_pool_scope_release_value_at_cleanup(
            afw_array_create_managed(NULL, x->p, x->xctx)->value,
            x->p, x->xctx))->internal;
    impl_splice(x, array->internal, removed);

    return removed->value;
}



/*
 * Adaptive function: unshift
 *
 * afw_function_execute_unshift
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Insert one or more values at the beginning of a mutable array, preserving the
 * relative order of the inserted values. Returns the modified array.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function unshift(
 *       array: array,
 *       ...values: any[]
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   array - (array) Target array. Must not be immutable.
 *
 *   values - (0 or more any) Values to insert at the front, in order.
 *
 * Returns:
 *
 *   (array) The modified array.
 */
const afw_value_t *
afw_function_execute_unshift(
    afw_function_execute_t *x)
{
    const afw_value_array_t *array;
    const afw_value_t *value;
    afw_size_t i;
    afw_integer_t insert_at;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 1, array);
    /* Mutate-input: get_assignable; last-release of that hold on dest p. */
    array = (const afw_value_array_t *)
        afw_pool_scope_get_assignable_for_scope_lifetime(
            &array->pub, x->p, x->xctx);

    /* Insert in order at 0, 1, 2, ... so relative order is preserved. */
    insert_at = 0;
    for (i = 2; i <= x->argc; i++) {
        AFW_FUNCTION_EVALUATE_PARAMETER(value, i);
        afw_array_insert_value(array->internal, insert_at, value, x->xctx);
        insert_at++;
    }

    return &array->pub;
}



/*
 * Adaptive function: to_reversed
 *
 * afw_function_execute_to_reversed
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return a new array with the entries of array in reverse order, leaving array
 * unchanged, as ECMAScript toReversed does. The new array is an `array<T>` only
 * if array is one.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function to_reversed(
 *       array: array
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   array - (array) The array to copy reversed.
 *
 * Returns:
 *
 *   (array) A new array with the entries of array reversed.
 */
const afw_value_t *
afw_function_execute_to_reversed(
    afw_function_execute_t *x)
{
    const afw_value_array_t *array;
    const afw_array_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 1, array);
    result = impl_shallow_copy(array->internal, x);
    impl_reverse(result, x);

    return result->value;
}



/*
 * Adaptive function: to_spliced
 *
 * afw_function_execute_to_spliced
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return a new array that is array with the changes splice would make, leaving
 * array unchanged, as ECMAScript toSpliced does: deleteCount values removed at
 * startIndex and values inserted there. Negative startIndex counts from the
 * end. If deleteCount is omitted, all values from startIndex to the end are
 * removed.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function to_spliced(
 *       array: array,
 *       startIndex: integer,
 *       deleteCount?: integer,
 *       ...values: any[]
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   array - (array) The array to copy.
 *
 *   startIndex - (integer) Zero-based start index, or negative from the end.
 *
 *   deleteCount - (optional integer) Number of values to remove. If omitted,
 *       remove through the end of the array. Negative is treated as zero.
 *
 *   values - (0 or more any) Values to insert at startIndex after removals.
 *
 * Returns:
 *
 *   (array) A new array with the changes made.
 */
const afw_value_t *
afw_function_execute_to_spliced(
    afw_function_execute_t *x)
{
    const afw_value_array_t *array;
    const afw_array_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 1, array);
    result = impl_shallow_copy(array->internal, x);
    impl_splice(x, result, NULL);

    return result->value;
}



/*
 * Adaptive function: with_at
 *
 * afw_function_execute_with_at
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return a new array that is array with the value at index replaced by value,
 * leaving array unchanged, as ECMAScript with does (with is a reserved word).
 * Negative index counts from the end, as in at. An index out of range is an
 * error.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function with_at(
 *       array: array,
 *       index: integer,
 *       value: any
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   array - (array) The array to copy.
 *
 *   index - (integer) Zero-based index, or negative from the end.
 *
 *   value - (any) The value to put at index.
 *
 * Returns:
 *
 *   (array) A new array with the value at index replaced.
 */
const afw_value_t *
afw_function_execute_with_at(
    afw_function_execute_t *x)
{
    const afw_value_array_t *array;
    const afw_value_integer_t *index;
    const afw_value_t *value;
    const afw_array_t *result;
    afw_integer_t count;
    afw_integer_t i;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 1, array);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(index, 2, integer);
    AFW_FUNCTION_EVALUATE_PARAMETER(value, 3);
    if (!value) {
        value = afw_value_undefined;
    }

    /* As in at, but out of range is an error (a RangeError in ECMAScript). */
    count = (afw_integer_t)afw_array_get_count(array->internal, x->xctx);
    i = index->internal;
    if (i < 0) {
        i = count + i;
    }
    if (i < 0 || i >= count) {
        AFW_THROW_ERROR_FZ(argument_error, x->xctx,
            "with_at index " AFW_INTEGER_FMT
            " is out of range for an array of length " AFW_INTEGER_FMT,
            index->internal, count);
    }

    result = impl_shallow_copy(array->internal, x);
    afw_array_set_value(result, i, value, x->xctx);

    return result->value;
}
