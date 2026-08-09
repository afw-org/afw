# -*- coding: utf-8 -*-
"""Load and validate advanced-test marker documents."""

import os

from _afwdev.common import nfc
from _afwdev.common.errors import AfwdevRunnerError

try:
    import yaml
except ImportError:  # pragma: no cover
    yaml = None


class AdvancedTestLoadError(AfwdevRunnerError):
    """Invalid or unreadable advanced-test marker."""


def load_advanced_test_document(marker_path):
    """
    Load advanced-test.yaml or .json into a dict and validate v1 schema.

    Returns the document dict (mutated only for normalization of defaults).
    """
    if not os.path.isfile(marker_path):
        raise AdvancedTestLoadError("Marker not found: " + marker_path)

    raw = None
    with nfc.open(marker_path, "r") as fd:
        text = fd.read()

    if marker_path.endswith(".yaml") or marker_path.endswith(".yml"):
        if yaml is None:
            raise AdvancedTestLoadError(
                "PyYAML is required to load advanced-test.yaml "
                "(install PyYAML / project python-requirements)")
        try:
            raw = yaml.safe_load(text)
        except Exception as e:
            raise AdvancedTestLoadError(
                "Failed to parse YAML {}: {}".format(marker_path, e)) from e
    elif marker_path.endswith(".json"):
        try:
            raw = nfc.json_loads(text)
        except Exception as e:
            raise AdvancedTestLoadError(
                "Failed to parse JSON {}: {}".format(marker_path, e)) from e
    else:
        raise AdvancedTestLoadError(
            "Marker must be advanced-test.yaml or advanced-test.json: "
            + marker_path)

    if not isinstance(raw, dict):
        raise AdvancedTestLoadError(
            "advanced-test document must be a mapping/object: " + marker_path)

    host = raw.get("host")
    if not host:
        raise AdvancedTestLoadError(
            "advanced-test requires 'host' field: " + marker_path)
    if host != "afwfcgi":
        raise AdvancedTestLoadError(
            "advanced-test host {!r} not supported in v1 (only 'afwfcgi'): "
            "{}".format(host, marker_path))

    steps = raw.get("steps")
    if not isinstance(steps, list) or len(steps) == 0:
        raise AdvancedTestLoadError(
            "advanced-test requires non-empty 'steps' list: " + marker_path)

    for i, step in enumerate(steps):
        if not isinstance(step, dict):
            raise AdvancedTestLoadError(
                "steps[{}] must be a mapping: {}".format(i, marker_path))
        name = step.get("name")
        if not name or not isinstance(name, str):
            raise AdvancedTestLoadError(
                "steps[{}] requires string 'name': {}".format(i, marker_path))
        has_eval = "eval" in step and step.get("eval") is not None
        has_script = "script" in step and step.get("script") is not None
        if has_eval and has_script:
            raise AdvancedTestLoadError(
                "steps[{}] ({!r}) must not set both 'eval' and 'script': "
                "{}".format(i, name, marker_path))
        if not has_eval and not has_script:
            raise AdvancedTestLoadError(
                "steps[{}] ({!r}) requires 'eval' or 'script': {}".format(
                    i, name, marker_path))
        if has_script and not isinstance(step.get("script"), str):
            raise AdvancedTestLoadError(
                "steps[{}] ({!r}) 'script' must be a string path: {}".format(
                    i, name, marker_path))
        if has_eval and not isinstance(step.get("eval"), str):
            raise AdvancedTestLoadError(
                "steps[{}] ({!r}) 'eval' must be a string: {}".format(
                    i, name, marker_path))

    if "timeout_s" in raw and raw["timeout_s"] is not None:
        try:
            raw["timeout_s"] = float(raw["timeout_s"])
        except (TypeError, ValueError) as e:
            raise AdvancedTestLoadError(
                "timeout_s must be a number: " + marker_path) from e
    else:
        raw["timeout_s"] = 120.0

    afwfcgi = raw.get("afwfcgi")
    if afwfcgi is None:
        raw["afwfcgi"] = {"threads": 1}
    elif not isinstance(afwfcgi, dict):
        raise AdvancedTestLoadError(
            "afwfcgi block must be a mapping: " + marker_path)
    else:
        threads = afwfcgi.get("threads", 1)
        try:
            threads = int(threads)
        except (TypeError, ValueError) as e:
            raise AdvancedTestLoadError(
                "afwfcgi.threads must be an integer: " + marker_path) from e
        if threads < 1:
            raise AdvancedTestLoadError(
                "afwfcgi.threads must be >= 1: " + marker_path)
        afwfcgi["threads"] = threads

    if not raw.get("description"):
        raw["description"] = os.path.basename(os.path.dirname(marker_path))

    return raw
