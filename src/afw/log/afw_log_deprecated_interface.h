// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework - Log Interface Header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFWLOGINTERFACE_H__
#define __AFWLOGINTERFACE_H__


/**
 * @addtogroup afw_log_deprecated
 * @{
 */

/**
 * @file afw_log_deprecated.h
 * @brief Header file for afw_log_deprecated.c.
 */

#include "afw_minimal.h"

/** Created by afw_create_log_deprecated */
typedef struct afw_log_deprecated_t afw_log_deprecated_t;

/** Trace Options. */
typedef struct afw_log_deprecated_trace_options_t {
    /** @fixme Figure this out with Jeremy. */
    int debug;
} afw_log_deprecated_trace_options_t;

/** @sa afwLogRelease() */
typedef void (*afw_log_deprecated_release_t)(
    afw_log_deprecated_t *log
);

/** @sa afw_log_deprecated_message_v() */
typedef void (*afw_log_deprecated_message_v_t)(
    afw_log_deprecated_t *log,
    apr_pool_t *temp_pool,
    const afw_utf8_z_t *prefix,
    const afw_utf8_z_t *fmt,
    va_list args
);

/** @sa afw_log_deprecated_error_v() */
typedef void (*afw_log_deprecated_error_v_t)(
    afw_log_deprecated_t *log,
    apr_pool_t *temp_pool,
    const afw_utf8_z_t *prefix,
    const afw_utf8_z_t *fmt,
    va_list args);

/** @sa afw_log_deprecated_trace_v() */
typedef void (*afw_log_deprecated_trace_v_t)(
    afw_log_deprecated_t *log,
    apr_pool_t *temp_pool,
    const afw_utf8_z_t *prefix,
    const afw_utf8_z_t *fmt,
    va_list args);

/** @brief afw_log_deprecated_t interface. */
typedef struct afw_log_deprecated_inf_t {
    afw_log_deprecated_release_t release;
    afw_log_deprecated_message_v_t log_message;
    afw_log_deprecated_error_v_t   log_error;
    afw_log_deprecated_trace_v_t   log_trace;
} afw_log_deprecated_inf_t;

/** @brief Public afw_log_deprecated_t object. */
struct afw_log_deprecated_t {
    const afw_log_deprecated_inf_t *inf;
    afw_log_deprecated_trace_options_t topt;
};

/**
 * @brief Release a log.
 * @param log: Log handle pointer.
 */
AFW_DEFINE_STATIC_INLINE(void) afw_release_log(afw_log_deprecated_t *log)
{
    log->inf->release(log);
}

/**
 * @brief Issue a message.
 * @param log Log handle.
 * @param temp_pool used for life of this call.
 * @param fmt format string.
 * @param args substitution values for fmt.
 */
AFW_DEFINE_STATIC_INLINE(void) afw_log_deprecated_message_v(
    afw_log_deprecated_t *log,
    apr_pool_t *temp_pool,
    const afw_utf8_z_t *prefix,
    const afw_utf8_z_t *fmt,
    va_list args)
{
    log->inf->log_message(log, temp_pool, prefix, fmt, args);
}

/**
 * @brief Issue a message.
 * @param log Log handle.
 * @param temp_pool used for life of this call.
 * @param fmt format string.
 * @param ... substitution values for fmt.
 */
AFW_DEFINE_STATIC_INLINE(void) afw_log_deprecated_message(
    afw_log_deprecated_t *log,
    apr_pool_t *temp_pool,
    const afw_utf8_z_t *prefix,
    const afw_utf8_z_t *fmt,
    ...)
{
    va_list args;
    va_start(args, fmt);
    log->inf->log_message(log, temp_pool, prefix, fmt, args);
    va_end(args);
}

/**
 * @brief Issue an error message.
 * @param log Log handle.
 * @param temp_pool used for life of this call.
 * @param fmt format string.
 * @param args substitution values for fmt.
 */
AFW_DEFINE_STATIC_INLINE(void) afw_log_deprecated_error_v(
    afw_log_deprecated_t *log,
    apr_pool_t *temp_pool,
    const afw_utf8_z_t *prefix,
    const afw_utf8_z_t *fmt,
    va_list args)
{
    log->inf->log_error(log, temp_pool, prefix, fmt, args);
}

/**
 * @brief Issue an error message.
 * @param log Log handle.
 * @param temp_pool used for life of this call.
 * @param fmt format string.
 * @param ... substitution values for fmt.
 */
AFW_DEFINE_STATIC_INLINE(void) afw_log_deprecated_error(
    afw_log_deprecated_t *log,
    apr_pool_t *temp_pool,
    const afw_utf8_z_t *prefix,
    const afw_utf8_z_t *fmt,
    ...)
{
    va_list args;
    va_start(args, fmt);
    log->inf->log_error(log, temp_pool, prefix, fmt, args);
    va_end(args);
}

/**
 * @brief Issue a trace message.
 * @param log Log handle.
 * @param temp_pool Used for life of this call.
 * @param fmt format string.
 * @param args substitution values for fmt.
 */
AFW_DEFINE_STATIC_INLINE(void) afw_log_deprecated_trace_v(
    afw_log_deprecated_t *log,
    apr_pool_t *temp_pool,
    const afw_utf8_z_t *prefix,
    const afw_utf8_z_t *fmt,
    va_list args)
{
    log->inf->log_trace(log, temp_pool, prefix, fmt, args);
}

/**
 * @brief Issue a trace message.
 * @param log Log handle.
 * @param temp_pool Used for life of this call.
 * @param fmt format string.
 * @param ... substitution values for fmt.
 */
AFW_DEFINE_STATIC_INLINE(void) afw_log_deprecated_trace(
    afw_log_deprecated_t *log,
    apr_pool_t *temp_pool,
    const afw_utf8_z_t *prefix,
    const afw_utf8_z_t *fmt,
    ...)
{
    va_list args;
    va_start(args, fmt);
    log->inf->log_trace(log, temp_pool, prefix, fmt, args);
    va_end(args);
}

/** @} */

#endif /* __AFWLOGINTERFACE_H__ */
