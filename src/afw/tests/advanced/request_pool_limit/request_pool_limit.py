#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
limitRequestPoolBytes vs GET /afw/_AdaptiveEnvironmentRegistry_/current.

Hermetic afwfcgi. A cheap eval must still work; the registry current get
must fail when the request-pool cap is below that object's materialize
cost. The error is often payload_too_large (cap tripped with room to
throw) but may be memory if allocation fails — either is OK. The worker
must stay up.

Measured 2026-09-15 on this tree (asked-for request pool):

- Cheap `return 1` succeeds at 64 KiB.
- REST GET current fails at 7 MiB and succeeds at 8 MiB
  (~1.5 MiB JSON). Script stringify of the same object uses more than
  REST encode.

maxObjects is a separate cardinality throw (see max_objects.as).
"""

from __future__ import print_function

import json
import os
import tempfile

from _afwdev.test.orchestrated.fcgi_client import fcgi_request
from _afwdev.test.orchestrated.hosts.afwfcgi import start_afwfcgi, stop_afwfcgi


# Small enough that GET current cannot succeed, large enough that a cheap
# eval still can.
SMALL_LIMIT = 65536

# Last measured fail for REST GET current; 8 MiB succeeds. Catalog growth
# keeps this a fail; a large shrink would need a re-measure.
CROSSING_LIMIT = 7 * 1024 * 1024

REGISTRY_CURRENT = "/afw/_AdaptiveEnvironmentRegistry_/current"

OOM_IDS = ("payload_too_large", "memory")

CHEAP_BODY = json.dumps(
    {"function": "eval<script>", "source": "return 1;"},
    separators=(",", ":"),
).encode("utf-8")


def _conf(limit):
    return (
        "[\n"
        "    {\n"
        "        type: \"application\",\n"
        "        applicationId: \"request-pool-limit\",\n"
        "        limitRequestPoolBytes: %d\n"
        "    },\n"
        "    {\n"
        "        type: \"requestHandler\",\n"
        "        uriPrefix: \"/\",\n"
        "        requestHandlerType: \"adapter\"\n"
        "    }\n"
        "]\n" % limit
    )


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


def _error_id(obj):
    if not isinstance(obj, dict):
        return None
    err = obj.get("error")
    if isinstance(err, dict):
        return err.get("id")
    return None


def _is_success_eval(result):
    obj, _ = _json_object(result)
    return (obj or {}).get("status") == "success" and (
        obj or {}).get("result") == 1


def _rest_get_current(sock):
    return fcgi_request(
        sock,
        path=REGISTRY_CURRENT,
        method="GET",
        body=b"",
        param_overrides={
            "CONTENT_TYPE": "",
            "CONTENT_LENGTH": "0",
            "HTTP_CONTENT_TYPE": "",
        },
    )


def _cheap(sock):
    return fcgi_request(sock, path="/afw", method="POST", body=CHEAP_BODY)


def _run_limit(limit, prefix, tests):
    work = tempfile.mkdtemp(prefix="afw_req_pool_%s_" % prefix)
    handle = None
    try:
        with open(os.path.join(work, "afw.conf"), "w", encoding="utf-8") as fd:
            fd.write(_conf(limit))
        handle = start_afwfcgi(work, threads=1)
        sock = handle["socket_path"]

        cheap = _cheap(sock)
        cheap_ok = _is_success_eval(cheap)
        tests.append(_case(
            "%s-cheap" % prefix,
            "Cheap eval succeeds with limitRequestPoolBytes=%d" % limit,
            passed=cheap_ok,
            error=None if cheap_ok else (
                "status={} body={}".format(
                    cheap.get("status_code"),
                    (cheap.get("body") or b"")[:400])),
        ))

        got = _rest_get_current(sock)
        obj, raw = _json_object(got)
        eid = _error_id(obj)
        oom_ok = eid in OOM_IDS
        tests.append(_case(
            "%s-get-registry-current" % prefix,
            "GET %s fails under limitRequestPoolBytes=%d "
            "(payload_too_large or memory)" % (REGISTRY_CURRENT, limit),
            passed=oom_ok,
            error=None if oom_ok else (
                "status={} id={!r} body={}".format(
                    got.get("status_code"), eid, (raw or "")[:400])),
        ))

        after = _cheap(sock)
        after_ok = _is_success_eval(after)
        tests.append(_case(
            "%s-worker-still-serves" % prefix,
            "Worker still serves after the registry current get failed",
            passed=after_ok,
            error=None if after_ok else (
                "status={} body={}".format(
                    after.get("status_code"),
                    (after.get("body") or b"")[:400])),
        ))
    except Exception as e:
        tests.append(_case(
            "%s-setup" % prefix,
            "Spawn afwfcgi with limitRequestPoolBytes=%d" % limit,
            passed=False,
            error=str(e),
        ))
    finally:
        stop_afwfcgi(handle)


def run():
    description = (
        "afwfcgi limitRequestPoolBytes trips GET "
        "/afw/_AdaptiveEnvironmentRegistry_/current; worker stays up"
    )
    tests = []
    _run_limit(SMALL_LIMIT, "small", tests)
    _run_limit(CROSSING_LIMIT, "crossing", tests)
    return {
        "description": description,
        "tests": tests,
    }
