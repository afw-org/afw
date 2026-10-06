#! /usr/bin/env python3
##
# @file family.py
# @brief Out-of-family tests: this run against a baseline run.
#
# "Out of family" is fuzzy: a flag means "worth a look", not an error.
# Only tests in both runs compare (same path); the others are counted as
# new or gone. A test that failed in either run is not flagged.
#
# Memory (xctx and chunk bytes) is close to deterministic, so it is the
# tighter check: ratio and floor from afwdev-settings.json. It is marked
# on the test's line as soon as the test finishes.
#
# Time is FYI and judged at the end: each test's CPU ratio to the
# baseline against the median ratio over all matched tests, so a busy
# machine moves everything together and only a test that stands out is
# flagged. Wall ms is the fallback where a run has no CPU numbers.
#

from statistics import median

from _afwdev.common import msg
from _afwdev.test.common import format_xctx_bytes

MEMORY_RATIO = 1.2
MEMORY_FLOOR = 16 * 1024
TIME_RATIO = 2.0
TIME_FLOOR_MS = 200
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
        "time_ratio": _number(s.get("test_family_time_ratio"), TIME_RATIO),
        "time_floor_ms": _number(s.get("test_family_time_floor_ms"),
                                 TIME_FLOOR_MS),
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


def _time_value(row, use_cpu):
    return _int(row, "cpu_ms") if use_cpu else _int(row, "ms")


def evaluate(records, base_run, last_run=None, settings=None):
    """Compare this run's file records with base_run (and last_run)."""
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
    matched = sorted(set(new) & set(base))
    added = sorted(set(new) - set(base))
    gone = sorted(set(base) - set(new))

    memory = []
    for path in matched:
        for name, r in memory_marks(base[path], new[path], settings):
            memory.append({"path": path, "metric": name, "ratio": r})
    memory.sort(key=lambda m: m["ratio"], reverse=True)

    use_cpu = any(_int(new[p], "cpu_ms") is not None and
                  _int(base[p], "cpu_ms") is not None for p in matched)
    ratios = {}
    for path in matched:
        if _failed(base[path]) or _failed(new[path]):
            continue
        o = _time_value(base[path], use_cpu)
        n = _time_value(new[path], use_cpu)
        if o is None or n is None or o <= 0:
            continue
        ratios[path] = (float(n) / float(o), o, n)
    drift = median([r for r, _o, _n in ratios.values()]) if ratios else None
    time = []
    t_ratio = settings.get("time_ratio", TIME_RATIO)
    t_floor = settings.get("time_floor_ms", TIME_FLOOR_MS)
    for path, (r, o, n) in ratios.items():
        expected = o * (drift or 1.0)
        if drift and r / drift >= t_ratio and n - expected >= t_floor:
            time.append({"path": path, "ratio": r,
                         "relative": r / drift, "old": o, "new": n})
    time.sort(key=lambda m: m["relative"], reverse=True)

    return {
        "label": None,
        "matched": len(matched),
        "new": added,
        "gone": gone,
        "memory": memory,
        "time": time,
        "time_metric": "cpu" if use_cpu else "wall",
        "drift": drift,
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
        "memory": [{"path": m["path"], "metric": m["metric"],
                    "ratio": round(m["ratio"], 3)} for m in result["memory"]],
        "time": [{"path": m["path"], "ratio": round(m["relative"], 3)}
                 for m in result["time"]],
        "time_metric": result["time_metric"],
        "drift": round(result["drift"], 3) if result["drift"] else None,
    }


def memory_paths(result):
    return sorted({m["path"] for m in (result or {}).get("memory") or []})


def print_summary(result):
    if not result:
        return
    nmem = len(memory_paths(result))
    ntime = len(result["time"])
    line = "Out of family: {t} (memory {m}, time {c}) vs {lab}".format(
        t=len(set(memory_paths(result)) | {m["path"] for m in result["time"]}),
        m=nmem, c=ntime, lab=result.get("label") or "?")
    extra = []
    if result["new"]:
        extra.append("{} new".format(len(result["new"])))
    if result["gone"]:
        extra.append("{} gone".format(len(result["gone"])))
    if extra:
        line += " (" + ", ".join(extra) + ")"
    if nmem:
        msg.warn(line)
    else:
        msg.highlighted_info(line)
    if result["drift"] and result["matched"]:
        msg.highlighted_info(
            "               {m} {d:.2f}× across {n} matched "
            "tests (FYI: busy machine or broad change)".format(
                m="CPU" if result["time_metric"] == "cpu" else "Wall time",
                d=result["drift"], n=result["matched"]))


def _fmt(n):
    return format_xctx_bytes(n) or "-"


def print_detail(result):
    """--error-detail: each flagged test against the last run and base."""
    if not result or not (result["memory"] or result["time"]):
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
    unit = "cpu" if result["time_metric"] == "cpu" else "wall"
    for m in result["time"][:DETAIL_TOP]:
        msg.highlighted_info(
            "  {p}  {u} {n}ms  base {o}ms ({r:.1f}×, "
            "{x:.1f}× the run's median)".format(
                p=m["path"], u=unit, n=m["new"], o=m["old"], r=m["ratio"],
                x=m["relative"]))
    more = len(result["memory"]) + len(result["time"]) - min(
        DETAIL_TOP, len(result["memory"])) - min(DETAIL_TOP, len(result["time"]))
    if more > 0:
        msg.highlighted_info("  … {} more".format(more))


def strict_failure(result, settings):
    """True when test_family_strict and memory is out of family."""
    return bool(settings.get("strict")) and bool(memory_paths(result))
