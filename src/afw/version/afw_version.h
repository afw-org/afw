// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Version Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_VERSION_H__
#define __AFW_VERSION_H__

#include "afw_common.h" 

/**
 * @defgroup afw_version Version information
 * @ingroup afw_c_api_public
 *
 * Adaptive Framework uses the semantic versioning scheme.  See
 * http://semver.org/.
 *
 * AFW_VERSION_THROW_ERROR_IF_NOT_COMPATIBLE() or
 * AFW_VERSION_ABORT_IF_NOT_COMPATIBLE should be executed early in the life of
 * Adaptive Framework extensions and applications to ensure compatibility
 * with Adaptive Framework Core (libafw).
 *
 * @{
 */

/**
 * @file afw_version.h
 * @brief Adaptive Framework Version Information
 */

AFW_BEGIN_DECLARES

/** Return AFW_VERSION_HEX. */
AFW_DECLARE(unsigned int)
afw_version();

/** Return Adaptive Framework version string. */
AFW_DECLARE(const afw_utf8_t *)
afw_version_string();

/** Return Adaptive Framework version string as utf8_z. */
AFW_DECLARE(const afw_utf8_z_t *)
afw_version_string_z();

/**
 * @brief Version check
 * @param version (see AFW_VERSION_HEX in afw_version.h)
 * @return NULL if version is ok or pointer to error string if not.
 * 
 * Returns an error message if this version of Adaptive Framework is not
 * compatible with the supplied version, as well as if any dependency
 * is not the appropriate level.
 */
AFW_DECLARE(const afw_utf8_z_t *)
afw_version_check(unsigned int version);

/**
 * @brief Version check against the version compiled against.
 * @return NULL if version is ok or pointer to error string if not.
 */
#define AFW_VERSION_CHECK afw_version_check(AFW_VERSION_HEX)

/**
 * @brief Throw error if AFW core is not compatible with the supplied version.
 * @param info will be included in error message.
 * @param compiled_version is AFW_VERSION_HEX to check against.
 * @param xctx of caller.
 *
 * Throw an error if this version of Adaptive Framework is not compatible with
 * the compiled_version, as well as if any dependency are not the appropriate
 * level.
 */
#define AFW_VERSION_THROW_ERROR_SUPPLIED_HEX_VERSION_IF_NOT_COMPATIBLE( \
    info, compiled_version, xctx) \
do { \
    const afw_utf8_z_t *msg_z; \
    msg_z = afw_version_check(compiled_version); \
    if (msg_z) { \
        AFW_THROW_ERROR_FZ(general, xctx, \
            AFW_UTF8_FMT " %s", \
            AFW_UTF8_FMT_ARG(info), msg_z); \
    } \
} while (0)

/**
 * @brief Throw error if AFW core is not compatible with the version compiled
 *    against.
 * @param id_z is a afw_utf8_z_t * that will be included in error message.
 * @param xctx of caller.
 *
 * Throw an error if this version of Adaptive Framework is not compatible with
 * the version compiled against, as well as if any dependency are not the
 * appropriate level.
 */
#define AFW_VERSION_THROW_ERROR_IF_NOT_COMPATIBLE(id_z, xctx) \
do { \
    const afw_utf8_z_t *msg_z; \
    msg_z = AFW_VERSION_CHECK; \
    if (msg_z) { \
        AFW_THROW_ERROR_FZ(general, xctx, "%s %s", id_z, msg_z); \
    } \
} while (0)

/**
 * @brief Abort if AFW core is not compatible with the version compiled
 *    against.
 * @param id_z is a afw_utf8_z_t * that will be included in error message.
 *
 * Use AFW_VERSION_THROW_ERROR_IF_NOT_COMPATIBLE(xctx) instead if xctx
 * is available.
 *
 * Print error and abort() if this version of Adaptive Framework is not
 * compatible with the version compiled against, as well as if any dependency
 * are not the appropriate level.
 */
#define AFW_VERSION_ABORT_IF_NOT_COMPATIBLE(id_z) \
do { \
    const afw_utf8_z_t *msg_z; \
    msg_z = AFW_VERSION_CHECK; \
    if (msg_z) { \
        fprintf(stderr, "%s %s", id_z, msg_z); \
        abort(); \
    } \
} while (0)

AFW_END_DECLARES

/** @} */

#endif /* __AFW_VERSION_H__ */
