#!/usr/bin/env python3

import os
import subprocess

# test configuration settings
Environment = "lmdb-model-multi-env"

# Seed from a *separate* afw process, so the rows are committed before a
# test file starts. Inside one afw process every adapter's transaction
# lasts until the process exits (see ../model_adapter/config.py), and
# several cases here depend on what is committed versus only written by
# another session.
SEED_SCRIPT = (
    "add_object('lmdbA', 'OrderRow', {item: 's1'}, 's1');"
    "add_object('lmdbA', 'OrderRow', {item: 's2'}, 's2');"
    "add_object('lmdbB', 'Audit', {op: 'add', orderId: 's1', item: 's1'},"
    " 's1-add');"
)

def remove_file(path):
    if os.path.exists(path):
        os.remove(path)

def cleanup():
    for env_dir in ("lmdbA", "lmdbB"):
        remove_file(os.path.join(env_dir, "data.mdb"))
        remove_file(os.path.join(env_dir, "lock.mdb"))

def seed():
    subprocess.run(
        ["afw", "--conf", "afw.conf", "--syntax", "script",
            "--expression", SEED_SCRIPT],
        check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL
    )

def before_all():
    cleanup()

def before_each():
    cleanup()
    seed()

def after_all():
    cleanup()
