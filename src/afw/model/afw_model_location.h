// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework model location
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_MODEL_LOCATION_H__
#define __AFW_MODEL_LOCATION_H__

#include "afw_interface.h"

/**
 * @addtogroup afw_model
 * @{
 */

/**
 * @file afw_model_location.h
 * @brief Header file for Adaptive Framework model location.
 */

AFW_BEGIN_DECLARES


/**
 * @brief Struct used by adapter referenced by modelLocationAdapterId.
 *
 *
 */
struct afw_model_location_s {

    /** @brief Model location adapter. */
    const afw_adapter_t *model_location_adapter;

    /** @brief Mutex for locking access to this struct. */
    afw_thread_mutex_t *mutex;

    /** @brief First loaded model. */
    const afw_model_t * AFW_ATOMIC first_model;

};


/* @brief Create model location. */
AFW_DECLARE(const afw_model_location_t *)
afw_model_location_create(
    const afw_adapter_t *adapter,
    const afw_pool_t *p, afw_xctx_t *xctx);

AFW_DECLARE(const afw_model_t *)
afw_model_location_get_model(
    const afw_adapter_t *model_location_adapter,
    const afw_utf8_t *model_id,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_MODEL_LOCATION_H__ */
