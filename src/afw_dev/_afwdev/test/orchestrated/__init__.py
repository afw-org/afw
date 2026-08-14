# -*- coding: utf-8 -*-
"""
Orchestrated tests (orchestration.yaml|json).

Host + feed + tests + schedule. Replaces experimental advanced-test markers.
`afwdev blast` retired → schedule.firehose.
"""

from _afwdev.test.orchestrated.runner import run_orchestrated_test

__all__ = ["run_orchestrated_test"]
