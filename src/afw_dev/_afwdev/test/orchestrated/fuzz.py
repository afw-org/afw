#! /usr/bin/env python3
##
# @file fuzz.py
# @brief Fuzz sources for an orchestrated firehose (#485 part B).
#
# A firehose step with a fuzz: block sends generated requests instead of
# (or as well as) fromTests:
#
#   - firehose:
#       maxRequests: 2000
#       seed: 7
#       fuzz:
#         kind: functionCalls
#         callsPerRequest: 25
#         exclude: [some_function*]
#
# functionCalls: each request is a script of try-wrapped calls to built-in
# functions with odd arguments. The function list comes from the server
# under test (its _AdaptiveFunction_ objects, extensions included), less
# DEFAULT_EXCLUDE and the leaf's exclude patterns (fnmatch).
#
# Request i is built only from (seed, i), so nothing generated is stored
# and `afwdev test -T <leaf> --replay SEED:INDEX` resends exactly it.
#

import fnmatch
import random

# Functions that change server state, write into the HTTP response, or
# return different values each call (which would defeat --replay).
DEFAULT_EXCLUDE = (
    "service_*", "extension_*", "flag_*", "sleep*",
    "eval_from_file*", "compile_from_file*", "open_file*",
    "journal_*", "log*", "trace*", "debug*",
    "add_object*", "replace_object*", "modify_object*", "delete_object*",
    "reconcile_object*", "perform*", "authorization_check*",
    "index_create*", "index_remove*", "model_default_*_action*",
    "write*", "stream*", "*_to_response*", "*_to_stream*",
    "random_*", "*<null>",
    # Loops: a fuzzed while(true, ...) runs until the request timeout.
    "while", "do_while", "for", "for_of",
)

REQUEST_TIMEOUT_S = 30

# Script that lists [functionId, parameter count] on the server.
FUNCTION_LIST_SCRIPT = """\
const fs = retrieve_objects("afw", "_AdaptiveFunction_");
let out = [];
for (const f of fs) {
    push(out, [f.functionId,
        f.parameters === undefined ? 0 : length(f.parameters)]);
}
return out;
"""

# Argument values: edges of each type, odd strings, nesting, closures,
# and values made by other functions.
VALUES = (
    'null', 'undefined', 'true', 'false', '0', '-1', '1', '2', '255',
    '#integerMax', '#integerMin', '#doubleMax', '1.5', '-0.0', '1e308',
    '-1e308', '""', '"x"', '"abc"', '" "', '"\\u0000"', '"\\uFFFF"',
    '"\\u{10FFFF}"', '"' + 'a' * 20000 + '"', '"%s%n%x"', '"${x}"',
    '[]', '[1,2,3]', '[[[]]]', '[null,undefined]', '["a",1,true]', '{}',
    '{a:1}', '{a:{b:[1,{c:2}]}}', '{_meta_:{objectType:"x"}}',
    'function (x) { return x; }', 'function (a, b) { return a + b; }',
    'function () { return undefined; }', 'function () { throw "t"; }',
    'date("2020-02-29")', 'date("-9999-01-01")',
    'dateTime("2020-01-01T00:00:00Z")', 'time("23:59:60")',
    'dayTimeDuration("P1D")', 'dayTimeDuration("-P99999999DT1S")',
    'yearMonthDuration("P1Y")', 'yearMonthDuration("-P9999999999Y")',
    'base64Binary("AA==")', 'base64Binary("")', 'hexBinary("00ff")',
    'hexBinary("")', 'anyURI("http://x/y?z#w")', 'anyURI("")',
    'x500Name("cn=a")', 'rfc822Name("a@b")', 'ipAddress("1.2.3.4")',
    'dnsName("a.b")', 'ia5String("x")', 'script("return 1;")',
    'template("a ${1+1}")', 'regexp("(a+)+$")', 'regexp("[")',
    'object()', 'array()',
    'get_object("afw","_AdaptiveProcess_","current")',
    'retrieve_objects("afw","_AdaptiveDataType_")',
    'current::x', 'integer("12")', 'double("NaN")', 'double("INF")',
    'string(1)', 'xpathExpression("/a/b")', 'compile<script>("return 1;")',
    'decompile(1)',
)


class FuzzError(ValueError):
    pass


def parse_replay(text):
    """'SEED:INDEX' or 'SEED:FIRST-LAST' -> (seed, first, last)."""
    try:
        seed_s, rest = str(text).split(":", 1)
        if "-" in rest:
            first_s, last_s = rest.split("-", 1)
        else:
            first_s = last_s = rest
        seed, first, last = int(seed_s), int(first_s), int(last_s)
    except ValueError:
        raise FuzzError(
            "--replay wants SEED:INDEX or SEED:FIRST-LAST, got {!r}".format(
                text))
    if first < 0 or last < first:
        raise FuzzError("--replay range {!r} is empty".format(text))
    return seed, first, last


class FunctionCalls(object):
    """kind: functionCalls. Picklable; workers rebuild requests."""

    needs_functions = True
    source_suffix = ".as"

    def __init__(self, spec, functions, seed):
        self.seed = int(seed or 0)
        self.calls = int(spec.get("callsPerRequest") or 25)
        if self.calls < 1:
            raise FuzzError("fuzz.callsPerRequest must be at least 1")
        # A request that takes this long is a finding (a hang), named
        # like any other failure.
        self.request_timeout = float(
            spec.get("requestTimeout_s") or REQUEST_TIMEOUT_S)
        patterns = tuple(DEFAULT_EXCLUDE) + tuple(spec.get("exclude") or ())
        self.functions = sorted(
            (fid, int(n)) for fid, n in functions
            if not any(fnmatch.fnmatchcase(fid, p) for p in patterns))
        if not self.functions:
            raise FuzzError("fuzz: no functions left after exclude")

    def name(self, index):
        return "fuzz {}:{}".format(self.seed, index)

    def source(self, index):
        # Request i depends only on (seed, i). Integer seed: stable across
        # Python runs, unlike hash().
        rnd = random.Random(self.seed * 1000003 + index)
        # The calls run inside a function, so a fuzzed return() or
        # break() leaves the function, not the script: the script's own
        # result is always true.
        lines = ["const fuzz = function () {"]
        for i in range(self.calls):
            lines.append(
                "    try {{ let r{} = {}; }} catch (e) {{ }}".format(
                    i, self._call(rnd, rnd.choice(self.functions))))
        lines.append("};")
        lines.append("fuzz();")
        return "\n".join(lines) + "\nreturn true;\n"

    def item(self, index):
        # No expect: a fuzzed return(), break(), or continue() can end the
        # script early with any value. A request fails on an error that
        # escapes its try, an error status, or a server that stops.
        return {
            "name": self.name(index),
            "sourceType": "script",
            "source": self.source(index),
            "fuzzIndex": index,
        }

    def _call(self, rnd, fn, depth=0):
        fid, n = fn
        c = rnd.random()
        if c < 0.1:
            n = max(0, n - 1)
        elif c < 0.2:
            n += 1
        elif c < 0.3:
            n += rnd.randint(2, 6)
        args = ", ".join(self._value(rnd, depth) for _ in range(n))
        return "{}({})".format(fid, args)

    def _value(self, rnd, depth):
        r = rnd.random()
        if depth < 2 and r < 0.08:
            return "[" + ",".join(
                self._value(rnd, depth + 1)
                for _ in range(rnd.randint(0, 4))) + "]"
        if depth < 2 and r < 0.14:
            return "{" + ",".join(
                "k{}:{}".format(i, self._value(rnd, depth + 1))
                for i in range(rnd.randint(0, 3))) + "}"
        if depth < 2 and r < 0.2:
            return self._call(rnd, rnd.choice(self.functions), depth + 1)
        return rnd.choice(VALUES)


class Hostile(object):
    """kind: hostile. Odd FastCGI requests from fuzz_hostile_pieces.build.

    Any HTTP status is a fine answer; see reply_problem().
    """

    needs_functions = False
    source_suffix = ".txt"

    def __init__(self, spec, seed):
        self.seed = int(seed or 0)
        self.request_timeout = float(
            spec.get("requestTimeout_s") or REQUEST_TIMEOUT_S)
        self.functions = []

    def name(self, index):
        return "hostile {}:{}".format(self.seed, index)

    def request(self, index):
        from _afwdev.test.orchestrated import fuzz_hostile_pieces
        rnd = random.Random(self.seed * 1000003 + index)
        req = fuzz_hostile_pieces.build(rnd)
        body = req.get("body") or b""
        if isinstance(body, str):
            body = body.encode("utf-8", "surrogateescape")
        return {
            "path": req.get("path") or "/",
            "method": req.get("method") if req.get("method") is not None
            else "GET",
            "body": body,
            "overrides": dict(req.get("overrides") or {}),
        }

    def item(self, index):
        return {"name": self.name(index), "hostile": self.request(index),
                "fuzzIndex": index}

    def source(self, index):
        """The request as text, for --replay and diag/fuzz-in-flight/."""
        req = self.request(index)
        lines = ["method: {!r}".format(req["method"]),
                 "path: {!r}".format(req["path"])]
        for name in sorted(req["overrides"]):
            lines.append("param {}: {!r}".format(name, req["overrides"][name]))
        body = req["body"]
        shown = body[:2000]
        lines.append("body ({} bytes): {!r}{}".format(
            len(body), shown, " ..." if len(body) > len(shown) else ""))
        return "\n".join(lines) + "\n"


def reply_problem(result):
    """Why an afwfcgi reply is not a well-formed answer, or None.

    result is fcgi_client.fcgi_request's dict. Any HTTP status counts as
    an answer; a missing end record or status line does not.
    """
    if result.get("app_status") is None:
        return "no FastCGI end record"
    if not result.get("stdout_raw"):
        return "empty reply"
    code = result.get("status_code")
    if not isinstance(code, int) or code < 100 or code > 599:
        return "no HTTP status in the reply ({!r})".format(code)
    return None


def make_source(spec, functions, seed):
    kind = (spec or {}).get("kind")
    if kind == "functionCalls":
        return FunctionCalls(spec, functions, seed)
    if kind == "hostile":
        return Hostile(spec, seed)
    raise FuzzError(
        "fuzz.kind must be 'functionCalls' or 'hostile', got {!r}".format(
            kind))
