// See the 'COPYING' file in the project root for licensing information.
/*
 * Helpers for afw_log implementation development
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_LOG_IMPL_H__
#define __AFW_LOG_IMPL_H__

#include "afw_minimal.h"

/**
 * @defgroup afw_log_impl afw_log
 * @ingroup afw_c_api_impl
 *
 * Interface log implementation helpers.
 * @{
 */

/**
 * @file afw_log_impl.h
 * @brief  Helpers for log implementation development.
 */

AFW_BEGIN_DECLARES

/**
 * @brief Struct for afw_log_impl_t.
 *
 * This struct is point to by afw_log_t and contains implementation specific
 * variables.
 */
struct afw_log_impl_s {
    
    /** brief associated log. */
    const afw_log_t *log;
    
    /** brief Log specific log:: variables (compiled properties). */
    const afw_object_t *log_variables;
    
    /** brief Log specific custom:: variables (compiled properties). */
    const afw_object_t *custom_variables;
    
    /** brief Compiled filter or NULL if no filter. */
    const afw_value_t *filter;
    
    /** brief Compiled format or NULL for default. */
    const afw_value_t *format;
  
    /** brief Log priority mask. */
    afw_log_priority_mask_t mask;
};



/**
 * @brief Developers should call this in all create functions for afw_log.
 * @param inf afw_log_inf_t pointer for implementation.
 * @param instance_size 0 or size greater than sizeof(afw_log_t).
 * @param properties config object.
 * @param p ceded for use by pool resources.  Log release, releases it.
 * @param xctx of caller.
 *
 * This function creates and initializes an afw_log instance.  This should be called
 * in the beginning of each log create function.  Afterwards, the afw_log
 * implementation can process additional properties, if needed.
 *
 * An instance_size can be specified to get a larger instance than
 * sizeof(afw_log_t) for private use.
 *
 * A new pool is created in the environment's pool for use by the log.
 *
 * Properties from config that are processed:
 *
 *    log_id    This is the log's id and can be used to reference this log,
 *              for example, to change its logging priority or report on on
 *              problems with the log. It will default to log_type, but
 *              must be unique.
 *
 *    priority   This is a string containing a `<priority>` from the table
 *               below.  This priority and higher that will be logged in
 *               this log.  This parameter can also have a value of "none"
 *               if all logging is to be controlled by `<priority>` properties.
 *               The default is notice.
 *
 *    `<priority>` Any `<priority>` can be set to true or false, overriding what
 *               is set or defaulted to by the priority property.
 *
 *    filter     expression -- future --
 *
 *    format     expression -- future ---
 *
 *    debug      expression -- future --- Note: maybe filter will do, but this
 *               might do priority="none",debug=true.  Maybe any of the
 *               `<priority>` can be true/false or a filter expression???
 *
 * `<priority>` in table below are listed highest priority to lowest.
 * `<priority>` trace1-8 are not available in many log configurations, such as
 * syslog.
 *
 *  `<priority>`  | Description
 *  ---------- | -----------
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
 */
AFW_DECLARE(afw_log_t *)
afw_log_impl_create_cede_p(
    const afw_log_inf_t *inf,
    afw_size_t instance_size,
    const afw_object_t *properties,
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Developers should call this for configuration property errors.
 * @param log.
 * @param property_name this is in error.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_log_impl_throw_property_invalid(
    const afw_log_t *log,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx);


/**
 * @brief Developers should call this for missing required configuration
 *     property.
 * @param log.
 * @param property_name this is in error.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_log_impl_throw_property_required(
    const afw_log_t *log,
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx);


/** @} */

AFW_END_DECLARES

#endif /* __AFW_LOG_IMPL_H__ */
