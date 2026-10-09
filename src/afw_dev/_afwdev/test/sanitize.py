#!/usr/bin/env python3

##
# @file sanitize.py
# @ingroup afwdev_test
# @brief Environment and report helpers for the sanitizer test modes.
# @details --env-mode asan and tsan run every test against the build tree
#          that `afwdev build --cdev --sanitize address` / `thread` makes
#          in build/asan/cmake/ / build/tsan/cmake/
#          (_afwdev.test.build_tree sets PATH, LD_LIBRARY_PATH and the C
#          probe dirs). prepare_sanitizer_environment() sets the
#          sanitizer options so any report fails the process that hit
#          it. See designs/asan-opt-in.md.
#

import os
import re
import resource

from _afwdev.common import msg

# --env-mode (also its tree's directory: build/<mode>/cmake/) -> the
# --sanitize variant that builds that tree, the sanitizer it adds, and
# names for messages. UBSan rides along with each. Keep in step with
# _SANITIZE_VARIANTS in _afwdev/build/build.py.
SANITIZER_MODES = {
    'asan': {
        'variant': 'address',
        'sanitizer': 'address',
        'short': 'ASan',
        'name': 'AddressSanitizer',
    },
    'tsan': {
        'variant': 'thread',
        'sanitizer': 'thread',
        'short': 'TSan',
        'name': 'ThreadSanitizer',
    },
}


def sanitizer_mode(options):
    """SANITIZER_MODES entry for options' --env-mode, or None."""
    return SANITIZER_MODES.get((options or {}).get('mode') or 'afw')


# Defaults; a key already in the caller's *SAN_OPTIONS wins.
# detect_odr_violation=0: every extension exports
# afw_environment_extension_instance and is opened RTLD_GLOBAL.
_ASAN_OPTIONS_DEFAULT = (
    ('detect_odr_violation', '0'),
    ('detect_leaks', '1'),
)
# halt_on_error=1: the first report fails the process, like an ASan one
# (TSan would otherwise go on and only exit 66 at the end).
# second_deadlock_stack=1: lock-order reports show both stacks.
_TSAN_OPTIONS_DEFAULT = (
    ('halt_on_error', '1'),
    ('second_deadlock_stack', '1'),
)
# halt_on_error=1: a UBSan report fails the process, like an ASan one.
_UBSAN_OPTIONS_DEFAULT = (
    ('print_stacktrace', '1'),
    ('halt_on_error', '1'),
)

# RLIMIT_STACK cap under tsan. Request threads get a stack as large as
# RLIMIT_STACK; TSan's shadow call stack holds about 64K frames, so deep
# recursion on an 8 MiB stack overruns it (TSan itself SEGVs) before the
# C stack headroom check trips. 4 MiB keeps the check first.
TSAN_STACK_LIMIT_BYTES = 4 * 1024 * 1024

_REPORT_RE = re.compile(
    r'(==\d+==ERROR: (?:AddressSanitizer|LeakSanitizer|ThreadSanitizer)'
    r': [^\n]*'
    r'|WARNING: ThreadSanitizer: [^\n]*'
    r'|SUMMARY: (?:AddressSanitizer|LeakSanitizer|ThreadSanitizer'
    r'|UndefinedBehaviorSanitizer): [^\n]*'
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


def _set_options(name, defaults):
    os.environ[name] = _merge_options(os.environ.get(name), defaults)


def _cap_stack_limit(limit):
    """Lower the RLIMIT_STACK soft limit to limit; children inherit it.

    Returns the soft limit now in effect.
    """
    soft, hard = resource.getrlimit(resource.RLIMIT_STACK)
    if hard != resource.RLIM_INFINITY:
        limit = min(limit, hard)
    if soft == resource.RLIM_INFINITY or soft > limit:
        resource.setrlimit(resource.RLIMIT_STACK, (limit, hard))
        soft = limit
    return soft


def prepare_sanitizer_environment(options):
    """Set the sanitizer options (and tsan's stack cap) for the whole run.

    _afwdev.test.build_tree.prepare() has already pointed the run at the
    mode's build tree and checked it is that sanitizer's build.
    """
    mode = (options or {}).get('mode')
    _set_options('UBSAN_OPTIONS', _UBSAN_OPTIONS_DEFAULT)
    if mode == 'asan':
        _set_options('ASAN_OPTIONS', _ASAN_OPTIONS_DEFAULT)
        msg.highlighted_info('ASan options: ASAN_OPTIONS=' +
            os.environ['ASAN_OPTIONS'])
    elif mode == 'tsan':
        _set_options('TSAN_OPTIONS', _TSAN_OPTIONS_DEFAULT)
        stack = _cap_stack_limit(TSAN_STACK_LIMIT_BYTES)
        msg.highlighted_info('TSan options: TSAN_OPTIONS=' +
            os.environ['TSAN_OPTIONS'] + ' (stack limit ' +
            str(stack // 1024) + ' KiB)')


def refuse_sanitized_lib_for_valgrind():
    """Exit if the libafw tests would use is sanitizer-built.

    Valgrind cannot run an ASan or TSan process (it fails at start).
    """
    from _afwdev.test.c_probe import libafw_sanitizers
    found = libafw_sanitizers()
    if found:
        msg.error_exit('--env-mode valgrind cannot run a sanitizer build '
            '(libafw built with ' + ','.join(found) + '). Use the normal '
            'install, or --env-mode asan / tsan for a sanitizer build.')


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
