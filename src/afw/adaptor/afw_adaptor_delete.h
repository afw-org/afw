// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Adaptor Delete Object
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_ADAPTOR_DELETE_H__
#define __AFW_ADAPTOR_DELETE_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_adaptor
 * @{
 */

/**
 * @file afw_adaptor_delete.h
 * @brief Adaptive Framework adaptor delete object 
 */

AFW_BEGIN_DECLARES

/**
 * @brief Delete object from cache and via adaptor.
 * @param adaptor_id of adaptor used to delete object.
 * @param object_type_id of object to delete.
 * @param object_id of object to delete.
 * @param journal_entry is a memory object where journal entry properties will
 *    be added.
 * @param adaptor_type_specific parameter or NULL.
 * @param xctx of caller.
 * @return
 *
 * Delete an existing adaptive object via adaptor. The object will not
 * actually be deleted until method commit() is called.
 */
AFW_DECLARE(void)
afw_adaptor_delete_object(
    const afw_utf8_t *adaptor_id,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *object_id,
    const afw_object_t *journal_entry,
    const afw_object_t *adaptor_type_specific,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_ADAPTOR_DELETE_H__ */
