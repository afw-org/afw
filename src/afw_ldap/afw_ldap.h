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

/* Include generated declare helpers. */
#include "generated/afw_ldap_declare_helpers.h"

/**
 * @defgroup afw_ldap LDAP Adapter
 * @ingroup afw_included_extensions
 *
 * LDAP adapter type (`src/afw_ldap/`): directory-backed adaptive objects.
 * This srcdir is self-contained relative to libafw core (public APIs only).
 * Factory/session pattern from afwdev `add-adapter-type`. Use adapter
 * session call macros for CRUD; this header maps extension layout and
 * error RV source ids.
 *
 * @{
 */

/**
 * @file afw_ldap.h
 * @brief Public header for the LDAP adapter extension.
 *
 * Start here for LDAP as an adapter-type extension example (with
 * `afw_vfs` / `afw_lmdb`). See @ref afw_included_extensions and
 * @ref afw_dev_extending.
 */

AFW_LDAP_BEGIN_DECLARES

/** ldap rv source id */
#define AFW_ERROR_RV_SOURCE_ID_Z_ldap "ldap"

AFW_LDAP_END_DECLARES

/** @} */

#endif /* __AFW_LDAP_H__ */
