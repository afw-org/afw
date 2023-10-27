// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework pool.
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_pool.c
 * @brief Adaptive Framework pool.
 */

#include "afw_internal.h"


AFW_DEFINE_INTERNAL(void)
afw_pool_internal_add_child(
    afw_pool_internal_self_t *parent,
    afw_pool_internal_self_t *child, afw_xctx_t *xctx)
{
    afw_pool_get_reference(&parent->pub, xctx);

    child->next_sibling = parent->first_child;
    parent->first_child = child;
}


AFW_DEFINE_INTERNAL(void)
afw_pool_internal_remove_as_child(
    afw_pool_internal_self_t *parent,
    afw_pool_internal_self_t *child,
    afw_xctx_t *xctx)
{
    afw_pool_internal_self_t *prev;
    afw_pool_internal_self_t *sibling;

    for (prev = NULL, sibling = parent->first_child;
        sibling;
        prev = sibling, sibling = sibling->next_sibling)
    {
        if (sibling == child) {
            if (!prev) {
                parent->first_child = sibling->next_sibling;
            }
            else {
                prev->next_sibling = sibling->next_sibling;
            }
            break;
        }
    }

    if (!sibling) {
        AFW_THROW_ERROR_Z(general, "Not a child of parent", xctx);
    }

    afw_pool_release(&parent->pub, xctx);
}


/* Create skeleton pool struct. */
AFW_DEFINE_INTERNAL(afw_pool_internal_self_t *)
afw_pool_internal_create(
    afw_pool_internal_self_t *parent,
    const afw_pool_inf_t *inf,
    afw_size_t instance_size,
    afw_xctx_t *xctx)
{
    apr_pool_t *apr_p;
    afw_pool_internal_self_t *self;

    apr_pool_create(&apr_p, (parent) ? parent->apr_p : NULL);
    if (!apr_p) {
        AFW_THROW_ERROR_Z(memory, "Unable to allocate pool", xctx);
    }
    self = apr_pcalloc(apr_p, instance_size);
    if (!self) {
        AFW_THROW_ERROR_Z(memory, "Unable to allocate pool", xctx);
    }
    self->pub.inf = inf;
    self->apr_p = apr_p;
    self->parent = parent;
    self->pool_number = afw_atomic_integer_increment(
        &((afw_environment_t *)xctx->env)->pool_number);

    /* If parent, add this new child. */
    if (parent) {
        afw_pool_internal_add_child(parent, self, xctx);
    }

    /* Return new pool. */
    return self;
}
