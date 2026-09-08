// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Stack Support
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_stack.c
 * @brief Growable stack implementation used by compiler and runtime.
 */

#include "afw_internal.h"



typedef struct impl_stack_self_s {
    afw_stack_t pub;
    afw_size_t initial_count;
    afw_size_t maximum_count;
    afw_size_t entry_size;
    afw_boolean_t create_own_pool;
} impl_stack_self_t;



/* Create a stack during xctx creation. */
void
afw_stack_internal_set_qualifier_stack(
    afw_xctx_t *xctx)
{
    /*
     * Fixed size: entry pointers stay valid. Early xctx create cannot
     * use AFW_TRY / afw_pool_calloc().
     */
    xctx->qualifier_stack = afw_vector_create_fixed_unhandled(
        afw_xctx_qualifier_stack_t, 100, xctx->p, xctx);
}



/* Create a stack during xctx creation. */
void
afw_stack_internal_set_evaluation_stack(
    afw_xctx_t *xctx)
{
    afw_size_t n;

    /*
     * Allocate the cap up front so the vector never grows (entry
     * pointers stay valid). Early xctx create cannot use AFW_TRY.
     */
    n = xctx->env->evaluation_stack_maximum_count;
    if (n == 0) {
        n = xctx->env->evaluation_stack_initial_count;
    }
    if (n == 0) {
        n = AFW_ENVIRONMENT_DEFAULT_EVALUATION_STACK_MAXIMUM_COUNT;
    }
    xctx->evaluation_stack = afw_vector_create_fixed_unhandled(
        afw_xctx_evaluation_stack_t, n, xctx->p, xctx);
}


/* Create a stack. */
AFW_DEFINE(afw_stack_t *)
afw_stack_create_impl(
    afw_size_t entry_size,
    afw_size_t initial_count,
    afw_size_t maximum_count,
    afw_boolean_t create_own_pool,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    impl_stack_self_t *self;

    if (create_own_pool) {
        p = afw_pool_create(p, xctx);
    }

    self = afw_pool_calloc_type(p, impl_stack_self_t, xctx);
    self->pub.p = p;
    self->initial_count = initial_count;
    self->entry_size = entry_size;
    self->maximum_count = maximum_count;
    self->create_own_pool = create_own_pool;

    self->pub.first = afw_pool_calloc(p, initial_count * entry_size, xctx);
    self->pub.top = (afw_octet_t *)self->pub.first - entry_size;
    self->pub.end = (afw_octet_t *)self->pub.first +
        (entry_size * initial_count);

    return &self->pub;
}


/* Release stack. */
AFW_DEFINE(void)
afw_stack_release_impl(
    afw_stack_t * instance,
    afw_xctx_t *xctx)
{
    impl_stack_self_t *self =
        (impl_stack_self_t *)instance;

    /* If this stack owns a child pool, release it. */
    if (self->create_own_pool) {
        afw_pool_release(self->pub.p, xctx);
    }
}



/* Copy stack. */
AFW_DEFINE(void)
afw_stack_copy_impl(
    afw_stack_t *instance,
    afw_size_t *count,
    const void ***ptr,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    impl_stack_self_t *self = (impl_stack_self_t *)instance;
    afw_size_t size;

    *ptr = NULL;
    size = (afw_octet_t *)instance->top - (afw_octet_t *)instance->first +
        self->entry_size;
    if (count) {
        *count = size / self->entry_size;
    }
    if (size > 0) {
        *ptr = afw_pool_malloc(p, size, xctx);
        memcpy((void *)*ptr, instance->first, size);
    }
}



/*
 * Implementation of method extend for interface afw_stack.
 */
AFW_DEFINE(void)
afw_stack_extend_impl(
    afw_stack_t * instance,
    afw_xctx_t *xctx)
{
    impl_stack_self_t *self =
        (impl_stack_self_t *)instance;

    afw_size_t count, new_count;
    afw_octet_t *new_first;

    count = ((afw_octet_t *)self->pub.end - (afw_octet_t *)self->pub.first) /
        self->entry_size;

    if (self->maximum_count == 0) {
        new_count = count * 2;
    }
    else {
        if (count >= self->maximum_count) {
            /*
             * Make this adjustment so that evaluation backtrace will not
             * calculate top index to be past end.
             */
            self->pub.top = (afw_octet_t *)self->pub.top - self->entry_size;
            AFW_THROW_ERROR_Z(general, "Stack max_count exceeded", xctx);
        }
        new_count = count * 2;
        if (new_count > self->maximum_count) {
            new_count = self->maximum_count;
        }
    }

    new_first = afw_pool_calloc(self->pub.p, new_count * self->entry_size,
        xctx);
    memcpy(new_first, self->pub.first, self->entry_size * count);
    self->pub.first = new_first;
    self->pub.top = (afw_octet_t *)self->pub.first +
        (self->entry_size * count);
    self->pub.end = (afw_octet_t *)self->pub.first +
        (self->entry_size * new_count);
}
