#! /usr/bin/env python3
##
# @file family.py
# @brief Out-of-family tests: this run against a baseline run.
#
# "Out of family" is fuzzy: a flag means "worth a look", not an error.
# Only tests in both runs compare (same path); the others are counted as
# new or gone. A test whose file changed since the baseline (its "sha"
# differs) is counted as changed, not compared: fixes usually add cases
# to an existing test, and that test's memory grows with it (#504). A
# baseline from before sha gets it from git at its commit
# (history.fill_content_hashes); a row still without one compares. A
# test that failed in either run is not flagged.
#
# The check is memory only (xctx and chunk bytes, close to
# deterministic): ratio and floor from afwdev-settings.json, each test
# against the same test in the baseline. It is marked on the test's
# line as soon as the test finishes. A run's numbers are the count of
# tests that tripped.
#
# Time is not compared. A small test's CPU is mostly afw starting up
# (about 19 of 21 ms for test262/keywords.as) and moves with machine
# load and context switching, not with the test. History still keeps
# each test's cpu_ms for --trend cpu.
#

from _afwdev.common import msg
from _afwdev.test.common import format_xctx_bytes

MEMORY_RATIO = 1.2
MEMORY_FLOOR = 16 * 1024
DETAIL_TOP = 20

# Set in the parent before tests start, and in each -j worker by
# init_worker, so workers can mark memory on their test lines.
_context = {"base": None, "label": None, "settings": {}}


def settings_from(options):
    s = (options or {}).get("afwdev_settings") or {}
    return {
        "memory_ratio": _number(s.get("test_family_memory_ratio"),
                                MEMORY_RATIO),
        "memory_floor": _number(s.get("test_family_memory_floor"),
                                MEMORY_FLOOR),
        "strict": bool(s.get("test_family_strict")),
    }


def _number(value, default):
    try:
        n = float(value)
    except (TypeError, ValueError):
        return default
    return n if n > 0 else default


def set_context(base_files, label, settings):
    """base_files: {path: row} of the baseline, or None for no compare."""
    _context["base"] = base_files
    _context["label"] = label
    _context["settings"] = settings or {}


def worker_args():
    return (_context["base"], _context["label"], _context["settings"])


def init_worker(base_files, label, settings):
    """multiprocessing.Pool initializer."""
    set_context(base_files, label, settings)


def _int(row, key):
    if not isinstance(row, dict):
        return None
    v = row.get(key)
    if v is None or isinstance(v, bool):
        return None
    try:
        n = int(v)
    except (TypeError, ValueError):
        return None
    return n if n >= 0 else None


def _failed(row):
    return bool(row) and int(row.get("failed") or 0) > 0


def _changed(base_row, new_row):
    """True when both rows have a content hash and they differ."""
    old = (base_row or {}).get("sha")
    new = (new_row or {}).get("sha")
    return bool(old) and bool(new) and old != new


def _memory_out(old, new, settings):
    """(ratio) when new is out of family vs old, else None."""
    if old is None or new is None or old <= 0 or new <= old:
        return None
    ratio = settings.get("memory_ratio", MEMORY_RATIO)
    floor = settings.get("memory_floor", MEMORY_FLOOR)
    if new > old * ratio and new - old >= floor:
        return float(new) / float(old)
    return None


def memory_marks(base_row, new_row, settings=None):
    """[(metric, ratio)] for xctx and chunk bytes out of family."""
    settings = settings or _context["settings"]
    if not base_row or not new_row:
        return []
    if _failed(base_row) or _failed(new_row):
        return []
    if _changed(base_row, new_row):
        return []
    out = []
    for key, name in (("xctx_bytes", "xctx"), ("xctx_chunk_bytes", "chunk")):
        r = _memory_out(_int(base_row, key), _int(new_row, key), settings)
        if r is not None:
            out.append((name, r))
    return out


def line_marker(record):
    """Marker for a finished test's line, or "" (memory only)."""
    base = _context["base"]
    if not base:
        return ""
    marks = memory_marks(base.get(record.get("path")), record)
    if not marks:
        return ""
    return "  " + " ".join(
        "▲{} {:.1f}× base".format(name, r) for name, r in marks)


def evaluate(records, base_run, last_run=None, settings=None):
    """This run's memory against base_run, test by test."""
    settings = settings or {}
    new = {r["path"]: r for r in records or [] if r.get("path")}
    base = {}
    for row in (base_run or {}).get("files") or []:
        if isinstance(row, dict) and row.get("path"):
            base[row["path"]] = row
    last = {}
    for row in (last_run or {}).get("files") or []:
        if isinstance(row, dict) and row.get("path"):
            last[row["path"]] = row
    both = set(new) & set(base)
    changed = sorted(p for p in both if _changed(base[p], new[p]))
    matched = sorted(both - set(changed))
    added = sorted(set(new) - set(base))
    gone = sorted(set(base) - set(new))

    memory = []
    for path in matched:
        for name, r in memory_marks(base[path], new[path], settings):
            memory.append({"path": path, "metric": name, "ratio": r})
    memory.sort(key=lambda m: m["ratio"], reverse=True)

    return {
        "label": None,
        "matched": len(matched),
        "new": added,
        "gone": gone,
        "changed": changed,
        "memory": memory,
        "base_files": base,
        "last_files": last,
        "new_files": new,
    }


def summary_record(result):
    """What history keeps about this comparison."""
    if not result:
        return None
    return {
        "against": result.get("label"),
        "matched": result["matched"],
        "new": len(result["new"]),
        "gone": len(result["gone"]),
        "changed": len(result.get("changed") or []),
        "memory": [{"path": m["path"], "metric": m["metric"],
                    "ratio": round(m["ratio"], 3)} for m in result["memory"]],
    }


def memory_paths(result):
    return sorted({m["path"] for m in (result or {}).get("memory") or []})


def print_summary(result):
    if not result:
        return
    nmem = len(memory_paths(result))
    line = "Out of family: {m} (memory, {n} matched tests) vs {lab}".format(
        m=nmem, n=result["matched"], lab=result.get("label") or "?")
    extra = []
    if result["new"]:
        extra.append("{} new".format(len(result["new"])))
    if result["gone"]:
        extra.append("{} gone".format(len(result["gone"])))
    if result.get("changed"):
        extra.append("{} changed".format(len(result["changed"])))
    if extra:
        line += " (" + ", ".join(extra) + ")"
    if nmem:
        msg.warn(line)
    else:
        msg.highlighted_info(line)


def _fmt(n):
    return format_xctx_bytes(n) or "-"


def print_detail(result):
    """--error-detail: each flagged test against the last run and base,
    then the tests not compared because their file changed."""
    if not result:
        return
    _print_memory_detail(result)
    _print_changed_detail(result)


def _print_changed_detail(result):
    changed = result.get("changed") or []
    if not changed:
        return
    msg.highlighted_info("")
    msg.highlighted_info(
        "Not compared, test file changed since " +
        (result.get("label") or "?") + ":")
    for p in changed[:DETAIL_TOP]:
        msg.highlighted_info("  " + p)
    if len(changed) > DETAIL_TOP:
        msg.highlighted_info("  … {} more".format(len(changed) - DETAIL_TOP))


def _print_memory_detail(result):
    if not result["memory"]:
        return
    base = result["base_files"]
    last = result["last_files"]
    new = result["new_files"]
    msg.highlighted_info("")
    msg.highlighted_info("Out of family vs " + (result.get("label") or "?") +
                         " (fuzzy: worth a look):")
    for m in result["memory"][:DETAIL_TOP]:
        key = "xctx_bytes" if m["metric"] == "xctx" else "xctx_chunk_bytes"
        now = _int(new[m["path"]], key)
        parts = ["  {p}  {k} {n}".format(
            p=m["path"], k=m["metric"], n=_fmt(now))]
        lrow = last.get(m["path"])
        if lrow and _int(lrow, key):
            parts.append("last {v} ({r:.1f}×)".format(
                v=_fmt(_int(lrow, key)), r=float(now) / _int(lrow, key)))
        parts.append("base {v} ({r:.1f}×)".format(
            v=_fmt(_int(base[m["path"]], key)), r=m["ratio"]))
        msg.warn("  ".join(parts))
    if len(result["memory"]) > DETAIL_TOP:
        msg.highlighted_info("  … {} more".format(
            len(result["memory"]) - DETAIL_TOP))


def strict_failure(result, settings):
    """True when test_family_strict and memory is out of family."""
    return bool(settings.get("strict")) and bool(memory_paths(result))
