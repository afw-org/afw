// See the 'COPYING' file in the project root for licensing information.
/*
 * Helpers for afw_adapter implementation index development
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_ADAPTER_IMPL_INDEX_H__
#define __AFW_ADAPTER_IMPL_INDEX_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_adapter_index_impl
 * @{
 */

/**
 * @file afw_adapter_impl_index.h
 * @brief  Helpers for afw_adapter implementation index development
 */

AFW_BEGIN_DECLARES

/* definition for an individual index */
typedef struct {
    const afw_utf8_t * object_type_id;
    const afw_value_t * property_name;
} afw_adapter_impl_index_property_t;

/* context to hold data for our callback routine */
typedef struct {
    const afw_adapter_impl_index_t * instance;
    const afw_utf8_t *object_type_id;
    const afw_value_t *property_name;
    const afw_utf8_t *index_type;
    int mode;
    int num_indexed;
} impl_retrieve_cb_context_old_t;

typedef enum afw_adapter_impl_index_mode_e {
    afw_adapter_impl_index_mode_add,
    afw_adapter_impl_index_mode_delete,
    afw_adapter_impl_index_mode_replace,
    afw_adapter_impl_index_mode_repair
} afw_adapter_impl_index_mode_t;

/*
 * Value passed as the "operator" parameter of
 * afw_adapter_impl_index_open_cursor() for a match filter entry that
 * afw_query_criteria_match_literal_prefix() reduced to a literal "starts
 * with" prefix. Deliberately outside the range of
 * afw_query_criteria_filter_op_id_t so an implementation's switch on
 * query criteria op ids can add a case for this one without colliding.
 *
 * The cursor should seek exactly like a >= scan on the prefix value, but
 * iteration must stop as soon as the key no longer starts with the
 * prefix - unlike >=, "starts with" is not monotonic to the end of the
 * index, so it cannot rely on end-of-database as its only stop condition.
 */
#define AFW_ADAPTER_IMPL_INDEX_OPERATOR_STARTS_WITH ((int)-1)

/* context to hold data for our callback routine */
typedef struct {
    const afw_adapter_impl_index_t * instance;
    const afw_utf8_t * key;
    const afw_object_t * indexDefinition;
    afw_adapter_impl_index_mode_t mode;
    size_t num_processed;
    size_t num_indexed;
    afw_boolean_t test;
} impl_retrieve_objects_cb_context_t;

/* common index routines, helpful for adapters and utilities */
AFW_DECLARE(const afw_object_t *) afw_adapter_impl_index_create(
    const afw_utf8_t  * adapterId,
    const afw_utf8_t  * key,
    const afw_utf8_t  * value,
    const afw_array_t  * objectType,
    const afw_utf8_t  * filter,
    const afw_array_t  * options,
    afw_boolean_t       retroactive,
    afw_boolean_t       test,
    const afw_pool_t  * pool,
    afw_xctx_t       * xctx);

AFW_DECLARE(const afw_object_t *) afw_adapter_impl_index_remove(
    const afw_utf8_t  * adapterId,
    const afw_utf8_t  * key,
    const afw_pool_t  * pool,
    afw_xctx_t       * xctx);

AFW_DECLARE(const afw_object_t *) afw_adapter_impl_index_list(
    const afw_utf8_t * adapterId,
    const afw_utf8_t * object_type_id,
    const afw_pool_t  * pool,
    afw_xctx_t *xctx);

AFW_DECLARE(void) afw_adapter_impl_index_reindex_object(
    const afw_adapter_impl_index_t * instance,
    const afw_utf8_t * object_type_id,
    const afw_object_t * old_object,
    const afw_object_t * new_object,
    const afw_utf8_t * object_id,
    afw_xctx_t *xctx);

AFW_DECLARE(void) afw_adapter_impl_index_unindex_object(
    const afw_adapter_impl_index_t * instance,
    const afw_utf8_t * object_type_id,
    const afw_object_t * object,
    const afw_utf8_t * object_id,
    afw_xctx_t *xctx);

AFW_DECLARE(void) afw_adapter_impl_index_object(
    const afw_adapter_impl_index_t * instance,
    const afw_utf8_t * object_type_id,
    const afw_object_t * object,
    const afw_utf8_t * object_id,
    afw_xctx_t *xctx);

/**
 * @brief Is property_name indexed for object_type_id?
 *
 * The same test afw_adapter_impl_index_sargable() makes for each filter
 * entry, so an implementation can check what a query will open.
 */
AFW_DECLARE(afw_boolean_t) afw_adapter_impl_index_is_property_indexed(
    const afw_adapter_impl_index_t * instance,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * property_name,
    afw_xctx_t *xctx);

AFW_DECLARE(afw_boolean_t) afw_adapter_impl_index_sargable(
    const afw_adapter_impl_index_t * instance,
    const afw_utf8_t * object_type_id,
    const afw_query_criteria_t * criteria,
    afw_xctx_t *xctx);

AFW_DECLARE(void) afw_adapter_impl_index_query(
    const afw_adapter_impl_index_t * instance,
    const afw_utf8_t * object_type_id,
    const afw_query_criteria_t * criteria,
    afw_object_cb_t callback,
    void * context, 
    const afw_pool_t * pool,
    afw_xctx_t *xctx);

/**
 * @brief A query filter tested through an adapter's index definitions.
 *
 * An index definition with a value script (a computed name, maybe with a
 * filter script) or the case-insensitive-string option gives its name
 * another meaning than the object's property. Index cursors find what
 * the index holds; the re-test of an index query and an adapter's scan
 * test those names the same way, so every plan gives the same answer
 * (issue #516).
 */
typedef struct afw_adapter_impl_index_query_test_s
    afw_adapter_impl_index_query_test_t;

/**
 * @brief Create a query test for criteria on object_type_id.
 * @param instance Index instance (NULL returns NULL).
 * @param object_type_id Object type of the objects tested.
 * @param criteria Parsed query criteria.
 * @param p Pool for the test; scripts are compiled into it once.
 * @param xctx of caller.
 * @return The test, or NULL when no relation names a computed or
 *    case-insensitive index: use afw_query_criteria_test_object(), at no
 *    extra cost per object.
 *
 * Throws when an object type now declares a computed index's name.
 */
AFW_DECLARE(const afw_adapter_impl_index_query_test_t *)
afw_adapter_impl_index_query_test_create(
    const afw_adapter_impl_index_t * instance,
    const afw_utf8_t * object_type_id,
    const afw_query_criteria_t * criteria,
    const afw_pool_t * p,
    afw_xctx_t *xctx);

/**
 * @brief Test object with a query test.
 * @param test From afw_adapter_impl_index_query_test_create().
 * @param object Object to test.
 * @param p to use.
 * @param xctx of caller.
 * @return True if object passes.
 */
AFW_DECLARE(afw_boolean_t)
afw_adapter_impl_index_query_test_object(
    const afw_adapter_impl_index_query_test_t * test,
    const afw_object_t * object,
    const afw_pool_t * p,
    afw_xctx_t *xctx);

AFW_DECLARE(void) afw_adapter_impl_index_open_definitions(
    const afw_adapter_impl_index_t * indexer,
    const afw_object_t             * indexDefinitions,
    const afw_pool_t               * pool,
    afw_xctx_t                    * xctx);


AFW_END_DECLARES

/** @} */  // end of @addtogroup @addtogroup

#endif /* __AFW_ADAPTER_IMPL_INDEX_H__ */
