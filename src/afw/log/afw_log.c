// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_log interface for log director
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_log.c
 * @brief Implementation of afw_log interface for log director
 */

#include "afw_internal.h"



/* Declares and rti/inf defines for interface afw_log */
#define AFW_IMPLEMENTATION_ID "log"
#include "afw_log_impl_declares.h"
#include "afw_service_type_impl_declares.h"

static const afw_value_t *
impl_log_current_variable_get_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx);


static const afw_log_priority_id_map_entry_t impl_log_priority_id_map[] =
{
    { afw_s_emerg,   afw_log_priority_emerg,   "System unusable" },
    { afw_s_alert,   afw_log_priority_alert,   "Immediate action required" },
    { afw_s_crit,    afw_log_priority_crit,    "Critical conditions" },
    { afw_s_err,     afw_log_priority_err,     "Error conditions" },
    { afw_s_warning, afw_log_priority_warning, "Warning, errors may occur" },
    { afw_s_notice,  afw_log_priority_notice,  "Normal but significant" },
    { afw_s_info,    afw_log_priority_info,    "Informational" },
    { afw_s_debug,   afw_log_priority_debug,   "Debug" },
    { afw_s_trace1,  afw_log_priority_trace1,  "Trace level 1 message" },
    { afw_s_trace2,  afw_log_priority_trace2,  "Trace level 2 message" },
    { afw_s_trace3,  afw_log_priority_trace3,  "Trace level 3 message" },
    { afw_s_trace4,  afw_log_priority_trace4,  "Trace level 4 message" },
    { afw_s_trace5,  afw_log_priority_trace5,  "Trace level 5 message" },
    { afw_s_trace6,  afw_log_priority_trace6,  "Trace level 6 message" },
    { afw_s_trace7,  afw_log_priority_trace7,  "Trace level 7 message" },
    { afw_s_trace8,  afw_log_priority_trace8,  "Trace level 8 message" },
    { NULL,           afw_log_priority_invalid, "Invalid log priority" }
};

typedef struct impl_log_entry_s impl_log_entry_t;

struct impl_log_entry_s {
    impl_log_entry_t *next;
    const afw_log_t *log;
};

typedef struct impl_log_head_s {
    impl_log_entry_t *first_log;
    impl_log_entry_t *last_log;
} impl_log_head_t;


typedef struct impl_log_self_s {
    afw_log_t pub;
    
    impl_log_head_t *head;
} impl_afw_log_self_t;


typedef struct {
    impl_afw_log_self_t *self;
    const afw_pool_t *p;
    afw_log_priority_t priority;
    const afw_utf8_z_t *source_z;
    const afw_utf8_t *message;
    afw_log_priority_mask_t mask;
    impl_log_entry_t *e;
    const afw_utf8_t *formatted_message;
} impl_format_write_t;


/* Reset global mask based on log chain. */
static void
impl_reset_environment_log_mask(afw_xctx_t *xctx)
{
    impl_afw_log_self_t *self;
    impl_log_entry_t *e;
    afw_log_priority_mask_t new_mask;
    afw_log_priority_mask_t *env_mask;
    afw_log_impl_t *impl;

    AFW_LOCK_BEGIN(xctx->env->active_log_list_lock) {
        new_mask = 0;
        self = (impl_afw_log_self_t *)xctx->env->log;
        for (e = self->head->first_log; e; e = e->next) {
            if (e->log) {
                new_mask |= e->log->impl->mask;
            }
        }
        impl = (afw_log_impl_t *)self->pub.impl;
        impl->mask = new_mask;
        env_mask = (afw_log_priority_mask_t *)&xctx->env->log_mask;
        *env_mask = new_mask;
    }
    AFW_LOCK_END;
}


/*
 * Get common variable callback. 
 *
 * Note: Make sure to update
 * afw_log_internal_register_logType_context_type()
 * with parallel changes to this function.
 */
static const afw_value_t *
impl_log_current_variable_get_cb(
    const afw_xctx_qualifier_stack_entry_t *entry,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    impl_format_write_t *wa = (impl_format_write_t *)entry->data;
    const afw_value_t *result;
    afw_value_string_t *source;

    result = NULL;

    if (afw_utf8_equal(name, afw_s_message)) {
        result = afw_value_create_unmanaged_string(wa->message, wa->p, xctx);
    }

    else if (afw_utf8_equal(name, afw_s_xctxUUID)) {
        result = afw_value_create_unmanaged_string(xctx->uuid, wa->p, xctx);
    }

    else if (afw_utf8_equal(name, afw_s_source)) {
        if (wa->source_z) {
            source = afw_value_allocate_unmanaged_string(wa->p, xctx);
            source->internal.s = afw_utf8_z_source_file(wa->source_z);
            source->internal.len = strlen(source->internal.s);
            result = (const afw_value_t *)source;
        }
        else {
            result = afw_value_empty_string;
        }
    }

    return result;
}



/*
 * Register logType context type.
 *
 * Note: Make sure to update impl_log_current_variable_get_cb()
 * with parallel changes to this function.
 */
void afw_log_internal_register_logType_context_type(
    const afw_utf8_t *log_type_id,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *context_type_id;
    const afw_utf8_t *conf_object_type_id;
    const afw_object_t *context_type_object;
    const afw_object_t *qualifier_definitions;
    const afw_object_t *variable_definitions;

    context_type_id = afw_utf8_printf(xctx->env->p, xctx,
        "logType-" AFW_UTF8_FMT, AFW_UTF8_FMT_ARG(log_type_id));
    conf_object_type_id = afw_utf8_printf(xctx->env->p, xctx,
        "_AdaptiveConf_log_" AFW_UTF8_FMT, AFW_UTF8_FMT_ARG(log_type_id));
    context_type_object = afw_context_type_create(
        context_type_id, xctx->env->p, xctx);
    qualifier_definitions =
        afw_context_type_insure_qualifier_definitions_object_exists(
            context_type_object, xctx);
    afw_object_meta_add_parent_path(qualifier_definitions,
        afw_s_a_context_type_application_qualifier_definitions_path,
        xctx);

    variable_definitions =
        afw_context_type_insure_variable_definitions_object_exists(
            context_type_object, afw_s_log, xctx);
    afw_context_variable_definitions_add_based_on_object_type_id(
        variable_definitions, conf_object_type_id,
        false, xctx);

    variable_definitions =
        afw_context_type_insure_variable_definitions_object_exists(
            context_type_object, afw_s_current, xctx);
    afw_context_variable_definition_add_z(variable_definitions,
        afw_s_message, afw_s_internal,
        &afw_value_unmanaged_string_inf,
        "Message",
        "Unformatted message that is being logged.",
        NULL, NULL,
        xctx);
    afw_context_variable_definition_add_z(variable_definitions,
        afw_s_xctxUUID, afw_s_internal,
        &afw_value_unmanaged_string_inf,
        "UUID",
        "The current UUID of the execution context (xctx).",
        NULL, NULL,
        xctx);
    afw_context_variable_definition_add_z(variable_definitions,
        afw_s_source, afw_s_internal,
        &afw_value_unmanaged_string_inf,
        "Source",
        "Source file that issued message.",
        NULL, NULL,
        xctx);

    afw_environment_register_context_type(context_type_id,
        context_type_object, xctx);
}



/* Internal create environment log. */
extern const afw_log_t *
afw_log_internal_create_environment_log(
    afw_xctx_t *xctx)
{
    impl_afw_log_self_t *self;
    const afw_log_t *log;

    self = afw_xctx_calloc_type(impl_afw_log_self_t, xctx);
    self->pub.inf = &impl_afw_log_inf;
    log = (const afw_log_t *)self;

    /* Log id to log_type (implementation_id). */
    afw_memory_copy(&self->pub.log_id, &impl_afw_log_inf.rti.implementation_id);
    self->pub.p = xctx->p;
    self->pub.impl = afw_xctx_calloc_type(afw_log_impl_t, xctx);

    /* Allocate log head and register its singleton. */
    self->head = afw_xctx_calloc_type(impl_log_head_t, xctx);

    /* Return log implementation. */
    return log;
}


/* Get priority_id/priority map array. */
AFW_DEFINE(const afw_log_priority_id_map_entry_t *)
afw_log_get_priority_id_map()
{
    return &impl_log_priority_id_map[0];
}


/* Convert a log priority id to internal. */
AFW_DEFINE(afw_log_priority_t)
afw_log_priority_id_to_priority(
    const afw_utf8_t *priority_id)
{
    const afw_log_priority_id_map_entry_t *e;

    for (e = &impl_log_priority_id_map[0]; e->priority_id; e++) {
        if (afw_utf8_equal(priority_id, e->priority_id)) break;
    }

    return e->priority;
}


/* Convert a log priority to priority id. */
AFW_DEFINE(const afw_utf8_t *)
afw_log_priority_to_priority_id(
    afw_log_priority_t priority)
{
    const afw_log_priority_id_map_entry_t *e;

    for (e = &impl_log_priority_id_map[0]; e->priority_id;  e++)
    {
        if (priority == e->priority) break;
    }

    return e->priority_id;
}



/* Add a log to environment. */
AFW_DEFINE(void)
afw_log_add_to_environment(
    const afw_log_t * instance,
    afw_xctx_t *xctx)
{
    afw_log_impl_t *impl = (afw_log_impl_t *)instance->impl;
    impl_log_entry_t *e;
    impl_log_head_t *head;
    const afw_pool_t *p;
    impl_afw_log_self_t *env_log;

    /* Use p of environment. */
    p = xctx->env->p;

    AFW_LOCK_BEGIN(xctx->env->active_log_list_lock) {

        /* Register log.  Will throw error if already registered. */
        afw_environment_register_log(&instance->log_id, instance, xctx);

        /* Get environment log's self. */
        env_log = (impl_afw_log_self_t *)xctx->env->log;

        /* Get head of entries. Allocate if necessary. */
        head = env_log->head;
        if (!head) {
            head = afw_pool_calloc_type(p, impl_log_head_t, xctx);
            env_log->head = head;
        }

        /* Replace same log id. */
        /** @fixme Need to use a "log anchor" with id in it. */
        for (e = head->first_log; e; e = e->next) {
            if (e->log && afw_utf8_equal(&e->log->log_id, &instance->log_id))
            {
                if (e->log) {
                    /** @fixme runtime object might be accessing on another thread.
                    afw_log_destroy(e->log, xctx);
                     */
                }
                e->log = instance;
                break;
            }

        }
        if (e) {
            break;
        }

        /* Allocate new entry. */
        e = afw_pool_calloc_type(p, impl_log_entry_t, xctx);
        e->log = instance;

        /* Add entry to list. */
        if (!head->first_log) {
            head->first_log = e;
        }
        else {
            head->last_log->next = e;
        }
        head->last_log = e;

    }
    AFW_LOCK_END;

    afw_log_set_mask((const afw_log_t *)env_log, impl->mask, xctx);
}


/* Set a log's mask and insure env->mask is correct. */
AFW_DEFINE(void)
afw_log_set_mask(
    const afw_log_t * instance,
    afw_log_priority_mask_t mask,
    afw_xctx_t *xctx)
{
    /* Set own mask. */
    afw_log_set_own_mask(instance, mask, xctx);

    /* Reset environment log mask. */
    impl_reset_environment_log_mask(xctx);
}


/* Log an message using a printf style format and va_list. */
AFW_DEFINE(void)
afw_log_write_vz(
    const afw_log_t * instance,
    afw_log_priority_t priority,
    const afw_utf8_z_t * source_z,
    const afw_utf8_z_t * format_z,
    va_list ap,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *message;

    message = afw_utf8_printf_v(format_z, ap, xctx->p, xctx);
    afw_log_write(instance, priority, source_z, message,
        xctx);
}


/* Configuration handler for entry type "log". */
void afw_log_internal_conf_type_create_cede_p(
    const afw_utf8_t *type,
    const afw_object_t *conf,
    const afw_utf8_t *source_location,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    /* Start log. */
    afw_service_start_using_AdaptiveConf_cede_p(
        conf, source_location,
        p, xctx);
}


/*
 * Implementation of method destroy of interface afw_log.
 */
void
impl_afw_log_destroy(
    const afw_log_t * instance,
    afw_xctx_t *xctx)
{
    afw_pool_release(instance->p, xctx);
}


/*
 * Implementation of method set_mask of interface afw_log.
 */
void
impl_afw_log_set_own_mask(
    const afw_log_t * instance,
    afw_log_priority_mask_t mask,
    afw_xctx_t *xctx)
{
    afw_log_impl_t *impl = (afw_log_impl_t *)instance->impl;

    /* Set mask. */
    impl->mask = mask;
}


/* Format a log message. */
static void
impl_write_formatted_message(
    impl_format_write_t *wa,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;
    int top;

    /* Make sure there is not a recursive error while logging. */
    top = afw_xctx_qualifier_stack_top_get(xctx);
    AFW_TRY {

        if (!wa->p) {
            wa->p = afw_pool_create(xctx->p, xctx);
        }

        /* Add qualifiers, if needed.  Note: last one pushed has precedence. */
        if (wa->e && (wa->e->log->impl->filter || wa->e->log->impl->format))
        {
            afw_xctx_qualifier_stack_qualifier_object_push(afw_s_log,
                wa->e->log->properties,
                true, wa->p, xctx);
            afw_xctx_qualifier_stack_qualifier_push(afw_s_current, NULL, true,
                impl_log_current_variable_get_cb, (void *)wa,
                wa->p, xctx);
            if (wa->e->log->impl->custom_variables) {
                afw_xctx_qualifier_stack_qualifier_object_push(afw_s_custom,
                    wa->e->log->impl->custom_variables,
                    true, wa->p, xctx);
            }
        }

        /* If filter and not true, skip write. */
        if (wa->e && wa->e->log->impl->filter) {
            value = afw_value_evaluate(wa->e->log->impl->filter,
                wa->p, xctx);
            if (!value ||
                !afw_value_is_boolean(value) ||
                !((const afw_value_boolean_t *)value)->internal)
            {
                break;
            }
        }

        /* Message with format. */
        if (wa->e && wa->e->log->impl->format) {
            value = afw_value_evaluate(wa->e->log->impl->format,
                wa->p, xctx);
            if (!value || !afw_value_is_string(value)) {
                AFW_THROW_ERROR_Z(general,
                    "log format did not evaluate to single string",
                    xctx);
            }
            wa->formatted_message =
                &((afw_value_string_t *)value)->internal;
        }

        /* Message with default format. */
        else {
            /* No time or program name[pid] */
            wa->formatted_message = afw_utf8_printf(wa->p, xctx,
                "[" AFW_UTF8_FMT
                " " AFW_UTF8_FMT
                "] " AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(&xctx->env->application_id),
                AFW_UTF8_FMT_ARG(xctx->uuid),
                AFW_UTF8_FMT_ARG(wa->message));
        }

        /* Write message.  If no log started yet, just write to stderr. */
        if (wa->e) {
            afw_log_write(wa->e->log, wa->priority,
                wa->source_z, wa->formatted_message, xctx);
        } else {
            fprintf(xctx->env->stderr_fd, AFW_UTF8_FMT "\n",
                AFW_UTF8_FMT_ARG(wa->formatted_message));

            /* flush output */
            fflush(xctx->env->stderr_fd);
        }
    }

    AFW_CATCH_UNHANDLED {
        /** @fixme Throwing error is a bad idea, so indicate some other way. */
        /** @fixme Maybe abort? */
        fprintf(xctx->env->stderr_fd,
            "***ERROR*** while writing log - %s\n", AFW_ERROR_THROWN->message_z);
    }

    AFW_FINALLY{
        afw_xctx_qualifier_stack_top_set(top, xctx);
    }

    AFW_ENDTRY;
}



/*
 * Implementation of method write of interface afw_log.
 */
void
impl_afw_log_write(
    const afw_log_t * instance,
    afw_log_priority_t priority,
    const afw_utf8_z_t * source_z,
    const afw_utf8_t * message,
    afw_xctx_t *xctx)
{
    impl_format_write_t wa;
    afw_boolean_t lock_held;
    afw_boolean_t log_found;

    wa.self = (impl_afw_log_self_t *)instance;
    wa.p = NULL;
    wa.priority = priority;
    wa.source_z = source_z;
    wa.message = message;
    lock_held = false;

    AFW_TRY {

        /* Get lock for active list. */
        afw_lock_obtain(xctx->env->active_log_list_lock, xctx);
        lock_held = true;

        /* Make mask from priority for short circuit test. */
        wa.mask = 1 << priority;

        /* If logs added to environment, process them. */
        wa.e = wa.self->head->first_log;
        log_found = false;
        if (wa.e) {
            for (; wa.e; wa.e = wa.e->next) {
                if (wa.e->log) {
                    log_found = true;
                    if (wa.e->log->impl->mask & wa.mask)
                    {
                        impl_write_formatted_message(&wa, xctx);
                    }
                }
            }
        }

        /*
         * If no logs have been added to environment, write to stdout.
         */
        if (!log_found) {
            wa.e = NULL;
            impl_write_formatted_message(&wa, xctx);
        }
    }

    AFW_CATCH_UNHANDLED{
        /** @fixme Throwing error is a bad idea, so indicate some other way. */
        /** @fixme Maybe abort? */
        fprintf(xctx->env->stderr_fd,
            "***ERROR*** while writing log - %s\n", AFW_ERROR_THROWN->message_z);
    }

    AFW_FINALLY{
        if (lock_held) {
            afw_lock_release(xctx->env->active_log_list_lock, xctx);
        }
        if (wa.p) {
            afw_pool_destroy(wa.p, xctx);
        }
    }

    AFW_ENDTRY;
}



void
afw_log_internal_register_service_type(afw_xctx_t *xctx)
{
    afw_service_type_t *self;

    self = afw_xctx_calloc_type(afw_service_type_t, xctx);
    self->inf = &impl_afw_service_type_inf;
    afw_memory_copy(&self->service_type_id, afw_s_log);
    self->conf_type = afw_environment_get_conf_type(afw_s_log, xctx);
    if (!self->conf_type) {
        AFW_THROW_ERROR_Z(general, "conf_type must already be registered",
            xctx);
    }
    self->title = afw_s_a_service_type_log_title;
    self->conf_type_object = afw_runtime_get_object(afw_s__AdaptiveConfType_,
        afw_s_log, xctx);
    afw_environment_register_service_type(afw_s_log, self, xctx);
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
    const afw_log_t *log;

    log = afw_environment_get_log(id, xctx);
    return (log) ? 1 : 0;
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
    const afw_log_factory_t *factory;
    const afw_log_t *log;
    const afw_utf8_t *log_type;

    log_type = afw_object_old_get_property_as_utf8(properties,
        afw_s_logType, p, xctx);
    if (!log_type) {
        AFW_THROW_ERROR_Z(general, "parameter logType missing", xctx);
    }

    factory = afw_environment_get_log_type(log_type, xctx);
    if (!factory) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "logType " AFW_UTF8_FMT_Q " is not a registered log type.",
            AFW_UTF8_FMT_ARG(log_type));
    }
   
    /* Create log. */
    log = afw_log_factory_create_log_cede_p(factory,
        properties, p, xctx);

    /* Add/replace log in environment. */
    afw_log_add_to_environment(log, xctx);
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
    impl_afw_log_self_t *self;
    impl_log_entry_t *e;

    afw_environment_register_log(id, NULL, xctx);

    /*
     * This counts on afw_service only calling if not stopped, if log is not
     * found, no error is thrown.
     */
    AFW_LOCK_BEGIN(xctx->env->active_log_list_lock) {
        self = (impl_afw_log_self_t *)xctx->env->log;
        for (e = self->head->first_log; e; e = e->next)
        {
            if (e->log && afw_utf8_equal(&e->log->log_id, id)) {
                /** @fixme Need to be able to destroy, but runtime object might
                 * be using.
                 *
                afw_log_destroy(e->log, xctx);
                 */
                e->log = NULL;
            }
        }
    }
    AFW_LOCK_END;

    /* Reset environment log mask. */
    impl_reset_environment_log_mask(xctx);
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
    /*
     * This counts on afw_service only calling if not started, so if stopped,
     * this will start log. Restart is same as start at this point.
     */
    impl_afw_service_type_start_cede_p(instance, properties, p, xctx);
}


AFW_DEFINE(void)
afw_log_impl_throw_property_invalid(
    const afw_log_t *log,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx)
{
    AFW_THROW_ERROR_FZ(general, xctx,
        "Configuration type=log, logId=" AFW_UTF8_FMT_Q
        " property name " AFW_UTF8_FMT_Q " is not valid.",
        AFW_UTF8_FMT_ARG(&log->log_id),
        AFW_UTF8_FMT_ARG(property_name));
}


AFW_DEFINE(void)
afw_log_impl_throw_property_required(
    const afw_log_t *log,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx)
{
    AFW_THROW_ERROR_FZ(general, xctx,
        "Configuration type=log, logId=" AFW_UTF8_FMT_Q
        " property name " AFW_UTF8_FMT_Q " is required.",
        AFW_UTF8_FMT_ARG(&log->log_id),
        AFW_UTF8_FMT_ARG(property_name));
}


/* Developers should call this in all create functions for afw_log. */
AFW_DEFINE(afw_log_t *)
afw_log_impl_create_cede_p(
    const afw_log_inf_t *inf,
    afw_size_t instance_size,
    const afw_object_t *properties,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_log_t *self;
    afw_log_impl_t *impl;
    const afw_log_priority_id_map_entry_t *e;
    const afw_utf8_t *s;
    afw_boolean_t b;
    afw_boolean_t found;

    /* Allocate new log instance in new subpool. */
    self = afw_pool_calloc(p,
        (instance_size != 0) ? instance_size : sizeof(afw_log_t),
        xctx);
    self->inf = inf;
    self->p = p;
    self->impl = impl = afw_pool_calloc_type(p, afw_log_impl_t, xctx);

    /* Prepare properties. */
    self->properties = afw_environment_prepare_conf_type_properties(
        properties, xctx);

    /* Get source location.  Default it to adaptor. */
    self->source_location = afw_object_old_get_property_as_string(
        properties, afw_s_sourceLocation, xctx);
    if (!self->source_location) {
        self->source_location = afw_s_log;
    }

    /* Get log_id from parameters. Default to log_type. */
    s = afw_object_old_get_property_as_utf8(properties,
        afw_s_logId, p, xctx);
    memcpy(&self->log_id, s, sizeof(afw_utf8_t));

    /* Service id. */
    self->service_id = afw_utf8_printf(p, xctx,
        "log-" AFW_UTF8_FMT,
        AFW_UTF8_FMT_ARG(&self->log_id));

    /* Process <priority>, if they exists. */
    for (e = afw_log_get_priority_id_map(); e->priority_id; e++) {
        b = afw_object_old_get_property_as_boolean(properties, e->priority_id,
            &found, xctx);
        if (found) {
            afw_log_set_priority_in_mask(&impl->mask, e->priority, b);
        }
    }

    /* Compile filter, if it exists. */
    impl->filter = afw_object_old_get_property_as_compiled_script(
        properties, afw_s_filter, self->source_location, NULL, p, xctx); 

    /* Compile format, if it exists. */
    impl->format = afw_object_old_get_property_as_compiled_template(
        properties, afw_s_format, self->source_location, NULL, p, xctx); 

    /* Return new log. */
    return self;
}
