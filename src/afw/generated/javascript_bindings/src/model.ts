// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for model
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * This function returns a _AdaptiveAction_ object for the default action
 * performed if a onAdd parameter is not specified on the associated
 * _AdaptiveModelObjectType_. The action is not performed, but be aware that
 * functions called while producing the _AdaptiveAction_ object may cause
 * side effects. This function can be called as part of a onAdd expression or
 * for testing.
 * 
 * @param {string} adaptorId - This is the adaptorId of a model adaptor.
 *     Variable custom::adaptorId can be used to access this value in model
 *     expressions.
 * 
 * @param {string} objectType - This is the adaptive object type of object
 *     being added. Variable custom::objectType can be used to access this
 *     value in model expressions.
 * 
 * @param {object} object - This is the object to add. Variable
 *     custom::object can be used to access this value in model expressions.
 * 
 * @param {string} objectId - This is the optional preferred objectId of
 *     object to add. The adaptor may ignore this. Variable custom::objectId
 *     can be used to access this value in model expressions.
 * 
 * @param {string} modelId - This specifics a modelId of model to use for
 *     producing results. If not specified, the adaptor's current model will
 *     be used.
 * 
 * @param {object} context - This specifies additional context information
 *     available to model expressions.
 * 
 * @returns {object} The default action object.
 */
export function afwModelDefaultAddObjectAction(client : any, objectType : string, object : object, adaptorId? : string, objectId? : string, modelId? : string, context? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "model_default_add_object_action";
    _action["objectType"] = objectType;
    _action["object"] = object;

    if (adaptorId !== undefined)
        _action["adaptorId"] = adaptorId;

    if (objectId !== undefined)
        _action["objectId"] = objectId;

    if (modelId !== undefined)
        _action["modelId"] = modelId;

    if (context !== undefined)
        _action["context"] = context;

    return client.perform(_action);
}

/**
 * This function returns a _AdaptiveAction_ object for the default action
 * performed if a onDeleteObject parameter is not specified on the associated
 * _AdaptiveModelObjectType_. The action is not performed, but be aware that
 * functions called while producing the _AdaptiveAction_ object may cause
 * side effects. This function can be called as part of a onDelete expression
 * or for testing.
 * 
 * @param {string} adaptorId - This is the adaptorId of a model adaptor.
 *     Variable custom::adaptorId can be used to access this value in model
 *     expressions.
 * 
 * @param {string} objectType - This is the adaptive object type of object
 *     being deleted. Variable custom::objectType can be used to access this
 *     value in model expressions.
 * 
 * @param {string} objectId - This is the objectId of object to delete.
 *     Variable custom::object can be used to access this value in model
 *     expressions.
 * 
 * @param {string} modelId - This specifics a modelId of model to use for
 *     producing results. If not specified, the adaptor's current model will
 *     be used.
 * 
 * @param {object} context - This specifies additional context information
 *     available to model expressions.
 * 
 * @returns {object} The default action object.
 */
export function afwModelDefaultDeleteObjectAction(client : any, objectType : string, objectId : string, adaptorId? : string, modelId? : string, context? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "model_default_delete_object_action";
    _action["objectType"] = objectType;
    _action["objectId"] = objectId;

    if (adaptorId !== undefined)
        _action["adaptorId"] = adaptorId;

    if (modelId !== undefined)
        _action["modelId"] = modelId;

    if (context !== undefined)
        _action["context"] = context;

    return client.perform(_action);
}

/**
 * This function returns a _AdaptiveAction_ object for the default action
 * performed if a onModifyObject parameter is not specified on the associated
 * _AdaptiveModelObjectType_. The action is not performed, but be aware that
 * functions called while producing the _AdaptiveAction_ object may cause
 * side effects. This function can be called as part of a onModify expression
 * or for testing.
 * 
 * @param {string} adaptorId - This is the adaptorId of a model adaptor.
 *     Variable custom::adaptorId can be used to access this value in model
 *     expressions.
 * 
 * @param {string} objectType - This is the adaptive object type of object
 *     being modified. Variable custom::objectType can be used to access this
 *     value in model expressions.
 * 
 * @param {string} objectId - This is the objectId of object to modify.
 *     Variable custom::objectId can be used to access this value in model
 *     expressions.
 * 
 * @param {list} entries - This is a list of modifications. Variable
 *     custom::actions can be used to access this value in model expressions.
 *     Entries are of the form:
 *     
 *     [
 *     'add_value',
 *     'property name' or ['property name', ... ],
 *     value
 *     ]
 *     
 *     [
 *     'remove_property',
 *     'property name' or ['property name', ... ]
 *     ]
 *     
 *     [
 *     'remove_value',
 *     'property name' or ['property name', ... ],
 *     value
 *     ]
 *     
 *     [
 *     'set_property',
 *     'property name' or ['property name', ... ],
 *     value
 *     ]
 * 
 * @param {string} modelId - This specifics a modelId of model to use for
 *     producing results. If not specified, the adaptor's current model will
 *     be used.
 * 
 * @param {object} context - This specifies additional context information
 *     available to model expressions.
 * 
 * @returns {object} The default action object.
 */
export function afwModelDefaultModifyObjectAction(client : any, objectType : string, objectId : string, entries : any[], adaptorId? : string, modelId? : string, context? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "model_default_modify_object_action";
    _action["objectType"] = objectType;
    _action["objectId"] = objectId;
    _action["entries"] = entries;

    if (adaptorId !== undefined)
        _action["adaptorId"] = adaptorId;

    if (modelId !== undefined)
        _action["modelId"] = modelId;

    if (context !== undefined)
        _action["context"] = context;

    return client.perform(_action);
}

/**
 * This function returns a _AdaptiveAction_ object for the default action
 * performed if a onReplaceObject parameter is not specified on the
 * associated _AdaptiveModelObjectType_. The action is not performed, but be
 * aware that functions called while producing the _AdaptiveAction_ object
 * may cause side effects. This function can be called as part of a onReplace
 * expression or for testing.
 * 
 * @param {string} adaptorId - This is the adaptorId of a model adaptor.
 *     Variable custom::adaptorId can be used to access this value in model
 *     expressions.
 * 
 * @param {string} objectType - This is the adaptive object type of object
 *     being replaced. Variable custom::objectType can be used to access this
 *     value in model expressions.
 * 
 * @param {string} objectId - The objectId of object to replace. Variable
 *     custom::objectId can be used to access this value in model
 *     expressions.
 * 
 * @param {object} object - This is the object to replace. Variable
 *     custom::object can be used to access this value in model expressions.
 * 
 * @param {string} modelId - This specifics a modelId of model to use for
 *     producing results. If not specified, the adaptor's current model will
 *     be used.
 * 
 * @param {object} context - This specifies additional context information
 *     available to model expressions.
 * 
 * @returns {object} The default action object.
 */
export function afwModelDefaultReplaceObjectAction(client : any, objectType : string, objectId : string, object : object, adaptorId? : string, modelId? : string, context? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "model_default_replace_object_action";
    _action["objectType"] = objectType;
    _action["objectId"] = objectId;
    _action["object"] = object;

    if (adaptorId !== undefined)
        _action["adaptorId"] = adaptorId;

    if (modelId !== undefined)
        _action["modelId"] = modelId;

    if (context !== undefined)
        _action["context"] = context;

    return client.perform(_action);
}

/**
 * This is the function signature for current:mapBackObject used by
 * onGetObjects and onRetrieveObjects. Calling this directly will throw a 'Do
 * not call directly' error.
 * 
 * @param {object} mappedObject - This is the object to map back.
 * 
 * @returns {object} This is the mappedObject mapped back from mapped adaptor
 *     object.
 */
export function afwModelMapBackObjectSignature(client : any, mappedObject : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "model_mapBackObject_signature";
    _action["mappedObject"] = mappedObject;

    return client.perform(_action);
}

/**
 * This is the function signature for current:mapObject used by onAddObject.
 * onDeleteObject, onModifyObject, and onReplaceObject. Calling this directly
 * will throw a 'Do not call directly' error.
 * 
 * @param {object} object - This is the object to map.
 * 
 * @returns {object} This is the object mapped to mappedAdaptor object.
 */
export function afwModelMapObjectSignature(client : any, object : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "model_mapObject_signature";
    _action["object"] = object;

    return client.perform(_action);
}

/**
 * This is the function signature for current:returnObject used by
 * onGetObjects and onRetrieveObjects. Calling this directly will throw a 'Do
 * not call directly' error.
 * 
 * @param {object} object - This is the object to return.
 * 
 * @param {boolean} userData - If this is present and true, the object will
 *     be mapped its mapped adaptor's object type to the model adaptor's
 *     object type.
 * 
 * @returns {boolean} This will return true if no more objects can be
 *     returned for any reason including a limit exceeded, connection closed,
 *     or server stopping.
 */
export function afwModelReturnObjectSignature(client : any, object : object, userData? : boolean) : any {

    let _action : IAnyObject = {};

    _action["function"] = "model_returnObject_signature";
    _action["object"] = object;

    if (userData !== undefined)
        _action["userData"] = userData;

    return client.perform(_action);
}

