#!/usr/bin/env python3

##
# @file sanitize.py
# @ingroup afwdev_test
# @brief Environment and report helpers for afwdev test --env-mode asan.
# @details The asan mode runs every test against the prefix that
#          `afwdev build --cdev --sanitize address` installs into
#          build/asan/install/. prepare_asan_environment() puts that
#          prefix first on PATH (afw, afwfcgi and afw --local are started
#          by name), points C probes at its lib and include dirs, and sets
#          ASAN_OPTIONS / UBSAN_OPTIONS so any sanitizer report fails the
#          process that hit it. See designs/asan-opt-in.md.
#

import json
import os
import re
import subprocess

from _afwdev.common import msg
from _afwdev.build.cmake import SANITIZE_STAMP_NAME

ASAN_BUILD_COMMAND = './afwdev build --cdev --sanitize address'

# Defaults; a key already in the caller's ASAN_OPTIONS / UBSAN_OPTIONS wins.
# detect_odr_violation=0: every extension exports
# afw_environment_extension_instance and is opened RTLD_GLOBAL.
_ASAN_OPTIONS_DEFAULT = (
    ('detect_odr_violation', '0'),
    ('detect_leaks', '1'),
)
# halt_on_error=1: a UBSan report fails the process, like an ASan one.
_UBSAN_OPTIONS_DEFAULT = (
    ('print_stacktrace', '1'),
    ('halt_on_error', '1'),
)

_REPORT_RE = re.compile(
    r'(==\d+==ERROR: (?:AddressSanitizer|LeakSanitizer): [^\n]*'
    r'|SUMMARY: (?:AddressSanitizer|LeakSanitizer|UndefinedBehaviorSanitizer)'
    r': [^\n]*'
    r'|[^\s:]+:\d+:\d+: runtime error: [^\n]*)')

_FRAME_RE = re.compile(r'^\s+#\d+ .*$', re.MULTILINE)


def asan_prefix(options):
    """build/asan/install of the package being tested."""
    root = (options or {}).get('afw_package_dir_path') or os.getcwd()
    return os.path.join(root, 'build', 'asan', 'install')


def read_stamp(prefix):
    """The --sanitize stamp in prefix, or None."""
    try:
        with open(os.path.join(prefix, SANITIZE_STAMP_NAME),
                encoding='utf-8') as f:
            return json.load(f)
    except (OSError, ValueError):
        return None


def _lib_dir(prefix):
    for sub in ('lib', 'lib64'):
        d = os.path.join(prefix, sub, 'afw')
        if os.path.exists(os.path.join(d, 'libafw.so')):
            return d
    return None


def _merge_options(current, defaults):
    """Append defaults whose key the caller did not set."""
    parts = [p for p in (current or '').split(':') if p]
    have = {p.split('=', 1)[0] for p in parts}
    for key, value in defaults:
        if key not in have:
            parts.append(key + '=' + value)
    return ':'.join(parts)


def _git_head(root):
    try:
        return subprocess.run(['git', 'rev-parse', 'HEAD'], cwd=root,
            capture_output=True, text=True, check=True).stdout.strip()
    except (OSError, subprocess.CalledProcessError):
        return None


def _set_sanitizer_options():
    os.environ['ASAN_OPTIONS'] = _merge_options(
        os.environ.get('ASAN_OPTIONS'), _ASAN_OPTIONS_DEFAULT)
    os.environ['UBSAN_OPTIONS'] = _merge_options(
        os.environ.get('UBSAN_OPTIONS'), _UBSAN_OPTIONS_DEFAULT)


def prepare_asan_environment(options, build_tree=False):
    """Point this process (and every child) at the ASan build, or exit.

    Missing build: error with the build command. Stale stamp (another
    commit, or built from a dirty tree): warning with the same command.
    With build_tree, _afwdev.test.build_tree has already pointed the run
    at build/asan/cmake/; only the sanitizer options are set here.
    """
    if build_tree:
        _set_sanitizer_options()
        msg.highlighted_info('ASan options: ASAN_OPTIONS=' +
            os.environ['ASAN_OPTIONS'])
        return
    prefix = asan_prefix(options)
    stamp = read_stamp(prefix)
    lib_dir = _lib_dir(prefix)
    if stamp is None or lib_dir is None or \
            not os.path.isfile(os.path.join(prefix, 'bin', 'afw')):
        msg.error_exit('--env-mode asan needs the ASan build in ' + prefix +
            '. Build it first: ' + ASAN_BUILD_COMMAND)
    if 'address' not in (stamp.get('sanitizers') or []):
        msg.error_exit(prefix + ' is not an AddressSanitizer build (' +
            SANITIZE_STAMP_NAME + ' sanitizers: ' +
            str(stamp.get('sanitizers')) + '). Rebuild: ' +
            ASAN_BUILD_COMMAND)

    root = (options or {}).get('afw_package_dir_path') or os.getcwd()
    head = _git_head(root)
    if head and stamp.get('commit') and stamp.get('commit') != head:
        msg.warn('ASan build is from commit ' + stamp['commit'][:10] +
            ', HEAD is ' + head[:10] + '. If C changed since, rebuild: ' +
            ASAN_BUILD_COMMAND)
    elif stamp.get('dirty'):
        msg.warn('ASan build was made from a tree with uncommitted '
            'changes (' + str(stamp.get('built')) + '). If C changed '
            'since, rebuild: ' + ASAN_BUILD_COMMAND)

    os.environ['PATH'] = os.path.join(prefix, 'bin') + os.pathsep + \
        os.environ.get('PATH', '')
    os.environ['AFW_LIB_DIR'] = lib_dir
    os.environ['AFW_INCLUDE_DIR'] = os.path.join(prefix, 'include', 'afw')
    _set_sanitizer_options()
    msg.highlighted_info('ASan test environment: ' + prefix +
        ' (ASAN_OPTIONS=' + os.environ['ASAN_OPTIONS'] + ')')


def refuse_sanitized_lib_for_valgrind():
    """Exit if the libafw tests would use is sanitizer-built.

    Valgrind cannot run an ASan process (it fails at start).
    """
    from _afwdev.test.c_probe import libafw_sanitizers
    found = libafw_sanitizers()
    if found:
        msg.error_exit('--env-mode valgrind cannot run a sanitizer build '
            '(libafw built with ' + ','.join(found) + '). Use the normal '
            'install, or --env-mode asan for the ASan build.')


def sanitizer_report_summary(text, max_frames=6):
    """Short summary of sanitizer reports in text, or None if there are none.

    The first report line(s) and the top stack frames, so a failure says
    what kind of error and where, not only that the process died.
    """
    if not text:
        return None
    reports = []
    for m in _REPORT_RE.finditer(text):
        line = m.group(0).strip()
        if line not in reports:
            reports.append(line)
    if not reports:
        return None
    frames = [f.strip() for f in _FRAME_RE.findall(text)[:max_frames]]
    return '\n'.join(reports[:3] + frames)
