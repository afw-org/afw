#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Python mode: a run() that raises is a failed test, not a hang.

run_test() captured stdout/stderr through pipes with drain threads.
After an exception the writers stayed open, so closing a reader waited
on its blocked drain thread forever.
"""

import os
import tempfile
import threading

from _afwdev.test.modes import python as python_mode

_TIMEOUT_S = 30


def run():
    description = "Python mode: run() that raises"
    outcome = {}

    with tempfile.TemporaryDirectory(prefix="afw_python_run_raises_") as d:
        test = os.path.join(d, "raises.py")
        with open(test, "w", encoding="utf-8") as f:
            f.write("def run():\n    raise ImportError('deliberate')\n")

        def call():
            outcome["result"] = python_mode.run_test(test, {})

        t = threading.Thread(target=call, daemon=True)
        t.start()
        t.join(timeout=_TIMEOUT_S)
        returned = not t.is_alive()

    response, error, _debug = outcome.get("result", (None, None, None))
    return {
        "description": description,
        "tests": [
            {
                "test": "returns",
                "description": "run_test returns within {}s".format(
                    _TIMEOUT_S),
                "passed": returned,
                "skip": False,
            },
            {
                "test": "reports-error",
                "description": "the exception comes back as the error",
                "passed": (
                    returned and response is None and error is not None
                    and "deliberate" in str(error)),
                "skip": False,
            },
        ],
    }
