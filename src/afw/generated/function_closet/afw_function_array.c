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
 * Add the entries of one or more arrays to another.
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
 *       ...source_rest: (array of array)
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
 * Construct an array with 0 or more elements.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function array(
 *       ...values: (array of any)
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   values - (0 or more any dataType) A value can refer to any adaptable value
 *       belonging to any data type or an array expression. In the case of an
 *       array expression, indicated by '...' followed by an expression that
 *       results in an array, every element within that array is included in the
 *       newly created array.
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
