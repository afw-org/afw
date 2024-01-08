// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_log interface for *nix systems
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_os_log.c
 * @brief Implementation of afw_log interface for *nix systems
 */

#include "afw.h"
#include "afw_log_impl.h"

/* Declares and rti/inf defines for interface afw_log_factory and afw_log. */
#define AFW_IMPLEMENTATION_ID "syslog"
#include "afw_log_factory_impl_declares.h"
#include "afw_log_impl_declares.h"
#include "afw_log_impl.h"

#include <syslog.h>

static const int afw_syslog_map[] = {
    [afw_log_priority_emerg]   = LOG_EMERG,
    [afw_log_priority_alert]   = LOG_ALERT,
    [afw_log_priority_crit]    = LOG_CRIT,
    [afw_log_priority_err]     = LOG_ERR,
    [afw_log_priority_warning] = LOG_WARNING,
    [afw_log_priority_notice]  = LOG_NOTICE,
    [afw_log_priority_info]    = LOG_INFO,
    [afw_log_priority_debug]   = LOG_DEBUG,
    [afw_log_priority_trace1]  = LOG_DEBUG,
    [afw_log_priority_trace2]  = LOG_DEBUG,
    [afw_log_priority_trace3]  = LOG_DEBUG,
    [afw_log_priority_trace4]  = LOG_DEBUG,
    [afw_log_priority_trace5]  = LOG_DEBUG,
    [afw_log_priority_trace6]  = LOG_DEBUG,
    [afw_log_priority_trace7]  = LOG_DEBUG,
    [afw_log_priority_trace8]  = LOG_DEBUG
};

/* Typedef for afw_log self. */
typedef struct impl_afw_os_log_self_s {
    afw_log_t pub;

    /* Private implementation variables */

} impl_afw_os_log_self_t;


static const afw_utf8_t impl_factory_description =
AFW_UTF8_LITERAL("Log type for writing to syslog.");

/* Factory singleton instance. */
static const afw_log_factory_t impl_afw_log_factory =
{
    &impl_afw_log_factory_inf,
    AFW_UTF8_LITERAL(AFW_IMPLEMENTATION_ID),
    &impl_factory_description
};


/* Get the factory for log type file. */
AFW_DEFINE(const afw_log_factory_t *)
afw_os_log_factory_get()
{
    return &impl_afw_log_factory;
}


/* Create for os_log */
AFW_DEFINE(const afw_log_t *)
afw_os_log_create(
    const afw_object_t *properties,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    impl_afw_os_log_self_t *self;

    self = (impl_afw_os_log_self_t *)afw_log_impl_create_cede_p(
        &impl_afw_log_inf, sizeof(impl_afw_os_log_self_t),
        properties, p, xctx);
    int facility = LOG_DAEMON;
    char *identifier = NULL;

    /* FIXME Finish processing parameters implementation specific parameters. */

    /* open for logging */
    openlog(identifier, LOG_PID, facility);

    /* Return new instance. */
    return (afw_log_t *)self;
}



/*
 * Implementation of method create_log_cede_p of interface afw_log_factory.
 */
const afw_log_t *
impl_afw_log_factory_create_log_cede_p (
    const afw_log_factory_t * instance,
    const afw_object_t * properties,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    return afw_os_log_create(properties, p, xctx);
}



/*
 * Implementation of method destroy of interface afw_log.
 */
void
impl_afw_log_destroy(
    const afw_log_t * instance,
    afw_xctx_t *xctx)
{
    /* 
        If you are writing shared library code that uses openlog to 
        generate custom syslog output, you should use closelog to drop 
        the GNU C Library’s internal reference to the ident pointer 
        when you are done.
     */
    closelog();

    afw_pool_release(instance->p, xctx);
}



/*
 * Implementation of method set_mask of interface afw_log.
 */
void
impl_afw_log_set_own_mask(
    const afw_log_t * instance,
    afw_log_priority_mask_t mask,
    afw_xctx_t *xctx)
{
    afw_log_impl_t *impl = (afw_log_impl_t *)instance->impl;

    /* Set mask. */
    impl->mask = mask;
}



/*
 * Implementation of method write of interface afw_log.
 */
void
impl_afw_log_write(
    const afw_log_t * instance,
    afw_log_priority_t priority,
    const afw_utf8_z_t * source_z,
    const afw_utf8_t * message,
    afw_xctx_t *xctx)
{
    int syslog_priority;

    syslog_priority = afw_syslog_map[priority];

    syslog(syslog_priority, AFW_UTF8_FMT, AFW_UTF8_FMT_ARG(message));   
}
