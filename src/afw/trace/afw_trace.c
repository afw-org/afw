// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Trace
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_trace.c
 * @brief Adaptive Framework Trace
 */

#include "afw_internal.h"



/* Write trace. */
AFW_DEFINE(void)
afw_trace_write(
    afw_log_priority_t priority,
    const afw_interface_implementation_rti_t *rti,
    const afw_utf8_z_t *source_z,
    const afw_utf8_t *message,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *msg;

    msg = message;
    if (rti) {
        msg = afw_utf8_printf(xctx->p, xctx,
            "Trace[" AFW_UTF8_FMT ":" AFW_UTF8_FMT "] " AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(&rti->interface_name),
            AFW_UTF8_FMT_ARG(&rti->implementation_id),
            AFW_UTF8_FMT_ARG(message));
    }
    afw_log_write(xctx->env->log, priority,
        source_z, msg, xctx);
}


/*  Write trace using a printf style format. */
AFW_DEFINE(void)
afw_trace_write_fz(
    afw_log_priority_t priority,
    const afw_interface_implementation_rti_t * rti,
    const afw_utf8_z_t * source_z,
    afw_xctx_t *xctx,
    const afw_utf8_z_t * format_z,
    ...)
{
    va_list ap;
    const afw_utf8_t *message;

    va_start(ap, format_z);
    message = afw_utf8_printf_v(format_z, ap, xctx->p, xctx);
    afw_trace_write(priority, rti, source_z, message, xctx);
    va_end(ap);
}


/* Write trace using a printf style format and va_list. */
AFW_DEFINE(void)
afw_trace_write_vz(
    afw_log_priority_t priority,
    const afw_interface_implementation_rti_t *rti,
    const afw_utf8_z_t *source_z,
    const afw_utf8_z_t *format_z,
    va_list ap,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *message;

    message = afw_utf8_printf_v(format_z, ap, xctx->p, xctx);
    afw_trace_write(priority, rti, source_z, message, xctx);
}

/* Write trace from zero terminate string. */
AFW_DEFINE(void)
afw_trace_write_z(
    afw_log_priority_t priority,
    const afw_interface_implementation_rti_t *rti,
    const afw_utf8_z_t * source_z,
    const afw_utf8_z_t * message_z,
    afw_xctx_t *xctx)
{
    afw_utf8_t message;

    message.s = message_z;
    message.len = strlen(message_z);
    afw_trace_write(priority, rti, source_z, &message, xctx);
}
