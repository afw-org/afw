// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Service Support
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_SERVICE_H__
#define __AFW_SERVICE_H__

/** @fixme Can be afw_minimal.h instead if not part of it's #includes. Keep simple. */
#include "afw_interface.h"

/**
 * @defgroup afw_service Service support
 * @ingroup afw_c_api_public
 * 
 * Adaptive service API.
 * 
 * @{
 */

/**
 * @file afw_service.h
 * @brief Adaptive Framework service support
 */

AFW_BEGIN_DECLARES

/**
 * @brief Function passed to afw_service_start_cede_p.
 */
typedef void (*afw_service_create_cede_p_t)(
    afw_service_t *service,
    void *data,
    const afw_object_t *properties,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Runtime service struct.
 *
 * There is one of these for each service that is registered.
 */
struct afw_service_s {

    /** @brief Pool that holds service's resources. */
    const afw_pool_t *p;

    /**
     * @brief Service's name.
     *
     * The service name is a type followed by a dash ('-') followed by an
     * associated id.  The type must be a registered configuration type that
     * has isService=true.  The id an appropriate id based on the type.  For
     * example, for type adaptor, the associated id is the adaptorId.
     */
    afw_utf8_t service_id;

    /**
     * @brief The part before the dash ('-') in service_id.
     *
     * This is also the conf_type.
     */
    const afw_utf8_t *type;

    /** @brief afw_service_type instance for this service_type_id. */
    const afw_service_type_t *service_type;

    /** @brief The part after the dash ('-') in the service id. */
    const afw_utf8_t *conf_id;

    /** @brief This is the value of the subtype property from conf or NULL. */
    const afw_utf8_t *conf_subtype;

    /** @brief The source location associated with this service. */
    const afw_utf8_t *source_location;

    /** @brief The properties object containing runtime and conf properties. */
    const afw_object_t *properties;

    /** @brief The source location associated with the conf for this service. */
    const afw_utf8_t *conf_source_location;

    /** @brief The status of the service. */
    afw_service_status_t status;

    /** @brief Start time. */
    afw_dateTime_t start_time;

    /**
     * @brief Optional status message.
     *
     * In the case of status=error, this is the error message.
     */
    const afw_utf8_t *status_message;

    /**
     * @brief Optional status debug.
     *
     * This is a debug version of status_message.
     */
    const afw_utf8_t *status_debug;

    /** @brief Mutex used when changing status. */
    afw_thread_mutex_t *mutex;

    /** @fixme Might go away. */
    void *data;

    /** @brief Has a service conf object. */
    afw_boolean_t has_service_conf;
};


/**
 * @brief Prepare expression evaluation context for service::.
 * @param service for which the request is being made.
 * @param p used for result.
 * @param xctx of caller.
 * @param _AdaptiveAuthorizationResult_ object.
 */
AFW_DECLARE(void)
afw_service_context_prepare(
    const afw_service_t *service,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Convert afw_service_startup_t enum to corresponding utf8.
 * @param startup enum.
 * @return corresponding utf8.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_service_startup_as_utf8(afw_service_startup_t startup);


/**
 * @brief Convert afw_service_startup_t enum to corresponding adaptive value.
 * @param startup enum.
 * @return corresponding adaptive value.
 */
AFW_DECLARE(const afw_value_t *)
afw_service_startup_as_value(afw_service_startup_t startup);


/**
 * @brief Get description for a afw_service_startup_t enum.
 * @param startup enum.
 * @return description.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_service_startup_description(afw_service_startup_t startup);


/**
 * @brief Get description as value for a afw_service_startup_t enum.
 * @param startup enum.
 * @return description.
 */
AFW_DECLARE(const afw_value_t *)
afw_service_startup_description_as_value(afw_service_startup_t startup);


/**
 * @brief Convert utf8 to corresponding afw_service_startup_t enum.
 * @param startup utf8.
 * @return corresponding utf8.
 */
AFW_DECLARE(afw_service_startup_t)
afw_service_startup_as_enum(const afw_utf8_t *s);


/**
 * @brief Convert afw_service_status_t enum to corresponding utf8.
 * @param status enum.
 * @return corresponding utf8.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_service_status_as_utf8(afw_service_status_t status);


/**
 * @brief Convert afw_service_status_t enum to corresponding adaptive value.
 * @param status enum.
 * @return corresponding adaptive value.
 */
AFW_DECLARE(const afw_value_t *)
afw_service_status_as_value(afw_service_status_t status);


/**
 * @brief Get description for a afw_service_status_t enum.
 * @param status enum.
 * @return description.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_service_status_description(afw_service_status_t status);


/**
 * @brief Get description as value for a afw_service_status_t enum.
 * @param status enum.
 * @return description.
 */
AFW_DECLARE(const afw_value_t *)
afw_service_status_description_as_value(afw_service_status_t status);


/**
 * @brief Convert utf8 to corresponding afw_service_status_t enum.
 * @param status utf8.
 * @return corresponding utf8.
 */
AFW_DECLARE(afw_service_status_t)
afw_service_status_as_enum(const afw_utf8_t *s);


/** @brief Get a service object. */
AFW_DECLARE(const afw_object_t *)
afw_service_get_object(
    const afw_utf8_t *service_id,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Start a service using _AdaptiveConf_ object and cede p.
 * @param conf is compiled _AdaptiveConf_ derived object.
 * @param source_location of where service defined
 * @param p to cede control to create function.
 * @param xctx of caller.
 *
 * Parameter conf should be a _AdaptiveConf_ object that is in the
 * p pool that has been validated and any compilable values compiled.
 */
AFW_DECLARE(void)
afw_service_start_using_AdaptiveConf_cede_p(
    const afw_object_t *properties,
    const afw_utf8_t *source_location,
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Start a service.
 * @param service_id to start.
 * @param manual_start true indicates this is a manual start.
 * @param xctx of caller.
 *
 * Start service if it is not already started.  Services with
 * startup immediate and permanent can be started.  If
 * manual_start is true, startup manual can also be started.
 */
AFW_DECLARE(void)
afw_service_start(
    const afw_utf8_t *service_id,
    afw_boolean_t manual_start,
    afw_xctx_t *xctx);



/**
 * @brief Stop a service.
 * @param service_id to start.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_service_stop(
    const afw_utf8_t *service_id,
    afw_xctx_t *xctx);



/**
 * @brief Restart a service.
 * @param service_id to start.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_service_restart(
    const afw_utf8_t *service_id,
    afw_xctx_t *xctx);



AFW_END_DECLARES

/** @} */

#endif /* __AFW_SERVICE_H__ */
