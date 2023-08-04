<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for stream
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category stream
 */
class stream
{
    private $session;

    /**
     * Constructs the stream class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * close()
     *
     * This will close an open stream
     *
     * @param integer $streamNumber The streamNumber for the stream to close.
     *
     * @return null
     */
    public function close(, $streamNumber)
    {
        $request = $this->$session->request();

        $request->set("function", "close");

        /* pass along required parameters to the request payload */
        $request->set("streamNumber", $streamNumber);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * flush()
     *
     * Write the content of the stream's buffers to its destination.
     *
     * @param integer $streamNumber The streamNumber for the stream to flush.
     *
     * @return null
     */
    public function flush(, $streamNumber)
    {
        $request = $this->$session->request();

        $request->set("function", "flush");

        /* pass along required parameters to the request payload */
        $request->set("streamNumber", $streamNumber);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * get_stream_error()
     *
     * Get the most recent stream error.
     *
     *
     * @return string The most recent stream error.
     */
    public function get_stream_error()
    {
        $request = $this->$session->request();

        $request->set("function", "get_stream_error");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * open_file()
     *
     * This will open a file stream.
     *
     * @param string $streamId This is the streamId that will be associated
     *                         with this open file stream.
     * @param string $path This is the path to the file to open. The
     *                     rootDirectory of the path is defined in the
     *                     application object.
     * @param string $mode This is the access mode string. Values can be:
     *                     r - Open an existing file text file for read.
     *                     w - Open a text file for writing. If the file
     *                     does not exist, it will be created.
     *                     a - Open a text file for writing additional data
     *                     to the end. If the file does not exist, it will be
     *                     created.
     *                     r+ - Open a text file for both reading and
     *                     writing.
     *                     w+ - Open a text file for both reading and
     *                     writing. If the file exists, it will be
     *                     overwritten. If the file does not exist, it will
     *                     be created.
     *                     a+ - Open a text file for both reading and
     *                     writing. Reading will begin at the start of the
     *                     file while writing will be appended to the end.
     *                     
     *                     All of these modes expect data type string. If you
     *                     are using data type base64Binary or hexBinary you
     *                     can use corresponding binary modes, "rb", "wb",
     *                     "ab", "rb+", "r+b", "wb+", "w+b", "ab+", and
     *                     "a+b".
     * @param boolean $autoFlush If specified and true, this will
     *                           automatically flush the stream's buffers
     *                           after every write.
     *
     * @return integer The streamNumber for the streamId or -1 if there was
     *                 an error. Use get_stream_error() for error
     *                 information.
     */
    public function open_file(, $streamId, $path, $mode, $autoFlush = null)
    {
        $request = $this->$session->request();

        $request->set("function", "open_file");

        /* pass along required parameters to the request payload */
        $request->set("streamId", $streamId);
        $request->set("path", $path);
        $request->set("mode", $mode);

        /* pass along any optional parameters to the request payload */
        if ($autoFlush != null)
            $request->set('autoFlush', $autoFlush);

        return $request->get_result();
    }

    /**
     * open_response()
     *
     * This will open a response text write-only stream that will be written
     * to the http response.
     *
     * @param string $streamId This is the streamId that will be associated
     *                         with this open response stream.
     * @param boolean $autoFlush If specified and true, this will
     *                           automatically flush the stream's buffers
     *                           after every write.
     *
     * @return integer The streamNumber for the streamId or -1 if there was
     *                 an error. Use get_stream_error() for error
     *                 information.
     */
    public function open_response(, $streamId, $autoFlush = null)
    {
        $request = $this->$session->request();

        $request->set("function", "open_response");

        /* pass along required parameters to the request payload */
        $request->set("streamId", $streamId);

        /* pass along any optional parameters to the request payload */
        if ($autoFlush != null)
            $request->set('autoFlush', $autoFlush);

        return $request->get_result();
    }

    /**
     * open_uri()
     *
     * This will open a read or write stream for a URI.
     *
     * @param string $streamId This is the streamId that will be associated
     *                         with this open URI stream.
     * @param string $uri This is the URI of the stream to open.
     * @param string $mode This is the access mode string. Values can be "r"
     *                     for read or "w" for write.
     * @param boolean $autoFlush If specified and true, this will
     *                           automatically flush the stream's buffers
     *                           after every write.
     *
     * @return integer The streamNumber for the streamId or -1 if there was
     *                 an error. Use get_stream_error() for error
     *                 information.
     */
    public function open_uri(, $streamId, $uri, $mode, $autoFlush = null)
    {
        $request = $this->$session->request();

        $request->set("function", "open_uri");

        /* pass along required parameters to the request payload */
        $request->set("streamId", $streamId);
        $request->set("uri", $uri);
        $request->set("mode", $mode);

        /* pass along any optional parameters to the request payload */
        if ($autoFlush != null)
            $request->set('autoFlush', $autoFlush);

        return $request->get_result();
    }

    /**
     * print()
     *
     * Evaluate and convert 0 or more values to its string value, then write
     * them to stdout. An undefined value is represented by `<undefined>`.
     *
     * @param  $values Values to print.
     *
     * @return null
     */
    public function print(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "print");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * println()
     *
     * Evaluate and convert 0 or more values to their string value, then
     * write them to stdout. A newline character ('\n') is written after the
     * last value. An undefined value is represented by `<undefined>`.
     *
     * @param  $value Values to print.
     *
     * @return null
     */
    public function println(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "println");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * read()
     *
     * Read a UTF-8 text stream up to a specified number of octets. The
     * stream must contain valid UTF-8 or an error is thrown.
     *
     * @param integer $streamNumber Stream number
     * @param  $n The maximum number of octets to read.
     *
     * @return string The UTF-8 string read. Check the size of this value to
     *                determine the actual number of octets read.
     */
    public function read(, $streamNumber, $n)
    {
        $request = $this->$session->request();

        $request->set("function", "read");

        /* pass along required parameters to the request payload */
        $request->set("streamNumber", $streamNumber);
        $request->set("n", $n);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * read_to_base64Binary()
     *
     * Read a stream up to a specified number of octets. The result will be
     * the internal memory of a base64Binary value.
     *
     * @param integer $streamNumber Stream number
     * @param  $n The maximum number of octets to read.
     *
     * @return base64Binary The base64Binary value read. Check the size of
     *                      this value to determine the actual number of
     *                      octets read.
     */
    public function read_to_base64Binary(, $streamNumber, $n)
    {
        $request = $this->$session->request();

        $request->set("function", "read_to_base64Binary");

        /* pass along required parameters to the request payload */
        $request->set("streamNumber", $streamNumber);
        $request->set("n", $n);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * read_to_hexBinary()
     *
     * Read a stream up to a specified number of octets. The result will be
     * the internal memory of a hexBinary value.
     *
     * @param integer $streamNumber Stream number
     * @param  $n The maximum number of octets to read.
     *
     * @return hexBinary The hexBinary value read. Check the size of this
     *                   value to determine the actual number of octets read.
     */
    public function read_to_hexBinary(, $streamNumber, $n)
    {
        $request = $this->$session->request();

        $request->set("function", "read_to_hexBinary");

        /* pass along required parameters to the request payload */
        $request->set("streamNumber", $streamNumber);
        $request->set("n", $n);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * readln()
     *
     * Read a UTF-8 text stream line. The stream must contain valid UTF-8 or
     * an error is thrown.
     *
     * @param integer $streamNumber Stream number
     *
     * @return string The UTF-8 string read.
     */
    public function readln(, $streamNumber)
    {
        $request = $this->$session->request();

        $request->set("function", "readln");

        /* pass along required parameters to the request payload */
        $request->set("streamNumber", $streamNumber);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * stream()
     *
     * This will return the streamNumber for a streamId. This function useful
     * to obtain the number of the automatically opened standard streams
     * "console", "stderr" and "stdout" as well and any other open stream.
     *
     * @param string $streamId The id of a stream.
     *
     * @return integer The streamNumber for the streamId or -1 if there was
     *                 an error. Use get_stream_error() for error
     *                 information.
     */
    public function stream(, $streamId)
    {
        $request = $this->$session->request();

        $request->set("function", "stream");

        /* pass along required parameters to the request payload */
        $request->set("streamId", $streamId);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * write()
     *
     * Evaluate and convert 0 or more values to its string value, then write
     * them to stream. An value with an undefined value is represented by
     * `<undefined>`.
     *
     * @param integer $streamNumber The streamNumber for the stream to write.
     * @param any $value Values to write as their string value.
     *
     * @return null
     */
    public function write(, $streamNumber, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "write");

        /* pass along required parameters to the request payload */
        $request->set("streamNumber", $streamNumber);
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * write_internal()
     *
     * Write a value's internal memory. This is especially useful for writing
     * data type base64Binary and hexBinary.
     *
     * @param integer $streamNumber The streamNumber for the stream to write.
     * @param any $value The internal memory of this value is written.
     *
     * @return null
     */
    public function write_internal(, $streamNumber, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "write_internal");

        /* pass along required parameters to the request payload */
        $request->set("streamNumber", $streamNumber);
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * writeln()
     *
     * Evaluate and convert 0 or more values to its string value, then write
     * them to stream. A newline character ('\n') is written after the last
     * value. An undefined value is represented by `<undefined>`.
     *
     * @param integer $streamNumber The streamNumber for the stream to write.
     * @param  $value Values to write.
     *
     * @return null
     */
    public function writeln(, $streamNumber, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "writeln");

        /* pass along required parameters to the request payload */
        $request->set("streamNumber", $streamNumber);
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
