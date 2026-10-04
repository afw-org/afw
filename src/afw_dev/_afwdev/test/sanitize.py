#!/usr/bin/env python3

##
# @file sanitize.py
# @ingroup afwdev_test
# @brief Environment and report helpers for afwdev test --env-mode asan.
# @details The asan mode runs every test against the build tree that
#          `afwdev build --cdev --sanitize address` makes in
#          build/asan/cmake/ (_afwdev.test.build_tree sets PATH,
#          LD_LIBRARY_PATH and the C probe dirs).
#          prepare_asan_environment() sets ASAN_OPTIONS / UBSAN_OPTIONS so
#          any sanitizer report fails the process that hit it. See
#          designs/asan-opt-in.md.
#

import os
import re

from _afwdev.common import msg

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


def _merge_options(current, defaults):
    """Append defaults whose key the caller did not set."""
    parts = [p for p in (current or '').split(':') if p]
    have = {p.split('=', 1)[0] for p in parts}
    for key, value in defaults:
        if key not in have:
            parts.append(key + '=' + value)
    return ':'.join(parts)


def _set_sanitizer_options():
    os.environ['ASAN_OPTIONS'] = _merge_options(
        os.environ.get('ASAN_OPTIONS'), _ASAN_OPTIONS_DEFAULT)
    os.environ['UBSAN_OPTIONS'] = _merge_options(
        os.environ.get('UBSAN_OPTIONS'), _UBSAN_OPTIONS_DEFAULT)


def prepare_asan_environment(options):
    """Set ASAN_OPTIONS / UBSAN_OPTIONS for the whole run.

    _afwdev.test.build_tree.prepare() has already pointed the run at
    build/asan/cmake/ and checked it is an ASan build.
    """
    _set_sanitizer_options()
    msg.highlighted_info('ASan options: ASAN_OPTIONS=' +
        os.environ['ASAN_OPTIONS'])


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
