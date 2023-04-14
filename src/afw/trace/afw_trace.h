// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Software Trace Header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_TRACE_H__
#define __AFW_TRACE_H__


/** @addtogroup afw_c_api
 *
 * @{
 */

/** @addtogroup afw_trace Software Trace
 *
 * These macros and functions assist writing trace message to the environment
 * log.  All trace message have associated runtime information (rti) that can
 * be used for prefixing messages as well as filtering.  The first parameter
 * of macros AFW_TRACE(), AFW_TRACE_Z(), AFW_TRACE_FZ(), and AFW_TRACE_VZ()
 * is a instance, which is used to locate the implementation rti.  The
 * AFW_TRACE_WRITE*() macros and afw_trace_write*() functions have rti as the
 * first parameter.
 *
 * All trace message are written to the environment log.  See afw_log.h for
 * more information.
 *
 * @{
 */

/**
 * @file afw_trace.h
 * @brief Adaptive Framework Software Trace Header.
 */

#include "afw_interface.h"

AFW_BEGIN_DECLARES

/**
 * @brief Define a static trace_id rti.
 * @param label for the static trace_id rti.
 * @param trace_id
 *
 * The first parameter of afw_trace_write*() functions and AFW_TRACE_WRITE*()
 * macros is an rti.  This macro defines a static rti that can be passed to
 * these for non-interface tracing.
 */
#define AFW_TRACE_STATIC_TRACE_ID(label, trace_id) \
static const afw_interface_implementation_rti_t label = \
    {"afw_trace_id", __FILE__, trace_id};


/**
 * @brief If applicable, write trace.
 * @param trace_level is a literal 1 - 8 added to "trace" to form log level.
 * @param flag_index of flag to check to determine if trace is applicable.
 * @param instance associated with trace message.
 * @param message to log.
 * @param xctx of caller.
 *
 * The trace_level 1 is the most commonly used level in Adaptive Framework.
 * Use 2 - 8 when a particular trace benefits from a different log priority.
 * For example, 2 - 8 could indicate more detail for a particular flag.
 */
#define afw_trace(trace_level, flag_index, instance, message, xctx) \
    if ((flag_index) < (xctx)->flags_count && (xctx)->flags[flag_index]) \
        afw_trace_write( \
            afw_log_priority_trace##trace_level, \
            ((instance) ? \
                &((const afw_instance_t *)instance)->inf->rti \
                : NULL), \
            AFW__FILE_LINE__, message, xctx)


/**
 * @brief If applicable, write trace using a printf style format.
 * @param trace_level is a literal 1 - 8 added to "trace" to form log level.
 * @param flag_index of flag to check to determine if trace is applicable.
 * @param instance associated with trace message.
 * @param xctx of caller.
 * @param format_z for message to log.
 * @param ... parameters for format.
 *
 * The trace_level 1 is the most commonly used level in Adaptive Framework.
 * Use 2 - 8 when a particular trace benefits from a different log priority.
 * For example, 2 - 8 could indicate more detail for a particular flag.
 */
#define afw_trace_fz(trace_level, flag_index, instance, xctx, format_z, ...) \
    if ((flag_index) < (xctx)->flags_count && (xctx)->flags[flag_index]) \
        afw_trace_write_fz( \
            afw_log_priority_trace##trace_level, \
            ((instance) ? \
                &((const afw_instance_t *)instance)->inf->rti \
                : NULL), \
            AFW__FILE_LINE__, xctx, format_z, __VA_ARGS__)


/**
 * @brief If applicable, write trace using a printf style format and va_list.
 * @param trace_level is a literal 1 - 8 added to "trace" to form log level.
 * @param flag_index of flag to check to determine if trace is applicable. 
 * @param instance associated with trace message.
 * @param format_z for message to log.
 * @param ap va_list for format.
 * @param xctx of caller.
 *
 * The trace_level 1 is the most commonly used level in Adaptive Framework.
 * Use 2 - 8 when a particular trace benefits from a different log priority.
 * For example, 2 - 8 could indicate more detail for a particular flag.
 */
#define afw_trace_vz(trace_level, flag_index, instance, format_z, ap, xctx) \
    if ((flag_index) < (xctx)->flags_count && (xctx)->flags[flag_index]) \
        afw_trace_write_vz( \
            afw_log_priority_trace##trace_level, \
            ((instance) ? \
                &((const afw_instance_t *)instance)->inf->rti \
                : NULL), \
            AFW__FILE_LINE__, format_z, ap, xctx)


/**
 * @brief If applicable, write trace from zero terminate string.
 * @param trace_level is a literal 1 - 8 added to "trace" to form log level.
 * @param flag_index of flag to check to determine if trace is applicable. 
 * @param instance associated with trace message.
 * @param message_z to log.
 * @param xctx of caller.
 *
 * The trace_level 1 is the most commonly used level in Adaptive Framework.
 * Use 2 - 8 when a particular trace benefits from a different log priority.
 * For example, 2 - 8 could indicate more detail for a particular flag.
 */
#define afw_trace_z(trace_level, flag_index, instance, message_z, xctx) \
    if ((flag_index) < (xctx)->flags_count && (xctx)->flags[flag_index]) \
        afw_trace_write_z( \
            afw_log_priority_trace##trace_level, \
            ((instance) ? \
                &((const afw_instance_t *)instance)->inf->rti \
                : NULL), \
            AFW__FILE_LINE__, message_z, xctx)


/**
 * @brief Write trace.
 * @param priority is log priority for log message (afw_log_priority_t)
 * @param rti associated with trace message.
 * @param source_z file:line.
 * @param message to log.
 * @param xctx of caller.
 *
 * Normally afw_trace() should be called instead.
 */
AFW_DECLARE(void)
afw_trace_write(
    afw_log_priority_t priority,
    const afw_interface_implementation_rti_t *rti,
    const afw_utf8_z_t *source_z,
    const afw_utf8_t *message,
    afw_xctx_t *xctx);


/**
 * @brief Write trace using a printf style format.
 * @param priority is log priority for log message (afw_log_priority_t)
 * @param rti associated with trace message.
 * @param source_z file:line.
 * @param xctx of caller.
 * @param format_z for message to log.
 * @param ... parameters for format.
 *
 * Normally afw_trace_fz() should be called instead.
 */
AFW_DECLARE(void)
afw_trace_write_fz(
    afw_log_priority_t priority,
    const afw_interface_implementation_rti_t *rti,
    const afw_utf8_z_t *source_z,
    afw_xctx_t *xctx,
    const afw_utf8_z_t *format_z,
    ...);


/**
 * @brief Write trace using a printf style format and va_list.
 * @param priority is log priority for log message (afw_log_priority_t)
 * @param rti associated with trace message.
 * @param source_z file:line.
 * @param format_z for message to log.
 * @param ap va_list for format.
 * @param xctx of caller.
 *
 * Normally afw_trace_vz() should be called instead.
 */
AFW_DECLARE(void)
afw_trace_write_vz(
    afw_log_priority_t priority,
    const afw_interface_implementation_rti_t *rti,
    const afw_utf8_z_t *source_z,
    const afw_utf8_z_t *format_z,
    va_list ap,
    afw_xctx_t *xctx);


/**
 * @brief Write trace from zero terminate string.
 * @param priority is log priority for log message (afw_log_priority_t)
 * @param rti associated with trace message.
 * @param source_z file:line.
 * @param message_z to log.
 * @param xctx of caller.
 *
 * Normally afw_trace_z() should be called instead.
 */
AFW_DECLARE(void)
afw_trace_write_z(
    afw_log_priority_t priority,
    const afw_interface_implementation_rti_t *rti,
    const afw_utf8_z_t *source_z,
    const afw_utf8_z_t *message_z,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */
/** @} */

#endif /* __AFW_TRACE_H__ */
