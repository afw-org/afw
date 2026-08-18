#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
LDAP filter splice helpers: RFC 4515 escape, attr token, nesting.

A retrieve through the adapter needs a live directory and schema
metadata (value_to_bv). Script cannot reach the splice. This boots a
C probe against installed libafwldap and calls the helpers directly.
"""

from _afwdev.test.c_probe import run_c_probe


def run():
    return run_c_probe(
        "ldap_filter_probe.c",
        "LDAP filter escape, attr token, and nesting",
        [
            (
                "escape-plain",
                "plain and UTF-8 values are unchanged",
            ),
            (
                "escape-specials",
                "NUL ( ) * \\ become RFC 4515 hex escapes",
            ),
            (
                "attr-ok",
                "descr, numericoid, and option names are accepted",
            ),
            (
                "attr-bad",
                "empty or illegal attribute types are rejected",
            ),
            (
                "depth-ok",
                "256-deep query-criteria chain is accepted",
            ),
            (
                "depth-too-deep",
                "257-deep query-criteria chain is query_too_complex",
            ),
        ],
        libraries=("afwldap", "afw"),
    )
