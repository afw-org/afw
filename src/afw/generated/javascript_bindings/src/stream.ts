// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for stream
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * This will close an open stream
 * 
 * @param {integer} streamNumber - The streamNumber for the stream to close.
 * 
 * @returns {null}
 */
export function afwClose(client : any, streamNumber : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "close";
    _action["streamNumber"] = streamNumber;

    return client.perform(_action);
}

/**
 * Write the content of the stream's buffers to its destination.
 * 
 * @param {integer} streamNumber - The streamNumber for the stream to flush.
 * 
 * @returns {null}
 */
export function afwFlush(client : any, streamNumber : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "flush";
    _action["streamNumber"] = streamNumber;

    return client.perform(_action);
}

/**
 * Get the most recent stream error.
 * 
 * @returns {string} The most recent stream error.
 */
export function afwGetStreamError(client : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "get_stream_error";

    return client.perform(_action);
}

/**
 * This will open a file stream.
 * 
 * @param {string} streamId - This is the streamId that will be associated
 *     with this open file stream.
 * 
 * @param {string} path - This is the path to the file to open. The
 *     rootDirectory of the path is defined in the application object.
 * 
 * @param {string} mode - This is the access mode string. Values can be:
 *     r - Open an existing file text file for read.
 *     w - Open a text file for writing. If the file does not exist, it
 *     will be created.
 *     a - Open a text file for writing additional data to the end. If the
 *     file does not exist, it will be created.
 *     r+ - Open a text file for both reading and writing.
 *     w+ - Open a text file for both reading and writing. If the file
 *     exists, it will be overwritten. If the file does not exist, it will be
 *     created.
 *     a+ - Open a text file for both reading and writing. Reading will
 *     begin at the start of the file while writing will be appended to the
 *     end.
 *     
 *     All of these modes expect data type string. If you are using data type
 *     base64Binary or hexBinary you can use corresponding binary modes,
 *     "rb", "wb", "ab", "rb+", "r+b", "wb+", "w+b", "ab+", and "a+b".
 * 
 * @param {boolean} autoFlush - If specified and true, this will
 *     automatically flush the stream's buffers after every write.
 * 
 * @returns {integer} The streamNumber for the streamId or -1 if there was an
 *     error. Use get_stream_error() for error information.
 */
export function afwOpenFile(client : any, streamId : string, path : string, mode : string, autoFlush? : boolean) : any {

    let _action : IAnyObject = {};

    _action["function"] = "open_file";
    _action["streamId"] = streamId;
    _action["path"] = path;
    _action["mode"] = mode;

    if (autoFlush !== undefined)
        _action["autoFlush"] = autoFlush;

    return client.perform(_action);
}

/**
 * This will open a response text write-only stream that will be written to
 * the http response.
 * 
 * @param {string} streamId - This is the streamId that will be associated
 *     with this open response stream.
 * 
 * @param {boolean} autoFlush - If specified and true, this will
 *     automatically flush the stream's buffers after every write.
 * 
 * @returns {integer} The streamNumber for the streamId or -1 if there was an
 *     error. Use get_stream_error() for error information.
 */
export function afwOpenResponse(client : any, streamId : string, autoFlush? : boolean) : any {

    let _action : IAnyObject = {};

    _action["function"] = "open_response";
    _action["streamId"] = streamId;

    if (autoFlush !== undefined)
        _action["autoFlush"] = autoFlush;

    return client.perform(_action);
}

/**
 * This will open a read or write stream for a URI.
 * 
 * @param {string} streamId - This is the streamId that will be associated
 *     with this open URI stream.
 * 
 * @param {string} uri - This is the URI of the stream to open.
 * 
 * @param {string} mode - This is the access mode string. Values can be "r"
 *     for read or "w" for write.
 * 
 * @param {boolean} autoFlush - If specified and true, this will
 *     automatically flush the stream's buffers after every write.
 * 
 * @returns {integer} The streamNumber for the streamId or -1 if there was an
 *     error. Use get_stream_error() for error information.
 */
export function afwOpenUri(client : any, streamId : string, uri : string, mode : string, autoFlush? : boolean) : any {

    let _action : IAnyObject = {};

    _action["function"] = "open_uri";
    _action["streamId"] = streamId;
    _action["uri"] = uri;
    _action["mode"] = mode;

    if (autoFlush !== undefined)
        _action["autoFlush"] = autoFlush;

    return client.perform(_action);
}

/**
 * Evaluate and convert 0 or more values to its string value, then write them
 * to stdout. An undefined value is represented by '<undefined>'.
 * 
 * @param {} values - Values to print.
 * 
 * @returns {null}
 */
export function afwPrint(client : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "print";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Evaluate and convert 0 or more values to their string value, then write
 * them to stdout. A newline character ('\n') is written after the last
 * value. An undefined value is represented by '<undefined>'.
 * 
 * @param {} value - Values to print.
 * 
 * @returns {null}
 */
export function afwPrintln(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "println";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Read a UTF-8 text stream up to a specified number of octets. The stream
 * must contain valid UTF-8 or an error is thrown.
 * 
 * @param {integer} streamNumber - Stream number
 * 
 * @param {} n - The maximum number of octets to read.
 * 
 * @returns {string} The UTF-8 string read. Check the size of this value to
 *     determine the actual number of octets read.
 */
export function afwRead(client : any, streamNumber : number, n : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "read";
    _action["streamNumber"] = streamNumber;
    _action["n"] = n;

    return client.perform(_action);
}

/**
 * Read a stream up to a specified number of octets. The result will be the
 * internal memory of a base64Binary value.
 * 
 * @param {integer} streamNumber - Stream number
 * 
 * @param {} n - The maximum number of octets to read.
 * 
 * @returns {base64Binary} The base64Binary value read. Check the size of
 *     this value to determine the actual number of octets read.
 */
export function afwReadToBase64Binary(client : any, streamNumber : number, n : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "read_to_base64Binary";
    _action["streamNumber"] = streamNumber;
    _action["n"] = n;

    return client.perform(_action);
}

/**
 * Read a stream up to a specified number of octets. The result will be the
 * internal memory of a hexBinary value.
 * 
 * @param {integer} streamNumber - Stream number
 * 
 * @param {} n - The maximum number of octets to read.
 * 
 * @returns {hexBinary} The hexBinary value read. Check the size of this
 *     value to determine the actual number of octets read.
 */
export function afwReadToHexBinary(client : any, streamNumber : number, n : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "read_to_hexBinary";
    _action["streamNumber"] = streamNumber;
    _action["n"] = n;

    return client.perform(_action);
}

/**
 * Read a UTF-8 text stream line. The stream must contain valid UTF-8 or an
 * error is thrown.
 * 
 * @param {integer} streamNumber - Stream number
 * 
 * @returns {string} The UTF-8 string read.
 */
export function afwReadln(client : any, streamNumber : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "readln";
    _action["streamNumber"] = streamNumber;

    return client.perform(_action);
}

/**
 * This will return the streamNumber for a streamId. This function useful to
 * obtain the number of the automatically opened standard streams "console",
 * "stderr" and "stdout" as well and any other open stream.
 * 
 * @param {string} streamId - The id of a stream.
 * 
 * @returns {integer} The streamNumber for the streamId or -1 if there was an
 *     error. Use get_stream_error() for error information.
 */
export function afwStream(client : any, streamId : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "stream";
    _action["streamId"] = streamId;

    return client.perform(_action);
}

/**
 * Evaluate and convert 0 or more values to its string value, then write them
 * to stream. An value with an undefined value is represented by
 * '<undefined>'.
 * 
 * @param {integer} streamNumber - The streamNumber for the stream to write.
 * 
 * @param {any} value - Values to write as their string value.
 * 
 * @returns {null}
 */
export function afwWrite(client : any, streamNumber : number, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "write";
    _action["streamNumber"] = streamNumber;
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Write a value's internal memory. This is especially useful for writing
 * data type base64Binary and hexBinary.
 * 
 * @param {integer} streamNumber - The streamNumber for the stream to write.
 * 
 * @param {any} value - The internal memory of this value is written.
 * 
 * @returns {null}
 */
export function afwWriteInternal(client : any, streamNumber : number, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "write_internal";
    _action["streamNumber"] = streamNumber;
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Evaluate and convert 0 or more values to its string value, then write them
 * to stream. A newline character ('\n') is written after the last value. An
 * undefined value is represented by '<undefined>'.
 * 
 * @param {integer} streamNumber - The streamNumber for the stream to write.
 * 
 * @param {} value - Values to write.
 * 
 * @returns {null}
 */
export function afwWriteln(client : any, streamNumber : number, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "writeln";
    _action["streamNumber"] = streamNumber;
    _action["value"] = value;

    return client.perform(_action);
}

