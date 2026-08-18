#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Runner outcome flags: passed / skip must be bool so the mark and the
count cannot disagree.
"""

from _afwdev.test.common import parse_test_run, show_all_cases


def run():
    description = "Harness outcome flags, missing tests list, show-all"
    tests = []

    response = {
        "description": "mixed leftovers",
        "tests": [
            {
                "test": "and-trap",
                "description": "truthy leftover string",
                "passed": "compiler error text",
                "skip": False,
            },
            {
                "test": "real-fail",
                "description": "false",
                "passed": False,
                "skip": False,
            },
            {
                "test": "real-pass",
                "description": "true",
                "passed": True,
                "skip": False,
            },
            {
                "test": "skipped",
                "description": "skip leftover",
                "passed": False,
                "skip": "yes",
            },
        ],
    }
    has_failures, all_success, num_failures, num_skipped, num_passed, \
        all_skipped = parse_test_run("fake", {}, response, None)

    tests.append({
        "test": "string-passed-is-bool-true",
        "description": "truthy passed string becomes True (counts as pass)",
        "passed": response["tests"][0]["passed"] is True,
        "skip": False,
    })
    tests.append({
        "test": "counts-agree",
        "description": "2 passed, 1 failed, 1 skipped",
        "passed": (
            num_passed == 2
            and num_failures == 1
            and num_skipped == 1
            and has_failures is True
            and all_success is False
            and all_skipped is False
        ),
        "skip": False,
    })
    tests.append({
        "test": "skip-coerced",
        "description": "non-empty skip string becomes True",
        "passed": response["tests"][3]["skip"] is True,
        "skip": False,
    })

    missing = parse_test_run(
        "fake", {}, {"description": "no tests key"}, None)
    tests.append({
        "test": "missing-tests-is-fail",
        "description": "response without a tests list is a failure, not a crash",
        "passed": (
            missing[0] is True
            and missing[2] == 1
            and missing[4] == 0
        ),
        "skip": False,
    })

    skipped = parse_test_run("fake", {}, None, None)
    tests.append({
        "test": "none-response-is-skip",
        "description": "mode skip (None, None) stays a skip",
        "passed": skipped[5] is True and skipped[2] == 0,
        "skip": False,
    })

    tests.append({
        "test": "pattern-shows-all",
        "description": "a real --test-pattern shows passing cases",
        "passed": (
            show_all_cases({"test-pattern": "c_probe/helper"}) is True
            and show_all_cases({"test-pattern": ".*"}) is False
            and show_all_cases({"show_all": True}) is True
        ),
        "skip": False,
    })

    return {
        "description": description,
        "tests": tests,
    }
