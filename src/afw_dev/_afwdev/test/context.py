#! /usr/bin/env python3

##
# @file context.py
# @ingroup afwdev_test
# @brief Per-run context for Python-mode tests (options, paths).
# @details python mode pushes this before calling run(). Helpers such as
#          c_probe read it so --env-mode valgrind can wrap a compiled
#          probe without changing every run() signature.
#

import threading

_state = threading.local()


def push(options=None, test=None, testEnvironment=None, testGroupConfig=None):
    """Push a run context for the current thread."""
    stack = getattr(_state, "stack", None)
    if stack is None:
        stack = []
        _state.stack = stack
    stack.append({
        "options": options or {},
        "test": test,
        "testEnvironment": testEnvironment,
        "testGroupConfig": testGroupConfig,
    })


def pop():
    """Pop the current run context, if any."""
    stack = getattr(_state, "stack", None)
    if stack:
        stack.pop()


def current():
    """Return the current run context dict, or empty."""
    stack = getattr(_state, "stack", None)
    if stack:
        return stack[-1]
    return {}


def options():
    """Return options from the current context, or empty dict."""
    return current().get("options") or {}
