// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Director Internal Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_LOG_INTERNAL_H__
#define __AFW_LOG_INTERNAL_H__

#include "afw_interface.h"

/**
 * @defgroup afw_log_internal Log
 * @ingroup afw_c_api_internal
 *
 * Internal log API.
 * @{
 */

/**
 * @file afw_log_internal.h
 * @brief Header file for Adaptive Framework Request Handler Internals
 */

AFW_BEGIN_DECLARES

/** @brief Internal create environment log. */
extern const afw_log_t * afw_log_internal_create_environment_log(
    afw_xctx_t *xctx);

/**
 * @internal
 * @brief Register log service type.
 *
 * Definition in afw_log_service_type.c.
 */
void
afw_log_internal_register_service_type(afw_xctx_t *xctx);


/** @brief Configuration handler for entry type "log". */
void afw_log_internal_conf_type_create_cede_p(
    const afw_utf8_t *type,
    const afw_object_t *entry,
    const afw_utf8_t *source_location,
    const afw_pool_t *p, afw_xctx_t *xctx);


/** @brief Register logType context type. */
void afw_log_internal_register_logType_context_type(
    const afw_utf8_t *log_type_id,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_LOG_INTERNAL_H__ */
