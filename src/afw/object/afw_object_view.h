// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Object View
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_OBJECT_VIEW_H__
#define __AFW_OBJECT_VIEW_H__


/** @addtogroup afw_c_api
 * @{
 */

/**
 * @addtogroup afw_object_view Object view
 *
 * Adaptive Framework object view.
 *
 * @{
 */

/**
 * @file afw_object_view.h
 * @brief Header file for Adaptive Framework object view
 */

#include "afw_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create an object view of an object in specified pool.
 * @param instance of object to make a view of.
 * @param entity_path to use or NULL if the path of instance should be used.
 * @param options for the view.
 * @param p to use.
 * @param xctx of caller.
 *
 * Object should already be valid and normalized.
 */
AFW_DECLARE(const afw_object_t *)
afw_object_view_create(
    const afw_object_t *instance,
    const afw_utf8_t *entity_path,
    const afw_object_options_t *options,
    const afw_pool_t *p, afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */
/** @} */

#endif /* __AFW_OBJECT_VIEW_H__ */
