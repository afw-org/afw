// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework LMDB Adaptor Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_LMDB_H__
#define __AFW_LMDB_H__

#include "afw_minimal.h"

/**
 * @defgroup afw_lmdb Lightning Memory-mapped Database Adaptor
 * @ingroup afw_included_extensions
 * 
 * LMDB adaptor.
 * 
 * @{
 */

/**
 * @file afw_lmdb.h
 * @brief Adaptive Framework LMDB Adaptor
 */

AFW_BEGIN_DECLARES

/** lmdb rv source id */
#define AFW_ERROR_RV_SOURCE_ID_Z_lmdb "lmdb"

/** lmdb_internal rv source id */
#define AFW_ERROR_RV_SOURCE_ID_Z_lmdb_internal "lmdb_internal"

/** lmdb_journal rv source id */
#define AFW_ERROR_RV_SOURCE_ID_Z_lmdb_journal "lmdb_journal"

/** lmdb_index rv source id */
#define AFW_ERROR_RV_SOURCE_ID_Z_lmdb_index "lmdb_index"

/** Prefix for lmdb adaptor synthetic names quoted string. */
#define AFW_LMDB_Q_OBJECT_TYPE_SYNTHETIC_PREFIX "_lmdb"

AFW_END_DECLARES

/** @} */

#endif /* __AFW_LMDB_H__ */
