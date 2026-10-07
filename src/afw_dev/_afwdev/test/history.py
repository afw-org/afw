#! /usr/bin/env python3
##
# @file history.py
# @brief Dated test-run records, --compare, and --trend.
#
# Per-file rows: path, ms, xctx_bytes, xctx_chunk_bytes (null if the
# binary did not stamp poolBytesInUse / poolChunkBytes). Console shows
# comma-separated bytes. ms is noisy. Failed paths are not flagged for
# bytes/ms. Compare/trend never fail the process in v1.
#

import os
import re
import subprocess
import threading
from datetime import datetime, timedelta, timezone

from _afwdev.common import msg, nfc
from _afwdev.test.common import format_xctx_bytes

DEFAULT_HISTORY_DIR = os.path.expanduser("~/.afw/test-history")
BYTES_RATIO = 1.5
BYTES_FLOOR = 32 * 1024
MS_RATIO = 2.0
MS_FLOOR_MS = 200
TREND_DEFAULT_COUNT = 10
TREND_TOP = 10
NEW_GONE_LIST_MAX = 20


def git_meta(cwd=None):
    """Short and full commit, branch, dirty flag. None if not a git tree."""
    meta = {"commit": None, "commit_full": None, "branch": None,
            "dirty": False}
    kw = dict(stderr=subprocess.DEVNULL, text=True, cwd=cwd or os.getcwd())
    try:
        meta["commit_full"] = subprocess.check_output(
            ["git", "rev-parse", "HEAD"], **kw).strip()
        meta["commit"] = subprocess.check_output(
            ["git", "rev-parse", "--short", "HEAD"], **kw).strip()
        meta["branch"] = subprocess.check_output(
            ["git", "branch", "--show-current"], **kw).strip() or None
        dirty = subprocess.check_output(
            ["git", "status", "--porcelain"], **kw)
        meta["dirty"] = bool(dirty.strip())
    except Exception:
        pass
    return meta


def env_mode(options):
    return (options or {}).get("mode") or "afw"


def history_dir(options):
    """test_history_dir from afwdev-settings.json, or ~/.afw/test-history.

    history_dir in options is for afwdev's own tests.
    """
    explicit = (options or {}).get("history_dir") or ""
    if explicit:
        return os.path.expanduser(explicit)
    settings = (options or {}).get("afwdev_settings") or {}
    setting = settings.get("test_history_dir") or ""
    if setting:
        return os.path.expanduser(setting)
    return DEFAULT_HISTORY_DIR


def should_write_history(options):
    """Every run records history unless test_history is false."""
    settings = (options or {}).get("afwdev_settings") or {}
    return settings.get("test_history", True) is not False


def _mode_suffix(mode):
    safe = re.sub(r"[^A-Za-z0-9._-]+", "-", str(mode or "afw"))
    return safe or "afw"


def sanitize_ref_label(label):
    """Filename-safe reference label, or empty if none."""
    if not label or not str(label).strip():
        return ""
    safe = re.sub(r"[^A-Za-z0-9._-]+", "-", str(label).strip())
    return safe.strip("-.")


def is_reference_name(name):
    return "-ref-" in os.path.basename(name or "")


def ref_label_from_name(name, mode):
    """Label from `{stamp}-ref-{label}-{mode}.json`, or None."""
    base = os.path.basename(name or "")
    suffix = "-" + _mode_suffix(mode) + ".json"
    marker = "-ref-"
    if not base.endswith(suffix) or marker not in base:
        return None
    stem = base[: -len(suffix)]
    _stamp, label = stem.split(marker, 1)
    return label or None


def is_reference_run(run):
    if not run:
        return False
    if run.get("reference"):
        return True
    return is_reference_name(run.get("_basename") or run.get("_path") or "")


def history_filename(mode, when=None, ref_label=None):
    when = when or datetime.now(timezone.utc)
    stamp = when.strftime("%Y-%m-%dT%H%M%S")
    stamp += "{:03d}Z".format(when.microsecond // 1000)
    mode_s = _mode_suffix(mode)
    label = sanitize_ref_label(ref_label)
    if label:
        return "{}-ref-{}-{}.json".format(stamp, label, mode_s)
    return "{}-{}.json".format(stamp, mode_s)


def list_run_files(dir_path, mode):
    """Dated run JSON paths for mode, oldest first. Skips latest-* symlinks."""
    if not dir_path or not os.path.isdir(dir_path):
        return []
    suffix = "-" + _mode_suffix(mode) + ".json"
    names = []
    for name in os.listdir(dir_path):
        if name.startswith("latest"):
            continue
        if name.endswith(suffix):
            names.append(name)
    names.sort()
    return [os.path.join(dir_path, n) for n in names]


def load_run(path):
    with nfc.open(path, "r") as fd:
        data = nfc.json_load(fd)
    if not isinstance(data, dict):
        raise ValueError("history file is not an object: " + path)
    data["_path"] = path
    data["_basename"] = os.path.basename(path)
    return data


def _latest_path(dir_path, mode):
    return os.path.join(
        dir_path, "latest-{}.json".format(_mode_suffix(mode)))


def _drop_dangling_latest(dir_path, mode):
    """Remove latest-{mode}.json when its target is gone. Returns 1 or 0."""
    latest = _latest_path(dir_path, mode)
    if not os.path.islink(latest):
        return 0
    target = os.readlink(latest)
    full = target if os.path.isabs(target) else os.path.join(dir_path, target)
    if os.path.exists(full):
        return 0
    os.remove(latest)
    return 1


def clear_history(options):
    """Delete ordinary history for this mode. Reference runs stay."""
    dir_path = history_dir(options)
    mode = env_mode(options)
    removed = 0
    if dir_path and os.path.isdir(dir_path):
        for path in list_run_files(dir_path, mode):
            if is_reference_name(path):
                continue
            if os.path.isfile(path) and not os.path.islink(path):
                os.remove(path)
                removed += 1
        removed += _drop_dangling_latest(dir_path, mode)
    msg.highlighted_info(
        "Removed {n} history file(s) from {d}".format(
            n=removed, d=dir_path))
    return removed


def write_history(summary, options):
    """Write dated JSON and latest-{mode}.json symlink. Returns the path.

    Other afwdev test runs may list this directory at any time, so a
    file appears under its final name only when complete: write a hidden
    temp file, fsync it, then link it into place. os.link never replaces
    a file, so two runs that finish in the same millisecond get
    different stamps. latest is swapped in one step with os.replace.
    """
    dir_path = history_dir(options)
    mode = env_mode(options)
    os.makedirs(dir_path, exist_ok=True)
    ref_label = sanitize_ref_label((options or {}).get("_history_label"))
    payload = dict(summary)
    payload.pop("_path", None)
    payload.pop("_basename", None)
    if ref_label:
        payload["reference"] = True
        payload["label"] = ref_label
    when = datetime.now(timezone.utc)
    owner = "{}.{}".format(os.getpid(), threading.get_ident())
    tmp = os.path.join(dir_path, ".{}.{}.tmp".format(
        history_filename(mode, when, ref_label or None), owner))
    try:
        with nfc.open(tmp, "w") as fd:
            nfc.json_dump(payload, fd, indent=2, sort_keys=True)
            fd.write("\n")
            fd.flush()
            os.fsync(fd.fileno())
        while True:
            path = os.path.join(
                dir_path, history_filename(mode, when, ref_label or None))
            try:
                os.link(tmp, path)
                break
            except FileExistsError:
                when += timedelta(milliseconds=1)
    finally:
        try:
            os.remove(tmp)
        except OSError:
            pass
    latest = _latest_path(dir_path, mode)
    tmp_link = "{}.{}.tmp".format(latest, owner)
    try:
        if os.path.lexists(tmp_link):
            os.remove(tmp_link)
        os.symlink(os.path.basename(path), tmp_link)
        os.replace(tmp_link, latest)
    except OSError:
        pass
    msg.highlighted_info("History:       " + path)
    return path


def files_by_path(run):
    out = {}
    for row in (run or {}).get("files") or []:
        if not isinstance(row, dict):
            continue
        path = row.get("path")
        if path:
            out[path] = row
    return out


def _failed(row):
    return int((row or {}).get("failed") or 0) > 0


def _int_field(row, key):
    if not row:
        return None
    n = row.get(key)
    if n is None:
        return None
    try:
        n = int(n)
    except (TypeError, ValueError):
        return None
    return n if n >= 0 else None


def _xctx_bytes(row):
    """Asked-for bytes from a file row, or None."""
    return _int_field(row, "xctx_bytes")


def _xctx_chunk_bytes(row):
    """Chunk bytes from a file row, or None."""
    return _int_field(row, "xctx_chunk_bytes")


def max_file_metric(records, key):
    """Max of a non-negative int field across file records, or 0."""
    m = 0
    for row in records or []:
        n = _int_field(row, key)
        if n is not None and n > m:
            m = n
    return m


def _trend_metric(options):
    """'ms', 'cpu', 'chunk', or 'bytes' from --trend METRIC."""
    raw = ((options or {}).get("trend_metric") or "bytes")
    metric = str(raw).strip().lower()
    if metric in ("ms", "cpu"):
        return metric
    if metric in ("chunk", "chunks"):
        return "chunk"
    return "bytes"


def _fmt_metric(n, metric):
    if n is None:
        return "-"
    if metric in ("ms", "cpu"):
        return str(int(n))
    return format_xctx_bytes(n) or "0"


def _ms(row):
    if not row:
        return None
    n = row.get("ms")
    if n is None:
        return None
    try:
        n = int(n)
    except (TypeError, ValueError):
        return None
    return n if n >= 0 else None


def _metric_fatter(old_row, new_row, getter):
    if _failed(old_row) or _failed(new_row):
        return False
    old_b = getter(old_row)
    new_b = getter(new_row)
    if old_b is None or new_b is None or old_b <= 0:
        return False
    if new_b <= old_b:
        return False
    return (new_b > old_b * BYTES_RATIO) and (new_b - old_b >= BYTES_FLOOR)


def _metric_thinner(old_row, new_row, getter):
    if _failed(old_row) or _failed(new_row):
        return False
    old_b = getter(old_row)
    new_b = getter(new_row)
    if old_b is None or new_b is None or new_b <= 0:
        return False
    if new_b >= old_b:
        return False
    return (old_b > new_b * BYTES_RATIO) and (old_b - new_b >= BYTES_FLOOR)


def _bytes_fatter(old_row, new_row):
    """True if new xctx bytes is out of family vs old."""
    return _metric_fatter(old_row, new_row, _xctx_bytes)


def _bytes_thinner(old_row, new_row):
    return _metric_thinner(old_row, new_row, _xctx_bytes)


def _chunk_fatter(old_row, new_row):
    return _metric_fatter(old_row, new_row, _xctx_chunk_bytes)


def _chunk_thinner(old_row, new_row):
    return _metric_thinner(old_row, new_row, _xctx_chunk_bytes)


def _ms_slower(old_row, new_row):
    if _failed(old_row) or _failed(new_row):
        return False
    old_m = _ms(old_row)
    new_m = _ms(new_row)
    if old_m is None or new_m is None or old_m <= 0:
        return False
    if new_m <= old_m:
        return False
    return (new_m > old_m * MS_RATIO) and (new_m - old_m >= MS_FLOOR_MS)


def _ms_faster(old_row, new_row):
    if _failed(old_row) or _failed(new_row):
        return False
    old_m = _ms(old_row)
    new_m = _ms(new_row)
    if old_m is None or new_m is None or new_m <= 0:
        return False
    if new_m >= old_m:
        return False
    return (old_m > new_m * MS_RATIO) and (old_m - new_m >= MS_FLOOR_MS)


def _ratio_bytes(old_row, new_row, getter=_xctx_bytes):
    old_b = getter(old_row)
    new_b = getter(new_row)
    if not old_b or new_b is None:
        return 0
    return float(new_b) / float(old_b)


def compare_runs(old, new):
    """Return a dict: compared/new/gone paths and bytes/ms movers."""
    old_files = files_by_path(old)
    new_files = files_by_path(new)
    old_paths = set(old_files)
    new_paths = set(new_files)
    compared = sorted(old_paths & new_paths)
    added = sorted(new_paths - old_paths)
    gone = sorted(old_paths - new_paths)
    bytes_missing = 0
    chunk_missing = 0
    fatter = []
    thinner = []
    chunk_fatter = []
    chunk_thinner = []
    slower = []
    faster = []
    for path in compared:
        o = old_files[path]
        n = new_files[path]
        if _xctx_bytes(o) is None or _xctx_bytes(n) is None:
            bytes_missing += 1
        if _xctx_chunk_bytes(o) is None or _xctx_chunk_bytes(n) is None:
            chunk_missing += 1
        if _bytes_fatter(o, n):
            fatter.append(path)
        if _bytes_thinner(o, n):
            thinner.append(path)
        if _chunk_fatter(o, n):
            chunk_fatter.append(path)
        if _chunk_thinner(o, n):
            chunk_thinner.append(path)
        if _ms_slower(o, n):
            slower.append(path)
        if _ms_faster(o, n):
            faster.append(path)
    fatter.sort(
        key=lambda p: _ratio_bytes(old_files[p], new_files[p]), reverse=True)
    chunk_fatter.sort(
        key=lambda p: _ratio_bytes(
            old_files[p], new_files[p], _xctx_chunk_bytes), reverse=True)
    return {
        "compared": compared,
        "added": added,
        "gone": gone,
        "bytes_missing": bytes_missing,
        "chunk_missing": chunk_missing,
        "fatter": fatter,
        "thinner": thinner,
        "chunk_fatter": chunk_fatter,
        "chunk_thinner": chunk_thinner,
        "slower": slower,
        "faster": faster,
        "old_files": old_files,
        "new_files": new_files,
        "old": old,
        "new": new,
    }


def _run_label(run):
    return (run.get("git") or {}).get("commit") or run.get("_basename") or "?"


def resolve_trend_runs(options):
    """--compare-to run (default the baseline), then later runs, oldest first.

    Later means untagged runs of this mode stamped after it, the newest
    TREND_DEFAULT_COUNT of them. With no baseline yet, the most recent
    runs.
    """
    from _afwdev.test import baseline
    mode = env_mode(options)
    dir_path = history_dir(options)
    ref = (options or {}).get("compare_to") or "baseline"
    if ref == "baseline" and not baseline.select_baseline(options):
        # No baseline yet: the most recent runs.
        whole = [p for p in list_run_files(dir_path, mode)
                 if not baseline._narrowed_file(p)]
        recent = whole[-(TREND_DEFAULT_COUNT + 1):]
        start, after = (recent[0], recent[1:]) if recent else (None, [])
    else:
        start, _label = baseline.resolve(options)
        after = [p for p in list_run_files(dir_path, mode)
                 if not is_reference_name(p)
                 and os.path.basename(p) > os.path.basename(start or "")]
    if not start:
        raise ValueError("no history to start --trend from")
    after = [p for p in after if not baseline._narrowed_file(p)]
    files = [start] + after[-TREND_DEFAULT_COUNT:]
    if len(files) < 2:
        raise ValueError(
            "need a run after {s} for --trend (mode {m})".format(
                s=os.path.basename(start), m=mode))
    return [load_run(p) for p in files]


def print_baselines(options):
    from _afwdev.test import baseline
    mode = env_mode(options)
    found = baseline.baselines(options)
    if not found:
        msg.highlighted_info("Baselines ({m}): none".format(m=mode))
        return
    msg.highlighted_info("Baselines ({m}):".format(m=mode))
    for p in found:
        full, short = baseline.file_commit(p)
        msg.highlighted_info("  {c}  {l}".format(
            c=short or "?", l=ref_label_from_name(p, mode)))


def _path_filter(options):
    pattern = (options or {}).get("test-pattern") or ".*"
    if pattern == ".*":
        return None
    try:
        return re.compile(pattern)
    except re.error as e:
        msg.error_exit("Invalid --test-pattern regex: " + str(e))


def trend_runs(runs, options=None):
    peer_run = None
    for run in runs:
        if is_reference_run(run):
            peer_run = run
            break
    if peer_run is None:
        peer_run = runs[0]
    first = files_by_path(peer_run)
    last = files_by_path(runs[-1])
    rx = _path_filter(options)
    def ok(path):
        return True if rx is None else bool(rx.search(path))
    first_p = {p for p in first if ok(p)}
    last_p = {p for p in last if ok(p)}
    added = sorted(last_p - first_p)
    gone = sorted(first_p - last_p)
    peer_paths = first_p & last_p
    peer_ms = []
    for run in runs:
        by = files_by_path(run)
        total = 0
        n = 0
        for pth in peer_paths:
            row = by.get(pth)
            if not row:
                continue
            m = _ms(row)
            if m is None:
                continue
            total += m
            n += 1
        peer_ms.append({"ms": total, "n": n})
    peer_label = peer_run.get("label") or (
        "ref" if is_reference_run(peer_run) else "first")
    def _series_max(getter):
        out = []
        for run in runs:
            mx = 0
            any_b = False
            for path, row in files_by_path(run).items():
                if not ok(path):
                    continue
                b = getter(row)
                if b is None:
                    continue
                any_b = True
                mx = max(mx, b)
            out.append(mx if any_b else None)
        return out

    series_max = _series_max(_xctx_bytes)
    series_max_chunk = _series_max(_xctx_chunk_bytes)
    movers = []
    metric = _trend_metric(options)
    if metric == "ms":
        getter = _ms
    elif metric == "cpu":
        getter = lambda row: _int_field(row, "cpu_ms")
    elif metric == "chunk":
        getter = _xctx_chunk_bytes
    else:
        getter = _xctx_bytes
    for path in sorted(first_p & last_p):
        o = first[path]
        n = last[path]
        old_v, new_v = getter(o), getter(n)
        if old_v is None or new_v is None or old_v <= 0:
            continue
        ratio = float(new_v) / float(old_v)
        series = []
        for run in runs:
            row = files_by_path(run).get(path)
            series.append(getter(row) if row else None)
        present = [x for x in series if x is not None]
        movers.append({
            "path": path,
            "first": old_v,
            "last": new_v,
            "min": min(present) if present else None,
            "max": max(present) if present else None,
            "ratio": ratio,
        })
    movers.sort(key=lambda m: m["ratio"], reverse=True)
    return {
        "runs": runs,
        "new": added,
        "gone": gone,
        "series_max": series_max,
        "series_max_chunk": series_max_chunk,
        "peer_ms": peer_ms,
        "peer_label": peer_label,
        "movers": movers,
        "metric": metric,
    }


def print_trend(result, show_all=False):
    runs = result["runs"]
    msg.highlighted_info(
        "Trend {n} runs  new {a}  gone {g}  (mode {m}, peer {lab})".format(
            n=len(runs),
            a=len(result["new"]),
            g=len(result["gone"]),
            m=runs[0].get("mode") or "afw",
            lab=result.get("peer_label") or "first",
        ))
    fuzz_bits = []
    for run in runs:
        totals = run.get("fuzz") or fuzz_totals(run.get("files"))
        if totals.get("leaves"):
            fuzz_bits.append("{:,}{}".format(
                totals.get("requests") or 0,
                "!" if totals.get("serverExits") or totals.get("failed")
                else ""))
        else:
            fuzz_bits.append("-")
    if any(b != "-" for b in fuzz_bits):
        msg.highlighted_info(
            "Fuzz requests (! = failures or exits):  " + "  ".join(fuzz_bits))
    peer = result.get("peer_ms") or []
    if peer:
        bits = []
        for rec in peer:
            bits.append("{:.1f}s".format((rec.get("ms") or 0) / 1000.0))
        npeer = peer[-1].get("n") if peer else 0
        msg.highlighted_info(
            "Peer ms ({n} files):  {bits}".format(
                n=npeer, bits="  ".join(bits)))
    def _print_maxes(title, maxes):
        if not any(x is not None for x in (maxes or [])):
            return
        bits = []
        for run, n in zip(runs, maxes):
            label = run.get("_basename") or _run_label(run)
            if label.endswith(".json"):
                label = label[:-5]
            bits.append("{}:{}".format(label, _fmt_metric(n, "bytes")))
        msg.highlighted_info(title + "  " + "  ".join(bits))

    metric = result.get("metric") or "bytes"
    _print_maxes("Run max xctx:", result.get("series_max") or [])
    _print_maxes("Run max chunk:", result.get("series_max_chunk") or [])
    movers = result["movers"]
    show = movers if show_all else movers[:TREND_TOP]
    if show:
        msg.highlighted_info("")
        unit = {"ms": "wall ms", "cpu": "cpu ms",
                "chunk": "chunk"}.get(metric, "bytes")
        msg.highlighted_info("Top movers ({u}, first → last):".format(u=unit))
        for m in show:
            msg.highlighted_info(
                "  {f} → {l}  ({r:.2f}×)  min {mn} max {mx}  {path}".format(
                    f=_fmt_metric(m["first"], metric),
                    l=_fmt_metric(m["last"], metric),
                    r=m["ratio"],
                    mn=_fmt_metric(m["min"], metric),
                    mx=_fmt_metric(m["max"], metric),
                    path=m["path"],
                ))
    def _list(title, paths):
        if not paths:
            return
        msg.highlighted_info("")
        msg.highlighted_info(title + " ({n}):".format(n=len(paths)))
        listing = paths if show_all or len(paths) <= NEW_GONE_LIST_MAX else paths[:NEW_GONE_LIST_MAX]
        for p in listing:
            msg.highlighted_info("  " + p)
        if len(paths) > len(listing):
            msg.highlighted_info(
                "  … {n} more (use --show-all)".format(
                    n=len(paths) - len(listing)))
    _list("New since first run", result["new"])
    _list("Gone since first run", result["gone"])


def fuzz_from_response(response):
    """Fuzz summaries from an orchestrated leaf's step timings."""
    out = []
    steps = (response or {}).get("stepTimings") if isinstance(
        response, dict) else None
    for step in steps or []:
        fh = step.get("firehose") if isinstance(step, dict) else None
        if isinstance(fh, dict) and isinstance(fh.get("fuzz"), dict):
            out.append(fh["fuzz"])
    return out


def fuzz_totals(records):
    """{leaves, requests, failed, serverExits} over a run's file records."""
    totals = {"leaves": 0, "requests": 0, "failed": 0, "serverExits": 0}
    for rec in records or []:
        fuzz = rec.get("fuzz") if isinstance(rec, dict) else None
        if not fuzz:
            continue
        totals["leaves"] += 1
        for f in fuzz:
            totals["requests"] += int(f.get("requests") or 0)
            totals["failed"] += int(f.get("failed") or 0)
            totals["serverExits"] += int(f.get("serverExits") or 0)
    return totals


def file_record(path, duration_ms, xctx_bytes, num_passed, num_skipped,
                num_failed, xctx_chunk_bytes=None, cpu_ms=None, fuzz=None):
    rec = {
        "path": path,
        "ms": int(duration_ms),
        "passed": int(num_passed),
        "skipped": int(num_skipped),
        "failed": int(num_failed),
        "xctx_bytes": None,
        "xctx_chunk_bytes": None,
    }
    if xctx_bytes is not None:
        rec["xctx_bytes"] = int(xctx_bytes)
    if xctx_chunk_bytes is not None:
        rec["xctx_chunk_bytes"] = int(xctx_chunk_bytes)
    if cpu_ms is not None:
        rec["cpu_ms"] = int(cpu_ms)
    if fuzz:
        rec["fuzz"] = fuzz
    return rec
