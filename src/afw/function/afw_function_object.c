// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for object
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_object.c
 * @brief afw_function_execute_* functions for object.
 */

#include "afw_internal.h"



/*
 * Adaptive function: add_properties
 *
 * afw_function_execute_add_properties
 *
 * See afw_function_bindings.h for more information.
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
 *       ...source_rest: (list of object)
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   target - (object) Target object. This object must not be immutable.
 *
 *   source - (1 or more object) Source object(s).
 *
 * Returns:
 *
 *   (object) The modified target object.
 */
const afw_value_t *
afw_function_execute_add_properties(
    afw_function_execute_t *x)
{
    const afw_value_object_t *target;
    const afw_value_object_t *source;
    const afw_iterator_t *iterator;
    const afw_utf8_t *property_name;
    const afw_value_t *value;
    afw_size_t count;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(target, 1, object);

    for (count = 2; count <= x->argc; count++)
    {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(
            source, count, object);
        for (iterator = NULL;;) {
            value = afw_object_get_next_property(
                source->internal, &iterator, &property_name, x->xctx);
            if (!value) break;
            afw_object_set_property(target->internal,
                property_name, value, x->xctx);
        }
    }

    return (const afw_value_t *)target;
}



/*
 * Adaptive function: apply_object_options
 *
 * afw_function_execute_apply_object_options
 *
 * See afw_function_bindings.h for more information.
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
 *       options?: (object _AdaptiveObjectOptions_)
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   object - (object) Object used to produce result.
 *
 *   options - (optional object _AdaptiveObjectOptions_) Object options. See
 *       /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for more
 *       information.
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

    return afw_value_create_object(result, x->p, x->xctx);
}



/*
 * Adaptive function: local_object_meta_set_ids
 *
 * afw_function_execute_local_object_meta_set_ids
 *
 * See afw_function_bindings.h for more information.
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
 *       adaptorId: string,
 *       objectType: string,
 *       objectId: string
 *   ): null;
 * ```
 *
 * Parameters:
 *
 *   object - (object) Object to set ids in.
 *
 *   adaptorId - (string) Adaptor id for object.
 *
 *   objectType - (string) Object type id for object.
 *
 *   objectId - (string) Object id for object.
 *
 * Returns:
 *
 *   (null)
 */
const afw_value_t *
afw_function_execute_local_object_meta_set_ids(
    afw_function_execute_t *x)
{
    const afw_value_object_t *object;
    const afw_value_string_t *adaptorId;
    const afw_value_string_t *objectType;
    const afw_value_string_t *objectId;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(object, 1, object);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adaptorId, 2, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectType, 3, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectId, 4, string);

    afw_object_meta_set_ids(object->internal,
        &adaptorId->internal, &objectType->internal, &objectId->internal,
        x->xctx);

    return afw_value_null;
}



/*
 * Adaptive function: property_exists
 *
 * afw_function_execute_property_exists
 *
 * See afw_function_bindings.h for more information.
 *
 * Return true if the named property exists in an object.
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
 *   object - (object) Object to get property from.
 *
 *   name - (string) Name of property to check.
 *
 * Returns:
 *
 *   (boolean) True if object has named property.
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

    has = afw_object_has_property(object->internal, &name->internal, x->xctx);

    return (has) ? afw_value_true : afw_value_false;
}



/*
 * Adaptive function: property_get
 *
 * afw_function_execute_property_get
 *
 * See afw_function_bindings.h for more information.
 *
 * Return the value of a property of an object. If property is not available,
 * return a default or null value.
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
 *   name - (string) Name of property to get.
 *
 *   defaultValue - (optional any dataType) The default value of property if it
 *       does not exist in object. If not specified, null value is the default.
 *
 * Returns:
 *
 *   (any dataType) Evaluated property value or default.
 */
const afw_value_t *
afw_function_execute_property_get(
    afw_function_execute_t *x)
{
    const afw_value_object_t *object;
    const afw_value_string_t *name;
    const afw_value_t *the_default; /* Can be null. */
    const afw_value_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(object, 1, object);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(name, 2, string);

    the_default = afw_value_null;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        the_default = afw_value_evaluate(x->argv[3], x->p, x->xctx);
    }

    result = afw_object_get_property(object->internal,
        &name->internal, x->xctx);
    if (!result) {
        result = the_default;
    }

    return afw_value_evaluate(result, x->p, x->xctx);
}



/*
 * Adaptive function: property_is_not_null
 *
 * afw_function_execute_property_is_not_null
 *
 * See afw_function_bindings.h for more information.
 *
 * Return true if the named property exists in an object and is not null.
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
 *   object - (object) Object to get property from.
 *
 *   name - (string) Name of property to check.
 *
 * Returns:
 *
 *   (boolean) True if object has named property that is not null.
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

    value = afw_object_get_property(object->internal, &name->internal,
        x->xctx);

    return (value && !afw_value_is_null(value))
        ? afw_value_true
        : afw_value_false;
}
