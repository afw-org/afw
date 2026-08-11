# -*- coding: utf-8 -*-
"""Discovery helpers for orchestrated-test marker leaves."""

import os

MARKER_STEM = "orchestration"
MARKER_YAML = MARKER_STEM + ".yaml"
MARKER_JSON = MARKER_STEM + ".json"


def find_orchestration_marker(directory):
    """
    If directory is an orchestrated-test leaf, return absolute path to the marker.

    Both .yaml and .json present → ValueError (ambiguous).
    """
    yaml_path = os.path.join(directory, MARKER_YAML)
    json_path = os.path.join(directory, MARKER_JSON)
    has_yaml = os.path.isfile(yaml_path)
    has_json = os.path.isfile(json_path)
    if has_yaml and has_json:
        raise ValueError(
            "Ambiguous orchestrated-test leaf (both .yaml and .json): "
            + directory)
    if has_yaml:
        return yaml_path
    if has_json:
        return json_path
    return None


def is_orchestration_marker_path(path):
    """True if path is orchestration.yaml or orchestration.json."""
    if not path:
        return False
    base = os.path.basename(path)
    return base == MARKER_YAML or base == MARKER_JSON
