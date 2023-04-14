// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework afw_log interface helpers header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_LOG_H__
#define __AFW_LOG_H__


/** @addtogroup afw_c_api
 * @{
 */


/** @fixme In middle of implementation. */
#define afw_log_release afw_log_destroy



/** @addtogroup afw_log Log
 *
 *  Adaptive Framework Log Support.
 *
 *  Log priorities are defined in enum afw_log_priority_t in afw_interface.h.
 *  The AFW_LOG macros accept the last part of the enum value name as the
 *  priority parameter.
 *
 *  Log priority trace1-8 are not logged, but instead goes to stderr or the
 *  configured trace destination.
 *
 *  priority   | Description
 *  ---------- | -----------
 *  none       | Priorities will be set individually
 *  emerg      | System unusable
 *  alert      | Immediate action required
 *  crit       | Critical conditions
 *  err        | Error conditions
 *  warning    | Warning, errors may occur
 *  notice     | Normal but significant
 *  info       | Informational
 *  debug      | Debug
 *  trace1     | Trace level 1 message
 *  trace2     | Trace level 2 message
 *  trace3     | Trace level 3 message
 *  trace4     | Trace level 4 message
 *  trace5     | Trace level 5 message
 *  trace6     | Trace level 6 message
 *  trace7     | Trace level 7 message
 *  trace8     | Trace level 8 message
 *
 * @{
 */

/**
 * @file afw_log.h
 * @brief Adaptive Framework afw_log interface helpers header
 */

#include "afw_interface.h"

AFW_BEGIN_DECLARES

/** priority_id/priority map entry. */
typedef struct afw_log_priority_id_map_entry_s {
    const afw_utf8_t *priority_id;
    afw_log_priority_t priority;
    const afw_utf8_z_t *description;
} afw_log_priority_id_map_entry_t;


/** Get priority_id/priority map array. */
AFW_DECLARE(const afw_log_priority_id_map_entry_t *)
afw_log_get_priority_id_map();


/**
 * @brief Convert a log priority id to priority.
 * @param id of log priority to convert
 * @return enum corresponding to log priority or -1 if not found.
 */
AFW_DECLARE(afw_log_priority_t)
afw_log_priority_id_to_priority(
    const afw_utf8_t *priority_id);


/**
 * @brief Convert a log priority to priority id.
 * @param priority of log priority enum to convert
 * @return priority_id or NULL if invalid.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_log_priority_to_priority_id(
    afw_log_priority_t priority);


/**
 * @brief Get the priority mask for a log priority.
 * @param priority enum to convert
 * @return priority_mask.
 */
AFW_DEFINE_STATIC_INLINE(afw_log_priority_mask_t)
afw_log_priority_mask(
    afw_log_priority_t priority)
{
    return 1 << priority;
}

/**
 * @brief Get the priority mask for all equal or higher log priorities.
 * @param priority enum of lowest priority for mask.
 * @return priority_mask.
 */
AFW_DEFINE_STATIC_INLINE(afw_log_priority_mask_t)
afw_log_up_to_priority_mask(
    afw_log_priority_t priority)
{
    return (1 << (priority + 1) ) - 1;
}

/**
 * @brief Determined if log priority corresponding bit is on in mask.
 * @param mask to check.
 * @param priority to test.
 * @return true or false.
 */
AFW_DEFINE_STATIC_INLINE(afw_boolean_t)
afw_log_priority_in_mask(
    afw_log_priority_mask_t mask, afw_log_priority_t priority)
{
    return ((1 << priority) & mask) != 0;
}

/**
 * @brief Set the corresponding bit for a priority in a mask.
 * @param mask to check.
 * @param priority to test.
 * @param value true or false.
 * @return true or false.
 */
AFW_DEFINE_STATIC_INLINE(void)
afw_log_set_priority_in_mask(
    afw_log_priority_mask_t *mask,
    afw_log_priority_t priority,
    afw_boolean_t value)
{
    if (value) {
        *mask |= (1 << priority);
    }

    else {
        *mask &= ~(1 << priority);
    }
}

/**
 * @brief Log an afw_utf8_t message to environment's log.
 * @param priority of message. See table above.
 * @param message to log.
 * @param xctx of caller.
 */
#define AFW_LOG(priority, message, xctx) \
    afw_log_write(xctx->env->log, afw_log_priority_ ## priority, \
        AFW__FILE_LINE__, message, xctx)

/**
 * @brief Log an afw_utf8_z_t message to environment's log.
 * @param priority of message. See table above.
 * @param message to log.
 * @param xctx of caller.
 */
#define AFW_LOG_Z(priority, message_z, xctx) \
    afw_log_write_z(xctx->env->log, afw_log_priority_ ## priority, \
        AFW__FILE_LINE__, message_z, xctx)

/**
 * @brief Log an message to environment's log using a printf style format and
 *    va_list.
 * @param priority of message. See table above.
 * @param format_z for message to log.
 * @param ap va_list for format.
 * @param xctx of caller.
 */
#define AFW_LOG_VZ(priority, format_z, ap, xctx) \
    afw_log_write_vz(xctx->env->log, afw_log_priority_ ## priority, \
        AFW__FILE_LINE__, format_z, ap, xctx)

/**
 * @brief Log an message to environment's log using a printf style format
 *    and parameters.
 * @param priority of message. See table above.
 * @param xctx of caller.
 * @param format_z for message to log.
 * @param ... parameters for format.
 */
#define AFW_LOG_FZ(priority, xctx, format_z, ...) \
    afw_log_write_fz(xctx->env->log, afw_log_priority_ ## priority, \
        AFW__FILE_LINE__, xctx, format_z, __VA_ARGS__)

 
/**
 * @brief Log an afw_utf8_z_t message.
 * @param instance of log
 * @param priority afw_log_priority_t
 * @param source_z file:line.
 * @param message_z message to log.
 * @param xctx of caller.
 */
AFW_DEFINE_STATIC_INLINE(void)
afw_log_write_z(
    const afw_log_t * instance,
    afw_log_priority_t priority,
    const afw_utf8_z_t * source_z,
    const afw_utf8_z_t * message_z,
    afw_xctx_t *xctx)
{
    afw_utf8_t message;

    message.s = message_z;
    message.len = strlen(message_z);
    afw_log_write(instance, priority, source_z, &message,
        xctx);
}

/**
 * @brief Log an message using a printf style format and va_list.
 * @param instance of log
 * @param priority afw_log_priority_t
 * @param source_z file:line.
 * @param format_z for message to log.
 * @param ap va_list for format.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_log_write_vz(
    const afw_log_t * instance,
    afw_log_priority_t priority,
    const afw_utf8_z_t * source_z,
    const afw_utf8_z_t * format_z,
    va_list ap,
    afw_xctx_t *xctx);

/**
 * @brief Log an message using a printf style format and parameters.
 * @param instance of log
 * @param priority afw_log_priority_t
 * @param source_z file:line.
 * @param xctx of caller.
 * @param format_z for message to log.
 * @param ... parameters for format.
 */
AFW_DEFINE_STATIC_INLINE(void)
afw_log_write_fz(
    const afw_log_t * instance,
    afw_log_priority_t priority,
    const afw_utf8_z_t * source_z,
    afw_xctx_t *xctx,
    const afw_utf8_z_t * format_z,
    ...)
{
    va_list ap;

    va_start(ap, format_z);
    afw_log_write_vz(instance, priority, source_z, format_z,
        ap, xctx);
    va_end(ap);
}


/**
 * @brief Get the factory for log type file.
 * @return factory singleton instance.
 */
AFW_DECLARE(const afw_log_factory_t *)
afw_log_file_factory_get();



/**
 * @brief Get the factory for log type standard.
 * @return factory singleton instance.
 */
AFW_DECLARE(const afw_log_factory_t *)
afw_log_standard_factory_get();



/**
 * @brief Add a log to the list of logs called by environment log.
 * @param instance log to add.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_log_add_to_environment(
    const afw_log_t * instance,
    afw_xctx_t *xctx);


/*
 * @brief Set a log's mask and insure env->mask is correct.
 * @param instance
 * @param mask
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_log_set_mask(
    const afw_log_t * instance,
    afw_log_priority_mask_t mask,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */
/** @} */


#endif /* __AFW_LOG_H__ */
