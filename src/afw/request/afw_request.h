// See the 'COPYING' file in the project root for licensing information.
/*
 * Helpers for interfaces afw_request*
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_REQUEST_H__
#define __AFW_REQUEST_H__

#include "afw_interface.h"

/**
 * @defgroup afw_request Request
 * @ingroup afw_c_api_public
 *
 * Provide afw_request interface common support.
 * 
 * @{
 */

/**
 * @file afw_request.h
 * @brief Header for interface afw_request* helpers.
 */

AFW_BEGIN_DECLARES

/** @brief Request property name CONTENT_TYPE quoted string.
 *
 * The content type of the request.
 */
#define AFW_REQUEST_Q_PN_CONTENT_TYPE AFW_Q_CONTENT_TYPE

/** @brief Request property name CONTENT_TYPE afw_utf8_t.
 *
 * The content type of the request.
 */
#define AFW_REQUEST_s_PN_CONTENT_TYPE afw_s_CONTENT_TYPE

/** @brief Request property name CONTENT_LENGTH quoted string.
 *
 * The content length of the request, if available.
 */
#define AFW_REQUEST_Q_PN_CONTENT_LENGTH AFW_Q_CONTENT_LENGTH

/** @brief Request property name CONTENT_LENGTH afw_utf8_t.
 *
 * The content length of the request, if available.
 */
#define AFW_REQUEST_s_PN_CONTENT_LENGTH afw_s_CONTENT_LENGTH

/**
 * @brief Request property name HTTP_ACCEPT quoted string.
 */
#define AFW_REQUEST_Q_PN_HTTP_ACCEPT AFW_Q_HTTP_ACCEPT

/**
 * @brief Request property name HTTP_ACCEPT afw_utf8_t.
 */
#define AFW_REQUEST_s_PN_HTTP_ACCEPT afw_s_HTTP_ACCEPT

/**
 * @brief Request property name PATH_INFO quoted string.
 *
 * The query string.
 */
#define AFW_REQUEST_Q_PN_PATH_INFO AFW_Q_PATH_INFO

/**
 * @brief Request property name PATH_INFO afw_utf8_t.
 *
 * The query string.
 */
#define AFW_REQUEST_s_PN_PATH_INFO afw_s_PATH_INFO

/**
 * @brief Request property name QUERY_STRING quoted string.
 *
 * The query string.
 */
#define AFW_REQUEST_Q_PN_QUERY_STRING AFW_Q_QUERY_STRING

/**
 * @brief Request property name QUERY_STRING afw_utf8_t.
 *
 * The query string.
 */
#define AFW_REQUEST_s_PN_QUERY_STRING afw_s_QUERY_STRING

/**
 * @brief Request property name REQUEST_METHOD quoted string.
 *
 * The http method of the request. Usually, "GET", "POST", "PUT", "PATCH", or
 * "DELETE".
 */
#define AFW_REQUEST_Q_PN_REQUEST_METHOD AFW_Q_REQUEST_METHOD

/**
 * @brief Request accept header REQUEST_METHOD afw_utf8_t.
 *
 * The http method of the request. Usually, "GET", "POST", "PUT", "PATCH", or
 * "DELETE".
 */
#define AFW_REQUEST_s_PN_REQUEST_METHOD afw_s_REQUEST_METHOD

/**
 * @brief Request property name REQUEST_URI quoted string.
 *
 * The query string.
 */
#define AFW_REQUEST_Q_PN_REQUEST_URI AFW_Q_REQUEST_URI

/**
 * @brief Request property name REQUEST_URI afw_utf8_t.
 *
 * The query string.
 */
#define AFW_REQUEST_s_PN_REQUEST_URI afw_s_REQUEST_URI

/** @brief Request method GET quoted string. */
#define AFW_REQUEST_Q_METHOD_GET AFW_Q_GET

/** @brief Request method GET afw_utf8_t */
#define AFW_REQUEST_s_METHOD_GET afw_s_GET

/** Request method POST quoted string. */
#define AFW_REQUEST_Q_METHOD_POST AFW_Q_POST

/** Request method POST afw_utf8_t. */
#define AFW_REQUEST_s_METHOD_POST afw_s_POST

/** Request method PUT quoted string. */
#define AFW_REQUEST_Q_METHOD_PUT AFW_Q_PUT

/** Request method PUT afw_utf8_t. */
#define AFW_REQUEST_s_METHOD_PUT afw_s_PUT

/** Request method PATCH quoted string. */
#define AFW_REQUEST_Q_METHOD_PATCH AFW_Q_PATCH

/** Request method PATCH afw_utf8_t. */
#define AFW_REQUEST_s_METHOD_PATCH afw_s_PATCH

/** Request method DELETE quoted string. */
#define AFW_REQUEST_Q_METHOD_DELETE AFW_Q_DELETE

/** Request method DELETE afw_utf8_t. */
#define AFW_REQUEST_s_METHOD_DELETE afw_s_DELETE


/** Request state enum.  This CAN be used by implementations of afw_request. */
typedef enum afw_request_state_e {
    afw_request_state_initial,
    afw_request_state_content_read,
    afw_request_state_status_set,
    afw_request_state_header_written,
    afw_request_state_response_written,
    afw_request_state_response_finished
} afw_request_state_t;

/**
 * @brief Read a request body into a utf-8 string in a specifed pool.
 * @param request instance
 * @param p Pool for string.
 * @param xctx of caller.
 * @return string containing request body.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_request_body_to_utf8(
    const afw_request_t * instance,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Read a request body to value in a specifed pool.
 * @param request instance
 * @param p Pool for string.
 * @param xctx of caller.
 * @return value
 *
 * The request body is parsed based on content-type header.
 */
AFW_DECLARE(const afw_value_t *)
afw_request_body_to_value(
    const afw_request_t * instance,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/*
 * @brief Get response content type.
 * @param request instance.
 * @param response_content_type return address.
 * @param type return address.
 * @param type_parameter return address.
 * @param xctx of caller.
 */
AFW_DEFINE(void)
afw_request_get_response_content_type(
    const afw_request_t *instance,
    const afw_content_type_t **response_content_type,
    const afw_utf8_t **type,
    const afw_utf8_t **type_parameter,
    afw_xctx_t *xctx);

    
/*
 * @brief Get response content type and set header first time called.
 * @param request instance
 * @param xctx of caller.
 * @return content type instance
 */
AFW_DECLARE(const afw_content_type_t *)
afw_request_prepare_response_content_type(
    const afw_request_t * instance, afw_xctx_t *xctx);


/*
 * @brief Create writer for response body.
 * @param request instance
 * @param xctx of caller.
 * @return afw_writer instance
 */
AFW_DEFINE(const afw_stream_t *)
afw_request_response_body_raw_writer_create(
    const afw_request_t *request,
    afw_xctx_t *xctx);



/**
 * @brief Write value to response body.
 * @param request instance
 * @param value to write.
 * @param options.
 * @param xctx of caller.
 *
 * The value is converted based on the content-type header and written to the
 * response body.
 */
AFW_DECLARE(void)
afw_request_write_value_to_response_body(
    const afw_request_t * instance,
    const afw_value_t *value,
    const afw_object_options_t *options,
    afw_xctx_t *xctx);



/**
 * @brief Write xctx error to response body.
 * @param request instance
 * @param http response code.
 * @param error to write.
 * @param xctx of caller.
 *
 * This function leaves xctx->error unchanged if it is successful.
 */
AFW_DECLARE(void)
afw_request_write_error_to_response_body(
    const afw_request_t * instance,
    const afw_utf8_t *code,
    const afw_error_t *error,
    afw_xctx_t *xctx);



/**
 * @brief Write simple success to response body.
 * @param request instance
 * @param response is a memory object where response meta will be added.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_request_write_success_response(
    const afw_request_t * instance,
    const afw_object_t * response,
    afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_REQUEST_H__ */
