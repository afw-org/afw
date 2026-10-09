#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""afwdev test --build-tree and --env-mode asan / tsan helpers (no real build).

Uses a fake cmake tree in a temp dir. Only the refusal paths of
build_tree.prepare() run here: they exit before touching os.environ.
"""

import contextlib
import io
import json
import os
import shutil
import tempfile

from _afwdev.test import build_tree
from _afwdev.test import sanitize
from _afwdev.test.c_probe import libafw_build_cache


def _case(name, description, passed, error=None):
    return {
        "test": name,
        "description": description,
        "passed": bool(passed),
        "skip": False,
        "error": error,
    }


def _touch(path, executable=False):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, 'w') as f:
        f.write('')
    if executable:
        os.chmod(path, 0o755)


def _fake_tree(root, sanitize_value=None):
    """<root>/build/cmake-like tree: afw, afwfcgi, libafw + two extensions."""
    tree = os.path.join(root, 'tree')
    src = os.path.join(tree, 'src')
    _touch(os.path.join(src, 'afw_command', 'afw'), executable=True)
    _touch(os.path.join(src, 'afw_server_fcgi', 'afwfcgi'), executable=True)
    _touch(os.path.join(src, 'afw', 'libafw.so'))
    _touch(os.path.join(src, 'afw_lmdb', 'libafwlmdb.so'))
    _touch(os.path.join(src, 'afw_yaml', 'libafwyaml.so'))
    _touch(os.path.join(src, 'afw_command', 'CMakeFiles', 'x.o'))
    cache = 'CMAKE_INSTALL_PREFIX:PATH=/nowhere\n'
    if sanitize_value is not None:
        cache += 'AFWDEV_SANITIZE:UNINITIALIZED=' + sanitize_value + '\n'
    with open(os.path.join(tree, 'CMakeCache.txt'), 'w') as f:
        f.write(cache)
    with open(os.path.join(tree, 'compile_commands.json'), 'w') as f:
        json.dump([
            {"directory": tree, "file": "a.c",
                "command": "cc -I/abs/one -Irel/two -o a.o -c a.c"},
            {"directory": tree, "file": "b.c",
                "arguments": ["cc", "-I", "/abs/three", "-I/abs/one",
                    "-c", "b.c"]},
        ], f)
    return tree


def _exits(fn, *args):
    try:
        with contextlib.redirect_stdout(io.StringIO()), \
                contextlib.redirect_stderr(io.StringIO()):
            fn(*args)
    except SystemExit:
        return True
    return False


_ASAN_TEXT = """\
[afw abc] Service 'adapter-afw' starting.
=================================================================
==123==ERROR: AddressSanitizer: use-after-poison on address 0xffff at pc 0x1
READ of size 8 at 0xffff thread T0
    #0 0x1 in impl_convert_value_to_json src/afw/json/afw_json_from_value.c:359
    #1 0x2 in impl_convert_list_to_json src/afw/json/afw_json_from_value.c:244
SUMMARY: AddressSanitizer: use-after-poison src/afw/json/afw_json_from_value.c:359 in impl_convert_value_to_json
"""

_TSAN_TEXT = """\
==================
WARNING: ThreadSanitizer: data race (pid=1569437)
  Write of size 1 at 0xffff7b52f428 by main thread:
    #0 impl_handle_shutdown_signal src/afw_server_fcgi/afw_server_fcgi.c:85 (afwfcgi+0xd878)
  Previous read of size 1 at 0xffff7b52f428 by thread T1:
    #0 impl_afw_server_request_thread_start src/afw_server_fcgi/afw_server_fcgi.c:339 (afwfcgi+0x12d90)
SUMMARY: ThreadSanitizer: data race src/afw_server_fcgi/afw_server_fcgi.c:85 in impl_handle_shutdown_signal
==================
"""

_UBSAN_TEXT = (
    "/w/src/afw/function/afw_function_integer.c:263:14: runtime error: "
    "signed integer overflow: 24 * 9223372036854775807 cannot be "
    "represented in type 'long int'\n")


def run():
    tests = []
    work = tempfile.mkdtemp(prefix='afwdev_build_tree_test_')
    try:
        tree = _fake_tree(work)
        bin_dirs, lib_dirs = build_tree._scan(tree)
        names = lambda ds: sorted(os.path.basename(d) for d in ds)
        tests.append(_case(
            "scan",
            "finds the afw / afwfcgi dirs and every lib*.so dir",
            passed=(
                names(bin_dirs) == ['afw_command', 'afw_server_fcgi']
                and names(lib_dirs) == ['afw', 'afw_lmdb', 'afw_yaml']
            ),
            error=str((names(bin_dirs), names(lib_dirs))),
        ))

        includes = build_tree._include_dirs(tree)
        tests.append(_case(
            "include-dirs",
            "-I dirs from compile_commands.json: command and arguments "
            "forms, relative to directory, deduplicated in order",
            passed=includes == ['/abs/one', os.path.join(tree, 'rel', 'two'),
                '/abs/three'],
            error=str(includes),
        ))

        tests.append(_case(
            "tree-per-mode",
            "asan / tsan use build/asan/cmake / build/tsan/cmake; other "
            "modes use build/cmake",
            passed=(
                build_tree.tree_dir({'afw_package_dir_path': '/p/',
                    'mode': 'asan'}) == '/p/build/asan/cmake'
                and build_tree.tree_dir({'afw_package_dir_path': '/p/',
                    'mode': 'tsan'}) == '/p/build/tsan/cmake'
                and build_tree.tree_dir({'afw_package_dir_path': '/p/',
                    'mode': 'valgrind'}) == '/p/build/cmake'
                and build_tree.tree_dir({'afw_package_dir_path': '/p/'})
                    == '/p/build/cmake'
            ),
        ))

        tests.append(_case(
            "build-command",
            "the rebuild hint names the mode's --sanitize variant",
            passed=(
                build_tree.build_command({'mode': 'asan'}) ==
                    './afwdev build --cdev --sanitize address'
                and build_tree.build_command({'mode': 'tsan'}) ==
                    './afwdev build --cdev --sanitize thread'
                and build_tree.build_command({'mode': 'valgrind'}) ==
                    './afwdev build --cdev'
            ),
        ))

        tests.append(_case(
            "cache-value",
            "reads AFWDEV_SANITIZE from CMakeCache.txt",
            passed=(
                build_tree.cache_value(
                    _fake_tree(os.path.join(work, 's'), 'address;undefined'),
                    'AFWDEV_SANITIZE') == 'address;undefined'
                and build_tree.cache_value(tree, 'AFWDEV_SANITIZE') is None
            ),
        ))

        # prepare() refusals: a package whose build/cmake is the fake tree.
        pkg = os.path.join(work, 'pkg') + os.sep
        os.makedirs(os.path.join(pkg, 'build'))
        missing = _exits(build_tree.prepare,
            {'afw_package_dir_path': pkg, 'mode': 'afw'})
        shutil.copytree(_fake_tree(os.path.join(work, 'san'), 'address'),
            os.path.join(pkg, 'build', 'cmake'))
        sanitized_for_afw = _exits(build_tree.prepare,
            {'afw_package_dir_path': pkg, 'mode': 'afw'})
        os.makedirs(os.path.join(pkg, 'build', 'asan'))
        shutil.copytree(tree, os.path.join(pkg, 'build', 'asan', 'cmake'))
        plain_for_asan = _exits(build_tree.prepare,
            {'afw_package_dir_path': pkg, 'mode': 'asan'})
        os.makedirs(os.path.join(pkg, 'build', 'tsan'))
        shutil.copytree(
            _fake_tree(os.path.join(work, 'asan'), 'address;undefined'),
            os.path.join(pkg, 'build', 'tsan', 'cmake'))
        asan_for_tsan = _exits(build_tree.prepare,
            {'afw_package_dir_path': pkg, 'mode': 'tsan'})
        tests.append(_case(
            "prepare-refusals",
            "missing tree, a sanitizer tree for a normal mode, a normal "
            "tree for asan, and an ASan tree for tsan all exit with an "
            "error",
            passed=(missing and sanitized_for_afw and plain_for_asan
                and asan_for_tsan),
            error=str((missing, sanitized_for_afw, plain_for_asan,
                asan_for_tsan)),
        ))

        tests.append(_case(
            "probe-build-cache",
            "libafw_build_cache finds CMakeCache.txt for a build-tree libafw",
            passed=libafw_build_cache(os.path.join(tree, 'src', 'afw'))
                == os.path.join(tree, 'CMakeCache.txt'),
        ))
    finally:
        shutil.rmtree(work, ignore_errors=True)

    asan = sanitize.sanitizer_report_summary(_ASAN_TEXT)
    tsan = sanitize.sanitizer_report_summary(_TSAN_TEXT)
    ubsan = sanitize.sanitizer_report_summary(_UBSAN_TEXT)
    tests.append(_case(
        "report-summary",
        "ASan, TSan and UBSan reports become a short summary; clean output "
        "is None",
        passed=(
            asan is not None
            and 'ERROR: AddressSanitizer: use-after-poison' in asan
            and 'impl_convert_value_to_json' in asan
            and tsan is not None
            and 'WARNING: ThreadSanitizer: data race' in tsan
            and 'SUMMARY: ThreadSanitizer: data race' in tsan
            and 'impl_handle_shutdown_signal' in tsan
            and ubsan is not None
            and 'runtime error: signed integer overflow' in ubsan
            and sanitize.sanitizer_report_summary(
                "[afw] Service 'adapter-afw' started.\n") is None
            and sanitize.sanitizer_report_summary(None) is None
        ),
        error=str((asan, tsan, ubsan)),
    ))

    merged = sanitize._merge_options('detect_leaks=0:verbosity=1',
        (('detect_odr_violation', '0'), ('detect_leaks', '1')))
    tests.append(_case(
        "options-merge",
        "default ASAN_OPTIONS keys are added; a key the caller set wins",
        passed=merged == 'detect_leaks=0:verbosity=1:detect_odr_violation=0',
        error=merged,
    ))

    tests.append(_case(
        "sanitizer-modes",
        "asan and tsan map to the address and thread --sanitize variants",
        passed=(
            sanitize.sanitizer_mode({'mode': 'asan'})['variant'] == 'address'
            and sanitize.sanitizer_mode({'mode': 'tsan'})['variant']
                == 'thread'
            and sanitize.sanitizer_mode({'mode': 'valgrind'}) is None
            and sanitize.sanitizer_mode({}) is None
        ),
    ))

    return {
        "description":
            "afwdev test --build-tree and --env-mode asan / tsan helpers",
        "tests": tests,
    }
