// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework LMDB Adaptor Metadata Header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_LMDB_METADATA_H__
#define __AFW_LMDB_METADATA_H__


/**
 * @file afw_lmdb_metadata.h
 * @brief Adaptive Framework LMDB Adaptor Metadata handler.
 */

#include "afw_minimal.h"
#include "afw_lmdb.h"
#include "afw_lmdb_internal.h"
#include "afw_utf8.h"

AFW_BEGIN_DECLARES


AFW_END_DECLARES

struct afw_lmdb_metadata_s {

    /* Use count. */
    unsigned int use_count;

    /* Schema */
    const afw_object_t *schema;

    /* Subpool for metadata. */
    const afw_pool_t *p;
};


/**
 * @brief Refreshes LMDB adaptors Metadata.
 * @param adaptor Internal LMDB Adaptor
 * @param xctx of caller
 *
 *
 *
 */
void afw_lmdb_metadata_refresh(
    afw_lmdb_adaptor_t *adaptor,
    afw_xctx_t *xctx);


/**
 * @brief Called by afw_lmdb_adaptor_session() to retrieve metadata objects.
 */
const afw_list_t * afw_lmdb_metadata_retrieve_objects(
    afw_lmdb_adaptor_session_t *self,
    const afw_utf8_t *object_type_id,
    const afw_query_criteria_t *criteria,
    afw_xctx_t *xctx);

/**
 * @brief Called by afw_lmdb_adaptor_session() to get a metadata object.
 */
const afw_object_t * afw_lmdb_metadata_get_object(
    afw_lmdb_adaptor_session_t * self,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * id,
    const afw_query_criteria_t * criteria,
    afw_xctx_t *xctx);

afw_boolean_t afw_lmdb_metadata_handles(
    const afw_utf8_t *object_type_id);

#endif /* __AFW_LMDB_METADATA_H__ */
