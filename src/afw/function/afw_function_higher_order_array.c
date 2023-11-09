// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for higher_order_array
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_higher_order_array.c
 * @brief afw_function_execute_* functions for higher_order_array.
 */

#include "afw_internal.h"

typedef struct {
    const afw_value_function_definition_t *function;
    const afw_value_t *functor;
    const afw_array_t *array;
    afw_size_t n;
    const afw_data_type_t *data_type;
    const afw_value_t * *entry_arg_ptr;
    const void *entry_internal;
    const afw_value_t *entry_result;
    void *data;
    const afw_pool_t *p;
    afw_xctx_t *xctx;
} impl_call_over_array_cb_e_t;

typedef afw_boolean_t
(*impl_call_over_array_cb_t)(impl_call_over_array_cb_e_t *e);

static const afw_data_type_t *
impl_over_array(
    afw_function_execute_t *x,
    impl_call_over_array_cb_t callback,
    void *data)
{

    afw_size_t functor_argc;
    const afw_value_t * (*functor_argv);
    const afw_iterator_t *iterator;
    impl_call_over_array_cb_e_t e;

    /* Initialize param. */
    afw_memory_clear(&e);
    e.function = x->function;
    e.data = data;
    e.p = x->p;
    e.xctx = x->xctx;

    /* Create skeleton call. */
    functor_argc = x->argc - 1;
    functor_argv = afw_pool_calloc(e.p, sizeof(afw_value_t *) * x->argc,
        e.xctx);
    functor_argv[0] = afw_function_evaluate_function_parameter(
        x->argv[1], e.p, e.xctx);
    e.functor = afw_value_call_create(NULL, functor_argc, functor_argv, false,
        e.p, e.xctx);

    /*
     * Evaluate argv[n+1] to the new argv[n]. Replace first typed array with
     * pointer to an allocated value of the same data type.
     */
    for (e.n = 1; e.n <= functor_argc; e.n++) {
        functor_argv[e.n] = afw_value_evaluate(x->argv[e.n + 1], e.p, e.xctx);
        if (!e.entry_arg_ptr && afw_value_is_array(functor_argv[e.n])) {
            e.entry_arg_ptr = &functor_argv[e.n];
            e.array = ((const afw_value_array_t *)*e.entry_arg_ptr)->internal;
            *e.entry_arg_ptr = NULL;
            e.data_type = afw_array_get_data_type(e.array, e.xctx);
            if (e.data_type) {
                *e.entry_arg_ptr = (const afw_value_t *)
                    afw_value_common_allocate(e.data_type, e.p, e.xctx);
            }
        }
    }

    /* There must be a typed array in parameter array. */
    if (!e.entry_arg_ptr) {
        AFW_THROW_ERROR_Z(arg_error, "Missing typed array arg", e.xctx);
    }

    /* Call function with each entry in typed array as a single value. */
    if (e.data_type) {
        for (iterator = NULL;;) {
            afw_array_get_next_internal(e.array, &iterator, NULL,
                &e.entry_internal, e.xctx);
            if (!e.entry_internal) {
                break;
            }
            memcpy(AFW_VALUE_INTERNAL(*e.entry_arg_ptr), e.entry_internal,
                e.data_type->c_type_size);
            e.entry_result = afw_value_evaluate(e.functor, e.p, e.xctx);

            if (!callback(&e))
            {
                break;
            }
        }
    }

    /* Call function with each entry in untyped array as a single value. */
    else {
        for (iterator = NULL;;) {
            *e.entry_arg_ptr = afw_array_get_next_value(
                e.array, &iterator, NULL, e.xctx);
            if (!*e.entry_arg_ptr) {
                break;
            }
            e.entry_result = afw_value_evaluate(e.functor, e.p, e.xctx);
            if (!callback(&e))
            {
                break;
            }
        }
    }

    return e.data_type;
}


typedef struct {
    /* stop_on passed to impl_of(). */
    afw_boolean_t stop_on;

    /* This will be set to stop_on if found.  Initialize to !stop_on. */
    afw_boolean_t result;
} impl_of_data_t;

static afw_boolean_t
impl_of_cb(impl_call_over_array_cb_e_t *e)
{
    impl_of_data_t *data = (impl_of_data_t *)e->data;

    AFW_VALUE_ASSERT_IS_DATA_TYPE(e->entry_result, boolean, e->xctx);

    /* If function returned stop_on result, return appropriate result. */
    if (((const afw_value_boolean_t *)e->entry_result)->internal ==
        data->stop_on)
    {
        data->result = data->stop_on;
        return false;
    }

    return true;
}

static const afw_value_t *
impl_of(
    afw_function_execute_t *x,
    afw_boolean_t stop_on)
{
    impl_of_data_t data;

    data.stop_on = stop_on;
    data.result = !stop_on; /* Assume will not be found. */
    impl_over_array(x, impl_of_cb, (void *)&data);

    /* Return true or false based on result. */
    return data.result ? afw_value_true : afw_value_false;
}

static const afw_value_t *
impl_bag_of_bag(
    afw_function_execute_t *x,
    afw_boolean_t all_1, afw_boolean_t all_2)
{
    const afw_value_array_t *array1, *array2, *arrayx;
    const afw_data_type_t *data_type_1, *data_type_2;
    const afw_iterator_t *iterator1, *iterator2;
    const void *internal1, *internal2;
    void *e1, *e2;
    const afw_value_t * f_argv[3];
    const afw_value_t *v;
    const afw_value_t *call;
    afw_boolean_t is_true, any_1, any_2;

    /* The first arg is the function to call, and other 2 are typed arrays. */
    f_argv[0] = afw_function_evaluate_function_parameter(
        x->argv[1], x->p, x->xctx);
    call = afw_value_call_create(NULL, 2, &f_argv[0], false, x->p, x->xctx);

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array1, 2, array);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array2, 3, array);

    /* Work off of any* variables. */
    any_1 = !all_1;
    any_2 = !all_2;

    /* If any_of_all, reverse bags and any* values. */
    if (!all_1 && all_2) {
        arrayx = array1;
        array1 = array2;
        array2 = arrayx;
        any_1 = !all_2;
        any_2 = !all_1;
    }

    /* Get required data type from each array. */
    data_type_1 = afw_array_get_data_type(array1->internal, x->xctx);
    data_type_2 = afw_array_get_data_type(array2->internal, x->xctx);
    if (!data_type_1 || !data_type_2) {
        AFW_THROW_ERROR_Z(general,
            "array1 and array2 must both be typed",
            x->xctx);
    }

    /* Allocate a single value to use for each bag1 entry. */
    f_argv[1] = (afw_value_t *)afw_value_common_allocate(
        data_type_1, x->p, x->xctx);
    e1 = (void *)&((afw_value_common_t *)f_argv[1])->internal;

    /* Allocate a single value to use for each bag2 entry. */
    f_argv[2] = (afw_value_t *)afw_value_common_allocate(
        data_type_2, x->p, x->xctx);
    e2 = (void *)&((afw_value_common_t *)f_argv[2])->internal;

    /* Call function for each combination of bag1 and bag2 entries. */
    is_true = true;

    for (iterator1 = NULL;;) {
        afw_array_get_next_internal(array1->internal,
            &iterator1, NULL, &internal1, x->xctx);
        if (!internal1) {
            break;
        }
        is_true = true;
        for (iterator2 = NULL;;) {
            afw_array_get_next_internal(array2->internal,
                &iterator2, NULL, &internal2, x->xctx);
            if (!internal2) {
                break;
            }
            memcpy(e1, internal1, data_type_1->c_type_size);
            memcpy(e2, internal2, data_type_2->c_type_size);
            v = afw_value_evaluate(call, x->p, x->xctx);
            if (!afw_value_is_boolean(v)) {
                AFW_THROW_ERROR_Z(arg_error,
                    "First argument must be a boolean function", x->xctx);
            }

            /* If true, indicate and break if any_2 enough. */
            if (((const afw_value_boolean_t *)v)->internal) {
                is_true = true;
                if (any_2) break;
            }

            /* If false, indicate and break if all must be true. */
            else {
                is_true = false;
                if (!any_2) break;
            }
        }

        /* If any_1 is ok, return true. */
        if (any_1 && is_true) break;

        /* If all must be true, return false. */
        if (!any_1 && !is_true) break;
    }

    /* If made it though loop, result is whatever is_true is. */
    return (is_true) ? afw_value_true : afw_value_false;
}



/*
 * Adaptive function: all_of
 *
 * afw_function_execute_all_of
 *
 * See afw_function_bindings.h for more information.
 *
 * Returns true if all values in an array pass the predicate test.
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
 *       ...values_rest: (array of any)
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   predicate - (function (... values: any): boolean) This function is called
 *       for each value in the first array in values or until false is returned.
 *       If no calls return false, the result is true.
 *
 *   values - (1 or more any dataType) These are the parameters passed to
 *       predicate with the exception that the first array is passed one value
 *       at a time. At least one array is required.
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_all_of(
    afw_function_execute_t *x)
{
    return impl_of(x, false);
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
 *       predicate: (function (any value1: any, value2: any): boolean),
 *       array1: array,
 *       array2: array
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   predicate - (function (any value1: any, value2: any): boolean) The
 *       predicate is passed two parameters, the first is a value from array1
 *       and the second is a value from array2.
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
    return impl_bag_of_bag(x, true, true);
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
 *       predicate: (function (value1: any, value2: any): boolean),
 *       array1: array,
 *       array2: array
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   predicate - (function (value1: any, value2: any): boolean) The predicate is
 *       passed two parameters, the first is a value from array1 and the second
 *       is a value from array2.
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
    return impl_bag_of_bag(x, true, false);
}



/*
 * Adaptive function: any_of
 *
 * afw_function_execute_any_of
 *
 * See afw_function_bindings.h for more information.
 *
 * Returns true if any value in an array pass the predicate test.
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
 *       ...values_rest: (array of any)
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   predicate - (function (... values: any): boolean) This function is called
 *       for each value in the first array in values or until true is returned.
 *       If no calls return true, the result is false.
 *
 *   values - (1 or more any dataType) These are the parameters passed to
 *       predicate with the exception that the first array is passed one value
 *       at a time. At least one array is required.
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_any_of(
    afw_function_execute_t *x)
{
    return impl_of(x, true);
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
 *       predicate: (function (value1: any, value2: any):boolean),
 *       array1: array,
 *       array2: array
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   predicate - (function (value1: any, value2: any):boolean) The predicate is
 *       passed two parameters, the first is a value from array1 and the second
 *       is a value from array2.
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
    return impl_bag_of_bag(x, false, true);
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
 *       predicate: (function (value1: any, value2: any): boolean),
 *       array1: array,
 *       array2: array
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   predicate - (function (value1: any, value2: any): boolean) The predicate is
 *       passed two parameters, the first is a value from array1 and the second
 *       is a value from array2.
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
    return impl_bag_of_bag(x, false, false);
}



typedef struct {
    const afw_array_t *filtered_array;
} impl_filter_data_t;

static afw_boolean_t
impl_filter_cb(impl_call_over_array_cb_e_t *e)
{
    impl_filter_data_t * data = (impl_filter_data_t *)e->data;

    AFW_VALUE_ASSERT_IS_DATA_TYPE(e->entry_result, boolean, e->xctx);

    if (((const afw_value_boolean_t *)e->entry_result)->internal) {
        afw_array_add_internal(data->filtered_array,
            e->data_type, e->entry_internal, e->xctx);
    }

    return true;
}

/*
 * Adaptive function: filter
 *
 * afw_function_execute_filter
 *
 * See afw_function_bindings.h for more information.
 *
 * This produces an array containing only values from another array that pass a
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
 *       ...values_rest: (array of any)
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   predicate - (function (... values: any): boolean) This is a boolean
 *       function that is called to determine if an array entry should be
 *       included in the returned array.
 *
 *   values - (1 or more any dataType) These are the values passed to the
 *       predicate with the exception that the first array is passed as the
 *       single current value from the array. At least one array is required.
 *
 * Returns:
 *
 *   (array) This is the resulting filtered array.
 */
const afw_value_t *
afw_function_execute_filter(
    afw_function_execute_t *x)
{
    impl_filter_data_t data;

    data.filtered_array = afw_array_create_with_options(0, NULL, x->p, x->xctx);
    impl_over_array(x, impl_filter_cb, (void *)&data);

    return afw_value_create_array_unmanaged(data.filtered_array, x->p, x->xctx);
}



typedef struct {
    const afw_value_t *found_value;
} impl_find_data_t;

static afw_boolean_t
impl_find_cb(impl_call_over_array_cb_e_t *e)
{
    impl_find_data_t * data = (impl_find_data_t *)e->data;

    AFW_VALUE_ASSERT_IS_DATA_TYPE(e->entry_result, boolean, e->xctx);

    if (((const afw_value_boolean_t *)e->entry_result)->internal) {
        data->found_value = afw_value_common_create(
            e->entry_internal, e->data_type, e->p, e->xctx);
        return false;
    }

    return true;
}

/*
 * Adaptive function: find
 *
 * afw_function_execute_find
 *
 * See afw_function_bindings.h for more information.
 *
 * The predicate is called for each value in the first array in values until
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
 *       ...values_rest: (array of any)
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   predicate - (function (... values: any): boolean) This is a boolean
 *       function that is called to determine if an array entry passes the test.
 *
 *   values - (1 or more any dataType) These are the values passed to the
 *       predicate with the exception that the first array is passed as the
 *       single current value from the array. At least one array is required.
 *
 * Returns:
 *
 *   (any dataType) The first value that passes the test is returned.
 */
const afw_value_t *
afw_function_execute_find(
    afw_function_execute_t *x)
{
    impl_find_data_t data;

    afw_memory_clear(&data);
    impl_over_array(x, impl_find_cb, (void *)&data);

    return (data.found_value) ? data.found_value : afw_value_undefined;
}



typedef struct {
    const afw_array_t *mapped_array;
} impl_map_data_t;

static afw_boolean_t
impl_map_cb(impl_call_over_array_cb_e_t *e)
{
    impl_map_data_t * data = (impl_map_data_t *)e->data;

    if (!data->mapped_array) {
        data->mapped_array = afw_array_create_generic(e->p, e->xctx);
    }

    afw_array_add_value(data->mapped_array, e->entry_result, e->xctx);

    return true;
}

/*
 * Adaptive function: map
 *
 * afw_function_execute_map
 *
 * See afw_function_bindings.h for more information.
 *
 * This function creates an array of the results of calling functor with each
 * value of the first array in values
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
 *       ...values_rest: (array of any)
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   functor - (function (... values: any): any)
 *
 *   values - (1 or more any dataType) These are the parameters passed to
 *       functor with the exception that the first array is passed one value at
 *       a time. At least one array is required.
 *
 * Returns:
 *
 *   (array)
 */
const afw_value_t *
afw_function_execute_map(
    afw_function_execute_t *x)
{
    impl_map_data_t data;
    const afw_data_type_t *data_type;

    afw_memory_clear(&data);
    data_type = impl_over_array(x, impl_map_cb, (void *)&data);

    if (!data.mapped_array) {
        if (data_type)
        {
            return data_type->empty_array_value;
        }
        else {
            return afw_data_type_null->empty_array_value;
        }
    }

    afw_array_determine_data_type_and_set_immutable(data.mapped_array, x->xctx);
    
    return afw_value_create_array_unmanaged(data.mapped_array, x->p, x->xctx);
}



/*
 * Adaptive function: reduce
 *
 * afw_function_execute_reduce
 *
 * See afw_function_bindings.h for more information.
 *
 * Reduce calls functor for each value in array with two parameters, accumulator
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
 *       array: array
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   functor - (function (accumulator: any, value: any): any) This function is
 *       called for each value in an array. The returned value is passed as the
 *       accumulator parameter on the next call to functor().
 *
 *   accumulator - (any dataType) This is an initial accumulator value passed to
 *       functor(). Normally, the dataType of accumulator will be the dataTape
 *       for the reduce() return value, but this is not required.
 *
 *   array - (array) This is an array to be reduced.
 *
 * Returns:
 *
 *   (any dataType) This is the final return value from functor() or the
 *       accumulator parameter value if array is empty.
 */
const afw_value_t *
afw_function_execute_reduce(
    afw_function_execute_t *x)
{
    const afw_value_array_t *array;
    const afw_value_t *accumulator;
    const afw_iterator_t *iterator;
    const afw_value_t * f_argv[3];
    const afw_value_t *call;

    f_argv[0] = afw_function_evaluate_function_parameter(
        x->argv[1], x->p, x->xctx);
    call = afw_value_call_create(NULL, 2, &f_argv[0], false, x->p, x->xctx);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(accumulator, 2);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 3, array);

    for (iterator = NULL;;) {
        f_argv[2] = afw_array_get_next_value(array->internal, &iterator,
            x->p, x->xctx);
        if (!f_argv[2]) {
            break;
        }
        f_argv[1] = accumulator;
        accumulator = afw_value_evaluate(call, x->p, x->xctx);
    }

    return accumulator;
}



typedef struct {
    const afw_value_t *compareFunction;
    afw_value_common_t *args[3];
    const void **values;
    afw_size_t c_type_size;
    afw_size_t count;
    const afw_pool_t *p;
    afw_xctx_t *xctx;
} impl_sort_ctx_t;

static afw_size_t
impl_partition(
    impl_sort_ctx_t *ctx,
    afw_size_t low,
    afw_size_t high) 
{ 
    const void *pivot;
    const void *value;
    const afw_value_t *return_value;
    afw_size_t i, j;

    /* Start pivot from high. */
    pivot = ctx->values[high];

    /* Put everything to right and left of pivot based on compare. */
    memcpy(&(ctx->args[2]->internal), pivot, ctx->c_type_size);
    for (i = low, j = low; j <= high - 1; j++)
    {
        /* Call compareFunction with ctx->values[j] and pivot. */
        memcpy(&(ctx->args[1]->internal), ctx->values[j], ctx->c_type_size);
        return_value = afw_value_evaluate(ctx->compareFunction,
            ctx->p, ctx->xctx);
        AFW_VALUE_ASSERT_IS_DATA_TYPE(return_value, boolean, ctx->xctx);

        /*
         * if compareFunction(values[j], pivot) is true, swap values[i] and
         * value[j] then increment i
         */
        if (((const afw_value_boolean_t *)return_value)->internal)
        {
            value = ctx->values[i];
            ctx->values[i] = ctx->values[j];
            ctx->values[j] = value;
            i++;
        }
    }

    /* Swap values[new pivot index] and values[high] */
    value = ctx->values[i];
    ctx->values[i] = ctx->values[high];
    ctx->values[high] = value;

    /* Return new pivot index. */
    return i;
} 
 
static void
impl_quick_sort(
    impl_sort_ctx_t *ctx,
    afw_size_t low,
    afw_size_t high) 
{ 
    afw_size_t pivot_i;

    if (low < high) 
    { 
        pivot_i = impl_partition(ctx, low, high);
        if (pivot_i > 0) {
            impl_quick_sort(ctx, low, pivot_i - 1);
        }
        impl_quick_sort(ctx, pivot_i + 1, high);
    } 
} 

/*
 * Adaptive function: sort
 *
 * afw_function_execute_sort
 *
 * See afw_function_bindings.h for more information.
 *
 * This produces an array with values sorted based on result of compareFunction.
 * The compareFunction is passed two values from the array and must return an
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
 *       array: array
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   compareFunction - (function (value1: any, value2: any): integer) This
 *       function is called with two value from array.
 *
 *   array - (array) This is the array to sort.
 *
 * Returns:
 *
 *   (array) This the the resulting sorted array.
 */
const afw_value_t *
afw_function_execute_sort(
    afw_function_execute_t *x)
{
    const afw_value_array_t *array;
    const afw_array_t *result_array;
    const afw_data_type_t *data_type;
    const afw_iterator_t *iterator;
    const void **value;
    impl_sort_ctx_t ctx;

    /* Initialize sort ctx. */
    afw_memory_clear(&ctx);
    ctx.p = x->p;
    ctx.xctx = x->xctx;

    /* The first arg is the function to call, and other 2 are typed arrays. */
    ctx.args[0] = (afw_value_common_t *)
        afw_function_evaluate_function_parameter(x->argv[1], ctx.p, ctx.xctx);
    ctx.compareFunction = afw_value_call_create(NULL,
        2, (const afw_value_t * const *)&ctx.args[0], false, ctx.p, ctx.xctx);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 2, array);

    /* Get the data type and count.  If count is 0, return empty array. */
    data_type = afw_array_get_data_type(array->internal, ctx.xctx);
    if (!data_type) {
        AFW_THROW_ERROR_Z(general,
            "sort() requires array to be typed", ctx.xctx);
    }
    ctx.c_type_size = data_type->c_type_size;
    ctx.count = afw_array_get_count(array->internal, ctx.xctx);
    if (ctx.count == 0) {
        return data_type->empty_array_value;
    }

    /* Allocate two values of the correct data type. */
    ctx.args[1] = afw_value_common_allocate(data_type, ctx.p, ctx.xctx);
    ctx.args[2] = afw_value_common_allocate(data_type, ctx.p, ctx.xctx);

    /* Make array of pointers to internal values. */
    ctx.values = afw_pool_malloc(ctx.p, sizeof(void *) * ctx.count, ctx.xctx);
    for (iterator = NULL, value = ctx.values;
        afw_array_get_next_internal(array->internal, &iterator,
            NULL, value, ctx.xctx);
        value++);

    /* Sort. */
    if (ctx.count > 0) {
        impl_quick_sort(&ctx, 0, ctx.count - 1);
    }

    /* Return sorted array. */
    result_array = afw_array_create_wrapper_for_array(
        ctx.values, true, data_type, ctx.count, ctx.p, ctx.xctx);
    return afw_value_create_array_unmanaged(result_array, ctx.p, ctx.xctx);
}
