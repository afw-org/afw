// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Adapter Delete Object
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_ADAPTER_DELETE_H__
#define __AFW_ADAPTER_DELETE_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_adapter
 * @{
 */

/**
 * @file afw_adapter_delete.h
 * @brief Adaptive Framework adapter delete object 
 */

AFW_BEGIN_DECLARES

/**
 * @brief Delete object from cache and via adapter.
 * @param adapter_id of adapter used to delete object.
 * @param object_type_id of object to delete.
 * @param object_id of object to delete.
 * @param journal_entry is a memory object where journal entry properties will
 *    be added.
 * @param adapter_type_specific parameter or NULL.
 * @param xctx of caller.
 * @return
 *
 * Delete an existing adaptive object via adapter. The object will not
 * actually be deleted until method commit() is called.
 */
AFW_DECLARE(void)
afw_adapter_delete_object(
    const afw_utf8_t *adapter_id,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_object_t *journal_entry,
    const afw_object_t *adapter_type_specific,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_ADAPTER_DELETE_H__ */
