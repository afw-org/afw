// See the 'COPYING' file in the project root for licensing information.
/*
 * Implementation of afw_xctx interface
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_stream.c
 * @brief Implementation of afw_stream interface.
 */

#include "afw_internal.h"


/* Get stream by streamId. */
AFW_DEFINE(void)
afw_stream_get_by_streamId(
    const afw_utf8_t *streamId,
    const afw_stream_t **stream,
    afw_size_t *streamNumber,
    afw_xctx_t *xctx)
{
    const afw_stream_t * const *streams;
    afw_size_t number;

    *stream = NULL;
    *streamNumber = (afw_size_t)-1;
    if (xctx->stream_anchor) {
        if (afw_utf8_equal(streamId, afw_s_stdout)) {
            *stream = afw_stream_standard(stdout, xctx);
            *streamNumber = (afw_size_t)afw_stream_number_stdout;
        }
        else if (afw_utf8_equal(streamId, afw_s_stderr)) {
            *stream = afw_stream_standard(stderr, xctx);
            *streamNumber = (afw_size_t)afw_stream_number_stderr;
        }
        else if (afw_utf8_equal(streamId, afw_s_console)) {
            *stream = afw_stream_standard(console, xctx);
            *streamNumber = (afw_size_t)afw_stream_number_console;
        }
        else if (afw_utf8_equal(streamId, afw_s_response_body)) {
            *stream = afw_stream_standard(response_body, xctx);
            *streamNumber = (afw_size_t)afw_stream_number_response_body;
        }
        else if (afw_utf8_equal(streamId, afw_s_raw_response_body)) {
            *stream = afw_stream_standard(raw_response_body, xctx);
            *streamNumber = (afw_size_t)afw_stream_number_raw_response_body;
        }
        else {
            /* Custom slots only; index and table entry must stay in sync. */
            for (number = (afw_size_t)afw_stream_number_count;
                number < xctx->stream_anchor->maximum_number_of_streams;
                number++)
            {
                streams = &xctx->stream_anchor->streams[number];
                if (*streams &&
                    afw_utf8_equal((*streams)->streamId, streamId))
                {
                    *stream = *streams;
                    *streamNumber = number;
                    break;
                }
            }
        }
    }
}



/*  Get stream by streamNumber. */
AFW_DEFINE(const afw_stream_t *)
afw_stream_get_by_streamNumber(
    const afw_integer_t streamNumber,
    afw_xctx_t *xctx)
{
    const afw_stream_t *stream;
    afw_size_t number;

    if (streamNumber >= 0 && streamNumber < afw_stream_number_count) {
        stream = afw_stream_standard_impl(
            (afw_stream_number_t)streamNumber, xctx);
        return stream;
    }

    number = (afw_size_t)streamNumber;
    stream = NULL;
    if (xctx->stream_anchor &&
        streamNumber >= 0 &&
        number == streamNumber &&
        number < xctx->stream_anchor->maximum_number_of_streams)
    {
        stream = xctx->stream_anchor->streams[number];
    }
    return stream;
}



/* Get streamNumber for streamId. */
AFW_DEFINE(afw_size_t)
afw_stream_get_streamNumber_for_streamId(
    const afw_utf8_t *streamId,
    afw_xctx_t *xctx)
{
    const afw_stream_t *stream;
    afw_size_t streamNumber;

    afw_stream_get_by_streamId(streamId,
        &stream, &streamNumber, xctx);

    return streamNumber;
}



/* Set an opening stream and get its streamNumber. */
AFW_DEFINE(afw_size_t)
afw_stream_set(
    const afw_stream_t *stream,
    afw_xctx_t *xctx)
{
    const afw_stream_t **table;
    afw_size_t count, number;

    number = (afw_size_t)-1;
    if (xctx->stream_anchor) {
        table = (const afw_stream_t **)xctx->stream_anchor->streams;

        /*
         * Reject duplicate streamId in any slot. Allocate only from custom
         * slots (index >= afw_stream_number_count) so standards stay reserved.
         */
        for (count = 0;
            count < xctx->stream_anchor->maximum_number_of_streams;
            count++)
        {
            if (table[count] &&
                afw_utf8_equal(table[count]->streamId, stream->streamId))
            {
                return (afw_size_t)-1;
            }
        }
        for (count = (afw_size_t)afw_stream_number_count;
            count < xctx->stream_anchor->maximum_number_of_streams;
            count++)
        {
            if (!table[count]) {
                number = count;
                break;
            }
        }
        if (number != (afw_size_t)-1) {
            table[number] = stream;
        }
    }

    return number;
}



/* Clear a stream table slot after release (custom streams). */
AFW_DEFINE(void)
afw_stream_clear_slot(
    afw_size_t streamNumber,
    afw_xctx_t *xctx)
{
    if (xctx->stream_anchor &&
        streamNumber < xctx->stream_anchor->maximum_number_of_streams)
    {
        *(const afw_stream_t **)&xctx->stream_anchor->streams[streamNumber] =
            NULL;
    }
}




/* Implementation for get xctx writer instance. */
AFW_DEFINE(const afw_stream_t *)
afw_stream_standard_impl(afw_stream_number_t n, afw_xctx_t *xctx)
{
    const afw_content_type_t *response_content_type;
    afw_stream_anchor_t *stream_anchor =
        (afw_stream_anchor_t *)xctx->stream_anchor;
    const afw_stream_t **stream;

    if (n < 0 || n >= afw_stream_number_count) {
        AFW_THROW_ERROR_Z(general,
            "afw_stream_standard_impl() n parameter out of range",
            xctx);
    }

    response_content_type = (xctx->request)
        ? afw_request_prepare_response_content_type(xctx->request, xctx)
        : NULL;

    if (!stream_anchor->streams[n]) {
        stream = (const afw_stream_t **)&stream_anchor->streams[n];
        switch (n) {

        case afw_stream_number_raw_response_body:
            if (xctx->request) {
                *stream = afw_request_response_body_raw_writer_create(
                    xctx->request, xctx);
            }
            else {
                *stream = afw_stream_fd_create(xctx->env->stdout_fd,
                    afw_s_raw_response_body, false, true, true, false,
                    xctx->p, xctx);
            }
            break;

        case afw_stream_number_response_body:
            if (afw_content_type_is_application_afw(response_content_type))
            {
                *stream = afw_content_type_application_afw_stream_create(
                    afw_stream_standard(raw_response_body, xctx),
                    afw_s_response, NULL, xctx->p, xctx);
            }
            else {
                *stream = afw_stream_standard(raw_response_body, xctx);
            }
            break;

        case afw_stream_number_console:
            if (afw_flag_by_id_is_active(
                afw_s_a_flag_response_console_stream, xctx))
            {
                if (afw_content_type_is_application_afw(response_content_type))
                {
                    *stream = afw_content_type_application_afw_stream_create(
                        afw_stream_standard(raw_response_body, xctx),
                        afw_s_console, NULL, xctx->p, xctx);
                }
                else {
                    AFW_THROW_ERROR_Z(general,
                        "flag response:console:stream requires content-type "
                        "application/x-afw",
                        xctx);
                }
            }
            else if (afw_flag_by_id_is_active(
                afw_s_a_flag_response_console, xctx))
            {
                *stream = afw_utf8_stream_create(afw_s_console,
                    xctx->p, xctx);
            }
            else {
                *stream = afw_stream_fd_create(xctx->env->stdout_fd,
                    afw_s_console, false, true, true, false, xctx->p, xctx);
            }
            break;

        case afw_stream_number_stderr:
            if (afw_flag_by_id_is_active(
                afw_s_a_flag_response_stderr_stream, xctx))
            {
                if (afw_content_type_is_application_afw(response_content_type))
                {
                    *stream = afw_content_type_application_afw_stream_create(
                        afw_stream_standard(raw_response_body, xctx),
                        afw_s_stderr, NULL, xctx->p, xctx);
                }
                else {
                    AFW_THROW_ERROR_Z(general,
                        "flag response:stderr:stream requires content-type "
                        "application/x-afw",
                        xctx);
                }
            }
            else if (afw_flag_by_id_is_active(
                afw_s_a_flag_response_stderr, xctx))
            {
                *stream = afw_utf8_stream_create(afw_s_stderr,
                    xctx->p, xctx);
            }
            else {
                *stream = afw_stream_fd_create(xctx->env->stdout_fd,
                    afw_s_stderr, false, true, true, false, xctx->p, xctx);
            }
            break;

        case afw_stream_number_stdout:
            if (afw_flag_by_id_is_active(
                afw_s_a_flag_response_stdout_stream, xctx))
            {
                if (afw_content_type_is_application_afw(response_content_type))
                {
                    *stream = afw_content_type_application_afw_stream_create(
                        afw_stream_standard(raw_response_body, xctx),
                        afw_s_stdout, NULL, xctx->p, xctx);
                }
                else {
                    AFW_THROW_ERROR_Z(general,
                        "flag response:stdout:stream requires content-type "
                        "application/x-afw",
                        xctx);
                }
            }
            else if (afw_flag_by_id_is_active(
                afw_s_a_flag_response_stdout, xctx))
            {
                *stream = afw_utf8_stream_create(afw_s_stdout,
                    xctx->p, xctx);
            }
            else {
                *stream = afw_stream_fd_create(xctx->env->stdout_fd,
                    afw_s_stdout, false, true, true, false, xctx->p, xctx);
            }
            break;

        default:
            AFW_THROW_ERROR_Z(general, "Internal error", xctx);
        }
    }

    return stream_anchor->streams[n];
}


/**
 * @internal
 * @brief Called internal to xctx to create xctx->stream_anchor.
 * @param xctx of caller.
 */
AFW_DEFINE_INTERNAL(const afw_stream_anchor_t *)
afw_stream_internal_stream_anchor_create(afw_xctx_t *xctx)
{
    afw_stream_anchor_t *stream_anchor;

    stream_anchor = apr_pcalloc(afw_pool_get_apr_pool(xctx->p),
        sizeof(afw_stream_anchor_t));
    stream_anchor->maximum_number_of_streams = afw_stream_number_count + 20;
    stream_anchor->streams = apr_pcalloc(afw_pool_get_apr_pool(xctx->p),
        stream_anchor->maximum_number_of_streams * sizeof(afw_stream_t *));

    return stream_anchor;
}


/* Release xctx streams. */
AFW_DEFINE_INTERNAL(void)
afw_stream_internal_release_all_streams(afw_xctx_t *xctx)
{
    afw_size_t i;
    afw_size_t max;

    if (!xctx->stream_anchor) {
        return;
    }

    max = xctx->stream_anchor->maximum_number_of_streams;

    /* Release all streams except raw_response_body first. */
    for (i = 0; i < max; i++) {
        if (xctx->stream_anchor->streams[i] &&
            i != (afw_size_t)afw_stream_number_raw_response_body)
        {
            afw_stream_release(xctx->stream_anchor->streams[i], xctx);
            *(afw_stream_t **)&xctx->stream_anchor->streams[i] = NULL;
        }
    }

    /* Release raw_response_body last. */
    if (xctx->stream_anchor->streams[afw_stream_number_raw_response_body]) {
        afw_stream_release(
            xctx->stream_anchor->streams[afw_stream_number_raw_response_body],
            xctx);
        *(afw_stream_t **)
            &xctx->stream_anchor->streams[afw_stream_number_raw_response_body]
            = NULL;
    }
}



/* Call afw_stream_write() with an integer. */
AFW_DEFINE(void)
afw_stream_write_integer(
    const afw_stream_t *writer,
    afw_integer_t integer,
    afw_xctx_t *xctx)
{
    char buf[AFW_INTEGER_MAX_BUFFER];

    snprintf(buf, AFW_INTEGER_MAX_BUFFER, AFW_INTEGER_FMT,
        integer);
    afw_stream_write_z(writer, buf, xctx);
}



/* Call afw_stream_write() with a size. */
AFW_DEFINE(void)
afw_stream_write_size(
    const afw_stream_t *writer,
    afw_size_t size,
    afw_xctx_t *xctx)
{
    char buf[AFW_SIZE_T_MAX_BUFFER];

    snprintf(buf, AFW_SIZE_T_MAX_BUFFER, AFW_SIZE_T_FMT,
        size);
    afw_stream_write_z(writer, buf, xctx);
}
