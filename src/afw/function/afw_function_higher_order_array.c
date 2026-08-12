// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for higher_order_array
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_higher_order_array.c
 * @brief Adaptive function execute implementations for category `higher_order_array`.
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
    const afw_iterator_old_t *iterator;
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
    e.functor = afw_value_call_create(afw_function_execute_contextual(x),
        functor_argc, functor_argv, false, e.p, e.xctx);

    /*
     * Evaluate argv[n+1] to the new argv[n]. Prefer a real array as the
     * walked sequence; only then fall back to materializing the first
     * keyless-iterator value (utf8 code points) via as_array_sequence
     * (#153). Do not materialize every string arg — filter/find/any_of
     * pass scalar string thresholds next to the bag.
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
    if (!e.entry_arg_ptr) {
        for (e.n = 1; e.n <= functor_argc; e.n++) {
            if (afw_value_has_iterator(functor_argv[e.n])) {
                functor_argv[e.n] = afw_value_as_array_sequence(
                    functor_argv[e.n], e.p, e.xctx);
                if (afw_value_is_array(functor_argv[e.n])) {
                    e.entry_arg_ptr = &functor_argv[e.n];
                    e.array =
                        ((const afw_value_array_t *)*e.entry_arg_ptr)->
                            internal;
                    *e.entry_arg_ptr = NULL;
                    e.data_type = afw_array_get_data_type(e.array, e.xctx);
                    if (e.data_type) {
                        *e.entry_arg_ptr = (const afw_value_t *)
                            afw_value_common_allocate(
                                e.data_type, e.p, e.xctx);
                    }
                    break;
                }
            }
        }
    }

    /* There must be a typed array (or materializable sequence) arg. */
    if (!e.entry_arg_ptr) {
        AFW_THROW_ERROR_Z(arg_error, "Missing typed array arg", e.xctx);
    }

    /*
     * Call the functor once per array entry. For a single-type array, a
     * reusable value buffer holds the current element. Entries that are
     * undefined (or not of the array data type) are passed by value pointer
     * so we never memcpy undefined into a typed buffer.
     */
    if (e.data_type) {
        const afw_value_t *typed_slot;
        const afw_value_t *entry_value;
        const afw_data_type_t *entry_dt;

        typed_slot = *e.entry_arg_ptr;
        for (iterator = NULL;;) {
            entry_value = afw_array_get_next_value(
                e.array, &iterator, NULL, e.xctx);
            if (!entry_value) {
                break;
            }

            if (afw_value_is_undefined(entry_value)) {
                *e.entry_arg_ptr = entry_value;
                e.entry_internal = NULL;
            }
            else {
                entry_dt = afw_value_get_data_type(entry_value, e.xctx);
                if (entry_dt != e.data_type) {
                    *e.entry_arg_ptr = entry_value;
                    e.entry_internal = NULL;
                }
                else {
                    e.entry_internal = AFW_VALUE_INTERNAL(entry_value);
                    memcpy(AFW_VALUE_INTERNAL(typed_slot), e.entry_internal,
                        e.data_type->c_type_size);
                    *e.entry_arg_ptr = typed_slot;
                }
            }

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
    return data.result ? afw_boolean_v_true : afw_boolean_v_false;
}

static const afw_value_t *
impl_bag_of_bag(
    afw_function_execute_t *x,
    afw_boolean_t all_1, afw_boolean_t all_2)
{
    const afw_value_array_t *array1, *array2, *arrayx;
    const afw_data_type_t *data_type_1, *data_type_2;
    const afw_iterator_old_t *iterator1, *iterator2;
    const void *internal1, *internal2;
    void *e1, *e2;
    const afw_value_t * f_argv[3];
    const afw_value_t *v;
    const afw_value_t *call;
    afw_boolean_t is_true, any_1, any_2;

    /* The first arg is the function to call, and other 2 are typed arrays. */
    f_argv[0] = afw_function_evaluate_function_parameter(
        x->argv[1], x->p, x->xctx);
    call = afw_value_call_create(afw_function_execute_contextual(x),
        2, &f_argv[0], false, x->p, x->xctx);

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
    return (is_true) ? afw_boolean_v_true : afw_boolean_v_false;
}



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
    return impl_of(x, false);
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
    return impl_bag_of_bag(x, true, true);
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
    return impl_bag_of_bag(x, true, false);
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
    return impl_of(x, true);
}



/*
 * Adaptive function: any_of_all
 *
 * afw_function_execute_any_of_all
 *
 * See afw_function_bindings_internal.h for more information.
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
    return impl_bag_of_bag(x, false, true);
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
        /*
         * Typed matching elements use entry_internal. Undefined / mismatched
         * entries are passed by value pointer (entry_internal is NULL).
         */
        if (e->entry_internal && e->data_type) {
            afw_array_push_internal(data->filtered_array,
                e->data_type, e->entry_internal, e->xctx);
        }
        else if (e->entry_arg_ptr && *e->entry_arg_ptr) {
            afw_array_push_value(data->filtered_array,
                *e->entry_arg_ptr, e->xctx);
        }
    }

    return true;
}

/*
 * Adaptive function: filter
 *
 * afw_function_execute_filter
 *
 * See afw_function_bindings_internal.h for more information.
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
    impl_filter_data_t data;

    data.filtered_array = afw_array_create_with_options(0, NULL, x->p, x->xctx);
    impl_over_array(x, impl_filter_cb, (void *)&data);

    return afw_value_create_unmanaged_array(data.filtered_array, x->p, x->xctx);
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
        if (e->entry_internal && e->data_type) {
            data->found_value = afw_value_common_create(
                e->entry_internal, e->data_type, e->p, e->xctx);
        }
        else if (e->entry_arg_ptr) {
            data->found_value = *e->entry_arg_ptr;
        }
        return false;
    }

    return true;
}

/*
 * Adaptive function: find
 *
 * afw_function_execute_find
 *
 * See afw_function_bindings_internal.h for more information.
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
    const afw_value_t *to_push;

    if (!data->mapped_array) {
        data->mapped_array = afw_array_create_generic(e->p, e->xctx);
    }

    to_push = e->entry_result;
    /*
     * The typed-array path reuses one value buffer for matching elements. If
     * the functor returns that buffer (identity map, or return of the current
     * element), clone from the stable array entry internal so results do not
     * all alias the last element. When entry_internal is NULL, the argument
     * was passed by value pointer (undefined or type mismatch) and is already
     * stable.
     */
    if (e->data_type && e->entry_arg_ptr && e->entry_internal &&
        to_push == *e->entry_arg_ptr)
    {
        to_push = afw_value_common_create(
            e->entry_internal, e->data_type, e->p, e->xctx);
    }

    afw_array_push_value(data->mapped_array, to_push, e->xctx);

    return true;
}

/*
 * Adaptive function: map
 *
 * afw_function_execute_map
 *
 * See afw_function_bindings_internal.h for more information.
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
    
    return afw_value_create_unmanaged_array(data.mapped_array, x->p, x->xctx);
}



/*
 * Adaptive function: reduce
 *
 * afw_function_execute_reduce
 *
 * See afw_function_bindings_internal.h for more information.
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
    const afw_value_array_t *array;
    const afw_value_t *accumulator;
    const afw_iterator_old_t *iterator;
    const afw_value_t * f_argv[3];
    const afw_value_t *call;

    f_argv[0] = afw_function_evaluate_function_parameter(
        x->argv[1], x->p, x->xctx);
    call = afw_value_call_create(afw_function_execute_contextual(x),
        2, &f_argv[0], false, x->p, x->xctx);
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
 * See afw_function_bindings_internal.h for more information.
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
    const afw_value_array_t *array;
    const afw_array_t *result_array;
    const afw_data_type_t *data_type;
    const afw_iterator_old_t *iterator;
    const void **value;
    impl_sort_ctx_t ctx;

    /* Initialize sort ctx. */
    afw_memory_clear(&ctx);
    ctx.p = x->p;
    ctx.xctx = x->xctx;

    /* The first arg is the function to call, and other 2 are typed arrays. */
    ctx.args[0] = (afw_value_common_t *)
        afw_function_evaluate_function_parameter(x->argv[1], ctx.p, ctx.xctx);
    ctx.compareFunction = afw_value_call_create(afw_function_execute_contextual(x),
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
    result_array = afw_array_create_view_of_c_array(
        ctx.values, true, data_type, ctx.count, ctx.p, ctx.xctx);
    return afw_value_create_unmanaged_array(result_array, ctx.p, ctx.xctx);
}
