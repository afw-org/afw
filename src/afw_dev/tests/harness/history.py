#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""History compare/trend: path match, optional bytes, thresholds."""

import os
import shutil
import tempfile
import time

from _afwdev.test.common import format_test_timing, format_xctx_bytes
from _afwdev.test import run_dir
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

    root = tempfile.mkdtemp()
    saved_tmpdir = (os.environ.get("TMPDIR"), tempfile.tempdir)
    try:
        opts = {"tmpdir": root, "mode": "afw",
                "afwdev_settings": {"test_keep_runs": 3}}
        runs = run_dir.runs_root(opts)
        os.makedirs(runs)
        base_time = time.time() - 3600
        for i, (name, locked) in enumerate((
                ("0101-000000-afw", False),
                ("0101-000001-afw", True),
                ("0101-000002-afw", False),
                ("0101-000003-afw", False),
                ("0101-000004-valgrind", False))):
            path = os.path.join(runs, name)
            os.mkdir(path)
            if locked:
                with open(os.path.join(path, run_dir.LOCK_NAME), "w") as fd:
                    fd.write(str(os.getpid()))
            stamp = base_time + i
            if name.endswith("valgrind"):
                stamp = base_time - 100
            os.utime(path, (stamp, stamp))
        made = run_dir.create(opts, "afw")
        left = sorted(os.listdir(runs))
        latest = os.path.join(runs, run_dir.LATEST_NAME)
        tests.append({
            "test": "run-dir-create-and-prune",
            "description":
                "keeps the newest test_keep_runs of the mode, counting the "
                "new one; a live run and other modes stay; new run has "
                "lock, tmp/ as TMPDIR, and latest",
            "passed": (
                "0101-000000-afw" not in left
                and "0101-000001-afw" in left
                and "0101-000002-afw" in left
                and "0101-000003-afw" in left
                and "0101-000004-valgrind" in left
                and os.path.basename(made) in left
                and run_dir.in_use(made)
                and os.environ.get("TMPDIR") ==
                    os.path.join(made, run_dir.SCRATCH_NAME)
                and tempfile.gettempdir() ==
                    os.path.join(made, run_dir.SCRATCH_NAME)
                and os.path.realpath(latest) == os.path.realpath(made)
            ),
            "skip": False,
        })
        second = run_dir.create(dict(opts), "afw")
        run_dir.release(opts)
        tests.append({
            "test": "run-dir-parallel-and-release",
            "description":
                "a second run gets its own directory; release drops the lock",
            "passed": (
                second != made
                and not run_dir.in_use(made)
                and os.path.isdir(made)
            ),
            "skip": False,
        })
        for name in ("afwdev_test_output", "afw_req_body_x1",
                     "afw_vector_probe_x2", "afw-take-tests", "afw_subset"):
            os.mkdir(os.path.join(root, name))
        run_dir.clear(opts)
        top = sorted(os.listdir(root))
        left = sorted(os.listdir(runs))
        tests.append({
            "test": "clear-temps",
            "description":
                "unlocked runs and known leftovers go; live runs and "
                "hand-made directories stay",
            "passed": (
                os.path.basename(made) not in left
                and os.path.basename(second) in left
                and "0101-000001-afw" in left
                and "afwdev_test_output" not in top
                and "afw_req_body_x1" not in top
                and "afw_vector_probe_x2" not in top
                and "afw-take-tests" in top
                and "afw_subset" in top
            ),
            "skip": False,
        })
        tests.append({
            "test": "socket-path-limit",
            "description": "a socket path over 107 bytes is reported",
            "passed": (
                run_dir.socket_path_error("/tmp/" + "a" * 102) is None
                and run_dir.socket_path_error("/tmp/" + "a" * 103)
                is not None
            ),
            "skip": False,
        })
    finally:
        if saved_tmpdir[0] is None:
            os.environ.pop("TMPDIR", None)
        else:
            os.environ["TMPDIR"] = saved_tmpdir[0]
        tempfile.tempdir = saved_tmpdir[1]
        shutil.rmtree(root, ignore_errors=True)

    return {
        "description": description,
        "tests": tests,
    }
