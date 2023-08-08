package afw::stream;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    close 
    flush 
    get_stream_error 
    open_file 
    open_response 
    open_uri 
    print 
    println 
    read 
    read_to_base64Binary 
    read_to_hexBinary 
    readln 
    stream 
    write 
    write_internal 
    writeln 
);

=head1 NAME

afw::stream - Perl module for stream functions

=head2 Functions

The following functions are exported by default

=head3 close

This will close an open stream
Close a stream

=head4 Parameters

    $streamNumber

The streamNumber for the stream to close.

=head3 flush

Write the content of the stream's buffers to its destination.
Flush stream buffer

=head4 Parameters

    $streamNumber

The streamNumber for the stream to flush.

=head3 get_stream_error

Get the most recent stream error.
Get stream error

=head4 Parameters

=head3 open_file

This will open a file stream.
Open a file stream

=head4 Parameters

    $streamId

This is the streamId that will be associated with this open file stream.

    $path

This is the path to the file to open. The rootDirectory of the path is
defined in the application object.

    $mode

This is the access mode string. Values can be:
  r - Open an existing file text file for read.
  w - Open a text file for writing. If the file does not exist, it will be
created.
  a - Open a text file for writing additional data to the end. If the file
does not exist, it will be created.
  r+ - Open a text file for both reading and writing.
  w+ - Open a text file for both reading and writing. If the file exists, it
will be overwritten. If the file does not exist, it will be created.
  a+ - Open a text file for both reading and writing. Reading will begin at
the start of the file while writing will be appended to the end.

All of these modes expect data type string. If you are using data type
base64Binary or hexBinary you can use corresponding binary modes, 'rb', 'wb',
'ab', 'rb+', 'r+b', 'wb+', 'w+b', 'ab+', and 'a+b'.

    $autoFlush

If specified and true, this will automatically flush the stream's buffers
after every write.

=head3 open_response

This will open a response text write-only stream that will be written to the
http response.
Open a response stream

=head4 Parameters

    $streamId

This is the streamId that will be associated with this open response stream.

    $autoFlush

If specified and true, this will automatically flush the stream's buffers
after every write.

=head3 open_uri

This will open a read or write stream for a URI.
Open a URI

=head4 Parameters

    $streamId

This is the streamId that will be associated with this open URI stream.

    $uri

This is the URI of the stream to open.

    $mode

This is the access mode string. Values can be 'r' for read or 'w' for write.

    $autoFlush

If specified and true, this will automatically flush the stream's buffers
after every write.

=head3 print

Evaluate and convert 0 or more values to its string value, then write them to
stdout. An undefined value is represented by `<undefined>`.
Print value

=head4 Parameters

    $values

Values to print.

=head3 println

Evaluate and convert 0 or more values to their string value, then write them
to stdout. A newline character ('\n') is written after the last value. An
undefined value is represented by `<undefined>`.
Print value followed by '\n'

=head4 Parameters

    $value

Values to print.

=head3 read

Read a UTF-8 text stream up to a specified number of octets. The stream must
contain valid UTF-8 or an error is thrown.
Read from a UTF-8 text stream

=head4 Parameters

    $streamNumber

Stream number

    $n

The maximum number of octets to read.

=head3 read_to_base64Binary

Read a stream up to a specified number of octets. The result will be the
internal memory of a base64Binary value.
Read from a stream to base64Binary

=head4 Parameters

    $streamNumber

Stream number

    $n

The maximum number of octets to read.

=head3 read_to_hexBinary

Read a stream up to a specified number of octets. The result will be the
internal memory of a hexBinary value.
Read from a stream to hexBinary

=head4 Parameters

    $streamNumber

Stream number

    $n

The maximum number of octets to read.

=head3 readln

Read a UTF-8 text stream line. The stream must contain valid UTF-8 or an
error is thrown.
Read from a UTF-8 text stream line

=head4 Parameters

    $streamNumber

Stream number

=head3 stream

This will return the streamNumber for a streamId. This function useful to
obtain the number of the automatically opened standard streams 'console',
'stderr' and 'stdout' as well and any other open stream.
Get streamNumber for a streamId

=head4 Parameters

    $streamId

The id of a stream.

=head3 write

Evaluate and convert 0 or more values to its string value, then write them to
stream. An value with an undefined value is represented by `<undefined>`.
Write values to stream as sting

=head4 Parameters

    $streamNumber

The streamNumber for the stream to write.

    $value

Values to write as their string value.

=head3 write_internal

Write a value's internal memory. This is especially useful for writing data
type base64Binary and hexBinary.
Write internal

=head4 Parameters

    $streamNumber

The streamNumber for the stream to write.

    $value

The internal memory of this value is written.

=head3 writeln

Evaluate and convert 0 or more values to its string value, then write them to
stream. A newline character ('\n') is written after the last value. An
undefined value is represented by `<undefined>`.
Write values to stream as string followed by '\n'

=head4 Parameters

    $streamNumber

The streamNumber for the stream to write.

    $value

Values to write.

=cut

sub close {
    my ($streamNumber) = @_;

    my $request = $session->request()

    $request->set("function" => "close");
    $request->set("streamNumber", $streamNumber);

    return $request->getResult();
}

sub flush {
    my ($streamNumber) = @_;

    my $request = $session->request()

    $request->set("function" => "flush");
    $request->set("streamNumber", $streamNumber);

    return $request->getResult();
}

sub get_stream_error {
    my () = @_;

    my $request = $session->request()

    $request->set("function" => "get_stream_error");

    return $request->getResult();
}

sub open_file {
    my ($streamId, $path, $mode, $autoFlush) = @_;

    my $request = $session->request()

    $request->set("function" => "open_file");
    $request->set("streamId", $streamId);
    $request->set("path", $path);
    $request->set("mode", $mode);

    if (defined $autoFlush)
        $request->set("autoFlush", $autoFlush);

    return $request->getResult();
}

sub open_response {
    my ($streamId, $autoFlush) = @_;

    my $request = $session->request()

    $request->set("function" => "open_response");
    $request->set("streamId", $streamId);

    if (defined $autoFlush)
        $request->set("autoFlush", $autoFlush);

    return $request->getResult();
}

sub open_uri {
    my ($streamId, $uri, $mode, $autoFlush) = @_;

    my $request = $session->request()

    $request->set("function" => "open_uri");
    $request->set("streamId", $streamId);
    $request->set("uri", $uri);
    $request->set("mode", $mode);

    if (defined $autoFlush)
        $request->set("autoFlush", $autoFlush);

    return $request->getResult();
}

sub print {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "print");
    $request->set("values", $values);

    return $request->getResult();
}

sub println {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "println");
    $request->set("value", $value);

    return $request->getResult();
}

sub read {
    my ($streamNumber, $n) = @_;

    my $request = $session->request()

    $request->set("function" => "read");
    $request->set("streamNumber", $streamNumber);
    $request->set("n", $n);

    return $request->getResult();
}

sub read_to_base64Binary {
    my ($streamNumber, $n) = @_;

    my $request = $session->request()

    $request->set("function" => "read_to_base64Binary");
    $request->set("streamNumber", $streamNumber);
    $request->set("n", $n);

    return $request->getResult();
}

sub read_to_hexBinary {
    my ($streamNumber, $n) = @_;

    my $request = $session->request()

    $request->set("function" => "read_to_hexBinary");
    $request->set("streamNumber", $streamNumber);
    $request->set("n", $n);

    return $request->getResult();
}

sub readln {
    my ($streamNumber) = @_;

    my $request = $session->request()

    $request->set("function" => "readln");
    $request->set("streamNumber", $streamNumber);

    return $request->getResult();
}

sub stream {
    my ($streamId) = @_;

    my $request = $session->request()

    $request->set("function" => "stream");
    $request->set("streamId", $streamId);

    return $request->getResult();
}

sub write {
    my ($streamNumber, $value) = @_;

    my $request = $session->request()

    $request->set("function" => "write");
    $request->set("streamNumber", $streamNumber);
    $request->set("value", $value);

    return $request->getResult();
}

sub write_internal {
    my ($streamNumber, $value) = @_;

    my $request = $session->request()

    $request->set("function" => "write_internal");
    $request->set("streamNumber", $streamNumber);
    $request->set("value", $value);

    return $request->getResult();
}

sub writeln {
    my ($streamNumber, $value) = @_;

    my $request = $session->request()

    $request->set("function" => "writeln");
    $request->set("streamNumber", $streamNumber);
    $request->set("value", $value);

    return $request->getResult();
}

42;
