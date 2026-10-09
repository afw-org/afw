#!/usr/bin/env python3

import os

# test configuration settings
Environment = "file-journal-self"

def cleanup():
    # remove everything under objects/ except its .gitignore
    for root, dirs, files in os.walk("objects"):
        for f in files:
            if not (root == "objects" and f == ".gitignore"):
                os.remove(os.path.join(root, f))

def before_each():
    cleanup()

def before_all():
    cleanup()

def after_all():
    cleanup()
