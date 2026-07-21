// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for stream
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_stream.c
 * @brief Adaptive function execute implementations for category `stream`.
 */

#include "afw.h"



/* Require stream by number or throw. */
static const afw_stream_t *
impl_require_stream(
    afw_integer_t streamNumber,
    afw_function_execute_t *x)
{
    const afw_stream_t *stream;

    stream = afw_stream_get_by_streamNumber(streamNumber, x->xctx);
    if (!stream) {
        AFW_THROW_ERROR_FZ(general, x->xctx,
            "Invalid streamNumber " AFW_INTEGER_FMT, streamNumber);
    }
    return stream;
}



/*
 * Adaptive function: flush
 *
 * afw_function_execute_flush
 *
 * See afw_function_bindings.h for more information.
 *
 * Write the content of the stream's buffers to its destination.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function flush(
 *       streamNumber: integer
 *   ): void;
 * ```
 *
 * Parameters:
 *
 *   streamNumber - (integer) The streamNumber for the stream to flush.
 *
 * Returns:
 *
 *   (void)
 */
const afw_value_t *
afw_function_execute_flush(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *streamNumber;
    const afw_stream_t *stream;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(streamNumber,
        1, integer);

    stream = impl_require_stream(streamNumber->internal, x);
    afw_stream_flush(stream, x->xctx);

    return afw_value_undefined;
}



/*
 * Adaptive function: print
 *
 * afw_function_execute_print
 *
 * See afw_function_bindings.h for more information.
 *
 * Evaluate and convert 0 or more values to its string value, then write them to
 * stdout. An undefined value is represented by 'undefined'.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function print(
 *       ...values: (array of any)
 *   ): void;
 * ```
 *
 * Parameters:
 *
 *   values - (0 or more any dataType) Values to print.
 *
 * Returns:
 *
 *   (void)
 */
const afw_value_t *
afw_function_execute_print(
    afw_function_execute_t *x)
{
    const afw_value_t *value;
    const afw_stream_t *stream;
    const afw_utf8_t *s;
    afw_size_t i;

    stream = afw_stream_standard(stdout, x->xctx);
    for (i = 1; i <= x->argc; i++) {
        AFW_FUNCTION_EVALUATE_PARAMETER(value, i);
        if (value) {
            value = afw_value_convert_to_string(value, true, x->p, x->xctx);
            s = AFW_VALUE_INTERNAL(value);
            afw_stream_write(stream, s->s, s->len, x->xctx);
        }
        else {
            afw_stream_write_utf8(stream, afw_s_undefined, x->xctx);
        }
    }
    afw_stream_flush(stream, x->xctx);

    return afw_value_undefined;
}



/*
 * Adaptive function: println
 *
 * afw_function_execute_println
 *
 * See afw_function_bindings.h for more information.
 *
 * Evaluate and convert 0 or more values to their string value, then write them
 * to stdout. A newline character ('\n') is written after the last value. An
 * undefined value is represented by 'undefined'.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function println(
 *       ...value: (array of any)
 *   ): void;
 * ```
 *
 * Parameters:
 *
 *   value - (0 or more any dataType) Values to print.
 *
 * Returns:
 *
 *   (void)
 */
const afw_value_t *
afw_function_execute_println(
    afw_function_execute_t *x)
{
    const afw_stream_t *stream;
    const afw_value_t *value;
    const afw_utf8_t *s;
    afw_size_t i;

    stream = afw_stream_standard(stdout, x->xctx);
    for (i = 1; i <= x->argc; i++) {
        AFW_FUNCTION_EVALUATE_PARAMETER(value, i);
        if (value) {
            value = afw_value_convert_to_string(value, true, x->p, x->xctx);
            s = AFW_VALUE_INTERNAL(value);
            afw_stream_write(stream, s->s, s->len, x->xctx);
        }
        else {
            afw_stream_write_utf8(stream, afw_s_undefined, x->xctx);
        }
    }
    afw_stream_write_eol(stream, x->xctx);
    afw_stream_flush(stream, x->xctx);

    return afw_value_undefined;
}



/*
 * Adaptive function: write
 *
 * afw_function_execute_write
 *
 * See afw_function_bindings.h for more information.
 *
 * Evaluate and convert 0 or more values to its string value, then write them to
 * stream. An value with an undefined value is represented by 'undefined'.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function write(
 *       streamNumber: integer,
 *       ...value: (array of any)
 *   ): void;
 * ```
 *
 * Parameters:
 *
 *   streamNumber - (integer) The streamNumber for the stream to write.
 *
 *   value - (0 or more any dataType) Values to write as their string value.
 *
 * Returns:
 *
 *   (void)
 */
const afw_value_t *
afw_function_execute_write(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *streamNumber;
    const afw_stream_t *stream;
    const afw_value_t *value;
    const afw_utf8_t *s;
    afw_size_t i;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(streamNumber,
        1, integer);

    stream = impl_require_stream(streamNumber->internal, x);

    for (i = 2; i <= x->argc; i++) {
        AFW_FUNCTION_EVALUATE_PARAMETER(value, i);
        if (value) {
            value = afw_value_convert_to_string(value, true, x->p, x->xctx);
            s = AFW_VALUE_INTERNAL(value);
            afw_stream_write(stream, s->s, s->len, x->xctx);
        }
        else {
            afw_stream_write_utf8(stream, afw_s_undefined, x->xctx);
        }
    }

    return afw_value_undefined;
}



/*
 * Adaptive function: writeln
 *
 * afw_function_execute_writeln
 *
 * See afw_function_bindings.h for more information.
 *
 * Evaluate and convert 0 or more values to its string value, then write them to
 * stream. A newline character ('\n') is written after the last value. An
 * undefined value is represented by 'undefined'.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function writeln(
 *       streamNumber: integer,
 *       ...value: (array of any)
 *   ): void;
 * ```
 *
 * Parameters:
 *
 *   streamNumber - (integer) The streamNumber for the stream to write.
 *
 *   value - (0 or more any dataType) Values to write.
 *
 * Returns:
 *
 *   (void)
 */
const afw_value_t *
afw_function_execute_writeln(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *streamNumber;
    const afw_stream_t *stream;
    const afw_value_t *value;
    const afw_utf8_t *s;
    afw_size_t i;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(streamNumber,
        1, integer);

    stream = impl_require_stream(streamNumber->internal, x);

    for (i = 2; i <= x->argc; i++) {
        AFW_FUNCTION_EVALUATE_PARAMETER(value, i);
        if (value) {
            value = afw_value_convert_to_string(value, true, x->p, x->xctx);
            s = AFW_VALUE_INTERNAL(value);
            afw_stream_write(stream, s->s, s->len, x->xctx);
        }
        else {
            afw_stream_write_utf8(stream, afw_s_undefined, x->xctx);
        }
    }
    afw_stream_write_eol(stream, x->xctx);

    return afw_value_undefined;
}



/*
 * Adaptive function: close
 *
 * afw_function_execute_close
 *
 * See afw_function_bindings.h for more information.
 *
 * This will close an open stream
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function close(
 *       streamNumber: integer
 *   ): void;
 * ```
 *
 * Parameters:
 *
 *   streamNumber - (integer) The streamNumber for the stream to close.
 *
 * Returns:
 *
 *   (void)
 */
const afw_value_t *
afw_function_execute_close(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *streamNumber;
    const afw_stream_t *stream;
    afw_size_t number;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(streamNumber,
        1, integer);

    stream = impl_require_stream(streamNumber->internal, x);
    number = (afw_size_t)streamNumber->internal;

    /* Do not allow close of standard stream slots via this API. */
    if (number < (afw_size_t)afw_stream_number_count) {
        AFW_THROW_ERROR_Z(general,
            "Cannot close a standard stream with close()", x->xctx);
    }

    afw_stream_release(stream, x->xctx);
    afw_stream_clear_slot(number, x->xctx);

    return afw_value_undefined;
}



/*
 * Adaptive function: open_file
 *
 * afw_function_execute_open_file
 *
 * See afw_function_bindings.h for more information.
 *
 * Open a file stream for reading and/or writing. The path is resolved using
 * application rootFilePaths (longest matching prefix; host path must remain
 * under that root). See /afw/_AdaptiveObjectType_/_AdaptiveRootFilePaths_.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * This function requires 'execute' access.
 *
 * Declaration:
 *
 * ```
 *   function open_file(
 *       streamId: string,
 *       path: string,
 *       mode: string,
 *       autoFlush?: boolean
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   streamId - (string) This is the streamId that will be associated with this
 *       open file stream.
 *
 *   path - (string) Logical path resolved using rootFilePaths (longest matching
 *       prefix; host path must remain under that root).
 *
 *   mode - (string) This is the access mode string. Values can be:
 *         r - Open an existing file text file for read.
 *         w - Open a text file for writing. If the file does not exist, it will
 *       be created.
 *         a - Open a text file for writing additional data to the end. If the
 *       file does not exist, it will be created.
 *         r+ - Open a text file for both reading and writing.
 *         w+ - Open a text file for both reading and writing. If the file
 *       exists, it will be overwritten. If the file does not exist, it will be
 *       created.
 *         a+ - Open a text file for both reading and writing. Reading will
 *       begin at the start of the file while writing will be appended to the
 *       end.
 * 
 *       All of these modes expect data type string. If you are using data type
 *       base64Binary or hexBinary you can use corresponding binary modes, 'rb',
 *       'wb', 'ab', 'rb+', 'r+b', 'wb+', 'w+b', 'ab+', and 'a+b'.
 *
 *   autoFlush - (optional boolean) If specified and true, this will
 *       automatically flush the stream's buffers after every write.
 *
 * Returns:
 *
 *   (integer) The streamNumber for the streamId. Throws on error (invalid path,
 *       open failure, or streamId already open).
 */
const afw_value_t *
afw_function_execute_open_file(
    afw_function_execute_t *x)
{
    const afw_value_string_t *streamId;
    const afw_value_string_t *path;
    const afw_value_string_t *mode;
    const afw_value_boolean_t *autoFlush;
    const afw_stream_t *stream;
    const afw_utf8_t *resolved_path;
    afw_size_t number;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(streamId,
        1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(path,
        2, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(mode,
        3, string);
    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(autoFlush,
        4, boolean);

    number = afw_stream_get_streamNumber_for_streamId(
        &streamId->internal, x->xctx);
    if (number != (afw_size_t)-1) {
        AFW_THROW_ERROR_FZ(general, x->xctx,
            "streamId " AFW_UTF8_FMT_Q " is already open",
            AFW_UTF8_FMT_ARG(&streamId->internal));
    }

    resolved_path = afw_file_path_resolve_rootFilePaths(
        &path->internal, x->p, x->xctx);

    /*
     * Copy streamId into the xctx pool so lookup by stream() remains valid
     * for the life of the stream table (not just the open_file call pool).
     */
    stream = afw_stream_fd_open_and_create(
        afw_utf8_create_copy(
            streamId->internal.s, streamId->internal.len,
            x->xctx->p, x->xctx),
        resolved_path, &mode->internal,
        autoFlush && autoFlush->internal, x->p, x->xctx);

    number = afw_stream_set(stream, x->xctx);
    if (number == (afw_size_t)-1) {
        afw_stream_release(stream, x->xctx);
        AFW_THROW_ERROR_FZ(general, x->xctx,
            "streamId " AFW_UTF8_FMT_Q " could not be set",
            AFW_UTF8_FMT_ARG(&streamId->internal));
    }

    return afw_value_create_unmanaged_integer(
        (afw_integer_t)number, x->p, x->xctx);
}



/*
 * Adaptive function: read
 *
 * afw_function_execute_read
 *
 * See afw_function_bindings.h for more information.
 *
 * Read a UTF-8 text stream up to a specified number of octets. The stream must
 * contain valid UTF-8 or an error is thrown.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function read(
 *       streamNumber: integer,
 *       n: any
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   streamNumber - (integer) Stream number.
 *
 *   n - (any dataType) The maximum number of octets to read.
 *
 * Returns:
 *
 *   (string) The UTF-8 string read. Check the size of this value to determine
 *       the actual number of octets read.
 */
const afw_value_t *
afw_function_execute_read(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *streamNumber;
    const afw_value_t *n_value;
    const afw_stream_t *stream;
    afw_octet_t *buffer;
    afw_size_t n;
    afw_size_t got;
    afw_utf8_t s;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(streamNumber,
        1, integer);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(n_value, 2);

    n_value = afw_value_convert(n_value,
        afw_data_type_integer, true, x->p, x->xctx);
    if (((const afw_value_integer_t *)n_value)->internal < 0) {
        AFW_THROW_ERROR_Z(general,
            "read() n must be a non-negative integer", x->xctx);
    }
    n = (afw_size_t)((const afw_value_integer_t *)n_value)->internal;

    stream = impl_require_stream(streamNumber->internal, x);
    if (n == 0) {
        return afw_v_a_empty_string;
    }

    buffer = afw_pool_malloc(x->p, n, x->xctx);
    got = afw_stream_read(stream, buffer, n, x->xctx);
    s.s = (const afw_utf8_octet_t *)buffer;
    s.len = got;
    if (got > 0 && !afw_utf8_is_valid(s.s, s.len, x->xctx)) {
        AFW_THROW_ERROR_Z(general,
            "read() result is not valid UTF-8", x->xctx);
    }

    return afw_value_create_unmanaged_string(&s, x->p, x->xctx);
}



/*
 * Adaptive function: read_to_base64Binary
 *
 * afw_function_execute_read_to_base64Binary
 *
 * See afw_function_bindings.h for more information.
 *
 * Read a stream up to a specified number of octets. The result will be the
 * internal memory of a base64Binary value.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function read_to_base64Binary(
 *       streamNumber: integer,
 *       n: any
 *   ): base64Binary;
 * ```
 *
 * Parameters:
 *
 *   streamNumber - (integer) Stream number.
 *
 *   n - (any dataType) The maximum number of octets to read.
 *
 * Returns:
 *
 *   (base64Binary) The base64Binary value read. Check the size of this value to
 *       determine the actual number of octets read.
 */
const afw_value_t *
afw_function_execute_read_to_base64Binary(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *streamNumber;
    const afw_value_t *n_value;
    const afw_stream_t *stream;
    afw_memory_t mem;
    afw_size_t n;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(streamNumber,
        1, integer);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(n_value, 2);

    n_value = afw_value_convert(n_value,
        afw_data_type_integer, true, x->p, x->xctx);
    if (((const afw_value_integer_t *)n_value)->internal < 0) {
        AFW_THROW_ERROR_Z(general,
            "read_to_base64Binary() n must be a non-negative integer", x->xctx);
    }
    n = (afw_size_t)((const afw_value_integer_t *)n_value)->internal;

    stream = impl_require_stream(streamNumber->internal, x);
    mem.ptr = n ? afw_pool_malloc(x->p, n, x->xctx) : NULL;
    mem.size = n ? afw_stream_read(stream, (void *)mem.ptr, n, x->xctx) : 0;

    return afw_value_create_unmanaged_base64Binary(&mem, x->p, x->xctx);
}



/*
 * Adaptive function: read_to_hexBinary
 *
 * afw_function_execute_read_to_hexBinary
 *
 * See afw_function_bindings.h for more information.
 *
 * Read a stream up to a specified number of octets. The result will be the
 * internal memory of a hexBinary value.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function read_to_hexBinary(
 *       streamNumber: integer,
 *       n: any
 *   ): hexBinary;
 * ```
 *
 * Parameters:
 *
 *   streamNumber - (integer) Stream number.
 *
 *   n - (any dataType) The maximum number of octets to read.
 *
 * Returns:
 *
 *   (hexBinary) The hexBinary value read. Check the size of this value to
 *       determine the actual number of octets read.
 */
const afw_value_t *
afw_function_execute_read_to_hexBinary(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *streamNumber;
    const afw_value_t *n_value;
    const afw_stream_t *stream;
    afw_memory_t mem;
    afw_size_t n;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(streamNumber,
        1, integer);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(n_value, 2);

    n_value = afw_value_convert(n_value,
        afw_data_type_integer, true, x->p, x->xctx);
    if (((const afw_value_integer_t *)n_value)->internal < 0) {
        AFW_THROW_ERROR_Z(general,
            "read_to_hexBinary() n must be a non-negative integer", x->xctx);
    }
    n = (afw_size_t)((const afw_value_integer_t *)n_value)->internal;

    stream = impl_require_stream(streamNumber->internal, x);
    mem.ptr = n ? afw_pool_malloc(x->p, n, x->xctx) : NULL;
    mem.size = n ? afw_stream_read(stream, (void *)mem.ptr, n, x->xctx) : 0;

    return afw_value_create_unmanaged_hexBinary(&mem, x->p, x->xctx);
}



/*
 * Adaptive function: readln
 *
 * afw_function_execute_readln
 *
 * See afw_function_bindings.h for more information.
 *
 * Read a UTF-8 text stream line. The stream must contain valid UTF-8 or an
 * error is thrown.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function readln(
 *       streamNumber: integer
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   streamNumber - (integer) Stream number.
 *
 * Returns:
 *
 *   (string) The UTF-8 string read.
 */
const afw_value_t *
afw_function_execute_readln(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *streamNumber;
    const afw_stream_t *stream;
    afw_octet_t ch;
    afw_size_t got;
    afw_size_t capacity;
    afw_size_t len;
    afw_octet_t *buffer;
    afw_utf8_t s;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(streamNumber,
        1, integer);

    stream = impl_require_stream(streamNumber->internal, x);

    capacity = 256;
    len = 0;
    buffer = afw_pool_malloc(x->p, capacity, x->xctx);

    for (;;) {
        got = afw_stream_read(stream, &ch, 1, x->xctx);
        if (got == 0) {
            break;
        }
        if (ch == '\n') {
            break;
        }
        if (ch == '\r') {
            /* Optional CRLF: peek not available; treat CR as line end. */
            continue;
        }
        if (len + 1 > capacity) {
            afw_octet_t *nb;
            capacity *= 2;
            nb = afw_pool_malloc(x->p, capacity, x->xctx);
            memcpy(nb, buffer, len);
            buffer = nb;
        }
        buffer[len++] = ch;
    }

    s.s = (const afw_utf8_octet_t *)buffer;
    s.len = len;
    if (len > 0 && !afw_utf8_is_valid(s.s, s.len, x->xctx)) {
        AFW_THROW_ERROR_Z(general,
            "readln() result is not valid UTF-8", x->xctx);
    }

    return afw_value_create_unmanaged_string(&s, x->p, x->xctx);
}



/*
 * Adaptive function: stream
 *
 * afw_function_execute_stream
 *
 * See afw_function_bindings.h for more information.
 *
 * Return the streamNumber for a streamId, including automatically opened
 * standard streams 'console', 'stderr' and 'stdout', as well as any custom open
 * stream. Throws if streamId is not open.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function stream(
 *       streamId: string
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   streamId - (string) The id of a stream.
 *
 * Returns:
 *
 *   (integer) The streamNumber for the streamId. Throws if the stream is not
 *       open.
 */
const afw_value_t *
afw_function_execute_stream(
    afw_function_execute_t *x)
{
    const afw_value_string_t *streamId;
    afw_size_t number;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(streamId, 1, string);

    number = afw_stream_get_streamNumber_for_streamId(&streamId->internal,
        x->xctx);
    if (number == (afw_size_t)-1) {
        AFW_THROW_ERROR_FZ(general, x->xctx,
            "streamId " AFW_UTF8_FMT_Q " is not open",
            AFW_UTF8_FMT_ARG(&streamId->internal));
    }

    return afw_value_create_unmanaged_integer(
        (afw_integer_t)number, x->p, x->xctx);
}



/*
 * Adaptive function: write_internal
 *
 * afw_function_execute_write_internal
 *
 * See afw_function_bindings.h for more information.
 *
 * Write a value's internal memory. This is especially useful for writing data
 * type base64Binary and hexBinary.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function write_internal(
 *       streamNumber: integer,
 *       value: any
 *   ): void;
 * ```
 *
 * Parameters:
 *
 *   streamNumber - (integer) The streamNumber for the stream to write.
 *
 *   value - (any dataType) The internal memory of this value is written
 *       (string, hexBinary, or base64Binary).
 *
 * Returns:
 *
 *   (void)
 */
const afw_value_t *
afw_function_execute_write_internal(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *streamNumber;
    const afw_value_t *value;
    const afw_stream_t *stream;
    const afw_memory_t *mem;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(streamNumber,
        1, integer);
    AFW_FUNCTION_EVALUATE_REQUIRED_PARAMETER(value, 2);

    stream = impl_require_stream(streamNumber->internal, x);

    if (AFW_VALUE_IS_DATA_TYPE(value, hexBinary) ||
        AFW_VALUE_IS_DATA_TYPE(value, base64Binary))
    {
        mem = AFW_VALUE_INTERNAL(value);
        if (mem && mem->size && mem->ptr) {
            afw_stream_write(stream, mem->ptr, mem->size, x->xctx);
        }
    }
    else if (AFW_VALUE_IS_DATA_TYPE(value, string)) {
        const afw_utf8_t *s = AFW_VALUE_INTERNAL(value);
        afw_stream_write(stream, s->s, s->len, x->xctx);
    }
    else {
        AFW_THROW_ERROR_Z(general,
            "write_internal() requires string, hexBinary, or base64Binary",
            x->xctx);
    }

    return afw_value_undefined;
}
