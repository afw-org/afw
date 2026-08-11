# -*- coding: utf-8 -*-
"""
Run one `afw --local` session (stdin protocol) for orchestrated-test leaves.

Binary is expected on PATH from `./afwdev build … --install`.
Output path/fd is stdout (``1``) so the harness captures the local protocol
response the same way the historical local_test.py did.
"""

import os
import re
import subprocess

from _afwdev.common.errors import AfwdevProcessError


class AfwLocalHostError(AfwdevProcessError):
    """Failed to run afw --local."""


# Length-prefixed local-mode greeting (first stdout chunk).
_LOCAL_BANNER_RE = re.compile(
    br"^\d+\n"
    br"afw [0-9A-Za-z._+-]+\n"
    br"\n"
    br"Local mode\.\n"
)


def normalize_local_stdout(data):
    """Stabilize version-dependent local-mode banner for comparison."""
    if not isinstance(data, (bytes, bytearray)):
        data = str(data).encode("utf-8", errors="surrogateescape")
    return _LOCAL_BANNER_RE.sub(b"@@AFW_LOCAL_BANNER@@\n", data, count=1)


def run_afw_local(
        input_bytes,
        work_dir,
        conf_path=None,
        under_valgrind=False,
        timeout=120.0):
    """
    Spawn ``afw --local 1`` [``-f conf``], feed input_bytes on stdin, return
    stdout bytes. Raises AfwLocalHostError on non-zero exit or spawn failure.
    """
    if input_bytes is None:
        input_bytes = b""
    if not isinstance(input_bytes, (bytes, bytearray)):
        input_bytes = str(input_bytes).encode("utf-8")

    argv = ["afw", "--local", "1"]
    if conf_path and os.path.isfile(conf_path):
        argv.extend(["-f", conf_path])

    if under_valgrind:
        argv = [
            "valgrind",
            "--error-exitcode=99",
            "--leak-check=no",
            "--show-possibly-lost=no",
        ] + argv

    try:
        result = subprocess.run(
            argv,
            input=bytes(input_bytes),
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            cwd=work_dir,
            timeout=float(timeout),
        )
    except FileNotFoundError as e:
        raise AfwLocalHostError(
            "afw not found on PATH (install with "
            "./afwdev build --cdev or --fulldev --install): {}".format(e)
        ) from e
    except subprocess.TimeoutExpired as e:
        raise AfwLocalHostError(
            "afw --local timed out after {}s".format(timeout)
        ) from e
    except Exception as e:
        raise AfwLocalHostError(
            "Failed to run afw --local: {}".format(e)) from e

    # Historical local_test.py ignored process exit code: afw --local often
    # exits non-zero after a successful session (status on stderr only).
    # Fail only when there is no stdout and a non-zero exit.
    out = result.stdout or b""
    if result.returncode != 0 and not out:
        err = (result.stderr or b"").decode("utf-8", errors="replace")
        raise AfwLocalHostError(
            "afw --local exit {} with empty stdout: {}".format(
                result.returncode, err[:1500] or "(no stderr)"),
            returncode=result.returncode,
        )

    return out
