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
#include <stdlib.h>



struct afw_thread_attr_s {
    int unused;
};


AFW_DEFINE(afw_thread_attr_t *)
afw_thread_attr_create(
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    return afw_pool_calloc_type(p, afw_thread_attr_t, xctx);
}


afw_thread_t *
afw_thread_internal_create_base_thread(void)
{
    afw_thread_t *self;
    const afw_memory_region_t *region;

    self = (afw_thread_t *)calloc(1, sizeof(afw_thread_t));
    if (!self) {
        return NULL;
    }
    self->type = afw_thread_type_base;
    self->os_thread = NULL;
    self->pool_number = 1;
    region = afw_memory_region_create(
        AFW_MEMORY_REGION_FREE_LIST_MAX_BYTES, NULL);
    if (!region) {
        free(self);
        return NULL;
    }
    self->memory_region = region;
    return self;
}


void
afw_thread_internal_release_base_thread(
    afw_thread_t *thread,
    afw_xctx_t *xctx)
{
    if (!thread) {
        return;
    }
    if (thread->memory_region) {
        afw_memory_region_release(thread->memory_region, xctx);
        thread->memory_region = NULL;
    }
    free(thread);
}


static void *
impl_thread_start(void *data)
{
    afw_thread_t *self = data;

    afw_os_c_stack_bounds(&self->c_stack_base, &self->c_stack_size);
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
    self->type = afw_thread_type_request;
    self->xctx = afw_xctx_internal_create_thread_xctx(self, xctx);

    /* Joinable POSIX default; thread_attr is stored but unused. */
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
