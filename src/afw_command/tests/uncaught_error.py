#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
afw prints an error that ends a script, expression, or test_script and
releases it before the environment goes away.

The error owns a copy of what it points to. afw's evaluate and main
return from AFW_FINALLY, which used to skip the release in AFW_ENDTRY,
so every uncaught error leaked that block. Under --env-mode asan, afw
is the ASan build and LeakSanitizer reports it.
"""

import os
import subprocess
import tempfile


def _case(name, description, passed, error=None):
    return {
        "test": name,
        "description": description,
        "passed": bool(passed),
        "skip": False,
        "error": error,
    }


def _run(args, source=None):
    path = None
    try:
        if source is not None:
            with tempfile.NamedTemporaryFile(
                    "w", suffix=".as", delete=False) as tf:
                tf.write(source)
                path = tf.name
            args = args + [path]
        return subprocess.run(
            ["afw"] + args,
            stdin=subprocess.DEVNULL,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            timeout=60,
        )
    finally:
        if path:
            os.unlink(path)


def _check(name, description, r):
    out = (r.stdout or b"").decode("utf-8", "replace") + \
        (r.stderr or b"").decode("utf-8", "replace")
    ok = "--- Error ---" in out and "Sanitizer" not in out and \
        r.returncode >= 0
    return _case(name, description, ok,
        None if ok else "returncode={} output={!r}".format(
            r.returncode, out[-1500:]))


def run():
    tests = []
    tests.append(_check("script-syntax-error",
        "script that does not compile",
        _run(["-s", "script"], "let x = ;\n")))
    tests.append(_check("script-throw",
        "script that throws",
        _run(["-s", "script"], 'throw "boom" data { a: [1, 2] };\n')))
    tests.append(_check("expression-error",
        "-x expression that throws",
        _run(["-x", 'error("boom")'])))
    tests.append(_check("test-script-bad-shebang",
        "test_script whose shebang is not a test_script",
        _run(["-s", "test_script"], "#!/bin/sh\nreturn 1;\n")))
    return {
        "description": "afw releases an uncaught error",
        "tests": tests,
    }
