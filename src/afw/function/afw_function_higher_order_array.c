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


/*
 * The walk behind any_of and all_of (XACML order: the predicate, then
 * values; the first array is walked and the other values are passed on
 * every call). The ECMAScript-named functions use impl_walk.
 */
typedef struct {
    const afw_value_function_definition_t *function;
    const afw_value_t *functor;
    const afw_array_t *array;
    afw_size_t n;
    const afw_data_type_t *data_type;
    const afw_value_t * *entry_arg_ptr;
    const void *entry_internal;
    const afw_value_t *entry_value;
    const afw_value_t *entry_result;
    void *data;
    const afw_pool_t *p;
    afw_xctx_t *xctx;
} impl_call_over_array_cb_e_t;

typedef afw_boolean_t
(*impl_call_over_array_cb_t)(impl_call_over_array_cb_e_t *e);

static const afw_data_type_t *
impl_over_values(
    afw_function_execute_t *x,
    impl_call_over_array_cb_t callback,
    void *data)
{

    afw_size_t functor_argc;
    const afw_value_t * (*functor_argv);
    const afw_iterator_old_t *iterator;
    afw_size_t remaining;
    afw_size_t iterable_count;
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
     * walked sequence; only then, if the values parameter is marked
     * iterableAsArray, fall back to materializing the first keyless-iterator
     * value (utf8 code points) via convert_to_array_sequence (#153, #583).
     * Do not materialize every string arg — filter/find/any_of pass scalar
     * string thresholds next to the array.
     */
    for (e.n = 1; e.n <= functor_argc; e.n++) {
        functor_argv[e.n] = afw_value_evaluate(
            x->argv[e.n + 1], e.p, e.xctx);
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
    if (!e.entry_arg_ptr &&
        x->function->parameters_count >= 2 &&
        afw_value_is_boolean_true(x->function->parameters[1]->iterableAsArray))
    {
        /*
         * With no array, exactly one value may be read as the array:
         * any_of(eq<string>, "b", "abc") can't tell which string to walk.
         */
        for (iterable_count = 0, e.n = 1; e.n <= functor_argc; e.n++) {
            if (afw_value_has_iterator(functor_argv[e.n])) {
                iterable_count++;
            }
        }
        if (iterable_count > 1) {
            AFW_THROW_ERROR_FZ(argument_error, e.xctx,
                "No value passed to %ku is an array and more than one could"
                " be read as one; write [...value] for the one to walk",
                &x->function->functionId->internal);
        }
        for (e.n = 1; e.n <= functor_argc; e.n++) {
            if (afw_value_has_iterator(functor_argv[e.n])) {
                functor_argv[e.n] = afw_value_convert_to_array_sequence(
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

    /* There must be an array (or materializable sequence) arg. */
    if (!e.entry_arg_ptr) {
        AFW_THROW_ERROR_FZ(argument_error, e.xctx,
            "One of the values passed to %ku must be an array",
            &x->function->functionId->internal);
    }

    /*
     * Call the functor once per array entry. For a single-type array, a
     * reusable value buffer holds the current element. Entries that are
     * undefined (or not of the array data type) are passed by value pointer
     * so we never memcpy undefined into a typed buffer.
     *
     * Visit only the entries there at the start: a functor that adds to
     * the array it is called over must not keep the loop going.
     */
    if (e.data_type) {
        const afw_value_t *typed_slot;
        const afw_value_t *entry_value;
        const afw_data_type_t *entry_dt;

        typed_slot = *e.entry_arg_ptr;
        remaining = afw_array_get_count(e.array, e.xctx);
        for (iterator = NULL; remaining > 0; remaining--) {
            /*
             * The functor may replace this entry in the array; keep it
             * for the callback that uses it after (the read rule).
             */
            entry_value = afw_value_hold_read(afw_array_get_next_value(
                e.array, &iterator, e.xctx), e.p, e.xctx);
            if (!entry_value) {
                break;
            }

            e.entry_value = entry_value;
            if (afw_value_is_undefined(entry_value)) {
                *e.entry_arg_ptr = entry_value;
                e.entry_internal = NULL;
            }
            else {
                entry_dt = afw_value_get_data_type(entry_value, e.xctx);
                /*
                 * Only a value with the buffer's own inf can be copied
                 * into it. A value of the same data type with another inf
                 * (a closure or script function is data type function)
                 * is passed as is.
                 */
                if (entry_dt != e.data_type ||
                    entry_value->inf != typed_slot->inf)
                {
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
        remaining = afw_array_get_count(e.array, e.xctx);
        for (iterator = NULL; remaining > 0; remaining--) {
            *e.entry_arg_ptr = afw_value_hold_read(afw_array_get_next_value(
                e.array, &iterator, e.xctx), e.p, e.xctx);
            if (!*e.entry_arg_ptr) {
                break;
            }
            e.entry_value = *e.entry_arg_ptr;
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
    impl_over_values(x, impl_of_cb, (void *)&data);

    /* Return true or false based on result. */
    return data.result ? afw_boolean_v_true : afw_boolean_v_false;
}

static const afw_value_t *
impl_array_of_array(
    afw_function_execute_t *x,
    afw_boolean_t all_1, afw_boolean_t all_2)
{
    const afw_value_array_t *array1, *array2;
    const afw_iterator_old_t *iterator1, *iterator2;
    /* Cleared: call create may read the args (compile-time type check). */
    const afw_value_t * f_argv[3] = {NULL, NULL, NULL};
    const afw_value_t *v;
    const afw_value_t *call;
    afw_boolean_t is_true, any_1, any_2;
    afw_size_t remaining1, remaining2;

    /* The first arg is the function to call, and other 2 are typed arrays. */
    f_argv[0] = afw_function_evaluate_function_parameter(
        x->argv[1], x->p, x->xctx);
    call = afw_value_call_create(afw_function_execute_contextual(x),
        2, &f_argv[0], false, x->p, x->xctx);

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array1, 2, array);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array2, 3, array);

    /*
     * Outer quantifier over array1, inner over array2; the predicate gets
     * a value from array1 then one from array2. any_of_all is "some value
     * of array1 with every value of array2" (XACML any-of-all:
     * any_of_all(gt, [3, 5], [1, 2]) is true). It swapped the arrays,
     * which also swapped the predicate's arguments, and returned false.
     *
     * The predicate decides about each pair, so the arrays need no data
     * type, as in ECMAScript every / some: an empty literal [] or a mixed
     * array is fine ("array1 and array2 must both be typed"). Over no
     * values, "all" is true and "any" is false.
     */
    any_1 = !all_1;
    any_2 = !all_2;
    is_true = all_1;

    /* Visit the entries there at the start (see impl_over_array). */
    remaining1 = afw_array_get_count(array1->internal, x->xctx);
    for (iterator1 = NULL; remaining1 > 0; remaining1--) {
        f_argv[1] = afw_array_get_next_value(array1->internal,
            &iterator1, x->xctx);
        if (!f_argv[1]) {
            break;
        }
        is_true = all_2;
        remaining2 = afw_array_get_count(array2->internal, x->xctx);
        for (iterator2 = NULL; remaining2 > 0; remaining2--) {
            f_argv[2] = afw_array_get_next_value(array2->internal,
                &iterator2, x->xctx);
            if (!f_argv[2]) {
                break;
            }
            v = afw_value_evaluate(call, x->p, x->xctx);
            if (!afw_value_is_boolean(v)) {
                AFW_THROW_ERROR_Z(argument_error,
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
 * The walk behind the ECMAScript-named array functions (map, filter, find,
 * find_index, some, every): array first, callback second (#599).
 */
typedef struct {
    /* The entry being visited, its index, and what the callback returned. */
    const afw_value_t *entry_value;
    afw_size_t index;
    const afw_value_t *entry_result;
    void *data;
    const afw_pool_t *p;
    afw_xctx_t *xctx;
} impl_walk_e_t;

typedef afw_boolean_t
(*impl_walk_cb_t)(impl_walk_e_t *e);

/*
 * Call the callback once per entry of the array, in index order, and
 * callback() with each result until it returns false.
 *
 * A script function is called with (value, index, array), as in
 * ECMAScript; it ignores the arguments it does not declare. A built-in is
 * called with the value alone: its other parameters have their own
 * meanings, so map(a, add<integer>) is an error, not a quiet add of the
 * index (catch the accident, allow the intent).
 *
 * The array parameter is not iterableAsArray: a string throws with
 * "write [...value]" rather than being walked letter by letter.
 */
static void
impl_walk(
    afw_function_execute_t *x,
    impl_walk_cb_t callback,
    void *data)
{
    const afw_value_array_t *array;
    /* Cleared: call create may read the args (compile-time type check). */
    const afw_value_t *f_argv[4] = {NULL, NULL, NULL, NULL};
    const afw_value_t *call;
    const afw_iterator_old_t *iterator;
    afw_boolean_t is_built_in;
    afw_size_t remaining;
    impl_walk_e_t e;

    afw_memory_clear(&e);
    e.data = data;
    e.p = x->p;
    e.xctx = x->xctx;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 1, array);
    f_argv[0] = afw_function_evaluate_function_parameter(
        x->argv[2], e.p, e.xctx);
    is_built_in = afw_value_is_function_definition(f_argv[0]);
    f_argv[3] = &array->pub;
    call = afw_value_call_create(afw_function_execute_contextual(x),
        is_built_in ? 1 : 3, &f_argv[0], false, e.p, e.xctx);

    /*
     * Visit only the entries there at the start: a callback that adds to
     * the array it is called over must not keep the loop going.
     */
    remaining = afw_array_get_count(array->internal, e.xctx);
    for (iterator = NULL; remaining > 0; remaining--, e.index++) {
        /*
         * The callback may replace this entry in the array; keep it for
         * the callback() that uses it after (the read rule).
         */
        e.entry_value = afw_value_hold_read(afw_array_get_next_value(
            array->internal, &iterator, e.xctx), e.p, e.xctx);
        if (!e.entry_value) {
            break;
        }
        f_argv[1] = e.entry_value;
        if (!is_built_in) {
            f_argv[2] = afw_value_create_unmanaged_integer(
                (afw_integer_t)e.index, e.p, e.xctx);
        }
        e.entry_result = afw_value_evaluate(call, e.p, e.xctx);
        if (!callback(&e)) {
            break;
        }
    }
}



/* A predicate's result: true or false, nothing else. */
static afw_boolean_t
impl_walk_test(impl_walk_e_t *e)
{
    AFW_VALUE_ASSERT_IS_DATA_TYPE(e->entry_result, boolean, e->xctx);
    return ((const afw_value_boolean_t *)e->entry_result)->internal;
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
    return impl_array_of_array(x, true, true);
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
    return impl_array_of_array(x, true, false);
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
    return impl_array_of_array(x, false, true);
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
    return impl_array_of_array(x, false, false);
}



/* Stop at the first false (every) or true (some). */
static afw_boolean_t
impl_every_cb(impl_walk_e_t *e)
{
    if (!impl_walk_test(e)) {
        *(afw_boolean_t *)e->data = false;
        return false;
    }
    return true;
}

static afw_boolean_t
impl_some_cb(impl_walk_e_t *e)
{
    if (impl_walk_test(e)) {
        *(afw_boolean_t *)e->data = true;
        return false;
    }
    return true;
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
    afw_boolean_t result;

    result = true;
    impl_walk(x, impl_every_cb, &result);
    return result ? afw_boolean_v_true : afw_boolean_v_false;
}



static afw_boolean_t
impl_filter_cb(impl_walk_e_t *e)
{
    if (impl_walk_test(e)) {
        afw_array_push_value((const afw_array_t *)e->data,
            e->entry_value, e->xctx);
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
    const afw_value_array_t *result;

    /* New array: create_managed RC 1. Last-release of that hold on dest p. */
    result = (const afw_value_array_t *)
        afw_pool_scope_release_value_at_cleanup(
            afw_array_create_managed(NULL, x->p, x->xctx)->value,
            x->p, x->xctx);
    impl_walk(x, impl_filter_cb, (void *)result->internal);

    return &result->pub;
}



/* Stop at the first entry the predicate accepts; find and find_index. */
static afw_boolean_t
impl_find_cb(impl_walk_e_t *e)
{
    if (impl_walk_test(e)) {
        *(impl_walk_e_t *)e->data = *e;
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
    impl_walk_e_t found;

    afw_memory_clear(&found);
    impl_walk(x, impl_find_cb, &found);

    return (found.entry_value) ? found.entry_value : afw_value_undefined;
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
    impl_walk_e_t found;

    afw_memory_clear(&found);
    impl_walk(x, impl_find_cb, &found);

    return afw_value_create_unmanaged_integer(
        found.entry_value ? (afw_integer_t)found.index : -1,
        x->p, x->xctx);
}



static afw_boolean_t
impl_map_cb(impl_walk_e_t *e)
{
    afw_array_push_value((const afw_array_t *)e->data,
        e->entry_result, e->xctx);
    return true;
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
    const afw_value_array_t *result;

    /* New array: create_managed RC 1. Last-release of that hold on dest p. */
    result = (const afw_value_array_t *)
        afw_pool_scope_release_value_at_cleanup(
            afw_array_create_managed(NULL, x->p, x->xctx)->value,
            x->p, x->xctx);
    impl_walk(x, impl_map_cb, (void *)result->internal);

    return &result->pub;
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
    const afw_value_array_t *array;
    const afw_value_t *accumulator;
    const afw_iterator_old_t *iterator;
    /* Cleared: call create may read the args (compile-time type check). */
    const afw_value_t * f_argv[5] = {NULL, NULL, NULL, NULL, NULL};
    const afw_value_t *call;
    const afw_value_t *kept;
    const afw_value_t *next;
    const afw_pool_t *step_p;
    const afw_pool_t *next_p;
    afw_boolean_t is_built_in;
    afw_size_t remaining;
    afw_size_t index;
    afw_xctx_t *xctx = x->xctx;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 1, array);
    f_argv[0] = afw_function_evaluate_function_parameter(
        x->argv[2], x->p, xctx);

    /* As in impl_walk: a script function also gets index and array. */
    is_built_in = afw_value_is_function_definition(f_argv[0]);
    f_argv[4] = &array->pub;
    call = afw_value_call_create(afw_function_execute_contextual(x),
        is_built_in ? 2 : 4, &f_argv[0], false, x->p, xctx);

    /* Visit the entries there at the start, like map and filter. */
    remaining = afw_array_get_count(array->internal, xctx);
    iterator = NULL;
    index = 0;

    /*
     * Passed initialValue (even undefined, as in ECMAScript) is the first
     * accumulator. Otherwise the first entry is, and the walk starts at
     * the second; an empty array then has nothing to return.
     */
    if (x->argc >= 3) {
        AFW_FUNCTION_EVALUATE_PARAMETER(accumulator, 3);
        if (!accumulator) {
            accumulator = afw_value_undefined;
        }
    }
    else {
        if (remaining == 0) {
            AFW_THROW_ERROR_Z(argument_error,
                "reduce of an empty array with no initialValue", xctx);
        }
        accumulator = afw_value_hold_read(afw_array_get_next_value(
            array->internal, &iterator, xctx), x->p, xctx);
        remaining--;
        index++;
    }

    /*
     * Each step evaluates into its own pool, and only the current
     * accumulator is kept, by its assignable value: a step's pool and
     * the accumulator before it go when the next step is done, so memory
     * does not grow with every accumulator made (#517).
     */
    kept = NULL;
    step_p = NULL;
    next_p = NULL;
    AFW_TRY {

        for (; remaining > 0; remaining--, index++) {
            f_argv[2] = afw_array_get_next_value(array->internal,
                &iterator, xctx);
            if (!f_argv[2]) {
                break;
            }
            f_argv[1] = accumulator;
            next_p = afw_pool_create(x->p, xctx);
            if (!is_built_in) {
                f_argv[3] = afw_value_create_unmanaged_integer(
                    (afw_integer_t)index, next_p, xctx);
            }
            next = afw_value_get_assignable(
                afw_value_evaluate(call, next_p, xctx), x->p, xctx);
            afw_value_release(kept, xctx);
            kept = next;
            accumulator = kept;
            if (step_p) {
                afw_pool_release(step_p, xctx);
            }
            step_p = next_p;
            next_p = NULL;
        }

        /* Caller does not release: dest p takes the kept reference. */
        if (kept) {
            afw_pool_release_value_at_cleanup(kept, x->p, xctx);
            kept = NULL;
        }
    }
    AFW_FINALLY {
        afw_value_release(kept, xctx);
        if (next_p) {
            afw_pool_release(next_p, xctx);
        }
        if (step_p) {
            afw_pool_release(step_p, xctx);
        }
    }
    AFW_ENDTRY;

    return accumulator;
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
    afw_boolean_t result;

    result = false;
    impl_walk(x, impl_some_cb, &result);
    return result ? afw_boolean_v_true : afw_boolean_v_false;
}



/* A value to sort and where it was, so equal values keep their order. */
typedef struct {
    const afw_value_t *value;
    afw_size_t index;
} impl_sort_entry_t;

typedef struct {
    const afw_value_t *compareFunction;
    const afw_value_t *args[3];
    const impl_sort_entry_t **entries;
    afw_size_t count;
    const afw_pool_t *p;
    afw_xctx_t *xctx;
} impl_sort_ctx_t;

/* True from compareFunction means the first argument comes before the second. */
static afw_boolean_t
impl_sort_before(impl_sort_ctx_t *ctx, const void *first, const void *second)
{
    const afw_value_t *return_value;

    ctx->args[1] = first;
    ctx->args[2] = second;
    return_value = afw_value_evaluate(ctx->compareFunction,
        ctx->p, ctx->xctx);
    AFW_VALUE_ASSERT_IS_DATA_TYPE(return_value, boolean, ctx->xctx);
    return ((const afw_value_boolean_t *)return_value)->internal;
}

/*
 * Neither direction true means the two values are equal; then the one
 * that came first stays first (a stable sort, as in ECMAScript).
 */
static int
impl_sort_compare(const void *a, const void *b, void *data)
{
    impl_sort_ctx_t *ctx = data;
    const impl_sort_entry_t *ea = a;
    const impl_sort_entry_t *eb = b;

    if (ea == eb) {
        return 0;
    }
    if (ea->value != eb->value) {
        if (impl_sort_before(ctx, ea->value, eb->value)) {
            return -1;
        }
        if (impl_sort_before(ctx, eb->value, ea->value)) {
            return 1;
        }
    }
    return (ea->index < eb->index) ? -1 : 1;
}

/*
 * Adaptive function: sort
 *
 * afw_function_execute_sort
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return a new array with the same entries as array, ordered using
 * compareFunction. Entries that compare equal keep their order (a stable sort).
 * The array may hold any mix of data types, as compareFunction decides the
 * order; the result has the array's data type, if it has one. compareFunction
 * is called with two entries and must return true when the first should sort
 * before the second (boolean), not a numeric sort key.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function sort(
 *       array: array,
 *       compareFunction: (value1: any, value2: any) => boolean
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   array - (array) Array to sort.
 *
 *   compareFunction - ((value1: any, value2: any) => boolean) Return true if
 *       value1 should be ordered before value2.
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
    const afw_value_array_t *result;
    const afw_data_type_t *data_type;
    const afw_iterator_old_t *iterator;
    afw_size_t i;
    impl_sort_ctx_t ctx;
    impl_sort_entry_t *entry;

    /* Initialize sort ctx. */
    afw_memory_clear(&ctx);
    ctx.p = x->p;
    ctx.xctx = x->xctx;

    /* The array, then the compare function. */
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 1, array);
    ctx.args[0] =
        afw_function_evaluate_function_parameter(x->argv[2], ctx.p, ctx.xctx);
    ctx.compareFunction = afw_value_call_create(afw_function_execute_contextual(x),
        2, (const afw_value_t * const *)&ctx.args[0], false, ctx.p, ctx.xctx);

    /*
     * The compare function orders the values, so the array needs no data
     * type, as in ECMAScript: an empty literal [] sorts to [] and a mixed
     * array sorts by the function ("sort() requires array to be typed").
     * The result has the array's data type, or none.
     */
    data_type = afw_array_get_data_type(array->internal, ctx.xctx);
    ctx.count = afw_array_get_count(array->internal, ctx.xctx);
    if (ctx.count == 0 && data_type) {
        return data_type->empty_array_value;
    }

    /* An untyped empty array: an empty untyped result. */
    if (ctx.count == 0) {
        return afw_pool_scope_release_value_at_cleanup(
            afw_array_create_managed(NULL, ctx.p, ctx.xctx)->value,
            ctx.p, ctx.xctx);
    }

    /* Entries (value and position) and pointers to them to sort. */
    entry = afw_pool_malloc(ctx.p,
        sizeof(impl_sort_entry_t) * ctx.count, ctx.xctx);
    ctx.entries = afw_pool_malloc(ctx.p,
        sizeof(const impl_sort_entry_t *) * ctx.count, ctx.xctx);
    for (iterator = NULL, i = 0; i < ctx.count; i++) {
        /* The compare function may replace entries (the read rule). */
        entry[i].value = afw_value_hold_read(
            afw_array_get_next_value(array->internal, &iterator, ctx.xctx),
            ctx.p, ctx.xctx);
        entry[i].index = i;
        ctx.entries[i] = &entry[i];
    }

    /*
     * True from compareFunction means the first value comes first.
     * False in both directions means the values are equal.
     */
    afw_sort((const void **)ctx.entries, ctx.count,
        impl_sort_compare, &ctx);

    /* New array: create_managed RC 1. Last-release of that hold on dest p. */
    result = (const afw_value_array_t *)
        afw_pool_scope_release_value_at_cleanup(
            afw_array_create_managed(data_type, ctx.p, ctx.xctx)->value,
            ctx.p, ctx.xctx);
    for (i = 0; i < ctx.count; i++) {
        afw_array_push_value(result->internal, ctx.entries[i]->value,
            ctx.xctx);
    }
    return &result->pub;
}
