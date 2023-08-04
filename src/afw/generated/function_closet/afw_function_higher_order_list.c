// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for higher_order_list
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_higher_order_list.c
 * @brief afw_function_execute_* functions for higher_order_list.
 */

#include "afw.h"



/*
 * Adaptive function: all_of
 *
 * afw_function_execute_all_of
 *
 * See afw_function_bindings.h for more information.
 *
 * Returns true if all values in a list pass the predicate test.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function all_of(
 *       predicate: (function (... values: any): boolean),
 *       values_1: any,
 *       ...values_rest: (list of any)
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   predicate - (function (... values: any): boolean) This function is called
 *       for each value in the first list in values or until false is returned.
 *       If no calls return false, the result is true.
 *
 *   values - (1 or more any dataType) These are the parameters passed to
 *       predicate with the exception that the first list is passed one value
 *       at a time. At least one list is required.
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
 * of values from list1 and list2 returns true.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function all_of_all(
 *       predicate: (function (any value1: any, value2: any): boolean),
 *       list1: list,
 *       list2: list
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   predicate - (function (any value1: any, value2: any): boolean) The
 *       predicate is passed two parameters, the first is a value from list1
 *       and the second is a value from list2.
 *
 *   list1 - (list)
 *
 *   list2 - (list)
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
 * This function returns true if the result of calling predicate with all of
 * the combination of values from list1 and any of the values of list2 returns
 * true.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function all_of_any(
 *       predicate: (function (value1: any, value2: any): boolean),
 *       list1: list,
 *       list2: list
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   predicate - (function (value1: any, value2: any): boolean) The predicate
 *       is passed two parameters, the first is a value from list1 and the
 *       second is a value from list2.
 *
 *   list1 - (list)
 *
 *   list2 - (list)
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
 * Returns true if any value in a list pass the predicate test.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function any_of(
 *       predicate: (function (... values: any): boolean),
 *       values_1: any,
 *       ...values_rest: (list of any)
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   predicate - (function (... values: any): boolean) This function is called
 *       for each value in the first list in values or until true is returned.
 *       If no calls return true, the result is false.
 *
 *   values - (1 or more any dataType) These are the parameters passed to
 *       predicate with the exception that the first list is passed one value
 *       at a time. At least one list is required.
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
 * of values from list2 and any of the values of list1 returns true.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function any_of_all(
 *       predicate: (function (value1: any, value2: any):boolean),
 *       list1: list,
 *       list2: list
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   predicate - (function (value1: any, value2: any):boolean) The predicate is
 *       passed two parameters, the first is a value from list1 and the second
 *       is a value from list2.
 *
 *   list1 - (list)
 *
 *   list2 - (list)
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
 * This function returns true if the result of calling predicate with any of
 * the combination of values from list1 and list2 returns true.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function any_of_any(
 *       predicate: (function (value1: any, value2: any): boolean),
 *       list1: list,
 *       list2: list
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   predicate - (function (value1: any, value2: any): boolean) The predicate
 *       is passed two parameters, the first is a value from list1 and the
 *       second is a value from list2.
 *
 *   list1 - (list)
 *
 *   list2 - (list)
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
 * This produces a list containing only values from another list that pass a
 * predicate test.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function filter(
 *       predicate: (function (... values: any): boolean),
 *       values_1: any,
 *       ...values_rest: (list of any)
 *   ): list;
 * ```
 *
 * Parameters:
 *
 *   predicate - (function (... values: any): boolean) This is a boolean
 *       function that is called to determine if a list entry should be
 *       included in the returned list.
 *
 *   values - (1 or more any dataType) These are the values passed to the
 *       predicate with the exception that the first list is passed as the
 *       single current value from the list. At least one list is required.
 *
 * Returns:
 *
 *   (list) This is the resulting filtered list.
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
 * The predicate is called for each value in the first list in values until
 * true is returned, then that value is returned.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function find(
 *       predicate: (function (... values: any): boolean),
 *       values_1: any,
 *       ...values_rest: (list of any)
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   predicate - (function (... values: any): boolean) This is a boolean
 *       function that is called to determine if a list entry passes the test.
 *
 *   values - (1 or more any dataType) These are the values passed to the
 *       predicate with the exception that the first list is passed as the
 *       single current value from the list. At least one list is required.
 *
 * Returns:
 *
 *   (any dataType) The first value that passes the test is returned.
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
 * This function creates a list of the results of calling functor with each
 * value of the first list in values
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function map(
 *       functor: (function (... values: any): any),
 *       values_1: any,
 *       ...values_rest: (list of any)
 *   ): list;
 * ```
 *
 * Parameters:
 *
 *   functor - (function (... values: any): any)
 *
 *   values - (1 or more any dataType) These are the parameters passed to
 *       functor with the exception that the first list is passed one value at
 *       a time. At least one list is required.
 *
 * Returns:
 *
 *   (list)
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
 * Reduce calls functor for each value in list with two parameters, accumulator
 * and value, and must return a value of any dataType. Parameter accumulator is
 * the reduce() accumulator parameter value on first call and the return value
 * of previous functor() call on subsequent calls. The dataType of the return
 * value should normally be the same as accumulator, but this is not required.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function reduce(
 *       functor: (function (accumulator: any, value: any): any),
 *       accumulator: any,
 *       list: list
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   functor - (function (accumulator: any, value: any): any) This function is
 *       called for each value in a list. The returned value is passed as the
 *       accumulator parameter on the next call to functor().
 *
 *   accumulator - (any dataType) This is an initial accumulator value passed
 *       to functor(). Normally, the dataType of accumulator will be the
 *       dataTape for the reduce() return value, but this is not required.
 *
 *   list - (list) This is a list to be reduced.
 *
 * Returns:
 *
 *   (any dataType) This is the final return value from functor() or the
 *       accumulator parameter value if list is empty.
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
 * This produces a list with values sorted based on result of compareFunction.
 * The compareFunction is passed two values from the list and must return an
 * integer less than 0 if the first value is less than the second value, 0 if
 * they are equal, and a integer greater than 0 if the first value is greater
 * than the second value.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function sort(
 *       compareFunction: (function (value1: any, value2: any): integer),
 *       list: list
 *   ): list;
 * ```
 *
 * Parameters:
 *
 *   compareFunction - (function (value1: any, value2: any): integer) This
 *       function is called with two value from list.
 *
 *   list - (list) This is the list to sort.
 *
 * Returns:
 *
 *   (list) This the the resulting sorted list.
 */
const afw_value_t *
afw_function_execute_sort(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
