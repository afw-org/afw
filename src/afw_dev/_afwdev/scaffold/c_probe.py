#! /usr/bin/env python3
##
# @file c_probe.py
# @ingroup afwdev_scaffold
# @brief Prime a C probe test leaf from the closet pair.
#

import os
import re

from _afwdev.common import msg, nfc, resources
from _afwdev.scaffold.common import msg_added_files

_NAME_RE = re.compile(r'^[A-Za-z][A-Za-z0-9_]*$')
_TESTS_DIR_NAMES = ('tests', 'tests-extra')


def _resolve_path(raw):
    raw = (raw or "").strip()
    if not raw:
        msg.error_exit("path is required")
    if os.path.isabs(raw):
        return os.path.abspath(raw)
    return os.path.abspath(os.path.join(os.getcwd(), raw))


def _split_tests_leaf(dest):
    """Return (rel_parts, tests_kind) or error_exit.

    dest must be strictly under a tests/ or tests-extra/ directory.
    """
    parts = dest.split(os.sep)
    for i, p in enumerate(parts):
        if p not in _TESTS_DIR_NAMES:
            continue
        if i == len(parts) - 1:
            msg.error_exit(
                "path must be a test leaf under " + p + "/, not " + p +
                "/ itself: " + dest)
        rel = parts[i + 1:]
        for seg in rel:
            if seg in ('.', '..') or seg.startswith('_'):
                msg.error_exit(
                    "path cannot contain '.', '..', or '_' directories")
            if not _NAME_RE.match(seg):
                msg.error_exit(
                    "each path segment must be a simple identifier "
                    "(letter, then letters, digits, underscore): " + seg)
        return rel, p
    msg.error_exit(
        "path must be under a tests/ or tests-extra/ directory: " + dest)


def _assert_new_leaf(dest):
    if os.path.isfile(dest):
        msg.error_exit("Refusing to overwrite file: " + dest)
    if os.path.isdir(dest):
        existing = [n for n in os.listdir(dest) if n not in ('.', '..')]
        if existing:
            msg.error_exit(
                "C probe leaf must be empty (not a parent of other tests): "
                + dest)
    parent = os.path.dirname(dest)
    if os.path.isfile(os.path.join(parent, 'orchestration.yaml')) or \
            os.path.isfile(os.path.join(parent, 'orchestration.json')):
        msg.error_exit(
            "Refusing to create a probe inside an orchestrated test leaf: "
            + parent)


def _display_rel(options, dest):
    pkg = options.get('afw_package_dir_path') or ''
    if pkg:
        pkg = os.path.abspath(pkg)
        if dest == pkg or dest.startswith(pkg + os.sep):
            return os.path.relpath(dest, pkg)
    cwd = os.getcwd()
    if dest == cwd or dest.startswith(cwd + os.sep):
        return os.path.relpath(dest, cwd)
    return dest


def prime_test_c_probe(options):
    dest = _resolve_path(options.get('probe_path'))
    rel_parts, tests_kind = _split_tests_leaf(dest)
    name = rel_parts[-1]
    pattern = '/'.join(rel_parts)
    rel_dest = _display_rel(options, dest)

    msg.highlighted_info(
        "Priming C probe " + pattern + " at " + rel_dest, empty_before=True)
    options['added_files'] = []

    _assert_new_leaf(dest)
    os.makedirs(dest, exist_ok=True)

    c_name = name + '_probe.c'
    py_name = name + '.py'
    c_text = resources.load_resource(
        options, 'closet/c_probe/skeleton_probe.c')
    py_text = resources.load_resource(
        options, 'closet/c_probe/skeleton_probe.py')
    c_text = c_text.replace('skeleton_probe', name + '_probe')
    py_text = py_text.replace('skeleton_probe', name + '_probe')
    py_text = py_text.replace('Skeleton C probe', name.replace('_', ' '))

    for filename, text in ((c_name, c_text), (py_name, py_text)):
        path = os.path.join(dest, filename)
        rel = os.path.join(rel_dest, filename)
        if os.path.exists(path):
            msg.error_exit("Already exists: " + rel)
        msg.info('Writing ' + rel)
        options['added_files'].append(rel)
        with nfc.open(path, mode='w') as fd:
            fd.write(text)

    if tests_kind == 'tests-extra':
        how = "afwdev test -T '" + rel_dest + "'"
    else:
        how = "afwdev test --test-pattern '" + pattern + "'"
    msg.success('C probe leaf primed at ' + rel_dest + '.')
    msg.info('Edit the marked blocks, then:  ' + how)
    msg_added_files(options)
