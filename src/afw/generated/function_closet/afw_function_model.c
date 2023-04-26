// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for model
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_model.c
 * @brief afw_function_execute_* functions for model.
 */

#include "afw.h"



/*
 * Adaptive function: model_default_add_object_action
 *
 * afw_function_execute_model_default_add_object_action
 *
 * See afw_function_bindings.h for more information.
 *
 * This function returns a _AdaptiveAction_ object for the default action
 * performed if a onAdd parameter is not specified on the associated
 * _AdaptiveModelObjectType_. The action is not performed, but be aware that
 * functions called while producing the _AdaptiveAction_ object may cause side
 * effects. This function can be called as part of a onAdd expression or for
 * testing.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function model_default_add_object_action(
 *       adaptorId: string,
 *       objectType: string,
 *       object: object,
 *       objectId?: string,
 *       modelId?: string,
 *       context?: (object _AdaptiveContextType_)
 *   ): (object _AdaptiveAction_);
 * ```
 *
 * Parameters:
 *
 *   adaptorId - (string) This is the adaptorId of a model adaptor. Variable
 *       custom::adaptorId can be used to access this value in model
 *       expressions.
 *
 *   objectType - (string) This is the adaptive object type of object being
 *       added. Variable custom::objectType can be used to access this value in
 *       model expressions.
 *
 *   object - (object) This is the object to add. Variable custom::object can
 *       be used to access this value in model expressions.
 *
 *   objectId - (optional string) This is the optional preferred objectId of
 *       object to add. The adaptor may ignore this. Variable custom::objectId
 *       can be used to access this value in model expressions.
 *
 *   modelId - (optional string) This specifics a modelId of model to use for
 *       producing results. If not specified, the adaptor's current model will
 *       be used.
 *
 *   context - (optional object _AdaptiveContextType_) This specifies
 *       additional context information available to model expressions.
 *
 * Returns:
 *
 *   (object _AdaptiveAction_) The default action object.
 */
const afw_value_t *
afw_function_execute_model_default_add_object_action(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Adaptive function: model_default_delete_object_action
 *
 * afw_function_execute_model_default_delete_object_action
 *
 * See afw_function_bindings.h for more information.
 *
 * This function returns a _AdaptiveAction_ object for the default action
 * performed if a onDeleteObject parameter is not specified on the associated
 * _AdaptiveModelObjectType_. The action is not performed, but be aware that
 * functions called while producing the _AdaptiveAction_ object may cause side
 * effects. This function can be called as part of a onDelete expression or for
 * testing.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function model_default_delete_object_action(
 *       adaptorId: string,
 *       objectType: string,
 *       objectId: string,
 *       modelId?: string,
 *       context?: (object _AdaptiveContextType_)
 *   ): (object _AdaptiveAction_);
 * ```
 *
 * Parameters:
 *
 *   adaptorId - (string) This is the adaptorId of a model adaptor. Variable
 *       custom::adaptorId can be used to access this value in model
 *       expressions.
 *
 *   objectType - (string) This is the adaptive object type of object being
 *       deleted. Variable custom::objectType can be used to access this value
 *       in model expressions.
 *
 *   objectId - (string) This is the objectId of object to delete. Variable
 *       custom::object can be used to access this value in model expressions.
 *
 *   modelId - (optional string) This specifics a modelId of model to use for
 *       producing results. If not specified, the adaptor's current model will
 *       be used.
 *
 *   context - (optional object _AdaptiveContextType_) This specifies
 *       additional context information available to model expressions.
 *
 * Returns:
 *
 *   (object _AdaptiveAction_) The default action object.
 */
const afw_value_t *
afw_function_execute_model_default_delete_object_action(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Adaptive function: model_default_modify_object_action
 *
 * afw_function_execute_model_default_modify_object_action
 *
 * See afw_function_bindings.h for more information.
 *
 * This function returns a _AdaptiveAction_ object for the default action
 * performed if a onModifyObject parameter is not specified on the associated
 * _AdaptiveModelObjectType_. The action is not performed, but be aware that
 * functions called while producing the _AdaptiveAction_ object may cause side
 * effects. This function can be called as part of a onModify expression or for
 * testing.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function model_default_modify_object_action(
 *       adaptorId: string,
 *       objectType: string,
 *       objectId: string,
 *       entries: list,
 *       modelId?: string,
 *       context?: (object _AdaptiveContextType_)
 *   ): (object _AdaptiveAction_);
 * ```
 *
 * Parameters:
 *
 *   adaptorId - (string) This is the adaptorId of a model adaptor. Variable
 *       custom::adaptorId can be used to access this value in model
 *       expressions.
 *
 *   objectType - (string) This is the adaptive object type of object being
 *       modified. Variable custom::objectType can be used to access this value
 *       in model expressions.
 *
 *   objectId - (string) This is the objectId of object to modify. Variable
 *       custom::objectId can be used to access this value in model
 *       expressions.
 *
 *   entries - (list) This is a list of modifications. Variable custom::actions
 *       can be used to access this value in model expressions. Entries are of
 *       the form:
 *       
 *           [
 *               "add_value",
 *               "property name" or ["property name", ... ],
 *               value
 *           ]
 *       
 *           [
 *               "remove_property",
 *               "property name" or ["property name", ... ]
 *           ]
 *       
 *           [
 *               "remove_value",
 *               "property name" or ["property name", ... ],
 *               value
 *           ]
 *       
 *           [
 *               "set_property",
 *               "property name" or ["property name", ... ],
 *               value
 *           ].
 *
 *   modelId - (optional string) This specifics a modelId of model to use for
 *       producing results. If not specified, the adaptor's current model will
 *       be used.
 *
 *   context - (optional object _AdaptiveContextType_) This specifies
 *       additional context information available to model expressions.
 *
 * Returns:
 *
 *   (object _AdaptiveAction_) The default action object.
 */
const afw_value_t *
afw_function_execute_model_default_modify_object_action(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Adaptive function: model_default_replace_object_action
 *
 * afw_function_execute_model_default_replace_object_action
 *
 * See afw_function_bindings.h for more information.
 *
 * This function returns a _AdaptiveAction_ object for the default action
 * performed if a onReplaceObject parameter is not specified on the associated
 * _AdaptiveModelObjectType_. The action is not performed, but be aware that
 * functions called while producing the _AdaptiveAction_ object may cause side
 * effects. This function can be called as part of a onReplace expression or
 * for testing.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function model_default_replace_object_action(
 *       adaptorId: string,
 *       objectType: string,
 *       objectId: string,
 *       object: object,
 *       modelId?: string,
 *       context?: (object _AdaptiveContextType_)
 *   ): (object _AdaptiveAction_);
 * ```
 *
 * Parameters:
 *
 *   adaptorId - (string) This is the adaptorId of a model adaptor. Variable
 *       custom::adaptorId can be used to access this value in model
 *       expressions.
 *
 *   objectType - (string) This is the adaptive object type of object being
 *       replaced. Variable custom::objectType can be used to access this value
 *       in model expressions.
 *
 *   objectId - (string) The objectId of object to replace. Variable
 *       custom::objectId can be used to access this value in model
 *       expressions.
 *
 *   object - (object) This is the object to replace. Variable custom::object
 *       can be used to access this value in model expressions.
 *
 *   modelId - (optional string) This specifics a modelId of model to use for
 *       producing results. If not specified, the adaptor's current model will
 *       be used.
 *
 *   context - (optional object _AdaptiveContextType_) This specifies
 *       additional context information available to model expressions.
 *
 * Returns:
 *
 *   (object _AdaptiveAction_) The default action object.
 */
const afw_value_t *
afw_function_execute_model_default_replace_object_action(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Adaptive function: model_mapBackObject_signature
 *
 * afw_function_execute_model_mapBackObject_signature
 *
 * See afw_function_bindings.h for more information.
 *
 * This is the function signature for current:mapBackObject used by
 * onGetObjects and onRetrieveObjects. Calling this directly will throw a "Do
 * not call directly" error.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function model_mapBackObject_signature(
 *       mappedObject: object
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   mappedObject - (object) This is the object to map back.
 *
 * Returns:
 *
 *   (object) This is the mappedObject mapped back from mapped adaptor object.
 */
const afw_value_t *
afw_function_execute_model_mapBackObject_signature(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Adaptive function: model_mapObject_signature
 *
 * afw_function_execute_model_mapObject_signature
 *
 * See afw_function_bindings.h for more information.
 *
 * This is the function signature for current:mapObject used by onAddObject.
 * onDeleteObject, onModifyObject, and onReplaceObject. Calling this directly
 * will throw a "Do not call directly" error.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function model_mapObject_signature(
 *       object: object
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   object - (object) This is the object to map.
 *
 * Returns:
 *
 *   (object) This is the object mapped to mappedAdaptor object.
 */
const afw_value_t *
afw_function_execute_model_mapObject_signature(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



/*
 * Adaptive function: model_returnObject_signature
 *
 * afw_function_execute_model_returnObject_signature
 *
 * See afw_function_bindings.h for more information.
 *
 * This is the function signature for current:returnObject used by onGetObjects
 * and onRetrieveObjects. Calling this directly will throw a "Do not call
 * directly" error.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function model_returnObject_signature(
 *       object: object,
 *       userData?: boolean
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   object - (object) This is the object to return.
 *
 *   userData - (optional boolean) If this is present and true, the object will
 *       be mapped its mapped adaptor's object type to the model adaptor's
 *       object type.
 *
 * Returns:
 *
 *   (boolean) This will return true if no more objects can be returned for any
 *       reason including a limit exceeded, connection closed, or server
 *       stopping.
 */
const afw_value_t *
afw_function_execute_model_returnObject_signature(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}
