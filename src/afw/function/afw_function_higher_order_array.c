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
 * any_of and all_of (XACML order): call predicate with the other values,
 * the first array among them passed one entry at a time, until it
 * returns stop_on. The ECMAScript-named functions use impl_walk.
 */
static const afw_value_t *
impl_of(
    afw_function_execute_t *x,
    afw_boolean_t stop_on)
{
    const afw_value_t **f_argv;
    const afw_value_t **entry_arg;
    const afw_value_t *call;
    const afw_value_t *result;
    const afw_array_t *array;
    const afw_iterator_old_t *iterator;
    afw_size_t f_argc;
    afw_size_t n;
    afw_size_t remaining;
    afw_size_t iterable_count;

    /* Cleared: call create may read the args (compile-time type check). */
    f_argc = x->argc - 1;
    f_argv = afw_pool_calloc(x->p, sizeof(afw_value_t *) * x->argc,
        x->xctx);
    f_argv[0] = afw_function_evaluate_function_parameter(
        x->argv[1], x->p, x->xctx);
    call = afw_value_call_create(afw_function_execute_contextual(x),
        f_argc, f_argv, false, x->p, x->xctx);

    /*
     * Evaluate argv[n+1] to f_argv[n]. Prefer a real array as the walked
     * sequence; only then, as the values parameter is iterableAsArray,
     * fall back to reading the one iterable value (utf8 code points) as
     * an array (#153, #583). Not every string: any_of(eq<string>, "b",
     * ["a", "b"]) passes "b" next to the array.
     */
    entry_arg = NULL;
    for (n = 1; n <= f_argc; n++) {
        f_argv[n] = afw_value_evaluate(x->argv[n + 1], x->p, x->xctx);
        if (!entry_arg && afw_value_is_array(f_argv[n])) {
            entry_arg = &f_argv[n];
        }
    }
    if (!entry_arg) {
        /*
         * With no array, exactly one value may be read as the array:
         * any_of(eq<string>, "b", "abc") can't tell which string to walk.
         */
        for (iterable_count = 0, n = 1; n <= f_argc; n++) {
            if (afw_value_has_iterator(f_argv[n])) {
                iterable_count++;
                entry_arg = &f_argv[n];
            }
        }
        if (iterable_count > 1) {
            AFW_THROW_ERROR_FZ(argument_error, x->xctx,
                "No value passed to %ku is an array and more than one could"
                " be read as one; write [...value] for the one to walk",
                &x->function->functionId->internal);
        }
        if (entry_arg) {
            *entry_arg = afw_value_convert_to_array_sequence(
                *entry_arg, x->p, x->xctx);
            if (!afw_value_is_array(*entry_arg)) {
                entry_arg = NULL;
            }
        }
    }
    if (!entry_arg) {
        AFW_THROW_ERROR_FZ(argument_error, x->xctx,
            "One of the values passed to %ku must be an array",
            &x->function->functionId->internal);
    }
    array = ((const afw_value_array_t *)*entry_arg)->internal;

    /* Visit the entries there at the start (see impl_walk). */
    remaining = afw_array_get_count(array, x->xctx);
    for (iterator = NULL; remaining > 0; remaining--) {
        /* The predicate may replace this entry (the read rule). */
        *entry_arg = afw_value_hold_read(afw_array_get_next_value(
            array, &iterator, x->xctx), x->p, x->xctx);
        if (!*entry_arg) {
            break;
        }
        result = afw_value_evaluate(call, x->p, x->xctx);
        AFW_VALUE_ASSERT_IS_DATA_TYPE(result, boolean, x->xctx);
        if (((const afw_value_boolean_t *)result)->internal == stop_on) {
            return stop_on ? afw_boolean_v_true : afw_boolean_v_false;
        }
    }

    return stop_on ? afw_boolean_v_false : afw_boolean_v_true;
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

    /* The first arg is the function to call, and the other two arrays. */
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

    /* Visit the entries there at the start (see impl_walk). */
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
 * find_index, some, every, find_last, find_last_index, flat_map,
 * group_by): array first, callback second (#599).
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
impl_walk_ex(
    afw_function_execute_t *x,
    impl_walk_cb_t callback,
    void *data,
    afw_boolean_t from_end)
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
    if (from_end) {
        e.index = remaining - 1;
    }
    for (iterator = NULL; remaining > 0; remaining--) {
        /*
         * The callback may replace this entry in the array; keep it for
         * the callback() that uses it after (the read rule). From the
         * end, an index a callback removed reads as undefined, as in
         * ECMAScript findLast.
         */
        if (from_end) {
            e.entry_value = afw_array_get_entry_value(array->internal,
                (afw_integer_t)e.index, e.xctx);
            if (!e.entry_value) {
                e.entry_value = afw_value_undefined;
            }
            e.entry_value = afw_value_hold_read(e.entry_value, e.p, e.xctx);
        }
        else {
            e.entry_value = afw_value_hold_read(afw_array_get_next_value(
                array->internal, &iterator, e.xctx), e.p, e.xctx);
            if (!e.entry_value) {
                break;
            }
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
        if (from_end) {
            e.index--;
        }
        else {
            e.index++;
        }
    }
}

/* Walk in index order. */
#define impl_walk(x, callback, data) \
    impl_walk_ex(x, callback, data, false)



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
 * The next entry for reduce: in index order by the iterator (NULL at the
 * end), or from the end by index, where an index removed by a callback
 * reads as undefined, as in ECMAScript reduceRight.
 */
static const afw_value_t *
impl_next_entry(
    const afw_array_t *array,
    const afw_iterator_old_t **iterator,
    afw_size_t index,
    afw_boolean_t from_end,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;

    if (!from_end) {
        return afw_array_get_next_value(array, iterator, xctx);
    }
    value = afw_array_get_entry_value(array, (afw_integer_t)index, xctx);
    return value ? value : afw_value_undefined;
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
/* reduce, and reduce_right (from_end: last entry to first). */
static const afw_value_t *
impl_reduce(
    afw_function_execute_t *x,
    afw_boolean_t from_end)
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
    index = from_end ? remaining - 1 : 0;

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
            AFW_THROW_ERROR_FZ(argument_error, xctx,
                "%ku of an empty array with no initialValue",
                &x->function->functionId->internal);
        }
        accumulator = afw_value_hold_read(
            impl_next_entry(array->internal, &iterator, index, from_end,
                xctx), x->p, xctx);
        remaining--;
        index = from_end ? index - 1 : index + 1;
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

        for (; remaining > 0; remaining--,
            index = from_end ? index - 1 : index + 1)
        {
            f_argv[2] = impl_next_entry(array->internal, &iterator, index,
                from_end, xctx);
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


const afw_value_t *
afw_function_execute_reduce(
    afw_function_execute_t *x)
{
    return impl_reduce(x, false);
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



/*
 * sort and to_sorted: a stable merge sort that compares once per
 * comparison, asking whether the entry on the right comes before the
 * one on the left; if not, the left one stays first, so equal entries
 * keep their order. (afw_sort is a quicksort, not stable, and a boolean
 * comparator needs two calls there to tell "equal" apart.)
 *
 * compareFn is called as (right, left): true, or a negative integer,
 * means right comes first, so a boolean and an ECMAScript integer
 * comparator read the same way. With no compareFn, entries are ordered
 * as < orders them (data_type is then their one data type).
 */
typedef struct {
    const afw_value_t *call;
    const afw_value_t *args[3];
    const afw_data_type_t *data_type;
    const afw_pool_t *p;
    afw_xctx_t *xctx;
} impl_sort_ctx_t;

static afw_boolean_t
impl_sort_right_first(impl_sort_ctx_t *ctx,
    const afw_value_t *left, const afw_value_t *right)
{
    const afw_value_t *result;

    /* As < orders them (impl_sorted_entries rejects NaN, #628). */
    if (!ctx->call) {
        return afw_data_type_compare_internal(ctx->data_type,
            AFW_VALUE_INTERNAL(right), AFW_VALUE_INTERNAL(left),
            ctx->xctx) < 0;
    }

    ctx->args[1] = right;
    ctx->args[2] = left;
    result = afw_value_evaluate(ctx->call, ctx->p, ctx->xctx);
    if (afw_value_is_boolean(result)) {
        return ((const afw_value_boolean_t *)result)->internal;
    }
    if (afw_value_is_integer(result)) {
        return ((const afw_value_integer_t *)result)->internal < 0;
    }
    AFW_THROW_ERROR_Z(argument_error,
        "compareFn must return a boolean or an integer", ctx->xctx);
}

/* Sort values[0..count) using work as scratch of the same size. */
static void
impl_sort_merge(impl_sort_ctx_t *ctx,
    const afw_value_t **values, const afw_value_t **work, afw_size_t count)
{
    const afw_value_t **from;
    const afw_value_t **to;
    const afw_value_t **swap;
    afw_size_t width, lo, mid, hi, i, j, k;

    from = values;
    to = work;
    for (width = 1; width < count; width *= 2) {
        for (lo = 0; lo < count; lo += 2 * width) {
            mid = (lo + width < count) ? lo + width : count;
            hi = (mid + width < count) ? mid + width : count;
            for (i = lo, j = mid, k = lo; k < hi; k++) {
                if (i < mid &&
                    (j >= hi || !impl_sort_right_first(ctx, from[i], from[j])))
                {
                    to[k] = from[i++];
                }
                else {
                    to[k] = from[j++];
                }
            }
        }
        swap = from;
        from = to;
        to = swap;
    }
    if (from != values) {
        memcpy(values, from, sizeof(const afw_value_t *) * count);
    }
}

/*
 * The entries of array, ordered by compareFn (argv[2]) or, with none,
 * as < orders them. Undefined entries go last and are not compared, as
 * in ECMAScript. Each entry is held (the read rule): compareFn may
 * replace entries of the array.
 */
static const afw_value_t **
impl_sorted_entries(afw_function_execute_t *x,
    const afw_array_t *array, afw_size_t *count)
{
    impl_sort_ctx_t ctx;
    const afw_value_t **values;
    const afw_value_t *value;
    const afw_data_type_t *data_type;
    const afw_iterator_old_t *iterator;
    afw_size_t defined;
    afw_size_t undefined;

    afw_memory_clear(&ctx);
    ctx.p = x->p;
    ctx.xctx = x->xctx;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        ctx.args[0] = afw_function_evaluate_function_parameter(
            x->argv[2], ctx.p, ctx.xctx);
        ctx.call = afw_value_call_create(afw_function_execute_contextual(x),
            2, &ctx.args[0], false, ctx.p, ctx.xctx);
    }

    *count = afw_array_get_count(array, ctx.xctx);
    if (*count == 0) {
        return NULL;
    }

    /* Defined entries first, in order; undefined ones from the end. */
    values = afw_pool_malloc(ctx.p,
        sizeof(const afw_value_t *) * *count * 2, ctx.xctx);
    for (iterator = NULL, defined = 0, undefined = 0;
        defined + undefined < *count; )
    {
        value = afw_value_hold_read(
            afw_array_get_next_value(array, &iterator, ctx.xctx),
            ctx.p, ctx.xctx);
        if (!value || afw_value_is_undefined(value)) {
            undefined++;
            values[*count - undefined] = afw_value_undefined;
            continue;
        }
        values[defined++] = value;

        /*
         * With no compareFn, every defined entry is of one data type, and
         * not NaN: NaN has no order (#628).
         */
        if (!ctx.call) {
            if (afw_value_is_double(value) && afw_number_is_NaN(
                ((const afw_value_double_t *)value)->internal))
            {
                AFW_THROW_ERROR_FZ(argument_error, ctx.xctx,
                    "%ku with no compareFn orders entries as < does, and"
                    " entry " AFW_SIZE_T_FMT " is NaN, which has no order;"
                    " pass a compareFn, or filter it out with is_NaN(v)",
                    &x->function->functionId->internal,
                    defined + undefined - 1);
            }
            data_type = afw_value_get_data_type(value, ctx.xctx);
            if (!ctx.data_type) {
                ctx.data_type = data_type;
            }
            else if (data_type != ctx.data_type) {
                AFW_THROW_ERROR_FZ(argument_error, ctx.xctx,
                    "%ku with no compareFn sorts values of one data type,"
                    " as < compares them, but this array holds %ku and %ku;"
                    " pass a compareFn to order them",
                    &x->function->functionId->internal,
                    &ctx.data_type->data_type_id,
                    &data_type->data_type_id);
            }
        }
    }

    impl_sort_merge(&ctx, values, values + *count, defined);

    return values;
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
 * 1] sorts to [1, 9, 10]. NaN has no order, so it is an error. With compareFn,
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
    const afw_value_array_t *array;
    const afw_value_t **values;
    afw_size_t count;
    afw_size_t i;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 1, array);
    /* Mutate-input: get_assignable; last-release of that hold on dest p. */
    array = (const afw_value_array_t *)
        afw_pool_scope_get_assignable_for_scope_lifetime(
            &array->pub, x->p, x->xctx);
    if (afw_array_is_immutable(array->internal, x->xctx)) {
        AFW_THROW_ERROR_Z(read_only,
            "sort of a frozen array; to_sorted makes a sorted copy",
            x->xctx);
    }

    /* Nothing is written unless every comparison succeeds. */
    values = impl_sorted_entries(x, array->internal, &count);
    for (i = 0; i < count; i++) {
        afw_array_set_value(array->internal, (afw_integer_t)i, values[i],
            x->xctx);
    }

    return &array->pub;
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
 * every entry must be of one data type, and NaN has no order, so it is an
 * error. The new array is an `array<T>` only if array is one.
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
    const afw_value_array_t *array;
    const afw_value_array_t *result;
    const afw_value_t **values;
    afw_size_t count;
    afw_size_t i;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 1, array);
    values = impl_sorted_entries(x, array->internal, &count);

    /* New array: create_managed RC 1. Last-release of that hold on dest p. */
    result = (const afw_value_array_t *)
        afw_pool_scope_release_value_at_cleanup(
            afw_array_create_managed(
                afw_array_get_declared_data_type(array->internal, x->xctx),
                x->p, x->xctx)->value,
            x->p, x->xctx);
    for (i = 0; i < count; i++) {
        afw_array_push_value(result->internal, values[i], x->xctx);
    }

    return &result->pub;
}



/*
 * Adaptive function: find_last
 *
 * afw_function_execute_find_last
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Call predicate for each entry of array, from the last to the first, until it
 * returns true, then return that entry, as ECMAScript findLast does. Entries
 * whose value is undefined are included. If no entry passes, the result is
 * undefined (the same as a found undefined entry; use find_last_index if you
 * need to tell those apart).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function find_last(
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
 *   (any) The last matching entry, or undefined if none match.
 */
const afw_value_t *
afw_function_execute_find_last(
    afw_function_execute_t *x)
{
    impl_walk_e_t found;

    afw_memory_clear(&found);
    impl_walk_ex(x, impl_find_cb, &found, true);

    return (found.entry_value) ? found.entry_value : afw_value_undefined;
}



/*
 * Adaptive function: find_last_index
 *
 * afw_function_execute_find_last_index
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Call predicate for each entry of array, from the last to the first, until it
 * returns true, then return that entry's index, as ECMAScript findLastIndex
 * does. Entries whose value is undefined are included. If no entry passes, the
 * result is -1.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function find_last_index(
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
 *   (integer) The index of the last matching entry, or -1 if none match.
 */
const afw_value_t *
afw_function_execute_find_last_index(
    afw_function_execute_t *x)
{
    impl_walk_e_t found;

    afw_memory_clear(&found);
    impl_walk_ex(x, impl_find_cb, &found, true);

    return afw_value_create_unmanaged_integer(
        found.entry_value ? (afw_integer_t)found.index : -1,
        x->p, x->xctx);
}



/* An array result adds its entries; any other result adds itself. */
static afw_boolean_t
impl_flat_map_cb(impl_walk_e_t *e)
{
    const afw_array_t *result = (const afw_array_t *)e->data;
    const afw_array_t *entries;
    const afw_iterator_old_t *iterator;
    const afw_value_t *value;

    if (!afw_value_is_array(e->entry_result)) {
        afw_array_push_value(result, e->entry_result, e->xctx);
        return true;
    }
    entries = ((const afw_value_array_t *)e->entry_result)->internal;
    for (iterator = NULL;
        (value = afw_array_get_next_value(entries, &iterator, e->xctx)); )
    {
        afw_array_push_value(result, value, e->xctx);
    }
    return true;
}

/*
 * Adaptive function: flat_map
 *
 * afw_function_execute_flat_map
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Call callbackfn once for each entry of array, in index order, and return a
 * new array of the results, with each result that is an array replaced by its
 * entries, as ECMAScript flatMap does (map then flat(1)). A result that is not
 * an array is one entry, so a callback can return zero, one or more entries.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function flat_map(
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
 *       for each entry of array; an array result adds its entries, any other
 *       result adds itself. A script function is called with (value, index,
 *       array); a built-in function is called with the value alone, since its
 *       other parameters have their own meanings.
 *
 * Returns:
 *
 *   (array) A new array of the results, one level flattened.
 */
const afw_value_t *
afw_function_execute_flat_map(
    afw_function_execute_t *x)
{
    const afw_value_array_t *result;

    /* New array: create_managed RC 1. Last-release of that hold on dest p. */
    result = (const afw_value_array_t *)
        afw_pool_scope_release_value_at_cleanup(
            afw_array_create_managed(NULL, x->p, x->xctx)->value,
            x->p, x->xctx);
    impl_walk(x, impl_flat_map_cb, (void *)result->internal);

    return &result->pub;
}



/* Add the entry to the array of its key, a string (not converted). */
static afw_boolean_t
impl_group_by_cb(impl_walk_e_t *e)
{
    const afw_object_t *result = (const afw_object_t *)e->data;
    const afw_value_t *group;
    const afw_array_t *array;
    const afw_data_type_t *data_type;

    if (!afw_value_is_string(e->entry_result)) {
        data_type = afw_value_is_undefined(e->entry_result)
            ? NULL
            : afw_value_get_data_type(e->entry_result, e->xctx);
        AFW_THROW_ERROR_FZ(argument_error, e->xctx,
            "group_by callbackfn must return a string key, but for entry "
            AFW_SIZE_T_FMT " it returned %ku; write to_string(v)",
            e->index,
            data_type ? &data_type->data_type_id : afw_s_undefined);
    }
    group = afw_object_get_property(result, e->entry_result, e->xctx);
    if (group) {
        array = ((const afw_value_array_t *)group)->internal;
    }
    else {
        /* New array: create_managed RC 1; the object takes a reference. */
        array = ((const afw_value_array_t *)
            afw_pool_scope_release_value_at_cleanup(
                afw_array_create_managed(NULL, e->p, e->xctx)->value,
                e->p, e->xctx))->internal;
        afw_object_set_property(result, e->entry_result, array->value,
            e->xctx);
    }
    afw_array_push_value(array, e->entry_value, e->xctx);
    return true;
}

/*
 * Adaptive function: group_by
 *
 * afw_function_execute_group_by
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Call callbackfn once for each entry of array, in index order, and return a
 * new object with a property for each key it returned, whose value is an array
 * of the entries that got that key, in order, as ECMAScript Object.groupBy
 * does. Properties are in the order their keys first appeared. The key must be
 * a string; it is not converted (write to_string(v) for another data type).
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function group_by(
 *       array: array,
 *       callbackfn: (value: any, index?: integer, array?: array) => string
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   array - (array) The array to walk. A string is not walked as an array:
 *       write [...value] to walk its characters.
 *
 *   callbackfn - ((value: any, index?: integer, array?: array) => string)
 *       Returns the key, a string, of the group the entry goes in. A script
 *       function is called with (value, index, array); a built-in function is
 *       called with the value alone, since its other parameters have their own
 *       meanings.
 *
 * Returns:
 *
 *   (object) A new object of arrays, one property per key.
 *
 * Errors thrown:
 *
 *   argument_error - callbackfn returned something other than a string
 */
const afw_value_t *
afw_function_execute_group_by(
    afw_function_execute_t *x)
{
    const afw_object_t *result;

    /* New object: create_managed RC 1. Last-release of that hold on dest p. */
    result = afw_object_create_managed(x->p, x->xctx);
    afw_pool_scope_release_value_at_cleanup(result->value, x->p, x->xctx);
    impl_walk(x, impl_group_by_cb, (void *)result);

    return result->value;
}



/*
 * Adaptive function: reduce_right
 *
 * afw_function_execute_reduce_right
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Call callbackfn for each entry of array, from the last to the first, with the
 * accumulator and that entry, as ECMAScript reduceRight does; each call's
 * result is the accumulator for the next call. If initialValue is passed, it is
 * the first accumulator and every entry is visited; otherwise the last entry is
 * the first accumulator and the walk starts at the one before it. Undefined
 * entries are visited. An empty array with no initialValue is an error.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function reduce_right(
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
 *       the last entry of array is.
 *
 * Returns:
 *
 *   (any) The last result of callbackfn, or the initial accumulator if
 *       callbackfn is not called.
 */
const afw_value_t *
afw_function_execute_reduce_right(
    afw_function_execute_t *x)
{
    return impl_reduce(x, true);
}
