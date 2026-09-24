#!/usr/bin/env python3

import os

# test configuration settings
Environment = "lmdb-model-location"

def remove_file(path):
    if os.path.exists(path):
        os.remove(path)

def cleanup():
    remove_file("data.mdb")
    remove_file("lock.mdb")

def after_all():
    cleanup()

def before_all():
    os.environ["AFW_LMDB_TEST_PATH"] = os.getcwd()
    cleanup()

def before_each():
    os.environ["AFW_LMDB_TEST_PATH"] = os.getcwd()
    cleanup()
