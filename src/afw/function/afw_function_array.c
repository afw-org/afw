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

    for (count = 2; count <= x->argc; count++) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(source, count, array);
        for (iterator = NULL;;) {
            value = afw_array_get_next_value(
                source->internal, &iterator, x->p, x->xctx);
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
const afw_value_t *
afw_function_execute_array(
    afw_function_execute_t *x)
{
    afw_size_t n;
    const afw_array_t *array;
    const afw_array_t *l;
    const afw_value_t * const * arg;
    const afw_value_t *entry;
    const afw_value_t *value;
    const afw_iterator_old_t *iterator;

    /* Construct a new array with elements passed as arguments. */
    array = afw_array_create_generic(x->p, x->xctx);
    for (n = 1, arg = &x->argv[1]; n <= x->argc; n++, arg++) {
        afw_xctx_evaluation_stack_push_parameter_number(n, x->xctx);

        /* If array expression, add each element of array. */
        if (afw_value_is_array_expression(*arg)) {
            value = afw_value_evaluate(*arg, x->p, x->xctx);
            if (value) {
                l = afw_value_as_array(value, x->xctx);
                for (iterator = NULL;;) {
                    entry = afw_array_get_next_value(l, &iterator,
                        x->p, x->xctx);
                    if (!entry) {
                        break;
                    }
                    entry = afw_value_evaluate(entry, x->p, x->xctx);
                    afw_array_push_value(array, entry, x->xctx);
                }
            }
        }

        /* If not an array expression, add evaluated argument as element. */
        else {
            entry = afw_value_evaluate(*arg, x->p, x->xctx);
            entry = afw_value_closure_binding_create_if_needed(
                entry, x->xctx);
            afw_array_push_value(array, entry, x->xctx);
        }

        afw_xctx_evaluation_stack_pop_parameter_number(x->xctx);
    }

    /* Return constructed array. */
    return afw_value_create_unmanaged_array(array, x->p, x->xctx);
}



/*
 * @brief Adaptive Function includes<array>
 * @param function struct for this function.
 * @param argc number of values in argv.
 * @param argv array of values.
 * @param p to use for result.
 * @param xctx of caller.
 *
 * Checks whether or not an array contains any value.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 *   function includes<array>(
 *       array: array,
 *       searchElement: any,
 *       fromIndex?: integer
 *   ): boolean;
 *
 * Parameters:
 *
 *   array - (<Type>) The array to search.
 *
 *   searchElement - (any dataType) Element to find.
 *
 *   fromIndex - (optional integer) Index in the array to start search.
 *
 * Returns:
 *
 *   (boolean) Indicates if the element is found in array.
 *
 */
const afw_value_t *
afw_function_execute_includes_array(
    afw_function_execute_t *x)
{
    const afw_value_array_t *array;
    const afw_value_t *searchElement;
    const afw_value_integer_t *fromIndex;
    const afw_value_t *item;
    afw_xctx_t *xctx;
    afw_size_t index;
    afw_size_t len;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 1, array);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(searchElement, 2);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(fromIndex, 3, integer);
    
    xctx = x->xctx;
    index = 0;
    len = afw_array_get_count(array->internal, xctx);

    if (fromIndex) {
        /* Empty array: fromIndex % 0 is undefined (SIGFPE on some hosts). */
        if (len == 0) {
            return afw_boolean_v_false;
        }
        index = abs((int)fromIndex->internal) % len;

        if (fromIndex->internal < 0 && index != 0) {
            index = len - index;
        }
    }

    for ( ; index < len; index++) {
        item = afw_array_get_entry_value(array->internal, index, x->p, xctx);

        if (afw_value_equal(item, searchElement, xctx)) {
            return afw_boolean_v_true;
        }
    }

    return afw_boolean_v_false;
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

    array = afw_array_create_generic(x->p, x->xctx);
    for (i = 0; i < n; i++) {
        afw_array_push_value(array, afw_value_undefined, x->xctx);
    }

    /* Dual face on the instance; no extra heap header. */
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
        s = afw_array_of_string_get_next(array_of_string, &iterator, x->xctx);
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
            s = afw_array_of_string_get_next(
                array_of_string, &iterator, x->xctx);
            if (!s) {
                break;
            }

            /* If not first time, add separator. */
            if (c != result->internal.s) {
                memcpy(c, sep->s, sep->len);
                c += sep->len;
            }

            /* Add this string. */
            memcpy(c, s->s, s->len);
            c += s->len;
        }
        return &result->pub;
    }

    /* If len is zero, return empty string. */
    else {
        return afw_v_a_empty_string;
    }
}



/*
 * Adaptive function: reverse
 *
 * afw_function_execute_reverse
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Reverse the order of the elements in an array. If the array is typed, the
 * resulting array will be the same type.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
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
 *   array - (array) An array to reverse.
 *
 * Returns:
 *
 *   (array) An array with elements reversed.
 */
const afw_value_t *
afw_function_execute_reverse(
    afw_function_execute_t *x)
{
    const afw_value_array_t *array;
    const afw_array_t *result_array;
    const afw_iterator_old_t *iterator;
    const afw_value_t *value;
    const afw_array_setter_t *setter;
    const afw_data_type_t *data_type;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 1, array);

    data_type = afw_array_get_data_type(array->internal, x->xctx);
    result_array = afw_array_create_with_options(0, data_type, x->p, x->xctx);
    setter = afw_array_get_setter(result_array, x->xctx);
    for (iterator = NULL;;) {
        value = afw_array_get_next_value(array->internal, &iterator,
            x->p, x->xctx);
        if (!value) {
            break;
        }
        afw_array_setter_insert_value(setter, 0, value, x->xctx);
    }
    return afw_value_create_unmanaged_array(result_array, x->p, x->xctx);
}



/*
 * Adaptive function: slice
 *
 * afw_function_execute_slice
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * This function extracts a consecutive slice of values from an array.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function slice(
 *       array: array,
 *       startIndex?: integer,
 *       endIndex?: integer
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   array - (array) The array to slice.
 *
 *   startIndex - (optional integer) This is the zero based starting index. Use
 *       negative number to index from the end of the array. If not specified,
 *       an index of 0 is assumed.
 *
 *   endIndex - (optional integer) This is the zero based ending index. If
 *       positive, this is one more than the index of the last value to include
 *       in the array. If negative, the index is from the end of the array. If
 *       not specified, the slice is from startIndex up to and including the end
 *       of the array.
 *
 * Returns:
 *
 *   (array) An array containing the selected values. If all of the values are
 *       the same data type, the array will be an array of that data type.
 */
const afw_value_t *
afw_function_execute_slice(
    afw_function_execute_t *x)
{
    const afw_value_array_t *array;
    const afw_array_t *result_array;
    const afw_value_integer_t *integer;
    const afw_iterator_old_t *iterator;
    const afw_value_t *value;
    afw_integer_t start;
    afw_integer_t end;
    afw_integer_t count;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 1, array);
    count = (afw_integer_t)afw_array_get_count(array->internal, x->xctx);

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

    /* Create and return an array with slice. */
    result_array = afw_array_create_generic(x->p, x->xctx);
    for (iterator = NULL, count = 0; count < end; count++) {
        value = afw_array_get_next_value(array->internal, &iterator,
            x->p, x->xctx);
        if (!value) {
            AFW_THROW_ERROR_Z(general, "Expecting a value", x->xctx);
        }
        if (start <= count) {
            afw_array_push_value(result_array, value, x->xctx);
        }
    }
    afw_array_determine_data_type_and_set_immutable(result_array, x->xctx);
    return afw_value_create_unmanaged_array(result_array, x->p, x->xctx);
}



/*
 * Adaptive function: at
 *
 * afw_function_execute_at
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the value at a zero-based index in an array. Negative indexes count
 * from the end (-1 is the last element). If the index is out of range, the
 * result is undefined. Bracket indexing a[index] uses the same out-of-range
 * result (undefined).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function at(
 *       array: array,
 *       index: integer
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   array - (array) Array to index.
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
    const afw_value_array_t *array;
    const afw_value_integer_t *index;
    const afw_value_t *value;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 1, array);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(index, 2, integer);

    value = afw_array_get_entry_value(array->internal, index->internal,
        x->p, x->xctx);
    return value ? value : afw_value_undefined;
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

    value = afw_array_pop_value(array->internal, NULL, x->xctx);
    return value ? value : afw_value_undefined;
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

    value = afw_array_shift_value(array->internal, NULL, x->xctx);
    return value ? value : afw_value_undefined;
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
    const afw_value_integer_t *integer;
    const afw_array_t *removed;
    const afw_value_t *value;
    afw_integer_t start;
    afw_integer_t delete_count;
    afw_integer_t count;
    afw_integer_t i;
    afw_size_t arg;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 1, array);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(integer, 2, integer);

    count = (afw_integer_t)afw_array_get_count(array->internal, x->xctx);
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
    if (start + delete_count > count) {
        delete_count = count - start;
    }

    removed = afw_array_create_generic(x->p, x->xctx);
    for (i = 0; i < delete_count; i++) {
        value = afw_array_get_entry_value(array->internal, start,
            x->p, x->xctx);
        if (value) {
            afw_array_push_value(removed, value, x->xctx);
        }
        afw_array_remove_value_by_index(array->internal, start, x->xctx);
    }

    for (arg = 4; arg <= x->argc; arg++) {
        AFW_FUNCTION_EVALUATE_PARAMETER(value, arg);
        afw_array_insert_value(array->internal,
            start + (afw_integer_t)(arg - 4), value, x->xctx);
    }

    return afw_value_create_unmanaged_array(removed, x->p, x->xctx);
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

    /* Insert in order at 0, 1, 2, ... so relative order is preserved. */
    insert_at = 0;
    for (i = 2; i <= x->argc; i++) {
        AFW_FUNCTION_EVALUATE_PARAMETER(value, i);
        afw_array_insert_value(array->internal, insert_at, value, x->xctx);
        insert_at++;
    }

    return &array->pub;
}
