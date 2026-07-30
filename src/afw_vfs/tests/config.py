#!/usr/bin/env python

import os

Environment = "vfs"


def before_all():
    # Issue #15: vfsMap host paths may use environment:: templates
    os.environ["AFW_VFS_TEST_ROOT"] = os.getcwd()


def before_each():
    os.environ["AFW_VFS_TEST_ROOT"] = os.getcwd()
