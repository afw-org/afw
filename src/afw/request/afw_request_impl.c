// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Request Implementation Support
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_request_impl.c
 * @brief AFW request implementation support
 */

#include "afw_internal.h"



/* Trace request begin. */
AFW_DEFINE(void)
afw_request_impl_trace_begin(
    const afw_request_t *instance,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *s;

    if (afw_flag_is_active(xctx->env->flag_index_trace_request_detail,
        xctx))
    {
        s = afw_data_type_object_to_utf8(instance->properties,
            xctx->p, xctx);
        afw_trace_fz(1, xctx->env->flag_index_trace_request,
            instance, xctx,
            "RequestProperties=%" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(s)
        );
    }
    else if (afw_flag_is_active(xctx->env->flag_index_trace_request,
        xctx))
    {
        afw_trace_fz(1, xctx->env->flag_index_trace_request, 
            instance, xctx,
            "Method=%" AFW_UTF8_FMT
            " uri=%" AFW_UTF8_FMT
            " query_string=%" AFW_UTF8_FMT,
            AFW_UTF8_FMT_OPTIONAL_ARG(instance->method),
            AFW_UTF8_FMT_OPTIONAL_ARG(instance->uri),
            AFW_UTF8_FMT_OPTIONAL_ARG(instance->query_string));
    }
}
