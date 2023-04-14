// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_server implementation for afw_command_local
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_COMMAND_LOCAL_SERVER_H__
#define __AFW_COMMAND_LOCAL_SERVER_H__

#include "afw_interface.h"
#include "afw_command_internal.h"
#include "generated/afw_command_declare_helpers.h"

/**
 * @addtogroup afw_command_internal
 * @{
 */

/**
 * @file afw_command_local_server.h
 * @brief Implementation for interface afw_command_local
 *
 * This is the implementation of afw_server for afw_command_local.
 */

AFW_BEGIN_DECLARES

typedef enum {
    afw_command_local_server_mode_action,
    afw_command_local_server_mode_action_direct,
    afw_command_local_server_mode_evaluate,
    afw_command_local_server_mode_evaluate_direct, // default
    afw_command_local_server_mode_http_like
} afw_command_local_server_mode_t;




/**
 * @brief Self typedef for afw_command_local implementation of afw_server.
 */
typedef struct
afw_command_local_server_self_s {
    afw_server_t pub;
    afw_command_self_t *command_self;
    const afw_request_handler_t *director;
    
    apr_array_header_t *input_buffer;
    const afw_content_type_t *content_type;

    const afw_utf8_t *evaluate_function_id;

    FILE *fd_input;
    FILE *fd_output;

    afw_boolean_t fatal_error;
    afw_boolean_t eof;

    afw_command_local_server_mode_t mode;

    const afw_object_t *request_properties;

    union {
        const afw_object_t *properties_array[4];
        struct {
            const afw_object_t *this_request_properties;
            const afw_object_t *multi_request_mode_properties;
            const afw_object_t *multi_request_properties;
            const afw_object_t *should_be_NULL;
        };
    };


} afw_command_local_server_self_t;



AFW_COMMAND_DECLARE_INTERNAL(const afw_server_t *)
afw_command_local_server_create(
    afw_command_self_t *command_self);


AFW_COMMAND_DECLARE_INTERNAL(void)
afw_command_local_server_write_result(
    afw_command_local_server_self_t *self,
    const char *format, ...);


AFW_COMMAND_DECLARE_INTERNAL(void)
afw_command_local_server_write_error(
    afw_command_local_server_self_t *self, 
    const afw_error_t *error,
    afw_xctx_t *xctx);


AFW_COMMAND_DECLARE_INTERNAL(void)
afw_command_local_server_write_end(
    afw_command_local_server_self_t *self);



AFW_END_DECLARES

#endif /* __AFW_COMMAND_LOCAL_SERVER_H__ */

/** @} */
