// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for higher_order_array
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_higher_order_array.c
 * @brief afw_function_execute_* functions for higher_order_array.
 */

#include "afw.h"



/*
 * Adaptive function: all_of
 *
 * afw_function_execute_all_of
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return true if predicate returns true for every entry of the first array in
 * values (index order), or if that array is empty. Entries whose value is
 * undefined are included. every() is an alias for the common single-array form.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function all_of(
 *       predicate: (...values: any) => boolean,
 *       values_1: any,
 *       ...values_rest: any[]
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   predicate - ((...values: any) => boolean) This function is called for each
 *       value in the first array in values or until false is returned. If no
 *       calls return false, the result is true.
 *
 *   values - (1 or more any) These are the parameters passed to predicate with
 *       the exception that the first array is passed one value at a time. At
 *       least one array is required.
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_all_of(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: all_of_all
 *
 * afw_function_execute_all_of_all
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Returns true if the result of calling predicate with all of the combination
 * of values from array1 and array2 returns true.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function all_of_all(
 *       predicate: (value1: any, value2: any) => boolean,
 *       array1: array,
 *       array2: array
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   predicate - ((value1: any, value2: any) => boolean) The predicate is passed
 *       two parameters, the first is a value from array1 and the second is a
 *       value from array2.
 *
 *   array1 - (array)
 *
 *   array2 - (array)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_all_of_all(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: all_of_any
 *
 * afw_function_execute_all_of_any
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * This function returns true if the result of calling predicate with all of the
 * combination of values from array1 and any of the values of array2 returns
 * true.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function all_of_any(
 *       predicate: (value1: any, value2: any) => boolean,
 *       array1: array,
 *       array2: array
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   predicate - ((value1: any, value2: any) => boolean) The predicate is passed
 *       two parameters, the first is a value from array1 and the second is a
 *       value from array2.
 *
 *   array1 - (array)
 *
 *   array2 - (array)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_all_of_any(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: any_of
 *
 * afw_function_execute_any_of
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return true if predicate returns true for any entry of the first array in
 * values (index order). Entries whose value is undefined are included. Empty
 * array yields false. some() is an alias for the common single-array form.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function any_of(
 *       predicate: (...values: any) => boolean,
 *       values_1: any,
 *       ...values_rest: any[]
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   predicate - ((...values: any) => boolean) This function is called for each
 *       value in the first array in values or until true is returned. If no
 *       calls return true, the result is false.
 *
 *   values - (1 or more any) These are the parameters passed to predicate with
 *       the exception that the first array is passed one value at a time. At
 *       least one array is required.
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_any_of(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: any_of_all
 *
 * afw_function_execute_any_of_all
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Returns true if, for at least one value of array1, the predicate returns true
 * with every value of array2 (the predicate gets the value from array1 first).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function any_of_all(
 *       predicate: (value1: any, value2: any) => boolean,
 *       array1: array,
 *       array2: array
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   predicate - ((value1: any, value2: any) => boolean) The predicate is passed
 *       two parameters, the first is a value from array1 and the second is a
 *       value from array2.
 *
 *   array1 - (array)
 *
 *   array2 - (array)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_any_of_all(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: any_of_any
 *
 * afw_function_execute_any_of_any
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * This function returns true if the result of calling predicate with any of the
 * combination of values from array1 and array2 returns true.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function any_of_any(
 *       predicate: (value1: any, value2: any) => boolean,
 *       array1: array,
 *       array2: array
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   predicate - ((value1: any, value2: any) => boolean) The predicate is passed
 *       two parameters, the first is a value from array1 and the second is a
 *       value from array2.
 *
 *   array1 - (array)
 *
 *   array2 - (array)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_any_of_any(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: every
 *
 * afw_function_execute_every
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return true if predicate returns true for every entry of array (index order),
 * or if array is empty. Entries whose value is undefined are included.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function every(
 *       array: array,
 *       predicate: (value: any, index?: integer, array?: array) => boolean
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   array - (array) The array to walk. A string is not walked as an array:
 *       write [...value] to walk its characters.
 *
 *   predicate - ((value: any, index?: integer, array?: array) => boolean)
 *       Called for each entry of array until it returns false. A script
 *       function is called with (value, index, array); a built-in function is
 *       called with the value alone, since its other parameters have their own
 *       meanings.
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_every(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: filter
 *
 * afw_function_execute_filter
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return a new array of the entries of array for which predicate returns true.
 * Every index is considered, including entries whose value is undefined. Order
 * of kept entries is preserved.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function filter(
 *       array: array,
 *       predicate: (value: any, index?: integer, array?: array) => boolean
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   array - (array) The array to walk. A string is not walked as an array:
 *       write [...value] to walk its characters.
 *
 *   predicate - ((value: any, index?: integer, array?: array) => boolean)
 *       Returns true to keep the entry. A script function is called with
 *       (value, index, array); a built-in function is called with the value
 *       alone, since its other parameters have their own meanings.
 *
 * Returns:
 *
 *   (array) A new array of the entries that passed the test (possibly empty).
 */
const afw_value_t *
afw_function_execute_filter(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: find
 *
 * afw_function_execute_find
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Call predicate for each entry of array, in index order, until it returns
 * true, then return that entry. Entries whose value is undefined are included.
 * If no entry passes, the result is undefined (the same as a found undefined
 * entry; use find_index if you need to tell those apart).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function find(
 *       array: array,
 *       predicate: (value: any, index?: integer, array?: array) => boolean
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   array - (array) The array to walk. A string is not walked as an array:
 *       write [...value] to walk its characters.
 *
 *   predicate - ((value: any, index?: integer, array?: array) => boolean)
 *       Returns true for the entry to return. A script function is called with
 *       (value, index, array); a built-in function is called with the value
 *       alone, since its other parameters have their own meanings.
 *
 * Returns:
 *
 *   (any) The first matching entry, or undefined if none match.
 */
const afw_value_t *
afw_function_execute_find(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: find_index
 *
 * afw_function_execute_find_index
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Call predicate for each entry of array, in index order, until it returns
 * true, then return that entry's index. Entries whose value is undefined are
 * included. If no entry passes, the result is -1.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function find_index(
 *       array: array,
 *       predicate: (value: any, index?: integer, array?: array) => boolean
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   array - (array) The array to walk. A string is not walked as an array:
 *       write [...value] to walk its characters.
 *
 *   predicate - ((value: any, index?: integer, array?: array) => boolean)
 *       Returns true for the entry whose index to return. A script function is
 *       called with (value, index, array); a built-in function is called with
 *       the value alone, since its other parameters have their own meanings.
 *
 * Returns:
 *
 *   (integer) The index of the first matching entry, or -1 if none match.
 */
const afw_value_t *
afw_function_execute_find_index(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: map
 *
 * afw_function_execute_map
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Call callbackfn once for each entry of array, in index order from 0 through
 * length minus one, and return a new array of the same length with the results.
 * Entries whose value is undefined (including omitted elements in array
 * literals) are included; callbackfn receives undefined for those indexes.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function map(
 *       array: array,
 *       callbackfn: (value: any, index?: integer, array?: array) => any
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   array - (array) The array to walk. A string is not walked as an array:
 *       write [...value] to walk its characters.
 *
 *   callbackfn - ((value: any, index?: integer, array?: array) => any) Called
 *       for each entry of array; its result is the entry of the new array at
 *       the same index. A script function is called with (value, index, array);
 *       a built-in function is called with the value alone, since its other
 *       parameters have their own meanings.
 *
 * Returns:
 *
 *   (array) A new array with one result per entry of array.
 */
const afw_value_t *
afw_function_execute_map(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: reduce
 *
 * afw_function_execute_reduce
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Call callbackfn for each entry of array, in index order, with the accumulator
 * and that entry; each call's result is the accumulator for the next call. If
 * initialValue is passed, it is the first accumulator and every entry is
 * visited; otherwise the first entry is the first accumulator and the walk
 * starts at the second. Undefined entries are visited. An empty array with no
 * initialValue is an error.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function reduce(
 *       array: array,
 *       callbackfn: (accumulator: any, value: any, index?: integer, array?: array) => any,
 *       initialValue?: any
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   array - (array) The array to walk. A string is not walked as an array:
 *       write [...value] to walk its characters.
 *
 *   callbackfn - ((accumulator: any, value: any, index?: integer, array?:
 *       array) => any) Called for each entry visited; its result is the next
 *       accumulator. A script function is called with (accumulator, value,
 *       index, array); a built-in function is called with (accumulator, value).
 *
 *   initialValue - (optional any) The first accumulator. When it is not passed,
 *       the first entry of array is.
 *
 * Returns:
 *
 *   (any) The last result of callbackfn, or the initial accumulator if
 *       callbackfn is not called.
 */
const afw_value_t *
afw_function_execute_reduce(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: some
 *
 * afw_function_execute_some
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return true if predicate returns true for any entry of array (index order).
 * Entries whose value is undefined are included. An empty array yields false.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function some(
 *       array: array,
 *       predicate: (value: any, index?: integer, array?: array) => boolean
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   array - (array) The array to walk. A string is not walked as an array:
 *       write [...value] to walk its characters.
 *
 *   predicate - ((value: any, index?: integer, array?: array) => boolean)
 *       Called for each entry of array until it returns true. A script function
 *       is called with (value, index, array); a built-in function is called
 *       with the value alone, since its other parameters have their own
 *       meanings.
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_some(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: sort
 *
 * afw_function_execute_sort
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Sort the entries of array in place, ordered by compareFn, or, with no
 * compareFn, as < orders them, and return the same array, as in ECMAScript. The
 * sort is stable: entries that compare equal keep their order. With no
 * compareFn, every entry must be of one data type (a mix is an error, as it is
 * for <); unlike ECMAScript, entries are not converted to strings, so [10, 9,
 * 1] sorts to [1, 9, 10]. NaN sorts after every other double. With compareFn,
 * the array may hold any mix of data types. Undefined entries go last and are
 * not compared, as in ECMAScript. A frozen array is an error before compareFn
 * is called. Use to_sorted for a sorted copy.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function sort(
 *       array: array,
 *       compareFn?: (a: any, b: any) => boolean | integer
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   array - (array) The array to sort. Must not be frozen.
 *
 *   compareFn - (optional (a: any, b: any) => boolean | integer) Called with
 *       two defined entries, once per comparison. If omitted, entries are
 *       ordered as < orders them. Return true, or a negative integer, when a
 *       sorts before b; false, zero or a positive integer otherwise. Entries
 *       that compare equal keep their order. Any other result, a double
 *       included, is an error.
 *
 * Returns:
 *
 *   (array) The array, sorted.
 */
const afw_value_t *
afw_function_execute_sort(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: to_sorted
 *
 * afw_function_execute_to_sorted
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return a new array with the entries of array ordered by compareFn, or, with
 * no compareFn, as < orders them, leaving array unchanged, as ECMAScript
 * toSorted does. The sort is stable: entries that compare equal keep their
 * order. Undefined entries go last and are not compared. With no compareFn,
 * every entry must be of one data type, and NaN sorts after every other double.
 * The new array is an `array<T>` only if array is one.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function to_sorted(
 *       array: array,
 *       compareFn?: (a: any, b: any) => boolean | integer
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   array - (array) The array to sort.
 *
 *   compareFn - (optional (a: any, b: any) => boolean | integer) Called with
 *       two defined entries, once per comparison. If omitted, entries are
 *       ordered as < orders them. Return true, or a negative integer, when a
 *       sorts before b; false, zero or a positive integer otherwise. Entries
 *       that compare equal keep their order. Any other result, a double
 *       included, is an error.
 *
 * Returns:
 *
 *   (array) A new array with the entries of array in sorted order.
 */
const afw_value_t *
afw_function_execute_to_sorted(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
