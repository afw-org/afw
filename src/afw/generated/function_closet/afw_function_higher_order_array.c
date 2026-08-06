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
 * See afw_function_bindings.h for more information.
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
 * See afw_function_bindings.h for more information.
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
 * See afw_function_bindings.h for more information.
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
 * See afw_function_bindings.h for more information.
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
 * See afw_function_bindings.h for more information.
 *
 * Returns true if the result of calling predicate with all of the combination
 * of values from array2 and any of the values of array1 returns true.
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
 * See afw_function_bindings.h for more information.
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
 * Adaptive function: filter
 *
 * afw_function_execute_filter
 *
 * See afw_function_bindings.h for more information.
 *
 * Return a new array of entries from the first array in values for which
 * predicate returns true. Every index is considered, including entries whose
 * value is undefined. Order of kept entries is preserved.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function filter(
 *       predicate: (...values: any) => boolean,
 *       values_1: any,
 *       ...values_rest: any[]
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   predicate - ((...values: any) => boolean) This is a boolean function that
 *       is called to determine if an array entry should be included in the
 *       returned array.
 *
 *   values - (1 or more any) These are the values passed to the predicate with
 *       the exception that the first array is passed as the single current
 *       value from the array. At least one array is required.
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
 * See afw_function_bindings.h for more information.
 *
 * Call predicate for each entry of the first array in values, in index order,
 * until it returns true, then return that entry. Entries whose value is
 * undefined are included. If no entry passes, the result is undefined (the same
 * as a found undefined entry; use filter if you need to tell those apart).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function find(
 *       predicate: (...values: any) => boolean,
 *       values_1: any,
 *       ...values_rest: any[]
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   predicate - ((...values: any) => boolean) This is a boolean function that
 *       is called to determine if an array entry passes the test.
 *
 *   values - (1 or more any) These are the values passed to the predicate with
 *       the exception that the first array is passed as the single current
 *       value from the array. At least one array is required.
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
 * Adaptive function: map
 *
 * afw_function_execute_map
 *
 * See afw_function_bindings.h for more information.
 *
 * Call functor once for each entry of the first array in values, in index order
 * from 0 through length minus one, and return a new array of the same length
 * with the results. Entries whose value is undefined (including omitted
 * elements in array literals) are included; the functor receives undefined for
 * those indexes. Additional values parameters, if present, are passed through
 * on every call.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function map(
 *       functor: (...values: any) => any,
 *       values_1: any,
 *       ...values_rest: any[]
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   functor - ((...values: any) => any)
 *
 *   values - (1 or more any) The first array is walked one entry at a time as
 *       the first argument to functor. Additional parameters are passed on
 *       every call. At least one array is required.
 *
 * Returns:
 *
 *   (array) A new array with one result per entry of the first array.
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
 * See afw_function_bindings.h for more information.
 *
 * Call functor for each entry of array, in index order, with the current
 * accumulator and that entry. The first call uses the accumulator argument;
 * each later call uses the previous return value. Every index is visited,
 * including undefined entries. If array is empty, the accumulator argument is
 * returned without calling functor.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function reduce(
 *       functor: (accumulator: any, value: any) => any,
 *       accumulator: any,
 *       array: array
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   functor - ((accumulator: any, value: any) => any) This function is called
 *       for each value in an array. The returned value is passed as the
 *       accumulator parameter on the next call to functor().
 *
 *   accumulator - (any) This is an initial accumulator value passed to
 *       functor(). Normally, the dataType of accumulator will be the data type
 *       for the reduce() return value, but this is not required.
 *
 *   array - (array) This is an array to be reduced.
 *
 * Returns:
 *
 *   (any) The final value returned by functor, or the initial accumulator if
 *       array is empty.
 */
const afw_value_t *
afw_function_execute_reduce(
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
 * See afw_function_bindings.h for more information.
 *
 * Return a new array with the same entries as array, ordered using
 * compareFunction. The array must have a single element data type (for example
 * all integers or all strings); mixed or empty untyped arrays are not accepted.
 * compareFunction is called with two entries and must return true when the
 * first should sort before the second (boolean), not a numeric sort key.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function sort(
 *       compareFunction: (value1: any, value2: any) => boolean,
 *       array: array
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   compareFunction - ((value1: any, value2: any) => boolean) Return true if
 *       value1 should be ordered before value2.
 *
 *   array - (array) Array to sort. Must be single-type (all entries the same
 *       data type).
 *
 * Returns:
 *
 *   (array) A new array with the entries of array in sorted order.
 */
const afw_value_t *
afw_function_execute_sort(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
