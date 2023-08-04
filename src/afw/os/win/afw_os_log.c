// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_log interface for Windows
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_os_log.c
 * @brief Implementation of afw_log interface for Windows
 */

#include "afw.h"
#include "afw_log_impl.h"

/* Declares and rti/inf defines for interface afw_log */
#define AFW_IMPLEMENTATION_ID "event_log"
#include "afw_log_impl_declares.h"
#include "afw_log_factory_impl_declares.h"

/* Typedef for afw_log self. */
typedef struct impl_afw_log_self_s {
    afw_log_t pub;

    /* Private implementation variables */

} impl_afw_os_log_self_;


static const afw_utf8_t impl_factory_description =
AFW_UTF8_LITERAL("Log type for writing to event log.");

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
AFW_DEFINE(const afw_log_t *) afw_os_log_create(
    const afw_object_t *properties,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    impl_afw_os_log_self_ *self;

    self = (impl_afw_os_log_self_ *)afw_log_impl_create_cede_p(
        &impl_afw_log_inf, sizeof(impl_afw_os_log_self_),
        properties, p, xctx);

    /* Finish processing parameters implementation specific parameters. */

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
    /* Assign instance pointer to self. */
    impl_afw_os_log_self_ * self =
        (impl_afw_os_log_self_ *)instance;
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
    /* Assign instance pointer to self. */
    impl_afw_os_log_self_ * self =
        (impl_afw_os_log_self_ *)instance;

    /** @fixme Write to event log. */
    fprintf(xctx->env->stderr_fd,
        "FIXME log_type=event_log: " AFW_UTF8_FMT "\n",
        AFW_UTF8_FMT_ARG(message));
}
