// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Register Generated (afw_vfs_)
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/*  ----------------------------- N O T E -------------------------------------
 *
 * This file is generated by "afwdev generate afw_vfs".
 *
 * Do not change this file directly or the changes will be lost the next time
 * this file is regenerated.
 *
 * -------------------------------------------------------------------------*/



/**
 * @file afw_vfs_generated.c
 * @brief Adaptive Framework register generated (afw_vfs_).
 */

#include "afw.h"
#include "afw_vfs_generated.h"
#include "afw_vfs_version_info.h"

static const afw_utf8_z_t * impl_version_info_z[] = {
    "compileDate", __DATE__,
    "compileTime", __TIME__,
    "id", "afw_vfs",
    "gitBranch", AFW_GIT_INFO_BRANCH,
    "gitDescribe", AFW_GIT_INFO_DESCRIBE,
    "gitSha", AFW_GIT_INFO_SHA,
    "libafwVersion", AFW_VERSION_STRING,
    "libafwVersionWithGitInfo", AFW_VERSION_WITH_GIT_INFO,
    "version", AFW_VFS_VERSION_STRING,
    "versionWithGitInfo", AFW_VFS_VERSION_WITH_GIT_INFO,
    NULL
};

static const afw_utf8_t impl_version_info_path =
    AFW_UTF8_LITERAL("/afw/_AdaptiveVersionInfo_/afw_vfs");

void afw_vfs_generated_register(afw_xctx_t *xctx)
{

    /* Register object maps. */
    afw_vfs_register_runtime_object_maps(xctx);

    /* Register const runtime objects. */
    afw_vfs_const_objects_register(xctx);

    /* Register adaptor_type vfs */
    afw_vfs_adaptor_type_vfs_register(xctx);

    /* Register version info object. */
    afw_runtime_env_set_object(
        afw_object_create_const_from_key_value_strings_z(
            &impl_version_info_path,
            impl_version_info_z,
            xctx->env->p, xctx),
        false, xctx);

}
