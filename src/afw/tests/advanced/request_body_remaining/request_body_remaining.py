#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Declared Content-Length bounds how much of the request body is read.

Uses hermetic afwfcgi and the FastCGI client so CONTENT_LENGTH can
differ from the stdin bytes. A matching-length POST must still work;
extra stdin after the declared length is an error; the worker stays up.
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

MATCHED_BODY = json.dumps(
    {"function": "eval<script>", "source": "return 1;"},
    separators=(",", ":"),
).encode("utf-8")


def _case(name, description, passed, error=None):
    return {
        "test": name,
        "description": description,
        "passed": bool(passed),
        "skip": False,
        "error": error,
    }


def _json_object(result):
    body = result.get("body") or b""
    text = body.decode("utf-8", errors="replace")
    try:
        obj = json.loads(text) if text.strip() else {}
    except ValueError:
        return None, text
    if not isinstance(obj, dict):
        return None, text
    return obj, text


def _is_success(result):
    obj, _ = _json_object(result)
    return int(result.get("status_code") or 0) == 200 and (
        obj or {}).get("status") == "success"


def run():
    description = (
        "Request body read uses remaining Content-Length; extra stdin "
        "is an error and the worker stays up"
    )
    tests = []
    work = tempfile.mkdtemp(prefix="afw_req_body_")
    handle = None
    try:
        with open(os.path.join(work, "afw.conf"), "w", encoding="utf-8") as fd:
            fd.write(MINIMAL_CONF)
        handle = start_afwfcgi(work, threads=1)
        sock = handle["socket_path"]

        matched = fcgi_request(
            sock, path="/afw", method="POST", body=MATCHED_BODY)
        matched_ok = _is_success(matched)
        tests.append(_case(
            "matched-content-length",
            "POST /afw with Content-Length equal to the body succeeds",
            passed=matched_ok,
            error=None if matched_ok else (
                "status={} body={}".format(
                    matched.get("status_code"),
                    (matched.get("body") or b"")[:400])),
        ))

        extra = MATCHED_BODY + (b"X" * 64)
        oversized = fcgi_request(
            sock,
            path="/afw",
            method="POST",
            body=extra,
            param_overrides={
                "CONTENT_LENGTH": str(len(MATCHED_BODY)),
            },
        )
        obj, raw = _json_object(oversized)
        err = (obj or {}).get("error") if isinstance(obj, dict) else None
        err_id = err.get("id") if isinstance(err, dict) else None
        err_msg = err.get("message") if isinstance(err, dict) else ""
        oversized_ok = (
            int(oversized.get("status_code") or 0) == 400
            and err_id == "request_syntax"
            and "Content-Length" in (err_msg or "")
        )
        tests.append(_case(
            "extra-after-content-length",
            "Extra stdin after Content-Length is request_syntax, not success",
            passed=oversized_ok,
            error=None if oversized_ok else (
                "status={} id={!r} message={!r} body={}".format(
                    oversized.get("status_code"),
                    err_id,
                    err_msg,
                    (raw or "")[:400])),
        ))

        after = fcgi_request(
            sock, path="/afw", method="POST", body=MATCHED_BODY)
        after_ok = _is_success(after)
        tests.append(_case(
            "worker-after-mismatch",
            "The same afwfcgi worker still serves a matching POST",
            passed=after_ok,
            error=None if after_ok else (
                "status={} body={}".format(
                    after.get("status_code"),
                    (after.get("body") or b"")[:400])),
        ))
    except Exception as e:
        tests.append(_case(
            "setup",
            "Spawn afwfcgi and exercise request-body reads",
            passed=False,
            error=str(e),
        ))
    finally:
        stop_afwfcgi(handle)

    return {
        "description": description,
        "tests": tests,
    }
