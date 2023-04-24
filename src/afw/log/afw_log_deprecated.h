// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework - Log Header for Basic Log  DEPRECATED
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_LOG_DEPRECATED_H__
#define __AFW_LOG_DEPRECATED_H__

#include "afw_log_deprecated_interface.h"
#include <apr_file_io.h>

/**
 * @defgroup afw_log_deprecated Log (deprecated)
 * @ingroup afw_c_api_public
 *
 * Adaptive Framework log.
 * 
 * @{
 */

/**
 * @file afw_log_deprecated.h
 * @brief Header file for afw_log_deprecated.c.
 */

AFW_BEGIN_DECLARES

/**
 * @brief Create a basic log.
 * @param temp_pool: Used for life of this call.
 * @param module_pool: Used for life of this log.
 * @param file: Log file.
 * @param encoded: encoding for output or NULL
 * @param trace: Trace options.
 * @return afw_log_deprecated_t * or NULL if error.
 */
AFW_DECLARE(afw_log_deprecated_t *)
afw_create_log_deprecated(
    apr_pool_t *temp_pool,
    apr_pool_t *module_pool,
    apr_file_t *log_file,
    const char *encoding
);

AFW_END_DECLARES

/** @} */

#endif /* __AFW_LOG_DEPRECATED_H__ */
