#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Runner outcome flags: passed / skip must be bool so the mark and the
count cannot disagree.
"""

from _afwdev.test.common import parse_test_run


def run():
    description = "Harness coerces passed/skip so mark and count agree"
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

    return {
        "description": description,
        "tests": tests,
    }
