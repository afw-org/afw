#!/usr/bin/env python3

import os

# test configuration settings
Environment = "lmdb-journal"

def remove_file(path):
    if os.path.exists(path):
        os.remove(path)

def cleanup():
    remove_file("adapter_db/data.mdb")
    remove_file("adapter_db/lock.mdb")
    remove_file("journal_db/data.mdb")
    remove_file("journal_db/lock.mdb")

def after_all():
    cleanup()

def before_all():
    # Issue #15: LMDB env.path is a template using this root
    os.environ["AFW_LMDB_TEST_PATH"] = os.getcwd()
    cleanup()

def before_each():
    os.environ["AFW_LMDB_TEST_PATH"] = os.getcwd()
    cleanup()
