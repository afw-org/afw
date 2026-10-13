#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
POST /afw of a built-in that returns a new array or object (#600).

Hermetic afwfcgi. The action path calls the function with the response's
own pool, which has no scope above it; these used to fail with
"for_scope_lifetime dest p reached job heap". Single function and
actions[] forms.
"""

from __future__ import print_function

import json
import os
import tempfile

from _afwdev.test.orchestrated.fcgi_client import fcgi_request
from _afwdev.test.orchestrated.hosts.afwfcgi import start_afwfcgi, stop_afwfcgi


CONF = (
    "[\n"
    "    {\n"
    "        type: \"requestHandler\",\n"
    "        uriPrefix: \"/\",\n"
    "        requestHandlerType: \"adapter\"\n"
    "    }\n"
    "]\n"
)

SINGLE = [
    ("reverse", {"function": "reverse", "array": [1, 2]}, [2, 1]),
    ("slice", {"function": "slice", "array": [1, 2, 3], "startIndex": 1},
        [2, 3]),
    ("keys", {"function": "keys", "object": {"a": 1}}, ["a"]),
    ("add_entries", {"function": "add_entries", "target": [],
        "source": [1]}, [1]),
]

ACTIONS = {
    "actions": [
        {"function": "reverse", "array": [1, 2]},
        {"function": "slice", "array": [1, 2, 3], "startIndex": 2},
    ]
}


def _case(name, description, passed, error=None):
    return {
        "test": name,
        "description": description,
        "passed": bool(passed),
        "skip": False,
        "error": error,
    }


def _post(sock, body):
    result = fcgi_request(sock, path="/afw", method="POST",
        body=json.dumps(body, separators=(",", ":")).encode("utf-8"))
    raw = (result.get("body") or b"").decode("utf-8", errors="replace")
    try:
        return json.loads(raw), raw
    except ValueError:
        return None, raw


def run():
    tests = []
    work = tempfile.mkdtemp(prefix="afw_perform_post_")
    handle = None
    try:
        with open(os.path.join(work, "afw.conf"), "w", encoding="utf-8") as fd:
            fd.write(CONF)
        handle = start_afwfcgi(work, threads=1)
        sock = handle["socket_path"]

        for name, body, expect in SINGLE:
            obj, raw = _post(sock, body)
            ok = (isinstance(obj, dict) and obj.get("status") == "success"
                and obj.get("result") == expect)
            tests.append(_case(name,
                "POST /afw %s returns %r" % (name, expect),
                passed=ok, error=None if ok else raw[:400]))

        obj, raw = _post(sock, ACTIONS)
        got = None
        if isinstance(obj, dict) and obj.get("status") == "success":
            got = [a.get("result") for a in obj.get("actions", [])]
        ok = got == [[2, 1], [3]]
        tests.append(_case("actions",
            "POST /afw actions[] of reverse and slice",
            passed=ok, error=None if ok else raw[:400]))
    except Exception as e:
        tests.append(_case("setup", "Spawn afwfcgi", passed=False,
            error=str(e)))
    finally:
        stop_afwfcgi(handle)
    return {
        "description": "POST /afw of built-ins that return a new array "
            "or object (#600)",
        "tests": tests,
    }
