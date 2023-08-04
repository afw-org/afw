//>>> See COPYING at repository root
/*
 * Adaptive Framework Debug
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_debug.c
 * @brief Adaptive Framework Debug
 */

#include "afw_internal.h"



/* Write debug. */
AFW_DEFINE(void)
afw_debug_write(
    const afw_interface_implementation_rti_t *rti,
    const afw_utf8_z_t *source_z,
    const afw_utf8_t *message,
    afw_xctx_t *xctx)
{
    if (rti) {
        fprintf(xctx->env->debug_fd,
            ">debug [" AFW_UTF8_FMT ":" AFW_UTF8_FMT "] " AFW_UTF8_FMT
            " (%s)\n",
            AFW_UTF8_FMT_ARG(&rti->interface_name),
            AFW_UTF8_FMT_ARG(&rti->implementation_id),
            AFW_UTF8_FMT_ARG(message),
            afw_utf8_z_source_file(source_z));
    }
    else {
        fprintf(xctx->env->debug_fd, ">debug " AFW_UTF8_FMT " (%s)\n",
            AFW_UTF8_FMT_ARG(message), afw_utf8_z_source_file(source_z));
    }
    fflush(xctx->env->debug_fd);
}


/*  Write debug using a printf style format. */
AFW_DEFINE(void)
afw_debug_write_fz(
    const afw_interface_implementation_rti_t * rti,
    const afw_utf8_z_t * source_z,
    afw_xctx_t *xctx,
    const afw_utf8_z_t * format_z,
    ...)
{
    va_list ap;
    afw_utf8_t message;

    va_start(ap, format_z);
    message.s = apr_pvsprintf(afw_pool_get_apr_pool(xctx->p), format_z, ap);
    message.len = strlen(message.s);
    afw_debug_write(rti, source_z, &message, xctx);
    va_end(ap);
}


/* Write debug using a printf style format and va_list. */
AFW_DEFINE(void)
afw_debug_write_vz(
    const afw_interface_implementation_rti_t *rti,
    const afw_utf8_z_t *source_z,
    const afw_utf8_z_t *format_z,
    va_list ap,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *message;

    message = afw_utf8_printf_v(format_z, ap, xctx->p, xctx);
    afw_debug_write(rti, source_z, message, xctx);
}

/* Write debug from zero terminate string. */
AFW_DEFINE(void)
afw_debug_write_z(
    const afw_interface_implementation_rti_t *rti,
    const afw_utf8_z_t * source_z,
    const afw_utf8_z_t * message_z,
    afw_xctx_t *xctx)
{
    afw_utf8_t message;

    message.s = message_z;
    message.len = strlen(message_z);
    afw_debug_write(rti, source_z, &message, xctx);
}
