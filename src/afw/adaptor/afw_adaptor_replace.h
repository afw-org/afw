// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Adaptor Replace Object
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_ADAPTOR_REPLACE_H__
#define __AFW_ADAPTOR_REPLACE_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_adaptor
* @{
 */

/**
 * @file afw_adaptor_replace.h
 * @brief Adaptive Framework adaptor replace object 
 */

AFW_BEGIN_DECLARES

/**
 * @brief Replace object and remove from cache.
 * @param adaptor_id Id of adaptor used to patch object.
 * @param object_type_id of object to update.
 * @param object_id of object to update.
 * @param replacement_object Replace an existing object. The values of properties
 * in this updated object will completely replace the values of properties in
 * the existing object. The afw_object_meta_get_object_id() and
 * afw_object_type_get_object_type() methods of the update object are used to
 * identify the existing object to update.
 * @param journal_entry is a memory object where journal entry properties will
 *    be added.
 * @param adaptor_type_specific parameter or NULL.
 * @param xctx of caller.
 * @return
 *
 * Completely replace an existing adaptive object via adaptor.
 */
AFW_DECLARE(void)
afw_adaptor_replace_object(
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_object_t *replacement_object,
    const afw_object_t *journal_entry,
    const afw_object_t *adaptor_type_specific,
    afw_xctx_t *xctx);



AFW_END_DECLARES

/** @} */

#endif /* __AFW_ADAPTOR_REPLACE_H__ */
