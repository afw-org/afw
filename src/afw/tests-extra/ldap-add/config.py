#!/usr/bin/env python3
"""Private slapd for one inetOrgPerson add. Same starter as firehose."""

import importlib.util
import os

_HERE = os.path.dirname(os.path.abspath(__file__))
_spec = importlib.util.spec_from_file_location(
    "ldap_add_firehose_config",
    os.path.join(_HERE, "..", "firehose", "config.py"))
_firehose = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(_firehose)


def before_all():
    _firehose.before_all()


def after_all():
    _firehose.after_all()
