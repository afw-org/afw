# -*- coding: utf-8 -*-
"""Load and validate orchestration.yaml / .json documents."""

import os

from _afwdev.common import nfc
from _afwdev.common.errors import AfwdevRunnerError

try:
    import yaml
except ImportError:  # pragma: no cover
    yaml = None


class OrchestrationLoadError(AfwdevRunnerError):
    """Invalid or unreadable orchestration marker."""


_SOURCE_TYPES_EVAL = {
    "script": "eval<script>",
    "template": "eval<template>",
    "test_script": "eval<script>",
    # expression source is Adaptive expression text; feed as script with return
    "expression": "eval<script>",
}


def load_orchestration_document(marker_path):
    """
    Load orchestration.yaml or .json and validate v1 sequential schema
    (plus optional schedule.firehose / sequential / parallel).

    Returns the document dict (normalized defaults applied).
    """
    if not os.path.isfile(marker_path):
        raise OrchestrationLoadError("Marker not found: " + marker_path)

    with nfc.open(marker_path, "r") as fd:
        text = fd.read()

    if marker_path.endswith(".yaml") or marker_path.endswith(".yml"):
        if yaml is None:
            raise OrchestrationLoadError(
                "PyYAML is required to load orchestration.yaml "
                "(install PyYAML / project python-requirements)")
        try:
            raw = yaml.safe_load(text)
        except Exception as e:
            raise OrchestrationLoadError(
                "Failed to parse YAML {}: {}".format(marker_path, e)) from e
    elif marker_path.endswith(".json"):
        try:
            raw = nfc.json_loads(text)
        except Exception as e:
            raise OrchestrationLoadError(
                "Failed to parse JSON {}: {}".format(marker_path, e)) from e
    else:
        raise OrchestrationLoadError(
            "Marker must be orchestration.yaml or orchestration.json: "
            + marker_path)

    if not isinstance(raw, dict):
        raise OrchestrationLoadError(
            "orchestration document must be a mapping/object: " + marker_path)

    host = raw.get("host")
    if not host:
        raise OrchestrationLoadError(
            "orchestration requires 'host' field: " + marker_path)
    # afwfcgi = FastCGI hermetic server; local / afw-local = afw --local stdin
    if host not in ("afwfcgi", "local", "afw-local"):
        raise OrchestrationLoadError(
            "orchestration host {!r} not supported "
            "(afwfcgi, local, afw-local): {}".format(host, marker_path))
    if host == "afw-local":
        raw["host"] = "local"

    tests = raw.get("tests")
    if not isinstance(tests, list) or len(tests) == 0:
        raise OrchestrationLoadError(
            "orchestration requires non-empty 'tests' list: " + marker_path)

    names = set()
    for i, item in enumerate(tests):
        if not isinstance(item, dict):
            raise OrchestrationLoadError(
                "tests[{}] must be a mapping: {}".format(i, marker_path))
        name = item.get("name")
        if not name or not isinstance(name, str):
            raise OrchestrationLoadError(
                "tests[{}] requires string 'name': {}".format(i, marker_path))
        if name in names:
            raise OrchestrationLoadError(
                "duplicate tests[].name {!r}: {}".format(name, marker_path))
        names.add(name)

        if item.get("skip"):
            continue

        if "feed" in item and item["feed"] is not None:
            if not isinstance(item["feed"], dict):
                raise OrchestrationLoadError(
                    "tests[{}] ({!r}) 'feed' must be a mapping: {}".format(
                        i, name, marker_path))

        # Effective feed kind (document default applied later if missing).
        item_feed = item.get("feed") if isinstance(item.get("feed"), dict) else {}
        doc_feed = raw.get("feed") if isinstance(raw.get("feed"), dict) else {}
        kind = item_feed.get("kind") or doc_feed.get("kind") or "action"
        # REST: method/path only. local: needs stdin body (source).
        needs_source = kind not in ("rest",)

        has_source = item.get("source") is not None
        has_path = item.get("sourcePath") is not None
        if has_source and has_path:
            raise OrchestrationLoadError(
                "tests[{}] ({!r}) must not set both 'source' and 'sourcePath': "
                "{}".format(i, name, marker_path))
        if needs_source and not has_source and not has_path:
            raise OrchestrationLoadError(
                "tests[{}] ({!r}) requires 'source' or 'sourcePath' "
                "(unless feed.kind is rest): {}".format(i, name, marker_path))
        if has_path and not isinstance(item.get("sourcePath"), str):
            raise OrchestrationLoadError(
                "tests[{}] ({!r}) 'sourcePath' must be a string: {}".format(
                    i, name, marker_path))
        if has_source and not isinstance(item.get("source"), str):
            raise OrchestrationLoadError(
                "tests[{}] ({!r}) 'source' must be a string: {}".format(
                    i, name, marker_path))

        st = item.get("sourceType") or "script"
        if not isinstance(st, str):
            raise OrchestrationLoadError(
                "tests[{}] ({!r}) 'sourceType' must be a string: {}".format(
                    i, name, marker_path))
        item["sourceType"] = st

    if "feed" in raw and raw["feed"] is not None:
        if not isinstance(raw["feed"], dict):
            raise OrchestrationLoadError(
                "document 'feed' must be a mapping: " + marker_path)
    else:
        # Both hosts default to FCGI-like action authoring; raw local protocol
        # is feed.kind: local on host local.
        raw["feed"] = {"kind": "action", "accept": "application/json"}
    if "timeout_s" in raw and raw["timeout_s"] is not None:
        try:
            raw["timeout_s"] = float(raw["timeout_s"])
        except (TypeError, ValueError) as e:
            raise OrchestrationLoadError(
                "timeout_s must be a number: " + marker_path) from e
    else:
        raw["timeout_s"] = 120.0

    afwfcgi = raw.get("afwfcgi")
    if afwfcgi is None:
        raw["afwfcgi"] = {"threads": 1}
    elif not isinstance(afwfcgi, dict):
        raise OrchestrationLoadError(
            "afwfcgi block must be a mapping: " + marker_path)
    else:
        threads = afwfcgi.get("threads", 1)
        try:
            threads = int(threads)
        except (TypeError, ValueError) as e:
            raise OrchestrationLoadError(
                "afwfcgi.threads must be an integer: " + marker_path) from e
        if threads < 1:
            raise OrchestrationLoadError(
                "afwfcgi.threads must be >= 1: " + marker_path)
        afwfcgi["threads"] = threads

    if raw.get("schedule") is not None:
        if not isinstance(raw["schedule"], list):
            raise OrchestrationLoadError(
                "schedule must be a list of phases: " + marker_path)

    if not raw.get("description"):
        raw["description"] = os.path.basename(os.path.dirname(marker_path))

    return raw


def merge_feed(document_feed, test_feed):
    """Document feed defaults; test feed overrides field-by-field."""
    out = {}
    if document_feed:
        out.update(document_feed)
    if test_feed:
        for k, v in test_feed.items():
            if v is not None:
                out[k] = v
    if "kind" not in out:
        out["kind"] = "action"
    if "accept" not in out and out.get("kind") == "action":
        out["accept"] = "application/json"
    return out


def parse_triple_lt_path(value):
    """
    If value is a string of the form '<<< rel/path' (optional whitespace),
    return the relative path string. Otherwise return None.
    """
    if not isinstance(value, str):
        return None
    stripped = value.lstrip()
    if not stripped.startswith("<<<"):
        return None
    rel = stripped[3:].strip()
    if "\n" in rel:
        rel = rel.split("\n", 1)[0].strip()
    return rel or None


def _validate_rel_path(rel, item_name, what):
    if not rel:
        raise AfwdevRunnerError(
            "test {!r}: {!r} '<<<' requires a path".format(item_name, what))
    if rel.startswith("/") or rel.startswith("\\") or (
            len(rel) >= 2 and rel[1] == ":"):
        raise AfwdevRunnerError(
            "test {!r}: {!r} '<<<' path must be relative".format(
                item_name, what))
    parts = rel.replace("\\", "/").split("/")
    if ".." in parts or any(p == "" for p in parts):
        raise AfwdevRunnerError(
            "test {!r}: invalid {!r} '<<<' path {!r}".format(
                item_name, what, rel))
    return rel


def resolve_file_bytes(value, work_dir, item_name=None, what="value",
                       missing_ok=False):
    """
    Resolve a string value that may be '<<< rel/path' to raw bytes.

    Inline (non-<<<) strings are UTF-8 encoded. Returns (bytes|None, rel_path|None)
    where rel_path is set only for the <<< form (for golden capture).
    """
    name = item_name or "?"
    rel = parse_triple_lt_path(value)
    if rel is not None:
        rel = _validate_rel_path(rel, name, what)
        path = os.path.join(work_dir, rel)
        if not os.path.isfile(path):
            if missing_ok:
                return None, rel
            raise AfwdevRunnerError(
                "test {!r}: {!r} '<<<' file not found: {} "
                "(create it with: afwdev test --capture-goldens -T <leaf>)"
                .format(name, what, path))
        with nfc.open(path, "rb") as fd:
            return fd.read(), rel
    if value is None:
        return None, None
    if isinstance(value, bytes):
        return value, None
    if isinstance(value, str):
        return value.encode("utf-8"), None
    raise AfwdevRunnerError(
        "test {!r}: {!r} must be a string or bytes".format(name, what))


def resolve_file_text(value, work_dir, item_name=None, what="value",
                      missing_ok=False):
    """Like resolve_file_bytes but returns unicode text (UTF-8 for files)."""
    data, rel = resolve_file_bytes(
        value, work_dir, item_name=item_name, what=what, missing_ok=missing_ok)
    if data is None:
        return None, rel
    if isinstance(data, bytes):
        return data.decode("utf-8"), rel
    return data, rel


def resolve_source_text(item, work_dir):
    """
    Return source string for a test item.

    Supports sourcePath, source with <<< rel/path, or inline source.
    Paths are relative to the leaf work_dir.
    """
    if item.get("sourcePath"):
        rel = item["sourcePath"]
        path = os.path.join(work_dir, rel)
        if not os.path.isfile(path):
            raise AfwdevRunnerError("sourcePath not found: " + path)
        with nfc.open(path, "r") as fd:
            return fd.read()

    src = item.get("source")
    if src is None:
        raise AfwdevRunnerError(
            "test {!r} has no source".format(item.get("name")))

    text, _rel = resolve_file_text(
        src, work_dir, item_name=item.get("name"), what="source")
    return text


def eval_function_for_source_type(source_type, feed):
    """Pick Adaptive function id for action feed from sourceType / feed.function."""
    if feed.get("function"):
        return feed["function"]
    return _SOURCE_TYPES_EVAL.get(source_type or "script", "eval<script>")
