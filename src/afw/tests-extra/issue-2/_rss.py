#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Sample /proc RSS of `afw -s script` running a hard-loop workload.

Used by rss_hard_loops.py (afwdev python mode) and as a CLI:

    python3 _rss.py integer_assign
    python3 _rss.py object_prop_assign --duration 20 --interval 5
"""

from __future__ import print_function

import argparse
import os
import signal
import subprocess
import sys
import time


HERE = os.path.dirname(os.path.abspath(__file__))
WORKLOAD_DIR = os.path.join(HERE, "_workloads")


def workload_path(name):
    if name.endswith(".as"):
        path = name if os.path.isabs(name) else os.path.join(WORKLOAD_DIR, name)
    else:
        path = os.path.join(WORKLOAD_DIR, name + ".as")
    if not os.path.isfile(path):
        raise ValueError("unknown workload: %s" % name)
    return path


def list_workloads():
    names = []
    if not os.path.isdir(WORKLOAD_DIR):
        return names
    for fn in sorted(os.listdir(WORKLOAD_DIR)):
        if fn.endswith(".as"):
            names.append(fn[:-3])
    return names


def _read_status_kib(pid):
    path = "/proc/%d/status" % pid
    out = {}
    with open(path, "r") as fd:
        for line in fd:
            if line.startswith("VmRSS:") or line.startswith("VmSize:") or \
                    line.startswith("VmData:") or line.startswith("VmPeak:"):
                parts = line.split()
                out[parts[0].rstrip(":")] = int(parts[1])
    return out


def _kill(proc):
    if proc.poll() is not None:
        return
    try:
        proc.send_signal(signal.SIGTERM)
    except OSError:
        return
    try:
        proc.wait(timeout=1.0)
    except Exception:
        try:
            proc.kill()
            proc.wait(timeout=1.0)
        except Exception:
            pass


def sample_afw_script(
        script_path,
        duration_s=8.0,
        interval_s=2.0,
        warmup_s=2.0,
        afw="afw"):
    """Spawn afw, sample RSS, kill. Returns a dict with samples and slope."""

    stderr = subprocess.PIPE
    try:
        proc = subprocess.Popen(
            [afw, "-s", "script", script_path],
            stdin=subprocess.DEVNULL,
            stdout=subprocess.DEVNULL,
            stderr=stderr,
        )
    except FileNotFoundError as e:
        raise RuntimeError(
            "afw not on PATH (./afwdev build --cdev --install): " + str(e)
        ) from e

    samples = []
    died_early = None
    ready_deadline = time.time() + 2.0
    while time.time() < ready_deadline:
        if proc.poll() is not None:
            break
        try:
            st = _read_status_kib(proc.pid)
            if (st.get("VmRSS") or 0) > 0:
                break
        except (IOError, OSError):
            break
        time.sleep(0.05)
    t0 = time.time()
    try:
        deadline = t0 + float(duration_s)
        next_sample = t0
        while True:
            now = time.time()
            if proc.poll() is not None:
                err = b""
                try:
                    err = proc.stderr.read() if proc.stderr else b""
                except Exception:
                    pass
                died_early = {
                    "returncode": proc.returncode,
                    "stderr": err.decode("utf-8", errors="replace")[-4000:],
                }
                break
            if now >= next_sample:
                try:
                    st = _read_status_kib(proc.pid)
                except (IOError, OSError):
                    break
                samples.append({
                    "t_s": round(now - t0, 3),
                    "pid": proc.pid,
                    "VmRSS": st.get("VmRSS"),
                    "VmSize": st.get("VmSize"),
                    "VmData": st.get("VmData"),
                    "VmPeak": st.get("VmPeak"),
                })
                next_sample = now + float(interval_s)
            if now >= deadline:
                break
            time.sleep(min(0.1, max(0.0, next_sample - time.time())))
    finally:
        _kill(proc)
        if proc.stderr:
            try:
                proc.stderr.close()
            except Exception:
                pass

    window = [s for s in samples if s["t_s"] + 1e-9 >= float(warmup_s)]
    if len(window) < 2:
        window = samples
    slope = None
    if len(window) >= 2 and window[0]["VmRSS"] is not None and \
            window[-1]["VmRSS"] is not None:
        dt = window[-1]["t_s"] - window[0]["t_s"]
        if dt > 0:
            slope = (window[-1]["VmRSS"] - window[0]["VmRSS"]) / dt

    return {
        "script": script_path,
        "pid": proc.pid,
        "samples": samples,
        "warmup_s": float(warmup_s),
        "duration_s": float(duration_s),
        "interval_s": float(interval_s),
        "slope_kib_s": slope,
        "died_early": died_early,
    }


def format_report(name, result):
    lines = []
    slope = result.get("slope_kib_s")
    slope_s = "n/a" if slope is None else "%.1f KiB/s (%.2f MiB/s)" % (
        slope, slope / 1024.0)
    lines.append("%s  slope=%s" % (name, slope_s))
    for s in result.get("samples") or []:
        rss = s.get("VmRSS")
        lines.append(
            "  t=%6.2fs  VmRSS=%s kB  VmData=%s kB  VmPeak=%s kB" % (
                s["t_s"], rss, s.get("VmData"), s.get("VmPeak")))
    early = result.get("died_early")
    if early:
        lines.append("  died early returncode=%s" % early.get("returncode"))
        err = (early.get("stderr") or "").strip()
        if err:
            lines.append("  stderr: %s" % err.splitlines()[-1])
    return "\n".join(lines)


def _cli(argv):
    p = argparse.ArgumentParser(
        description="Sample RSS of an issue-2 hard-loop Adaptive Script")
    p.add_argument(
        "workload",
        nargs="?",
        help="workload name (without .as) or path")
    p.add_argument("--list", action="store_true", help="list workloads")
    p.add_argument("--duration", type=float, default=15.0)
    p.add_argument("--interval", type=float, default=5.0)
    p.add_argument("--warmup", type=float, default=2.0)
    p.add_argument("--afw", default="afw")
    args = p.parse_args(argv)
    if args.list or not args.workload:
        for n in list_workloads():
            print(n)
        if not args.workload:
            return 0
    path = workload_path(args.workload)
    print("running %s for %.1fs (sample every %.1fs, warmup %.1fs)" % (
        path, args.duration, args.interval, args.warmup), file=sys.stderr)
    result = sample_afw_script(
        path,
        duration_s=args.duration,
        interval_s=args.interval,
        warmup_s=args.warmup,
        afw=args.afw,
    )
    print(format_report(args.workload, result))
    return 0


if __name__ == "__main__":
    sys.exit(_cli(sys.argv[1:]))
