// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_log interface for log director
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_log.c
 * @brief Implementation of afw_log interface for log director
 */

#include "afw_internal.h"



/* Declares and rti/inf defines for interface afw_log */
#define AFW_IMPLEMENTATION_ID "file"
#include "afw_log_factory_impl_declares.h"
#include "afw_log_impl_declares.h"


typedef struct impl_log_self_s {
    afw_log_t pub;
    const afw_expression_t *expression;
} impl_afw_log_file_self_t;


static const afw_utf8_t impl_factory_description =
AFW_UTF8_LITERAL("Log type for writing log to a file.");

/* Factory singleton instance. */
static const afw_log_factory_t impl_afw_log_factory =
{
    &impl_afw_log_factory_inf,
    AFW_UTF8_LITERAL(AFW_IMPLEMENTATION_ID),
    &impl_factory_description
};


/* Get the factory for log type file. */
AFW_DEFINE(const afw_log_factory_t *)
afw_log_file_factory_get()
{
    return &impl_afw_log_factory;
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
    impl_afw_log_file_self_t *self;

    self = (impl_afw_log_file_self_t *)afw_log_impl_create_cede_p(
        &impl_afw_log_inf, sizeof(impl_afw_log_file_self_t),
        properties, p, xctx);

    /* Finish processing parameters implementation specific parameters. */

    /* Return new instance. */
    return (afw_log_t *)self;
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
    fprintf(xctx->env->stdout_fd, "FIXME log_type=file: " AFW_UTF8_FMT_Q "\n",
        AFW_UTF8_FMT_ARG(message));
}
