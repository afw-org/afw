#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Fuzz sources for orchestrated firehose steps (#485 part B)."""

import hashlib
import os
import socket
import tempfile

from _afwdev.test.orchestrated import fuzz
from _afwdev.test.orchestrated.fcgi_client import FcgiClientError
from _afwdev.test.orchestrated.runner import (
    _env_mode_body, _exit_signature, _is_timeout, _shrink_reduce)

# sha256 of FunctionCalls.source(0..49) for seeds 1 and 7 over FIXED.
# Replays name requests by (seed, index); if this changes, every recorded
# replay means a different request. Change it only on purpose.
FIXED = [["add<integer>", 2], ["concat", 2], ["length", 1],
         ["abs<integer>", 1], ["eq<string>", 2]]
FIXED_DIGEST = (
    "baa7618cbe0692b75f154522bd4e2c61f897215bcf66743e4506aa21aff86512")


def run():
    tests = []
    functions = [["add<integer>", 2], ["concat", 2], ["service_stop", 1],
                 ["while", 2], ["random_integer", 2], ["length", 1],
                 ["skip_me", 1], ["eq<null>", 2]]
    spec = {"kind": "functionCalls", "callsPerRequest": 5,
            "exclude": ["skip_*"]}
    a = fuzz.make_source(spec, functions, 7)
    b = fuzz.make_source(spec, list(reversed(functions)), 7)
    names = [f for f, _n in a.functions]
    tests.append({
        "test": "fuzz-exclude",
        "description":
            "default deny list (state changes, loops, random_*, <null>) and "
            "the leaf's exclude patterns",
        "passed": names == ["add<integer>", "concat", "length"],
        "skip": False,
    })
    tests.append({
        "test": "fuzz-deterministic",
        "description":
            "request i depends only on (seed, i) and the function list, "
            "not its order",
        "passed": (
            a.source(42) == b.source(42)
            and a.source(42) != a.source(43)
            and a.item(42)["name"] == "fuzz 7:42"
            and "expect" not in a.item(42)
            and a.source(42).count("try {") == 5
        ),
        "skip": False,
    })
    bad = []
    for text in ("7", "x:1", "7:5-3", "7:-1"):
        try:
            fuzz.parse_replay(text)
            bad.append(text)
        except fuzz.FuzzError:
            pass
    tests.append({
        "test": "fuzz-replay-parse",
        "description": "SEED:INDEX and SEED:FIRST-LAST; anything else is an error",
        "passed": (
            fuzz.parse_replay("7:42") == (7, 42, 42)
            and fuzz.parse_replay("7:40-45") == (7, 40, 45)
            and not bad
        ),
        "skip": False,
    })
    try:
        fuzz.make_source({"kind": "nope"}, functions, 1)
        kind_ok = False
    except fuzz.FuzzError:
        kind_ok = True
    tests.append({
        "test": "fuzz-kind",
        "description": "an unknown fuzz kind is an error",
        "passed": kind_ok,
        "skip": False,
    })
    body = {"maxRequests": 400, "seed": 1,
            "envModes": {"valgrind": {"maxRequests": 50},
                         "asan": {"skip": True}}}
    v = _env_mode_body(body, {"mode": "valgrind"})
    tests.append({
        "test": "firehose-env-modes",
        "description":
            "envModes replaces step fields for that --env-mode; skip skips",
        "passed": (
            v["maxRequests"] == 50 and v["seed"] == 1
            and _env_mode_body(body, {"mode": "afw"}) is body
            and _env_mode_body(body, {"mode": "asan"}) is None
        ),
        "skip": False,
    })
    h = fuzz.make_source({"kind": "hostile"}, [], 3)
    req = h.request(7)
    tests.append({
        "test": "hostile-request",
        "description":
            "hostile request i depends only on (seed, i); item and replay "
            "text name it",
        "passed": (
            req == h.request(7)
            and isinstance(req["body"], bytes)
            and isinstance(req["overrides"], dict)
            and h.item(7)["name"] == "hostile 3:7"
            and h.item(7)["hostile"] == req
            and "method: " in h.source(7) and "path: " in h.source(7)
            and not h.needs_functions
        ),
        "skip": False,
    })
    ok = {"app_status": 0, "status_code": 404, "stdout_raw": b"Status: 404"}
    tests.append({
        "test": "hostile-reply-judging",
        "description":
            "any HTTP status is an answer; no end record, an empty reply, "
            "or no status is a problem",
        "passed": (
            fuzz.reply_problem(ok) is None
            and fuzz.reply_problem(dict(ok, status_code=500)) is None
            and fuzz.reply_problem(dict(ok, app_status=None)) is not None
            and fuzz.reply_problem(dict(ok, stdout_raw=b"")) is not None
            and fuzz.reply_problem(dict(ok, status_code=None)) is not None
        ),
        "skip": False,
    })
    _shrink_tests(tests)
    return {
        "description": "Fuzz sources for orchestrated firehose steps",
        "tests": tests,
    }


class _Proc(object):
    def __init__(self, code):
        self.returncode = code


def _shrink_tests(tests):
    h = hashlib.sha256()
    for seed in (1, 7):
        fc = fuzz.FunctionCalls({"callsPerRequest": 25}, FIXED, seed)
        for i in range(50):
            h.update(fc.source(i).encode())
    tests.append({
        "test": "functioncalls-stable",
        "description":
            "request (seed, index) still renders the same script, so "
            "recorded replays keep naming the same request",
        "passed": h.hexdigest() == FIXED_DIGEST,
        "skip": False,
    })
    tests.append({
        "test": "replay-shrink-suffix",
        "description": "SEED:INDEX:shrink splits off the shrink request",
        "passed": (
            fuzz.split_shrink("7:42:shrink") == ("7:42", True)
            and fuzz.split_shrink("7:40-45") == ("7:40-45", False)
        ),
        "skip": False,
    })
    fc = fuzz.FunctionCalls({"callsPerRequest": 4}, FIXED, 3)
    start = fc.shrink_start(5)
    smaller = list(fc.shrink_smaller(start))
    tests.append({
        "test": "shrink-functioncalls-smaller",
        "description":
            "smaller candidates try each call alone, drop one call, and "
            "null one argument; the start renders as the request itself",
        "passed": (
            fc.shrink_text(start) == fc.source(5)
            and all(len(c) == 1 for c in smaller[:4])
            and all(len(c) == 3 for c in smaller[4:8])
            and all(len(c) == 4 for c in smaller[8:])
            and fc.shrink_item(start, "x")["source"] == fc.source(5)
        ),
        "skip": False,
    })
    hreq = {"path": "/afw", "method": "PUT", "body": b"abcd",
            "overrides": {"A": "1", "B": None}}
    hs = list(fuzz.make_source({"kind": "hostile"}, [], 1)
              .shrink_smaller(hreq))
    tests.append({
        "test": "shrink-hostile-smaller",
        "description":
            "smaller candidates drop one override, empty or halve the "
            "body, then try GET, without changing the original",
        "passed": (
            [sorted(c["overrides"]) for c in hs[:2]] == [["B"], ["A"]]
            and [c["body"] for c in hs[2:5]] == [b"", b"ab", b"cd"]
            and hs[5]["method"] == "GET" and len(hs) == 6
            and hreq["overrides"] == {"A": "1", "B": None}
        ),
        "skip": False,
    })

    def drop_one(c):
        for i in range(len(c)):
            yield c[:i] + c[i + 1:]

    got, _n = _shrink_reduce([1, 2, 5, 3, 4], drop_one, lambda c: 5 in c,
                             100)
    pair, _n = _shrink_reduce([1, 2, 5, 3, 7], drop_one,
                              lambda c: 5 in c and 7 in c, 100)
    stuck, n = _shrink_reduce([1, 2, 3], drop_one, lambda c: False, 100)
    capped, n2 = _shrink_reduce(list(range(50)), drop_one, lambda c: True, 7)
    tests.append({
        "test": "shrink-reduce",
        "description":
            "keeps only what still fails (one value, or two together), "
            "leaves a never-failing start whole, and stops at the limit",
        "passed": (
            got == [5] and pair == [5, 7] and stuck == [1, 2, 3]
            and n == 3 and n2 == 7
        ),
        "skip": False,
    })
    try:
        try:
            raise socket.timeout("timed out")
        except socket.timeout as e:
            raise FcgiClientError("Timeout waiting for afwfcgi") from e
    except FcgiClientError as e:
        timed_out = e
    tests.append({
        "test": "shrink-timeout-by-type",
        "description":
            "a timeout is found by the exception it was raised from, not "
            "by words in a message",
        "passed": (
            _is_timeout(timed_out)
            and not _is_timeout(FcgiClientError("Timeout in message only"))
        ),
        "skip": False,
    })
    with tempfile.TemporaryDirectory() as d:
        def sig(code, text):
            path = os.path.join(d, "log")
            with open(path, "w") as fd:
                fd.write(text)
            return _exit_signature({"process": _Proc(code),
                                    "log_path": path})
        a = sig(-11, "x\n==1234==ERROR: AddressSanitizer: SEGV on unknown "
                "address 0x0000000012 (pc 0x7f1)\nSUMMARY: AddressSanitizer:"
                " SEGV f.c:10 in g\n")
        b = sig(-11, "==999==ERROR: AddressSanitizer: SEGV on unknown "
                "address 0x0000000034 (pc 0x7f2)\n")
        c = sig(-11, "==5==ERROR: AddressSanitizer: heap-use-after-free on "
                "0x1\n")
        e = sig(-6, "==5==ERROR: AddressSanitizer: SEGV on 0x1\n")
    tests.append({
        "test": "shrink-exit-signature",
        "description":
            "a crash matches only the same exit code and the same first "
            "sanitizer line, with addresses and pids ignored",
        "passed": a == b and a != c and a != e and a[0] == -11,
        "skip": False,
    })
