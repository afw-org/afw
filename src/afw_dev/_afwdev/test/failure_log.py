#! /usr/bin/env python3
##
# @file failure_log.py
# @brief Append diagnostic records for test failures.
#
# failures.log in the run directory (run_dir.py), kept with the run.
# Every failure is one line. The first few of each distinct error also
# get the message body and, when present, a tail of afwfcgi stderr
# and stdout. log type standard writes stdout. History keeps only a
# one-line summary of each failure. A run with no failures has no log.
#

import json
import os
import re
import threading
from datetime import datetime, timezone

from _afwdev.common.errors import error_message, error_to_dict
from _afwdev.test.history import env_mode, git_meta
from _afwdev.test import run_dir

# Full snippet for this many occurrences of the same name+error.
DETAIL_PER_SIGNATURE = 3
# One-line message cap. The detail block keeps a longer copy.
LINE_MAX = 300
DETAIL_MAX = 8000
STDERR_TAIL = 4000

_thread_lock = threading.Lock()


def _now_iso():
    return datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")


def _one_line(text, limit=LINE_MAX):
    flat = re.sub(r"\s+", " ", str(text or "")).strip()
    if len(flat) > limit:
        return flat[: limit - 3] + "..."
    return flat


def _signature(name, message):
    return _one_line(name or "?", 120) + "\n" + _one_line(message, 200)


def _read_tail(path, limit):
    if not path or not os.path.isfile(path):
        return ""
    try:
        with open(path, "rb") as fd:
            fd.seek(0, os.SEEK_END)
            size = fd.tell()
            fd.seek(max(0, size - limit), os.SEEK_SET)
            data = fd.read().decode("utf-8", errors="replace")
    except OSError:
        return ""
    return data.strip()


def _detail_text(err, detail):
    parts = []
    message = error_message(err) or ""
    # Keep a multiline message (valgrind stacks). A one-line message is
    # already the record line.
    if message and "\n" in message.strip():
        parts.append(message.strip())
    if detail:
        parts.append(str(detail).strip())
    obj = None
    if err is not None and hasattr(err, "object"):
        obj = err.object
    if obj is not None:
        try:
            dumped = json.dumps(error_to_dict(err), indent=2, default=str)
        except (TypeError, ValueError):
            dumped = str(obj)
        parts.append(dumped)
    text = "\n".join(p for p in parts if p)
    if len(text) > DETAIL_MAX:
        text = text[: DETAIL_MAX - 3] + "..."
    return text


def begin(options):
    """Create the run's failure log. No-op if it already exists."""
    if options is None:
        return None
    if options.get("_failure_log"):
        return options.get("_failure_log")
    directory = run_dir.current(options)
    if not directory:
        return None
    try:
        mode = env_mode(options)
        path = os.path.join(directory, run_dir.FAILURES_NAME)
        meta = git_meta()
        header = [
            "# afwdev test failures",
            "# when: {}".format(_now_iso()),
            "# mode: {}".format(mode),
            "# commit: {} branch: {} dirty: {}".format(
                meta.get("commit") or "?",
                meta.get("branch") or "?",
                "true" if meta.get("dirty") else "false"),
            "",
        ]
        with open(path, "w", encoding="utf-8") as fd:
            fd.write("\n".join(header))
        options["_failure_log"] = path
        return path
    except OSError:
        return None


def record(options, name, message, err=None, detail=None, stderr_path=None,
           stdout_path=None):
    """Append one failure. Safe from threads and from -j worker processes."""
    path = (options or {}).get("_failure_log")
    if not path or not message:
        return
    sig = _signature(name, message)
    line = _one_line(message)
    try:
        with _thread_lock:
            _append(path, name, line, sig, err, detail, stderr_path,
                    stdout_path)
    except OSError:
        return


def _load_state(state_path):
    try:
        with open(state_path, "r", encoding="utf-8") as fd:
            data = json.load(fd)
        if isinstance(data, dict):
            data.setdefault("ordinal", 0)
            data.setdefault("sigs", {})
            return data
    except (OSError, ValueError):
        pass
    return {"ordinal": 0, "sigs": {}}


def _append(path, name, line, sig, err, detail, stderr_path,
            stdout_path=None):
    import fcntl
    state_path = path + ".state.json"
    with open(path, "a", encoding="utf-8") as fd:
        fcntl.flock(fd, fcntl.LOCK_EX)
        try:
            state = _load_state(state_path)
            state["ordinal"] = int(state.get("ordinal") or 0) + 1
            sigs = state["sigs"]
            seen = int(sigs.get(sig) or 0) + 1
            sigs[sig] = seen
            ordinal = state["ordinal"]
            show_detail = seen <= DETAIL_PER_SIGNATURE
            fd.write("# {} {} {}\n".format(
                ordinal, _now_iso(), _one_line(name or "?", 160)))
            fd.write(line + "\n")
            if show_detail:
                extra = _detail_text(err, detail)
                if extra and extra != line:
                    fd.write(extra + "\n")
                tail = _read_tail(stderr_path, STDERR_TAIL)
                if tail:
                    fd.write("--- afwfcgi stderr ---\n")
                    fd.write(tail + "\n")
                out = _read_tail(stdout_path, STDERR_TAIL)
                if out:
                    fd.write("--- afwfcgi stdout ---\n")
                    fd.write(out + "\n")
            else:
                fd.write("# repeat {}\n".format(seen))
            fd.write("\n")
            fd.flush()
            with open(state_path, "w", encoding="utf-8") as sf:
                json.dump(state, sf)
        finally:
            fcntl.flock(fd, fcntl.LOCK_UN)


def path_if_failed(options):
    """This run's failures.log when it recorded a failure, else None."""
    path = (options or {}).get("_failure_log")
    if not path or not os.path.isfile(path + ".state.json"):
        return None
    try:
        state = _load_state(path + ".state.json")
    except OSError:
        return None
    if int(state.get("ordinal") or 0) > 0:
        return path
    return None


def finish(options):
    """Remove the log when the run had no failures."""
    path = (options or {}).get("_failure_log")
    if not path:
        return
    state_path = path + ".state.json"
    try:
        # A state file means at least one failure was appended. Keep both.
        if os.path.isfile(state_path):
            return
        if os.path.isfile(path):
            os.remove(path)
    except OSError:
        return
