#!/usr/bin/env python3

import shutil
import os
import tempfile

# The run's scratch directory (afwdev test sets TMPDIR per run).
TMP = tempfile.gettempdir()

# test configuration settings
Environment = "afwdev"

def remove_file(file):
    try:
        os.remove(file)
    except OSError:
        pass

def before_each():
    # remove any residual packages from a prior, broken run
    remove_file(os.path.join(TMP, "test-settings.code-workspace"))    
    remove_file(os.path.join(TMP, "afwdev-settings.json"))

def before_all():

    shutil.rmtree(os.path.join(TMP, "test-package-1"), ignore_errors=True)
    shutil.rmtree(os.path.join(TMP, "test-package-2"), ignore_errors=True)
    shutil.rmtree(os.path.join(TMP, "test-package-3"), ignore_errors=True)
    shutil.rmtree(os.path.join(TMP, "test-package-4"), ignore_errors=True)
    shutil.rmtree(os.path.join(TMP, "test-package-5"), ignore_errors=True)