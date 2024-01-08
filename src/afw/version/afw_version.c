// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_version.c
 * @brief Adaptive Framework version information
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
    /* Add checks and throw errors as needed. */
    return NULL;
}
