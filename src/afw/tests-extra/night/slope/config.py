#!/usr/bin/env python3
"""Private slapd from the firehose leaf, plus a fresh metrics directory."""

import importlib.util
import os

METRICS = "/tmp/afw-night-slope-metrics"
_HERE = os.path.dirname(os.path.abspath(__file__))
_spec = importlib.util.spec_from_file_location(
    "night_firehose_config",
    os.path.join(_HERE, "..", "..", "firehose", "config.py"))
_firehose = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(_firehose)


def before_all():
    os.makedirs(METRICS, exist_ok=True)
    for name in ("rss-first.txt", "rss-last.txt", "metrics.tsv"):
        path = os.path.join(METRICS, name)
        try:
            os.remove(path)
        except FileNotFoundError:
            pass
    _firehose.before_all()


def after_all():
    _firehose.after_all()
