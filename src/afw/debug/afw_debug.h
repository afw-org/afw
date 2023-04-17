//>>> See COPYING at repository root
/*
 * Adaptive Framework Software Debug Header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_DEBUG_H__
#define __AFW_DEBUG_H__


/**
 * @addtogroup afw_debug Software Debug
 * @ingroup afw_c_api
 *
 * These macros and functions assist writing debug message to the environment
 * log.  All debug message have associated runtime information (rti) that can
 * be used for prefixing messages as well as filtering.  The first parameter
 * of macros AFW_DEBUG(), AFW_DEBUG_Z(), AFW_DEBUG_FZ(), and AFW_DEBUG_VZ()
 * is a instance, which is used to locate the implementation rti.  The
 * AFW_DEBUG_WRITE*() macros and afw_debug_write*() functions have rti as the
 * first parameter.
 *
 * All debug message are written to the environment log.  See afw_log.h for
 * more information.
 * @{
 */

/**
 * @file afw_debug.h
 * @brief Adaptive Framework Software Debug Header.
 */

#include "afw_interface.h"

AFW_BEGIN_DECLARES

/**
 * @brief Define a static debug_id rti.
 * @param label for the static debug_id rti.
 * @param debug_id
 *
 * The first parameter of afw_debug_write*() functions and AFW_DEBUG_WRITE*()
 * macros is an rti.  This macro defines a static rti that can be passed to
 * these for non-interface debugging.
 */
#define AFW_DEBUG_STATIC_DEBUG_ID(label, debug_id) \
static const afw_interface_implementation_rti_t label = \
    {"afw_debug_id", __FILE__, debug_id};


/**
 * @brief If applicable, write debug.
 * @param flag_index of flag to check to determine if debug is applicable.
 * @param instance associated with debug message.
 * @param message to log.
 * @param xctx of caller.
 */
#define afw_debug(flag_index, instance, message, xctx) \
    if ((flag_index) < (xctx)->flags_count && (xctx)->flags[flag_index]) \
        afw_debug_write( \
            ((instance) ? \
                &((const afw_instance_t *)instance)->inf->rti \
                : NULL), \
            AFW__FILE_LINE__, message, xctx)


/**
 * @brief If applicable, write debug using a printf style format.
 * @param flag_index of flag to check to determine if debug is applicable.
 * @param instance associated with debug message.
 * @param xctx of caller.
 * @param format_z for message to log.
 * @param ... parameters for format.
 */
#define afw_debug_fz(flag_index, instance, xctx, format_z, ...) \
    if ((flag_index) < (xctx)->flags_count && (xctx)->flags[flag_index]) \
        afw_debug_write_fz( \
            ((instance) ? \
                &((const afw_instance_t *)instance)->inf->rti \
                : NULL), \
            AFW__FILE_LINE__, xctx, format_z, __VA_ARGS__)
    /*
        afw_debug_write_fz( \
            (instance) ? &(instance)->inf->rti : NULL, \
            AFW__FILE_LINE__, xctx, format_z, __VA_ARGS__)
    */



/**
 * @brief If applicable, write debug using a printf style format and va_list.
 * @param flag_index of flag to check to determine if debug is applicable. 
 * @param instance associated with debug message.
 * @param format_z for message to log.
 * @param ap va_list for format.
 * @param xctx of caller.
 */
#define afw_debug_vz(flag_index, instance, format_z, ap, xctx) \
    if ((flag_index) < (xctx)->flags_count && (xctx)->flags[flag_index]) \
        afw_debug_write_vz( \
            ((instance) ? \
                &((const afw_instance_t *)instance)->inf->rti \
                : NULL), \
            AFW__FILE_LINE__, format_z, ap, xctx)


/**
 * @brief If applicable, write debug from zero terminate string.
 * @param flag_index of flag to check to determine if debug is applicable. 
 * @param instance associated with debug message.
 * @param message_z to log.
 * @param xctx of caller.
 */
#define afw_debug_z(flag_index, instance, message_z, xctx) \
    if ((flag_index) < (xctx)->flags_count && (xctx)->flags[flag_index]) \
        afw_debug_write_z( \
            ((instance) ? \
                &((const afw_instance_t *)instance)->inf->rti \
                : NULL), \
            AFW__FILE_LINE__, message_z, xctx)


/**
 * @brief Write debug.
 * @param rti associated with debug message.
 * @param source_z file:line.
 * @param message to log.
 * @param xctx of caller.
 *
 * Normally afw_debug() should be called instead.
 */
AFW_DECLARE(void)
afw_debug_write(
    const afw_interface_implementation_rti_t *rti,
    const afw_utf8_z_t *source_z,
    const afw_utf8_t *message,
    afw_xctx_t *xctx);


/**
 * @brief Write debug using a printf style format.
 * @param rti associated with debug message.
 * @param source_z file:line.
 * @param xctx of caller.
 * @param format_z for message to log.
 * @param ... parameters for format.
 *
 * Normally afw_debug_fz() should be called instead.
 */
AFW_DECLARE(void)
afw_debug_write_fz(
    const afw_interface_implementation_rti_t *rti,
    const afw_utf8_z_t *source_z,
    afw_xctx_t *xctx,
    const afw_utf8_z_t *format_z,
    ...);


/**
 * @brief Write debug using a printf style format and va_list.
 * @param rti associated with debug message.
 * @param source_z file:line.
 * @param format_z for message to log.
 * @param ap va_list for format.
 * @param xctx of caller.
 *
 * Normally afw_debug_vz() should be called instead.
 */
AFW_DECLARE(void)
afw_debug_write_vz(
    const afw_interface_implementation_rti_t *rti,
    const afw_utf8_z_t *source_z,
    const afw_utf8_z_t *format_z,
    va_list ap,
    afw_xctx_t *xctx);


/**
 * @brief Write debug from zero terminate string.
 * @param rti associated with debug message.
 * @param source_z file:line.
 * @param message_z to log.
 * @param xctx of caller.
 *
 * Normally afw_debug_z() should be called instead.
 */
AFW_DECLARE(void)
afw_debug_write_z(
    const afw_interface_implementation_rti_t *rti,
    const afw_utf8_z_t *source_z,
    const afw_utf8_z_t *message_z,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_DEBUG_H__ */
