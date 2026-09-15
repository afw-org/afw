#! /usr/bin/env python3
##
# @file history.py
# @brief Dated test-run records, --compare, and --trend.
#
# Per-file rows: path, ms, xctx_bytes (null if the binary did not stamp
# poolBytesInUse). Console shows comma-separated bytes. ms is noisy.
# Failed paths are not flagged for bytes/ms. Compare/trend never fail the
# process in v1.
#

import glob
import os
import re
import subprocess
from datetime import datetime, timezone

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
    """Short commit, branch, dirty flag. Empty dict fields if not a git tree."""
    meta = {"commit": None, "branch": None, "dirty": False}
    kw = dict(stderr=subprocess.DEVNULL, text=True, cwd=cwd or os.getcwd())
    try:
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
    """Resolved history directory, or None if history is not in play."""
    explicit = (options or {}).get("history_dir") or ""
    if explicit:
        return os.path.expanduser(explicit)
    settings = (options or {}).get("afwdev_settings") or {}
    setting = settings.get("test_history_dir") or ""
    if setting:
        return os.path.expanduser(setting)
    if (options or {}).get("history"):
        return DEFAULT_HISTORY_DIR
    return DEFAULT_HISTORY_DIR


def should_write_history(options):
    if (options or {}).get("history") or (options or {}).get("history_ref"):
        return True
    settings = (options or {}).get("afwdev_settings") or {}
    return bool(settings.get("test_history_dir"))


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


def select_trend_files(dir_path, mode, count=TREND_DEFAULT_COUNT):
    """All reference runs for mode plus the last N non-reference, oldest first."""
    all_files = list_run_files(dir_path, mode)
    refs = [p for p in all_files if is_reference_name(p)]
    nonrefs = [p for p in all_files if not is_reference_name(p)]
    count = max(1, int(count))
    chosen = refs + nonrefs[-count:]
    # Unique, keep timestamp order (basename sorts with the stamp prefix).
    seen = set()
    out = []
    for p in sorted(chosen, key=lambda x: os.path.basename(x)):
        if p in seen:
            continue
        seen.add(p)
        out.append(p)
    return out


def load_run(path):
    with nfc.open(path, "r") as fd:
        data = nfc.json_load(fd)
    if not isinstance(data, dict):
        raise ValueError("history file is not an object: " + path)
    data["_path"] = path
    data["_basename"] = os.path.basename(path)
    return data


def write_history(summary, options):
    """Write dated JSON and latest-{mode}.json symlink. Returns the path."""
    dir_path = history_dir(options)
    mode = env_mode(options)
    os.makedirs(dir_path, exist_ok=True)
    ref_label = sanitize_ref_label((options or {}).get("history_ref"))
    path = os.path.join(dir_path, history_filename(mode, ref_label=ref_label or None))
    payload = dict(summary)
    payload.pop("_path", None)
    payload.pop("_basename", None)
    if ref_label:
        payload["reference"] = True
        payload["label"] = ref_label
    with nfc.open(path, "w") as fd:
        nfc.json_dump(payload, fd, indent=2, sort_keys=True)
        fd.write("\n")
    latest = os.path.join(dir_path, "latest-{}.json".format(_mode_suffix(mode)))
    try:
        if os.path.islink(latest) or os.path.exists(latest):
            os.remove(latest)
        os.symlink(os.path.basename(path), latest)
    except OSError:
        pass
    msg.highlighted_info("Wrote test history to " + path)
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


def _xctx_bytes(row):
    """Asked-for bytes from a file row, or None."""
    if not row:
        return None
    n = row.get("xctx_bytes")
    if n is None:
        return None
    try:
        n = int(n)
    except (TypeError, ValueError):
        return None
    return n if n >= 0 else None


def _trend_metric(options):
    """'ms' or 'bytes'."""
    raw = ((options or {}).get("trend_metric") or "bytes")
    metric = str(raw).strip().lower()
    if metric == "ms":
        return "ms"
    return "bytes"


def _fmt_metric(n, metric):
    if n is None:
        return "-"
    if metric == "ms":
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


def _bytes_fatter(old_row, new_row):
    """True if new xctx bytes is out of family vs old."""
    if _failed(old_row) or _failed(new_row):
        return False
    old_b = _xctx_bytes(old_row)
    new_b = _xctx_bytes(new_row)
    if old_b is None or new_b is None or old_b <= 0:
        return False
    if new_b <= old_b:
        return False
    return (new_b > old_b * BYTES_RATIO) and (new_b - old_b >= BYTES_FLOOR)


def _bytes_thinner(old_row, new_row):
    if _failed(old_row) or _failed(new_row):
        return False
    old_b = _xctx_bytes(old_row)
    new_b = _xctx_bytes(new_row)
    if old_b is None or new_b is None or new_b <= 0:
        return False
    if new_b >= old_b:
        return False
    return (old_b > new_b * BYTES_RATIO) and (old_b - new_b >= BYTES_FLOOR)


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


def _ratio_bytes(old_row, new_row):
    old_b = _xctx_bytes(old_row)
    new_b = _xctx_bytes(new_row)
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
    fatter = []
    thinner = []
    slower = []
    faster = []
    for path in compared:
        o = old_files[path]
        n = new_files[path]
        if _xctx_bytes(o) is None or _xctx_bytes(n) is None:
            bytes_missing += 1
        if _bytes_fatter(o, n):
            fatter.append(path)
        if _bytes_thinner(o, n):
            thinner.append(path)
        if _ms_slower(o, n):
            slower.append(path)
        if _ms_faster(o, n):
            faster.append(path)
    fatter.sort(
        key=lambda p: _ratio_bytes(old_files[p], new_files[p]), reverse=True)
    return {
        "compared": compared,
        "added": added,
        "gone": gone,
        "bytes_missing": bytes_missing,
        "fatter": fatter,
        "thinner": thinner,
        "slower": slower,
        "faster": faster,
        "old_files": old_files,
        "new_files": new_files,
        "old": old,
        "new": new,
    }


def _run_label(run):
    return (run.get("git") or {}).get("commit") or run.get("_basename") or "?"


def print_compare(result, show_all=False):
    old = result["old"]
    new = result["new"]
    n_comp = len(result["compared"])
    msg.highlighted_info(
        "Compared {n}  new {a}  gone {g}  (mode {m}, {oc} → {nc})".format(
            n=n_comp,
            a=len(result["added"]),
            g=len(result["gone"]),
            m=old.get("mode") or new.get("mode") or "afw",
            oc=_run_label(old),
            nc=_run_label(new),
        ))
    if result["bytes_missing"]:
        msg.highlighted_info(
            "bytes missing in {n} compared path(s) "
            "(old afw / non-test_script)".format(
                n=result["bytes_missing"]))
    msg.highlighted_info(
        "Memory:  {f} fatter  {t} thinner  "
        "(threshold {r}× and +{floor})".format(
            f=len(result["fatter"]),
            t=len(result["thinner"]),
            r=BYTES_RATIO,
            floor=format_xctx_bytes(BYTES_FLOOR),
        ))
    msg.highlighted_info(
        "Time:    {s} slower  {f} faster   (noisy; {r}× and +{ms}ms)".format(
            s=len(result["slower"]),
            f=len(result["faster"]),
            r=int(MS_RATIO),
            ms=MS_FLOOR_MS,
        ))
    old_files = result["old_files"]
    new_files = result["new_files"]
    show = result["fatter"] if not show_all else result["fatter"]
    if show:
        msg.highlighted_info("")
        msg.highlighted_info("Fatter:")
        for path in show[:TREND_TOP] if not show_all else show:
            o = old_files[path]
            n = new_files[path]
            msg.highlighted_info(
                "  {ob} → {nb}  {path}".format(
                    ob=_fmt_metric(_xctx_bytes(o), "bytes"),
                    nb=_fmt_metric(_xctx_bytes(n), "bytes"),
                    path=path))
    def _list(title, paths):
        if not paths:
            return
        msg.highlighted_info("")
        msg.highlighted_info(title + " ({n}):".format(n=len(paths)))
        if len(paths) > NEW_GONE_LIST_MAX and not show_all:
            for p in paths[:NEW_GONE_LIST_MAX]:
                msg.highlighted_info("  " + p)
            msg.highlighted_info(
                "  … {n} more (use --show-all)".format(
                    n=len(paths) - NEW_GONE_LIST_MAX))
        else:
            for p in paths:
                msg.highlighted_info("  " + p)
    _list("New", result["added"])
    _list("Gone", result["gone"])


def resolve_compare_paths(options):
    """Return (old_path, new_path) from --compare args and history dir."""
    args = options.get("compare")
    if args is False or args is None:
        raise ValueError("compare not requested")
    dir_path = history_dir(options)
    mode = env_mode(options)
    runs = list_run_files(dir_path, mode)
    if isinstance(args, str):
        args = [args]
    args = list(args or [])
    if len(args) == 0:
        if len(runs) < 2:
            raise ValueError(
                "need at least two history files for mode {m} in {d}".format(
                    m=mode, d=dir_path))
        return runs[-2], runs[-1]
    if len(args) == 1:
        if not runs:
            raise ValueError(
                "no history files for mode {m} in {d} to use as newer".format(
                    m=mode, d=dir_path))
        return os.path.expanduser(args[0]), runs[-1]
    if len(args) >= 2:
        return os.path.expanduser(args[0]), os.path.expanduser(args[1])
    raise ValueError("invalid --compare arguments")


def resolve_trend_runs(options):
    """Load run dicts oldest-first for --trend."""
    args = options.get("trend")
    if args is False or args is None:
        raise ValueError("trend not requested")
    dir_path = history_dir(options)
    mode = env_mode(options)
    if isinstance(args, str):
        args = [args]
    args = list(args or [])
    count = TREND_DEFAULT_COUNT
    files = []
    if len(args) == 1 and re.fullmatch(r"[0-9]+", args[0]):
        count = max(1, int(args[0]))
        args = []
    if args:
        for a in args:
            expanded = glob.glob(os.path.expanduser(a)) or [os.path.expanduser(a)]
            files.extend(expanded)
        files = sorted(set(files), key=lambda p: os.path.basename(p))
    else:
        files = select_trend_files(dir_path, mode, count)
    if len(files) < 2:
        raise ValueError(
            "need at least two history files for --trend (mode {m})".format(
                m=mode))
    runs = [load_run(p) for p in files]
    modes = {(r.get("mode") or "afw") for r in runs}
    if len(modes) > 1:
        raise ValueError(
            "refusing mixed env-mode in --trend: " + ", ".join(sorted(modes)))
    return runs


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
    series_max = []
    for run in runs:
        mx = 0
        any_b = False
        for path, row in files_by_path(run).items():
            if not ok(path):
                continue
            b = _xctx_bytes(row)
            if b is None:
                continue
            any_b = True
            mx = max(mx, b)
        series_max.append(mx if any_b else None)
    movers = []
    metric = _trend_metric(options)
    for path in sorted(first_p & last_p):
        o = first[path]
        n = last[path]
        if metric == "ms":
            old_v, new_v = _ms(o), _ms(n)
        else:
            old_v, new_v = _xctx_bytes(o), _xctx_bytes(n)
        if old_v is None or new_v is None or old_v <= 0:
            continue
        ratio = float(new_v) / float(old_v)
        series = []
        for run in runs:
            row = files_by_path(run).get(path)
            if metric == "ms":
                series.append(_ms(row))
            else:
                series.append(_xctx_bytes(row))
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
    peer = result.get("peer_ms") or []
    if peer:
        bits = []
        for rec in peer:
            bits.append("{:.1f}s".format((rec.get("ms") or 0) / 1000.0))
        npeer = peer[-1].get("n") if peer else 0
        msg.highlighted_info(
            "Peer ms ({n} files):  {bits}".format(
                n=npeer, bits="  ".join(bits)))
    maxes = result.get("series_max") or []
    metric = result.get("metric") or "bytes"
    if any(x is not None for x in maxes):
        bits = []
        for run, n in zip(runs, maxes):
            label = run.get("_basename") or _run_label(run)
            if label.endswith(".json"):
                label = label[:-5]
            bits.append("{}:{}".format(label, _fmt_metric(n, "bytes")))
        msg.highlighted_info("Run max:  " + "  ".join(bits))
    movers = result["movers"]
    show = movers if show_all else movers[:TREND_TOP]
    if show:
        msg.highlighted_info("")
        unit = "ms" if metric == "ms" else "bytes"
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


def file_record(path, duration_ms, xctx_bytes, num_passed, num_skipped, num_failed):
    rec = {
        "path": path,
        "ms": int(duration_ms),
        "passed": int(num_passed),
        "skipped": int(num_skipped),
        "failed": int(num_failed),
        "xctx_bytes": None,
    }
    if xctx_bytes is not None:
        rec["xctx_bytes"] = int(xctx_bytes)
    return rec
