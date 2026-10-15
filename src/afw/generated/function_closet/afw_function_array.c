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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: at
 *
 * afw_function_execute_at
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the value at a zero-based index in an array, or the code point at a
 * zero-based index in a string as a one-code-point string. Negative indexes
 * count from the end (-1 is the last). If the index is out of range, the result
 * is undefined. Bracket indexing a[index] and s[index] use the same
 * out-of-range result (undefined).
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: create_array
 *
 * afw_function_execute_create_array
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Create a new mutable array of the given length where every entry is value, or
 * undefined when value is not passed. Useful when you want a known length up
 * front before assigning entries; assigning one past the end appends, further
 * is an error. A computed array is create_array(n)->map(function(v, i) { ...
 * }). Length must be a non-negative integer and must not exceed 1,000,000. This
 * is a length-based constructor, not a conversion function (see also
 * array(...), which builds from elements).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function create_array(
 *       length: integer,
 *       value?: any
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   length - (integer) Number of entries (0 or more, up to the maximum).
 *
 *   value - (optional any) The value of every entry. A function is stored as
 *       the entry, not called; write create_array(n)->map(fn) for computed
 *       entries. If not passed, every entry is undefined.
 *
 * Returns:
 *
 *   (array) A new array of the requested length; each entry is value, or
 *       undefined.
 *
 * Errors thrown:
 *
 *   argument_error - length is negative or exceeds the maximum allowed
 */
const afw_value_t *
afw_function_execute_create_array(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: fill
 *
 * afw_function_execute_fill
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Set the entries of array from start up to (not including) end to value, in
 * place, and return the same array. Negative indexes count from the end and
 * indexes out of range are clamped, as for slice. The length of the array does
 * not change. A frozen array is an error. A function passed as value is stored,
 * not called.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function fill(
 *       array: array,
 *       value: any,
 *       start?: integer,
 *       end?: integer
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   array - (array) The array to fill. Must not be frozen.
 *
 *   value - (any) The value to store in each entry.
 *
 *   start - (optional integer) The first index to fill. The default is 0.
 *       Negative counts from the end.
 *
 *   end - (optional integer) One more than the last index to fill. The default
 *       is the length. Negative counts from the end.
 *
 * Returns:
 *
 *   (array) The array, filled.
 */
const afw_value_t *
afw_function_execute_fill(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: flat
 *
 * afw_function_execute_flat
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return a new array with the entries of array, where an entry that is an array
 * is replaced by its entries, down to depth levels. The default depth is 1; a
 * negative depth flattens every level; 0 copies the array. Undefined entries
 * are kept (Adaptive arrays have no holes).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function flat(
 *       array: array,
 *       depth?: integer
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   array - (array) The array to flatten.
 *
 *   depth - (optional integer) How many levels of nested arrays to flatten. The
 *       default is 1; a negative depth flattens every level.
 *
 * Returns:
 *
 *   (array) A new, flattened array.
 */
const afw_value_t *
afw_function_execute_flat(
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: push
 *
 * afw_function_execute_push
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Append one or more values to the end of a mutable array. Returns the new
 * length of the array. The array must not be immutable (for example after
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
 *   ): integer;
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
 *   (integer) The new length of the array.
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
 * See afw_function_bindings_internal.h for more information.
 *
 * Reverse the order of the entries of array in place and return the same array.
 * A frozen array is an error. Use to_reversed for a reversed copy.
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: slice
 *
 * afw_function_execute_slice
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * This function extracts a consecutive slice of values from an array, or of
 * code points from a string. For a string, or another value stored as UTF-8,
 * the result is a string.
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
 *       added to it. For a string, a string containing the selected code
 *       points.
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: to_reversed
 *
 * afw_function_execute_to_reversed
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return a new array with the entries of array in reverse order, leaving array
 * unchanged. The new array is an `array<T>` only if array is one.
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: to_spliced
 *
 * afw_function_execute_to_spliced
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return a new array that is array with the changes splice would make, leaving
 * array unchanged: deleteCount values removed at startIndex and values inserted
 * there. Negative startIndex counts from the end. If deleteCount is omitted,
 * all values from startIndex to the end are removed.
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: unshift
 *
 * afw_function_execute_unshift
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Insert one or more values at the beginning of a mutable array, preserving the
 * relative order of the inserted values. Returns the new length of the array.
 * The array must not be immutable (for example after freeze).
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
 *   ): integer;
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
 *   (integer) The new length of the array.
 */
const afw_value_t *
afw_function_execute_unshift(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: with_at
 *
 * afw_function_execute_with_at
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return a new array that is array with the value at index replaced by value,
 * leaving array unchanged (it is named with_at because with is a reserved
 * word). Negative index counts from the end, as in at. An index out of range is
 * an error.
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
