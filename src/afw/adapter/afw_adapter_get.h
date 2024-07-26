// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Adapter Get Object
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_ADAPTER_GET_H__
#define __AFW_ADAPTER_GET_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_adapter
 * @{
 */

/**
 * @file afw_adapter_get.h
 * @brief Adaptive Framework adapter get object 
 */

AFW_BEGIN_DECLARES

/**
 * @brief Get and cache object.
 * @param adapter_id Id of adapter used to get object.
 * @param object_type_id Object type of object to get.
 * @param object_id Object id of object to get.
 * @param options for object processing or NULL.
 * @param criteria Query criteria. Only select criteria is applicable. If
 * criteria is NULL, all properties are returned.
 * @param journal_entry is a memory object where journal entry properties will
 *    be added.
 * @param adapter_type_specific parameter or NULL.
 * @param p to use for returned object.
 * @param xctx of caller.
 * @return Pointer to the instance of the object in store session. NULL is
 * returned if object is not found.
 *
 * If object is already in cache, return it.  If not, get object via adapter
 * and add it to cache.
 *
 * If criteria->own is false, all of the uncached ancestors of retrieved objects
 * will be retrieved and placed in cache.
 */
AFW_DECLARE(const afw_object_t *)
afw_adapter_get_object(
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_object_options_t *options,
    const afw_query_criteria_t *criteria,
    const afw_object_t *journal_entry,
    const afw_object_t *adapter_type_specific,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Get and cache _AdaptiveObjectType_ object.
 * @param adapter_id Id of adapter used to get object.
 * @param object_type_id Object type of object to get.
 * @param journal_entry is a memory object where journal entry properties will
 *    be added.
 * @param xctx of caller.
 * @return Pointer to object type instance.
 *
 * If object is already in cache, return it.  If not, get object via adapter
 * and add it to cache.
 */
AFW_DECLARE(const afw_object_type_t *)
afw_adapter_get_object_type(
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *object_type_id,
    const afw_object_t *journal_entry,
    afw_xctx_t *xctx);



/**
 * @brief Get and cache object by path.
 * @param path Path of object to get.
 * @param journal_entry is a memory object where journal entry properties will
 *    be added.
 * @param adapter_type_specific parameter or NULL.
 * @param xctx of caller.
 * @return Pointer to the instance of the object in store session. NULL is
 * returned if object is not found.
 *
 * Retrieve a read-only copy of an adaptive object via adapter.
 */
AFW_DECLARE(const afw_object_t *)
afw_adapter_get_object_by_path(
    const afw_utf8_t *path,
    const afw_object_t *journal_entry,
    const afw_object_t *adapter_type_specific,
    afw_xctx_t *xctx);


/**
 * @brief Get and cache objects by paths.
 * @param paths NULL terminated list of path pointers of objects to get.
 * @param journal_entry is a memory object where journal entry properties will
 *    be added.
 * @param xctx of caller.
 * @return NULL terminated list of pointers to the instances of the object in
 *    store session.
 *
 * Objects are read only.  If a path are not found, an error is thrown and no
 * following object path is processed.
 */
AFW_DECLARE(const afw_object_t * const *)
afw_adapter_get_objects_by_paths(
    const afw_utf8_t * const * paths,
    const afw_object_t *journal_entry,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_ADAPTER_GET_H__ */
