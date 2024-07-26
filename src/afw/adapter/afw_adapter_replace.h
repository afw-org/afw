// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Adapter Replace Object
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_ADAPTER_REPLACE_H__
#define __AFW_ADAPTER_REPLACE_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_adapter
* @{
 */

/**
 * @file afw_adapter_replace.h
 * @brief Adaptive Framework adapter replace object 
 */

AFW_BEGIN_DECLARES

/**
 * @brief Replace object and remove from cache.
 * @param adapter_id Id of adapter used to patch object.
 * @param object_type_id of object to update.
 * @param object_id of object to update.
 * @param replacement_object Replace an existing object. The values of properties
 * in this updated object will completely replace the values of properties in
 * the existing object. The afw_object_meta_get_object_id() and
 * afw_object_type_get_object_type() methods of the update object are used to
 * identify the existing object to update.
 * @param journal_entry is a memory object where journal entry properties will
 *    be added.
 * @param adapter_type_specific parameter or NULL.
 * @param xctx of caller.
 * @return
 *
 * Completely replace an existing adaptive object via adapter.
 */
AFW_DECLARE(void)
afw_adapter_replace_object(
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_object_t *replacement_object,
    const afw_object_t *journal_entry,
    const afw_object_t *adapter_type_specific,
    afw_xctx_t *xctx);



AFW_END_DECLARES

/** @} */

#endif /* __AFW_ADAPTER_REPLACE_H__ */
