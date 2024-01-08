// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_request implementation for afw_command_local
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_COMMAND_LOCAL_REQUEST_H__
#define __AFW_COMMAND_LOCAL_REQUEST_H__

#include "afw_interface.h"
#include "afw_command_local_server.h"
#include "generated/afw_command_declare_helpers.h"


/**
 * @addtogroup afw_command_internal
 * @{
 */

/**
 * @file afw_command_local_request.h
 * @brief Implementation for interface afw_command_local
 *
 * This is the implementation of afw_request for afw_command_local.
 */


AFW_BEGIN_DECLARES

/**
 * @brief Self typedef for afw_command_local implementation of afw_request.
 */
typedef struct
afw_command_local_request_self_s {
    afw_request_t pub;

    afw_command_local_server_self_t *server_self;
    const afw_memory_t *body;
    afw_size_t remaining_body;
  
    /* Request state. */
    afw_request_state_t state;

    /* These are pairs of const afw_utf8_t * for name and value or NULL. */
    apr_array_header_t *response_headers;

    /* Response status code and reason. */
    const afw_utf8_t *status_code;
    const afw_utf8_t *status_reason;

} afw_command_local_request_self_t;



AFW_COMMAND_DECLARE_INTERNAL(const afw_request_t *)
afw_command_local_request_create(
    afw_command_local_server_self_t *server_self,
    const afw_memory_t *body,
    const afw_object_t *properties,
    afw_xctx_t *xctx);



AFW_END_DECLARES

#endif /* __AFW_COMMAND_LOCAL_REQUEST_H__ */

/** @} */
