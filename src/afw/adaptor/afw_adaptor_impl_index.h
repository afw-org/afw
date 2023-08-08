// See the 'COPYING' file in the project root for licensing information.
/*
 * Helpers for afw_adaptor implementation index development
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_ADAPTOR_IMPL_INDEX_H__
#define __AFW_ADAPTOR_IMPL_INDEX_H__

#include "afw_interface.h"

/**
 * @defgroup afw_adaptor_index_impl afw_adaptor_index
 * @ingroup afw_c_api_impl
 * @{
 */

/**
 * @file afw_adaptor_impl_index.h
 * @brief  Helpers for afw_adaptor implementation index development
 */

AFW_BEGIN_DECLARES

/* definition for an individual index */
typedef struct {
    const afw_utf8_t * object_type_id;
    const afw_utf8_t * property_name;
} afw_adaptor_impl_index_property_t;

/* context to hold data for our callback routine */
typedef struct {
    const afw_adaptor_impl_index_t * instance;
    const afw_utf8_t *object_type_id;
    const afw_utf8_t *property_name;
    const afw_utf8_t *index_type;
    int mode;
    int num_indexed;
} impl_retrieve_cb_context_old_t;

typedef enum afw_adaptor_impl_index_mode_e {
    afw_adaptor_impl_index_mode_add,
    afw_adaptor_impl_index_mode_delete,
    afw_adaptor_impl_index_mode_replace,
    afw_adaptor_impl_index_mode_repair
} afw_adaptor_impl_index_mode_t;

/* context to hold data for our callback routine */
typedef struct {
    const afw_adaptor_impl_index_t * instance;
    const afw_utf8_t * key;
    const afw_object_t * indexDefinition;
    afw_adaptor_impl_index_mode_t mode;
    size_t num_processed;
    size_t num_indexed;
    afw_boolean_t test;
} impl_retrieve_objects_cb_context_t;

/* common index routines, helpful for adaptors and utilities */
AFW_DECLARE(const afw_object_t *) afw_adaptor_impl_index_create(
    const afw_utf8_t  * adaptorId,
    const afw_utf8_t  * key,
    const afw_utf8_t  * value,
    const afw_array_t  * objectType,
    const afw_utf8_t  * filter,
    const afw_array_t  * options,
    afw_boolean_t       retroactive,
    afw_boolean_t       test,
    const afw_pool_t  * pool,
    afw_xctx_t       * xctx);

AFW_DECLARE(const afw_object_t *) afw_adaptor_impl_index_remove(
    const afw_utf8_t  * adaptorId,
    const afw_utf8_t  * key,
    const afw_pool_t  * pool,
    afw_xctx_t       * xctx);

AFW_DECLARE(const afw_object_t *) afw_adaptor_impl_index_list(
    const afw_utf8_t * adaptorId,
    const afw_utf8_t * object_type_id,
    const afw_pool_t  * pool,
    afw_xctx_t *xctx);

AFW_DECLARE(void) afw_adaptor_impl_index_reindex_object(
    const afw_adaptor_impl_index_t * instance,
    const afw_utf8_t * object_type_id,
    const afw_object_t * old_object,
    const afw_object_t * new_object,
    const afw_utf8_t * object_id,
    afw_xctx_t *xctx);

AFW_DECLARE(void) afw_adaptor_impl_index_unindex_object(
    const afw_adaptor_impl_index_t * instance,
    const afw_utf8_t * object_type_id,
    const afw_object_t * object,
    const afw_utf8_t * object_id,
    afw_xctx_t *xctx);

AFW_DECLARE(void) afw_adaptor_impl_index_object(
    const afw_adaptor_impl_index_t * instance,
    const afw_utf8_t * object_type_id,
    const afw_object_t * object,
    const afw_utf8_t * object_id,
    afw_xctx_t *xctx);

AFW_DECLARE(afw_boolean_t) afw_adaptor_impl_index_sargable(
    const afw_adaptor_impl_index_t * instance,
    const afw_utf8_t * object_type_id,
    const afw_query_criteria_t * criteria,
    afw_xctx_t *xctx);

AFW_DECLARE(void) afw_adaptor_impl_index_query(
    const afw_adaptor_impl_index_t * instance,
    const afw_utf8_t * object_type_id,
    const afw_query_criteria_t * criteria,
    afw_object_cb_t callback,
    void * context, 
    const afw_pool_t * pool,
    afw_xctx_t *xctx);

AFW_DECLARE(void) afw_adaptor_impl_index_open_definitions(
    const afw_adaptor_impl_index_t * indexer,
    const afw_object_t             * indexDefinitions,
    const afw_pool_t               * pool,
    afw_xctx_t                    * xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_ADAPTOR_IMPL_INDEX_H__ */
