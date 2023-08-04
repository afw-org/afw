// See the 'COPYING' file in the project root for licensing information.
/*
 * AFW XACML - Basic Log Implementation.
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_log_deprecated.c
 * @brief Basic Log Implementation.
 */

#define _AFW_LOGIMPL
#include "afw_log_deprecated.h"

static void log_release(
    afw_log_deprecated_t *log
);

static void log_message_v(
    afw_log_deprecated_t *log,
    apr_pool_t *temp_pool,
    const afw_utf8_z_t *prefix,
    const afw_utf8_z_t *fmt,
    va_list args
);

static void log_error_v(
    afw_log_deprecated_t *log,
    apr_pool_t *temp_pool,
    const afw_utf8_z_t *prefix,
    const afw_utf8_z_t *fmt,
    va_list args);

static void log_trace_v(
    afw_log_deprecated_t *log,
    apr_pool_t *temp_pool,
    const afw_utf8_z_t *prefix,
    const afw_utf8_z_t *fmt,
    va_list args);

static const afw_log_deprecated_inf_t afw_log_deprecated_inf = {
    &log_release,
    &log_message_v,
    &log_error_v,
    &log_trace_v
};

typedef struct afw_log_deprecated_internal_t {
    afw_log_deprecated_t c;
    apr_pool_t *log_pool;
    apr_file_t *log_file;
    const char *encoding;
} afw_log_deprecated_internal_t;

/** @fixme Need to add convert to log code page. */

/* Create a log. */
AFW_DEFINE(afw_log_deprecated_t *)
afw_create_log_deprecated(
    apr_pool_t *temp_pool,
    apr_pool_t *module_pool,
    apr_file_t *log_file,
    const char *encoding)
{
    afw_log_deprecated_internal_t *self = NULL;
    apr_pool_t *log_pool;

    /* Create subpool for log. */
    if (apr_pool_create(&log_pool,module_pool) != APR_SUCCESS) {
        apr_file_printf(log_file,  AFW_MESSAGE_PREFIX
            "apr_pool_create() failed in afw_create_log_deprecated().");
        return NULL;
    };

    /* Get storage and initialize afw_log_deprecated_t struct. */
    self = (afw_log_deprecated_internal_t *)apr_pcalloc(log_pool, sizeof(afw_log_deprecated_internal_t));
    self->c.inf = &afw_log_deprecated_inf;
    self->log_pool = log_pool;
    self->log_file = log_file;
    self->encoding = apr_pstrdup(log_pool, encoding);

    /* Return with rc=0. */
    return (afw_log_deprecated_t *)self;
}

/* Release a log.  */
void log_release(afw_log_deprecated_t *log)
{
    afw_log_deprecated_internal_t *self = (afw_log_deprecated_internal_t *)log;

    /* Destroy log_pool and clear pointer. */
    apr_pool_destroy(self->log_pool);
}

void log_message_v(
    afw_log_deprecated_t *log,
    apr_pool_t *temp_pool,
    const afw_utf8_z_t *prefix,
    const afw_utf8_z_t *fmt,
    va_list args)
{
    afw_log_deprecated_internal_t *self = (afw_log_deprecated_internal_t *)log;
    apr_file_puts(apr_pvsprintf(temp_pool, (const char *)fmt, args), self->log_file);
    apr_file_putc('\n', self->log_file);
}

void log_error_v(
    afw_log_deprecated_t *log,
    apr_pool_t *temp_pool,
    const afw_utf8_z_t *prefix,
    const afw_utf8_z_t *fmt,
    va_list args)
{
    afw_log_deprecated_internal_t *self = (afw_log_deprecated_internal_t *)log;
    apr_file_puts(apr_pvsprintf(temp_pool, (const char *)fmt, args), self->log_file);
    apr_file_putc('\n', self->log_file);
}

void log_trace_v(
    afw_log_deprecated_t *log,
    apr_pool_t *temp_pool,
    const afw_utf8_z_t *prefix,
    const afw_utf8_z_t *fmt,
    va_list args)
{
    afw_log_deprecated_internal_t *self = (afw_log_deprecated_internal_t *)log;
    apr_file_puts(apr_pvsprintf(temp_pool, (const char *)fmt, args), self->log_file);
    apr_file_putc('\n', self->log_file);
}
