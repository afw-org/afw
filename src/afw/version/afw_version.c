// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_version.c
 * @brief Runtime version string and git-info helpers.
 */

#include "afw_internal.h"
#include "afw_version_info.h"

static const afw_utf8_t impl_version_string = 
AFW_UTF8_LITERAL(AFW_VERSION_STRING);

/* Return AFW_VERSION_HEX. */
AFW_DEFINE(unsigned int) afw_version()
{
    return AFW_VERSION_HEX;
}

/* Return Adaptive Framework version string. */
AFW_DEFINE(const afw_utf8_t *) afw_version_string()
{
    return &impl_version_string;
}

/* Return Adaptive Framework version string as utf8_z. */
AFW_DEFINE(const afw_utf8_z_t *) afw_version_string_z()
{
    return impl_version_string.s;
}

/* Version check */
AFW_DEFINE(const afw_utf8_z_t *) afw_version_check(unsigned int hex)
{
    static const afw_utf8_z_t *impl_msg_pre_1_0_0 =
        "built with a version of Adaptive Framework that is incompatible with "
        AFW_VERSION_STRING ". "
        "MAJOR and MINOR version must be the same.";
    static const afw_utf8_z_t *impl_msg_incompatible =
        "built with a version of Adaptive Framework that is incompatible with "
        AFW_VERSION_STRING ". "
        "MAJOR version must be the same and "
        "MINOR version must be the same or larger.";

    unsigned int curr_major = (AFW_VERSION_HEX >> 16) & 0xFF;
    unsigned int curr_minor = (AFW_VERSION_HEX >> 8) & 0xFF;
    //unsigned int curr_patch = (AFW_VERSION_HEX) & 0xFF;

    unsigned int comp_major = (hex >> 16) & 0xFF;
    unsigned int comp_minor = (hex >> 8) & 0xFF;
    //unsigned int comp_patch = hex & 0xFF;

    /* Case 1: Compiled version has MAJOR = 0 (pre-1.0.0) */
    if (comp_major == 0) {
        if (curr_major != 0 || curr_minor != comp_minor) {
            return impl_msg_pre_1_0_0;
        }
        /* PATCH can differ, no further check needed */
        return NULL;
    }

    /* Case 2: Compiled version has MAJOR > 0 (stable API) */
    if (curr_major != comp_major) {
        return impl_msg_incompatible;
    }
    if (curr_minor < comp_minor) {
        return impl_msg_incompatible;
    }
    /* PATCH can differ, no further check needed */

    return NULL;
}
