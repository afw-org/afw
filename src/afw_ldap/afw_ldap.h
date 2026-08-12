// See the 'COPYING' file in the project root for licensing information.
/*
 * AFW LDAP Adapter Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_LDAP_H__
#define __AFW_LDAP_H__

#include "afw_minimal.h"

/**
 * @defgroup afw_ldap LDAP Adapter
 * @ingroup afw_included_extensions
 *
 * LDAP adapter type (`src/afw_ldap/`): directory-backed adaptive objects.
 * Package-private C surface over libafw public APIs (self-contained srcdir).
 * Factory/session pattern from afwdev `add-adapter-type`. Use adapter
 * session call macros for CRUD; this header maps extension layout and
 * error RV source ids.
 *
 * @{
 */

/**
 * @file afw_ldap.h
 * @brief Package header for the LDAP adapter extension.
 *
 * Start here for LDAP as an adapter-type extension example (with
 * `afw_vfs` / `afw_lmdb`). See @ref afw_included_extensions and
 * @ref afw_dev_extending. Not installed as a second public C API.
 */

AFW_BEGIN_DECLARES

/** ldap rv source id */
#define AFW_ERROR_RV_SOURCE_ID_Z_ldap "ldap"

AFW_END_DECLARES

/** @} */

#endif /* __AFW_LDAP_H__ */
