// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for List
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_list.c
 * @brief afw_function_execute_* functions for list.
 */

#include "afw_internal.h"



/*
 * Adaptive function: add_entries
 *
 * afw_function_execute_add_entries
 *
 * See afw_function_bindings.h for more information.
 *
 * Add the entries of one or more lists to another.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function add_entries(
 *       target: list,
 *       source_1: list,
 *       ...source_rest: (list of list)
 *   ): list;
 * ```
 *
 * Parameters:
 *
 *   target - (list) Target list. This list must not be immutable.
 *
 *   source - (1 or more list) Source list(s).
 *
 * Returns:
 *
 *   (list) The modified target list.
 */
const afw_value_t *
afw_function_execute_add_entries(
    afw_function_execute_t *x)
{
    const afw_value_list_t *target;
    const afw_value_list_t *source;
    const afw_iterator_t *iterator;
    const afw_value_t *value;
    afw_size_t count;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(target, 1, list);

    for (count = 2; count <= x->argc; count++) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(source, count, list);
        for (iterator = NULL;;) {
            value = afw_list_get_next_value(
                source->internal, &iterator, x->p, x->xctx);
            if (!value) {
                break;
            }            
            afw_list_add_value(target->internal, value, x->xctx);
        }
    }

    return (const afw_value_t *)target;
}



/*
 * Adaptive function: list
 *
 * afw_function_execute_list
 *
 * See afw_function_bindings.h for more information.
 *
 * Converts 1 or more values in a list. If A value is a list, its individual
 * values are included.
 * 
 * If the data types of all values in the resulting list are the same, the list
 * will be a list of that data type. Otherwise, the resulting list will be
 * untyped.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function list(
 *       ...values: (list of any)
 *   ): list;
 * ```
 *
 * Parameters:
 *
 *   values - (0 or more any dataType) Value to convert.
 *
 * Returns:
 *
 *   (list) Converted value.
 *
 * Errors thrown:
 *
 *   cast_error - value could not be converted
 */
const afw_value_t *
afw_function_execute_list(
    afw_function_execute_t *x)
{
    afw_size_t n;
    const afw_list_t *list;
    const afw_list_t *l;
    const afw_value_t * const * arg;
    const afw_value_t *entry;
    const afw_value_t *value;
    const afw_iterator_t *iterator;

    list = afw_list_create_generic(x->p, x->xctx);
    for (n = 1, arg = &x->argv[1]; n <= x->argc; n++, arg++) {
        if (afw_value_is_list_expression(*arg)) {
            value = afw_value_evaluate(*arg, x->p, x->xctx);
            if (value) {
                l = afw_value_as_list(value, x->xctx);
                for (iterator = NULL;;) {
                    entry = afw_list_get_next_value(l, &iterator,
                        x->p, x->xctx);
                    if (!entry) {
                        break;
                    }
                    afw_list_add_value(list, entry, x->xctx);
                }
            }
        }
        else {
            entry = afw_value_evaluate(*arg, x->p, x->xctx);
            afw_list_add_value(list, entry, x->xctx);
        }
    }

    return afw_value_create_list(list, x->p, x->xctx);
}



/*
 * @brief Adaptive Function includes<list>
 * @param function struct for this function.
 * @param argc number of values in argv.
 * @param argv list of values.
 * @param p to use for result.
 * @param xctx of caller.
 *
 * Checks whether or not a list contains any value.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 *   function includes<list>(
 *       list: list,
 *       searchElement: any,
 *       fromIndex?: integer
 *   ): boolean;
 *
 * Parameters:
 *
 *   list - (<Type>) The list to search.
 *
 *   searchElement - (any dataType) Element to find.
 *
 *   fromIndex - (optional integer) Index in the list to start search.
 *
 * Returns:
 *
 *   (boolean) Indicates if the element is found in list.
 *
 */
const afw_value_t *
afw_function_execute_includes_list(
    afw_function_execute_t *x)
{
    const afw_value_list_t *list;
    const afw_value_t *searchElement;
    const afw_value_integer_t *fromIndex;
    const afw_value_t *item;
    afw_xctx_t *xctx;
    afw_size_t index;
    afw_size_t len;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(list, 1, list);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(searchElement, 2);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(fromIndex, 3, integer);
    
    xctx = x->xctx;
    index = 0;
    len = afw_list_get_count(list->internal, xctx);

    if (fromIndex) {
        index = abs((int)fromIndex->internal) % len;

        if (fromIndex->internal < 0 && index != 0) {
            index = len - index;
        }
    }

    for ( ; index < len; index++) {
        item = afw_list_get_entry_value(list->internal, index, x->p, xctx);

        if (afw_value_equal(item, searchElement, xctx)) {
            return afw_value_true;
        }
    }

    return afw_value_false;
}

/*
 * Adaptive function: join
 *
 * afw_function_execute_join
 *
 * See afw_function_bindings.h for more information.
 *
 * Concatenate the string values of the elements of a list with a separator.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function join(
 *       value: list,
 *       separator?: string
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   value - (list) A list of values of any data type.
 *
 *   separator - (optional string) The separator to use. If not specified, a
 *       comma (,) is used.
 *
 * Returns:
 *
 *   (string) Joined list values.
 */
const afw_value_t *
afw_function_execute_join(
    afw_function_execute_t *x)
{
    const afw_value_list_t *value;
    const afw_value_string_t *separator;
    const afw_utf8_t *sep;
    const afw_iterator_t *iterator;
    const afw_list_t *list_of_string;
    const afw_utf8_t *s;
    afw_size_t len;
    afw_value_string_t *result;
    afw_utf8_octet_t *c;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(value, 1, list);

    sep = &afw_s_a_comma;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(separator, 2, string);
        sep = &separator->internal;
    }

    /* Create list of strings and get len of all stings and separators. */
    list_of_string = afw_list_convert_to_list_of_strings(value->internal,
        x->p, x->xctx);
    for (iterator = NULL, len = 0;;) {
        s = afw_list_of_string_get_next(list_of_string, &iterator, x->xctx);
        if (!s) {
            break;
        }
        len += s->len + sep->len;
    }

    /* If len > 0 adjust len for extra separator, make result and return. */
    if (len > 0) {
        len -= sep->len;
        c = afw_pool_malloc(x->p, len, x->xctx);
        result = afw_value_allocate_string(x->p, x->xctx);
        result->internal.len = len;
        result->internal.s = c;
        for (iterator = NULL, len = 0;;) {
            s = afw_list_of_string_get_next(
                list_of_string, &iterator, x->xctx);
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
        return (const afw_value_t *)result;
    }

    /* If len is zero, return empty string. */
    else {
        return afw_value_empty_string;
    }
}



/*
 * Adaptive function: reverse
 *
 * afw_function_execute_reverse
 *
 * See afw_function_bindings.h for more information.
 *
 * Reverse the order of the elements in a list. If the list is typed, the
 * resulting list will be the same type.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function reverse(
 *       list: list
 *   ): list;
 * ```
 *
 * Parameters:
 *
 *   list - (list) A list to reverse.
 *
 * Returns:
 *
 *   (list) A list with elements reversed.
 */
const afw_value_t *
afw_function_execute_reverse(
    afw_function_execute_t *x)
{
    const afw_value_list_t *list;
    const afw_list_t *result_list;
    const afw_iterator_t *iterator;
    const afw_value_t *value;
    const afw_list_setter_t *setter;
    const afw_data_type_t *data_type;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(list, 1, list);

    data_type = afw_list_get_data_type(list->internal, x->xctx);
    result_list = afw_list_create_with_options(0, data_type, x->p, x->xctx);
    setter = afw_list_get_setter(result_list, x->xctx);
    for (iterator = NULL;;) {
        value = afw_list_get_next_value(list->internal, &iterator,
            x->p, x->xctx);
        if (!value) break;
        afw_list_setter_insert_value(setter, value, 0, x->xctx);
    }
    return afw_value_create_list(result_list, x->p, x->xctx);
}



/*
 * Adaptive function: slice
 *
 * afw_function_execute_slice
 *
 * See afw_function_bindings.h for more information.
 *
 * This function extracts a consecutive slice of values from a list.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function slice(
 *       list: list,
 *       startIndex?: integer,
 *       endIndex?: integer
 *   ): list;
 * ```
 *
 * Parameters:
 *
 *   list - (list) The list to slice.
 *
 *   startIndex - (optional integer) This is the zero based starting index. Use
 *       negative number to index from the end of the list. If not specified,
 *       an index of 0 is assumed.
 *
 *   endIndex - (optional integer) This is the zero based ending index. If
 *       positive, this is one more than the index of the last value to include
 *       in the list. If negative, the index is from the end of the list. If
 *       not specified, the slice is from startIndex up to and including the
 *       end of the list.
 *
 * Returns:
 *
 *   (list) A list containing the selected values. If all of the values are the
 *       same data type, the list will be a list of that data type.
 */
const afw_value_t *
afw_function_execute_slice(
    afw_function_execute_t *x)
{
    const afw_value_list_t *list;
    const afw_list_t *result_list;
    const afw_value_integer_t *integer;
    const afw_iterator_t *iterator;
    const afw_value_t *value;
    afw_integer_t start;
    afw_integer_t end;
    afw_integer_t count;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(list, 1, list);
    count = (afw_integer_t)afw_list_get_count(list->internal, x->xctx);

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

    /* Create and return a list with slice. */
    result_list = afw_list_create_generic(x->p, x->xctx);
    for (iterator = NULL, count = 0; count < end; count++) {
        value = afw_list_get_next_value(list->internal, &iterator,
            x->p, x->xctx);
        if (!value) {
            AFW_THROW_ERROR_Z(general, "Expecting a value", x->xctx);
        }
        if (start <= count) {
            afw_list_add_value(result_list, value, x->xctx);
        }
    }
    afw_list_determine_data_type_and_set_immutable(result_list, x->xctx);
    return afw_value_create_list(result_list, x->p, x->xctx);
}
