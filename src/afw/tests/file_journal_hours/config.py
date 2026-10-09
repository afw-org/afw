#!/usr/bin/env python3

import json
import os
import struct

# test configuration settings
Environment = "file-journal-hours"

# A file journal last written in hour 2020-01-01 00 UTC, holding two
# entries. The next entry the test writes is in the current hour, so the
# journal moves to a new hourly file and links the old one to it.
#
# Layout under the adapter root (see afw_file_journal.c):
#   _AdaptiveJournalEntry_/journal_lock               time of the last write
#   _AdaptiveJournalEntry_/path_to_first_journal_file relative path of first file
#   _AdaptiveJournalEntry_/y2020/m01/d01/h00          entries
# An entry is an 8 byte big-endian length followed by the entry as JSON.
# The lock is usec (4 bytes, big-endian), then one byte each for sec,
# min, hour, day, month, year in century, century, and a filler.
DIR = "journal/_AdaptiveJournalEntry_"
FIRST = "y2020/m01/d01/h00"
ENTRIES = [{"eventType": "hour-old", "n": 1},
           {"eventType": "hour-old", "n": 2}]


def cleanup():
    for root, dirs, files in os.walk("journal"):
        for f in files:
            if not (root == "journal" and f == ".gitignore"):
                os.remove(os.path.join(root, f))


def make_old_hour_journal():
    os.makedirs(os.path.join(DIR, FIRST.rsplit("/", 1)[0]), exist_ok=True)
    with open(os.path.join(DIR, FIRST), "wb") as f:
        for entry in ENTRIES:
            data = json.dumps(entry).encode("utf-8")
            f.write(struct.pack(">Q", len(data)) + data)
    with open(os.path.join(DIR, "path_to_first_journal_file"), "wb") as f:
        f.write(FIRST.encode("utf-8"))
    with open(os.path.join(DIR, "journal_lock"), "wb") as f:
        f.write(struct.pack(">IBBBBBBBB", 0, 0, 0, 0, 1, 1, 20, 20, 0))


def before_each():
    cleanup()
    make_old_hour_journal()

def before_all():
    cleanup()

def after_all():
    cleanup()
