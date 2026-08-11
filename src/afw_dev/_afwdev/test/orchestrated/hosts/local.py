# -*- coding: utf-8 -*-
"""
Run `afw --local` for orchestrated-test leaves.

Two authoring modes (same host):

* **High-level (FCGI-like):** harness builds length-framed segments for
  ``++afw-local-mode-action`` + JSON perform body (``feed.kind: action``).
* **Raw protocol:** caller supplies complete stdin bytes (``feed.kind: local``).

Binary is expected on PATH from ``./afwdev build … --install``.
Output path/fd is stdout (``1``). Optional ``-f conf`` when the leaf has
``afw.conf`` (flags, adapters, etc.).
"""

import json
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


def encode_segment(payload):
    """
    One local-mode input segment: length-prefixed chunks ending with ``0\\n``.

    A single UTF-8 body is one chunk then terminator.
    """
    if payload is None:
        payload = b""
    if isinstance(payload, str):
        payload = payload.encode("utf-8")
    if not isinstance(payload, (bytes, bytearray)):
        payload = bytes(payload)
    if len(payload) == 0:
        return b"0\n"
    return (
        str(len(payload)).encode("ascii") + b"\n" +
        bytes(payload) + b"0\n"
    )


def build_action_session_stdin(accept, body_obj):
    """
    Encode a single perform-like request as local stdin (then exit).

    Uses ``++afw-local-mode-action`` (HTTP-like path) so Accept application/x-afw
    and response stream flags work like FCGI.
    """
    accept = (accept or "application/json").strip()
    # Compact directive object (relaxed JSON not required; strict is fine)
    directive = '++afw-local-mode-action:{"ACCEPT":%s}' % (
        json.dumps(accept),
    )
    if isinstance(body_obj, (bytes, bytearray, str)):
        body = body_obj
        if isinstance(body, str):
            body = body.encode("utf-8")
    else:
        body = json.dumps(body_obj, separators=(",", ":")).encode("utf-8")

    return (
        encode_segment(directive) +
        encode_segment(body) +
        encode_segment(b"exit")
    )


def parse_local_json_responses(stdout_bytes):
    """
    Parse local stdout into a list of JSON objects (response bodies).

    Skips the version banner segment. Returns [] if nothing parses as JSON
    (e.g. pure application/x-afw wire).
    """
    data = bytes(stdout_bytes or b"")
    # Drop banner if present
    m = _LOCAL_BANNER_RE.match(data)
    if m:
        data = data[m.end():]

    responses = []
    i = 0
    n = len(data)
    while i < n:
        # Skip bare 0\n terminators / whitespace
        if data[i:i + 2] == b"0\n":
            i += 2
            continue
        if data[i:i + 1] in (b"\n", b"\r", b" "):
            i += 1
            continue
        # Length line
        nl = data.find(b"\n", i)
        if nl < 0:
            break
        length_s = data[i:nl].decode("ascii", errors="replace").strip()
        if not length_s.isdigit():
            break
        length = int(length_s)
        i = nl + 1
        if length == 0:
            continue
        chunk = data[i:i + length]
        i += length
        # Optional 0\n after chunk (segment end) is handled next loop
        text = chunk.decode("utf-8", errors="replace").strip()
        if not text:
            continue
        if text[0] not in "{[":
            # x-afw frame header line or other non-JSON — skip
            continue
        try:
            responses.append(json.loads(text))
        except Exception:
            continue
    return responses


def primary_json_response(stdout_bytes):
    """
    Best JSON response object for expect / expect-stdout (last successful-ish).
    """
    responses = parse_local_json_responses(stdout_bytes)
    if not responses:
        return None
    # Prefer last with status success
    for r in reversed(responses):
        if isinstance(r, dict) and r.get("status") == "success":
            return r
    return responses[-1]


def run_afw_local(
        input_bytes,
        work_dir,
        conf_path=None,
        under_valgrind=False,
        timeout=120.0):
    """
    Spawn ``afw --local 1`` [``-f conf``], feed input_bytes on stdin, return
    stdout bytes. Raises AfwLocalHostError on spawn failure or empty failure.
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

    # Historical: afw --local often exits non-zero after a useful session.
    out = result.stdout or b""
    if result.returncode != 0 and not out:
        err = (result.stderr or b"").decode("utf-8", errors="replace")
        raise AfwLocalHostError(
            "afw --local exit {} with empty stdout: {}".format(
                result.returncode, err[:1500] or "(no stderr)"),
            returncode=result.returncode,
        )

    return out
