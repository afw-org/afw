#!/usr/bin/env python3

import shutil
import os

# test configuration settings
Environment = "afwdev"

def remove_file(file):
    try:
        os.remove(file)
    except OSError:
        pass

def before_each():
    # remove any residual packages from a prior, broken run
    remove_file("/tmp/test-settings.code-workspace")    
    remove_file("/tmp/afwdev-settings.json")

def before_all():

    shutil.rmtree("/tmp/test-package-1", ignore_errors=True)
    shutil.rmtree("/tmp/test-package-2", ignore_errors=True)
    shutil.rmtree("/tmp/test-package-3", ignore_errors=True)
    shutil.rmtree("/tmp/test-package-4", ignore_errors=True)
    shutil.rmtree("/tmp/test-package-5", ignore_errors=True)