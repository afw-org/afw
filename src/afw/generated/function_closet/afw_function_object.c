// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for object
 *
 * Copyright (c) 2010-2024 Clemson University
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: entries
 *
 * afw_function_execute_entries
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return a new array of property entries for an object. Each entry is a
 * two-element array [name, value] where name is a string. Order matches keys()
 * for the same object. The value may be undefined. The result is a snapshot.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function entries(
 *       object: object
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   object - (object) Object to list property entries from.
 *
 * Returns:
 *
 *   (array) Array of [name, value] pair arrays.
 */
const afw_value_t *
afw_function_execute_entries(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: keys
 *
 * afw_function_execute_keys
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return a new array of the property names of an object, in the object's
 * property iteration order. The array is a snapshot; later changes to the
 * object do not change a previous result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function keys(
 *       object: object
 *   ): string[];
 * ```
 *
 * Parameters:
 *
 *   object - (object) Object to list property names from.
 *
 * Returns:
 *
 *   (string[]) Array of property name strings.
 */
const afw_value_t *
afw_function_execute_keys(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: property_delete
 *
 * afw_function_execute_property_delete
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Delete a property in an object.
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
 *   object - (object) This is the object to delete property from.
 *
 *   name - (string) This is a name of the property to delete.
 *
 * Returns:
 *
 *   (boolean) True if object had the property and it was deleted.
 */
const afw_value_t *
afw_function_execute_property_delete(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: property_delete_by_reference
 *
 * afw_function_execute_property_delete_by_reference
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Delete a property from an object by reference.
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
 *   reference - (any) This is a reference to the object property to delete.
 *
 * Returns:
 *
 *   (boolean) True if object had the property and it was deleted.
 */
const afw_value_t *
afw_function_execute_property_delete_by_reference(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
 * default is given, the result is undefined. Object/array defaults get a
 * mutable memory face (issues #110 / #17); other defaults are cloned.
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
 *       missing. Isolated when used (object/array face; otherwise clone).
 *
 * Returns:
 *
 *   (any) Property value, or default / undefined if missing.
 */
const afw_value_t *
afw_function_execute_property_get(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: values
 *
 * afw_function_execute_values
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return a new array of the property values of an object, in the same order as
 * keys() for that object. Values may be undefined if a property was set to
 * undefined. The array is a snapshot.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function values(
 *       object: object
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   object - (object) Object to list property values from.
 *
 * Returns:
 *
 *   (array) Array of property values.
 */
const afw_value_t *
afw_function_execute_values(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
