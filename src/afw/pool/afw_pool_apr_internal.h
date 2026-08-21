// See the 'COPYING' file in the project root for licensing information.
/*
 * Parked wrap-APR pool internal header (not wired).
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_POOL_APR_INTERNAL_H__
#define __AFW_POOL_APR_INTERNAL_H__

#include "afw_interface.h"

/**
 * @file afw_pool_apr_internal.h
 * @brief Parked wrap-APR pool internals. Live code uses afw_pool_internal.h.
 */

AFW_BEGIN_DECLARES

typedef struct afw_pool_apr_self_s
afw_pool_apr_self_t;

struct afw_pool_apr_self_s {

    afw_pool_t pub;

    apr_pool_t *apr_p;

    const afw_utf8_t *name;

    afw_pool_apr_self_t *parent;
    afw_pool_apr_self_t *first_child;
    afw_pool_apr_self_t *prev_sibling;
    afw_pool_apr_self_t *next_sibling;

    const afw_thread_t *thread;

    afw_pool_cleanup_t *first_cleanup;

    afw_integer_t reference_count;
    afw_integer_t pool_number;
    afw_size_t bytes_allocated;

    afw_boolean_t destroying;
};


AFW_DECLARE(const afw_pool_t *)
afw_pool_apr_internal_create_base_pool();


extern void
afw_pool_apr_print_debug_info(
    int indent,
    const afw_pool_t *pool,
    afw_xctx_t *xctx);


AFW_END_DECLARES

#endif /* __AFW_POOL_APR_INTERNAL_H__ */
