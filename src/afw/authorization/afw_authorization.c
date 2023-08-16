// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Authorization
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_authorization.c
 * @brief Functions for checking authorization.
 */

#include "afw_internal.h"


/* Declares and rti/inf defines for interface afw_authorization_handler */
#define AFW_IMPLEMENTATION_ID "authorizationHandler"
#include "afw_service_type_impl_declares.h"

static const afw_utf8_t
impl_s_a_notApplicable_policy_id =
AFW_UTF8_LITERAL("/afw/_AdaptivePolicyId_/notApplicable");

static const afw_value_string_t
impl_authorization_decision_id_action_id_control = {
    &afw_value_evaluated_string_inf,
    AFW_UTF8_LITERAL("control")
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_authorization_action_id_control =
(const afw_value_t *)& impl_authorization_decision_id_action_id_control;


static const afw_value_string_t
impl_authorization_decision_id_action_id_create = {
    &afw_value_evaluated_string_inf,
    AFW_UTF8_LITERAL("create")
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_authorization_action_id_create =
(const afw_value_t *)& impl_authorization_decision_id_action_id_create;


static const afw_value_string_t
impl_authorization_decision_id_action_id_delete = {
    &afw_value_evaluated_string_inf,
    AFW_UTF8_LITERAL("delete")
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_authorization_action_id_delete =
(const afw_value_t *)& impl_authorization_decision_id_action_id_delete;


static const afw_value_string_t
impl_authorization_decision_id_action_id_modify = {
    &afw_value_evaluated_string_inf,
    AFW_UTF8_LITERAL("modify")
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_authorization_action_id_modify =
(const afw_value_t *)& impl_authorization_decision_id_action_id_modify;


static const afw_value_string_t
impl_authorization_decision_id_action_id_query = {
    &afw_value_evaluated_string_inf,
    AFW_UTF8_LITERAL("query")
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_authorization_action_id_query =
(const afw_value_t *)& impl_authorization_decision_id_action_id_query;

static const afw_value_string_t
impl_authorization_decision_id_action_id_read = {
    &afw_value_evaluated_string_inf,
    AFW_UTF8_LITERAL("read")
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_authorization_action_id_read =
(const afw_value_t *)& impl_authorization_decision_id_action_id_read;



static const afw_value_string_t
impl_authorization_decision_id_deny_value = {
    &afw_value_evaluated_string_inf,
    AFW_UTF8_LITERAL("deny")
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_authorization_decision_id_deny_value =
(const afw_value_t *)& impl_authorization_decision_id_deny_value;


static const afw_value_string_t
impl_authorization_decision_id_indeterminate_value = {
    &afw_value_evaluated_string_inf,
    AFW_UTF8_LITERAL("indeterminate")
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_authorization_decision_id_indeterminate_value =
(const afw_value_t *)& impl_authorization_decision_id_indeterminate_value;


static const afw_value_string_t
impl_authorization_decision_id_indeterminateD_value = {
    &afw_value_evaluated_string_inf,
    AFW_UTF8_LITERAL("indeterminateD")
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_authorization_decision_id_indeterminateD_value =
(const afw_value_t *)& impl_authorization_decision_id_indeterminateD_value;


static const afw_value_string_t
impl_authorization_decision_id_indeterminateDP_value = {
    &afw_value_evaluated_string_inf,
    AFW_UTF8_LITERAL("indeterminateDP")
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_authorization_decision_id_indeterminateDP_value =
(const afw_value_t *)& impl_authorization_decision_id_indeterminateDP_value;


static const afw_value_string_t
impl_authorization_decision_id_indeterminateP_value = {
    &afw_value_evaluated_string_inf,
    AFW_UTF8_LITERAL("indeterminateP")
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_authorization_decision_id_indeterminateP_value =
(const afw_value_t *)& impl_authorization_decision_id_indeterminateP_value;


static const afw_value_string_t
impl_authorization_decision_id_notApplicable_value = {
    &afw_value_evaluated_string_inf,
    AFW_UTF8_LITERAL("notApplicable")
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_authorization_decision_id_notApplicable_value =
(const afw_value_t *)& impl_authorization_decision_id_notApplicable_value;


static const afw_value_string_t
impl_authorization_decision_id_permit_value = {
    &afw_value_evaluated_string_inf,
    AFW_UTF8_LITERAL("permit")
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_authorization_decision_id_permit_value =
(const afw_value_t *)&impl_authorization_decision_id_permit_value;



static const afw_value_string_t
impl_authorization_mode_id_core_value = {
    &afw_value_evaluated_string_inf,
    AFW_UTF8_LITERAL("core")
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_authorization_mode_id_core_value =
(const afw_value_t *)& impl_authorization_mode_id_core_value;


static const afw_value_string_t
impl_authorization_mode_id_intermediate_value = {
    &afw_value_evaluated_string_inf,
    AFW_UTF8_LITERAL("intermediate")
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_authorization_mode_id_intermediate_value =
(const afw_value_t *)& impl_authorization_mode_id_intermediate_value;


static const afw_value_string_t
impl_authorization_mode_id_user_value = {
    &afw_value_evaluated_string_inf,
    AFW_UTF8_LITERAL("user")
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_authorization_mode_id_user_value =
(const afw_value_t *)& impl_authorization_mode_id_user_value;



static const afw_value_t *
impl_requestId_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    afw_authorization_internal_authorizationCheck_context_t *ctx = entry->data;

    return ctx->requestId;
}

static const
afw_context_cb_variable_meta_t
impl_current_variable_meta_requestId =
{
    &afw_s_requestId,
    &afw_value_evaluated_string_inf,
    &afw_data_type_string_direct,
    "Request Id"
};

static const
afw_context_cb_variable_t
impl_current_variable_requestId = {
    &impl_current_variable_meta_requestId,
    "The id of the authorization check being processed.",
    impl_requestId_cb,
    1
};



static const afw_value_t *
impl_resourceId_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    afw_authorization_internal_authorizationCheck_context_t *ctx = entry->data;

    return ctx->resourceId;
}

static const
afw_context_cb_variable_meta_t
impl_current_variable_meta_resourceId =
{
    &afw_s_resourceId,
    &afw_value_evaluated_string_inf,
    &afw_data_type_string_direct,
    "Resource Id"
};

static const
afw_context_cb_variable_t
impl_current_variable_resourceId = {
    &impl_current_variable_meta_resourceId,
    "The id of the resource being accessed.",
    impl_resourceId_cb,
    1
};



static const afw_value_t *
impl_object_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    afw_authorization_internal_authorizationCheck_context_t *ctx = entry->data;

    return ctx->object;
}

static const
afw_context_cb_variable_meta_t
impl_current_variable_meta_object =
{
    &afw_s_object,
    &afw_value_evaluated_object_inf,
    &afw_data_type_object_direct,
    "Object"
};

static const
afw_context_cb_variable_t
impl_current_variable_object = {
    &impl_current_variable_meta_object,
    "The object being accessed.",
    impl_object_cb,
    1
};



static const afw_value_t *
impl_actionId_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    afw_authorization_internal_authorizationCheck_context_t *ctx = entry->data;

    return ctx->actionId;
}

static const
afw_context_cb_variable_meta_t
impl_current_variable_meta_actionId =
{
    &afw_s_actionId,
    &afw_value_evaluated_string_inf,
    &afw_data_type_string_direct,
    "Action Id"
};

static const
afw_context_cb_variable_t
impl_current_variable_actionId = {
    &impl_current_variable_meta_actionId,
    "The id of the action being performed on the resource.",
    impl_actionId_cb,
    1
};


AFW_DEFINE_INTERNAL_CONST_DATA(afw_context_cb_variable_t *)
impl_context_current_runtime_ctx[] = {
    &impl_current_variable_requestId,
    &impl_current_variable_resourceId,
    &impl_current_variable_object,
    &impl_current_variable_actionId,
    NULL
};


AFW_DEFINE_INTERNAL(afw_authorization_control_t *)
afw_authorization_internal_set_control(
    const afw_object_t *object,
    afw_xctx_t *xctx)
{
    afw_authorization_control_t *self;
    const afw_pool_t *p = xctx->env->p;
    const afw_object_t *not_applicable_object;
    const afw_array_t *list;
    afw_boolean_t found;
    afw_boolean_t deny_if_not_applicable;

    /* Can only call once. */
    if (xctx->env->authorization_control) {
        AFW_THROW_ERROR_Z(general,
            "afw_authorization_internal_set_control() already called",
            xctx);
    }

    /* Allocate self. */
    deny_if_not_applicable = false;
    self = afw_pool_calloc_type(p, afw_authorization_control_t, xctx);
    ((afw_environment_t *)xctx->env)->authorization_control = self;

    /* Set control from config object if, present. */
    if (object) {
        self->core_authorization_check =
            afw_object_old_get_property_as_compiled_hybrid(object,
                &afw_s_coreAuthorizationCheck,
                &afw_s_internal, NULL, p, xctx);

        self->initial_authorization_check =
            afw_object_old_get_property_as_compiled_hybrid(object,
                &afw_s_initialAuthorizationCheck,
                &afw_s_internal, NULL, p, xctx);

        self->check_intermediate_mode = afw_object_old_get_property_as_boolean(
            object, &afw_s_checkIntermediateMode, &found, xctx);

        deny_if_not_applicable = afw_object_old_get_property_as_boolean(
            object, &afw_s_denyIfNotApplicable, &found, xctx);
    }

    /* Set not_applicable_result */
    not_applicable_object = afw_object_create(p, xctx);
    afw_object_meta_set_object_type_id(not_applicable_object,
        &afw_s__AdaptiveAuthorizationResult_, xctx);
    afw_object_set_property(not_applicable_object, &afw_s_decisionId,
        ((deny_if_not_applicable)
            ? afw_authorization_decision_id_deny_value
            : afw_authorization_decision_id_permit_value),
        xctx);
    self->not_applicable_decision_id =
        (deny_if_not_applicable)
        ? &afw_s_deny
        : &afw_s_permit;
    list = afw_array_create_wrapper_for_array(
        &impl_s_a_notApplicable_policy_id,
        false, afw_data_type_anyURI, 1, p, xctx);
    afw_object_set_property_as_array(not_applicable_object,
        &afw_s_applicablePolicies, list, xctx);
    self->not_applicable_result = afw_value_create_object(not_applicable_object, p, xctx);

    /* Return self. */
    return self;
}


/* Perform authorization check on an object. */
AFW_DEFINE(const afw_value_t *)
afw_authorization_check(
    afw_boolean_t enforce,
    const afw_value_t *request_id_value,
    const afw_value_t *resource_id_value,
    const afw_value_t *object_value,
    const afw_value_t *action_id_value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    int top;
    const afw_value_t *result;
    const afw_value_t *result2;
    const afw_utf8_t *request_id;
    const afw_utf8_t *resource_id;
    const afw_utf8_t *action_id;
    const afw_utf8_t *decision_id;
    const afw_utf8_t *decision_id2;
    const afw_authorization_control_t *ctl;
    const afw_utf8_t *s;
    const afw_utf8_t *s2;
    const afw_object_t *obj;
    const afw_value_t *check;
    afw_authorization_internal_authorizationCheck_context_t ctx;
    const afw_authorization_handler_id_anchor_t *anchor; 
    const afw_authorization_handler_t *ah;
    const char * property_name;
    const afw_utf8_t *current_decider;
    const afw_utf8_t *final_decider;
    afw_boolean_t final_result;

    if (!request_id_value) {
        request_id_value = afw_uuid_create_string(p, xctx);
    }

    ctx.actionId = action_id_value;
    action_id = afw_value_as_string(action_id_value, xctx);
    request_id = afw_value_as_string(request_id_value, xctx);
    final_result = false;
    result = NULL;
    ctx.object = object_value;
    ctx.requestId = request_id_value;
    ctx.resourceId = resource_id_value;
    resource_id = afw_value_as_string(resource_id_value, xctx);
    current_decider = &afw_s_none;
    final_decider = &afw_s_none;

    /*
     * Bypass checks if any of the following:
     * . No authorization control
     * . No core authorization check and running in core mode
     * . Not checking intermediate mode and running in intermediate mode.
     */
    ctl = xctx->env->authorization_control;
    if (!ctl)
    {
        afw_trace_fz(1, xctx->env->flag_index_trace_authorization_check_bypass,
            NULL, xctx,
            "[Authorization check requestId " AFW_UTF8_FMT_Q
            " mode " AFW_UTF8_FMT_Q
            " actionId " AFW_UTF8_FMT_Q
            " resourceId " AFW_UTF8_FMT_Q
            "] call bypassed because there is no authorizationControl",
            AFW_UTF8_FMT_ARG(request_id),
            AFW_UTF8_FMT_ARG(&((const afw_value_string_t *)xctx->mode)->internal),
            AFW_UTF8_FMT_ARG(action_id),
            AFW_UTF8_FMT_ARG(resource_id));
        return NULL;
    }

    if (xctx->mode == afw_authorization_mode_id_core_value &&
        !ctl->core_authorization_check)
    {
        afw_trace_fz(1, xctx->env->flag_index_trace_authorization_check_bypass,
            NULL, xctx,
            "[Authorization check requestId " AFW_UTF8_FMT_Q
            " mode " AFW_UTF8_FMT_Q
            " actionId " AFW_UTF8_FMT_Q
            " resourceId " AFW_UTF8_FMT_Q
            "] call bypassed because mode is core and there is no "
            "coreAuthorizationCheck",
            AFW_UTF8_FMT_ARG(request_id),
            AFW_UTF8_FMT_ARG(&((const afw_value_string_t *)xctx->mode)->internal),
            AFW_UTF8_FMT_ARG(action_id),
            AFW_UTF8_FMT_ARG(resource_id));
        return NULL;
    }

    else if (xctx->mode == afw_authorization_mode_id_intermediate_value &&
        !ctl->check_intermediate_mode)
    {
        afw_trace_fz(1, xctx->env->flag_index_trace_authorization_check_bypass,
            NULL, xctx,
            "[Authorization check requestId " AFW_UTF8_FMT_Q
            " mode " AFW_UTF8_FMT_Q
            " actionId " AFW_UTF8_FMT_Q
            " resourceId " AFW_UTF8_FMT_Q
            "] call bypassed because mode is intermediate and "
            "checkIntermediateMode is false",
            AFW_UTF8_FMT_ARG(request_id),
            AFW_UTF8_FMT_ARG(&((const afw_value_string_t *)xctx->mode)->internal),
            AFW_UTF8_FMT_ARG(action_id),
            AFW_UTF8_FMT_ARG(resource_id));
        return NULL;
    }

    top = afw_xctx_qualifier_stack_top_get(xctx);
    ah = NULL;
    AFW_TRY{
        afw_context_push_cb_variables(&afw_s_current,
            impl_context_current_runtime_ctx, &ctx,
            p, xctx);

        afw_trace_fz(1, xctx->env->flag_index_trace_authorization_check,
            NULL, xctx,
            "[Authorization check requestId " AFW_UTF8_FMT_Q
            " mode " AFW_UTF8_FMT_Q
            " actionId " AFW_UTF8_FMT_Q
            " resourceId " AFW_UTF8_FMT_Q
            "] called",
            AFW_UTF8_FMT_ARG(request_id),
            AFW_UTF8_FMT_ARG(&((const afw_value_string_t *)xctx->mode)->internal),
            AFW_UTF8_FMT_ARG(action_id),
            AFW_UTF8_FMT_ARG(resource_id));

        if (afw_flag_is_active(
            xctx->env->flag_index_trace_authorization_check_detail, xctx))
        {
            /* Add additional tracing. */
        }

        /* Do core mode authorization check. */
        check = NULL;
        if (xctx->mode == afw_authorization_mode_id_core_value) {
            check = ctl->core_authorization_check;
            property_name = "coreAuthorizationCheck";
            current_decider = &afw_s_coreAuthorizationCheck;
            (void)current_decider; /* In catch. Avoid "not used" error. */
            final_decider = &afw_s_coreAuthorizationCheck;
        }
        else if (ctl->initial_authorization_check) {
            check = ctl->initial_authorization_check;
            property_name = "initialAuthorizationCheck";
            current_decider = &afw_s_initialAuthorizationCheck;
            (void)current_decider; /* In catch. Avoid "not used" error. */
            final_decider = &afw_s_initialAuthorizationCheck;
        }

        /* Do check. */
        if (check) {
            result = afw_value_evaluate(check, p, xctx);

            if (!result || afw_value_is_null(result)) {
                decision_id = &afw_s_notApplicable;
            }
            else {
                if (!afw_value_is_object(result)) {
                    AFW_THROW_ERROR_FZ(general, xctx,
                        "applicationControl.%s must return an "
                        "_AdaptiveAuthorizationDecision_ object", property_name);
                }
                decision_id = afw_object_old_get_property_as_string(
                    ((const afw_value_object_t *)result)->internal,
                    &afw_s_decisionId, xctx);
                if (!decision_id) {
                    AFW_THROW_ERROR_FZ(general, xctx,
                        "applicationControl.%s returned an "
                        "_AdaptiveAuthorizationDecision_ object with missing "
                        "decisionId property", property_name);
                }
            }

            if (afw_utf8_equal(decision_id, &afw_s_permit)) {
                final_result = true;
            }
            else if (afw_utf8_equal(decision_id, &afw_s_deny)) {
                final_result = true;
            }
            else if (afw_utf8_equal(decision_id, &afw_s_notApplicable)) {
                final_result = false;
            }
            else {
                AFW_THROW_ERROR_FZ(general, xctx,
                    "applicationControl.%s returned an "
                    "_AdaptiveAuthorizationDecision_ object with invalid "
                    "decisionId " AFW_UTF8_FMT_Q,
                    property_name, AFW_UTF8_FMT_ARG(decision_id));
            }
        }

        /* Check with authorization handlers. */
        if (!final_result && xctx->mode != afw_authorization_mode_id_core_value) {
            for (decision_id = &afw_s_notApplicable,
                result = NULL,
                anchor = xctx->env->authorization_control->first_anchor;
                anchor;
                anchor = anchor->next_anchor)
            {
                ah = anchor->authorization_handler;
                if (ah) {
                    current_decider = &ah->authorization_handler_id;
                    (void)current_decider; /* In catch. Avoid "not used". */
                    if (afw_utf8_equal(decision_id, &afw_s_permit)) {
                        if (!ah->allow_permit_override) {
                            afw_trace_fz(1, ah->trace_flag_index,
                                NULL, xctx,
                                "[Authorization check requestId " AFW_UTF8_FMT_Q
                                " mode " AFW_UTF8_FMT_Q
                                " actionId " AFW_UTF8_FMT_Q
                                " resourceId " AFW_UTF8_FMT_Q
                                " authorizationHandler " AFW_UTF8_FMT_Q
                                "] skipped because of a previous permit decision",
                                AFW_UTF8_FMT_ARG(request_id),
                                AFW_UTF8_FMT_ARG(
                                    &((const afw_value_string_t *)xctx->mode)->internal),
                                AFW_UTF8_FMT_ARG(action_id),
                                AFW_UTF8_FMT_ARG(resource_id),
                                AFW_UTF8_FMT_ARG(&ah->authorization_handler_id));
                            continue;
                        }
                    }
                    else if (afw_utf8_equal(decision_id, &afw_s_deny)) {
                        if (!ah->allow_deny_override) {
                            afw_trace_fz(1, ah->trace_flag_index,
                                NULL, xctx,
                                "[Authorization check requestId " AFW_UTF8_FMT_Q
                                " mode " AFW_UTF8_FMT_Q
                                " actionId " AFW_UTF8_FMT_Q
                                " resourceId " AFW_UTF8_FMT_Q
                                " authorizationHandler " AFW_UTF8_FMT_Q
                                "] skipped because of a previous deny decision",
                                AFW_UTF8_FMT_ARG(request_id),
                                AFW_UTF8_FMT_ARG(
                                    &((const afw_value_string_t *)xctx->mode)->internal),
                                AFW_UTF8_FMT_ARG(action_id),
                                AFW_UTF8_FMT_ARG(resource_id),
                                AFW_UTF8_FMT_ARG(&ah->authorization_handler_id));
                            continue;
                        }
                    }
                    result2 = afw_authorization_handler_check(ah,
                        resource_id_value, object_value, action_id_value, p, xctx);
                    if (result2) {
                        decision_id2 = afw_object_old_get_property_as_string(
                            ((const afw_value_object_t *)result2)->internal,
                            &afw_s_decisionId, xctx);
                        if (afw_utf8_equal(decision_id2, &afw_s_permit) ||
                            afw_utf8_equal(decision_id2, &afw_s_deny)) {
                            result = result2;
                            decision_id = decision_id2;
                            final_result = true;
                            final_decider = &ah->authorization_handler_id;
                        }
                        afw_trace_fz(1, ah->trace_flag_index,
                            NULL, xctx,
                            "[Authorization check requestId " AFW_UTF8_FMT_Q
                            " mode " AFW_UTF8_FMT_Q
                            " actionId " AFW_UTF8_FMT_Q
                            " resourceId " AFW_UTF8_FMT_Q
                            " authorizationHandler " AFW_UTF8_FMT_Q
                            "] handler decision is " AFW_UTF8_FMT_Q,
                            AFW_UTF8_FMT_ARG(request_id),
                            AFW_UTF8_FMT_ARG(
                                &((const afw_value_string_t *)xctx->mode)->internal),
                            AFW_UTF8_FMT_ARG(action_id),
                            AFW_UTF8_FMT_ARG(resource_id),
                            AFW_UTF8_FMT_ARG(&ah->authorization_handler_id),
                            AFW_UTF8_FMT_ARG(decision_id));

                        /** @fixme What to do about indeterminate? */
                    }
                }
            }         
        }

        /* If not applicable, permit/deny based on deny_if_not_applicable*/
        if (!final_result) {
            current_decider = &afw_s_denyIfNotApplicable;
            (void)current_decider; /* In catch. Avoid "not used" error. */
            final_decider = &afw_s_denyIfNotApplicable;
            result = ctl->not_applicable_result;
            decision_id = ctl->not_applicable_decision_id;
        }

        /* If permit at this point, handle for any obligations. */

    }
    AFW_CATCH_UNHANDLED{
        obj = afw_object_create(p, xctx);
        afw_object_meta_set_object_type_id(obj,
            &afw_s__AdaptiveAuthorizationResult_, xctx);
        afw_object_set_property(obj, &afw_s_decisionId,
            afw_authorization_decision_id_indeterminate_value,
            xctx);
        result = afw_value_create_object(obj, p, xctx);
        decision_id = &afw_s_indeterminate;
        final_decider = current_decider;

        if (afw_flag_is_active(
            xctx->env->flag_index_trace_authorization_decision, xctx))
        {
            s = afw_error_to_utf8(AFW_ERROR_THROWN, p, xctx);
            afw_trace_fz(1, xctx->env->flag_index_trace_authorization_decision_detail,
                NULL, xctx,
                "[Authorization check requestId " AFW_UTF8_FMT_Q
                " mode " AFW_UTF8_FMT_Q
                " actionId " AFW_UTF8_FMT_Q
                " resourceId " AFW_UTF8_FMT_Q
                "] detail for indeterminate produced by " AFW_UTF8_FMT_Q
                ":\n\n" AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(request_id),
                AFW_UTF8_FMT_ARG(
                    &((const afw_value_string_t *)xctx->mode)->internal),
                AFW_UTF8_FMT_ARG(action_id),
                AFW_UTF8_FMT_ARG(resource_id),
                AFW_UTF8_FMT_ARG(current_decider),
                AFW_UTF8_FMT_ARG(s));
        }
    }
    AFW_FINALLY{
        afw_xctx_qualifier_stack_top_set(top, xctx);
    }
    AFW_ENDTRY;

    afw_trace_fz(1, xctx->env->flag_index_trace_authorization_decision,
        NULL, xctx,
        "[Authorization check requestId " AFW_UTF8_FMT_Q
        " mode " AFW_UTF8_FMT_Q
        " actionId " AFW_UTF8_FMT_Q
        " resourceId " AFW_UTF8_FMT_Q
        "] final decision is " AFW_UTF8_FMT_Q
        " made by " AFW_UTF8_FMT_Q,
        AFW_UTF8_FMT_ARG(request_id),
        AFW_UTF8_FMT_ARG(
            &((const afw_value_string_t *)xctx->mode)->internal),
        AFW_UTF8_FMT_ARG(action_id),
        AFW_UTF8_FMT_ARG(resource_id),
        AFW_UTF8_FMT_ARG(decision_id),
        AFW_UTF8_FMT_ARG(final_decider));

    if (afw_flag_is_active(
        xctx->env->flag_index_trace_authorization_decision_detail, xctx))
    {
        /* Add additional tracing. */
    }

    if (enforce) {
        if (!afw_utf8_equal(decision_id, &afw_s_permit)) {
            obj = afw_object_create(p, xctx);
            afw_object_set_property(obj, &afw_s_actionId,
                action_id_value, xctx);
            afw_object_set_property(obj, &afw_s_resourceId,
                request_id_value, xctx);
            result = afw_value_create_object(obj, p, xctx);

            s = afw_value_as_string(action_id_value, xctx);
            s2 = afw_value_as_string(resource_id_value, xctx);
            AFW_THROW_ERROR_WITH_DATA_FZ(denied, result, xctx,
                "Access " AFW_UTF8_FMT_Q
                " to " AFW_UTF8_FMT_Q
                " is not permitted",
                AFW_UTF8_FMT_ARG(s), AFW_UTF8_FMT_ARG(s2));
        }
    }

    return result;
}



/* actionId */
/*FIXME
static const afw_value_t *
impl_actionId_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    impl_request_context_t *ctx = entry->data;

    if (!ctx->action_id) {
        return NULL;
    }

    if (!ctx->action_id_value.inf && ctx->action_id) {
        ctx->action_id_value.inf = &afw_value_evaluated_string_inf;
        afw_memory_copy(&ctx->action_id_value.internal, ctx->action_id);
    }
    return (const afw_value_t *)& ctx->action_id_value;
}

static const
afw_context_cb_variable_meta_t
impl_actionId_meta =
{
    &afw_s_actionId,
    &afw_value_evaluated_string_inf,
    &afw_data_type_string_direct,
    "Action Id"
};


static const
afw_context_cb_variable_t
impl_action_variable_actionId = {
    &impl_actionId_meta,
    "The actionId for authorization check.",
    impl_actionId_cb,
    1
};

static const afw_context_cb_variable_t *
impl_action_variables[] = {
    &impl_action_variable_actionId,
    NULL
};


 */


/* Internal function called to register core authorization. */
void
afw_authorization_internal_register(
    afw_xctx_t *xctx)
{
    afw_environment_t *env = (afw_environment_t *)xctx->env;

    /* Register service and conf type authorization_handler. */
    afw_authorization_internal_register_service_and_conf(xctx);

    /* Register authorization handler type script. */
    afw_authorization_internal_register_handler_type_script(xctx);

    /* Register context handlers */
    afw_context_type_register_cb_variables(
        &afw_s_authorizationCheck,
        &afw_s_current,
        "Qualified variables available during authorizationCheck processing",
        &impl_context_current_runtime_ctx[0],
        1, xctx);


    /* Register flag trace:authorization:detail */
    afw_flag_environment_register_flag(
        &afw_s_a_flag_trace_authorization_detail,
        &afw_s_a_flag_trace_authorization_detail_brief,
        &afw_s_a_flag_trace_authorization_detail_description,
        &afw_s_a_flag_trace_detail,
        xctx);

    /* Register flag trace:authorization */
    afw_flag_environment_register_flag(
        &afw_s_a_flag_trace_authorization,
        &afw_s_a_flag_trace_authorization_brief,
        &afw_s_a_flag_trace_authorization_description,
        &afw_s_a_flag_trace,
        xctx);
    afw_flag_add_included_by(
        &afw_s_a_flag_trace_authorization,
        &afw_s_a_flag_trace_authorization_detail,
        xctx);

    /* Register flag trace:authorization:check:bypass */
    afw_flag_environment_register_flag(
        &afw_s_a_flag_trace_authorization_check_bypass,
        &afw_s_a_flag_trace_authorization_check_bypass_brief,
        &afw_s_a_flag_trace_authorization_check_bypass_description,
        &afw_s_a_flag_trace_authorization,
        xctx);
    env->flag_index_trace_authorization_check_bypass =
        afw_environment_get_flag(
            &afw_s_a_flag_trace_authorization_check_bypass,
            xctx)->flag_index;

    /* Register flag trace:authorization:check:detail */
    afw_flag_environment_register_flag(
        &afw_s_a_flag_trace_authorization_check_detail,
        &afw_s_a_flag_trace_authorization_check_detail_brief,
        &afw_s_a_flag_trace_authorization_check_detail_description,
        &afw_s_a_flag_trace_authorization_detail,
        xctx);
    env->flag_index_trace_authorization_check_detail =
        afw_environment_get_flag(
            &afw_s_a_flag_trace_authorization_check_detail,
            xctx)->flag_index;

    /* Register flag trace:authorization:check */
    afw_flag_environment_register_flag(
        &afw_s_a_flag_trace_authorization_check,
        &afw_s_a_flag_trace_authorization_check_brief,
        &afw_s_a_flag_trace_authorization_check_description,
        &afw_s_a_flag_trace_authorization,
        xctx);
    env->flag_index_trace_authorization_check =
        afw_environment_get_flag(
            &afw_s_a_flag_trace_authorization_check,
            xctx)->flag_index;
    afw_flag_add_included_by(
        &afw_s_a_flag_trace_authorization_check,
        &afw_s_a_flag_trace_authorization_check_detail,
        xctx);

    /* Register flag trace:authorization:decision:detail */
    afw_flag_environment_register_flag(
        &afw_s_a_flag_trace_authorization_decision_detail,
        &afw_s_a_flag_trace_authorization_decision_detail_brief,
        &afw_s_a_flag_trace_authorization_decision_detail_description,
        &afw_s_a_flag_trace_authorization_detail,
        xctx);
    env->flag_index_trace_authorization_decision_detail =
        afw_environment_get_flag(
            &afw_s_a_flag_trace_authorization_decision_detail,
            xctx)->flag_index;

    /* Register flag trace:authorization:decision */
    afw_flag_environment_register_flag(
        &afw_s_a_flag_trace_authorization_decision,
        &afw_s_a_flag_trace_authorization_decision_brief,
        &afw_s_a_flag_trace_authorization_decision_description,
        &afw_s_a_flag_trace_authorization,
        xctx);
    env->flag_index_trace_authorization_decision =
        afw_environment_get_flag(
            &afw_s_a_flag_trace_authorization_decision,
            xctx)->flag_index;
    afw_flag_add_included_by(
        &afw_s_a_flag_trace_authorization_decision,
        &afw_s_a_flag_trace_authorization_decision_detail,
        xctx);

}


/*
 * Set instance to the active one for its id.  Instance can be NULL to stop
 * new access to this id.
 */
static void
impl_set_instance_active(
    const afw_utf8_t *authorization_handler_id,
    const afw_authorization_handler_t *authorization_handler,
    afw_xctx_t *xctx)
{
    afw_authorization_handler_id_anchor_t *anchor;
    afw_authorization_handler_id_anchor_t *stopping;
    afw_authorization_handler_id_anchor_t *prev;
    afw_authorization_handler_id_anchor_t *curr;
    afw_authorization_control_t *control;
    afw_integer_t processing_order;

    stopping = NULL;
    AFW_LOCK_WRITE_BEGIN(xctx->env->authorization_handler_id_anchor_rw_lock) {

        /*
         * Get anchor and control.  If not already registered, make a new one
         * in env->p and register it.
         */
        control = (afw_authorization_control_t * )xctx->env->authorization_control;
        if (!control) {
            control = afw_authorization_internal_set_control(NULL, xctx);
        }
        anchor = (afw_authorization_handler_id_anchor_t *)
            afw_environment_get_authorization_handler_id(
                authorization_handler_id, xctx);
        if (!anchor) {
            if (!authorization_handler) {
                AFW_THROW_ERROR_Z(general,
                    "authorization_handler can not be NULL for a new anchor",
                    xctx);
            }
            anchor = afw_pool_calloc_type(
                xctx->env->p, afw_authorization_handler_id_anchor_t, xctx);
            anchor->authorization_handler_id = afw_utf8_clone(
                authorization_handler_id,
                xctx->env->p, xctx);
            anchor->service_id = afw_utf8_clone(
                authorization_handler->service_id,
                xctx->env->p, xctx);
            afw_environment_register_authorization_handler_id(
                anchor->authorization_handler_id, anchor, xctx);
        }

        /*
         * If there is already an active instance, make a copy of active
         * anchor in the active instance's pool so it will go away when
         * instance is released put it in stopping chain.
         *
         * Removed anchor from active list.  It will be added back later
         * if there is an authorization handler.
         */
        else {
            for (prev = NULL,
                curr = (afw_authorization_handler_id_anchor_t *)
                    control->first_anchor;
                curr;
                prev = curr,
                curr = (afw_authorization_handler_id_anchor_t *)
                    curr->next_anchor)
            {
                if (afw_utf8_equal(
                    curr->authorization_handler_id, authorization_handler_id))
                {
                    if (prev) {
                        prev->next_anchor = curr->next_anchor;
                    }
                    else {
                        control->first_anchor = curr->next_anchor;
                    }
                    curr->next_anchor = NULL;
                    break;
                }
            }
            if (!authorization_handler) {
                stopping = afw_pool_calloc_type(anchor->authorization_handler->p,
                    afw_authorization_handler_id_anchor_t, xctx);
                afw_memory_copy(stopping, anchor);
                anchor->stopping = stopping;
            }
        }

        /* If starting, link into correct place in active anchors. */
        anchor->authorization_handler = authorization_handler;
        if (authorization_handler) {
            anchor->properties = authorization_handler->properties;
            anchor->reference_count = 1;
            for (prev = NULL,
                curr = (afw_authorization_handler_id_anchor_t *)
                    control->first_anchor;
                curr;
                prev = curr,
                curr = (afw_authorization_handler_id_anchor_t *)
                    curr->next_anchor)
            {
                if (curr->authorization_handler->priority >
                    anchor->authorization_handler->priority)
                {
                    anchor->next_anchor = curr;
                    if (prev) {
                        prev->next_anchor = anchor;
                    }
                    else {
                        control->first_anchor = anchor;
                    }
                    break;
                }
            }
            if (!curr) {
                if (prev) {
                    prev->next_anchor = anchor;
                }
                else {
                    control->first_anchor = anchor;
                }
            }
        }

        /* If stopping, clear some fields in anchor. */
        else {
            anchor->properties = NULL;
            anchor->reference_count = 0;
            anchor->processing_order = 0;
        }

        /* Update processing order. */
        for (processing_order = 1,
            curr = (afw_authorization_handler_id_anchor_t *)
            control->first_anchor;
            curr;
            processing_order++,
            curr = (afw_authorization_handler_id_anchor_t *)
            curr->next_anchor)
        {
            curr->processing_order = processing_order;
        }
    }

    AFW_LOCK_WRITE_END;

    /* If there was a previously active authorizationHandler, release it. */
    if (stopping) {
        afw_authorization_handler_release(
            stopping->authorization_handler, xctx);
    }
}



static const afw_authorization_handler_t *
impl_get_reference(
    const afw_utf8_t *authorization_handler_id,
    afw_xctx_t *xctx)
{
    afw_authorization_handler_id_anchor_t *anchor;
    const afw_authorization_handler_t *instance;

    AFW_LOCK_READ_BEGIN(xctx->env->authorization_handler_id_anchor_rw_lock) {

        instance = NULL;
        anchor = (afw_authorization_handler_id_anchor_t *)
            afw_environment_get_authorization_handler_id(
                authorization_handler_id, xctx);
        if (anchor) {
            instance = anchor->authorization_handler;
            if (instance) {
                anchor->reference_count++;
            }
        }
    }

    AFW_LOCK_READ_END;

    return instance;
}



/* Get reference to authorization handler */
AFW_DEFINE(const afw_authorization_handler_t *)
afw_authorization_handler_get_reference(
    const afw_utf8_t *authorization_handler_id,
    afw_xctx_t *xctx)
{
    const afw_authorization_handler_t *instance;
    const afw_utf8_t *service_id;

    instance = impl_get_reference(authorization_handler_id, xctx);

    /* If authorizationHandler is not registered, try starting it. */
    if (!instance) {
        service_id = afw_utf8_concat(xctx->p, xctx,
            &afw_s_authorizationHandler,
            &afw_s_a_dash, authorization_handler_id, NULL);
        afw_service_start(service_id, false, xctx);
        instance = impl_get_reference(authorization_handler_id, xctx);
        if (!instance) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Authorization Handler " AFW_UTF8_FMT_Q
                " is not available",
                AFW_UTF8_FMT_ARG(authorization_handler_id));
        }
    }

    return instance;
}


AFW_DEFINE(void)
afw_authorization_handler_release(
    const afw_authorization_handler_t *instance,
    afw_xctx_t *xctx)
{
    afw_authorization_handler_id_anchor_t *anchor;
    afw_authorization_handler_id_anchor_t *previous_anchor;
    afw_boolean_t destroy;

    destroy = false;
    AFW_LOCK_WRITE_BEGIN(xctx->env->authorization_handler_id_anchor_rw_lock) {

        /* Find anchor for this authorization handler instance. */
        for (
            anchor = (afw_authorization_handler_id_anchor_t *)
                afw_environment_get_authorization_handler_id(
                    &instance->authorization_handler_id, xctx),
            previous_anchor = NULL;
            anchor && anchor->authorization_handler != instance;
            previous_anchor = anchor,
            anchor = anchor->stopping);

        if (anchor) {
            anchor->reference_count--;;
            if (anchor->reference_count <= 0) {
                destroy = true;
                anchor->reference_count = 0;
                if (!previous_anchor) {
                    anchor->authorization_handler = NULL;
                }
                else {
                    previous_anchor->stopping = anchor->stopping;
                }
            }
        }
    }

    AFW_LOCK_WRITE_END;

    if (destroy) {
        afw_authorization_handler_destroy(instance, xctx);
    }
}


/* Configuration handler for entry type "authorizationHandler". */
void
impl_authorization_conf_type_create_cede_p(
    const afw_utf8_t *type,
    const afw_object_t *conf,
    const afw_utf8_t *source_location,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_authorization_handler_id_anchor_t *anchor;
    const afw_utf8_t *authorization_id;

    /* Get authorization_handler_id. */
    authorization_id = afw_object_old_get_property_as_string(conf,
        &afw_s_authorizationHandlerId, xctx);
    if (!authorization_id) {
        AFW_THROW_ERROR_FZ(general, xctx,
            AFW_UTF8_CONTEXTUAL_LABEL_FMT
            "authorizationId properties is required",
            AFW_UTF8_FMT_ARG(source_location));
    }

    /* See if authorizationHandler id already used. */
    anchor = afw_environment_get_authorization_handler_id(authorization_id, xctx);
    if (anchor) {
        AFW_THROW_ERROR_FZ(general, xctx,
            AFW_UTF8_FMT " authorizationHandlerId " AFW_UTF8_FMT_Q
            " is already running",
            source_location->len, source_location->s,
            authorization_id->len, authorization_id->s);
    }

    /* Start authorizationHandler. */
    afw_service_start_using_AdaptiveConf_cede_p(
        conf, source_location, p, xctx);
}



/* Internal function called to register core authorization. */
AFW_DEFINE_INTERNAL(void)
afw_authorization_internal_register_service_and_conf(
    afw_xctx_t *xctx)
{
    afw_service_type_t *self;

    /* Register type "authorization" configuration entry handler. */
    afw_environment_create_and_register_conf_type(
        &afw_s_authorizationHandler,
        impl_authorization_conf_type_create_cede_p,
        &afw_s_a_authorizationHandler_title,
        &afw_s_a_authorizationHandler_description,
        &afw_s_authorizationHandlerId,
        &afw_s_authorization_handler_id,
        &afw_s__AdaptiveAuthorizationHandler_,
        &afw_s_authorizationHandlerType,
        &afw_s_authorization_handler_type,
        &afw_s__AdaptiveAuthorizationHandlerType_,
        false,
        xctx);

    /* Create and register service type. */
    self = afw_xctx_calloc_type(afw_service_type_t, xctx);
    self->inf = &impl_afw_service_type_inf;
    afw_memory_copy(&self->service_type_id, &afw_s_authorizationHandler);
    self->conf_type = afw_environment_get_conf_type(
        &afw_s_authorizationHandler, xctx);
    if (!self->conf_type) {
        AFW_THROW_ERROR_Z(general, "conf_type must already be registered",
            xctx);
    }
    self->title = &afw_s_a_service_type_authorizationHandler_title;
    self->conf_type_object = afw_runtime_get_object(&afw_s__AdaptiveConfType_,
        &afw_s_authorizationHandler, xctx);
    afw_environment_register_service_type(&afw_s_authorizationHandler, self, xctx);
}



/*
 * Implementation of method related_instance_count of interface afw_service_type.
 */
afw_integer_t
impl_afw_service_type_related_instance_count (
    const afw_service_type_t * instance,
    const afw_utf8_t * id,
    afw_xctx_t *xctx)
{
    afw_authorization_handler_id_anchor_t *anchor;
    afw_authorization_handler_id_anchor_t *stopping;
    afw_integer_t result = 0;

    AFW_LOCK_READ_BEGIN(xctx->env->authorization_handler_id_anchor_rw_lock) {

        anchor = (afw_authorization_handler_id_anchor_t *)
            afw_environment_get_authorization_handler_id(id, xctx);
        if (anchor) {
            for (stopping = anchor->stopping;
                stopping;
                stopping = stopping->stopping)
            {
                result++;
            }
            if (anchor->authorization_handler) {
                result++;
            }
            else {
                result = -result;
            }
        }
    }
    AFW_LOCK_READ_END;

    return result;
}



/*
 * Implementation of method start_cede_p of interface afw_service_type.
 */
void
impl_afw_service_type_start_cede_p (
    const afw_service_type_t * instance,
    const afw_object_t * properties,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    const afw_authorization_handler_factory_t *factory;
    const afw_authorization_handler_t *authorization_handler;
    const afw_utf8_t *authorization_handler_type;

    authorization_handler_type = afw_object_old_get_property_as_utf8(
        properties, &afw_s_authorizationHandlerType, p, xctx);
    if (!authorization_handler_type) {
        AFW_THROW_ERROR_Z(general,
            "parameter authorizationHandlerType missing",
            xctx);
    }

    factory = afw_environment_get_authorization_handler_type(
        authorization_handler_type, xctx);
    if (!factory) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "authorizationHandlerType " AFW_UTF8_FMT_Q
            " is not a registered authorizationHandler type",
            AFW_UTF8_FMT_ARG(authorization_handler_type));
    }
   
    /* Create authorization handler. */
    authorization_handler =
        afw_authorization_handler_factory_create_authorization_handler_cede_p(
        factory, properties, p, xctx);

    /* Make this authorization handler the active one. */
    impl_set_instance_active(&authorization_handler->authorization_handler_id,
        authorization_handler, xctx);
}



/*
 * Implementation of method stop of interface afw_service_type.
 */
void
impl_afw_service_type_stop (
    const afw_service_type_t * instance,
    const afw_utf8_t * id,
    afw_xctx_t *xctx)
{
    impl_set_instance_active(id, NULL, xctx);
}



/*
 * Implementation of method restart_cede_p of interface afw_service_type.
 */
void
impl_afw_service_type_restart_cede_p (
    const afw_service_type_t * instance,
    const afw_object_t * properties,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    /* Count on already running. Start will restart if necessary. */
    impl_afw_service_type_start_cede_p(instance, properties, p, xctx);
}
