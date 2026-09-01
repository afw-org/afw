# -*- coding: utf-8 -*-
"""
Spawn / stop installed afwfcgi for hermetic orchestrated leaves.

Binary is expected on PATH from `./afwdev build … --install`.
"""

import os
import signal
import subprocess
import time

from _afwdev.common import msg, resources
from _afwdev.common.errors import AfwdevProcessError


class AfwfcgiHostError(AfwdevProcessError):
    """Failed to start or manage afwfcgi."""


def build_afwfcgi_argv(
        conf_path,
        socket_path,
        threads=1,
        under_valgrind=False,
        valgrind_suppressions=None):
    """
    Build argv to start afwfcgi (optionally under valgrind).

    Hook point for --env-mode valgrind and future flags.
    """
    afwfcgi = "afwfcgi"
    server = [
        afwfcgi,
        "-f", conf_path,
        "-p", socket_path,
        "-n", str(threads),
    ]
    if not under_valgrind:
        return server

    vg = [
        "valgrind",
        "--xml=yes",
        "--xml-fd=2",
        "--show-possibly-lost=no",
    ]
    if valgrind_suppressions and os.path.isfile(valgrind_suppressions):
        vg.append("--suppressions=" + valgrind_suppressions)
    return vg + server


def start_afwfcgi(
        work_dir,
        threads=1,
        under_valgrind=False,
        options=None,
        ready_timeout_s=15.0):
    """
    Start afwfcgi in work_dir with afw.conf and Unix socket afw.sock.

    Returns a handle dict: process, socket_path, conf_path, log_path, argv
    """
    conf_path = os.path.join(work_dir, "afw.conf")
    if not os.path.isfile(conf_path):
        raise AfwfcgiHostError(
            "orchestrated leaf requires afw.conf in work dir: " + work_dir)

    socket_path = os.path.join(work_dir, "afw.sock")
    if os.path.exists(socket_path):
        try:
            os.unlink(socket_path)
        except OSError:
            pass

    suppressions = None
    if under_valgrind and options is not None:
        # Copy test resources (valgrind.suppress) into work_dir like modes/valgrind.py
        try:
            resources.copy_resources(options, "test/", todir=work_dir)
            cand = os.path.join(work_dir, "valgrind.suppress")
            if os.path.isfile(cand):
                suppressions = cand
        except Exception as e:
            msg.debug("valgrind suppressions copy failed: {}".format(e))

    argv = build_afwfcgi_argv(
        conf_path,
        socket_path,
        threads=threads,
        under_valgrind=under_valgrind,
        valgrind_suppressions=suppressions,
    )

    log_path = os.path.join(work_dir, "afwfcgi.stderr.log")
    log_fd = open(log_path, "wb")

    msg.debug("Starting afwfcgi: " + " ".join(argv))
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
        raise AfwfcgiHostError(
            "afwfcgi not found on PATH (install with "
            "./afwdev build --cdev or --fulldev): {}".format(e)
        ) from e
    except Exception as e:
        log_fd.close()
        raise AfwfcgiHostError(
            "Failed to spawn afwfcgi: {}".format(e)) from e

    handle = {
        "process": proc,
        "socket_path": socket_path,
        "conf_path": conf_path,
        "log_path": log_path,
        "log_fd": log_fd,
        "argv": argv,
        "under_valgrind": under_valgrind,
    }

    deadline = time.time() + ready_timeout_s
    while time.time() < deadline:
        if proc.poll() is not None:
            log_fd.flush()
            err = _read_log_tail(log_path)
            stop_afwfcgi(handle)
            raise AfwfcgiHostError(
                "afwfcgi exited during startup (code {}): {}".format(
                    proc.returncode, err or "(no stderr)"))
        if os.path.exists(socket_path):
            # brief settle so accept loop is up
            time.sleep(0.05)
            return handle
        time.sleep(0.05)

    stop_afwfcgi(handle)
    raise AfwfcgiHostError(
        "Timeout waiting for afwfcgi socket {!r} ({}s). stderr: {}".format(
            socket_path, ready_timeout_s, _read_log_tail(log_path)))


def stop_afwfcgi(handle, grace_s=5.0):
    """Terminate afwfcgi process group and remove socket."""
    if not handle:
        return
    proc = handle.get("process")
    log_fd = handle.get("log_fd")
    socket_path = handle.get("socket_path")

    if proc is not None and proc.poll() is None:
        try:
            os.killpg(proc.pid, signal.SIGTERM)
        except (ProcessLookupError, PermissionError, OSError):
            try:
                proc.terminate()
            except Exception:
                pass
        deadline = time.time() + grace_s
        while time.time() < deadline and proc.poll() is None:
            time.sleep(0.05)
        if proc.poll() is None:
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

    if log_fd is not None:
        try:
            log_fd.close()
        except Exception:
            pass
        handle["log_fd"] = None

    if socket_path and os.path.exists(socket_path):
        try:
            os.unlink(socket_path)
        except OSError:
            pass


def valgrind_errors_in_log(log_path):
    """True if valgrind XML in stderr log reports an <error>."""
    if not log_path or not os.path.isfile(log_path):
        return False
    try:
        with open(log_path, "rb") as fd:
            data = fd.read()
        return b"<error>" in data
    except OSError:
        return False


def _read_log_tail(log_path, max_bytes=8000):
    try:
        with open(log_path, "rb") as fd:
            data = fd.read()
        if len(data) > max_bytes:
            data = data[-max_bytes:]
        return data.decode("utf-8", errors="replace")
    except OSError:
        return ""
