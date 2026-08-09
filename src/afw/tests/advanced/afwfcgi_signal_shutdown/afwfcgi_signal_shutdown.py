#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Hermetic check: afwfcgi exits on SIGTERM/SIGINT without needing SIGKILL (#158).

Spawns installed afwfcgi (-n 2), waits for the Unix socket, signals the
process group, and requires process exit within a short grace period.
Not an advanced-test marker leaf (no multi-step FCGI client); process
lifecycle only.
"""

from __future__ import print_function

import os
import signal
import subprocess
import tempfile
import time


MINIMAL_CONF = """\
[
  {
    "type": "application",
    "applicationId": "signal_shutdown_smoke"
  }
]
"""


def _case(name, description, passed, error=None, skip=False):
    return {
        "test": name,
        "description": description,
        "passed": bool(passed),
        "skip": bool(skip),
        "error": error,
    }


def _spawn_afwfcgi(work_dir, threads=2):
    conf_path = os.path.join(work_dir, "afw.conf")
    with open(conf_path, "w", encoding="utf-8") as fd:
        fd.write(MINIMAL_CONF)

    socket_path = os.path.join(work_dir, "afw.sock")
    if os.path.exists(socket_path):
        try:
            os.unlink(socket_path)
        except OSError:
            pass

    log_path = os.path.join(work_dir, "afwfcgi.stderr.log")
    log_fd = open(log_path, "wb")
    argv = [
        "afwfcgi",
        "-f", conf_path,
        "-p", socket_path,
        "-n", str(threads),
    ]
    try:
        proc = subprocess.Popen(
            argv,
            cwd=work_dir,
            stdin=subprocess.DEVNULL,
            stdout=subprocess.DEVNULL,
            stderr=log_fd,
            start_new_session=True,
        )
    except FileNotFoundError as e:
        log_fd.close()
        raise RuntimeError(
            "afwfcgi not on PATH (./afwdev build --cdev --install): "
            + str(e)
        ) from e

    return {
        "process": proc,
        "socket_path": socket_path,
        "log_path": log_path,
        "log_fd": log_fd,
    }


def _wait_socket(handle, timeout_s=15.0):
    proc = handle["process"]
    socket_path = handle["socket_path"]
    deadline = time.time() + timeout_s
    while time.time() < deadline:
        if proc.poll() is not None:
            raise RuntimeError(
                "afwfcgi exited during startup (code {}): {}".format(
                    proc.returncode, _log_tail(handle))
            )
        if os.path.exists(socket_path):
            time.sleep(0.05)
            return
        time.sleep(0.05)
    raise RuntimeError(
        "Timeout waiting for socket {!r}: {}".format(
            socket_path, _log_tail(handle))
    )


def _log_tail(handle, max_bytes=4000):
    path = handle.get("log_path")
    if not path or not os.path.isfile(path):
        return "(no log)"
    try:
        with open(path, "rb") as fd:
            data = fd.read()
        if len(data) > max_bytes:
            data = data[-max_bytes:]
        return data.decode("utf-8", errors="replace")
    except OSError:
        return "(log unreadable)"


def _signal_and_wait(handle, sig, grace_s=5.0):
    """
    Send sig to process group. Return dict:
      exited (bool), used_sigkill (bool), returncode, elapsed_s
    """
    proc = handle["process"]
    t0 = time.time()
    used_sigkill = False

    if proc.poll() is None:
        try:
            os.killpg(proc.pid, sig)
        except (ProcessLookupError, PermissionError, OSError):
            try:
                proc.send_signal(sig)
            except Exception:
                pass

        deadline = time.time() + grace_s
        while time.time() < deadline and proc.poll() is None:
            time.sleep(0.05)

        if proc.poll() is None:
            used_sigkill = True
            try:
                os.killpg(proc.pid, signal.SIGKILL)
            except (ProcessLookupError, PermissionError, OSError):
                try:
                    proc.kill()
                except Exception:
                    pass
            try:
                proc.wait(timeout=2.0)
            except Exception:
                pass

    elapsed = time.time() - t0
    return {
        "exited": proc.poll() is not None,
        "used_sigkill": used_sigkill,
        "returncode": proc.returncode,
        "elapsed_s": elapsed,
    }


def _cleanup(handle):
    if not handle:
        return
    proc = handle.get("process")
    log_fd = handle.get("log_fd")
    socket_path = handle.get("socket_path")
    if proc is not None and proc.poll() is None:
        try:
            os.killpg(proc.pid, signal.SIGKILL)
        except Exception:
            try:
                proc.kill()
            except Exception:
                pass
        try:
            proc.wait(timeout=2.0)
        except Exception:
            pass
    if log_fd is not None:
        try:
            log_fd.close()
        except Exception:
            pass
    if socket_path and os.path.exists(socket_path):
        try:
            os.unlink(socket_path)
        except OSError:
            pass


def _run_signal_case(sig, sig_name, threads=2, grace_s=5.0):
    work = tempfile.mkdtemp(prefix="afwfcgi_sig_")
    handle = None
    try:
        handle = _spawn_afwfcgi(work, threads=threads)
        _wait_socket(handle)
        socket_path = handle["socket_path"]
        result = _signal_and_wait(handle, sig, grace_s=grace_s)
        if result["used_sigkill"] or not result["exited"]:
            return False, (
                "{} did not stop afwfcgi within {}s without SIGKILL "
                "(returncode={}, elapsed={:.2f}s). stderr: {}".format(
                    sig_name,
                    grace_s,
                    result["returncode"],
                    result["elapsed_s"],
                    _log_tail(handle),
                )
            )
        # Unix listen path should be unlinked on clean exit (#158).
        if os.path.exists(socket_path):
            return False, (
                "{}: process exited but Unix socket path still exists: "
                "{!r}".format(sig_name, socket_path)
            )
        return True, None
    except Exception as e:
        return False, str(e)
    finally:
        _cleanup(handle)


def run():
    description = (
        "afwfcgi graceful stop on SIGTERM/SIGINT without SIGKILL (#158)"
    )
    tests = []

    for sig, name in (
        (signal.SIGTERM, "SIGTERM"),
        (signal.SIGINT, "SIGINT"),
    ):
        ok, err = _run_signal_case(sig, name, threads=2, grace_s=5.0)
        tests.append(
            _case(
                "afwfcgi_{}_n2".format(name.lower()),
                "afwfcgi -n 2 exits on {} within 5s without SIGKILL".format(
                    name),
                passed=ok,
                error=err,
            )
        )

    return {
        "description": description,
        "tests": tests,
    }
