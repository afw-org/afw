// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Authorization
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_AUTHORIZATION_INTERNAL_H__
#define __AFW_AUTHORIZATION_INTERNAL_H__

#include "afw_interface.h"

/**
 * @defgroup afw_authorization_internal Authorization
 * @ingroup afw_c_api_internal
 * 
 * Internal authorization API.
 * 
 * @{
 */

/**
 * @file afw_authorization.h
 * @brief Adaptive Framework authorization.
 */

AFW_BEGIN_DECLARES

/**
 * @brief Authorization handler id anchor.
 */
struct afw_authorization_handler_id_s {

    /** @brief Mutex for locking access to authorization handler related resources. */
    afw_thread_mutex_t *authorization_handler_mutex;

    /**
     * @brief First instance.
     *
     * If started, this is the started instance.  Others are stopping and will be
     * be removed when stopped.
     */
    const afw_authorization_handler_t *first;

    /** @brief Stopping instances. */
    afw_boolean_t is_started;
};



/**
 * @internal
 * @brief Authorization control struct 
 */
struct afw_authorization_internal_control_s {

    /** @brief First authorization handler id anchor in priority order. */
    const afw_authorization_handler_id_anchor_t *first_anchor;

    /** @brief Core authorization check or NULL. */
    const afw_value_t *core_authorization_check;

    /** @brief Initial authorization check or NULL. */
    const afw_value_t *initial_authorization_check;

    /**
     * @brief Final notApplicable result.
     *
     * Based on denyIfNotApplicable, this is either a deny or permit result.
     */
    const afw_value_t *not_applicable_result;

    /**
     * @brief Final notApplicable decision id.
     *
     * Based on denyIfNotApplicable, this is either deny or permit.
     */
    const afw_utf8_t *not_applicable_decision_id;

    /** @brief Check authorization for mode intermediate. */
    afw_boolean_t check_intermediate_mode;
};



/**
 * @internal
 * @brief Authorization handler id struct used for
 *    afw_register_authorization_handler_id.
 *
 * Reading and modifying this struct must be protected by
 * env->authorization_handler_id_anchor_rw_lock.
 */
struct afw_authorization_internal_handler_id_anchor_s {
    
    /** @brief Authorization Handler id. */
    const afw_utf8_t *authorization_handler_id;

    /** @brief Next authorization id anchor in priority order. */
    const afw_authorization_handler_id_anchor_t *next_anchor;

    /** @brief Authorization Handler instance or NULL. */
    const afw_authorization_handler_t *authorization_handler;

    /** @brief Associated properties. */
    const afw_object_t *properties;

    /** @brief Associated service id or NULL. */
    const afw_utf8_t *service_id;

    /** @brief Order this authorization handler will be processed. */
    afw_integer_t processing_order;

    /** @brief Reference count for this instance of authorization handler. */
    afw_integer_t reference_count;

    /** @brief First/next stopping authorization handler or NULL. */
    afw_authorization_handler_id_anchor_t *stopping;
};



/** @brief Self for authorization handler type script. */
typedef struct afw_authorization_handler_script_self_s {
    afw_authorization_handler_t pub;

    /* A value that evaluates to an _AdaptiveAuthorizationResult_ object. */
    const afw_value_t *authorization_check;

    /* Qualified variables objects available for authorization_check. */
    const afw_object_t *qualified_variables;

} afw_authorization_handler_script_self_t;



/** @brief Self for authorization check context. */
typedef struct {
    const afw_value_t *requestId;
    const afw_value_t *resourceId;
    const afw_value_t *object;
    const afw_value_t *actionId;
} afw_authorization_internal_authorizationCheck_context_t;


/**
 * @internal
 * @brief Internal function to set env->authorization_control.
 * @param object _AdaptiveAuthorizationControl_
 * @param xctx of caller.
 * @return authorization control.
 */
AFW_DEFINE_INTERNAL(afw_authorization_control_t *)
afw_authorization_internal_set_control(
    const afw_object_t *object,
    afw_xctx_t *xctx);


/**
 * @internal
 * @brief Internal function called to register core authorization.
 * @param xctx of caller.
 */
AFW_DECLARE_INTERNAL(void)
afw_authorization_internal_register(
    afw_xctx_t *xctx);


/**
 * @internal
 * @brief Internal function called to register authorization service and conf.
 * @param xctx of caller.
 *
 * This function is called by afw_authorization_internal_register().
 */
AFW_DECLARE_INTERNAL(void)
afw_authorization_internal_register_service_and_conf(
    afw_xctx_t *xctx);


/**
 * @internal
 * @brief Internal function called to register type=script authorization handler.
 * @param xctx of caller.
 *
 * This function is called by afw_authorization_internal_register().
 */
AFW_DECLARE_INTERNAL(void)
afw_authorization_internal_register_handler_type_script(
    afw_xctx_t *xctx);



/**
 * @brief Create type=script authorization handler.
 * @param properties for handler.
 * @param p to cede to handler.
 * @param xctx of caller.
 */
/* Create type=afw authorization handler. */
AFW_DECLARE(const afw_authorization_handler_t *)
afw_authorization_handler_type_script_create_cede_p(
    const afw_object_t *properties,
    const afw_pool_t *p, afw_xctx_t *xctx);

AFW_END_DECLARES

/** @} */

#endif /* __AFW_AUTHORIZATION_INTERNAL_H__ */
