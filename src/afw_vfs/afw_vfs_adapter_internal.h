#ifndef __AFW_VFS_ADAPTER_INTERNAL_H__
#define __AFW_VFS_ADAPTER_INTERNAL_H__

/* Internal implementation header for adapter type vfs */
// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_vfs_adapter_internal.h
 * @brief Internal header for adapter type vfs.
 */

#include "afw_interface.h"
#include "generated/afw_vfs_generated.h"

AFW_BEGIN_DECLARES

typedef struct afw_vfs_adapter_internal_s
afw_vfs_adapter_internal_t;

typedef struct afw_vfs_adapter_internal_session_s
afw_vfs_adapter_internal_session_t;

typedef struct afw_vfs_adapter_internal_session_context_s
afw_vfs_adapter_internal_session_context_t;



struct afw_vfs_adapter_internal_s {
    afw_adapter_t pub;

    /* Array of prefix/path pairs from conf.  Last key is NULL. */
    const afw_key_z_string_z_t *vfs_map;

    /*
     * List of vfsPath filename patterns to make executable. A NULL value for
     * s_z terminates the list.
     */
    const afw_utf8_utf8_z_t *mark_executable;
};



struct afw_vfs_adapter_internal_session_s {
    afw_adapter_session_t pub;
};



struct afw_vfs_adapter_internal_session_context_s {
    const afw_pool_t *p;
    const afw_adapter_impl_request_t *impl_request;
    const afw_query_criteria_t *criteria;
    void *original_context;
    afw_object_cb_t original_callback;
    const afw_utf8_t *suffix;
    afw_boolean_t includeHidden;
    afw_boolean_t recursive;
};


extern const afw_adapter_t *
afw_vfs_adapter_internal_create_cede_p(
    const afw_object_t *properties,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



extern const afw_adapter_session_t *
afw_vfs_adapter_internal_session_create(
    afw_vfs_adapter_internal_t *adapter,
    afw_xctx_t *xctx);

    
AFW_END_DECLARES

#endif /* AFW_VFS_ADAPTER_INTERNAL_H__ */
