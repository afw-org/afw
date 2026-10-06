#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
AFW_MEMORY_REGION_KEEP_FREED_BYTES (#485): at environment create it sets
the memory region free-list cap and keeps every freed region, so freed
pool chunks stay poisoned under ASan and valgrind. afwdev test sets it
for those env modes.
"""

import json
import os
import subprocess

VAR = "AFW_MEMORY_REGION_KEEP_FREED_BYTES"


def _process(value):
    env = {k: v for k, v in os.environ.items() if k != VAR}
    if value is not None:
        env[VAR] = value
    r = subprocess.run(
        ["afw", "-x",
         'stringify(get_object("afw", "_AdaptiveProcess_", "current"))'],
        capture_output=True, text=True, env=env, timeout=120)
    out = r.stdout.strip()
    try:
        value = json.loads(out)
        if isinstance(value, str):
            value = json.loads(value)
        return value
    except ValueError:
        return {"_error": out[-500:] + r.stderr[-500:]}


def _knobs(p):
    return (p.get("memoryRegionFreeListMaxBytes"),
            p.get("memoryRegionKeepSmallCount"),
            p.get("memoryRegionKeepLargeCount"))


def run():
    tests = []
    default = _knobs(_process(None))
    keep = _knobs(_process(str(64 * 1024 * 1024)))
    bad = [_knobs(_process(v)) for v in ("abc", "0", "12x", "")]
    tests.append({
        "test": "keep-freed-default",
        "description": "without the variable the region knobs are the defaults",
        "passed": default == (262144, 8, 1),
        "skip": False,
    })
    tests.append({
        "test": "keep-freed-set",
        "description": "the variable sets the cap and keeps every freed region",
        "passed": keep == (64 * 1024 * 1024, 1000000, 1000000),
        "skip": False,
    })
    tests.append({
        "test": "keep-freed-ignored",
        "description": "a value that is not a positive byte count is ignored",
        "passed": all(b == default for b in bad),
        "skip": False,
    })
    return {
        "description": "AFW_MEMORY_REGION_KEEP_FREED_BYTES",
        "tests": tests,
    }
