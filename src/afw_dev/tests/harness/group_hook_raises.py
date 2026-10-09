#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
A test group whose config.py hook raises fails that group, not the run.

The exception used to reach runner.run(): with -j the pool was
terminated, serially afwdev exited, either way with one "Test runner
caught Exception" line and no summary. Seen when an LMDB group's
before_each seed (subprocess.run(check=True)) failed under
--env-mode tsan. --bail still stops the run.
"""

import contextlib
import io
import os
import tempfile

from _afwdev.test import runner


def _case(name, description, passed, error=None):
    return {
        "test": name,
        "description": description,
        "passed": bool(passed),
        "skip": False,
        "error": error,
    }


def _group(work, name, before_each_body):
    root = os.path.join(work, name)
    os.makedirs(root)
    with open(os.path.join(root, "config.py"), "w", encoding="utf-8") as f:
        f.write("def before_each():\n    " + before_each_body + "\n")
    test = os.path.join(root, "never_runs.as")
    with open(test, "w", encoding="utf-8") as f:
        f.write("")
    return ("afw_dev", root, [test])


def _run_guarded(group, work):
    with contextlib.redirect_stdout(io.StringIO()), \
            contextlib.redirect_stderr(io.StringIO()):
        return runner._run_test_group_guarded(
            group, {}, [], os.path.join(work, "run"))


def run():
    tests = []
    with tempfile.TemporaryDirectory(prefix="afw_group_hook_raises_") as work:
        pwd = os.getcwd()

        raised = _run_guarded(
            _group(work, "raises", "raise ValueError('deliberate')"), work)
        detail = (raised[4] or [{}])[0].get("detail", "")
        tests.append(_case(
            "raise-fails-group",
            "a before_each that raises is one failure of its group",
            passed=(raised[1:4] == (0, 0, 1)
                and "ValueError: deliberate" in detail),
            error=str(raised[1:5]),
        ))
        tests.append(_case(
            "cwd-restored",
            "the working directory is restored after the failure",
            passed=os.getcwd() == pwd,
            error=os.getcwd(),
        ))

        exited = _run_guarded(
            _group(work, "exits", "import sys; sys.exit(3)"), work)
        tests.append(_case(
            "exit-fails-group",
            "a before_each that calls sys.exit() still fails its group",
            passed=(exited[1:4] == (0, 0, 1)
                and "sys.exit(3)" in (exited[4] or [{}])[0].get(
                    "detail", "")),
            error=str(exited[1:5]),
        ))

        body = runner._run_test_group_body

        def bail(*_args):
            raise runner._BailError("Bailing due to test failure")

        runner._run_test_group_body = bail
        try:
            _run_guarded(("afw_dev", work, []), work)
            bailed = False
        except runner._BailError:
            bailed = True
        finally:
            runner._run_test_group_body = body
        tests.append(_case(
            "bail-stops-run",
            "--bail is not caught per group: it still stops the run",
            passed=bailed,
        ))

    return {
        "description": "A raising test-group hook fails its group",
        "tests": tests,
    }
