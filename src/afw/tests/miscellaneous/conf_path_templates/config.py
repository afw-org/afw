#!/usr/bin/env python3
"""
Issue #15 conf path templates: set AFW_ISSUE15_ROOT to the test work dir
so afw.conf can use environment:: substitutions for host paths.
"""

import os


def before_all():
    # work_dir is already cwd when hooks run (see afwdev test runner).
    os.environ["AFW_ISSUE15_ROOT"] = os.getcwd()


def before_each():
    # Re-assert in case another group cleared env (runner restores env after group).
    os.environ["AFW_ISSUE15_ROOT"] = os.getcwd()
