#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""History compare/trend: path match, optional bytes, thresholds."""

import os
import tempfile

from _afwdev.test.common import format_test_timing, format_xctx_bytes
from _afwdev.test.failure_log import clear_failures
from _afwdev.test.history import (
    compare_runs, file_record, trend_runs, _bytes_fatter, BYTES_FLOOR,
    history_filename, is_reference_name, select_trend_files,
    clear_history, delete_history_ref, list_history_refs,
    ref_label_from_name,
)


def _run(files, mode="afw", commit="abc"):
    return {
        "mode": mode,
        "git": {"commit": commit, "branch": "develop", "dirty": False},
        "files": files,
        "max_xctx_bytes": 0,
    }


def run():
    description = "Test history compare and trend"
    tests = []

    small = file_record("a.as", 50, 10 * 1024, 1, 0, 0)
    huge = file_record("a.as", 50, 200 * 1024, 1, 0, 0)
    tests.append({
        "test": "bytes-fatter-threshold",
        "description": "10KiB → 200KiB is fatter; 10KiB → 12KiB is not",
        "passed": (
            _bytes_fatter(small, huge) is True
            and _bytes_fatter(
                small, file_record("a.as", 50, 12 * 1024, 1, 0, 0))
            is False
        ),
        "skip": False,
    })

    rec = file_record("a.as", 50, 10 * 1024, 1, 0, 0, xctx_chunk_bytes=64 * 1024)
    tests.append({
        "test": "file-record-chunk-bytes",
        "description": "file_record stores xctx_chunk_bytes",
        "passed": rec.get("xctx_chunk_bytes") == 64 * 1024,
        "skip": False,
    })

    tests.append({
        "test": "format-xctx-bytes-commas",
        "description": "console memory uses comma-separated max N xctx",
        "passed": (
            format_xctx_bytes(195097776) == "195,097,776"
            and format_test_timing(58, 12288) == "(58ms, max 12,288 xctx)"
            and format_test_timing(58, 12288, 16384) ==
                "(58ms, max 12,288 xctx, 16,384 chunk)"
            and format_test_timing(58) == "(58ms)"
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
    # old keep 40KiB, new keep 200KiB → fatter (5× and +160KiB)
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
        "description": "failed paths are not bytes-flagged; keep.as is fatter",
        "passed": (
            r["fatter"] == ["keep.as"]
            and "fail.as" not in r["fatter"]
        ),
        "skip": False,
    })
    tests.append({
        "test": "compare-bytes-missing-counted",
        "description": "nok.as has no bytes on new; keep has bytes on both",
        "passed": r["bytes_missing"] >= 1,
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
        "description": "b.as is new; a.as 20480→81920 bytes is a mover",
        "passed": (
            tr["new"] == ["b.as"]
            and tr["gone"] == []
            and tr["movers"]
            and tr["movers"][0]["path"] == "a.as"
            and tr["movers"][0]["first"] == 20 * 1024
            and tr["movers"][0]["last"] == 80 * 1024
            and tr["metric"] == "bytes"
        ),
        "skip": False,
    })

    tests.append({
        "test": "floor-constant",
        "description": "32,768 byte floor is the agreed bytes delta",
        "passed": BYTES_FLOOR == 32 * 1024,
        "skip": False,
    })

    ref_name = history_filename("afw", ref_label="pre-mgg")
    tests.append({
        "test": "history-ref-filename",
        "description": "-ref-LABEL- sits before -mode.json",
        "passed": (
            "-ref-pre-mgg-afw.json" in ref_name
            and is_reference_name(ref_name)
            and not is_reference_name("2026-09-14T010203000Z-afw.json")
        ),
        "skip": False,
    })

    tmp = tempfile.mkdtemp()
    try:
        open(os.path.join(tmp, "2026-01-01T000000000Z-ref-pre-mgg-afw.json"), "w").close()
        open(os.path.join(tmp, "2026-02-01T000000000Z-afw.json"), "w").close()
        open(os.path.join(tmp, "2026-03-01T000000000Z-afw.json"), "w").close()
        open(os.path.join(tmp, "latest-afw.json"), "w").close()
        selected = [os.path.basename(p) for p in select_trend_files(tmp, "afw", 1)]
        tests.append({
            "test": "trend-keeps-refs-plus-last-n",
            "description": "refs never age out; last 1 ordinary run is kept",
            "passed": (
                "2026-01-01T000000000Z-ref-pre-mgg-afw.json" in selected
                and "2026-03-01T000000000Z-afw.json" in selected
                and "2026-02-01T000000000Z-afw.json" not in selected
                and "latest-afw.json" not in selected
            ),
            "skip": False,
        })
    finally:
        for name in os.listdir(tmp):
            os.remove(os.path.join(tmp, name))
        os.rmdir(tmp)

    ref = _run([file_record("a.as", 10, 20 * 1024, 1, 0, 0)], commit="ref")
    ref["reference"] = True
    ref["label"] = "pre-mgg"
    later = _run([
        file_record("a.as", 10, 80 * 1024, 1, 0, 0),
        file_record("new.as", 10, 8 * 1024, 1, 0, 0),
    ], commit="later")
    tr_ref = trend_runs([ref, later], {})
    tests.append({
        "test": "trend-peer-oldest-ref",
        "description": "new/gone and movers vs oldest reference, not a later first",
        "passed": (
            tr_ref["peer_label"] == "pre-mgg"
            and tr_ref["new"] == ["new.as"]
            and tr_ref["peer_ms"][0]["ms"] == 10
            and tr_ref["peer_ms"][1]["n"] == 1
        ),
        "skip": False,
    })

    tests.append({
        "test": "ref-label-from-name",
        "description": "label is the -ref- segment before -mode.json",
        "passed": (
            ref_label_from_name(ref_name, "afw") == "pre-mgg"
            and ref_label_from_name(
                "2026-09-14T010203000Z-afw.json", "afw") is None
        ),
        "skip": False,
    })

    house = tempfile.mkdtemp()
    try:
        names = [
            "2026-01-01T000000000Z-ref-old-afw.json",
            "2026-02-01T000000000Z-afw.json",
            "2026-03-01T000000000Z-ref-thread-inf-afw.json",
            "2026-04-01T000000000Z-afw.json",
            "2026-05-01T000000000Z-afw.json",
            "2026-06-01T000000000Z-ref-other-afw.json",
            "2026-04-01T000000000Z-valgrind.json",
        ]
        for name in names:
            open(os.path.join(house, name), "w").close()
        os.symlink(
            "2026-02-01T000000000Z-afw.json",
            os.path.join(house, "latest-afw.json"))
        opts = {"mode": "afw", "history_dir": house}
        picked = [
            os.path.basename(p)
            for p in select_trend_files(house, "afw", 10, "thread-inf")
        ]
        tests.append({
            "test": "trend-one-ref-and-later",
            "description": "one label plus ordinary runs after that reference",
            "passed": (
                picked == [
                    "2026-03-01T000000000Z-ref-thread-inf-afw.json",
                    "2026-04-01T000000000Z-afw.json",
                    "2026-05-01T000000000Z-afw.json",
                ]
            ),
            "skip": False,
        })
        removed = clear_history(opts)
        left = sorted(os.listdir(house))
        tests.append({
            "test": "clear-history-keeps-refs",
            "description": "ordinary runs and a dangling latest go; refs stay",
            "passed": (
                removed == 4
                and "2026-02-01T000000000Z-afw.json" not in left
                and "2026-04-01T000000000Z-afw.json" not in left
                and "2026-05-01T000000000Z-afw.json" not in left
                and "latest-afw.json" not in left
                and "2026-01-01T000000000Z-ref-old-afw.json" in left
                and "2026-03-01T000000000Z-ref-thread-inf-afw.json" in left
                and "2026-06-01T000000000Z-ref-other-afw.json" in left
                and "2026-04-01T000000000Z-valgrind.json" in left
            ),
            "skip": False,
        })
        labels = list_history_refs(opts)
        tests.append({
            "test": "list-history-refs",
            "description": "labels for this mode, in timestamp order",
            "passed": labels == ["old", "thread-inf", "other"],
            "skip": False,
        })
        gone = delete_history_ref(
            {"mode": "afw", "history_dir": house,
             "delete_history_ref": "thread-inf"})
        left = sorted(os.listdir(house))
        tests.append({
            "test": "delete-history-ref",
            "description": "one label is removed; other refs and modes stay",
            "passed": (
                gone == 1
                and "2026-03-01T000000000Z-ref-thread-inf-afw.json" not in left
                and "2026-01-01T000000000Z-ref-old-afw.json" in left
                and "2026-06-01T000000000Z-ref-other-afw.json" in left
                and "2026-04-01T000000000Z-valgrind.json" in left
            ),
            "skip": False,
        })
    finally:
        for name in os.listdir(house):
            os.remove(os.path.join(house, name))
        os.rmdir(house)

    fails = tempfile.mkdtemp()
    try:
        for name in (
            "2026-01-01T000000000Z-afw.log",
            "2026-01-01T000000000Z-afw.log.state.json",
            "2026-01-01T000000000Z-valgrind.log",
            "2026-01-01T000000000Z-valgrind.log.state.json",
            "notes.txt",
        ):
            open(os.path.join(fails, name), "w").close()
        nfail = clear_failures({"mode": "afw"}, directory=fails)
        left = sorted(os.listdir(fails))
        tests.append({
            "test": "clear-failures-one-mode",
            "description": "afw logs and state go; valgrind and other files stay",
            "passed": (
                nfail == 2
                and left == [
                    "2026-01-01T000000000Z-valgrind.log",
                    "2026-01-01T000000000Z-valgrind.log.state.json",
                    "notes.txt",
                ]
            ),
            "skip": False,
        })
    finally:
        for name in os.listdir(fails):
            os.remove(os.path.join(fails, name))
        os.rmdir(fails)

    return {
        "description": description,
        "tests": tests,
    }
