#! /usr/bin/env python3
##
# @file fuzz_hostile_pieces.py
# @brief Request pieces for the hostile fuzz source (#485 step 5).
#
# fuzz.py calls build(rnd) once per request. Contract:
#
#   build(rnd) -> {"path": str, "method": str, "body": bytes,
#                  "overrides": {param name: str or None}}
#
#   - rnd is the random.Random fuzz.py passes in. Use only it: no other
#     randomness and no clock, so request i is the same every run and
#     --replay SEED:INDEX resends it.
#   - The request goes through fcgi_client.fcgi_request(path, method,
#     body, param_overrides=overrides). An override of None drops that
#     FastCGI parameter; any other value replaces the default (for
#     example CONTENT_LENGTH, CONTENT_TYPE, HTTP_ACCEPT, QUERY_STRING).
#   - Action bodies stay simple: nothing that loops (that is the mutate
#     source's job, after #490).
#   - Paths name only the leaf's adapters: afw (runtime) and data (a file
#     adapter rooted in the leaf's work directory).
#   - Keep each request under about 1MB.
#
# Any HTTP status is a fine answer. A request fails only on a timeout, a
# lost connection, or a reply without a FastCGI end record or status
# line.
#
# Variation matches the old local fcgi_fuzz.py: paths, methods, query
# strings, content types and accepts, CONTENT_LENGTH mismatches, dropped
# or oversized parameters, and malformed bodies, plus a share of ordinary
# valid requests.
#

import json

# Ordinary requests so a run still exercises the happy path.
_VALID = (
    ("GET", "/afw/_AdaptiveProcess_/current", None),
    ("GET", "/afw/_AdaptiveServer_/current", None),
    ("GET", "/data/Demo/seed", None),
    ("GET", "/data/Demo/", None),
    ("GET", "/afw/_AdaptiveObjectType_/_AdaptiveFunction_", None),
    ("GET", "/data/_AdaptiveObjectType_/Demo", None),
    ("GET", "/afw/_AdaptiveAdapter_/data", None),
    ("POST", "/afw", {"actions": [{"function": "add", "a": 1, "b": 2}]}),
    ("POST", "/afw", {"actions": [
        {"function": "eval<script>", "source": "return 1;"}]}),
    ("POST", "/afw", {
        "function": "get_object",
        "adapterId": "data",
        "objectType": "Demo",
        "objectId": "seed",
    }),
    ("POST", "/afw", {
        "function": "retrieve_objects",
        "adapterId": "data",
        "objectType": "Demo",
    }),
    ("PUT", "/data/Demo/fuzz", {"msg": "x"}),
)

# Paths may name only adapters afw and data. Traversal and junk on those
# two are the point; other adapter ids would 404 before the handler.
_PATHS = (
    "/afw",
    "/afw/",
    "/afw/_AdaptiveObjectType_",
    "/afw/_AdaptiveObjectType_/_AdaptiveFunction_",
    "/afw/_AdaptiveFunction_/add",
    "/afw/_AdaptiveFunction_/nonexistent",
    "/afw/_AdaptiveProcess_/current",
    "/afw/_AdaptiveServer_/current",
    "/afw/_AdaptiveAdapter_/afw",
    "/afw/_AdaptiveAdapter_/data",
    "/afw/_AdaptiveDataType_/string",
    "/afw/_AdaptiveRequestProperties_/current",
    "/afw/_AdaptiveObjectType_/_AdaptiveObjectType_",
    "/data",
    "/data/",
    "/data/Demo",
    "/data/Demo/",
    "/data/Demo/seed",
    "/data/Demo/seed/",
    "/data/Demo/fuzz",
    "/data/Demo/x",
    "/data/_AdaptiveObjectType_/Demo",
    "/data/Demo/..%2F..%2Fafw.conf",
    "/data/../afw.conf",
    "/data/Demo/%2e%2e/%2e%2e/afw.conf",
    "/%00",
    "/" + "a" * 5000,
    "/afw/%ZZ",
    "/afw/_AdaptiveFunction_/" + "%C0%AF" * 10,
    "/afw/ /x",
    "/data/Demo/\u00e9",
    "//",
    "/afw;x=1",
    "/afw/_AdaptiveProcess_/current/extra",
    "/not-an-adapter/x",
    "/AFW/_AdaptiveProcess_/current",
    "",
)

_METHODS = (
    "GET", "POST", "PUT", "PATCH", "DELETE", "HEAD", "OPTIONS",
    "TRACE", "BREW", "CONNECT", "MOVE", "", "get", "POST ",
)

_CTYPES = (
    "application/json",
    "application/x-afw",
    "application/ubjson",
    "text/plain",
    "application/xml",
    "multipart/form-data; boundary=x",
    "application/json; charset=latin1",
    "application/json; charset=utf-8",
    "application/yaml",
    "",
    ";;;",
    "application/json\r\nX-Injected: 1",
)

_ACCEPTS = (
    "application/json",
    "application/x-afw",
    "application/ubjson",
    "*/*",
    "text/html",
    "application/yaml",
    "application/xml",
    "",
    "application/json;q=0",
    "application/json, application/x-afw;q=0.8",
    "nosuch/type",
)

_QUERIES = (
    "a=1",
    "a",
    "=",
    "%",
    "a=%zz",
    "x=" + "y" * 3000,
    "_flags_=response:metrics",
    "_flags_=debug:x",
    "&&&",
    "pretty=true",
    "a=1&b=2",
    "a=1&a=2",
    "eq(msg,\"seed\")",
    "limit(1)",
    "",
    "a=\x00",
)

# No while/for/do_while and no recursion: AFW has no evaluation time
# limit yet (#490). Writes stay on adapter data (the leaf work directory).
_ACTIONS = (
    {"function": "add", "a": 1, "b": 2},
    {"function": "eval<script>", "source": "return 1;"},
    {"function": "eval<script>", "source": "return 1 + 2;"},
    {"function": "eval<script>", "source": "const x = 1; return x;"},
    {"function": "eval<script>", "source": "return true;"},
    {"function": "eval<script>", "source": "throw 'x';"},
    {"function": "eval<script>", "source": "return;"},
    {"function": "eval<script>", "source": "{{{"},
    {"function": "eval<script>",
     "source": "return get_object('afw','_AdaptiveServer_','current');"},
    {"function": "eval<script>", "source": 1},
    {"function": "evaluate", "expression": "1+2"},
    {"function": "evaluate", "expression": "1+"},
    {"function": "compile<script>", "source": "return 1;"},
    {"function": "retrieve_objects", "adapterId": "afw",
     "objectType": "_AdaptiveDataType_"},
    {"function": "retrieve_objects", "adapterId": "data",
     "objectType": "Demo"},
    {"function": "get_object", "adapterId": "data",
     "objectType": "Demo", "objectId": "seed"},
    {"function": "get_object", "adapterId": "afw",
     "objectType": "_AdaptiveProcess_", "objectId": "current"},
    {"function": "add_object", "adapterId": "data", "objectType": "Demo",
     "objectId": "fuzz", "object": {"msg": "x"}},
    {"function": "replace_object", "adapterId": "data", "objectType": "Demo",
     "objectId": "fuzz", "object": {"msg": "y"}},
    {"function": "replace_object", "adapterId": "data", "objectType": "Demo",
     "objectId": "seed", "object": "notobj"},
    {"function": "delete_object", "adapterId": "data", "objectType": "Demo",
     "objectId": "fuzz"},
    {"function": "delete_object", "adapterId": "data", "objectType": "Demo",
     "objectId": "../../afw.conf"},
    {"function": "nonexistent"},
    {"function": None},
    {"function": 1},
    {"function": "add"},
    {"function": "length", "value": [1, 2, 3]},
    {"function": "concat", "values": ["a", "b"]},
    {"_flags_": ["response:metrics", "debug:x"],
     "function": "add", "a": 1, "b": 2},
)

# A missing REQUEST_METHOD, QUERY_STRING, or URI (PATH_INFO or
# REQUEST_URI) is answered with a 400 (it used to be an empty reply).
_DROPPABLE = (
    "REQUEST_METHOD",
    "QUERY_STRING",
    "REQUEST_URI",
    "IGNORE_URI_PREFIX",
    "URI",
    "CONTENT_TYPE",
    "CONTENT_LENGTH",
    "SCRIPT_FILENAME",
    "SCRIPT_NAME",
    "PATH_INFO",
    "PATH_TRANSLATED",
    "DOCUMENT_URI",
    "DOCUMENT_ROOT",
    "SERVER_PROTOCOL",
    "GATEWAY_INTERFACE",
    "SERVER_SOFTWARE",
    "REMOTE_ADDR",
    "REMOTE_PORT",
    "SERVER_ADDR",
    "SERVER_PORT",
    "SERVER_NAME",
    "HTTPS",
    "SCHEME",
    "TIME_ISO8601",
    "REDIRECT_STATUS",
    "HTTP_ACCEPT",
    "HTTP_CONTENT_TYPE",
)

_MAX_BODY = 800000


def _skewed(rnd, lo, hi):
    """Mostly small values in [lo, hi]."""
    span = hi - lo
    if span <= 0:
        return lo
    r = rnd.random()
    if r < 0.7:
        return lo + rnd.randint(0, min(span, 64))
    if r < 0.9:
        return lo + rnd.randint(0, min(span, 1024))
    return rnd.randint(lo, hi)


def _json(obj):
    return json.dumps(obj).encode("utf-8")


def _valid(rnd):
    method, path, body = rnd.choice(_VALID)
    data = _json(body) if body is not None else b""
    return {"path": path, "method": method, "body": data, "overrides": {}}


def _body(rnd):
    r = rnd.random()
    if r < 0.50:
        acts = [rnd.choice(_ACTIONS) for _ in range(rnd.randint(0, 5))]
        kind = rnd.random()
        if kind < 0.80:
            obj = {"actions": acts}
        elif kind < 0.86:
            obj = rnd.choice(_ACTIONS)
        elif kind < 0.90:
            obj = {"actions": "x"}
        elif kind < 0.94:
            obj = {"actions": {}}
        elif kind < 0.97:
            obj = []
        else:
            obj = {"actions": [None, 1, "x"]}
        b = _json(obj)
    elif r < 0.62:
        b = b'{"actions":[' * _skewed(rnd, 1, 40000)
    elif r < 0.72:
        b = rnd.randbytes(_skewed(rnd, 0, 80000))
    elif r < 0.80:
        b = b'{"a":' + b"1" * _skewed(rnd, 1, 50000) + b"}"
    elif r < 0.86:
        depth = _skewed(rnd, 1, 8000)
        b = b"[" * depth + b"1" + b"]" * depth
    elif r < 0.90:
        b = b'{"msg":"x"}'
    elif r < 0.93:
        b = rnd.choice((
            b"null", b"true", b"[]", b"{}", b" ",
            b"\xef\xbb\xbf{\"a\":1}",
            b"<actions/>",
            b"---\nactions: []\n",
            b"{\"a\":\"\xff\xfe\"}",
            b"{\"actions\":",
        ))
    else:
        b = b""
    if b and rnd.random() < 0.3:
        i = rnd.randrange(len(b))
        b = b[:i] + bytes([rnd.randrange(256)]) + b[i + 1:]
    if rnd.random() < 0.05 and b:
        b = b"\xef\xbb\xbf" + b
    if len(b) > _MAX_BODY:
        b = b[:_MAX_BODY]
    return b


def _hostile(rnd):
    path = rnd.choice(_PATHS)
    query = None
    if rnd.random() < 0.3:
        query = rnd.choice(_QUERIES)
        if rnd.random() < 0.5:
            path = path + "?" + query
    method = rnd.choice(_METHODS)
    body = _body(rnd)
    overrides = {
        "CONTENT_TYPE": rnd.choice(_CTYPES),
        "HTTP_ACCEPT": rnd.choice(_ACCEPTS),
    }
    if rnd.random() < 0.5:
        overrides["HTTP_CONTENT_TYPE"] = overrides["CONTENT_TYPE"]
    elif rnd.random() < 0.5:
        overrides["HTTP_CONTENT_TYPE"] = rnd.choice(_CTYPES)

    if query is not None:
        if rnd.random() < 0.7:
            overrides["QUERY_STRING"] = query
        elif rnd.random() < 0.5:
            overrides["QUERY_STRING"] = rnd.choice(_QUERIES)
    elif rnd.random() < 0.2:
        overrides["QUERY_STRING"] = rnd.choice(_QUERIES)

    cl = rnd.random()
    if cl < 0.05:
        overrides["CONTENT_LENGTH"] = "-1"
    elif cl < 0.10:
        overrides["CONTENT_LENGTH"] = "99999999999999999999"
    elif cl < 0.15:
        overrides["CONTENT_LENGTH"] = str(len(body) + rnd.randint(1, 100))
    elif cl < 0.18:
        overrides["CONTENT_LENGTH"] = str(max(0, len(body) - rnd.randint(1, 50)))
    elif cl < 0.22:
        overrides["CONTENT_LENGTH"] = "abc"
    elif cl < 0.25:
        overrides["CONTENT_LENGTH"] = ""
    elif cl < 0.28:
        overrides["CONTENT_LENGTH"] = None
    elif cl < 0.31:
        overrides["CONTENT_LENGTH"] = "12x"
    elif cl < 0.34:
        overrides["CONTENT_LENGTH"] = "+" + str(len(body))
    elif cl < 0.37:
        overrides["CONTENT_LENGTH"] = "0" + str(len(body))

    if rnd.random() < 0.2:
        name = "HTTP_X_AFW_" + "X" * rnd.randint(1, 50)
        overrides[name] = "v" * rnd.randint(0, 20000)

    if rnd.random() < 0.1:
        overrides[rnd.choice(_DROPPABLE)] = None

    if rnd.random() < 0.08:
        overrides["REQUEST_METHOD"] = rnd.choice(_METHODS)

    if rnd.random() < 0.08:
        other = rnd.choice(_PATHS)
        overrides["REQUEST_URI"] = other
        overrides["URI"] = other

    if rnd.random() < 0.08:
        overrides["PATH_INFO"] = path.split("?", 1)[0]

    if rnd.random() < 0.05:
        overrides["IGNORE_URI_PREFIX"] = rnd.choice(("", "/", "/afw", "/data", "/x"))

    if rnd.random() < 0.05:
        overrides["SERVER_PROTOCOL"] = rnd.choice((
            "HTTP/1.1", "HTTP/1.0", "HTTP/2", "FTP", "", "HTTP/1.1 "))

    return {
        "path": path,
        "method": method,
        "body": body,
        "overrides": overrides,
    }


def build(rnd):
    if rnd.random() < 0.25:
        return _valid(rnd)
    return _hostile(rnd)
