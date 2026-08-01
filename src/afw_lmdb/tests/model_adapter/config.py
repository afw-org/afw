#!/usr/bin/env python3

import os
import subprocess

# test configuration settings
Environment = "lmdb-model"

# Seed lmdb data from a *separate* afw process. add_object() begins an
# explicit transaction on its cached adapter session (see
# afw_adapter_add.c) that persists for the life of the process, so
# seeding in-process would leave the nested-retrieve test's own lmdb
# session already holding a transaction and mask the MDB_BAD_RSLOT
# regression this test group exists to catch.
SEED_SCRIPT = (
    "add_object('lmdb','TestObjectType1',{TestString1:\"Row One\"});"
    "add_object('lmdb','TestObjectType1',{TestString1:\"Row Two\"});"
)

def remove_file(path):
    if os.path.exists(path):
        os.remove(path)

def cleanup():
    remove_file("data.mdb")
    remove_file("lock.mdb")

def seed():
    subprocess.run(
        ["afw", "--conf", "afw.conf", "--syntax", "script", "--expression", SEED_SCRIPT],
        check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL
    )

def before_all():
    cleanup()

def before_each():
    cleanup()
    seed()

def after_all():
    cleanup()
