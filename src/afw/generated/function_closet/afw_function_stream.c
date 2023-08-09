// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for stream
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_stream.c
 * @brief afw_function_execute_* functions for stream.
 */

#include "afw.h"



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
 *   ): null;
 * ```
 *
 * Parameters:
 *
 *   streamNumber - (integer) The streamNumber for the stream to close.
 *
 * Returns:
 *
 *   (null)
 */
const afw_value_t *
afw_function_execute_close(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
 *   ): null;
 * ```
 *
 * Parameters:
 *
 *   streamNumber - (integer) The streamNumber for the stream to flush.
 *
 * Returns:
 *
 *   (null)
 */
const afw_value_t *
afw_function_execute_flush(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: get_stream_error
 *
 * afw_function_execute_get_stream_error
 *
 * See afw_function_bindings.h for more information.
 *
 * Get the most recent stream error.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function get_stream_error(
 *   
 *   ): string;
 * ```
 *
 * Parameters:
 *
 * Returns:
 *
 *   (string) The most recent stream error.
 */
const afw_value_t *
afw_function_execute_get_stream_error(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: open_file
 *
 * afw_function_execute_open_file
 *
 * See afw_function_bindings.h for more information.
 *
 * This will open a file stream.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
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
 *   path - (string) This is the path to the file to open. The rootDirectory of
 *       the path is defined in the application object.
 *
 *   mode - (string) This is the access mode string. Values can be:
 *         r - Open an existing file text file for read.
 *         w - Open a text file for writing. If the file does not exist, it
 *       will be created.
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
 *       base64Binary or hexBinary you can use corresponding binary modes,
 *       'rb', 'wb', 'ab', 'rb+', 'r+b', 'wb+', 'w+b', 'ab+', and 'a+b'.
 *
 *   autoFlush - (optional boolean) If specified and true, this will
 *       automatically flush the stream's buffers after every write.
 *
 * Returns:
 *
 *   (integer) The streamNumber for the streamId or -1 if there was an error.
 *       Use get_stream_error() for error information.
 */
const afw_value_t *
afw_function_execute_open_file(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: open_response
 *
 * afw_function_execute_open_response
 *
 * See afw_function_bindings.h for more information.
 *
 * This will open a response text write-only stream that will be written to the
 * http response.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function open_response(
 *       streamId: string,
 *       autoFlush?: boolean
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   streamId - (string) This is the streamId that will be associated with this
 *       open response stream.
 *
 *   autoFlush - (optional boolean) If specified and true, this will
 *       automatically flush the stream's buffers after every write.
 *
 * Returns:
 *
 *   (integer) The streamNumber for the streamId or -1 if there was an error.
 *       Use get_stream_error() for error information.
 */
const afw_value_t *
afw_function_execute_open_response(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: open_uri
 *
 * afw_function_execute_open_uri
 *
 * See afw_function_bindings.h for more information.
 *
 * This will open a read or write stream for a URI.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function open_uri(
 *       streamId: string,
 *       uri: string,
 *       mode: string,
 *       autoFlush?: boolean
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   streamId - (string) This is the streamId that will be associated with this
 *       open URI stream.
 *
 *   uri - (string) This is the URI of the stream to open.
 *
 *   mode - (string) This is the access mode string. Values can be 'r' for read
 *       or 'w' for write.
 *
 *   autoFlush - (optional boolean) If specified and true, this will
 *       automatically flush the stream's buffers after every write.
 *
 * Returns:
 *
 *   (integer) The streamNumber for the streamId or -1 if there was an error.
 *       Use get_stream_error() for error information.
 */
const afw_value_t *
afw_function_execute_open_uri(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: print
 *
 * afw_function_execute_print
 *
 * See afw_function_bindings.h for more information.
 *
 * Evaluate and convert 0 or more values to its string value, then write them
 * to stdout. An undefined value is represented by `<undefined>`.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function print(
 *       ...values: (array of any)
 *   ): null;
 * ```
 *
 * Parameters:
 *
 *   values - (0 or more any dataType) Values to print.
 *
 * Returns:
 *
 *   (null)
 */
const afw_value_t *
afw_function_execute_print(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
 * undefined value is represented by `<undefined>`.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function println(
 *       ...value: (array of any)
 *   ): null;
 * ```
 *
 * Parameters:
 *
 *   value - (0 or more any dataType) Values to print.
 *
 * Returns:
 *
 *   (null)
 */
const afw_value_t *
afw_function_execute_println(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
 *   (base64Binary) The base64Binary value read. Check the size of this value
 *       to determine the actual number of octets read.
 */
const afw_value_t *
afw_function_execute_read_to_base64Binary(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: stream
 *
 * afw_function_execute_stream
 *
 * See afw_function_bindings.h for more information.
 *
 * This will return the streamNumber for a streamId. This function useful to
 * obtain the number of the automatically opened standard streams 'console',
 * 'stderr' and 'stdout' as well and any other open stream.
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
 *   (integer) The streamNumber for the streamId or -1 if there was an error.
 *       Use get_stream_error() for error information.
 */
const afw_value_t *
afw_function_execute_stream(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: write
 *
 * afw_function_execute_write
 *
 * See afw_function_bindings.h for more information.
 *
 * Evaluate and convert 0 or more values to its string value, then write them
 * to stream. An value with an undefined value is represented by `<undefined>`.
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
 *   ): null;
 * ```
 *
 * Parameters:
 *
 *   streamNumber - (integer) The streamNumber for the stream to write.
 *
 *   value - (0 or more any) Values to write as their string value.
 *
 * Returns:
 *
 *   (null)
 */
const afw_value_t *
afw_function_execute_write(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
 *   ): null;
 * ```
 *
 * Parameters:
 *
 *   streamNumber - (integer) The streamNumber for the stream to write.
 *
 *   value - (any) The internal memory of this value is written.
 *
 * Returns:
 *
 *   (null)
 */
const afw_value_t *
afw_function_execute_write_internal(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: writeln
 *
 * afw_function_execute_writeln
 *
 * See afw_function_bindings.h for more information.
 *
 * Evaluate and convert 0 or more values to its string value, then write them
 * to stream. A newline character ('\n') is written after the last value. An
 * undefined value is represented by `<undefined>`.
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
 *   ): null;
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
 *   (null)
 */
const afw_value_t *
afw_function_execute_writeln(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
