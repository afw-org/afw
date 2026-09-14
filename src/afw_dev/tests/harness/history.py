#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""History compare/trend: path match, optional k, thresholds."""

from _afwdev.test.history import (
    compare_runs, file_record, trend_runs, _k_fatter, K_FLOOR_BYTES,
)


def _run(files, mode="afw", commit="abc"):
    return {
        "mode": mode,
        "git": {"commit": commit, "branch": "develop", "dirty": False},
        "files": files,
        "max_xctx_kbytes": 0,
    }


def run():
    description = "Test history compare and trend"
    tests = []

    small = file_record("a.as", 50, 10 * 1024, 1, 0, 0)
    huge = file_record("a.as", 50, 200 * 1024, 1, 0, 0)
    tests.append({
        "test": "k-fatter-threshold",
        "description": "10k → 200k is fatter; 10k → 12k is not",
        "passed": (
            _k_fatter(small, huge) is True
            and _k_fatter(small, file_record("a.as", 50, 12 * 1024, 1, 0, 0))
            is False
        ),
        "skip": False,
    })

    old = _run([
        file_record("keep.as", 40, 40 * 1024, 2, 0, 0),
        file_record("gone.as", 10, 8 * 1024, 1, 0, 0),
        file_record("fail.as", 10, 8 * 1024, 0, 0, 1),
        file_record("nok.as", 10, None, 1, 0, 0),
    ], commit="old")
    new = _run([
        file_record("keep.as", 40, 200 * 1024, 2, 0, 0),
        file_record("new.as", 10, 8 * 1024, 1, 0, 0),
        file_record("fail.as", 10, 400 * 1024, 0, 0, 1),
        file_record("nok.as", 10, None, 1, 0, 0),
    ], commit="new")
    # old keep 40k, new keep 200k → fatter (5× and +160k)
    r = compare_runs(old, new)
    tests.append({
        "test": "compare-new-gone",
        "description": "new/gone/compared by path",
        "passed": (
            r["added"] == ["new.as"]
            and r["gone"] == ["gone.as"]
            and "keep.as" in r["compared"]
            and "fail.as" in r["compared"]
            and "nok.as" in r["compared"]
        ),
        "skip": False,
    })
    tests.append({
        "test": "compare-fatter-skips-failed",
        "description": "failed paths are not k-flagged; keep.as is fatter",
        "passed": (
            r["fatter"] == ["keep.as"]
            and "fail.as" not in r["fatter"]
        ),
        "skip": False,
    })
    tests.append({
        "test": "compare-k-missing-counted",
        "description": "nok.as has no k on new; keep has k on both",
        "passed": r["k_missing"] >= 1,
        "skip": False,
    })

    t1 = _run([file_record("a.as", 10, 20 * 1024, 1, 0, 0)], commit="1")
    t2 = _run([file_record("a.as", 10, 40 * 1024, 1, 0, 0)], commit="2")
    t3 = _run([
        file_record("a.as", 10, 80 * 1024, 1, 0, 0),
        file_record("b.as", 10, 8 * 1024, 1, 0, 0),
    ], commit="3")
    tr = trend_runs([t1, t2, t3], {})
    tests.append({
        "test": "trend-new-and-movers",
        "description": "b.as is new; a.as 20k→80k is a mover",
        "passed": (
            tr["new"] == ["b.as"]
            and tr["gone"] == []
            and tr["movers"]
            and tr["movers"][0]["path"] == "a.as"
            and tr["movers"][0]["first"] == 20
            and tr["movers"][0]["last"] == 80
        ),
        "skip": False,
    })

    tests.append({
        "test": "floor-constant",
        "description": "32k floor is the agreed k delta",
        "passed": K_FLOOR_BYTES == 32 * 1024,
        "skip": False,
    })

    return {
        "description": description,
        "tests": tests,
    }
