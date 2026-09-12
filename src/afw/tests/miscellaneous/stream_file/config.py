#!/usr/bin/env python3
"""
Seed symlink fixtures for rootFilePaths containment tests.

ok_link stays under the data/ host root (sibling follow).
escape_link points at a directory outside the work dir (must fail).
"""

import os


def before_all():
    files = os.path.join(os.getcwd(), "files")
    os.makedirs(files, exist_ok=True)

    outside = os.path.abspath(
        os.path.join(os.getcwd(), os.pardir, "filepath_outside"))
    os.makedirs(outside, exist_ok=True)
    secret = os.path.join(outside, "secret.txt")
    with open(secret, "w", encoding="utf-8") as f:
        f.write("should-not-read\n")

    escape = os.path.join(files, "escape_link")
    if os.path.lexists(escape):
        os.remove(escape)
    os.symlink(outside, escape)

    ok = os.path.join(files, "ok_link")
    if os.path.lexists(ok):
        os.remove(ok)
    os.symlink(os.path.join("include_root", "x.txt"), ok)
