#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Orchestration leaf parameters, $name, and --set (#485)."""

from _afwdev.test.orchestrated.load import (
    OrchestrationLoadError, apply_parameters, parse_sets)


def _raises(fn):
    try:
        fn()
    except OrchestrationLoadError:
        return True
    return False


def run():
    tests = []
    raw = {
        "parameters": {
            "seed": 1,
            "count": {"default": 400, "valgrind": 50,
                      "description": "requests"},
            "on": {"default": False},
        },
        "schedule": [{"firehose": {
            "seed": "$seed", "maxRequests": "$count", "flag": "$on",
            "text": "keep $seed inside text", "source": "$x is not ${x}",
        }}],
        "tests": [{"name": "t", "parameters": {"kept": True}}],
    }
    marker = "/x/leafname/orchestration.yaml"
    d = apply_parameters(raw, marker, [], "afw")
    fh = d["schedule"][0]["firehose"]
    tests.append({
        "test": "parameters-defaults",
        "description":
            "a whole $name value takes the default (per --env-mode when "
            "given); text containing $ is left alone; nested keys named "
            "parameters stay",
        "passed": (
            fh["seed"] == 1 and fh["maxRequests"] == 400
            and fh["flag"] is False
            and fh["text"] == "keep $seed inside text"
            and d["tests"][0]["parameters"] == {"kept": True}
            and apply_parameters(raw, marker, [], "valgrind")[
                "schedule"][0]["firehose"]["maxRequests"] == 50
        ),
        "skip": False,
    })
    sets = parse_sets(["seed=7", "leafname:count=12", "other:count=99",
                       "on=true", "unknown=1"])
    fh = apply_parameters(raw, marker, sets, "valgrind")[
        "schedule"][0]["firehose"]
    tests.append({
        "test": "parameters-set",
        "description":
            "--set NAME=VALUE for every leaf, LEAF:NAME=VALUE for one; "
            "values take the default's type; a name a leaf lacks is "
            "ignored unless the leaf is named",
        "passed": (
            fh["seed"] == 7 and fh["maxRequests"] == 12
            and fh["flag"] is True
        ),
        "skip": False,
    })
    tests.append({
        "test": "parameters-errors",
        "description":
            "an undeclared $name, a named leaf without the parameter, a "
            "value of the wrong type, and a set without = are errors",
        "passed": (
            _raises(lambda: apply_parameters(
                {"parameters": {}, "a": "$nope"}, marker, [], "afw"))
            and _raises(lambda: apply_parameters(
                raw, marker, parse_sets(["leafname:nope=1"]), "afw"))
            and _raises(lambda: apply_parameters(
                raw, marker, parse_sets(["seed=abc"]), "afw"))
            and _raises(lambda: parse_sets(["seed"]))
            and apply_parameters(raw, marker, parse_sets(["seed=none"]),
                                 "afw")["schedule"][0]["firehose"][
                                     "seed"] is None
        ),
        "skip": False,
    })
    return {
        "description": "Orchestration leaf parameters and --set",
        "tests": tests,
    }
