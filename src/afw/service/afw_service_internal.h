// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Service Support Internal
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_SERVICE_INTERNAL_H__
#define __AFW_SERVICE_INTERNAL_H__

#include "afw_interface.h"

/**
 * @defgroup afw_service_internal Service
 * @ingroup afw_c_api_internal
 * 
 * Internal service API.
 * 
 * @{
 */

AFW_BEGIN_DECLARES

/* Special routine for _AdaptiveService_ session retrieve_objects(). */
void
afw_service_internal_AdaptiveService_retrieve_objects(
    const afw_adaptor_session_t * instance,
    const afw_adaptor_impl_request_t * impl_request,
    const afw_utf8_t * object_type_id,
    const afw_query_criteria_t * criteria,
    void * context,
    afw_object_cb_t callback,
    const afw_object_t *adaptor_type_specific,
    const afw_pool_t * p,
    afw_xctx_t *xctx);

/* Special routine for _AdaptiveService_ session get_object(). */
void
afw_service_internal_AdaptiveService_get_object(
    const afw_adaptor_session_t * instance,
    const afw_adaptor_impl_request_t * impl_request,
    const afw_utf8_t * object_type_id,
    const afw_utf8_t * object_id,
    void * context,
    afw_object_cb_t callback,
    const afw_object_t *adaptor_type_specific,
    const afw_pool_t * p,
    afw_xctx_t *xctx);

/**
 * @internal
 * @brief Start initial services defined in conf adaptor.
 * @param service id.
 * @param p passed to
 *    afw_application_internal_application_conf_type_create_cede_p().
 * @param xctx of caller.
 *
 * Services are defined by object type _AdaptiveServiceConf_ objects
 * in the application's confAdaptorId adaptor.
 *
 * This is called from
 * afw_application_internal_application_conf_type_create_cede_p() with the p
 * passed to start services with startup equal "immediate" and "permanent".
 */
void
afw_service_internal_start_initial_services(
    const afw_pool_t *p, afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_SERVICE_INTERNAL_H__ */
