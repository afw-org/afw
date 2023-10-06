// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Authorization Default Handler
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_authorization.c
 * @brief Adaptive Framework script authorization handler
 */

#include "afw_internal.h"



/* Declares and rti/inf defines for interface afw_authorization_handler */
#define AFW_IMPLEMENTATION_ID "script"
#include "afw_authorization_handler_factory_impl_declares.h"
#include "afw_authorization_handler_impl_declares.h"


static const afw_utf8_t impl_factory_description =
AFW_UTF8_LITERAL("Authorization handler type - script.");

static const afw_authorization_handler_factory_t
impl_authorization_handler_factory_instance = {
    &impl_afw_authorization_handler_factory_inf,
    AFW_UTF8_LITERAL("script"),
    &impl_factory_description
};


/*
 * Implementation of method create_authorization_handler_cede_p of interface afw_authorization_handler_factory.
 */
const afw_authorization_handler_t *
impl_afw_authorization_handler_factory_create_authorization_handler_cede_p(
    const afw_authorization_handler_factory_t * instance,
    const afw_object_t *properties,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return afw_authorization_handler_type_script_create_cede_p(properties,
        p, xctx);
}



/* Create type=script authorization handler. */
AFW_DEFINE(const afw_authorization_handler_t *)
afw_authorization_handler_type_script_create_cede_p(
    const afw_object_t *properties,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_authorization_handler_script_self_t *self;
    const afw_authorization_handler_t *authorization_handler;
    const afw_utf8_t *detail_source_location;
    const afw_utf8_t *context_type_id;
    const afw_utf8_t *conf_object_type_id;
    const afw_object_t *context_type_object;
    const afw_object_t *qualifier_definitions;
    const afw_object_t *variable_definitions;

    authorization_handler =  afw_authorization_handler_impl_create_cede_p(
        &impl_afw_authorization_handler_inf,
        sizeof(afw_authorization_handler_script_self_t),
        properties, p, xctx);

    self = (afw_authorization_handler_script_self_t *)authorization_handler;

    /* Get and prepare qualifiers. */
    detail_source_location = afw_utf8_printf(
        self->pub.properties->p, xctx,
        AFW_UTF8_FMT "/authorizationCheck",
        AFW_UTF8_FMT_ARG(self->pub.source_location));
    self->authorization_check =
        afw_object_old_get_property_as_compiled_script(
            self->pub.properties,
            &afw_self_s_authorizationCheck,
            detail_source_location, NULL, p, xctx);

    /* Make context. */
    context_type_id = afw_utf8_printf(xctx->env->p, xctx,
        "authorizationHandler-" AFW_UTF8_FMT,
        AFW_UTF8_FMT_ARG(&self->pub.authorization_handler_id));
    conf_object_type_id = &afw_self_s__AdaptiveConf_authorizationHandler_script;
    context_type_object = afw_context_type_create(
        context_type_id, xctx->env->p, xctx);
    qualifier_definitions =
        afw_context_type_insure_qualifier_definitions_object_exists(
            context_type_object, xctx);
    afw_object_meta_add_parent_path(qualifier_definitions,
        &afw_self_s_a_context_type_application_qualifier_definitions_path,
        xctx);

    /* Configuration variables. */
    variable_definitions =
        afw_context_type_insure_variable_definitions_object_exists(
            context_type_object, &afw_self_s_authorizationHandler, xctx);
    afw_context_variable_definitions_add_based_on_object_type_id(
        variable_definitions, conf_object_type_id,
        false, xctx);

    /* Current variables. */
    variable_definitions =
        afw_context_type_insure_variable_definitions_object_exists(
            context_type_object, &afw_self_s_current, xctx);
    afw_context_variable_definition_add_z(variable_definitions,
        &afw_self_s_message, &afw_self_s_internal,
        &afw_value_evaluated_string_inf,
        "Message",
        "Unformatted message that is being logged.",
        NULL, NULL,
        xctx);

    /* qualifiedVariables definitions. */
    self->qualified_variables = afw_object_old_get_property_as_object(
        self->pub.properties, &afw_self_s_qualifiedVariables, xctx);
    if (self->qualified_variables) {
        detail_source_location = afw_utf8_printf(
            self->qualified_variables->p, xctx,
            AFW_UTF8_FMT "/" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(self->pub.source_location),
            AFW_UTF8_FMT_ARG(&afw_self_s_qualifiedVariables));
        self->qualified_variables = afw_object_create_clone(
            self->qualified_variables, p, xctx);
        afw_context_variable_definitions_compile_and_add_based_on_qualifiers_object(
            context_type_object, self->qualified_variables,
            detail_source_location, xctx);
    }

    /* Register context type. */
    afw_environment_register_context_type(context_type_id,
        context_type_object, xctx);

    /* Return new instance. */
    return authorization_handler;
}



/*
 * Implementation of method destroy of interface afw_authorization_handler.
 */
void
impl_afw_authorization_handler_destroy(
    const afw_authorization_handler_t * instance,
    afw_xctx_t *xctx)
{
    /* Assign instance pointer to self. */
    // afw_authorization_handler_script_self_t * self =
    //     (afw_authorization_handler_script_self_t *)instance;

    /* Add code, if needed. */
}




/*
 * Implementation of method check for interface afw_authorization_handler.
 */
const afw_value_t *
impl_afw_authorization_handler_check(
    const afw_authorization_handler_t * instance,
    const afw_value_t *resource_id,
    const afw_value_t *object,
    const afw_value_t *action_id,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    afw_authorization_handler_script_self_t *self =
        (afw_authorization_handler_script_self_t *)instance;
    const afw_value_t *result;
    int top;

    top = afw_xctx_qualifier_stack_top_get(xctx);
    AFW_TRY{
        if (self->qualified_variables) {
            afw_xctx_qualifier_stack_qualifiers_object_push(self->qualified_variables,
                true, xctx->p, xctx);
        }
        result = afw_value_evaluate(self->authorization_check, p, xctx);
        if (!result || afw_value_is_null(result)) {
            result = xctx->env->authorization_control->not_applicable_result;
        }
        else if (!afw_value_is_object(result)) {
            AFW_THROW_ERROR_Z(general,
                "authorizationCheck must return an "
                "_AdaptiveAuthorizationResult_ object",
                xctx);
        }
    }
    AFW_CATCH_UNHANDLED{
        AFW_ERROR_RETHROW;
    }
    AFW_FINALLY{
        afw_xctx_qualifier_stack_top_set(top, xctx);
    }
    AFW_ENDTRY;

    return result;
}


/* Internal function called to register type=script authorization handler. */
AFW_DEFINE_INTERNAL(void)
afw_authorization_internal_register_handler_type_script(
    afw_xctx_t *xctx)
{
    afw_environment_register_authorization_handler_type(&afw_self_s_script,
        &impl_authorization_handler_factory_instance, xctx);
}
