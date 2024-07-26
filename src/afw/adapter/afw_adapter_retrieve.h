// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Adapter Retrieve Objects
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_ADAPTER_RETRIEVE_H__
#define __AFW_ADAPTER_RETRIEVE_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_adapter
 * @{
 */

/**
 * @file afw_adapter_retrieve.h
 * @brief Adaptive Framework adapter retrieve objects
 */

AFW_BEGIN_DECLARES

/**
 * @brief Retrieve objects.
 * @param adapter_id Id of adapter used to retrieve objects.
 * @param object_type_id The object type of objects to be retrieved.
 * @param options for object processing or NULL.
 * @param criteria Query criteria.
 * @param journal_entry is a memory object where journal entry properties will
 *    be added.
 * @param context Pointer passed to callback routine.
 * @param callback Callback function will be called each time an object is
 *    retrieved and once with a NULL object pointer when finished.
 * @param adapter_type_specific or NULL.
 * @param p to use for retrieved objects.
 * @param xctx of caller.
 *
 * Retrieve and cache adaptive objects via this adapter that match criteria.
 *
 * If criteria->own is false, all of the ancestors of retrieved objects
 * will be retrieved and placed in cache.
 */
AFW_DECLARE(void)
afw_adapter_retrieve_objects(
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *object_type_id,
    const afw_object_options_t *options,
    const afw_query_criteria_t *criteria,
    const afw_object_t *journal_entry,
    void *context,
    afw_object_cb_t callback,
    const afw_object_t *adapter_type_specific,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



AFW_END_DECLARES

/** @} */

#endif /* __AFW_ADAPTER_RETRIEVE_H__ */
