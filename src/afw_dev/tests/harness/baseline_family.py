#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Baselines, --compare-to, history retention, and out-of-family checks."""

import json
import os
import shutil
import subprocess
import tempfile
from datetime import datetime, timedelta, timezone

from _afwdev.test import baseline, family
from _afwdev.test.history import file_record


def _git(repo, *args):
    return subprocess.run(
        ["git", "-c", "user.name=t", "-c", "user.email=t@t",
         "-c", "init.defaultBranch=main"] + list(args),
        cwd=repo, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
        text=True, check=True).stdout.strip()


def _commit(repo, name):
    with open(os.path.join(repo, name), "w") as fd:
        fd.write(name)
    _git(repo, "add", name)
    _git(repo, "commit", "-q", "-m", name)
    return _git(repo, "rev-parse", "HEAD")


def _write(hist, when, commit, label=None, dirty=False, mode="afw",
           narrowed=False):
    stamp = when.strftime("%Y-%m-%dT%H%M%S") + "000Z"
    name = "{}-ref-{}-{}.json".format(stamp, label, mode) if label else \
        "{}-{}.json".format(stamp, mode)
    data = {"git": {"commit": commit[:8], "commit_full": commit,
                    "dirty": dirty}, "mode": mode, "files": [],
            "narrowed": narrowed}
    with open(os.path.join(hist, name), "w") as fd:
        json.dump(data, fd, indent=2, sort_keys=True)
    return name


def _baseline_tests(tests):
    repo = tempfile.mkdtemp()
    hist = tempfile.mkdtemp()
    pwd = os.getcwd()
    try:
        _git(repo, "init", "-q")
        c1 = _commit(repo, "c1")
        _git(repo, "checkout", "-q", "-b", "feat-x")
        c2 = _commit(repo, "c2")
        _git(repo, "checkout", "-q", "main")
        _git(repo, "merge", "-q", "--no-ff", "-m", "merge x", "feat-x")
        _git(repo, "checkout", "-q", "-b", "feat-z", c1)
        c5 = _commit(repo, "c5")
        _git(repo, "checkout", "-q", "main")
        _git(repo, "checkout", "-q", "-b", "feat-y")
        c4 = _commit(repo, "c4")
        os.chdir(repo)

        t0 = datetime.now(timezone.utc) - timedelta(hours=10)
        h = lambda n: t0 + timedelta(hours=n)
        landed = _write(hist, h(0), c1)
        bx = _write(hist, h(1), c2, label="baseline-feat-x")
        at_base_dirty = _write(hist, h(2), c2, dirty=True)
        at_base_clean = _write(hist, h(2.5), c2)
        bz = _write(hist, h(3), c5, label="baseline-feat-z")
        own = [_write(hist, h(4 + i), c4) for i in range(3)]
        orphan = _write(hist, datetime.now(timezone.utc) - timedelta(days=40),
                        "deadbeef" * 5)
        hand = _write(hist, h(0.5), c1, label="pre-mgg")
        by_own = _write(hist, h(8), c4, label="baseline-feat-y")

        opts = {"mode": "afw", "history_dir": hist,
                "afwdev_settings": {"test_keep_runs_per_commit": 2}}
        picked = baseline.select_baseline(opts, branch="feat-y")
        tests.append({
            "test": "baseline-select",
            "description":
                "newest baseline HEAD contains, not an unmerged sibling's "
                "and not this branch's own",
            "passed": picked is not None and os.path.basename(picked) == bx,
            "skip": False,
        })

        removed = baseline.prune(opts, picked)
        left = set(os.listdir(hist))
        tests.append({
            "test": "history-prune",
            "description":
                "landed and orphan runs go, and runs past the per-commit cap; "
                "this branch's runs, a dirty run at the baseline commit, "
                "baselines, and hand tags stay",
            "passed": (
                landed not in left
                and at_base_clean not in left
                and orphan not in left
                and own[0] not in left
                and own[1] in left and own[2] in left
                and at_base_dirty in left
                and bx in left and bz in left and by_own in left
                and hand in left
                and removed == 4
            ),
            "skip": False,
        })

        opts["compare_to"] = "last"
        last, _ = baseline.resolve(opts)
        opts["compare_to"] = c5[:7]
        by_commit, _ = baseline.resolve(opts)
        opts["compare_to"] = "pre-mgg"
        by_tag, _ = baseline.resolve(opts)
        opts["compare_to"] = "nothing-like-this"
        try:
            baseline.resolve(opts)
            bad = False
        except ValueError:
            bad = True
        tests.append({
            "test": "compare-to",
            "description":
                "last (the previous run, tagged or not), a commit, a tag; "
                "anything else is an error",
            "passed": (
                os.path.basename(last) == by_own
                and os.path.basename(by_commit) == bz
                and os.path.basename(by_tag) == hand
                and bad
            ),
            "skip": False,
        })

        narrow = _write(hist, h(8.5), c4, narrowed=True)
        tests.append({
            "test": "last-whole-suite",
            "description":
                "with no baseline, the fallback skips a narrowed run",
            "passed": (
                os.path.basename(baseline.last_run(opts)) == narrow
                and os.path.basename(
                    baseline.last_run(opts, whole_suite=True)) == by_own
            ),
            "skip": False,
        })

        newer = _write(hist, h(9), c4, label="baseline-feat-y")
        gone = baseline.replace_own(opts, os.path.join(hist, newer))
        left = set(os.listdir(hist))
        opts2 = {"mode": "afw", "history_dir": hist,
                 "afwdev_settings": {"test_keep_baselines": 2}}
        baseline.prune(opts2)
        left2 = set(os.listdir(hist))
        tests.append({
            "test": "baseline-replace-and-keep",
            "description":
                "a new --baseline replaces this branch's older one; only "
                "test_keep_baselines newest baselines stay",
            "passed": (
                gone == 1 and by_own not in left and newer in left
                and newer in left2 and bz in left2 and bx not in left2
                and hand in left2
            ),
            "skip": False,
        })
    finally:
        os.chdir(pwd)
        shutil.rmtree(repo, ignore_errors=True)
        shutil.rmtree(hist, ignore_errors=True)


def _family_tests(tests):
    settings = family.settings_from({})
    rec = lambda p, x=None, cpu=None, failed=0: file_record(
        p, 10, x, 0 if failed else 1, 0, failed, cpu_ms=cpu)
    base = {"files": [
        rec("a.as", 100 * 1024), rec("b.as", 100 * 1024, failed=1),
        rec("small.as", 10 * 1024), rec("gone.as", 1024)]}
    new = [rec("a.as", 130 * 1024), rec("b.as", 300 * 1024),
           rec("small.as", 20 * 1024), rec("new.as", 1024)]
    r = family.evaluate(new, base, settings=settings)
    tests.append({
        "test": "family-memory-matched-only",
        "description":
            "1.3x and +30KB is out of family; a failed test, a 2x growth "
            "under the 16KB floor, and new/gone tests are not",
            "passed": (
            family.memory_paths(r) == ["a.as"]
            and r["matched"] == 3
            and r["new"] == ["new.as"] and r["gone"] == ["gone.as"]
        ),
        "skip": False,
    })

    base = {"files": [rec("t{}.as".format(i), cpu=1000) for i in range(10)]}
    new = [rec("t{}.as".format(i), cpu=1500) for i in range(9)]
    new.append(rec("t9.as", cpu=4000))
    r = family.evaluate(new, base, settings=settings)
    tests.append({
        "test": "family-time-against-median",
        "description":
            "every test 1.5x slower is the run's drift; only the one at "
            "4x stands out",
        "passed": (
            [m["path"] for m in r["time"]] == ["t9.as"]
            and r["time_metric"] == "cpu"
            and abs(r["drift"] - 1.5) < 1e-9
            and not family.memory_paths(r)
        ),
        "skip": False,
    })


def run():
    tests = []
    _baseline_tests(tests)
    _family_tests(tests)
    return {
        "description": "Baselines, --compare-to, retention, out of family",
        "tests": tests,
    }
