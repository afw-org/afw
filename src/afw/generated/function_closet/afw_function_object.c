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

#include "afw.h"



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
 *   function add_properties(
 *       target: object,
 *       source_1: object,
 *       ...source_rest: (list of object)
 *   ): object;
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function apply_object_options(
 *       object: object,
 *       options?: (object _AdaptiveObjectOptions_)
 *   ): object;
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function local_object_meta_set_ids(
 *       object: object,
 *       adaptorId: string,
 *       objectType: string,
 *       objectId: string
 *   ): null;
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function property_exists(
 *       object: object,
 *       name: string
 *   ): boolean;
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function property_get(
 *       object: object,
 *       name: string,
 *       defaultValue?: any
 *   ): any;
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function property_is_not_null(
 *       object: object,
 *       name: string
 *   ): boolean;
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}
