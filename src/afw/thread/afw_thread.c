// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework thread support.
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_thread.c
 * @brief Adaptive Framework thread support.
 */

#include "afw_internal.h"



AFW_DEFINE(afw_thread_attr_t *)
afw_thread_attr_create(
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    apr_threadattr_t *attr;

    apr_threadattr_create(&attr, afw_pool_get_apr_pool(p));

    return (afw_thread_attr_t *)attr;
}


static void* APR_THREAD_FUNC
impl_thread_start(apr_thread_t *thd, void *data)
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
    apr_status_t rv;

    self = afw_pool_create_thread(-1, xctx);
    self->thread_attr = thread_attr;
    self->start_function = start_function;
    self->start_function_arg = start_function_arg;
    self->name = name;
    self->thread_number = thread_number;
    self->xctx = afw_xctx_internal_create_thread_xctx(self, xctx);

    rv = apr_thread_create(&self->apr_thread, (apr_threadattr_t *)thread_attr,
        impl_thread_start, self, afw_pool_get_apr_pool(self->p));
    if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_Z(general, apr, rv,
            "apr_thread_create() failed", xctx);
    }

    return self;
}

AFW_DEFINE(void)
afw_thread_join(
    const afw_thread_t *thread,
    afw_xctx_t *xctx)
{
    apr_status_t rv, rv2;

    rv = apr_thread_join(&rv2, thread->apr_thread);
    if (rv != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_Z(general, apr, rv,
            "apr_thread_join() failed", xctx);
    }
    if (rv2 != APR_SUCCESS) {
        AFW_THROW_ERROR_RV_Z(general, apr, rv2,
            "apr_thread_join() dead thread error", xctx);
    }
}
