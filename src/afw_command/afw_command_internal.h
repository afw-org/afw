// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_command internal header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_COMMAND_INTERNAL_H__
#define __AFW_COMMAND_INTERNAL_H__

#include "afw_doxygen.h"
#include "afw_interface.h"
#include "generated/afw_command_generated.h"
#include "generated/afw_command_version_info.h"

/**
 * @defgroup afw_command_internal Command line interface internal
 * @ingroup afw_included_commands
 *
 * This is internal documentation and thus is not API documentation. Anything
 * can change between releases and even between patches, so do not use outside
 * of the source for this command.
 * @{
 */

/**
 * @file afw_command_internal.h
 * @brief Adaptive Framework afw command internal header.
 */

AFW_BEGIN_DECLARES

/**
 * @brief Self typedef for afw_command self
 */
typedef struct afw_command_self_s {
    afw_xctx_t *xctx;
    const afw_object_t *environment_variables_object;
    int index_first_non_option;

    const afw_utf8_t *source_location;

    union {
        afw_utf8_t in;
        const afw_utf8_z_t * in_z;
    };

    union {
        afw_utf8_t conf;
        const afw_utf8_z_t * conf_z;
    };

    union {
        afw_utf8_t expression;
        const afw_utf8_z_t * expression_z;
    };

    union {
        afw_utf8_t extension;
        const afw_utf8_z_t * extension_z;
    };

    union {
        afw_utf8_t type_in;
        const afw_utf8_z_t *type_in_z;
    };

    union {
        afw_utf8_t type_out;
        const afw_utf8_z_t *type_out_z;
    };

    union {
        afw_utf8_t local_mode_out;
        const afw_utf8_z_t *local_mode_z;
    };

    const afw_content_type_t *content_type_in;

    const afw_content_type_t *content_type_out;

    const afw_memory_t *in_content;

    FILE *fd_input;

    FILE *fd_output;

    const afw_server_t *local_server;

    apr_array_header_t *input_buffer;

    afw_utf8_octet_get_cb_t callback;

    void *callback_data;

    afw_compile_type_t compile_option;

    afw_compile_residual_check_t residual_check;

    afw_boolean_t help_option;

    afw_boolean_t interactive_mode;

    afw_boolean_t check_mode;

    afw_boolean_t can_span_lines;

    afw_boolean_t read_full;

    afw_boolean_t terminal_error;

    afw_boolean_t eof;

    int local_model_fd_num;

} afw_command_self_t;

AFW_END_DECLARES

/** @} */

#endif /* __AFW_COMMAND_INTERNAL_H__ */
