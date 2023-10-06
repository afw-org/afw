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

#include "afw_internal.h"



static const afw_value_string_t
impl_value_add_object = {
    &afw_value_evaluated_string_inf,
    AFW_UTF8_LITERAL("add_object")
};

static const afw_value_string_t
impl_value_delete_object = {
    &afw_value_evaluated_string_inf,
    AFW_UTF8_LITERAL("delete_object")
};

static const afw_value_string_t
impl_value_modify_object = {
    &afw_value_evaluated_string_inf,
    AFW_UTF8_LITERAL("modify_object")
};

static const afw_value_string_t
impl_value_replace_object = {
    &afw_value_evaluated_string_inf,
    AFW_UTF8_LITERAL("replace_object")
};


static void
impl_get_self_and_model(
    afw_model_internal_adaptor_session_self_t * *self,
    const afw_model_t * *model,
    const afw_value_string_t *adaptorId_value,
    const afw_value_string_t *modelId_value,
    afw_xctx_t *xctx)
{
    const afw_adaptor_session_t *session;

    session = afw_adaptor_session_get_cached(&adaptorId_value->internal,
        false, xctx);
    if (afw_utf8_equal_utf8_z(
        &session->inf->rti.implementation_id,
        "adaptor_impl"))
    {
        session =
            ((afw_adaptor_impl_session_t *)session)->wrapped_session;
    }
    *self = (afw_model_internal_adaptor_session_self_t *)session;
    if (!afw_utf8_equal(
        &(*self)->pub.inf->rti.implementation_id,
        &afw_self_s_model))
    {
        AFW_THROW_ERROR_FZ(general, xctx,
            AFW_UTF8_FMT_Q " is not a model adaptor",
            AFW_UTF8_FMT_ARG(&adaptorId_value->internal));
    }

    *model = (*self)->model;

    /** @fixme load different model if requested. */
    if (modelId_value) {
        AFW_THROW_ERROR_Z(general, "modelId is not implemented", xctx);
    }
}



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
 *   object - (object) This is the object to add. Variable custom::object can be
 *       used to access this value in model expressions.
 *
 *   objectId - (optional string) This is the optional preferred objectId of
 *       object to add. The adaptor may ignore this. Variable custom::objectId
 *       can be used to access this value in model expressions.
 *
 *   modelId - (optional string) This specifics a modelId of model to use for
 *       producing results. If not specified, the adaptor's current model will
 *       be used.
 *
 *   context - (optional object _AdaptiveContextType_) This specifies additional
 *       context information available to model expressions.
 *
 * Returns:
 *
 *   (object _AdaptiveAction_) The default action object.
 */
const afw_value_t *
afw_function_execute_model_default_add_object_action(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    afw_model_internal_adaptor_session_self_t *self;
    afw_model_internal_context_t *ctx;
    const afw_utf8_t *object_id;
    const afw_object_t *context;
    const afw_value_string_t *adaptorId_value;
    const afw_value_string_t *objectType_value;
    const afw_value_string_t *string_value;
    const afw_value_string_t *modelId_value;
    const afw_value_object_t *object_value;
    const afw_object_t *result;
    const afw_model_t *model;
    int top;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adaptorId_value, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectType_value, 2, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(object_value, 3, object);

    object_id = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(4)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(string_value, 4, string);
        object_id = &string_value->internal;
    }

    modelId_value = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(5)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(modelId_value, 5, string);
    }

    context = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(6)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(object_value, 6, object);
        context = object_value->internal;
    }

    /* _AdaptiveObjectType_ objects are immutable in model adaptor. */
    if (afw_utf8_equal(&objectType_value->internal, &afw_self_s__AdaptiveObjectType_)) {
        AFW_OBJECT_ERROR_OBJECT_IMMUTABLE;
    }

    /* Save top for qualifier stack. */
    top = afw_xctx_qualifier_stack_top_get(xctx);
    ctx = NULL;

    AFW_TRY {

        if (context) {
            afw_xctx_qualifier_stack_qualifiers_object_push(context, false, x->p, xctx);
        }

        /* Get session self and model. */
        impl_get_self_and_model(&self, &model, adaptorId_value, modelId_value, xctx);

        /* Prime context for "to adaptor". */
        ctx = afw_model_internal_create_to_adaptor_skeleton_context(
            self,
            &self->adaptor->instance_skeleton__AdaptiveModelCurrentOnAddObject_,
            model,
            NULL,
            &objectType_value->internal,
            object_id,
            xctx);
        ctx->object = object_value->internal;
        ctx->process_initial_values = true;

        /* Complete ctx for default add object. */
        afw_model_internal_complete_ctx_default_add_object(ctx, xctx);
    }

    /* Always restore top for qualifier stack to entry value. */
    AFW_FINALLY{
        afw_xctx_qualifier_stack_top_set(top, xctx);
    }

    AFW_ENDTRY;

    result = afw_object_create(x->p, xctx);
    afw_object_set_property(result, &afw_self_s_function,
        (const afw_value_t *)&impl_value_add_object, xctx);
    afw_object_set_property_as_string(result, &afw_self_s_adaptorId,
        self->adaptor->mapped_adaptor_id, xctx);
    afw_object_set_property_as_string(result, &afw_self_s_objectType,
        ctx->mapped_object_type_id, xctx);
    if (ctx->mapped_object_id) {
        afw_object_set_property_as_string(result, &afw_self_s_objectId,
            ctx->mapped_object_id, xctx);
    }
    afw_object_set_property_as_object(result, &afw_self_s_object,
        ctx->mapped_object, xctx);

    return afw_value_create_object(result, x->p, xctx);
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
 *   context - (optional object _AdaptiveContextType_) This specifies additional
 *       context information available to model expressions.
 *
 * Returns:
 *
 *   (object _AdaptiveAction_) The default action object.
 */
const afw_value_t *
afw_function_execute_model_default_delete_object_action(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    afw_model_internal_adaptor_session_self_t *self;
    afw_model_internal_context_t *ctx;
    const afw_object_t *context;
    const afw_value_string_t *adaptorId_value;
    const afw_value_string_t *objectType_value;
    const afw_value_string_t *objectId_value;
    const afw_value_string_t *modelId_value;
    const afw_value_object_t *context_value;
    const afw_object_t *result;
    const afw_model_t *model;
    int top;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adaptorId_value, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectType_value, 2, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectId_value, 3, string);

    modelId_value = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(5)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(modelId_value, 5, string);
    }


    context = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(5)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(context_value, 6, object);
        context = context_value->internal;
    }

    /* _AdaptiveObjectType_ objects are immutable in model adaptor. */
    if (afw_utf8_equal(&objectType_value->internal, &afw_self_s__AdaptiveObjectType_)) {
        AFW_OBJECT_ERROR_OBJECT_IMMUTABLE;
    }

    /* Save top for qualifier stack. */
    top = afw_xctx_qualifier_stack_top_get(xctx);
    ctx = NULL;

    AFW_TRY {

        if (context) {
            afw_xctx_qualifier_stack_qualifiers_object_push(context, false, x->p, xctx);
        }

        /* Get session self and model. */
        impl_get_self_and_model(&self, &model, adaptorId_value, modelId_value, xctx);

        /* Prime context for "to adaptor". */
        ctx = afw_model_internal_create_to_adaptor_skeleton_context(
            self,
            &self->adaptor->instance_skeleton__AdaptiveModelCurrentOnDeleteObject_,
            model,
            NULL,
            &objectType_value->internal,
            &objectId_value->internal,
            xctx);

        /* Complete ctx for default add object. */
        afw_model_internal_complete_ctx_default_delete_object(ctx, xctx);
    }

    /* Always restore top for qualifier stack to entry value. */
    AFW_FINALLY{
        afw_xctx_qualifier_stack_top_set(top, xctx);
    }

    AFW_ENDTRY;

    result = afw_object_create(x->p, xctx);
    afw_object_set_property(result, &afw_self_s_function,
        (const afw_value_t *)&impl_value_delete_object, xctx);
    afw_object_set_property_as_string(result, &afw_self_s_adaptorId,
        self->adaptor->mapped_adaptor_id, xctx);
    afw_object_set_property_as_string(result, &afw_self_s_objectType,
        ctx->mapped_object_type_id, xctx);
    afw_object_set_property_as_string(result, &afw_self_s_objectId,
            ctx->mapped_object_id, xctx);

    return afw_value_create_object(result, x->p, xctx);
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
 *       entries: array,
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
 *       custom::objectId can be used to access this value in model expressions.
 *
 *   entries - (array) This is an array of modifications. Variable
 *       custom::actions can be used to access this value in model expressions.
 *       Entries are of the form:
 * 
 *           [
 *               'add_value',
 *               'property name' or ['property name', ... ],
 *               value
 *           ]
 * 
 *           [
 *               'remove_property',
 *               'property name' or ['property name', ... ]
 *           ]
 * 
 *           [
 *               'remove_value',
 *               'property name' or ['property name', ... ],
 *               value
 *           ]
 * 
 *           [
 *               'set_property',
 *               'property name' or ['property name', ... ],
 *               value
 *           ].
 *
 *   modelId - (optional string) This specifics a modelId of model to use for
 *       producing results. If not specified, the adaptor's current model will
 *       be used.
 *
 *   context - (optional object _AdaptiveContextType_) This specifies additional
 *       context information available to model expressions.
 *
 * Returns:
 *
 *   (object _AdaptiveAction_) The default action object.
 */
const afw_value_t *
afw_function_execute_model_default_modify_object_action(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    afw_model_internal_adaptor_session_self_t *self;
    afw_model_internal_context_t *ctx;
    const afw_object_t *context;
    const afw_value_string_t *adaptorId_value;
    const afw_value_string_t *objectType_value;
    const afw_value_string_t *objectId_value;
    const afw_value_array_t *entries_value;
    const afw_value_string_t *modelId_value;
    const afw_value_object_t *context_value;
    const afw_object_t *result;
    const afw_model_t *model;
    int top;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adaptorId_value, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectType_value, 2, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectId_value, 3, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(entries_value, 4, array);

    modelId_value = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(5)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(modelId_value, 5, string);
    }

    context = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(6)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(context_value, 6, object);
        context = context_value->internal;
    }

    /* _AdaptiveObjectType_ objects are immutable in model adaptor. */
    if (afw_utf8_equal(&objectType_value->internal, &afw_self_s__AdaptiveObjectType_)) {
        AFW_OBJECT_ERROR_OBJECT_IMMUTABLE;
    }

    /* Save top for qualifier stack. */
    top = afw_xctx_qualifier_stack_top_get(xctx);
    ctx = NULL;

    AFW_TRY {

        if (context) {
            afw_xctx_qualifier_stack_qualifiers_object_push(context, false, x->p, xctx);
        }

        /* Get session self and model. */
        impl_get_self_and_model(&self, &model, adaptorId_value, modelId_value, xctx);

        /* Prime context for "to adaptor". */
        ctx = afw_model_internal_create_to_adaptor_skeleton_context(
            self,
            &self->adaptor->instance_skeleton__AdaptiveModelCurrentOnModifyObject_,
            model,
            NULL,
            &objectType_value->internal,
            &objectId_value->internal,
            xctx);

        /** @todo This next line was missing so do a complete review of this. */
        ctx->modify_entries_value = (const afw_value_t *)entries_value;

        /* Complete ctx for default modify object. */
        afw_model_internal_complete_ctx_default_modify_object(ctx, xctx);
    }

    /* Always restore top for qualifier stack to entry value. */
    AFW_FINALLY{
        afw_xctx_qualifier_stack_top_set(top, xctx);
    }

    AFW_ENDTRY;

    result = afw_object_create(x->p, xctx);
    afw_object_set_property(result, &afw_self_s_function,
        (const afw_value_t *)&impl_value_modify_object, xctx);
    afw_object_set_property_as_string(result, &afw_self_s_adaptorId,
        self->adaptor->mapped_adaptor_id, xctx);
    afw_object_set_property_as_string(result, &afw_self_s_objectType,
        ctx->mapped_object_type_id, xctx);
    afw_object_set_property_as_string(result, &afw_self_s_objectId,
            ctx->mapped_object_id, xctx);
    afw_object_set_property_as_array(result, &afw_self_s_entries,
            ctx->mapped_entries, xctx);

    return afw_value_create_object(result, x->p, xctx);
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
 * effects. This function can be called as part of a onReplace expression or for
 * testing.
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
 *       custom::objectId can be used to access this value in model expressions.
 *
 *   object - (object) This is the object to replace. Variable custom::object
 *       can be used to access this value in model expressions.
 *
 *   modelId - (optional string) This specifics a modelId of model to use for
 *       producing results. If not specified, the adaptor's current model will
 *       be used.
 *
 *   context - (optional object _AdaptiveContextType_) This specifies additional
 *       context information available to model expressions.
 *
 * Returns:
 *
 *   (object _AdaptiveAction_) The default action object.
 */
const afw_value_t *
afw_function_execute_model_default_replace_object_action(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    afw_model_internal_adaptor_session_self_t *self;
    afw_model_internal_context_t *ctx;
    const afw_object_t *context;
    const afw_value_string_t *adaptorId_value;
    const afw_value_string_t *objectType_value;
    const afw_value_string_t *objectId_value;
    const afw_value_object_t *object_value;
    const afw_value_string_t *modelId_value;
    const afw_value_object_t *context_value;
    const afw_object_t *result;
    const afw_model_t *model;
    int top;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adaptorId_value, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectType_value, 2, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(objectId_value, 3, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(object_value, 4, object);

    modelId_value = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(5)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(modelId_value, 5, string);
    }

    context = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(6)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(context_value, 6, object);
        context = context_value->internal;
    }

    /* _AdaptiveObjectType_ objects are immutable in model adaptor. */
    if (afw_utf8_equal(&objectType_value->internal, &afw_self_s__AdaptiveObjectType_)) {
        AFW_OBJECT_ERROR_OBJECT_IMMUTABLE;
    }

    /* Save top for qualifier stack. */
    top = afw_xctx_qualifier_stack_top_get(xctx);
    ctx = NULL;

    AFW_TRY {

        if (context) {
            afw_xctx_qualifier_stack_qualifiers_object_push(context, false, x->p, xctx);
        }

        /* Get session self and model. */
        impl_get_self_and_model(&self, &model, adaptorId_value, modelId_value, xctx);

        /* Prime context for "to adaptor". */
        ctx = afw_model_internal_create_to_adaptor_skeleton_context(
            self,
            &self->adaptor->instance_skeleton__AdaptiveModelCurrentOnReplaceObject_,
            model,
            NULL,
            &objectType_value->internal,
            &objectId_value->internal,
            xctx);

        /** @todo This next line was missing so do a complete review of this. */
        ctx->object_value = (const afw_value_t *)object_value;

        /* Complete ctx for default add object. */
        afw_model_internal_complete_ctx_default_replace_object(ctx, xctx);
    }

    /* Always restore top for qualifier stack to entry value. */
    AFW_FINALLY{
        afw_xctx_qualifier_stack_top_set(top, xctx);
    }

    AFW_ENDTRY;

    result = afw_object_create(x->p, xctx);
    afw_object_set_property(result, &afw_self_s_function,
        (const afw_value_t *)&impl_value_replace_object, xctx);
    afw_object_set_property_as_string(result, &afw_self_s_adaptorId,
        self->adaptor->mapped_adaptor_id, xctx);
    afw_object_set_property_as_string(result, &afw_self_s_objectType,
        ctx->mapped_object_type_id, xctx);
    afw_object_set_property_as_string(result, &afw_self_s_objectId,
            ctx->mapped_object_id, xctx);
    afw_object_set_property_as_array(result, &afw_self_s_entries,
            ctx->mapped_entries, xctx);

    return afw_value_create_object(result, x->p, xctx);
}



/*
 * Adaptive function: model_mapBackObject_signature
 *
 * afw_function_execute_model_mapBackObject_signature
 *
 * See afw_function_bindings.h for more information.
 *
 * This is the function signature for current:mapBackObject used by onGetObjects
 * and onRetrieveObjects. Calling this directly will throw a 'Do not call
 * directly' error.
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
    AFW_THROW_ERROR_Z(general, "Do not call directly", x->xctx);
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
 * will throw a 'Do not call directly' error.
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
    AFW_THROW_ERROR_Z(general, "Do not call directly", x->xctx);
}



/*
 * Adaptive function: model_returnObject_signature
 *
 * afw_function_execute_model_returnObject_signature
 *
 * See afw_function_bindings.h for more information.
 *
 * This is the function signature for current:returnObject used by onGetObjects
 * and onRetrieveObjects. Calling this directly will throw a 'Do not call
 * directly' error.
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
    AFW_THROW_ERROR_Z(general, "Do not call directly", x->xctx);
}
