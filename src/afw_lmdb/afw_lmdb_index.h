// See the 'COPYING' file in the project root for licensing information.
/*
 * Index routines for LMDB Adapter
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_LMDB_INDEX_H_
#define __AFW_LMDB_INDEX_H_


/**
 * @file afw_lmdb_index.h
 * @brief  Helpers for afw_adapter implementation index development
 */

#include "afw_interface.h"
#include "afw_lmdb_internal.h"

AFW_BEGIN_DECLARES


afw_adapter_impl_index_t * afw_lmdb_adapter_impl_index_create(
    const afw_lmdb_adapter_session_t * session,
    const afw_lmdb_adapter_t         * adapter,
    MDB_txn                          * txn,
    afw_xctx_t                      * xctx);

const afw_utf8_t * afw_lmdb_index_database(
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *property_name,
    const afw_pool_t *pool,
    afw_xctx_t *xctx);


#endif
