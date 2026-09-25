# -*- coding: utf-8 -*-
"""
HTTP front for one orchestrated afwfcgi.

File prefixes come from afwfcgi.http.maps. Every other path, including
/afw, is one FastCGI request on the leaf socket. /apps and /docs keep
the admin-app fallbacks from the devcontainer nginx conf.
"""

import mimetypes
import os
import socket
import threading
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from urllib.parse import unquote, urlsplit

from _afwdev.common import msg
from _afwdev.common.errors import AfwdevRunnerError
from _afwdev.common.package import find_afw_package_dir
from _afwdev.test.orchestrated.fcgi_client import fcgi_request


_RESERVED = "/afw"


class _Front:
    def __init__(self, httpd, thread, url):
        self.httpd = httpd
        self.thread = thread
        self.url = url


def _package_dir(options):
    path = None
    if options:
        path = options.get("afw_package_dir_path")
    if not path:
        path = find_afw_package_dir()
    if not path:
        raise AfwdevRunnerError(
            "afwfcgi.http needs the package root "
            "(options afw_package_dir_path)")
    return path


def _resolve_root(root, package_dir, work_dir):
    if root in (None, "", "."):
        return work_dir
    if os.path.isabs(root):
        return root
    return os.path.join(package_dir, root)


def prepare(http_doc, work_dir, options):
    """
    Bind 127.0.0.1 and set AFW_WORLD_HTTP_URL and AFW_TEST_WORK_DIR.

    Call serve() after afwfcgi is accepting. Returns a front handle.
    """
    if not isinstance(http_doc, dict):
        raise AfwdevRunnerError("afwfcgi.http must be a mapping")
    package_dir = _package_dir(options)
    maps = []
    for entry in http_doc.get("maps") or []:
        if not isinstance(entry, dict):
            raise AfwdevRunnerError("afwfcgi.http.maps entries must be mappings")
        prefix = entry.get("prefix") or ""
        if not isinstance(prefix, str) or not prefix.startswith("/"):
            raise AfwdevRunnerError(
                "afwfcgi.http.maps prefix must start with /: " + repr(prefix))
        prefix = prefix.rstrip("/") or "/"
        if prefix == "/":
            raise AfwdevRunnerError(
                "afwfcgi.http.maps prefix / would hide the request handler")
        if prefix == _RESERVED or prefix.startswith(_RESERVED + "/"):
            raise AfwdevRunnerError(
                "afwfcgi.http.maps must not use /afw; that path is the "
                "request handler")
        root = _resolve_root(entry.get("root"), package_dir, work_dir)
        if not os.path.isdir(root):
            raise AfwdevRunnerError(
                "afwfcgi.http map {} root is not a directory: {}".format(
                    prefix, root))
        maps.append((prefix, os.path.realpath(root)))
    maps.sort(key=lambda item: len(item[0]), reverse=True)

    port = http_doc.get("port")
    if port is None:
        port = 0
    else:
        port = int(port)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    try:
        sock.bind(("127.0.0.1", port))
    except OSError as e:
        sock.close()
        raise AfwdevRunnerError(
            "afwfcgi.http could not bind 127.0.0.1:{}: {}".format(port, e)
        ) from e
    sock.listen(64)
    host, bound = sock.getsockname()
    os.environ["AFW_TEST_WORK_DIR"] = work_dir
    os.environ["AFW_WORLD_HTTP_URL"] = "http://{}:{}".format(host, bound)

    httpd = ThreadingHTTPServer(
        (host, bound), _Handler, bind_and_activate=False)
    httpd.socket = sock
    httpd.server_address = (host, bound)
    httpd.front_maps = maps
    httpd.front_socket = None
    httpd.front_port = bound
    return _Front(httpd, None, os.environ["AFW_WORLD_HTTP_URL"])


def serve(front, socket_path):
    """Start the accept loop. afwfcgi must already be listening."""
    front.httpd.front_socket = socket_path
    thread = threading.Thread(
        target=front.httpd.serve_forever, name="afw-http-front", daemon=True)
    thread.start()
    front.thread = thread
    msg.highlighted_info("http " + front.url + "/")
    return front


def stop(front):
    if front is None or front.httpd is None:
        return
    httpd = front.httpd
    front.httpd = None
    try:
        httpd.shutdown()
    except Exception:
        pass
    try:
        httpd.server_close()
    except Exception:
        pass


def _under(root, path):
    real = os.path.realpath(path)
    root_real = os.path.realpath(root)
    if real == root_real or real.startswith(root_real + os.sep):
        return real
    return None


def _file_in(root, rel):
    rel = rel.lstrip("/")
    if not rel or rel.endswith("/"):
        return None
    candidate = _under(root, os.path.join(root, rel))
    if candidate and os.path.isfile(candidate):
        return candidate
    return None


def _static_file(maps, path):
    """Return (abs_path, spa) or (None, False)."""
    for prefix, root in maps:
        if path != prefix and not path.startswith(prefix + "/"):
            continue
        rel = path[len(prefix):].lstrip("/")
        found = _file_in(root, rel)
        if found:
            return found, False
        if rel:
            found = _file_in(root, rel + "/index.html")
            if found:
                return found, False
        if prefix == "/apps":
            parts = [p for p in rel.split("/") if p]
            if len(parts) >= 2:
                found = _file_in(
                    root, parts[0] + "/" + parts[1] + "/index.html")
                if found:
                    return found, True
        if prefix == "/docs":
            found = _file_in(root, "index.html")
            if found and (not rel or rel == "index.html"):
                return found, False
        return None, False
    return None, False


class _Handler(BaseHTTPRequestHandler):
    protocol_version = "HTTP/1.0"

    def log_message(self, fmt, *args):
        msg.debug("http " + (fmt % args))

    def do_GET(self):
        self._handle()

    def do_HEAD(self):
        self._handle(head=True)

    def do_POST(self):
        self._handle()

    def _handle(self, head=False):
        split = urlsplit(self.path)
        path = unquote(split.path or "/")
        if path != "/" and path.endswith("/"):
            path = path.rstrip("/")
        maps = self.server.front_maps

        if path == "/stop":
            ask = getattr(self.server, "ask_stop", None)
            if ask is not None:
                ask()
            payload = b"stopping\n"
            self.send_response(200)
            self.send_header("Content-Type", "text/plain")
            self.send_header("Content-Length", str(len(payload)))
            self.end_headers()
            if not head:
                self.wfile.write(payload)
            return

        if path == "/favicon.ico":
            self.send_response(204)
            self.end_headers()
            return

        if path in ("/", "") and any(prefix == "/apps" for prefix, _ in maps):
            self.send_response(301)
            self.send_header("Location", "/apps/afw/admin/Home")
            self.end_headers()
            return

        if path == "/docs" and any(prefix == "/docs" for prefix, _ in maps):
            self.send_response(301)
            self.send_header("Location", "/docs/index.html")
            self.end_headers()
            return

        static, _spa = _static_file(maps, path)
        if static:
            self._send_file(static, head=head)
            return
        # A mapped prefix that missed a file is 404, not a trip into afwfcgi.
        for prefix, _root in maps:
            if path == prefix or path.startswith(prefix + "/"):
                self.send_error(404, "not found")
                return
        self._send_fcgi(split, path, head=head)

    def _send_file(self, path, head=False):
        try:
            with open(path, "rb") as fd:
                data = fd.read()
        except OSError:
            self.send_error(404, "not found")
            return
        ctype = mimetypes.guess_type(path)[0] or "application/octet-stream"
        self.send_response(200)
        self.send_header("Content-Type", ctype)
        self.send_header("Content-Length", str(len(data)))
        self.end_headers()
        if not head:
            self.wfile.write(data)

    def _send_fcgi(self, split, path, head=False):
        socket_path = self.server.front_socket
        if not socket_path:
            self.send_error(503, "afwfcgi is not up")
            return
        length = int(self.headers.get("Content-Length") or "0")
        if length > 100 * 1024 * 1024:
            self.send_error(413, "body too large")
            return
        body = self.rfile.read(length) if length else b""
        if head:
            body = b""
        method = "HEAD" if head else self.command
        overrides = {
            "QUERY_STRING": split.query or "",
            "REQUEST_URI": self.path,
            "SERVER_PORT": str(self.server.front_port),
            "CONTENT_TYPE": self.headers.get("Content-Type") or "",
            "HTTP_ACCEPT": self.headers.get("Accept") or "*/*",
        }
        if overrides["CONTENT_TYPE"]:
            overrides["HTTP_CONTENT_TYPE"] = overrides["CONTENT_TYPE"]
        try:
            result = fcgi_request(
                socket_path,
                path=path,
                method=method,
                body=body,
                param_overrides=overrides,
                timeout=60.0,
            )
        except Exception as e:
            self.send_error(502, "afwfcgi: {}".format(e))
            return
        status = int(result.get("status_code") or 200)
        payload = result.get("body") or b""
        headers = result.get("headers") or {}
        self.send_response(status)
        ctype = headers.get("content-type")
        if ctype:
            self.send_header("Content-Type", ctype)
        self.send_header("Content-Length", str(len(payload)))
        self.end_headers()
        if not head:
            self.wfile.write(payload)
