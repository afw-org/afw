// See the 'COPYING' file in the project root for licensing information.
/*
 * Helpers for afw_content_type* implementation development
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_CONTENT_TYPE_IMPL_H__
#define __AFW_CONTENT_TYPE_IMPL_H__


/** @addtogroup afw_c_api_impl Interface
* @{
 */

/** @addtogroup afw_content_type_impl afw_content_type interfaces
* @{
 */

/**
 * @file afw_content_type_impl.h
 * @brief Helpers for afw_content_type* implementation development
 */

#include "afw_interface.h"

AFW_BEGIN_DECLARES

AFW_DECLARE(const afw_content_type_object_list_writer_t *)
afw_content_type_impl_create_object_list_writer(
    const afw_content_type_t * instance,
    const afw_object_options_t *options,
    void * context,
    afw_write_cb_t callback,
    const afw_memory_t * begin_object_list,
    const afw_memory_t * object_separator,
    const afw_memory_t * last_object_separator,
    const afw_memory_t * end_object_list,
    const afw_pool_t * p, afw_xctx_t *xctx);


/** @} */
/** @} */

AFW_END_DECLARES


#endif /* __AFW_CONTENT_TYPE_IMPL_H__ */
