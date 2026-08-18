#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
afwdev prime-test-c-probe writes an empty test leaf from the closet.
"""

import os
import shutil
import subprocess
import tempfile

from _afwdev.test.c_probe import run_c_probe


def _package_root():
    d = os.path.dirname(os.path.abspath(__file__))
    while d != "/" and not os.path.isfile(os.path.join(d, "afw-package.json")):
        d = os.path.dirname(d)
    return d


def _afwdev():
    return os.path.join(_package_root(), "src", "afw_dev", "afwdev.py")


def _run(args, cwd):
    return subprocess.run(
        [_afwdev()] + args, cwd=cwd, capture_output=True, text=True,
        timeout=120)


def run():
    description = "afwdev prime-test-c-probe writes a C probe test leaf"
    tests = []
    work = tempfile.mkdtemp(prefix="afw_prime_c_probe_")
    pkg = os.path.join(work, "pkg")
    try:
        r = _run(
            ["--noprompt", "make-afw-package", "pkg", pkg],
            work)
        if r.returncode != 0:
            return {
                "description": description,
                "tests": [{
                    "test": "make-package",
                    "description": "make-afw-package for prime-test-c-probe",
                    "passed": False,
                    "skip": False,
                    "error": (r.stderr or r.stdout or "exit %s" % r.returncode),
                }],
            }

        r = _run(
            ["--noprompt", "make-extension", "myext"],
            pkg)
        if r.returncode != 0:
            return {
                "description": description,
                "tests": [{
                    "test": "make-extension",
                    "description": "make-extension myext",
                    "passed": False,
                    "skip": False,
                    "error": (r.stderr or r.stdout or "exit %s" % r.returncode),
                }],
            }

        r = _run(
            ["--noprompt", "prime-test-c-probe", "src/myext/tests/hole"],
            pkg)
        leaf = os.path.join(pkg, "src", "myext", "tests", "hole")
        c_path = os.path.join(leaf, "hole_probe.c")
        py_path = os.path.join(leaf, "hole.py")
        c_text = ""
        py_text = ""
        if os.path.isfile(c_path):
            with open(c_path, encoding="utf-8") as f:
                c_text = f.read()
        if os.path.isfile(py_path):
            with open(py_path, encoding="utf-8") as f:
                py_text = f.read()
        tests.append({
            "test": "writes-leaf",
            "description": "creates tests/hole/hole_probe.c and hole.py",
            "passed": (
                r.returncode == 0
                and os.path.isfile(c_path)
                and os.path.isfile(py_path)
                and "hole_probe" in c_text
                and "skeleton_probe" not in c_text
                and "hole_probe.c" in py_text
            ),
            "skip": False,
            "error": None if r.returncode == 0 else (
                r.stderr or r.stdout or "exit %s" % r.returncode),
        })

        # Same idea as commands_test1.txt: after priming, the closet
        # must compile. The probe is not a cmake target, so run_c_probe
        # is the build.
        compiled = {"tests": []}
        if os.path.isfile(c_path):
            compiled = run_c_probe(
                c_path,
                "primed closet compiles",
                [("ok", "environment boots")],
                valgrind=False,
            )
        compile_one = (compiled.get("tests") or [{}])[0]
        tests.append({
            "test": "closet-compiles",
            "description": "primed ok case compiles and runs against libafw",
            "passed": compile_one.get("passed") is True,
            "skip": False,
            "error": compile_one.get("error"),
        })

        r2 = _run(
            ["--noprompt", "prime-test-c-probe", "src/myext/tests/hole"],
            pkg)
        err2 = (r2.stderr or r2.stdout or "")
        tests.append({
            "test": "refuses-existing",
            "description": "second prime of the same leaf fails",
            "passed": r2.returncode != 0 and (
                "empty" in err2 or "Already exists" in err2),
            "skip": False,
            "error": None if r2.returncode != 0 else "expected failure",
        })

        r3 = _run(
            ["--noprompt", "prime-test-c-probe", "src/myext/tests"],
            pkg)
        err3 = (r3.stderr or r3.stdout or "")
        tests.append({
            "test": "refuses-tests-root",
            "description": "tests/ itself is not a leaf",
            "passed": r3.returncode != 0 and "leaf" in err3,
            "skip": False,
            "error": None if r3.returncode != 0 else "expected failure",
        })

        r4 = _run(
            ["--noprompt", "prime-test-c-probe", "src/myext/not_tests/hole"],
            pkg)
        err4 = (r4.stderr or r4.stdout or "")
        tests.append({
            "test": "refuses-outside-tests",
            "description": "path must be under tests/ or tests-extra/",
            "passed": r4.returncode != 0 and "tests/" in err4,
            "skip": False,
            "error": None if r4.returncode != 0 else "expected failure",
        })

        ext = os.path.join(pkg, "src", "myext")
        r5 = _run(
            ["--noprompt", "prime-test-c-probe", "tests/fromcwd"],
            ext)
        fromcwd = os.path.join(ext, "tests", "fromcwd", "fromcwd_probe.c")
        tests.append({
            "test": "cwd-relative",
            "description": "path is relative to the current directory",
            "passed": r5.returncode == 0 and os.path.isfile(fromcwd),
            "skip": False,
            "error": None if r5.returncode == 0 else (
                r5.stderr or r5.stdout or "exit %s" % r5.returncode),
        })

        r6 = _run(
            ["--noprompt", "prime-test-c-probe",
             "src/myext/tests/advanced/other"],
            pkg)
        other = os.path.join(
            pkg, "src", "myext", "tests", "advanced", "other",
            "other_probe.c")
        tests.append({
            "test": "nested-leaf",
            "description": "tests/advanced/other is a nested leaf",
            "passed": r6.returncode == 0 and os.path.isfile(other),
            "skip": False,
            "error": None if r6.returncode == 0 else (
                r6.stderr or r6.stdout or "exit %s" % r6.returncode),
        })
    finally:
        shutil.rmtree(work, ignore_errors=True)

    return {
        "description": description,
        "tests": tests,
    }
