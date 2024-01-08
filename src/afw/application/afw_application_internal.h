// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework application internal header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_APPLICATION_INTERNAL_H__
#define __AFW_APPLICATION_INTERNAL_H__

#include "afw_interface.h"

/**
 * @defgroup afw_application_internal Application
 * @ingroup afw_c_api_internal
 *
 * Application internal.
 * @{
 */

/**
 * @file afw_application_internal.h
 * @brief Application internal header.
 */

AFW_BEGIN_DECLARES

/**
 * @brief Push application qualifiers on xctx's qualifier stack.
 * @param xctx of caller.
 */
void
afw_application_internal_push_qualifiers(afw_xctx_t *xctx);


/**
 * @brief Set basic application context type.
 *
 * If there is an application conf object,
 * afw_application_internal_application_conf_type_create_cede_p()
 * will added additional qualifier/variable definitions.
 */
void
afw_application_internal_register_basic_application_context_type(
    afw_xctx_t *xctx);


/**
 * @brief type=application conf handler.
 */
void afw_application_internal_application_conf_type_create_cede_p(
    const afw_utf8_t *type,
    const afw_object_t *conf,
    const afw_utf8_t *source_location,
    const afw_pool_t *p, afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_APPLICATION_INTERNAL_H__ */
