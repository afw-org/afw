// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_log interface for log director
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_log.c
 * @brief Implementation of afw_log interface for log director
 */

#include "afw_internal.h"



/* Declares and rti/inf defines for interface afw_log */
#define AFW_IMPLEMENTATION_ID "standard"
#include "afw_log_factory_impl_declares.h"
#include "afw_log_impl_declares.h"


typedef struct impl_afw_log_standard_self_s {
    afw_log_t pub;
    const afw_expression_t *expression;
} impl_afw_log_standard_self_t;


static const afw_utf8_t impl_factory_description =
AFW_UTF8_LITERAL("Log type for writing log stdout.");

/* Factory singleton instance. */
static const afw_log_factory_t impl_afw_log_factory =
{
    &impl_afw_log_factory_inf,
    AFW_UTF8_LITERAL(AFW_IMPLEMENTATION_ID),
    &impl_factory_description
};


/* Get the factory for log type file. */
AFW_DEFINE(const afw_log_factory_t *)
afw_log_standard_factory_get()
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
    impl_afw_log_standard_self_t *self;

    self = (impl_afw_log_standard_self_t *)afw_log_impl_create_cede_p(
        &impl_afw_log_inf, sizeof(impl_afw_log_standard_self_t),
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
    const afw_utf8_octet_t *s;
    afw_size_t len;
    afw_utf8_octet_t c;

    /* Loop writing octets to stdout. */
    for (s = message->s, len = message->len; len > 0; len--, s++) {
        c = *s;
        /* Convert anything < 32 to space. */
        if (c < 32 && c != '\n') {
            c = ' ';
        }
        if (putc(c, xctx->env->stdout_fd) == EOF) {
            /* Throwing error is a bad ideas so just break. */
            break;
        }
    }
    /* Write newline ignoring error. */
    putc('\n', xctx->env->stdout_fd);

    /* flush output */
    fflush(xctx->env->stdout_fd);
}
