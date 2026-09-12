// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework thread support.
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_thread.c
 * @brief Thread create/join and thread-attribute helpers.
 */

#include "afw_internal.h"



struct afw_thread_attr_s {
    int unused;
};


AFW_DEFINE(afw_thread_attr_t *)
afw_thread_attr_create(
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    return afw_pool_calloc_type(p, afw_thread_attr_t, xctx);
}


static void *
impl_thread_start(void *data)
{
    afw_thread_t *self = data;

    return self->start_function(self, self->start_function_arg);
}



AFW_DEFINE(const afw_thread_t *)
afw_thread_create(
    afw_thread_attr_t *thread_attr,
    afw_thread_function_t start_function,
    void *start_function_arg, 
    const afw_utf8_t *name,
    afw_integer_t thread_number,
    afw_xctx_t *xctx)
{
    afw_thread_t *self;

    self = afw_pool_thread_create(-1, xctx);
    self->thread_attr = thread_attr;
    self->start_function = start_function;
    self->start_function_arg = start_function_arg;
    self->name = name;
    self->thread_number = thread_number;
    self->xctx = afw_xctx_internal_create_thread_xctx(self, xctx);

    /* Joinable; thread_attr is currently unused (POSIX default). */
    self->os_thread = afw_os_thread_create(
        impl_thread_start, self, self->p, xctx);

    return self;
}

AFW_DEFINE(void)
afw_thread_join(
    const afw_thread_t *thread,
    afw_xctx_t *xctx)
{
    if (!thread) {
        return;
    }
    afw_os_thread_join(thread->os_thread, xctx);
}
