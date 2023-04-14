// See the 'COPYING' file in the project root for licensing information.
/*
 * Parse functions to support afw command local server
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


#include "afw.h"
#include "afw_command_internal.h"
#include "afw_command_local_parse.h"

/**
 * @addtogroup afw_command_internal
 * @{
 */

/**
 * @file afw_command_local_parse.c
 * @brief Implementation for interface afw_command_local
 *
 * This is the implementation of afw_server for afw_command_local.
 */

static void
impl_get_token(afw_command_local_parse_t *x)
{
    const afw_byte_t *start;

    for (;;) {
        if (x->cursor >= x->end) {
            x->token_type = afw_command_local_parse_token_type_end_of_input;
            return;
        }
        if (*x->cursor != ' ') {
            break;
        }
        x->cursor++;
    }

    if (*x->cursor == '\r') {
        x->token_type = afw_command_local_parse_token_type_end_of_line;
        x->cursor++;
        if (x->cursor >= x->end) {
            return;
        }
        if (*x->cursor == '\n') {
            x->cursor++;
            return;
        }
    }

    if (*x->cursor == '\n') {
        x->token_type = afw_command_local_parse_token_type_end_of_line;
        x->cursor++;
        return;
    }

    if (*x->cursor == ':') {
        x->token_type = afw_command_local_parse_token_type_colon;
        x->cursor++;
        return;
    }

    if (*x->cursor == '"') {
        x->cursor++;
        start = x->cursor;

        for (;; x->cursor++) {
            if (x->cursor >= x->end) {
                x->token_type = afw_command_local_parse_token_type_invalid;
                return;
            }
            if (*x->cursor == '"') {
                /** @todo need to deal with escaped stuff. */
                x->token_type = afw_command_local_parse_token_type_string;
                x->string = afw_utf8_create_copy(
                    (const afw_utf8_octet_t *)start,
                    (x->cursor - start),
                    x->p, x->xctx);
                x->cursor++;
                return;
            }
        }
    }

    for (start = x->cursor; ; x->cursor++) {
        if (x->cursor >= x->end) {
            x->token_type = afw_command_local_parse_token_type_invalid;
            return;
        }
        if (*x->cursor == ' ' ||
            *x->cursor == '\n' ||
            *x->cursor == '\r' ||
            *x->cursor == ':')
        {
            x->token_type = afw_command_local_parse_token_type_string;
            x->string = afw_utf8_create_copy(
                (const afw_utf8_octet_t *)start,
                (x->cursor - start),
                x->p, x->xctx);
            return;
        }
    }
}


/* This is a very minimal parser and should only be used for http-like. */
AFW_COMMAND_DEFINE_INTERNAL(const afw_memory_t *)
afw_command_local_parse_request(
    afw_command_local_server_self_t *self,
    const afw_memory_t **input,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *name;
    afw_memory_t *payload;
    afw_command_local_parse_t x;

    afw_memory_clear(&x);
    x.xctx = xctx;
    x.p = xctx->p;
    x.input = *input;
    x.cursor = x.input->ptr;
    x.end = (*input)->ptr + (*input)->size;
    x.line_number = 1;

    /* METHOD */
    impl_get_token(&x);
    if (x.token_type != afw_command_local_parse_token_type_string) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "Line %" AFW_SIZE_T_FMT ": Expecting METHOD",
            x.line_number);
    }
    afw_object_set_property_as_string(self->this_request_properties,
        &afw_s_REQUEST_METHOD, x.string, xctx);
 
    /* URI */
    impl_get_token(&x);
    if (x.token_type != afw_command_local_parse_token_type_string) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "Line %" AFW_SIZE_T_FMT ": Expecting URI",
            x.line_number);
    }
    afw_object_set_property_as_string(self->this_request_properties,
        &afw_s_REQUEST_URI, x.string, xctx);
 
    /* SERVER_PROTOCOL */
    impl_get_token(&x);
    if (x.token_type != afw_command_local_parse_token_type_string) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "Line %" AFW_SIZE_T_FMT ": Expecting URI",
            x.line_number);
    }
    afw_object_set_property_as_string(self->this_request_properties,
        &afw_command_s_SERVER_PROTOCOL, x.string, xctx);

    /* End of line */
    impl_get_token(&x);
    if (x.token_type != afw_command_local_parse_token_type_end_of_line) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "Line %" AFW_SIZE_T_FMT ": Expecting end of line",
            x.line_number);
    }
    if (x.cursor >= x.end) {
        return (const afw_memory_t *)&afw_s_a_empty_string;
    }
 
    /* Process all headers. */
    for (;;) {
        x.line_number++;

        impl_get_token(&x);

        /* Second end of line starts payload. */
        if (x.token_type == afw_command_local_parse_token_type_end_of_line) {
            payload = afw_xctx_malloc_type(afw_memory_t, xctx);
            payload->ptr = (const afw_byte_t *)x.cursor;
            payload->size = x.end - x.cursor;
            return payload;
        }

        /* End of input indicates no payload. */
        if (x.token_type == afw_command_local_parse_token_type_end_of_input) {
            return (const afw_memory_t *)&afw_s_a_empty_string;
        }

        /* name */
        if (x.token_type != afw_command_local_parse_token_type_string) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Line %" AFW_SIZE_T_FMT ": Expecting header name",
                x.line_number);
        }
        name = x.string;

        /* colon */
        impl_get_token(&x);
        if (x.token_type != afw_command_local_parse_token_type_colon) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Line %" AFW_SIZE_T_FMT ": Expecting colon",
                x.line_number);
        }

        /* value */
        impl_get_token(&x);
        if (x.token_type != afw_command_local_parse_token_type_string) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Line %" AFW_SIZE_T_FMT ": Expecting header value",
                x.line_number);
        }

        /* Add header to properties. */
        afw_object_set_property_as_string(self->this_request_properties,
            name, x.string, xctx);

        /* EOL */
        impl_get_token(&x);
        if (x.token_type != afw_command_local_parse_token_type_end_of_line) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Line %" AFW_SIZE_T_FMT ": Expecting end of line",
                x.line_number);
        }
    }
   
    return payload;
}

/** @} */
