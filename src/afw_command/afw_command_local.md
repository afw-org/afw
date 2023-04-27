# afw command --local

The --local parameter causes the afw command to act somewhat like a single
thread AFW server.

One use of this is to run "afw --local 1" from a program. This causes the
response output from afw local mode to be sent to stdout (fd 1), which can then
be piped to the the program for processing.

## afw command --local input and output segments

Input is read from stdin as a series of input segments and output is written to
the specified file descriptor as a series of output segments.

Anything written to stderr is not structured and is not written as segments.

Each segment consists of one or more chunks, each beginning with a length
followed by a newline (character: \n, Unicode: U+000A, ASCII: 10, hex: 0x0a).
This length is the number of octets in the Unicode UTF-8 encoded string that
follows. The last chunk of a segment must be "0\n".

The final segment written to the input stream must be "4\nexit0\n".

Example:

```
14\n"Hello World!"0\n
```

which produces the same "Hello World!" result as:

```
6\n"Hello8\n World!"0\n
```

Each input segment that begins with `++afw-local-` is a directive for afw
--local processing. All other segments are requests.

An output segment is produced for each input segment and is the JSON UTF-8
encoding \_AdaptiveResponse\_ object except when directive
`++afw-local-mode-http-like` is active.

## afw --local directives

If no directives are encountered, `++afw-local-mode-evaluate-direct` is implied
with "accept": "application/json" and "syntax" set to the --syntax specified
on the afw command or the default of expression.

Directives start with `++afw-local-` followed with a name and optionally
followed with a colon (':') and input for the directive.

### ++afw-local-mode-*

Directives beginning with `++afw-local-mode-` and followed with `action`,
`action-direct`, `evaluate`, `evaluate-direct`, or `http-like`, determine how
requests are processed. Each time one of these directives is encountered it
replaces the effects of the previous one.

#### ++afw-local-mode-action and ++afw-local-mode-action-direct

Directive `++afw-local-mode-action` and `++afw-local-mode-action-direct`
indicates that the request contains an \_AdaptiveAction\_ object in the current `CONTENT-TYPE`.

For `++afw-local-mode-action-direct`, this object is passed directly to
`afw_action_perform()` without going through normal server request process. If
you need accept content type `application/x-afw` or other request processing to
occur, use `++afw-local-mode-action` which issues a POST /afw to an HTTP-like
local server implementation.

This directive can be optionally followed immediately by a colon (':') and an
object in relaxed JSON syntax that has optional property "accept".

These properties are set in the request properties object.

| Property | Description |
| -------- | ----------- |
| `ACCEPT` | This is the media-type to use for output. The default is `application/json`. Content type `application/x-afw"`is not supported by `++afw-local-mode-action-direct`. |
| `CONTENT-TYPE` | This is the media-type to use for the input `_AdaptiveAction_` object. The default is `application/json`. Content type `application/x-afw` is not supported by `++afw-local-mode-action-direct`. |

Example:

```
++afw-local-mode-action:
{
	"ACCEPT": "application/xml"   // Use media-type "application/xml"
}
```

#### ++afw-local-mode-evaluate and ++afw-local-mode-evaluate-direct

Directive `++afw-local-mode-evaluate` and `++afw-local-mode-evaluate-direct`
indicates that an \_AdaptiveAction\_ object is created with the execute function
for the associated syntax as the "function" property and a "source" property
with the body of the request.

For `++afw-local-mode-evaluate-direct`, this object is passed directly to
`afw_action_perform()`  without going through normal server request process. If
you need accept content type `application/x-afw` or other request processing to
occur, use `++afw-local-mode-evaluate` which issues a POST /afw to an HTTP-like
local server implementation.

This directive can be optionally followed immediately by a colon (':') and an
object in relaxed JSON syntax that has optional properties "syntax" and
"accept".



| Property | Description |
| -------- | ----------- |
| `syntax` | This is the syntax of the request. The default is the --syntax value specified on the afw command which default to expression. |
| `ACCEPT` | This is the media-type to use for output. The default is application/json. Content type `application/x-afw` is not supported by `++afw-local-mode-evaluate-direct`. |

Example:

```
++afw-local-mode-evaluate:
{
    "ACCEPT": "application/xml",    // Use media-type "application/x-afw"
    "syntax": "template"            // Evaluate request as a template                 
}
```

#### ++afw-local-mode-http-like

Directive `++afw-local-mode-http-like` indicates that input segments contain a
request similar to a subset of an HTTP request. This request is processed by afw
in a similar way to how it processes an actual HTTP request, but via an
HTTP-like local server implementation.

The --conf specified on the afw command allow much of the configuration
available to an afw HTTP server. If there is not a conf file supplied, a request
handler is added for "/" that used the adaptor request handler.

The first line of the request must be an HTTP method name followed by a URI. The
remainder of the first line is ignored for now but for possible future
enhancements should be "HTTP1.1".

All headers are available as request properties.

Headers "CONTENT_TYPE", "ACCEPT" and "QUERY_STRING" are used if specified. The
other headers are ignored. "CONTENT_TYPE" and "ACCEPT" default to
"application/json".

Content length is determined by the size of the input request body, not the
"CONTENT_LENGTH" header.

Header lines end with a line feed ('\n' or 0x0A) or a carriage return line feed
('\r\n' or 0x0D0x0A).

The response output also looks similar to an HTTP response.

Example:

```
GET /afw/_AdaptiveDataType_/string HTTP1.1
ACCEPT: "application/xml"
```

Response:

```
HTTP/1.1 200 OK
Server: afw_command_local/1.0.0
Date: Sat, 26 Mar 2022 18:12:27 GMT
Content-Type: application/xml

<?xml version="1.0" encoding="UTF-8"?>
<object objectType="_AdaptiveDataType_">
	<string name="brief">A string</string>
	<string name="cType">afw_utf8_t</string>
	<string name="compileType">error</string>
	<string name="dataType">string</string>
	<string name="dataTypeParameterType">MediaType</string>
	<string name="description">Data type string hold a sequence of Unicode code points as defined at http://www.unicode.org.</string>
	<boolean name="directReturn">false</boolean>
	<boolean name="evaluated">false</boolean>
	<boolean name="jsonImpliesDataType">true</boolean>
	<string name="jsonPrimitive">string</string>
	<string name="ldapOid">1.3.6.1.4.1.1466.115.121.1.15{64512}</string>
	<boolean name="relationalCompares">true</boolean>
	<boolean name="scalar">true</boolean>
</object>
```

### ++afw-local-multi-request-properties

This directive supplies request properties for all subsequent requests using a relaxed JSON UTF-8 encoded object. This object will be available by accessing
/afw/\_AdaptiveRequest\/current and by using the "request::" qualifier. These
properties can be replaced by another ++afw-local-multi-request-properties
derivative.

Avoid properties ACCEPT, CONTENT_TYPE, REQUEST_METHOD and REQUEST_URI plus
any headers you plan to provide with `++afw-local-mode-http-like` since they
can be overridden by a request.

You can provide other HTTP-like request headers. While running afw --local, you
are running with your access to files and databases. Therefore, since you are limited by your system access, using "fake" headers is a way to test AFW authorization policies.

Example:

```
++afw-local-multi-request-properties:
{
	"REMOTE_USER": "TheBigBoss",
    "requestPurpose": "Example only",   // Request's purpose
}
```
