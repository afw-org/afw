# -*- coding: utf-8 -*-
"""Load and validate orchestration.yaml / .json documents."""

import os
import re

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


_PARAM_REF = re.compile(r"^\$([A-Za-z_][A-Za-z0-9_]*)$")
_ENV_MODES = ("afw", "afwfcgi", "actions", "valgrind", "asan", "tsan")


def parse_sets(values):
    """--set NAME=VALUE / LEAF:NAME=VALUE / @FILE -> [(leaf, name, text)].

    leaf is None for a value every leaf that declares NAME gets. @FILE is
    a YAML or JSON mapping of the same keys to values.
    """
    out = []
    for item in values or []:
        if item.startswith("@"):
            path = os.path.expanduser(item[1:])
            try:
                with nfc.open(path, "r") as fd:
                    text = fd.read()
                data = (yaml.safe_load(text) if yaml is not None
                        else nfc.json_loads(text))
            except Exception as e:
                raise OrchestrationLoadError(
                    "--set {}: {}".format(item, e)) from e
            if not isinstance(data, dict):
                raise OrchestrationLoadError(
                    "--set {}: must be a mapping".format(item))
            pairs = [(str(k), v) for k, v in data.items()]
        else:
            if "=" not in item:
                raise OrchestrationLoadError(
                    "--set wants NAME=VALUE or LEAF:NAME=VALUE, got {!r}"
                    .format(item))
            key, value = item.split("=", 1)
            pairs = [(key, value)]
        for key, value in pairs:
            leaf, _, name = key.rpartition(":")
            if not name:
                raise OrchestrationLoadError(
                    "--set {!r}: no parameter name".format(item))
            out.append((leaf or None, name, value))
    return out


def _convert(text, default, name):
    """A --set value as the type of the parameter's default."""
    if not isinstance(text, str):
        return text
    if text.lower() == "none":
        return None
    try:
        if isinstance(default, bool):
            if text.lower() in ("true", "1", "yes", "on"):
                return True
            if text.lower() in ("false", "0", "no", "off"):
                return False
            raise ValueError(text)
        if isinstance(default, int):
            return int(text)
        if isinstance(default, float):
            return float(text)
    except ValueError:
        raise OrchestrationLoadError(
            "--set {}={!r}: must be a {} like its default {!r}".format(
                name, text, type(default).__name__, default))
    return text


def resolve_parameters(raw, leaf_name, sets, mode):
    """{name: value} from the document's parameters and --set values."""
    declared = raw.get("parameters") or {}
    if not isinstance(declared, dict):
        raise OrchestrationLoadError("parameters must be a mapping")
    values = {}
    for name, spec in declared.items():
        if isinstance(spec, dict):
            unknown = set(spec) - {"default", "description"} - \
                set(_ENV_MODES)
            if unknown:
                raise OrchestrationLoadError(
                    "parameter {}: unknown key(s) {}; use default, "
                    "description, or an --env-mode".format(
                        name, ", ".join(sorted(unknown))))
            value = spec.get(mode, spec.get("default"))
        else:
            value = spec
        values[name] = value
    for leaf, name, text in sets or []:
        if leaf is not None and leaf != leaf_name:
            continue
        if name not in values:
            if leaf is not None:
                raise OrchestrationLoadError(
                    "--set {}:{}: leaf {} has no parameter {} (it has: "
                    "{})".format(leaf, name, leaf, name,
                                 ", ".join(sorted(values)) or "none"))
            continue
        values[name] = _convert(text, values[name], name)
    return values


def _substitute(value, params, where):
    if isinstance(value, str):
        m = _PARAM_REF.match(value)
        if not m:
            return value
        name = m.group(1)
        if name not in params:
            raise OrchestrationLoadError(
                "{}: ${} is not a declared parameter (declared: {})".format(
                    where, name, ", ".join(sorted(params)) or "none"))
        return params[name]
    if isinstance(value, list):
        return [_substitute(v, params, where) for v in value]
    if isinstance(value, dict):
        return {k: _substitute(v, params, where) for k, v in value.items()}
    return value


def apply_parameters(raw, marker_path, sets=None, mode=None):
    """Replace each value that is exactly $name with its parameter value.

    Only a whole value is replaced, so text in sources is never touched.
    The leaf name for LEAF:NAME=VALUE is the marker's directory name.
    """
    leaf_name = os.path.basename(os.path.dirname(os.path.abspath(
        marker_path)))
    params = resolve_parameters(raw, leaf_name, sets, mode or "afw")
    body = {k: v for k, v in raw.items() if k != "parameters"}
    out = _substitute(body, params, marker_path)
    out["parameters"] = raw.get("parameters") or {}
    out["_parameter_values"] = params
    return out


def describe_parameters(marker_path):
    """[(name, default, description, {env mode: default})] a leaf declares.

    Reads only the parameters block, for afwdev test --list. Returns []
    for a leaf without parameters or one that does not parse.
    """
    try:
        with nfc.open(marker_path, "r") as fd:
            text = fd.read()
        if marker_path.endswith(".json"):
            raw = nfc.json_loads(text)
        elif yaml is not None:
            raw = yaml.safe_load(text)
        else:
            return []
    except Exception:
        return []
    declared = (raw or {}).get("parameters") if isinstance(raw, dict) \
        else None
    if not isinstance(declared, dict):
        return []
    out = []
    for name, spec in declared.items():
        if isinstance(spec, dict):
            modes = {k: v for k, v in spec.items() if k in _ENV_MODES}
            out.append((name, spec.get("default"), spec.get("description"),
                        modes))
        else:
            out.append((name, spec, None, {}))
    return out


def load_orchestration_document(marker_path, sets=None, mode=None):
    """
    Load orchestration.yaml or .json and validate v1 sequential schema
    (plus optional schedule.firehose / sequential / parallel).

    parameters (name: default, or name: {default, description, <env
    mode>: default}) are substituted where a value is exactly $name;
    sets are parse_sets() entries from --set.

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
    raw = apply_parameters(raw, marker_path, sets, mode)

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
    fuzzes = any(
        isinstance(step, dict) and isinstance(step.get("firehose"), dict)
        and step["firehose"].get("fuzz") is not None
        for step in (raw.get("schedule") or []))
    if not isinstance(tests, list) or (len(tests) == 0 and not fuzzes):
        raise OrchestrationLoadError(
            "orchestration requires non-empty 'tests' list (or a firehose "
            "with fuzz:): " + marker_path)

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
        spec = parse_count_spec(threads, "afwfcgi.threads", marker_path)
        # Percent is of the CPUs on this machine, so one leaf travels.
        afwfcgi["threads"] = resolve_count_spec(spec, os.cpu_count() or 1)

    if raw.get("schedule") is not None:
        if not isinstance(raw["schedule"], list):
            raise OrchestrationLoadError(
                "schedule must be a list of phases: " + marker_path)

    if not raw.get("description"):
        raw["description"] = os.path.basename(os.path.dirname(marker_path))

    return raw


def parse_count_spec(value, what, where):
    """
    An absolute integer >= 1, or a percent string such as '50%' or '300%'.

    Percents are 1 through 400. Over 100 is more than one per base unit.
    The caller supplies the base they apply to.
    """
    if isinstance(value, str):
        text = value.strip()
        if text.endswith("%") and len(text) > 1:
            try:
                pct = float(text[:-1])
            except ValueError as e:
                raise OrchestrationLoadError(
                    "{} percent is not a number ({})".format(what, where)
                ) from e
            if pct <= 0.0 or pct > 400.0:
                raise OrchestrationLoadError(
                    "{} percent must be from 1 to 400 ({})".format(
                        what, where))
            return ("pct", pct)
    try:
        count = int(value)
    except (TypeError, ValueError) as e:
        raise OrchestrationLoadError(
            "{} must be an integer or a percent ({})".format(what, where)
        ) from e
    if count < 1:
        raise OrchestrationLoadError(
            "{} must be >= 1 ({})".format(what, where))
    return ("abs", count)


def resolve_count_spec(spec, base):
    """Turn a parse_count_spec() result into a count. Percent rounds."""
    kind, number = spec
    if kind == "abs":
        return int(number)
    resolved = int(round(float(base) * float(number) / 100.0))
    if resolved < 1:
        return 1
    return resolved


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
