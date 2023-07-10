// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Error Handling
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_error.c
 * @brief Adaptive Framework Error Handling
 */

#include "afw_internal.h"


typedef struct impl_error_code_map_s {
    afw_utf8_t id;
    afw_boolean_t error_allow_in_response;
    int http_response_code;
    afw_utf8_t description;
    afw_utf8_t html_description;
} impl_error_code_map_t;


static const impl_error_code_map_t impl_error_code_map[] = {
    {
        AFW_UTF8_LITERAL("is_not_specified"),
        false,
        500,
        AFW_UTF8_LITERAL("OK"),
        AFW_UTF8_LITERAL("200 OK")
    },

#define XX(id, error_allow_in_response, http_response_code, description) \
    { \
        AFW_UTF8_LITERAL( #id ), \
        error_allow_in_response, \
        http_response_code, \
        AFW_UTF8_LITERAL( #description ), \
        AFW_UTF8_LITERAL( #http_response_code " " #description ) \
    },
    AFW_ERROR_CODE_MAP(XX)
#undef XX
    
};


static const afw_utf8_t impl_s_a_html_unknown =
    AFW_UTF8_LITERAL("500 Unknown Error");



AFW_DEFINE(void)
afw_error_rv_set_z(
    afw_error_code_t code,
    const afw_utf8_z_t *rv_source_id_z,
    int rv,
    const afw_utf8_z_t * source_z,
    const afw_utf8_z_t * message_z,
    afw_xctx_t *xctx)
{
    afw_environment_error_rv_decoder_z_t decoder;
    afw_utf8_t rv_source_id;

    xctx->error->code = code;
    xctx->error->rv_source_id_z = rv_source_id_z;
    xctx->error->rv = rv;
    xctx->error->rv_decoded_z = NULL;
    xctx->error->source_z = source_z;
    xctx->error->message_z = message_z;

    if (code != afw_error_code_memory) {
        xctx->error->backtrace = afw_os_backtrace(code, -1, xctx);
    }

    /* If rv nonzero and decoder available, call it to get rv_decoded_z. */
    if (rv != 0 && rv_source_id_z)
    {
        rv_source_id.s = rv_source_id_z;
        rv_source_id.len = strlen(rv_source_id_z);
        decoder = afw_environment_get_error_rv_decoder(&rv_source_id, xctx);
        if (decoder) {
            xctx->error->rv_decoded_z = decoder(rv,
                &xctx->error->decode_rv_wa[0],
                sizeof(xctx->error->decode_rv_wa));
        }
    }
}


AFW_DEFINE(void)
afw_error_set_z(
    afw_error_code_t code,
    const afw_utf8_z_t * source_z,
    const afw_utf8_z_t * message_z,
    afw_xctx_t *xctx)
{
    afw_error_rv_set_z(code, NULL, 0, source_z, message_z,
        xctx);
}


AFW_DEFINE_ELLIPSIS(void)
afw_error_rv_set_fz(
    afw_error_code_t code,
    const afw_utf8_z_t *rv_source_id_z,
    int rv,
    const afw_utf8_z_t * source_z,
    afw_xctx_t *xctx,
    const afw_utf8_z_t * format_z, ...)
{
    va_list ap;

    va_start(ap, format_z);
    vsnprintf((char *)&(xctx->error->message_wa[0]),
        sizeof(xctx->error->message_wa),
        (const char *)format_z, ap);
    afw_error_rv_set_z(code, rv_source_id_z, rv, source_z,
        &xctx->error->message_wa[0], xctx);
    va_end(ap);
}

AFW_DEFINE_ELLIPSIS(void)
afw_error_set_fz(
    afw_error_code_t code,
    const afw_utf8_z_t * source_z,
    afw_xctx_t *xctx,
    const afw_utf8_z_t * format_z, ...)
{
    va_list ap;

    va_start(ap, format_z);
    vsnprintf((char *)&(xctx->error->message_wa[0]),
        sizeof(xctx->error->message_wa),
        (const char *)format_z, ap);
    afw_error_rv_set_z(code, NULL, 0, source_z,
        &xctx->error->message_wa[0], xctx);
    va_end(ap);
}


AFW_DEFINE(void)
afw_error_rv_set_vz(
    afw_error_code_t code,
    const afw_utf8_z_t *rv_source_id_z,
    int rv,
    const afw_utf8_z_t * source_z,
    const afw_utf8_z_t * format_z, va_list ap,
    afw_xctx_t *xctx)
{
    vsnprintf((char *)&(xctx->error->message_wa[0]),
        sizeof(xctx->error->message_wa),
        (const char *)format_z, ap);
    afw_error_rv_set_z(code, rv_source_id_z, rv, source_z,
        &xctx->error->message_wa[0], xctx);
}


AFW_DEFINE(void)
afw_error_set_vz(
    afw_error_code_t code,
    const afw_utf8_z_t * source_z,
    const afw_utf8_z_t * format_z, va_list ap,
    afw_xctx_t *xctx)
{
    vsnprintf((char *)&(xctx->error->message_wa[0]),
        sizeof(xctx->error->message_wa),
        (const char *)format_z, ap);
    afw_error_rv_set_z(code, NULL, 0, source_z,
        &xctx->error->message_wa[0], xctx);
}


static void
impl_write_source_lines(
    const afw_writer_t *w,
    const afw_utf8_t *source,
    afw_size_t error_line,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_size_t j, line;
    afw_size_t offset;
    afw_size_t number_of_lines;
    afw_size_t max_column_number;
    char buf2[(AFW_SIZE_T_MAX_BUFFER * 3) + 8];
    int offset_cell_octets;
    int line_cell_octets;
    afw_boolean_t new_line;

    afw_utf8_line_count_and_max_column(
        &number_of_lines, &max_column_number, source, 4, xctx);
    
    offset_cell_octets =
        afw_number_bytes_needed_size_t(source->len);

    line_cell_octets =
        afw_number_bytes_needed_size_t(number_of_lines);

    /* Check for \n. */
    line = 1;
    new_line = true;

    /* Loop writing source. */
    for (j = 0, offset = 0; j < source->len; j++)
    {
        if (source->s[j] == '\t')
        {
            afw_writer_write_z(w, "    ", xctx);
            offset += 4;
        }
        else {
            if (new_line) {
                new_line = false;
                snprintf(buf2, AFW_SIZE_T_MAX_BUFFER,
                    "%*" AFW_SIZE_T_FMT "  "
                    "%*" AFW_SIZE_T_FMT,
                    offset_cell_octets, offset,
                    line_cell_octets, line);
                afw_writer_write_z(w, buf2, xctx);
                if (error_line == line) {
                    afw_writer_write_z(w, " > ", xctx);
                }
                else {
                    afw_writer_write_z(w, " | ", xctx);
                }
            }
            if (source->s[j] == '\n')
            {
                if (line != 0) {
                    line++;
                    new_line = true;
                }
            }
            afw_writer_write(w, source->s + j, 1, xctx);
            offset++;
        }
    }
    if (line == 0 || !new_line) {
        afw_writer_write_eol(w, xctx);
    }   
}



static const afw_utf8_t *
impl_evaluation_backtrace(
    const afw_error_t *error,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_writer_t *w;
    afw_utf8_t s;
    const afw_utf8_t *result;
    afw_value_info_t info;
    char buf[AFW_INTEGER_MAX_BUFFER];
    char buf2[(AFW_SIZE_T_MAX_BUFFER * 3) + 8];
    const afw_compile_value_contextual_t *last_contextual;
    afw_integer_t i, entry_number;
    afw_size_t j, line, parameter_number;
    afw_size_t error_line, error_column;
    afw_size_t error_offset;
    afw_size_t line_number;
    afw_size_t column_number;
    afw_size_t number_of_lines;
    afw_size_t max_column_number;
    afw_size_t offset;
    afw_boolean_t new_line, caret_on_error;
    int offset_cell_octets;
    int line_cell_octets;
    int column_cell_octets;

    if (afw_stack_is_empty(xctx->evaluation_stack) &&
        !error->parser_source)
    {
        return NULL;
    }

    caret_on_error = false;
    last_contextual = NULL;
    offset_cell_octets = 0;
    line_cell_octets = 0;
    column_cell_octets = 0;


    w = afw_utf8_writer_create(&afw_s_a_tab, p, xctx);

    if (error->parser_source) {
        caret_on_error = afw_utf8_line_column_of_offset(
            &error_line, &error_column,
            error->parser_source, error->parser_cursor, 4, xctx);
        afw_writer_write_z(w, "---Compile error:", xctx);
        afw_writer_write(w, error->message_z, strlen(error->message_z), xctx);
        afw_writer_write_eol(w, xctx);
        afw_writer_write_eol(w, xctx);
        impl_write_source_lines(w, error->parser_source, error_line, p, xctx);
        afw_writer_write_eol(w, xctx);      
    }

    for (entry_number = 0,
        i = xctx->evaluation_stack->top - xctx->evaluation_stack->first,
        parameter_number = 0;
        i >= 0;
        i--)
    {

        if (!xctx->evaluation_stack->first[i].value) {
            afw_writer_write_utf8(w, &afw_s_a_undefined_as_string, xctx);
            afw_writer_write_eol(w, xctx);
            continue;
        }

        if (xctx->evaluation_stack->first[i].entry_id ==
            &afw_s_parameter_number)
        {
            i--;
            parameter_number =
                xctx->evaluation_stack->first[i].parameter_number;
            continue;
        }


        /* This should not need to be here, so just note to avoid crash. */
        if (xctx->evaluation_stack->first[i].parameter_number < 100)
        {
            afw_writer_write_z(w,
                "--- Unexpected parameter number. "
                "Evaluation stack error.",
                xctx);
            afw_writer_write_eol(w, xctx);
            parameter_number =
                xctx->evaluation_stack->first[i].parameter_number;
            continue;
        }

        afw_value_get_info(xctx->evaluation_stack->first[i].value, &info,
            w->p, xctx);

        /* Source location and source */
        if (info.contextual && info.contextual->compiled_value)
        {

            /* Source location each time it changes */
            if (!last_contextual ||
                last_contextual->source_location !=
                info.contextual->source_location)
            {
                error_offset = info.contextual->value_offset;
                if (entry_number == 0) {
                    caret_on_error = afw_utf8_line_column_of_offset(
                        &error_line, &error_column,
                        info.contextual->compiled_value->full_source,
                        info.contextual->value_offset, 4, xctx);
                }
                else {
                    caret_on_error = false;
                    afw_writer_write_eol(w, xctx);
                }
                afw_writer_write_z(w, "---CompiledValue ", xctx);
                entry_number++;
                snprintf(buf, AFW_INTEGER_MAX_BUFFER, "%" AFW_INTEGER_FMT,
                    entry_number);
                afw_writer_write_z(w, buf, xctx);

                afw_writer_write_eol(w, xctx);
            }

            /* Note when source not available. Probably needs to fix. */
            if (!info.contextual->compiled_value) {
                afw_writer_write_eol(w, xctx);
                afw_writer_write_z(w, "---Source <None>", xctx);
                afw_writer_write_eol(w, xctx);
                afw_writer_write_eol(w, xctx);
                afw_writer_write_z(w, "---Backtrace", xctx);
                afw_writer_write_eol(w, xctx);
            }

            /* Full source each time it changes */
            else if (!last_contextual ||
                (last_contextual->compiled_value &&
                last_contextual->compiled_value->full_source !=
                    info.contextual->compiled_value->full_source &&
                info.contextual->compiled_value->full_source))
            {             
                afw_utf8_line_count_and_max_column(
                    &number_of_lines, &max_column_number,
                    info.contextual->compiled_value->full_source,
                    4, xctx);

                offset_cell_octets =
                    afw_number_bytes_needed_size_t(
                        info.contextual->compiled_value->full_source->len);

                line_cell_octets =
                    afw_number_bytes_needed_size_t(number_of_lines);

                column_cell_octets =
                    afw_number_bytes_needed_size_t(max_column_number);

                afw_writer_write_eol(w, xctx);
                afw_writer_write_z(w, "---Source type:", xctx);

                afw_writer_write_utf8(w,
                    info.contextual->compiled_value->full_source_type,
                    xctx);

                afw_writer_write_z(w, " sourceLocation:", xctx);
                afw_writer_write_utf8(w,
                    info.contextual->source_location,
                    xctx);

                afw_utf8_line_column_of_offset(
                    &line_number, &column_number,
                    info.contextual->compiled_value->full_source,
                    info.contextual->value_offset, 4, xctx);
                afw_writer_write_z(w, " errorBetween:", xctx);
                snprintf(buf2, AFW_SIZE_T_MAX_BUFFER,
                    "%" AFW_SIZE_T_FMT
                    "(%" AFW_SIZE_T_FMT ":%" AFW_SIZE_T_FMT ")",
                    info.contextual->value_offset, line_number, column_number);
                afw_writer_write_z(w, buf2, xctx);

                offset = info.contextual->value_offset +
                    info.contextual->value_size - 1;
                afw_utf8_line_column_of_offset(
                    &line_number, &column_number,
                    info.contextual->compiled_value->full_source,
                    offset, 4, xctx);
                snprintf(buf2, AFW_SIZE_T_MAX_BUFFER,
                    "-%" AFW_SIZE_T_FMT
                    "(%" AFW_SIZE_T_FMT ":%" AFW_SIZE_T_FMT ")",
                    offset, line_number, column_number);
                afw_writer_write_z(w, buf2, xctx);
                afw_writer_write_eol(w, xctx);

                /* Check for \n. */
                line = 1;
                new_line = true;

                /* Loop writing source. */
                for (j = 0, offset = 0;
                    j < info.contextual->compiled_value->full_source->len;
                    j++)
                {
                    if (info.contextual->compiled_value->full_source->s[j]
                        == '\t')
                    {
                        afw_writer_write_z(w, "    ", xctx);
                        offset += 4;
                    }
                    else {
                        if (new_line) {
                            new_line = false;
                            snprintf(buf2, sizeof(buf2),
                                "%*" AFW_SIZE_T_FMT "  "
                                "%*" AFW_SIZE_T_FMT,
                                offset_cell_octets, offset,
                                line_cell_octets, line);
                            afw_writer_write_z(w, buf2, xctx);
                            if (caret_on_error && error_line == line) {
                                afw_writer_write_z(w, " > ", xctx);
                            }
                            else {
                                afw_writer_write_z(w, " | ", xctx);
                            }
                        }
                        if (info.contextual->compiled_value->full_source->s[j]
                            == '\n')
                        {
                            if (line != 0) {
                                line++;
                                new_line = true;
                            }
                        }
                        afw_writer_write(w,
                            info.contextual->compiled_value->full_source->s + j,
                            1, xctx);
                        offset++;
                    }
                }
                if (line == 0 || !new_line) {
                    afw_writer_write_eol(w, xctx);
                }

                afw_writer_write_eol(w, xctx);
                afw_writer_write_z(w, "---Backtrace", xctx);
                afw_writer_write_eol(w, xctx);
            }
        }

        /* Source location and source */
        if (info.contextual && info.contextual->compiled_value) {
            afw_utf8_line_column_of_offset(
                &line_number, &column_number,
                info.contextual->compiled_value->full_source,
                info.contextual->value_offset, 4, xctx);
            snprintf(buf2, AFW_SIZE_T_MAX_BUFFER,
                "%*" AFW_SIZE_T_FMT "  "
                "%*" AFW_SIZE_T_FMT ":" "%-*" AFW_SIZE_T_FMT,
                offset_cell_octets, info.contextual->value_offset,
                line_cell_octets, line_number,
                column_cell_octets, column_number);
            afw_writer_write_z(w, buf2, xctx);
            if (error_offset == info.contextual->value_offset) {
                afw_writer_write_z(w, " > ", xctx);
            }
            else {
                afw_writer_write_z(w, " | ", xctx);
            }
        }
        else {
            afw_writer_write_z(w, "? ", xctx);
        }

        /* Value Inf Id */
        afw_writer_write_utf8(w, info.value_inf_id, xctx);

        /* Detail */
        if (info.detail) {
            afw_writer_write_z(w, " ", xctx);
            afw_writer_write_utf8(w, info.detail, xctx);
        }

        if (parameter_number != 0) {
            afw_writer_write_z(w, " (evaluating parameter ", xctx);
            snprintf(buf, AFW_INTEGER_MAX_BUFFER, "%" AFW_SIZE_T_FMT,
                parameter_number);
            afw_writer_write_z(w, buf, xctx);
            afw_writer_write_z(w, ")", xctx);
            parameter_number = 0;
        }

        afw_writer_write_eol(w, xctx);
        last_contextual = info.contextual;
    }
    
    afw_utf8_writer_current_string(w, &s, xctx);
    result = afw_utf8_create_copy(s.s, s.len, p, xctx);
    afw_writer_release(w, xctx);
    return result;
}



AFW_DEFINE(const afw_utf8_t *)
afw_error_to_utf8(
    const afw_error_t *error,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *result;
    const afw_utf8_t *evaluation_backtrace;
    afw_boolean_t do_contextual;
    afw_boolean_t do_evaluation_backtrace;
    afw_boolean_t do_code_backtrace;

    /** @fixme "%.0" AFW_SIZE_T_FMT should cause 0 not to be printed, but it does. */
    do_contextual = afw_flag_is_active(
        xctx->env->flag_index_response_error_contextual, xctx);
    do_evaluation_backtrace = afw_flag_is_active(
        xctx->env->flag_index_response_error_backtraceEvaluation, xctx);
    do_code_backtrace = afw_flag_is_active(
        xctx->env->flag_index_response_error_backtrace, xctx);


    evaluation_backtrace = impl_evaluation_backtrace(error, p, xctx);

    result = afw_utf8_printf(p, xctx,
        "%s"                           /* message. */
        " [code=%s(%d)"                /* code-decoded */
        " rv=%s%s%d%s%s"               /* source:rv-decoded */

        "%s"                           /* recursive error */

        "%s%" AFW_UTF8_FMT "%s"        /* source location */
        "%.0" AFW_SIZE_T_FMT

        "]"

        "%s%" AFW_UTF8_FMT             /* evaluation backtrace */

        "%s%" AFW_UTF8_FMT,            /* code backtrace */

        /* message. */
        error->message_z,

        /* code-decoded */
        afw_error_code_id_z(error),
        error->code,

        /* rv-source-decoded */
        (error->rv_source_id_z) ? (char *)error->rv_source_id_z : "",
        (error->rv_source_id_z) ? ":" : "",
        error->rv,
        (error->rv_decoded_z) ? "-" : "",
        (error->rv_decoded_z) ? (char *)error->rv_decoded_z : "",

        /* recursive error */
        (!error->recursive_error)
        ? ""
        : ((error->recursive_error_in_finally)
            ? " recursive error in finally"
            : " recursive error in catch"),

        /* source location */
        (do_contextual && error->contextual && error->contextual->source_location)
            ? " source_location=" : "",
        (do_contextual && error->contextual && error->contextual->source_location)
            ? error->contextual->source_location->len : 0,
        (do_contextual && error->contextual && error->contextual->source_location)
            ? (char *)error->contextual->source_location->s : "",
        (do_contextual && error->contextual && error->contextual->value_offset != 0)
            ? " +" : "",
        (do_contextual && error->contextual && error->contextual->value_offset != 0)
            ? error->contextual->value_offset
            : 0,

        /* evaluation backtrace */
        (do_evaluation_backtrace && evaluation_backtrace)
            ? "\n\nEvaluation backtrace:\n" : "",
        (do_evaluation_backtrace && evaluation_backtrace)
            ? evaluation_backtrace->len : 0,
        (do_evaluation_backtrace && evaluation_backtrace)
            ? (char *)evaluation_backtrace->s : "",

        /* code backtrace */
        (do_code_backtrace && error->backtrace) ? "\nCode backtrace:\n" : "",
        (do_code_backtrace && error->backtrace) ? error->backtrace->len : 0,
        (do_code_backtrace && error->backtrace) ? (char *)error->backtrace->s : ""
    );

    return result;
}



AFW_DEFINE(void)
afw_error_write_log(afw_log_priority_t priority, 
    const afw_error_t *error, afw_xctx_t *xctx)
{
    const afw_utf8_t *s;

    if (error->contextual && error->contextual->source_location)
    {
        afw_log_write_fz(xctx->env->log,
            priority,
            afw_error_source_file(error),
            xctx,
            "%s [%" AFW_UTF8_FMT "%s%0d]",
            error->message_z,
            AFW_UTF8_FMT_ARG(error->contextual->source_location),
            (error->contextual && error->contextual->value_offset != 0) ? " +" : "",
            (error->contextual && error->contextual->value_offset != 0)
                ? error->contextual->value_offset
                : 0
            );
    }

    else {
        afw_log_write_fz(xctx->env->log,
            priority,
            afw_error_source_file(error),
            xctx,
            "%s",
            error->message_z);
    }

    if (true /** @fixme */) {
        s = afw_error_to_utf8(error, xctx->p, xctx);
        afw_log_write(xctx->env->log,
            afw_log_priority_debug, afw_error_source_file(error),
            s, xctx);
    }
}


AFW_DEFINE(int)
afw_error_print(FILE *fp, const afw_error_t *error)
{
    afw_integer_t line;
    const afw_utf8_octet_t *c;
    const afw_utf8_octet_t *end;
    const afw_utf8_octet_t *last_nl;
    afw_utf8_t value_source;
    int rv;

    rv = fprintf(fp, "error type:  %s\n", afw_error_code_id_z(error));
    if (rv < 0) goto return_rv;
    rv = fprintf(fp, "error type #: %d\n", error->code);
    if (rv < 0) goto return_rv;
    rv = fprintf(fp, "error source: %s\n", afw_error_source_file(error));
    if (rv < 0) goto return_rv;
    if (error->rv_source_id_z) {
        rv = fprintf(fp, "rv source:   %s\n", error->rv_source_id_z);
        if (rv < 0) goto return_rv;
    }
    if (error->rv) {
        rv = fprintf(fp, "rv:          %d\n", error->rv);
        if (rv < 0) goto return_rv;
    }
    if (error->rv_decoded_z) {
        rv = fprintf(fp, "rv decoded:  %s\n", error->rv_decoded_z);
        if (rv < 0) goto return_rv;
    }
    rv = fprintf(fp, "message:     %s\n", error->message_z);
    if (rv < 0) goto return_rv;

    /* recursive error */
    if (error->recursive_error) {
        if (error->recursive_error_in_finally) {
            rv = fprintf(fp, "recursive error: in finally\n");
        }
        else {
            rv = fprintf(fp, "recursive error: in catch\n");
        }
        if (rv < 0) goto return_rv;
    }

    if (error->contextual) {
        afw_value_contextual_resolve_value_source(&value_source,
            error->contextual);

        rv = fprintf(fp, "\ncontextual:\n");
        if (rv < 0) goto return_rv;
        if (error->contextual->source_location) {
            rv = fprintf(fp, "  source:  %" AFW_UTF8_FMT "\n",
                AFW_UTF8_FMT_ARG(error->contextual->source_location));
            if (rv < 0) goto return_rv;
        }

        rv = fprintf(fp, "  offset:  %" AFW_SIZE_T_FMT "\n",
            error->contextual->value_offset);
        if (rv < 0) goto return_rv;

        if (value_source.len != 0) {
            rv = fprintf(fp, "  around:  %" AFW_UTF8_FMT "\n",
                AFW_UTF8_FMT_ARG(&value_source));
            if (rv < 0) goto return_rv;

            c = value_source.s;
            end = c + (error->contextual->value_offset < value_source.len ?
                error->contextual->value_offset : value_source.len);
            last_nl = c;
            line = 0;
            for (; c < end; c++)
            {
                if (*c == '\n') {
                    line++;
                    last_nl = c;
                }
            }
            if (line != 1) {
                line++;
                rv = fprintf(fp, "  line:    %" AFW_INTEGER_FMT "\n", line);
                if (rv < 0) goto return_rv;
                rv = fprintf(fp, "  column:  %" AFW_INTEGER_FMT "\n",
                    (afw_integer_t)(end - last_nl));
                if (rv < 0) goto return_rv;
            }
        }
    }

    if (error->parser_cursor > 0) {
        rv = fprintf(fp, "  cursor:  %" AFW_INTEGER_FMT "\n", 
            (afw_integer_t)error->parser_cursor);
        if (rv < 0) goto return_rv;
    }

    if (error->backtrace) {
        rv = fprintf(fp, "\nbacktrace:\n%" AFW_UTF8_FMT "\n",
            AFW_UTF8_FMT_ARG(error->backtrace));
        if (rv < 0) goto return_rv;
    }

return_rv:

    return rv;
}


AFW_DEFINE(int)
afw_error_print_with_xctx(
    FILE *fp,
    const afw_error_t *error,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    int rv;
    const afw_utf8_t *backtraceExpression;

    rv = afw_error_print(fp, error);
    if (rv < 0) return rv;

    if (afw_flag_is_active(
        xctx->env->flag_index_response_error_backtraceEvaluation,
        xctx))
    {
        backtraceExpression = impl_evaluation_backtrace(error, p, xctx);
        if (backtraceExpression && backtraceExpression->len > 0) {
            rv = fprintf(fp, "evaluation backtrace:\n");
            if (rv < 0) return rv;
            rv = (int)fwrite(backtraceExpression->s, 1, backtraceExpression->len,
                fp);
        }
    }
    return rv;
}



static void
impl_add_contextual(
    const afw_object_t *object,
    const afw_compile_value_contextual_t *contextual,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_integer_t line;
    const afw_utf8_octet_t *c;
    const afw_utf8_octet_t *end;
    const afw_utf8_octet_t *last_nl;
    afw_utf8_t value_source;

    if (contextual->source_location) {
        afw_object_set_property_as_string(object,
            &afw_s_sourceLocation,
            contextual->source_location, xctx);
    }

    afw_value_contextual_resolve_value_source(&value_source, contextual);
    if (value_source.len > 0) {

        afw_object_set_property_as_integer(object,
            &afw_s_offset, (afw_integer_t)contextual->value_offset, xctx);
        line = 0;
        c = value_source.s;
        end = c + (contextual->value_offset < value_source.len ?
                contextual->value_offset : value_source.len);
        last_nl = c;
        for (;
            c < end;
            c++)
        {
            if (*c == '\n') {
                line++;
                last_nl = c;
            }
        }
        if (line != 1) {
            line++;
            afw_object_set_property_as_integer(object,
                &afw_s_line, line, xctx);
            afw_object_set_property_as_integer(object,
                &afw_s_column, (afw_integer_t)(end - last_nl), xctx);
        }
    }
}


/* Add error info to an existing object. */
AFW_DECLARE(void)
afw_error_add_to_object(
    const afw_object_t *object,
    const afw_error_t *error,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p = xctx->p;
    const afw_utf8_t *evaluation_backtrace;
    afw_size_t source_line;
    afw_size_t source_column;

    /* If there is contextual information, add it. */
    if (error->contextual) {
        impl_add_contextual(object, error->contextual,
            p, xctx);
    }

    /* Additional. */
    if (error->additional) {
        afw_object_set_property(object,
            &afw_s_additional,
            error->additional,
            xctx);
    }

    /* Contextual. */
    if (afw_flag_is_active(
        xctx->env->flag_index_response_error_contextual, xctx))
    {
        if (error->contextual) {
            impl_add_contextual(object, error->contextual,
                p, xctx);
        }
        afw_object_set_property_as_string(object,
            &afw_s_errorSource,
            afw_utf8_create(
                afw_error_source_file(error),
                AFW_UTF8_Z_LEN, p, xctx),
            xctx);
    }


    /* Backtrace. */
    if (error->backtrace &&
        afw_flag_is_active(
            xctx->env->flag_index_response_error_backtrace, xctx))
    {
        afw_object_set_property_as_string(object,
            &afw_s_backtrace, error->backtrace, xctx);
    }

    /* Evaluation backtrace. */
    if (afw_flag_is_active(
        xctx->env->flag_index_response_error_backtraceEvaluation, xctx))
    {
        evaluation_backtrace = impl_evaluation_backtrace(error, p, xctx);
        if (evaluation_backtrace) {
            afw_object_set_property_as_string(object,
                &afw_s_backtraceEvaluation, evaluation_backtrace, xctx);
        }
    }


    /* Add properties to object. */

    afw_object_set_property(object, &afw_s_error, afw_value_true, xctx);

    afw_object_set_property_as_integer(object,
        &afw_s_errorCode, error->code, xctx);


    /* If parser source available, set appropriate properties. */
    if (error->parser_source) {
        afw_object_set_property_as_integer(object,
            &afw_s_parserCursor,
            (afw_integer_t)error->parser_cursor,
            xctx);
        afw_object_set_property_as_string(object,
            &afw_s_parserSource, error->parser_source, xctx);
        afw_utf8_line_column_of_offset(
            &source_line, &source_column,
            error->parser_source, error->parser_cursor, 4, xctx);
        afw_object_set_property_as_integer(object,
            &afw_s_parserLineNumber, source_line, xctx);
        afw_object_set_property_as_integer(object,
            &afw_s_parserColumnNumber, source_column, xctx);
    }

    else if (error->parser_cursor > 0) {
        afw_object_set_property_as_integer(object,
            &afw_s_parserCursor,
            (afw_integer_t)error->parser_cursor,
            xctx);
    }

    afw_object_set_property_as_string(object,
        &afw_s_id,
        afw_utf8_create(afw_error_code_id_z(error),
            AFW_UTF8_Z_LEN, p, xctx),
        xctx);

    if (error->rv_source_id_z) {
        afw_object_set_property_as_string(object,
            &afw_s_rvSourceId,
            afw_utf8_create_copy(error->rv_source_id_z,
                AFW_UTF8_Z_LEN, p, xctx),
            xctx);
    }

    if (error->rv) {
        afw_object_set_property_as_integer(object,
            &afw_s_rv, error->rv, xctx);
    }

    if (error->rv_decoded_z) {
        afw_object_set_property_as_string(object,
            &afw_s_rvDecoded,
            afw_utf8_create_copy(error->rv_decoded_z,
                AFW_UTF8_Z_LEN, p, xctx),
            xctx);
    }

    afw_object_set_property_as_string(object,
        &afw_s_message,
        afw_utf8_create_copy(error->message_z,
            AFW_UTF8_Z_LEN, p, xctx),
        xctx);

    if (error->recursive_error) {
        afw_object_set_property_as_string(object,
            &afw_s_recursiveError,
            (error->recursive_error_in_finally)
            ? &afw_s_a_in_finally
            : &afw_s_a_in_catch,
            xctx);
    }


    afw_object_set_property_as_string(object,
        &afw_s_xctxUUID, xctx->uuid, xctx);
}


/* Create an object with error info in specified pool. */
AFW_DECLARE(const afw_object_t *)
afw_error_to_object(
    const afw_error_t *error,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_object_t *result;

    result = afw_object_create_managed(p, xctx);
    afw_error_add_to_object(result, error, xctx);

    return result;
}


AFW_DEFINE(const afw_utf8_z_t *)
afw_error_message_vz(
    const afw_utf8_z_t *format, va_list ap, afw_xctx_t *xctx)
{
    vsnprintf((char *)&(xctx->error->message_wa[0]),
        sizeof(xctx->error->message_wa), (const char *)format, ap);

    return &(xctx->error->message_wa[0]);
}


AFW_DEFINE_ELLIPSIS(const afw_utf8_z_t *)
afw_error_message(
    afw_xctx_t *xctx, const afw_utf8_z_t *format, ...)
{
    va_list ap;

    va_start(ap, format);

    vsnprintf((char *)&(xctx->error->message_wa[0]),
        sizeof(xctx->error->message_wa), (const char *)format, ap);

    va_end(ap);

    return &(xctx->error->message_wa[0]);
}


/* Returns http status for error. */
AFW_DEFINE(const afw_utf8_t *)
afw_error_http_status(const afw_error_t *error)
{
    const afw_utf8_t *result;

    result = NULL;

    if (error->code < sizeof(impl_error_code_map)
            / sizeof(impl_error_code_map_t) &&
        error->code >= 0)
    {
        result = &impl_error_code_map[error->code].html_description;
    }

    if (!result) result = &impl_s_a_html_unknown;

    return result;
}


/* Returns error->code description. */
AFW_DEFINE(const afw_utf8_z_t *)
afw_error_code_id_z(const afw_error_t *error)
{
    const afw_utf8_z_t *result;

    result = NULL;

    if (error->code < sizeof(impl_error_code_map)
            / sizeof(impl_error_code_map_t) &&
        error->code >= 0)
    {
        result = impl_error_code_map[error->code].id.s;
    }

    if (!result) result = "unknown";

    return result;
}


AFW_DEFINE(afw_boolean_t)
afw_error_allow_in_response(afw_error_code_t code) {
    if (code < sizeof(impl_error_code_map) / sizeof(impl_error_code_map_t) &&
        code >= 0)
    {
        return impl_error_code_map[code].error_allow_in_response;
    }
    else {
        return false;
    }
}
