# -*- coding: utf-8 -*-
"""
Parse application/x-afw framed response bodies and map orchestration expects.

Frame header (from afw_content_type_application_afw.c flush):
  <sequence> <size> <streamId>[ <info>...]\n
  <size bytes of payload>

Orchestration keys (plan):
  expect-<streamId>       — concatenated payloads only (no headers)
  expect-raw-<streamId>   — concatenated \"header\\npayload\" per frame
  expect-raw-response     — entire raw body (also alias expectResponse)
  expect-response         — payloads for streamId \"response\" only

expect-raw-response is always the full body (not streamId \"response\" raw frames).
"""

from __future__ import print_function

import re

# Local-mode banner on stdout (same as hosts/local.py)
_LOCAL_BANNER_RE = re.compile(
    br"^\d+\n"
    br"afw [0-9A-Za-z._+-]+\n"
    br"\n"
    br"Local mode\.\n"
)


def strip_local_banner(data):
    if not isinstance(data, (bytes, bytearray)):
        data = bytes(data or b"")
    else:
        data = bytes(data)
    m = _LOCAL_BANNER_RE.match(data)
    if m:
        return data[m.end():]
    return data


def local_output_app_body(stdout_bytes):
    """
    Reconstruct application response bytes from afw --local stdout.

    Local writes each write() as ``len\\n`` + content; ``0\\n`` ends a request.
    Drop the version banner chunk(s), concatenate remaining non-empty chunks.
    """
    data = strip_local_banner(stdout_bytes)
    pieces = []
    i = 0
    n = len(data)
    while i < n:
        nl = data.find(b"\n", i)
        if nl < 0:
            break
        length_s = data[i:nl].decode("ascii", errors="replace").strip()
        if not length_s.isdigit():
            break
        length = int(length_s)
        i = nl + 1
        if length == 0:
            # request end marker; continue (multi-request) or stop
            continue
        if i + length > n:
            pieces.append(data[i:])
            break
        pieces.append(data[i:i + length])
        i += length
    return b"".join(pieces)


def demux_x_afw(body):
    """
    Demux an application/x-afw body.

    Returns dict:
      frames: list of {seq, size, streamId, info, payload, raw_frame}
      payloads: {streamId: bytes}  concatenated payloads
      raw_frames: {streamId: bytes}  concatenated header+payload per frame
      raw: original body bytes
    """
    if body is None:
        body = b""
    if not isinstance(body, (bytes, bytearray)):
        body = bytes(body)
    else:
        body = bytes(body)

    frames = []
    payloads = {}
    raw_frames = {}
    i = 0
    n = len(body)

    while i < n:
        # Skip lone newlines
        if body[i:i + 1] in (b"\n", b"\r"):
            i += 1
            continue
        nl = body.find(b"\n", i)
        if nl < 0:
            break
        header_line = body[i:nl].decode("utf-8", errors="replace")
        header_raw = body[i:nl + 1]
        i = nl + 1
        parts = header_line.split()
        if len(parts) < 3:
            # Not a frame header; stop demux
            break
        try:
            seq = int(parts[0])
            size = int(parts[1])
        except ValueError:
            break
        stream_id = parts[2]
        info = " ".join(parts[3:]) if len(parts) > 3 else ""
        if size < 0 or i + size > n:
            # Truncated
            payload = body[i:]
            i = n
        else:
            payload = body[i:i + size]
            i += size
        raw_frame = header_raw + payload
        frames.append({
            "seq": seq,
            "size": size,
            "streamId": stream_id,
            "info": info,
            "payload": payload,
            "raw_frame": raw_frame,
        })
        payloads[stream_id] = payloads.get(stream_id, b"") + payload
        raw_frames[stream_id] = raw_frames.get(stream_id, b"") + raw_frame

    return {
        "frames": frames,
        "payloads": payloads,
        "raw_frames": raw_frames,
        "raw": body,
    }


def _is_expect_key(key):
    if not key or not isinstance(key, str):
        return False
    if key == "expectResponse":
        return True
    if key == "expect":
        return False
    if key.startswith("expect-"):
        return True
    return False


def classify_expect_key(key):
    """
    Return (kind, stream_id_or_None).

    kind: 'full_raw' | 'payload' | 'stream_raw' | None
    """
    if key == "expectResponse" or key == "expect-raw-response":
        return "full_raw", None
    if not key.startswith("expect-"):
        return None, None
    rest = key[len("expect-"):]
    if rest.startswith("raw-"):
        sid = rest[len("raw-"):]
        if not sid or sid == "response":
            # expect-raw-response already handled; expect-raw- with empty → none
            if sid == "response":
                return "full_raw", None
            return None, None
        return "stream_raw", sid
    if not rest:
        return None, None
    return "payload", rest


def collect_stream_expect_specs(item):
    """
    From a test item dict, list of
      {key, kind, stream_id, expected_spec}
    for stream/raw expects (not the Adaptive 'expect' value).
    """
    specs = []
    if not isinstance(item, dict):
        return specs
    for key, val in item.items():
        if not _is_expect_key(key):
            continue
        kind, sid = classify_expect_key(key)
        if kind is None:
            continue
        specs.append({
            "key": key,
            "kind": kind,
            "stream_id": sid,
            "expected_spec": val,
        })
    return specs


def flags_for_stream_expects(item, accept):
    """
    Adaptive _flags_ for side streams referenced by expects.
    JSON: response:stdout; x-afw: response:stdout:stream.
    """
    accept = (accept or "").strip().lower()
    is_xafw = "x-afw" in accept or accept == "application/afw"
    flags = []
    seen = set()
    for spec in collect_stream_expect_specs(item):
        if spec["kind"] == "full_raw":
            continue
        sid = spec.get("stream_id")
        if sid in (None, "response", "end"):
            continue
        if sid in seen:
            continue
        seen.add(sid)
        if is_xafw:
            flags.append("response:{}:stream".format(sid))
        else:
            flags.append("response:{}".format(sid))
    return flags


def actual_bytes_for_spec(spec, demuxed, full_body):
    """Compute actual bytes for one expect spec from demux result / full body."""
    kind = spec["kind"]
    if kind == "full_raw":
        return full_body if full_body is not None else demuxed.get("raw", b"")
    sid = spec["stream_id"]
    if kind == "payload":
        return demuxed.get("payloads", {}).get(sid, b"")
    if kind == "stream_raw":
        return demuxed.get("raw_frames", {}).get(sid, b"")
    return b""


def try_action_result_from_demux(demuxed):
    """
    Best-effort extract last action result from response-stream JSON payloads.
    """
    import json
    payloads = demuxed.get("payloads", {}).get("response", b"")
    if not payloads:
        return None
    # Payloads may be multiple JSON objects concatenated. Try progressive scan.
    text = payloads.decode("utf-8", errors="replace")
    # Split heuristically on }{ boundaries
    chunks = []
    depth = 0
    start = None
    for idx, ch in enumerate(text):
        if ch == "{":
            if depth == 0:
                start = idx
            depth += 1
        elif ch == "}":
            depth -= 1
            if depth == 0 and start is not None:
                chunks.append(text[start:idx + 1])
                start = None
    last_result = None
    for ch in chunks:
        try:
            obj = json.loads(ch)
        except Exception:
            continue
        if not isinstance(obj, dict):
            continue
        if "result" in obj and not obj.get("intermediate"):
            last_result = obj.get("result")
        elif "result" in obj:
            last_result = obj.get("result")
        if obj.get("status") == "error":
            return {"__error__": obj}
    return last_result
