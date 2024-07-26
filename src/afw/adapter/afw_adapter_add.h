// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Adapter Add Object
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_ADAPTER_ADD_H__
#define __AFW_ADAPTER_ADD_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_adapter
 * @{
 */

/**
 * @file afw_adapter_add.h
 * @brief Adaptive Framework adapter add object 
 */

AFW_BEGIN_DECLARES

/**
 * @brief Call adapter to add object and remove from cache.
 * @param adapter_id of adapter used to add object.
 * @param object_type_id of object to add.
 * @param suggested_object_id or NULL.  The adapter has the option to use this id
 *    or generate one.
 * @param object to add. The afw_object_meta_get_object_id() and
 * afw_object_type_get_object_type() methods of the object are used to
 * identify the existing object to add.
 * @param journal_entry is a memory object where journal entry properties will
 *    be added.
 * @param adapter_type_specific parameter or NULL.
 * @param xctx of caller.
 * @return Id of added object.
 *
 * Add an adaptive object via this adapter.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_adapter_add_object(
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *suggested_object_id,
    const afw_object_t *object,
    const afw_object_t *journal_entry,
    const afw_object_t *adapter_type_specific,
    afw_xctx_t *xctx);



AFW_END_DECLARES

/** @} */

#endif /* __AFW_ADAPTER_ADD_H__ */
