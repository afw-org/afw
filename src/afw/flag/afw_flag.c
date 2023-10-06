// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Trace
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_flag.c
 * @brief Adaptive Framework Trace
 */

#include "afw_internal.h"

/** @fixme should flag be condition or indicator and allow level? */


typedef struct  {
    const afw_flag_t *flag;


} impl_set_init_wa_t;



static void
impl_insure_flags_mutable(afw_xctx_t *xctx)
{
    afw_boolean_t *flags;
    afw_size_t count;

    if (!xctx->flags_is_mutable_copy ||
        xctx->env->flags_count_allocated != xctx->flags_count)
    {
        count = xctx->env->flags_count_allocated;
        flags = afw_xctx_malloc(count * sizeof(afw_boolean_t), xctx);
        memcpy(flags, (void *)xctx->env->default_flags, count * sizeof(afw_boolean_t));
        xctx->flags = flags;
        xctx->flags_count = count;
        xctx->flags_is_mutable_copy = true;
    }
}



static void
impl_set_applicable_flags(
    const afw_utf8_t *flag_id,
    afw_boolean_t set_to,
    afw_boolean_t required,
    afw_boolean_t *flags,
    afw_size_t flags_count,
    afw_xctx_t *xctx)
{
    const afw_flag_t *flag;
    afw_size_t i;

    flag = afw_environment_get_flag(flag_id, xctx);
    if (!flag) {
        if (!required) {
            return;
        }
        AFW_THROW_ERROR_FZ(general, xctx,
            "Flag " AFW_UTF8_FMT_Q " is not registered",
            AFW_UTF8_FMT_ARG(flag_id));
    }

    for (i = 0;
        i < flag->applicable_flags_count_allocated && i < flags_count;
        i++)
    {
        if (flag->applicable_flags[i]) {
            flags[i] = set_to;
        }
    }
}



static void
impl_flag_add_included_by(
    afw_flag_t *flag,
    afw_flag_t *included_by,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p = xctx->env->p;
    const afw_iterator_t *iterator;
    const afw_utf8_t *flag_id;
    const afw_data_type_t *data_type;
    afw_boolean_t *new_applicable;
    afw_flag_t *parent;
    afw_size_t i;
    afw_boolean_t already_included;


    /* Determined if flag id has already been added once. */
    already_included = false;
    if (included_by->includes_value) for (iterator = NULL;;) {
        afw_array_get_next_internal(included_by->includes_value->internal,
            &iterator, &data_type, (const void **)&flag_id, xctx);
        if (!flag_id) break;
        if (afw_utf8_equal(flag_id, flag->flag_id)) {
            already_included = true;
            break;
        }
    }

    /* If not an included_by->includes_value, make one. */
    else {
        included_by->includes_value = (const afw_value_array_t *)
            afw_value_create_array(
                afw_array_of_create(
                    afw_data_type_string, p, xctx),
                p, xctx);
    }

    /* If not already included, add includes and included by. */
    if (!already_included) {

        /* Add included_by to this flags included_by. */
        if (!flag->included_by_value) {
            flag->included_by_value = (const afw_value_array_t *)
                afw_value_create_array(
                    afw_array_of_create(
                        afw_data_type_string, p, xctx),
                    p, xctx);
        }
        afw_array_add_value(flag->included_by_value->internal,
            included_by->flag_id_value, xctx);

        /* Add flag to includes of included_by. */
        afw_array_add_value(included_by->includes_value->internal,
            flag->flag_id_value, xctx);
    }


    /* Set all of flag's applicable flags in included_by's applicable flags. */
    for (i = 0; i < flag->applicable_flags_count_allocated; i++) {
        if (flag->applicable_flags[i]) {
            if (i >= included_by->applicable_flags_count_allocated) {
                /* Causes memory leak. */
                new_applicable = afw_pool_calloc(p,
                    xctx->env->flags_count_allocated * sizeof(afw_boolean_t),
                    xctx);
                memcpy(new_applicable,
                    (const void *)included_by->applicable_flags,
                    (included_by->applicable_flags_count_allocated *
                        sizeof(afw_boolean_t)));
                included_by->applicable_flags = new_applicable;
                included_by->applicable_flags_count_allocated =
                    xctx->env->flags_count_allocated;
            }
            ((afw_boolean_t *)included_by->applicable_flags)[i] = true;
        }
    }


    /* Add flag to this included_by and to all the flags it is included by. */
    if (included_by->included_by_value) for (iterator = NULL;;) {
        afw_array_get_next_internal(included_by->included_by_value->internal,
            &iterator, &data_type, (const void **)&flag_id, xctx);
        if (!flag_id) break;
        parent = (afw_flag_t *)afw_environment_get_flag(flag_id, xctx);
        if (!parent) {
            AFW_THROW_ERROR_Z(general, "Unexpected situation", xctx);
        }
        impl_flag_add_included_by(included_by, parent, xctx);
    }
}



/* Refresh env->pub.default_flags based on env->default_flag_ids. */
static void
impl_refresh_default_flags(afw_xctx_t *xctx)
{
    afw_environment_t *env = (afw_environment_t *)xctx->env;
    afw_environment_internal_t *internal_env =
        (afw_environment_internal_t *)xctx->env;
    const afw_utf8_t * const * flag_id;
    const afw_pool_t *temp_p = NULL;
    afw_boolean_t *flags;
    afw_size_t flags_count;

    if (!internal_env->default_flag_ids) return;

    AFW_LOCK_BEGIN(xctx->env->flags_lock) {
        temp_p = afw_pool_create(xctx->p, xctx);
        flags_count = env->flags_count_allocated;
        flags = afw_pool_calloc(temp_p, sizeof(afw_boolean_t) * flags_count,
            xctx);
        for (flag_id = internal_env->default_flag_ids; *flag_id; flag_id++) {
            impl_set_applicable_flags(*flag_id, true, false, flags, flags_count,
                xctx);
        }
        memcpy((void *)env->default_flags, flags,
            sizeof(afw_boolean_t) * flags_count);
    }
    AFW_LOCK_END;

    if (temp_p) {
        afw_pool_destroy(temp_p, xctx);
    }
}



/* Register core flags. */
AFW_DEFINE_INTERNAL(void)
afw_flag_internal_early_register_core(afw_xctx_t *xctx)
{
    afw_environment_t *env = (afw_environment_t *)xctx->env;

    /* Register flag unspecified */
    afw_flag_environment_register_flag(
        afw_s_a_flag_unspecified,
        afw_s_a_flag_unspecified_brief,
        afw_s_a_flag_unspecified_description,
        NULL,
        xctx);

    /* Register flag compile:noImplicitAny */
    afw_flag_environment_register_flag(
        afw_s_a_flag_compile_noImplicitAny,
        afw_s_a_flag_compile_noImplicitAny_brief,
        afw_s_a_flag_compile_noImplicitAny_description,
        NULL,
        xctx);
    env->flag_index_compile_noImplicitAny_active =
        afw_environment_get_flag(
            afw_s_a_flag_compile_noImplicitAny,
            xctx)->flag_index;

    /* Register flag compile:noOptimize */
    afw_flag_environment_register_flag(
        afw_s_a_flag_compile_noOptimize,
        afw_s_a_flag_compile_noOptimize_brief,
        afw_s_a_flag_compile_noOptimize_description,
        NULL,
        xctx);
    env->flag_index_compile_noOptimize_active =
        afw_environment_get_flag(
            afw_s_a_flag_compile_noOptimize,
            xctx)->flag_index;

    /* Register flag debug:detail */
    afw_flag_environment_register_flag(
        afw_s_a_flag_debug_detail,
        afw_s_a_flag_debug_detail_brief,
        afw_s_a_flag_debug_detail_description,
        NULL,
        xctx);

    /* Register flag debug */
    afw_flag_environment_register_flag(
        afw_s_a_flag_debug,
        afw_s_a_flag_debug_brief,
        afw_s_a_flag_debug_description,
        afw_s_a_flag_debug_detail,
        xctx);

#ifdef AFW_LOCK_DEBUG
    /* Register flag debug:lock */
    afw_flag_environment_register_flag(
        afw_s_a_flag_debug_lock,
        afw_s_a_flag_debug_lock_brief,
        afw_s_a_flag_debug_lock_description,
        afw_s_a_flag_debug,
        xctx);
#endif

#ifdef AFW_POOL_DEBUG
    /* Register flag debug:pool:detail */
    afw_flag_environment_register_flag(
        afw_s_a_flag_debug_pool_detail,
        afw_s_a_flag_debug_pool_detail_brief,
        afw_s_a_flag_debug_pool_detail_description,
        afw_s_a_flag_debug_detail,
        xctx);
    env->flag_index_debug_pool_detail =
        afw_environment_get_flag(
            afw_s_a_flag_debug_pool_detail,
            xctx)->flag_index;

    /* Register flag debug:pool */
    afw_flag_environment_register_flag(
        afw_s_a_flag_debug_pool,
        afw_s_a_flag_debug_pool_brief,
        afw_s_a_flag_debug_pool_description,
        afw_s_a_flag_debug,
        xctx);
    afw_flag_add_included_by(
        afw_s_a_flag_debug_pool,
        afw_s_a_flag_debug_detail,
        xctx);
    env->flag_index_debug_pool =
        afw_environment_get_flag(
            afw_s_a_flag_debug_pool,
            xctx)->flag_index;
#endif

    /* Register flag debug:print:detail */
    afw_flag_environment_register_flag(
        afw_s_a_flag_debug_function_active_detail,
        afw_s_a_flag_debug_function_active_detail_brief,
        afw_s_a_flag_debug_function_active_detail_description,
        afw_s_a_flag_debug_detail,
        xctx);
    env->flag_index_debug_function_active_detail =
        afw_environment_get_flag(
            afw_s_a_flag_debug_function_active_detail,
            xctx)->flag_index;

    /* Register flag debug:print */
    afw_flag_environment_register_flag(
        afw_s_a_flag_debug_function_active,
        afw_s_a_flag_debug_function_active_brief,
        afw_s_a_flag_debug_function_active_description,
        afw_s_a_flag_debug,
        xctx);
    afw_flag_add_included_by(
        afw_s_a_flag_debug_function_active,
        afw_s_a_flag_debug_detail,
        xctx);
    afw_flag_add_included_by(
        afw_s_a_flag_debug_function_active,
        afw_s_a_flag_debug_function_active_detail,
        xctx);
    env->flag_index_debug_function_active =
        afw_environment_get_flag(
            afw_s_a_flag_debug_function_active,
            xctx)->flag_index;

    /* Register flag response:error */
    afw_flag_environment_register_flag(
        afw_s_a_flag_response_error,
        afw_s_a_flag_response_error_brief,
        afw_s_a_flag_response_error_description,
        NULL,
        xctx);

    /* Register flag response:error:backtrace */
    afw_flag_environment_register_flag(
        afw_s_a_flag_response_error_backtrace,
        afw_s_a_flag_response_error_backtrace_brief,
        afw_s_a_flag_response_error_backtrace_description,
        afw_s_a_flag_response_error,
        xctx);
    env->flag_index_response_error_backtrace =
        afw_environment_get_flag(
            afw_s_a_flag_response_error_backtrace,
            xctx)->flag_index;

    /* Register flag response:error:backtraceEvaluation */
    afw_flag_environment_register_flag(
        afw_s_a_flag_response_error_backtraceEvaluation,
        afw_s_a_flag_response_error_backtraceEvaluation_brief,
        afw_s_a_flag_response_error_backtraceEvaluation_description,
        afw_s_a_flag_response_error,
        xctx);
    env->flag_index_response_error_backtraceEvaluation =
        afw_environment_get_flag(
            afw_s_a_flag_response_error_backtraceEvaluation,
            xctx)->flag_index;

    /* Register flag response:error:contextual */
    afw_flag_environment_register_flag(
        afw_s_a_flag_response_error_contextual,
        afw_s_a_flag_response_error_contextual_brief,
        afw_s_a_flag_response_error_contextual_description,
        afw_s_a_flag_response_error,
        xctx);
    env->flag_index_response_error_contextual =
        afw_environment_get_flag(
            afw_s_a_flag_response_error_contextual,
            xctx)->flag_index;

    /* Register flag response:error:hasAdditionalDetail */
    afw_flag_environment_register_flag(
        afw_s_a_flag_response_error_hasAdditionalDetail,
        afw_s_a_flag_response_error_hasAdditionalDetail_brief,
        afw_s_a_flag_response_error_hasAdditionalDetail_description,
        afw_s_a_flag_response_error,
        xctx);
    env->flag_index_response_error_hasAdditionalDetail =
        afw_environment_get_flag(
            afw_s_a_flag_response_error_hasAdditionalDetail,
            xctx)->flag_index;
    afw_flag_add_included_by(
        afw_s_a_flag_response_error_hasAdditionalDetail,
        afw_s_a_flag_response_error_backtrace,
        xctx);
    afw_flag_add_included_by(
        afw_s_a_flag_response_error_hasAdditionalDetail,
        afw_s_a_flag_response_error_backtraceEvaluation,
        xctx);
    afw_flag_add_included_by(
        afw_s_a_flag_response_error_hasAdditionalDetail,
        afw_s_a_flag_response_error_contextual,
        xctx);

    /* Register response:console, stderr, and stdout. */
    afw_flag_environment_register_flag(
        afw_s_a_flag_response_console,
        afw_s_a_flag_response_console_brief,
        afw_s_a_flag_response_console_description,
        NULL,
        xctx);
    afw_flag_environment_register_flag(
        afw_s_a_flag_response_console_stream,
        afw_s_a_flag_response_console_stream_brief,
        afw_s_a_flag_response_console_stream_description,
        NULL,
        xctx);
    afw_flag_add_included_by(
        afw_s_a_flag_response_console,
        afw_s_a_flag_response_console_stream,
        xctx);

    afw_flag_environment_register_flag(
        afw_s_a_flag_response_stderr,
        afw_s_a_flag_response_stderr_brief,
        afw_s_a_flag_response_stderr_description,
        NULL,
        xctx);
    afw_flag_environment_register_flag(
        afw_s_a_flag_response_stderr_stream,
        afw_s_a_flag_response_stderr_stream_brief,
        afw_s_a_flag_response_stderr_stream_description,
        NULL,
        xctx);
    afw_flag_add_included_by(
        afw_s_a_flag_response_stderr,
        afw_s_a_flag_response_stderr_stream,
        xctx);

    afw_flag_environment_register_flag(
        afw_s_a_flag_response_stdout,
        afw_s_a_flag_response_stdout_brief,
        afw_s_a_flag_response_stdout_description,
        NULL,
        xctx);
    afw_flag_environment_register_flag(
        afw_s_a_flag_response_stdout_stream,
        afw_s_a_flag_response_stdout_stream_brief,
        afw_s_a_flag_response_stdout_stream_description,
        NULL,
        xctx);
    afw_flag_add_included_by(
        afw_s_a_flag_response_stdout,
        afw_s_a_flag_response_stdout_stream,
        xctx);


    /* Register flag trace:detail */
    afw_flag_environment_register_flag(
        afw_s_a_flag_trace_detail,
        afw_s_a_flag_trace_detail_brief,
        afw_s_a_flag_trace_detail_description,
        NULL,
        xctx);

    /* Register flag trace */
    afw_flag_environment_register_flag(
        afw_s_a_flag_trace,
        afw_s_a_flag_trace_brief,
        afw_s_a_flag_trace_description,
        afw_s_a_flag_trace_detail,
        xctx);

    /* Register flag trace:adaptor:detail */
    afw_flag_environment_register_flag(
        afw_s_a_flag_trace_adaptor_detail,
        afw_s_a_flag_trace_adaptor_detail_brief,
        afw_s_a_flag_trace_adaptor_detail_description,
        afw_s_a_flag_trace_detail,
        xctx);

    /* Register flag trace:adaptor */
    afw_flag_environment_register_flag(
        afw_s_a_flag_trace_adaptor,
        afw_s_a_flag_trace_adaptor_brief,
        afw_s_a_flag_trace_adaptor_description,
        afw_s_a_flag_trace,
        xctx);
    afw_flag_add_included_by(
        afw_s_a_flag_trace_adaptor,
        afw_s_a_flag_trace_adaptor_detail,
        xctx);

    /* Register flag trace:authorizationHandler:detail */
    afw_flag_environment_register_flag(
        afw_s_a_flag_trace_authorizationHandler_detail,
        afw_s_a_flag_trace_authorizationHandler_detail_brief,
        afw_s_a_flag_trace_authorizationHandler_detail_description,
        afw_s_a_flag_trace_detail,
        xctx);

    /* Register flag trace:authorizationHandler */
    afw_flag_environment_register_flag(
        afw_s_a_flag_trace_authorizationHandler,
        afw_s_a_flag_trace_authorizationHandler_brief,
        afw_s_a_flag_trace_authorizationHandler_description,
        afw_s_a_flag_trace,
        xctx);
    afw_flag_add_included_by(
        afw_s_a_flag_trace_authorizationHandler,
        afw_s_a_flag_trace_authorizationHandler_detail,
        xctx);

    /* Register flag trace:evaluation:detail */
    afw_flag_environment_register_flag(
        afw_s_a_flag_trace_evaluation_detail,
        afw_s_a_flag_trace_evaluation_detail_brief,
        afw_s_a_flag_trace_evaluation_detail_description,
        afw_s_a_flag_trace_detail,
        xctx);
    env->flag_index_trace_evaluation_detail =
        afw_environment_get_flag(
            afw_s_a_flag_trace_evaluation_detail,
            xctx)->flag_index;

    /* Register flag trace:request:detail */
    afw_flag_environment_register_flag(
        afw_s_a_flag_trace_request_detail,
        afw_s_a_flag_trace_request_detail_brief,
        afw_s_a_flag_trace_request_detail_description,
        afw_s_a_flag_trace_detail,
        xctx);
    env->flag_index_trace_request_detail =
        afw_environment_get_flag(
            afw_s_a_flag_trace_request_detail,
            xctx)->flag_index;

    /* Register flag trace:request */
    afw_flag_environment_register_flag(
        afw_s_a_flag_trace_request,
        afw_s_a_flag_trace_request_brief,
        afw_s_a_flag_trace_request_description,
        afw_s_a_flag_trace,
        xctx);
    afw_flag_add_included_by(
        afw_s_a_flag_trace_request,
        afw_s_a_flag_trace_request_detail,
        xctx);
    env->flag_index_trace_request =
        afw_environment_get_flag(
            afw_s_a_flag_trace_request,
            xctx)->flag_index;

}


/* Get flag by index */
AFW_DEFINE(const afw_flag_t *)
afw_flag_get_by_index(
    afw_size_t flag_index,
    afw_xctx_t *xctx)
{
    return (flag_index < xctx->env->flags_count_allocated)
        ? xctx->env->flag_by_index[flag_index]
        : NULL;
}


/* Get the flag index for a flag id. */
AFW_DEFINE(afw_size_t)
afw_flag_get_index(
    const afw_utf8_t *flag_id,
    afw_xctx_t *xctx)
{
    const afw_flag_t *flag;

    flag = afw_environment_get_flag(flag_id, xctx);
    if (!flag) {
        AFW_THROW_ERROR_FZ(general, xctx,
            AFW_UTF8_FMT_Q " is not a registered flagId",
            AFW_UTF8_FMT_ARG(flag_id));
    }
    return flag->flag_index;
}



/* Determine if flag for flag id is set in xctx. */
AFW_DECLARE(afw_boolean_t)
afw_flag_by_id_is_active(
    const afw_utf8_t *flag_id,
    afw_xctx_t *xctx)
{
    afw_size_t flag_index;

    flag_index = afw_flag_get_index(flag_id, xctx);
    return afw_flag_is_active(flag_index, xctx);
}



/*  Register a flag. */
AFW_DECLARE(void)
afw_flag_environment_register_flag(
    const afw_utf8_t *flag_id,
    const afw_utf8_t *brief,
    const afw_utf8_t *description,
    const afw_utf8_t *included_by_flag_id,
    afw_xctx_t *xctx)
{
    afw_environment_t *env = (afw_environment_t *)xctx->env;
    afw_environment_internal_t *internal_env =
        (afw_environment_internal_t *)xctx->env;
    const afw_pool_t *p = env->p;
    afw_flag_t *self;
    afw_flag_t *included_by;
    afw_size_t flags_count_allocated;
    afw_boolean_t *flags;
    afw_flag_t **flag_by_index;

    AFW_LOCK_BEGIN(env->flags_lock)
    {
        if (afw_environment_get_flag(flag_id, xctx)) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Flag " AFW_UTF8_FMT_Q " is already registered",
                AFW_UTF8_FMT_ARG(flag_id));
        }

        self = afw_pool_calloc_type(p, afw_flag_t, xctx);
        self->flag_id = flag_id;
        self->flag_id_value = afw_value_create_string(flag_id, p, xctx);
        self->brief = brief;
        self->description = description;
        self->object = afw_runtime_object_create_indirect(
            afw_s__AdaptiveFlag_, self->flag_id, self, p, xctx);
        self->object_value = afw_value_create_object(self->object,
            p, xctx);
        self->flag_index = (env->flags_count_registered)++;
        flags_count_allocated = env->flags_count_allocated;
        if (self->flag_index >= env->flags_count_allocated) {
            /*
             * This causes a small one time leak. If too bad adjust
             * AFW_FLAG_INITIAL_ALLOCATED_COUNT.
             */
            if (flags_count_allocated == 0) {
                flags_count_allocated = AFW_FLAG_INITIAL_ALLOCATED_COUNT;
            }
            else {
                flags_count_allocated = flags_count_allocated * 2;
            }

            /* env->flags */
            flags = afw_pool_calloc(p,
                flags_count_allocated * sizeof(afw_boolean_t), xctx);
            memcpy(flags, (void *)env->default_flags,
                env->flags_count_allocated * sizeof(afw_boolean_t));
            env->default_flags = (AFW_ATOMIC const afw_boolean_t *)flags;

            /* env->flag_by_index */
            flag_by_index = afw_pool_calloc(p,
                flags_count_allocated * sizeof(afw_flag_t **), xctx);
            memcpy(flag_by_index, (void *)env->flag_by_index,
                env->flags_count_allocated * sizeof(afw_flag_t **));
            env->flag_by_index = (const afw_flag_t * AFW_ATOMIC  *)flag_by_index;

            /* Set new allocated size now. */
            env->flags_count_allocated = flags_count_allocated;

            /* Set internal base xctx flags. */
            internal_env->base_xctx->flags =
                (afw_boolean_t *)env->default_flags;
            internal_env->base_xctx->flags_count =
                (afw_size_t)env->flags_count_allocated;
        }

        self->applicable_flags = afw_pool_calloc(p,
            flags_count_allocated * sizeof(afw_boolean_t), xctx);
        self->applicable_flags_count_allocated = flags_count_allocated;

        ((afw_boolean_t *)self->applicable_flags)[self->flag_index] = true;

        afw_environment_registry_register(
            afw_environemnt_registry_type_flag, flag_id, self, xctx);

        ((const afw_flag_t * AFW_ATOMIC *)env->flag_by_index)[self->flag_index] =
            self;

        if (included_by_flag_id) {
            included_by = (afw_flag_t *)afw_environment_registry_get(
                afw_environemnt_registry_type_flag,
                included_by_flag_id, xctx);
            if (!included_by) {
                AFW_THROW_ERROR_FZ(general, xctx,
                    "included_by_flag_id " AFW_UTF8_FMT_Q
                    " not found for flag_id " AFW_UTF8_FMT_Q,
                    AFW_UTF8_FMT_ARG(included_by_flag_id),
                    AFW_UTF8_FMT_ARG(flag_id));
            }
            impl_flag_add_included_by(self, included_by, xctx);
        }

        afw_runtime_env_set_object(self->object, false, xctx);

        impl_refresh_default_flags(xctx);
    }
    AFW_LOCK_END;
}



/* Add another include_by flag to a registered flag. */
AFW_DECLARE(void)
afw_flag_add_included_by(
    const afw_utf8_t *flag_id,
    const afw_utf8_t *included_by_flag_id,
    afw_xctx_t *xctx)
{
    afw_flag_t *flag;
    afw_flag_t *included_by;

    flag = (afw_flag_t *)afw_environment_get_flag(flag_id, xctx);
    included_by = (afw_flag_t *)afw_environment_get_flag(included_by_flag_id, xctx);

    if (!flag || !included_by) {
        AFW_THROW_ERROR_Z(general,
            "Both flag_id and included_by_flag_id must already be registered",
            xctx);
    }

    AFW_LOCK_BEGIN(xctx->env->flags_lock) {
        impl_flag_add_included_by(flag, included_by, xctx);
    }
    AFW_LOCK_END;
}



/** @fixme Add authorization to set flag */

/* Set flag to default value. */
AFW_DEFINE(void)
afw_flag_set_default(
    const afw_utf8_t *flag_id,
    afw_boolean_t set_to,
    afw_xctx_t *xctx)
{
    afw_environment_internal_t *internal_env =
        (afw_environment_internal_t *)xctx->env;
    const afw_utf8_t **id;
    const afw_pool_t *new_p;
    const afw_utf8_t **new_ids;
    const afw_utf8_t **new_id;
    afw_size_t count;

    AFW_LOCK_BEGIN(xctx->env->flags_lock) {
        /* Locate flag_id in defaults. */
        count = 0;
        if (internal_env->default_flag_ids) {
            for (id = (const afw_utf8_t **)internal_env->default_flag_ids;
                *id && !afw_utf8_equal(*id, flag_id);
                id++, count++);
        }

        /* If set and not already in list, add it. */
        if (set_to) {
            if (!internal_env->default_flag_ids || !*id) {
                new_p = afw_pool_create(internal_env->pub.p, xctx);
                new_ids = afw_pool_malloc(new_p,
                    (count + 2) * sizeof(afw_utf8_t **), xctx);
                new_id = new_ids;
                if (internal_env->default_flag_ids) {
                    for (id = (const afw_utf8_t **)internal_env->default_flag_ids;
                        *id;
                        id++, new_id++)
                    {
                        *new_id = afw_utf8_clone(*id, new_p, xctx);
                    }
                }
                *new_id++ = afw_utf8_clone(flag_id, new_p, xctx);
                *new_id = NULL;
                internal_env->default_flag_ids = new_ids;
                if (internal_env->default_flags_ids_p) {
                    afw_pool_destroy(internal_env->default_flags_ids_p, xctx);
                }
                internal_env->default_flags_ids_p = new_p;
            }
        }

        /* If unset and is in list, remove id. */
        else if (internal_env->default_flag_ids && *id) {
            for (; *id; id++) {
                *id = *(id + 1);
            }
        }

        impl_refresh_default_flags(xctx);
    }
    AFW_LOCK_END;
}



/* Set flag in xctx. */
AFW_DEFINE(void)
afw_flag_set(
    const afw_utf8_t *flag_id,
    afw_boolean_t set_to,
    afw_xctx_t *xctx)
{
    if (xctx == ((afw_environment_internal_t *)xctx->env)->base_xctx) {
        afw_flag_set_default(flag_id, set_to, xctx);
    }

    impl_insure_flags_mutable(xctx);
    impl_set_applicable_flags(flag_id, set_to, true,
        (afw_boolean_t *)xctx->flags, xctx->flags_count, xctx);
}


/* Set xctx default flags plus one or more additional flags. */
AFW_DEFINE(void)
afw_flag_set_to_defaults_plus_array(
    const afw_utf8_t * const *flag_ids,
    afw_xctx_t *xctx)
{
    const afw_utf8_t * const *flag_id;
    afw_boolean_t *flags;
    afw_size_t flags_count;

    flags_count = xctx->env->flags_count_allocated;
    flags = afw_xctx_malloc(flags_count * sizeof(afw_boolean_t), xctx);
    memcpy(flags, (void *)xctx->env->default_flags,
        flags_count * sizeof(afw_boolean_t));
    xctx->flags = flags;
    xctx->flags_count = flags_count;
    xctx->flags_is_mutable_copy = true;

    for (flag_id = flag_ids; *flag_id; flag_id++) {
        impl_set_applicable_flags(*flag_id, true, true,
        flags, flags_count, xctx);
    }
}



/* Set new default flags list. */
AFW_DEFINE(void)
afw_flag_set_default_flag_ids(
    const afw_array_t *default_flag_ids,
    afw_xctx_t *xctx)
{
    afw_environment_internal_t *internal_env =
        (afw_environment_internal_t *)xctx->env;
    const afw_pool_t *p;
    afw_size_t count;
    const afw_utf8_t **ids;
    const afw_utf8_t **id;
    const afw_utf8_t *s;
    const afw_data_type_t *data_type;
    const afw_iterator_t *iterator;

    AFW_LOCK_BEGIN(xctx->env->flags_lock) {
        p = afw_pool_create(xctx->env->p, xctx);
        count = afw_array_get_count(default_flag_ids, xctx);
        ids = afw_pool_malloc(p, (count + 1) * sizeof(afw_utf8_t *), xctx);
        ids[count] = NULL;

        iterator = NULL;
        for (id = ids;; id++) {
            afw_array_get_next_internal(default_flag_ids,
                &iterator, &data_type, (const void **)&s, xctx);
            if (!s) break;
            if (!afw_data_type_is_string(data_type)) {
                AFW_THROW_ERROR_Z(general,
                    "Default flagIds most all be strings", xctx);
            }
            *id = afw_utf8_clone(s, p, xctx);
        }

        internal_env->default_flag_ids = ids;
        if (internal_env->default_flags_ids_p) {
            afw_pool_destroy(internal_env->default_flags_ids_p, xctx);
        }
        internal_env->default_flags_ids_p = p;
        impl_refresh_default_flags(xctx);
    }
    AFW_LOCK_END;

}
