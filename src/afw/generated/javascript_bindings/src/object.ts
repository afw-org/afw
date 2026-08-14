// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for object
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file object.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the object category.
 */

interface IAnyObject {
    [prop : string] : any;
}

/**
 * Add the properties of one object to another replacing existing properties
 * by the same name.
 * 
 * @param {object} target - Target object or undefined if result will only
 *     contain the properties of source objects. The object must be mutable,
 *     if specified.
 * 
 * @param {object} source - Source object(s).
 * 
 * @returns {object} The resulting object.
 */
export function afwAddProperties(client : any, target : object, source : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "add_properties";
    _action["target"] = target;
    _action["source"] = source;

    return client.perform(_action);
}

/**
 * This will return an object with the specified object options applied.
 * 
 * @param {object} object - Object used to produce result.
 * 
 * @param {object} options - Object options. See
 *     /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for more information.
 * 
 * @returns {object} This is an object with object options applied.
 */
export function afwApplyObjectOptions(client : any, object : object, options? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "apply_object_options";
    _action["object"] = object;

    if (options !== undefined)
        _action["options"] = options;

    return client.perform(_action);
}

/**
 * Takes any number of object values and returns an array of array.
 * 
 * @param {array} values -
 * 
 * @returns {array}
 */
export function afwBagObject(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<object>";

    if (values !== undefined)
        _action["values"] = values;

    return client.perform(_action);
}

/**
 * This returns the integer number of values in array.
 * 
 * @param {array} value -
 * 
 * @returns {integer}
 */
export function afwBagSizeObject(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size<object>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Deep clone a object value.
 * 
 * @param {object} value - The object value to clone.
 * 
 * @returns {object} The cloned object value.
 */
export function afwCloneObject(client : any, value : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "clone<object>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Return a new array of property entries for an object. Each entry is a
 * two-element array [name, value] where name is a string. Order matches
 * keys() for the same object. The value may be undefined. The result is a
 * snapshot.
 * 
 * @param {object} object - Object to list property entries from.
 * 
 * @returns {array} Array of [name, value] pair arrays.
 */
export function afwEntries(client : any, object : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "entries";
    _action["object"] = object;

    return client.perform(_action);
}

/**
 * Determine if object arg1 is equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'eqx' ('===') instead
 * if you want false to be returned if arg1 and arg2's data type don't match.
 * 
 * @param {object} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqObject(client : any, arg1 : object, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<object>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for object arg1 is equal to the value and data type of arg2
 * then return the boolean result. Use 'eq' ('==') instead if you want arg2 to
 * be converted to the data type of arg1 before comparison.
 * 
 * @param {object} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxObject(client : any, arg1 : object, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<object>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Set a object value immutable so further mutation throws. If already
 * immutable, has no effect. Returns the same value.
 * 
 * @param {object} value - The object value to freeze.
 * 
 * @returns {object} The same value, now immutable.
 */
export function afwFreezeObject(client : any, value : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "freeze<object>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks for object arg1 is greater than or equal to object arg2 and return
 * the boolean result.
 * 
 * @param {object} arg1 -
 * 
 * @param {object} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeObject(client : any, arg1 : object, arg2 : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<object>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for object arg1 is greater than object arg2 and return the boolean
 * result.
 * 
 * @param {object} arg1 -
 * 
 * @param {object} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtObject(client : any, arg1 : object, arg2 : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<object>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType object and return the boolean result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsObject(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is<object>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Return a new array of the property names of an object, in the object's
 * property iteration order. The array is a snapshot; later changes to the
 * object do not change a previous result.
 * 
 * @param {object} object - Object to list property names from.
 * 
 * @returns {array} Array of property name strings.
 */
export function afwKeys(client : any, object : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "keys";
    _action["object"] = object;

    return client.perform(_action);
}

/**
 * Checks for object arg1 is less than or equal to object arg2 and return the
 * boolean result.
 * 
 * @param {object} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeObject(client : any, arg1 : object, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<object>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * This is used to set the ids in a local mutable object. The ids are used to
 * construct a local path.
 * 
 * @param {object} object - Object to set ids in.
 * 
 * @param {string} adapterId - Adapter id for object.
 * 
 * @param {string} objectType - Object type id for object.
 * 
 * @param {string} objectId - Object id for object.
 * 
 * @returns {void}
 */
export function afwLocalObjectMetaSetIds(client : any, object : object, objectType : string, objectId : string, adapterId? : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "local_object_meta_set_ids";
    _action["object"] = object;
    _action["objectType"] = objectType;
    _action["objectId"] = objectId;

    if (adapterId !== undefined)
        _action["adapterId"] = adapterId;

    return client.perform(_action);
}

/**
 * Checks for object arg1 is less that object arg2 and return the boolean
 * result.
 * 
 * @param {object} arg1 -
 * 
 * @param {object} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtObject(client : any, arg1 : object, arg2 : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<object>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if object arg1 is not equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use 'nex' ('!==') instead
 * if you want true to be returned if arg1 and arg2's data type don't match.
 * 
 * @param {object} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeObject(client : any, arg1 : object, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<object>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for object arg1 is not equal to the value or data type of arg2
 * then return the boolean result. Use 'ne' ('!=') instead if you want arg2 to
 * be converted to the data type of arg1 before comparison.
 * 
 * @param {object} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexObject(client : any, arg1 : object, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<object>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Converts value to data type object returning object result. A string is
 * parsed as JSON (or relaxed JSON) and must yield an object; an object is
 * left unchanged. This is not an object-literal constructor — use { ... } for
 * that.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {object} Converted value
 */
export function afwObject(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "object";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Delete a property in an object.
 * 
 * @param {object} object - This is the object to delete property from.
 * 
 * @param {string} name - This is a name of the property to delete.
 * 
 * @returns {boolean} True if object had the property and it was deleted.
 */
export function afwPropertyDelete(client : any, object : object, name : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "property_delete";
    _action["object"] = object;
    _action["name"] = name;

    return client.perform(_action);
}

/**
 * Delete a property from an object by reference.
 * 
 * @param {} reference - This is a reference to the object property to delete.
 * 
 * @returns {boolean} True if object had the property and it was deleted.
 */
export function afwPropertyDeleteByReference(client : any, reference : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "property_delete_by_reference";
    _action["reference"] = reference;

    return client.perform(_action);
}

/**
 * Return true if the named property is present on the object, including when
 * its value is undefined or null. False only when the key is missing. Use
 * is_defined / is_nullish for the value.
 * 
 * @param {object} object - Object to check.
 * 
 * @param {string} name - Property name.
 * 
 * @returns {boolean} True if the property is present.
 */
export function afwPropertyExists(client : any, object : object, name : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "property_exists";
    _action["object"] = object;
    _action["name"] = name;

    return client.perform(_action);
}

/**
 * Return the value of a property. Optional default applies only when the
 * property is missing — not when the value is undefined. If missing and no
 * default is given, the result is undefined. Object/array defaults get a
 * mutable memory face (issues #110 / #17); other defaults are cloned.
 * 
 * @param {object} object - Object to get property from.
 * 
 * @param {string} name - Property name.
 * 
 * @param {} defaultValue - Value to return only if the property is missing.
 *     Isolated when used (object/array face; otherwise clone).
 * 
 * @returns {} Property value, or default / undefined if missing.
 */
export function afwPropertyGet(client : any, object : object, name : string, defaultValue? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "property_get";
    _action["object"] = object;
    _action["name"] = name;

    if (defaultValue !== undefined)
        _action["defaultValue"] = defaultValue;

    return client.perform(_action);
}

/**
 * Return true if the named property is present and its value is not Adaptive
 * null. Undefined counts as not null. False if the property is missing or the
 * value is null. Not the same as is_defined or not is_nullish.
 * 
 * @param {object} object - Object to check.
 * 
 * @param {string} name - Property name.
 * 
 * @returns {boolean} True if present and value is not Adaptive null.
 */
export function afwPropertyIsNotNull(client : any, object : object, name : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "property_is_not_null";
    _action["object"] = object;
    _action["name"] = name;

    return client.perform(_action);
}

/**
 * Converts object value to string. For array values, the to_string() value
 * for each entry is returned separated with commas.
 * 
 * @param {object} value - A object value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringObject(client : any, value : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<object>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Return a new array of the property values of an object, in the same order
 * as keys() for that object. Values may be undefined if a property was set to
 * undefined. The array is a snapshot.
 * 
 * @param {object} object - Object to list property values from.
 * 
 * @returns {array} Array of property values.
 */
export function afwValues(client : any, object : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "values";
    _action["object"] = object;

    return client.perform(_action);
}

