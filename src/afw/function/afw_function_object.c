// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for object
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_object.c
 * @brief Adaptive function execute implementations for category `object`.
 */

#include "afw_internal.h"



/*
 * Adaptive function: add_properties
 *
 * afw_function_execute_add_properties
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Add the properties of one object to another replacing existing properties by
 * the same name.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function add_properties(
 *       target: object,
 *       source_1: object,
 *       ...source_rest: object[]
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   target - (object) Target object or undefined if result will only contain
 *       the properties of source objects. The object must be mutable, if
 *       specified.
 *
 *   source - (1 or more object) Source object(s).
 *
 * Returns:
 *
 *   (object) The resulting object.
 */
const afw_value_t *
afw_function_execute_add_properties(
    afw_function_execute_t *x)
{
    const afw_value_object_t *target;
    const afw_value_object_t *source;
    const afw_iterator_old_t *iterator;
    const afw_value_t *property_name;
    const afw_value_t *value;
    afw_size_t count;

    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(target, 1, object);
    if (!target) {
        const afw_object_t *created;

        /* New target is unmanaged in dest p. A store clones it. */
        created = afw_object_create_unmanaged(x->p, x->xctx);
        target = (const afw_value_object_t *)
            afw_object_as_value(created, x->p, x->xctx);
    }
    else {
        /* Mutate-input. */
        target = (const afw_value_object_t *)
            afw_pool_scope_get_assignable_for_scope_lifetime(
                &target->pub, x->p, x->xctx);
    }

    for (count = 2; count <= x->argc; count++)
    {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(
            source, count, object);
        for (iterator = NULL;;) {
            value = afw_object_get_next_property(
                source->internal, &iterator, &property_name, x->xctx);
            if (!value) {
                break;
            }
            afw_object_set_property(target->internal,
                property_name, value, x->xctx);
        }
    }

    return &target->pub;
}



/*
 * Adaptive function: apply_object_options
 *
 * afw_function_execute_apply_object_options
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * This will return an object with the specified object options applied.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function apply_object_options(
 *       object: object,
 *       options?: object // _AdaptiveObjectOptions_
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   object - (object) Object used to produce result.
 *
 *   options - (optional object _AdaptiveObjectOptions_) Object options. See
 *       /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for more information.
 *
 * Returns:
 *
 *   (object) This is an object with object options applied.
 */
const afw_value_t *
afw_function_execute_apply_object_options(
    afw_function_execute_t *x)
{
    const afw_value_object_t *object;
    const afw_value_object_t *options_object;
    const afw_object_options_t *options;
    const afw_object_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(object, 1, object);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(options_object, 2, object);

    options = afw_object_options_set_from_object(NULL,
        options_object->internal, x->p, x->xctx);

    result = afw_object_view_create(object->internal,
        NULL, options, x->p, x->xctx);

    return afw_value_create_unmanaged_object(result, x->p, x->xctx);
}



/*
 * Adaptive function: local_object_meta_set_ids
 *
 * afw_function_execute_local_object_meta_set_ids
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * This is used to set the ids in a local mutable object. The ids are used to
 * construct a local path.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function local_object_meta_set_ids(
 *       object: object,
 *       adapterId: string,
 *       objectType: string,
 *       objectId: string
 *   ): void;
 * ```
 *
 * Parameters:
 *
 *   object - (object) Object to set ids in.
 *
 *   adapterId - (string) Adapter id for object.
 *
 *   objectType - (string) Object type id for object.
 *
 *   objectId - (string) Object id for object.
 *
 * Returns:
 *
 *   (void)
 */
const afw_value_t *
afw_function_execute_local_object_meta_set_ids(
    afw_function_execute_t *x)
{
    const afw_value_object_t *object;
    const afw_value_string_t *adapterId;
    const afw_value_string_t *objectType;
    const afw_value_string_t *objectId;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(object, 1, object);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adapterId, 2, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectType, 3, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectId, 4, string);

    afw_object_meta_set_ids(object->internal,
        &adapterId->internal, &objectType->internal, &objectId->internal,
        x->xctx);

    /* Return void singleton. */
    return afw_value_void;
}



/*
 * Adaptive function: property_delete
 *
 * afw_function_execute_property_delete
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Set a property's value to undefined. The name stays on the object.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function property_delete(
 *       object: object,
 *       name: string
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   object - (object) This is the object.
 *
 *   name - (string) This is the property name.
 *
 * Returns:
 *
 *   (boolean) True if the object had the property. Its value is now undefined.
 */
const afw_value_t *
afw_function_execute_property_delete(
    afw_function_execute_t *x)
{
    const afw_value_object_t *object;
    const afw_value_string_t *name;
    afw_boolean_t has;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(object, 1, object);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(name, 2, string);

    /* If property exists, delete it and return true.*/
    has = afw_object_has_property(object->internal, &name->pub, x->xctx);
    if (has) {
        afw_object_set_property(object->internal,
            &name->pub, NULL, x->xctx);
    }
    return (has) ? afw_boolean_v_true : afw_boolean_v_false;
}



/*
 * Adaptive function: property_delete_by_reference
 *
 * afw_function_execute_property_delete_by_reference
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Set a referenced property's value to undefined. The name stays on the object.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function property_delete_by_reference(
 *       reference: any
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   reference - (any) This is a reference to the object property.
 *
 * Returns:
 *
 *   (boolean) True if the object had the property. Its value is now undefined.
 */
const afw_value_t *
afw_function_execute_property_delete_by_reference(
    afw_function_execute_t *x)
{
    const afw_value_t *value;
    const afw_value_reference_by_key_t *reference;
    const afw_value_t *object_value;
    const afw_value_object_t *object;
    const afw_value_t *key_value;
    const afw_value_string_t *name;
    afw_boolean_t has;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_IS(1);

    /* Arg 1 must be a reference by key value. */
    value = AFW_FUNCTION_ARGV(1);
    if (!afw_value_is_reference_by_key(value)) {
        AFW_THROW_ERROR_Z(general,
            "Expecting reference by key", x->xctx);
    }
    reference = (const afw_value_reference_by_key_t *)value;

    /* The aggregate_value must be an object. */
    object_value = afw_value_evaluate(
        reference->aggregate_value, x->p, x->xctx);
    if (!afw_value_is_object(object_value)) {
        AFW_THROW_ERROR_Z(general,
            "Expecting object reference", x->xctx);
    }
    object = (const afw_value_object_t *)object_value;

    /* The key must be a property name. */
    key_value = afw_value_evaluate(
        reference->key, x->p, x->xctx);
    if (!afw_value_is_string(key_value)) {
        AFW_THROW_ERROR_Z(general,
            "Expecting property reference", x->xctx);
    }
    name = (const afw_value_string_t *)key_value;

    /* If property exists, delete it and return true.*/
    has = afw_object_has_property(object->internal, &name->pub, x->xctx);
    if (has) {
        afw_object_set_property(object->internal,
            &name->pub, NULL, x->xctx);
    }
    return (has) ? afw_boolean_v_true : afw_boolean_v_false;
}



/*
 * Adaptive function: property_exists
 *
 * afw_function_execute_property_exists
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return true if the named property is present on the object, including when
 * its value is undefined or null. False only when the key is missing. Use
 * is_defined / is_nullish for the value.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function property_exists(
 *       object: object,
 *       name: string
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   object - (object) Object to check.
 *
 *   name - (string) Property name.
 *
 * Returns:
 *
 *   (boolean) True if the property is present.
 */
const afw_value_t *
afw_function_execute_property_exists(
    afw_function_execute_t *x)
{
    const afw_value_object_t *object;
    const afw_value_string_t *name;
    afw_boolean_t has;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(object, 1, object);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(name, 2, string);

    has = afw_object_has_property(object->internal, &name->pub, x->xctx);

    return (has) ? afw_boolean_v_true : afw_boolean_v_false;
}



/*
 * Adaptive function: property_get
 *
 * afw_function_execute_property_get
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the value of a property. Optional default applies only when the
 * property is missing — not when the value is undefined. If missing and no
 * default is given, the result is undefined. The default is the evaluated value
 * at that moment (identity).
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function property_get(
 *       object: object,
 *       name: string,
 *       defaultValue?: any
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   object - (object) Object to get property from.
 *
 *   name - (string) Property name.
 *
 *   defaultValue - (optional any) Value to return only if the property is
 *       missing. The evaluated value at that moment (identity).
 *
 * Returns:
 *
 *   (any) Property value, or default / undefined if missing.
 */
const afw_value_t *
afw_function_execute_property_get(
    afw_function_execute_t *x)
{
    const afw_value_object_t *object;
    const afw_value_string_t *name;
    const afw_value_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(object, 1, object);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(name, 2, string);

    result = afw_object_get_property(object->internal,
        &name->pub, x->xctx);
    if (!result) {
        result = afw_value_undefined;
        if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
            result = afw_value_evaluate(x->argv[3], x->p, x->xctx);
        }
    }

    return afw_value_evaluate(result, x->p, x->xctx);
}



/*
 * Adaptive function: property_is_not_null
 *
 * afw_function_execute_property_is_not_null
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return true if the named property is present and its value is not Adaptive
 * null. Undefined counts as not null. False if the property is missing or the
 * value is null. Not the same as is_defined or not is_nullish.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function property_is_not_null(
 *       object: object,
 *       name: string
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   object - (object) Object to check.
 *
 *   name - (string) Property name.
 *
 * Returns:
 *
 *   (boolean) True if present and value is not Adaptive null.
 */
const afw_value_t *
afw_function_execute_property_is_not_null(
    afw_function_execute_t *x)
{
    const afw_value_object_t *object;
    const afw_value_string_t *name;
    const afw_value_t *value;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(object, 1, object);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(name, 2, string);

    value = afw_object_get_property(object->internal, &name->pub,
        x->xctx);

    return (value && !afw_value_is_null(value))
        ? afw_boolean_v_true
        : afw_boolean_v_false;
}



/*
 * entries, keys and values: a new array of the [key, value] pairs, the
 * keys, or the values of an object's properties, or of an array's
 * entries with their integer indexes as keys (#599).
 */
typedef enum {
    impl_entries_kind_entries,
    impl_entries_kind_keys,
    impl_entries_kind_values
} impl_entries_kind_t;

static const afw_value_t *
impl_entries(
    afw_function_execute_t *x,
    impl_entries_kind_t kind)
{
    const afw_value_t *arg;
    const afw_value_array_t *result;
    const afw_array_t *pair;
    const afw_iterator_old_t *iterator;
    const afw_value_t *key;
    const afw_value_t *value;
    const afw_array_t *array;
    afw_integer_t index;

    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(arg, 1);
    array = NULL;
    if (afw_value_is_array(arg)) {
        array = ((const afw_value_array_t *)arg)->internal;
    }
    else if (!afw_value_is_object(arg)) {
        AFW_THROW_ERROR_FZ(argument_error, x->xctx,
            "Parameter 1 of function '%ku' must be an object or an array",
            &x->function->functionId->internal);
    }

    /* New array: create_managed RC 1. Last-release of that hold on dest p. */
    result = (const afw_value_array_t *)
        afw_pool_scope_release_value_at_cleanup(
            afw_array_create_managed(NULL, x->p, x->xctx)->value,
            x->p, x->xctx);
    for (iterator = NULL, index = 0;; index++) {
        if (array) {
            value = afw_array_get_next_value(array, &iterator, x->xctx);
            key = (value && kind != impl_entries_kind_values)
                ? afw_value_create_unmanaged_integer(index, x->p, x->xctx)
                : NULL;
        }
        else {
            value = afw_object_get_next_property(
                ((const afw_value_object_t *)arg)->internal,
                &iterator, &key, x->xctx);
        }
        if (!value) {
            break;
        }
        switch (kind) {
        case impl_entries_kind_entries:
            pair = ((const afw_value_array_t *)
                afw_pool_scope_release_value_at_cleanup(
                    afw_array_create_managed(NULL, x->p, x->xctx)->value,
                    x->p, x->xctx))->internal;
            afw_array_push_value(pair, key, x->xctx);
            afw_array_push_value(pair, value, x->xctx);
            afw_array_push_value(result->internal, pair->value, x->xctx);
            break;
        case impl_entries_kind_keys:
            afw_array_push_value(result->internal, key, x->xctx);
            break;
        case impl_entries_kind_values:
            afw_array_push_value(result->internal, value, x->xctx);
            break;
        }
    }

    return &result->pub;
}



/*
 * Adaptive function: entries
 *
 * afw_function_execute_entries
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return a new array of [key, value] pairs: for an object, its property names
 * and values; for an array, its indexes and entries, as ECMAScript entries
 * does. Order matches keys(). A value may be undefined. The result is a
 * snapshot.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function entries(
 *       object: any
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   object - (any) The object or array.
 *
 * Returns:
 *
 *   (array) Array of [key, value] pair arrays.
 */
const afw_value_t *
afw_function_execute_entries(
    afw_function_execute_t *x)
{
    return impl_entries(x, impl_entries_kind_entries);
}



/*
 * Adaptive function: keys
 *
 * afw_function_execute_keys
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return a new array of the keys: for an object, its property names (strings)
 * in its property iteration order; for an array, its indexes (integers), as
 * ECMAScript keys does. The result is a snapshot; later changes do not change
 * it.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function keys(
 *       object: any
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   object - (any) The object or array.
 *
 * Returns:
 *
 *   (array) Array of property name strings, or of integer indexes.
 */
const afw_value_t *
afw_function_execute_keys(
    afw_function_execute_t *x)
{
    return impl_entries(x, impl_entries_kind_keys);
}



/*
 * Adaptive function: values
 *
 * afw_function_execute_values
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return a new array of the values: for an object, its property values in the
 * same order as keys(); for an array, its entries, as ECMAScript values does. A
 * value may be undefined. The result is a snapshot.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function values(
 *       object: any
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   object - (any) The object or array.
 *
 * Returns:
 *
 *   (array) Array of values.
 */
const afw_value_t *
afw_function_execute_values(
    afw_function_execute_t *x)
{
    return impl_entries(x, impl_entries_kind_values);
}



/*
 * Adaptive function: from_entries
 *
 * afw_function_execute_from_entries
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return a new object with a property for each [key, value] pair of entries, in
 * order, as ECMAScript Object.fromEntries does; the other half of entries. Each
 * entry must be an array of exactly two entries whose first is a string (it is
 * not converted). When a key appears more than once, the last value wins, as in
 * an object literal.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function from_entries(
 *       entries: array
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   entries - (array) An array of [key, value] arrays.
 *
 * Returns:
 *
 *   (object) A new object.
 *
 * Errors thrown:
 *
 *   argument_error - an entry is not an array of two, or its key is not a
 *       string
 */
const afw_value_t *
afw_function_execute_from_entries(
    afw_function_execute_t *x)
{
    const afw_value_array_t *entries;
    const afw_object_t *result;
    const afw_iterator_old_t *iterator;
    const afw_value_t *entry;
    const afw_array_t *pair;
    const afw_value_t *key;
    const afw_value_t *value;
    afw_size_t index;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(entries, 1, array);

    /* New object: create_managed RC 1. Last-release of that hold on dest p. */
    result = afw_object_create_managed(x->p, x->xctx);
    afw_pool_scope_release_value_at_cleanup(result->value, x->p, x->xctx);

    /* Each entry a [string, value] pair; the last of a key wins. */
    for (iterator = NULL, index = 0;
        (entry = afw_array_get_next_value(entries->internal, &iterator,
            x->xctx));
        index++)
    {
        pair = afw_value_is_array(entry)
            ? ((const afw_value_array_t *)entry)->internal
            : NULL;
        if (!pair || afw_array_get_count(pair, x->xctx) != 2) {
            AFW_THROW_ERROR_FZ(argument_error, x->xctx,
                "from_entries entry " AFW_SIZE_T_FMT " must be a"
                " [key, value] array of two",
                index);
        }
        key = afw_array_get_entry_value(pair, 0, x->xctx);
        if (!key || !afw_value_is_string(key)) {
            AFW_THROW_ERROR_FZ(argument_error, x->xctx,
                "from_entries entry " AFW_SIZE_T_FMT " key must be a"
                " string (it is not converted); write to_string(v)",
                index);
        }
        value = afw_array_get_entry_value(pair, 1, x->xctx);
        afw_object_set_property(result, key, value, x->xctx);
    }

    return result->value;
}
