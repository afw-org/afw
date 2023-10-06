// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework LMDB Adaptor Metadata
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#include "afw.h"
#include "afw_lmdb_internal.h"
#include "afw_lmdb_metadata.h"

const afw_array_t * impl_get_schema_columns(
    afw_lmdb_adaptor_session_t *session,
    const afw_pool_t *p, afw_xctx_t *xctx);

const afw_array_t * impl_get_schema_tables(
    afw_lmdb_adaptor_session_t *session,
    const afw_pool_t *p, afw_xctx_t *xctx);


/* Called by afw_lmdb_adaptor_session() to get a metadata object. */
const afw_object_t * afw_lmdb_metadata_get_object(
    afw_lmdb_adaptor_session_t * self,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * id,
    const afw_query_criteria_t * criteria,
    afw_xctx_t *xctx)
{
    /* ObjectType. */
    if (afw_utf8_equal(
        object_type_id, AFW_OBJECT_S_OBJECT_TYPE_ID_OBJECT_TYPE))
    {
    }

    AFW_THROW_ERROR_Z(not_found, "Not found.", xctx);
}

void afw_lmdb_metadata_refresh(
    afw_lmdb_adaptor_t *adaptor,
    afw_xctx_t *xctx)
{
    afw_lmdb_metadata_t *new_metadata;
    afw_lmdb_metadata_t *old_metadata;
    const afw_pool_t *p;

    /* Create a subpool for metadata in adaptor's pool. */
    p = afw_pool_create(adaptor->pub.p, xctx);

    /* Allocate new afw_lmdb_metadata_t in subpool. */
    new_metadata = afw_pool_calloc_type(p, afw_lmdb_metadata_t, xctx);
    new_metadata->p = p;

    /* Start using new metadata. */
    old_metadata = adaptor->metadata;
    adaptor->metadata = new_metadata;
    if (old_metadata) {
        afw_pool_release(old_metadata->p, xctx);
    }

    return;
}

/* Used by afw_lmdb_adaptor_session() to retrieve metadata objects. */
const afw_array_t * afw_lmdb_metadata_retrieve_objects(
    afw_lmdb_adaptor_session_t * self,
    const afw_utf8_t * object_type_id,
    const afw_query_criteria_t * criteria,
    afw_xctx_t *xctx)
{
    /* ObjectType. */
    if (afw_utf8_equal(object_type_id, AFW_OBJECT_S_OBJECT_TYPE_ID_OBJECT_TYPE)) {
    }

    /* PropertyType. */
    if (afw_utf8_equal(object_type_id, AFW_OBJECT_S_OBJECT_TYPE_ID_VALUE_META)) {
    }

    /* If none of above, return empty array. */
    return afw_array_create_generic(xctx->p, xctx);
}

/* Determine if afw_lmdb_metadata.c handles an ObjectType. */
afw_boolean_t afw_lmdb_metadata_handles(const afw_utf8_t *object_type_id)
{
    if (object_type_id  && (
        afw_utf8_starts_with_z(object_type_id,
            AFW_LMDB_Q_OBJECT_TYPE_SYNTHETIC_PREFIX)
        ))
    {
        return AFW_TRUE;
    }

    return AFW_FALSE;
}
