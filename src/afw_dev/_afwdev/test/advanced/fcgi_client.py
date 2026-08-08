# -*- coding: utf-8 -*-
"""
Thin FastCGI responder client for talking to afwfcgi over a Unix socket.

Record framing follows the FastCGI 1.0 spec (responder role only).
Default params mirror the AFW reference nginx fastcgi_param set
(see designs/afwdev-advanced-test.md and /afw/nginx.conf).
"""

import datetime
import socket
import struct

# FastCGI constants
FCGI_VERSION = 1
FCGI_BEGIN_REQUEST = 1
FCGI_ABORT_REQUEST = 2
FCGI_END_REQUEST = 3
FCGI_PARAMS = 4
FCGI_STDIN = 5
FCGI_STDOUT = 6
FCGI_STDERR = 7
FCGI_RESPONDER = 1
FCGI_KEEP_CONN = 1


def _name_value_pair(name, value):
    name_b = name.encode("utf-8") if isinstance(name, str) else name
    value_b = value.encode("utf-8") if isinstance(value, str) else value
    nlen = len(name_b)
    vlen = len(value_b)

    def enc_len(n):
        if n < 128:
            return struct.pack("B", n)
        return struct.pack(">I", n | 0x80000000)

    return enc_len(nlen) + enc_len(vlen) + name_b + value_b


def _record(rec_type, request_id, content):
    clen = len(content)
    padding = (8 - (clen % 8)) % 8
    header = struct.pack(
        ">BBHHBx",
        FCGI_VERSION,
        rec_type,
        request_id,
        clen,
        padding,
    )
    return header + content + (b"\x00" * padding)


def default_fcgi_params(path, method="POST", body=b"", overrides=None):
    """
    Build nginx-parity default FastCGI params for a request.

    path: request path (e.g. /afw)
    method: HTTP method string
    body: request body bytes (for CONTENT_LENGTH)
    overrides: optional dict of param name -> string to merge last
    """
    if isinstance(body, str):
        body = body.encode("utf-8")
    if path is None:
        path = "/"
    if not path.startswith("/"):
        path = "/" + path

    now = datetime.datetime.utcnow().strftime("%Y-%m-%dT%H:%M:%SZ")
    params = {
        "IGNORE_URI_PREFIX": "/",
        "URI": path,
        "QUERY_STRING": "",
        "REQUEST_METHOD": method,
        "CONTENT_TYPE": "application/json",
        "CONTENT_LENGTH": str(len(body)),
        "SCRIPT_FILENAME": "",
        "SCRIPT_NAME": "",
        "PATH_INFO": "",
        "PATH_TRANSLATED": "",
        "REQUEST_URI": path,
        "DOCUMENT_URI": path,
        "DOCUMENT_ROOT": "",
        "SERVER_PROTOCOL": "HTTP/1.1",
        "GATEWAY_INTERFACE": "CGI/1.1",
        "SERVER_SOFTWARE": "afwdev-fcgi-client/1",
        "REMOTE_ADDR": "127.0.0.1",
        "REMOTE_PORT": "0",
        "SERVER_ADDR": "127.0.0.1",
        "SERVER_PORT": "8080",
        "SERVER_NAME": "localhost",
        "HTTPS": "",
        "SCHEME": "http",
        "TIME_ISO8601": now,
        "REDIRECT_STATUS": "200",
        "HTTP_ACCEPT": "application/json",
        "HTTP_CONTENT_TYPE": "application/json",
    }
    if overrides:
        for k, v in overrides.items():
            if v is None:
                params.pop(k, None)
            else:
                params[k] = str(v)
    return params


def _encode_params(params):
    chunks = []
    for name, value in params.items():
        if value is None:
            continue
        chunks.append(_name_value_pair(name, value))
    return b"".join(chunks)


def _read_record(sock):
    header = _recv_exact(sock, 8)
    if not header:
        return None
    version, rec_type, request_id, clen, padding, _ = struct.unpack(
        ">BBHHBB", header)
    content = _recv_exact(sock, clen) if clen else b""
    if padding:
        _recv_exact(sock, padding)
    return version, rec_type, request_id, content


def _recv_exact(sock, n):
    buf = bytearray()
    while len(buf) < n:
        chunk = sock.recv(n - len(buf))
        if not chunk:
            if not buf:
                return b""
            raise EOFError(
                "FCGI socket closed early (wanted {} got {})".format(
                    n, len(buf)))
        buf.extend(chunk)
    return bytes(buf)


def _parse_cgi_response(stdout_bytes):
    """
    Parse CGI/1.1 style stdout from a FastCGI responder.

    Returns (status_code, headers_dict, body_bytes).
    """
    # Headers end at blank line (CRLFCRLF or LFLF)
    sep = b"\r\n\r\n"
    idx = stdout_bytes.find(sep)
    if idx < 0:
        sep = b"\n\n"
        idx = stdout_bytes.find(sep)
    if idx < 0:
        # No headers — treat all as body
        return 200, {}, stdout_bytes

    header_blob = stdout_bytes[:idx]
    body = stdout_bytes[idx + len(sep):]
    headers = {}
    status_code = 200
    for line in header_blob.replace(b"\r\n", b"\n").split(b"\n"):
        if not line:
            continue
        if b":" not in line:
            continue
        name, _, value = line.partition(b":")
        name_s = name.decode("latin-1").strip()
        value_s = value.decode("latin-1").strip()
        headers[name_s.lower()] = value_s
        if name_s.lower() == "status":
            # "200 OK" or "Status: 200 OK"
            parts = value_s.split()
            if parts and parts[0].isdigit():
                status_code = int(parts[0])

    return status_code, headers, body


class FcgiClientError(Exception):
    """Transport or protocol error talking to afwfcgi."""


def fcgi_request(
        socket_path,
        path="/afw",
        method="POST",
        body=b"",
        param_overrides=None,
        timeout=30.0):
    """
    Send one FastCGI responder request to afwfcgi on a Unix socket.

    Returns dict:
      status_code, headers, body (bytes), stderr (bytes), app_status
    """
    if isinstance(body, str):
        body = body.encode("utf-8")

    params = default_fcgi_params(path, method=method, body=body,
                                 overrides=param_overrides)
    request_id = 1

    # FCGI_BEGIN_REQUEST body: role(2) flags(1) reserved(5)
    begin = struct.pack(">HB5s", FCGI_RESPONDER, 0, b"\x00" * 5)

    records = []
    records.append(_record(FCGI_BEGIN_REQUEST, request_id, begin))

    params_bytes = _encode_params(params)
    # Stream params in chunks if large
    offset = 0
    while offset < len(params_bytes):
        chunk = params_bytes[offset:offset + 65535]
        records.append(_record(FCGI_PARAMS, request_id, chunk))
        offset += len(chunk)
    records.append(_record(FCGI_PARAMS, request_id, b""))

    offset = 0
    while offset < len(body):
        chunk = body[offset:offset + 65535]
        records.append(_record(FCGI_STDIN, request_id, chunk))
        offset += len(chunk)
    records.append(_record(FCGI_STDIN, request_id, b""))

    payload = b"".join(records)

    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    try:
        sock.settimeout(timeout)
        try:
            sock.connect(socket_path)
        except OSError as e:
            raise FcgiClientError(
                "Cannot connect to afwfcgi socket {!r}: {}".format(
                    socket_path, e)) from e
        sock.sendall(payload)

        stdout = bytearray()
        stderr = bytearray()
        app_status = None

        while True:
            rec = _read_record(sock)
            if rec is None:
                break
            _, rec_type, rid, content = rec
            if rid != request_id and rid != 0:
                continue
            if rec_type == FCGI_STDOUT:
                if content:
                    stdout.extend(content)
            elif rec_type == FCGI_STDERR:
                if content:
                    stderr.extend(content)
            elif rec_type == FCGI_END_REQUEST:
                if len(content) >= 8:
                    app_status, protocol_status = struct.unpack(
                        ">IB3x", content[:8])
                    _ = protocol_status
                break
    except socket.timeout as e:
        raise FcgiClientError(
            "Timeout waiting for afwfcgi on {!r}".format(socket_path)) from e
    finally:
        try:
            sock.close()
        except Exception:
            pass

    status_code, headers, body_out = _parse_cgi_response(bytes(stdout))
    return {
        "status_code": status_code,
        "headers": headers,
        "body": body_out,
        "stderr": bytes(stderr),
        "app_status": app_status,
        "stdout_raw": bytes(stdout),
    }
