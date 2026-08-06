// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for array
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_array.c
 * @brief afw_function_execute_* functions for array.
 */

#include "afw.h"



/*
 * Adaptive function: add_entries
 *
 * afw_function_execute_add_entries
 *
 * See afw_function_bindings.h for more information.
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: array
 *
 * afw_function_execute_array
 *
 * See afw_function_bindings.h for more information.
 *
 * Construct a new array from the given values. If a value is written as
 * ...expression and the expression is an array, each of its entries is included
 * in order. An empty call produces an empty array.
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
 *
 * Errors thrown:
 *
 *   cast_error - value could not be converted
 */
const afw_value_t *
afw_function_execute_array(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: at
 *
 * afw_function_execute_at
 *
 * See afw_function_bindings.h for more information.
 *
 * Return the value at a zero-based index in an array. Negative indexes count
 * from the end (-1 is the last element). If the index is out of range, the
 * result is undefined (not an error). Bracket indexing a[index] is different:
 * an out-of-range index raises an error.
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: empty_array
 *
 * afw_function_execute_empty_array
 *
 * See afw_function_bindings.h for more information.
 *
 * Create a new mutable array of the given length where every entry is
 * undefined. Useful when you want a known length up front before assigning or
 * filling entries. Length must be a non-negative integer and must not exceed
 * 1,000,000.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function empty_array(
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
 *   arg_error - length is negative or exceeds the maximum allowed
 */
const afw_value_t *
afw_function_execute_empty_array(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: join
 *
 * afw_function_execute_join
 *
 * See afw_function_bindings.h for more information.
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: pop
 *
 * afw_function_execute_pop
 *
 * See afw_function_bindings.h for more information.
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: push
 *
 * afw_function_execute_push
 *
 * See afw_function_bindings.h for more information.
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: reverse
 *
 * afw_function_execute_reverse
 *
 * See afw_function_bindings.h for more information.
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: shift
 *
 * afw_function_execute_shift
 *
 * See afw_function_bindings.h for more information.
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: slice
 *
 * afw_function_execute_slice
 *
 * See afw_function_bindings.h for more information.
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: splice
 *
 * afw_function_execute_splice
 *
 * See afw_function_bindings.h for more information.
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: unshift
 *
 * afw_function_execute_unshift
 *
 * See afw_function_bindings.h for more information.
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
