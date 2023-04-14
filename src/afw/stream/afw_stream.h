// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Stream Implementation Header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_STREAM_H__
#define __AFW_STREAM_H__


/** @addtogroup afw_c_api
 * @{
 */

/** @addtogroup afw_stream Stream
 *
 * Adaptive Framework Stream.
 *
 * @{
 */

/**
 * @file afw_stream.h
 * @brief Adaptive Framework Stream Implementation Header.
 */

#include "afw_interface.h"

AFW_BEGIN_DECLARES


/** @brief enum for common xctx streams */
typedef enum {
    afw_stream_number_raw_response_body,
    afw_stream_number_response_body,
    afw_stream_number_console,
    afw_stream_number_stderr,
    afw_stream_number_stdout,
    afw_stream_number_count  /* Count is also max enum value + 1 */
} afw_stream_number_t;


/** @brief struct for common xctx streams */
struct afw_stream_anchor_s {
    afw_size_t maximum_number_of_streams;
    const afw_stream_t * const *streams;
    const afw_utf8_t *last_stream_error;
};



/**
 * @brief Get stream by streamId and streamNumber.
 * @param streamId.
 * @param stream return pointer.
 * @param streamNumber return pointer.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_stream_get_by_streamId(
    const afw_utf8_t *streamId,
    const afw_stream_t **stream,
    afw_size_t *streamNumber,
    afw_xctx_t *xctx);



/**
 * @brief Get stream by streamNumber.
 * @param streamNumber.
 * @param xctx of caller.
 * @return Stream or NULL if not found.
 */
AFW_DECLARE(const afw_stream_t *)
afw_stream_get_by_streamNumber(
    const afw_integer_t streamNumber,
    afw_xctx_t *xctx);



/**
 * @brief Get streamNumber for streamId.
 * @param streamId.
 * @param xctx of caller.
 * @return streamNumber or -1 if not found.
 */
AFW_DECLARE(afw_size_t)
afw_stream_get_streamNumber_for_streamId(
    const afw_utf8_t *streamId,
    afw_xctx_t *xctx);



/**
 * @brief Set an opening stream and get its streamNumber.
 * @param stream.
 * @param xctx of caller.
 * @return streamNumber or -1 if failed.
 */
AFW_DECLARE(afw_size_t)
afw_stream_set(
    const afw_stream_t *stream,
    afw_xctx_t *xctx);



/**
 * @brief Implementation for get xctx standard stream instance.
 * @param n of streams indexed by afw_stream_number_t.
 * @param xctx of caller.
 * 
 * @see afw_stream_standard()
 */
AFW_DECLARE(const afw_stream_t *)
afw_stream_standard_impl(afw_stream_number_t n, afw_xctx_t *xctx);



/**
 * @brief Get xctx stream instance.
 * @param enum_suffix is appended to afw_stream_number_
 * @param xctx of caller.
 */
#define afw_stream_standard(enum_suffix, xctx) \
(((xctx)->stream_anchor->streams[afw_stream_number_ ## enum_suffix]) \
? (xctx)->stream_anchor->streams[afw_stream_number_ ## enum_suffix] \
: afw_stream_standard_impl(afw_stream_number_ ## enum_suffix, xctx) )



/**
 * @brief Determine if an xctx's standard stream is set.
 * @param enum_suffix is appended to afw_stream_number_
 * @param xctx of caller.
 */
#define afw_stream_standard_is_set(enum_suffix, xctx) \
((xctx)->stream_anchor->streams[afw_stream_number_ ## enum_suffix] != NULL )



/**
 * @brief Set xctx's enum_suffix stream.
 * @param enum_suffix is appended to afw_stream_number_
 * @param stream
 * @param xctx of caller.
 * @return stream instance.
 */
#define afw_stream_standard_set(enum_suffix, stream, xctx) \
if ((xctx)->stream_anchor->streams[afw_stream_number_ ## enum_suffix]) { \
    AFW_THROW_ERROR(general, "Stream already set", xctx); \
} \
(xctx)->stream_anchor->streams[afw_stream_number_ ## enum_suffix] = stream

 
 
/**
 * @brief Release an xctx's stream.
 * @param enum_suffix is appended to afw_stream_number_
 * @param xctx of caller.
 *
 * Note: afw_stream_console_stream() will get a new one if called.
 */
#define afw_stream_standard_release(enum_suffix, xctx) \
if ((xctx)->stream_anchor->streams[afw_stream_number_ ## enum_suffix]) { \
    afw_stream_release( \
        (xctx)->stream_anchor->streams[afw_stream_number_ ## enum_suffix], \
        xctx); \
    *(afw_stream_t **)&(xctx)->stream_anchor-> \
        streams[afw_stream_number_ ## enum_suffix] = NULL; \
}



/**
 * @brief Call afw_stream_write() with zero terminated string.
 * @param writer instance.
 * @param s_z zero terminated string to write.
 * @param xctx of caller.
 */
#define afw_stream_write_z(writer, s_z, xctx) \
afw_stream_write(writer, s_z, strlen(s_z), xctx)



/**
 * @brief Write eol char to stream.
 * @param writer instance.
 * @param xctx of caller.
 */
#define afw_stream_write_eol(writer, xctx) \
afw_stream_write(writer, "\n", strlen("\n"), xctx)



/**
 * @brief Call afw_stream_write() with a afw_utf8_t string.
 * @param writer instance.
 * @param s utf8 string.
 * @param xctx of caller.
 */
#define afw_stream_write_utf8(writer, S, xctx) \
afw_stream_write(writer, (S)->s, (S)->len, xctx)



/**
 * @brief Call afw_stream_write() with an integer.
 * @param writer instance.
 * @param integer to write as as string.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_stream_write_integer(
    const afw_stream_t *writer,
    afw_integer_t integer,
    afw_xctx_t *xctx);



/**
 * @brief Call afw_stream_write() with an size.
 * @param writer instance.
 * @param size to write as string.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_stream_write_size(
    const afw_stream_t *writer,
    afw_size_t size,
    afw_xctx_t *xctx);



/**
 * @brief Create a stream to a file descriptor.
 * @param fd opened file descriptor.
 * @param streamId
 * @param allow_read
 * @param allow_write
 * @param auto_flush after each write.
 * @param close_on_release true closes fd on release call.
 * @param p to use.
 * @param xctx of caller.
 */
AFW_DECLARE(const afw_stream_t *)
afw_stream_fd_create(
    FILE *fd,
    const afw_utf8_t *streamId,
    afw_boolean_t allow_read,
    afw_boolean_t allow_write,
    afw_boolean_t auto_flush,
    afw_boolean_t close_on_release,
    const afw_pool_t * p,
    afw_xctx_t *xctx);



/**
 * @brief Open a file and create a stream for it.
 * @param streamId
 * @param path
 * @param mode
 * @param auto_flush
 * @param p to use.
 * @param xctx of caller.
 * @return stream
 */
AFW_DECLARE(const afw_stream_t *)
afw_stream_fd_open_and_create(
    const afw_utf8_t *streamId,
    const afw_utf8_t *path,
    const afw_utf8_t *mode,
    afw_boolean_t auto_flush,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @internal
 * @brief Called internal to xctx to create xctx->stream_anchor.
 * @param xctx of caller.
 */
AFW_DEFINE_INTERNAL(const afw_stream_anchor_t *)
afw_stream_internal_stream_anchor_create(afw_xctx_t *xctx);



/**
 * @internal
 * @brief Release all xctx's streams
 * @param xctx of caller.
 */
AFW_DEFINE_INTERNAL(void)
afw_stream_internal_release_all_streams(afw_xctx_t *xctx);


AFW_END_DECLARES

/** @} */
/** @} */

#endif /* __AFW_STREAM_H__ */
