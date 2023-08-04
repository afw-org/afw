// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Register Generated (afw_yaml_)
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/*  ----------------------------- N O T E -------------------------------------
 *
 * This file is generated by "afwdev generate afw_yaml".
 *
 * Do not change this file directly or the changes will be lost the next time
 * this file is regenerated.
 *
 * -------------------------------------------------------------------------*/



/**
 * @file afw_yaml_generated.c
 * @brief Adaptive Framework register generated (afw_yaml_).
 */

#include "afw.h"
#include "afw_yaml_generated.h"
#include "afw_yaml_version_info.h"

static const afw_utf8_z_t * impl_version_info_z[] = {
    "compileDate", __DATE__,
    "compileTime", __TIME__,
    "id", "afw_yaml",
    "gitBranch", AFW_GIT_INFO_BRANCH,
    "gitDescribe", AFW_GIT_INFO_DESCRIBE,
    "gitSha", AFW_GIT_INFO_SHA,
    "libafwVersion", AFW_VERSION_STRING,
    "libafwVersionWithGitInfo", AFW_VERSION_WITH_GIT_INFO,
    "version", AFW_YAML_VERSION_STRING,
    "versionWithGitInfo", AFW_YAML_VERSION_WITH_GIT_INFO,
    NULL
};

static const afw_utf8_t impl_version_info_path =
    AFW_UTF8_LITERAL("/afw/_AdaptiveVersionInfo_/afw_yaml");

void afw_yaml_generated_register(afw_xctx_t *xctx)
{

    /* Register version info object. */
    afw_runtime_env_set_object(
        afw_object_create_const_from_key_value_strings_z(
            &impl_version_info_path,
            impl_version_info_z,
            xctx->env->p, xctx),
        false, xctx);

}
