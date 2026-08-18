#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
@todo One line: why script cannot reach this hole.
"""

from _afwdev.test.c_probe import run_c_probe


def run():
    return run_c_probe(
        "skeleton_probe.c",  # @todo rename to match this group
        "Skeleton C probe",  # @todo suite description
        [
            # Names must match argv[1] in the .c
            ("ok", "environment boots"),
            # ("your_case", "what this case proves"),
        ],
        # Extra DSOs, e.g. libraries=("afwldap", "afw"),
    )
