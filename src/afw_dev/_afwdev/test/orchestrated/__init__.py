# -*- coding: utf-8 -*-
"""
Orchestrated tests (orchestration.yaml|json).

Host + feed + tests + schedule. Replaces experimental advanced-test markers
and is the path toward retiring afwdev blast (schedule.firehose).
"""

from _afwdev.test.orchestrated.runner import run_orchestrated_test

__all__ = ["run_orchestrated_test"]
