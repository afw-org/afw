#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Fuzz sources for orchestrated firehose steps (#485 part B)."""

from _afwdev.test.orchestrated import fuzz
from _afwdev.test.orchestrated.runner import _env_mode_body


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
    return {
        "description": "Fuzz sources for orchestrated firehose steps",
        "tests": tests,
    }
