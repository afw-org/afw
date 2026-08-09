# -*- coding: utf-8 -*-
"""
Advanced test leaves (advanced-test.yaml|json).

*** Experimental *** — usable for comment and real regression work; schema and
behavior may change over the next months. See designs/afwdev-advanced-test.md
and GitHub issue #157.
"""

from _afwdev.test.advanced.discovery import (
    MARKER_STEM,
    find_advanced_marker,
    is_advanced_marker_path,
)
from _afwdev.test.advanced.runner import run_advanced_test

__all__ = [
    "MARKER_STEM",
    "find_advanced_marker",
    "is_advanced_marker_path",
    "run_advanced_test",
]
