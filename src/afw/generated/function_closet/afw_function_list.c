// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for list
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_list.c
 * @brief afw_function_execute_* functions for list.
 */

#include "afw.h"



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
 *   value - (list) A list of values (statements) of any data type.
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: list
 *
 * afw_function_execute_list
 *
 * See afw_function_bindings.h for more information.
 *
 * Construct a list with 0 or more elements.
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
 *   values - (0 or more any dataType) A value can refer to any adaptable value
 *       belonging to any data type or a list expression. In the case of a list
 *       expression, indicated by '...' followed by an expression that results
 *       in a list, every element within that list is included in the newly
 *       created list.
 *
 * Returns:
 *
 *   (list) The constructed list.
 *
 * Errors thrown:
 *
 *   cast_error - value could not be converted
 */
const afw_value_t *
afw_function_execute_list(
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
