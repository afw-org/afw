// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for object
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Add the properties of one object to another replacing existing properties
 * by the same name.
 * 
 * @param {object} target - Target object. This object must not be immutable.
 * 
 * @param {object} source - Source object(s).
 * 
 * @returns {object} The modified target object.
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
 *     /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for more
 *     information.
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
 * Takes any number of object values and returns a list of list.
 * 
 * @param {list} values -
 * 
 * @returns {list}
 */
export function afwBagObject(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<object>";

    if (values !== undefined)
        _action["values"] = values;

    return client.perform(_action);
}

/**
 * This returns the integer number of values in list.
 * 
 * @param {list} value -
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
 * Determine if object arg1 is equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use "eqx" ("===")
 * instead if you want false to be returned if arg1 and arg2's data type
 * don't match.
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
 * then return the boolean result. Use "eq" ("==") instead if you want arg2
 * to be converted to the data type of arg1 before comparison.
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
 * @param {string} adaptorId - Adaptor id for object.
 * 
 * @param {string} objectType - Object type id for object.
 * 
 * @param {string} objectId - Object id for object.
 * 
 * @returns {null}
 */
export function afwLocalObjectMetaSetIds(client : any, object : object, objectType : string, objectId : string, adaptorId? : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "local_object_meta_set_ids";
    _action["object"] = object;
    _action["objectType"] = objectType;
    _action["objectId"] = objectId;

    if (adaptorId !== undefined)
        _action["adaptorId"] = adaptorId;

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
 * Determine if object arg1 is not equal to the value of arg2 converted to
 * the data type of arg1 then return the boolean result. Use "nex" ("!==")
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
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
 * Determine if for object arg1 is not equal to the value or data type of
 * arg2 then return the boolean result. Use "ne" ("!=") instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
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
 * Converts value to data type object returning object result.
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
 * @param {} reference - This is a reference to the object property to
 *     delete.
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
 * Return true if the named property exists in an object.
 * 
 * @param {object} object - Object to get property from.
 * 
 * @param {string} name - Name of property to check.
 * 
 * @returns {boolean} True if object has named property.
 */
export function afwPropertyExists(client : any, object : object, name : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "property_exists";
    _action["object"] = object;
    _action["name"] = name;

    return client.perform(_action);
}

/**
 * Return the value of a property of an object. If property is not available,
 * return a default or null value.
 * 
 * @param {object} object - Object to get property from.
 * 
 * @param {string} name - Name of property to get.
 * 
 * @param {} defaultValue - The default value of property if it does not
 *     exist in object. If not specified, null value is the default.
 * 
 * @returns {} Evaluated property value or default.
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
 * Return true if the named property exists in an object and is not null.
 * 
 * @param {object} object - Object to get property from.
 * 
 * @param {string} name - Name of property to check.
 * 
 * @returns {boolean} True if object has named property that is not null.
 */
export function afwPropertyIsNotNull(client : any, object : object, name : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "property_is_not_null";
    _action["object"] = object;
    _action["name"] = name;

    return client.perform(_action);
}

/**
 * Converts object value to string. For list values, the to_string() value
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

