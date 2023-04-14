// See the 'COPYING' file in the project root for licensing information.
/*
 * Parse functions to support afw command local server
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_COMMAND_LOCAL_PARSE_H__
#define __AFW_COMMAND_LOCAL_PARSE_H__

#include "afw_interface.h"
#include "afw_command_local_server.h"
#include "generated/afw_command_declare_helpers.h"

/**
 * @addtogroup afw_command_internal
 * @{
 */

/**
 * @file afw_command_local_parse.h
 * @brief Parse function to support afw command local server
 *
 * This is the implementation of afw_server for afw_command_local.
 */

AFW_BEGIN_DECLARES


typedef enum {
    afw_command_local_parse_token_type_invalid,
    afw_command_local_parse_token_type_colon,
    afw_command_local_parse_token_type_end_of_input,
    afw_command_local_parse_token_type_end_of_line,
    afw_command_local_parse_token_type_string
} afw_command_local_parse_token_type_t;


typedef struct {
    const afw_pool_t *p;
    afw_xctx_t *xctx;
    const afw_memory_t *input;
    const afw_byte_t *cursor;
    const afw_byte_t *end;
    const afw_utf8_t *string; // Only for token type string
    afw_size_t line_number;
    afw_command_local_parse_token_type_t token_type;
} afw_command_local_parse_t;


AFW_COMMAND_DECLARE_INTERNAL(const afw_memory_t *)
afw_command_local_parse_request(
    afw_command_local_server_self_t *self,
    const afw_memory_t **input,
    afw_xctx_t *xctx);


AFW_END_DECLARES

#endif /* __AFW_COMMAND_LOCAL_PARSE_H__ */

/** @} */
