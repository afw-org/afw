// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Authorization Implementation Support
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_authorization_impl.c
 * @brief Functions used by implementations of authorization handlers.
 */

#include "afw_internal.h"


static void
impl_set_trace_flag_fields(
    afw_authorization_handler_t *self,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p = self->p;
    const afw_pool_t *env_p = xctx->env->p;
    const afw_utf8_t *flag_id;
    const afw_utf8_t *brief;
    const afw_utf8_t *description;
    const afw_utf8_t *included_by_flag_id;
    const afw_flag_t *flag;

    /* authorization_handler_id_trace_flag_id */
    self->trace_flag_id =
        afw_utf8_printf(p, xctx,
        "trace:authorizationHandlerId:" AFW_UTF8_FMT,
        AFW_UTF8_FMT_ARG(&self->authorization_handler_id));

    /* authorization_handler_id_detail_flag_id */
    self->detail_flag_id =
        afw_utf8_printf(p, xctx,
        "trace:authorizationHandlerId:" AFW_UTF8_FMT ":detail",
        AFW_UTF8_FMT_ARG(&self->authorization_handler_id));

    /* authorization_handler_id_detail_flag_index */
    flag = afw_environment_get_flag(
        self ->detail_flag_id, xctx);
    if (!flag) {
        flag_id = afw_utf8_clone(
            self ->detail_flag_id,
            env_p, xctx);

        brief = afw_utf8_printf(env_p, xctx,
            "Detail trace of authorizationHandlerId " AFW_UTF8_FMT_Q,
            AFW_UTF8_FMT_ARG(&self->authorization_handler_id));

        description = afw_utf8_printf(env_p, xctx,
            "This produces a basic plus detail trace "
            "of authorizationHandlerId " AFW_UTF8_FMT_Q ".",
            AFW_UTF8_FMT_ARG(&self->authorization_handler_id));

        included_by_flag_id = afw_utf8_printf(env_p, xctx,
            "trace:authorizationHandlerType:" AFW_UTF8_FMT ":detail",
            AFW_UTF8_FMT_ARG(self->authorization_handler_type_id));

        afw_environment_register_flag(flag_id, brief, description,
            included_by_flag_id, xctx);

        flag = afw_environment_get_flag(
            self->detail_flag_id, xctx);
    }
    self->detail_flag_index = flag->flag_index;


    /* authorization_handler_id_trace_flag_index */
    flag = afw_environment_get_flag(
        self->trace_flag_id, xctx);
    if (!flag) {
        flag_id = afw_utf8_clone(
            self->trace_flag_id,
            env_p, xctx);

        brief = afw_utf8_printf(env_p, xctx,
            "Trace authorizationHandlerId " AFW_UTF8_FMT_Q,
            AFW_UTF8_FMT_ARG(&self->authorization_handler_id));

        description = afw_utf8_printf(env_p, xctx,
            "This produces a basic trace of authorizationHandlerId " AFW_UTF8_FMT_Q ".",
            AFW_UTF8_FMT_ARG(&self->authorization_handler_id));

        included_by_flag_id = afw_utf8_printf(env_p, xctx,
            "trace:authorizationHandlerType:" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(self->authorization_handler_type_id));

        afw_environment_register_flag(flag_id, brief, description,
            included_by_flag_id, xctx);

        afw_flag_add_included_by(self->trace_flag_id,
            self->detail_flag_id, xctx);

        flag = afw_environment_get_flag(
            self->trace_flag_id, xctx);
    }
    self->trace_flag_index = flag->flag_index;
}



AFW_DEFINE(afw_authorization_handler_t *)
afw_authorization_handler_impl_create_cede_p(
    const afw_authorization_handler_inf_t *inf,
    afw_size_t instance_size,
    const afw_object_t *properties,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_authorization_handler_t *self;
    const afw_utf8_t *s;
    afw_boolean_t found;

    self = (afw_authorization_handler_t *)afw_pool_calloc(p,
        (instance_size != 0)
            ? instance_size
            : sizeof(afw_authorization_handler_t),
        xctx);
    self->inf = inf;
    self->p = p;

    /* Prepare properties. */
    self->properties = afw_environment_prepare_conf_type_properties(
        properties, xctx);

    /* Get authorizationHandlerType from properties. */
    self->authorization_handler_type_id = afw_object_old_get_property_as_string(
        properties, &afw_s_authorizationHandlerType, xctx);

    /* Get source location.  Default it to authorization handler. */
    self->source_location = afw_object_old_get_property_as_string(
        properties, &afw_s_sourceLocation, xctx);
    if (!self->source_location) {
        self->source_location = &afw_s_authorization_handler;
    }

    /* Get authorization_handler_id from parameters. */
    s = afw_object_old_get_property_as_utf8(properties,
        &afw_s_authorizationHandlerId, p, xctx);
    if (!s) {
        AFW_THROW_ERROR_FZ(general, xctx,
            AFW_UTF8_FMT " requires 'id' property.",
            AFW_UTF8_FMT_ARG(self->source_location));
    }
    afw_memory_copy(&self->authorization_handler_id, s);

    /* Create lock. */
    s = afw_utf8_printf(p, xctx,
        "authorization_handler_id:" AFW_UTF8_FMT,
        AFW_UTF8_FMT_ARG(&self->authorization_handler_id));
    if (!afw_environment_get_lock(s, xctx)) {
        self->authorization_handler_lock_rw = afw_lock_create_rw_and_register(
            afw_utf8_clone(s, xctx->env->p, xctx),
            afw_utf8_printf(xctx->env->p, xctx,
                "Authorization handler id " AFW_UTF8_FMT_Q " read/write lock",
                AFW_UTF8_FMT_ARG(&self->authorization_handler_id)),
            afw_utf8_printf(xctx->env->p, xctx,
                "Read/write lock used internally by authorization_handler id "
                AFW_UTF8_FMT_Q " implementation.",
                AFW_UTF8_FMT_ARG(&self->authorization_handler_id)),
            xctx);
    }

    /* Service id. */
    self->service_id = afw_utf8_printf(p, xctx,
        "authorizationHandler-" AFW_UTF8_FMT,
        AFW_UTF8_FMT_ARG(&self->authorization_handler_id));

    /* priority default 9999 */
    self->priority = afw_object_old_get_property_as_integer(properties,
        &afw_s_priority, &found, xctx);
    if (!found) {
        self->priority = 9999;
    }

    /* allow_* and required. */
    self->allow_deny_override = afw_object_old_get_property_as_boolean(properties,
        &afw_s_allowDenyOverride, &found, xctx);
    self->allow_permit_override = afw_object_old_get_property_as_boolean(properties,
        &afw_s_allowPermitOverride, &found, xctx);
    self->required = afw_object_old_get_property_as_boolean(properties,
        &afw_s_required, &found, xctx);
    
    /* Set trace fields */
    impl_set_trace_flag_fields(self, xctx);

    /* Trace create */
    afw_trace_fz(1, self->trace_flag_index, self, xctx,
        "authorizationHandlerId " AFW_UTF8_FMT_Q " is being created",
        AFW_UTF8_FMT_ARG(&self->authorization_handler_id));

    /* Return new instance. */
    return self;
}
