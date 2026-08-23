#!/usr/bin/env python3

import json
import os
import threading
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

# test configuration settings
Environment = "curl"

# Minimal local HTTP stub for http_* function regression tests, standing in
# for httpbin.org so the suite runs deterministically, offline, and on every
# `afwdev test` invocation instead of only when TEST_CURL_HTTPBIN is set.
# Runs for the lifetime of this test group (see before_all / after_all
# below); its port is published to the .as tests via AFW_CURL_TEST_HTTP_PORT.
# Mirrors the smtp stub in tests/smtp/config.py.
#
# Behavior:
#   - GET/POST/PUT/PATCH/DELETE echo the request back as a JSON body
#     ({"method", "path", "headers", "data", and "json" when the request's
#     Content-Type contains "json"}), so tests can assert on request/response
#     round-tripping the same way the old httpbin.org-based tests did.
#   - HEAD/OPTIONS send headers only, per HTTP semantics.
#   - A path of "/status/<code>" forces that response code.

_server = None
_server_thread = None


class _Handler(BaseHTTPRequestHandler):
    protocol_version = "HTTP/1.1"

    def log_message(self, format, *args):
        pass

    def _status_from_path(self):
        if self.path.startswith("/status/"):
            try:
                return int(self.path[len("/status/"):])
            except ValueError:
                pass
        return 200

    def _read_body(self):
        length = int(self.headers.get("Content-Length", 0) or 0)
        return self.rfile.read(length) if length else b""

    def _respond(self, with_body):
        code = self._status_from_path()
        raw = self._read_body()

        self.close_connection = True

        payload = b""
        if with_body:
            body = {
                "method": self.command,
                "path": self.path,
                "headers": dict(self.headers.items()),
            }
            if raw:
                body["data"] = raw.decode("utf-8", errors="replace")
                if "json" in self.headers.get("Content-Type", ""):
                    try:
                        body["json"] = json.loads(raw)
                    except ValueError:
                        pass
            payload = json.dumps(body).encode("utf-8")

        self.send_response(code)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(payload)))
        self.send_header("X-Afw-Test", "stub")
        self.end_headers()
        if payload:
            self.wfile.write(payload)

    def do_GET(self):
        self._respond(with_body=True)

    do_POST = do_GET
    do_PUT = do_GET
    do_PATCH = do_GET
    do_DELETE = do_GET

    def do_HEAD(self):
        self._respond(with_body=False)

    do_OPTIONS = do_HEAD


def before_all():
    global _server, _server_thread

    _server = ThreadingHTTPServer(("127.0.0.1", 0), _Handler)
    os.environ["AFW_CURL_TEST_HTTP_PORT"] = str(_server.server_address[1])

    _server_thread = threading.Thread(target=_server.serve_forever, daemon=True)
    _server_thread.start()


def after_all():
    global _server, _server_thread

    if _server:
        _server.shutdown()
        _server.server_close()
    if _server_thread:
        _server_thread.join(timeout=5)
