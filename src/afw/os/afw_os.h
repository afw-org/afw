// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework OS Specific Header for unix-like systems
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_OS_H__
#define __AFW_OS_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_os
 * @{
 */

/**
 * @file afw_os.h
 * @brief Adaptive Framework OS Specific Header for unix-like systems 
 */

AFW_BEGIN_DECLARES

/**
 * @brief afw_os environment initialize
 * @param xctx of caller
 *
 * This is called when the Adaptive Framework is being initialized.  Pool
 * xctx->p will last for the life of the environment.  Any
 * afw_environment_register functions can be called from here.  One possible
 * use is to register an afw_log_factory so that the system's log can
 * be configured by environment configuration.
 */
AFW_DECLARE(void)
afw_os_environment_initialize(afw_xctx_t *xctx);


/**
 * @brief Return a process id or similar number.
 * @return process id or similar number specific to system.
 */
AFW_DECLARE(afw_uint32_t)
afw_os_get_pid();


/**
 * @brief Current resident set size for this process.
 * @return RSS in kilobytes, or 0 if unavailable.
 *
 * Linux: `/proc/self/statm` resident pages (open/read/close, no alloc).
 * Same units as `ru_maxrss` on Linux. Use this on `>debug pool` lines and
 * when comparing two samples; peak is `afw_os_get_maxrss()`. Win stubs 0.
 *
 * Do not call this from gdb after SIGSTOP — read `/proc/<pid>/status`
 * VmRSS from the gdb process instead.
 */
AFW_DECLARE(afw_size_t)
afw_os_get_rss();


/**
 * @brief Maximum resident set size for this process so far.
 * @return ru_maxrss from getrusage(RUSAGE_SELF), or 0 if unavailable.
 *
 * On Linux this is kilobytes (peak, never drops). Must not allocate.
 * Win stubs 0. Prefer `afw_os_get_rss()` for live leak samples.
 */
AFW_DECLARE(afw_size_t)
afw_os_get_maxrss();


/**
 * @brief Return the suffix appended to dso file names for this system. 
 * @return suffix such as ".so" or ".dll".
 */
AFW_DECLARE(const afw_utf8_t *)
afw_os_get_dso_suffix();


/**
 * @brief Create an instance of the OS log.
 * @param properties
 * @param p to use for log's resources.
 * @param xctx of caller.
 * @return log instance
 */
AFW_DECLARE(const afw_log_t *)
afw_os_log_create(
    const afw_object_t * properties,
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Get the factory for OS log.
 * @return factory singleton instance.
 */
AFW_DECLARE(const afw_log_factory_t *)
afw_os_log_factory_get();


/**
 * @brief Provide a backtrace if possible.
 * @param code Error code or 0.  Code afw_error_code_memory or others might
 *    influence whether producing backtrace is possible.
 * @param max_backtrace
 * @param xctx of caller.
 * @return backtrace or NULL if not possible.
 *
 * IMPORTANT Implementation of this function must not do anything that might
 * throw an error or change xctx->error or a recursive error producing a
 * stack overflow will occur.
 *
 * If max_backtrace is -1 or greater than the implementation's max, the max is
 * used.  If 0 is specifed, no trace occurs.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_os_backtrace(
    afw_error_code_t code,
    int max_backtrace,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */  // end of @addtogroup @addtogroup

#endif /* __AFW_OS_H__ */
