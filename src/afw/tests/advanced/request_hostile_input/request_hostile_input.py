#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Requests a client controls must not crash afwfcgi.

- A body with no Content-Type (PUT/POST object) dereferenced NULL.
- A deeply nested JSON body or script source overflowed the C stack
  in the compiler; it is now "C stack headroom exhausted".

Each case is followed by a normal request on the same worker.
"""

from __future__ import print_function

import json
import os
import tempfile

from _afwdev.test.orchestrated.fcgi_client import fcgi_request
from _afwdev.test.orchestrated.hosts.afwfcgi import start_afwfcgi, stop_afwfcgi


MINIMAL_CONF = """\
[
  {
    type: "requestHandler",
    uriPrefix: "/",
    requestHandlerType: "adapter"
  }
]
"""

GOOD_BODY = json.dumps(
    {"function": "eval<script>", "source": "return 1;"},
    separators=(",", ":"),
).encode("utf-8")

DEPTH = 200000


def _case(name, description, passed, error=None):
    return {
        "test": name,
        "description": description,
        "passed": bool(passed),
        "skip": False,
        "error": error,
    }


def _error(result):
    try:
        obj = json.loads((result.get("body") or b"").decode("utf-8", "replace"))
    except ValueError:
        return None
    return obj.get("error") if isinstance(obj, dict) else None


def _ok(result):
    try:
        obj = json.loads((result.get("body") or b"").decode("utf-8", "replace"))
    except ValueError:
        return False
    return int(result.get("status_code") or 0) == 200 and \
        isinstance(obj, dict) and obj.get("status") == "success"


def run():
    tests = []
    work = tempfile.mkdtemp(prefix="afw_hostile_req_")
    handle = None
    try:
        with open(os.path.join(work, "afw.conf"), "w", encoding="utf-8") as fd:
            fd.write(MINIMAL_CONF)
        handle = start_afwfcgi(work, threads=2)
        sock = handle["socket_path"]

        cases = [
            ("put-no-content-type",
             "PUT with a body and no Content-Type is an error",
             dict(path="/afw/_AdaptiveObjectType_/x", method="PUT",
                  body=b'{"a":1}',
                  param_overrides={"CONTENT_TYPE": None,
                                   "HTTP_CONTENT_TYPE": None}),
             "Content-Type"),
            ("post-no-content-type",
             "POST an object with no Content-Type is an error",
             dict(path="/afw/_AdaptiveObjectType_/", method="POST",
                  body=b'{"a":1}',
                  param_overrides={"CONTENT_TYPE": None,
                                   "HTTP_CONTENT_TYPE": None}),
             None),
            ("deep-json-body",
             "A deeply nested JSON body is an error, not a stack overflow",
             dict(path="/afw", method="POST",
                  body=b"[" * DEPTH + b"]" * DEPTH),
             None),
            ("deep-script-source",
             "eval<script> of deeply nested source is an error",
             dict(path="/afw", method="POST",
                  body=json.dumps({
                      "function": "eval<script>",
                      "source": "return " + "(" * DEPTH + "1" + ")" * DEPTH + ";",
                  }).encode("utf-8")),
             "headroom"),
        ]
        for name, desc, kwargs, want in cases:
            result = fcgi_request(sock, timeout=120.0, **kwargs)
            err = _error(result)
            msg = (err or {}).get("message", "") if isinstance(err, dict) else ""
            got_error = err is not None and (not want or want in msg)
            after = fcgi_request(sock, path="/afw", method="POST",
                body=GOOD_BODY)
            ok = got_error and _ok(after)
            tests.append(_case(name, desc, ok,
                None if ok else "status={} error={!r} after={}".format(
                    result.get("status_code"), err, after.get("status_code"))))
    except Exception as e:
        tests.append(_case("setup", "afwfcgi hostile request cases",
            False, str(e)))
    finally:
        stop_afwfcgi(handle)

    return {
        "description": "Client-controlled requests do not crash afwfcgi",
        "tests": tests,
    }
