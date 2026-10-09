#!/usr/bin/env python3

##
# @file build_tree.py
# @ingroup afwdev_test
# @brief afwdev test --build-tree: run tests against a cmake build tree.
# @details Tests normally run the installed afw, afwfcgi and libraries.
#          --build-tree points the whole run (children inherit it) at the
#          cmake tree of the build the --env-mode needs: build/cmake/, or
#          build/asan/cmake/ / build/tsan/cmake/ for asan / tsan. PATH
#          gets the tree's executables
#          and a link to the repo's ./afwdev; LD_LIBRARY_PATH gets every
#          library dir (required: afw_environment_load_extension falls
#          back to the installed lib dir); C probes get the tree's libafw,
#          library dirs and -I dirs. --env-mode asan and tsan always use
#          this (a sanitizer build is never installed system-wide); other
#          modes only with --build-tree. See designs/asan-opt-in.md.
#

import json
import os
import shlex
import subprocess

from _afwdev.common import msg
from _afwdev.test.sanitize import SANITIZER_MODES, sanitizer_mode

_EXECUTABLES = ('afw', 'afwfcgi')


def tree_dir(options):
    """cmake tree --build-tree uses for this --env-mode."""
    root = (options or {}).get('afw_package_dir_path') or os.getcwd()
    if sanitizer_mode(options):
        return os.path.join(root, 'build', options['mode'], 'cmake')
    return os.path.join(root, 'build', 'cmake')


def build_command(options):
    mode = sanitizer_mode(options)
    if mode:
        return './afwdev build --cdev --sanitize ' + mode['variant']
    return './afwdev build --cdev'


def cache_value(tree, name):
    """Value of a CMakeCache.txt entry, or None."""
    try:
        with open(os.path.join(tree, 'CMakeCache.txt'), encoding='utf-8',
                errors='replace') as f:
            for line in f:
                if line.startswith(name + ':'):
                    return line.split('=', 1)[1].strip()
    except OSError:
        pass
    return None


def _scan(tree):
    """(executable dirs, library dirs) under <tree>/src, sorted."""
    bin_dirs = []
    lib_dirs = []
    src = os.path.join(tree, 'src')
    for name in sorted(os.listdir(src)) if os.path.isdir(src) else []:
        d = os.path.join(src, name)
        if not os.path.isdir(d):
            continue
        files = os.listdir(d)
        if any(f in _EXECUTABLES and os.access(os.path.join(d, f), os.X_OK)
                for f in files):
            bin_dirs.append(d)
        if any(f.startswith('lib') and f.endswith('.so') for f in files):
            lib_dirs.append(d)
    return bin_dirs, lib_dirs


def _include_dirs(tree):
    """-I dirs used to compile the tree (compile_commands.json), in order."""
    seen = []
    try:
        with open(os.path.join(tree, 'compile_commands.json'),
                encoding='utf-8') as f:
            entries = json.load(f)
    except (OSError, ValueError):
        return seen
    for entry in entries:
        args = entry.get('arguments') or (entry.get('command') or '').split()
        directory = entry.get('directory') or tree
        for i, a in enumerate(args):
            d = None
            if a == '-I' and i + 1 < len(args):
                d = args[i + 1]
            elif a.startswith('-I') and len(a) > 2:
                d = a[2:]
            if d:
                d = os.path.normpath(os.path.join(directory, d))
                if d not in seen:
                    seen.append(d)
    return seen


def _afwdev_link_dir(options, tree):
    """Dir holding an `afwdev` wrapper for the repo's afwdev.py.

    The repo's ./afwdev only works from the repo root (relative path);
    tests run afwdev from other directories, so the wrapper uses the
    absolute path of src/afw_dev/afwdev.py.
    """
    root = (options or {}).get('afw_package_dir_path') or os.getcwd()
    target = os.path.join(root, 'src', 'afw_dev', 'afwdev.py')
    if not os.path.isfile(target):
        return None
    link_dir = os.path.join(tree, 'afwdev-bin')
    wrapper = os.path.join(link_dir, 'afwdev')
    body = '#!/bin/sh\nexec python3 ' + shlex.quote(target) + ' "$@"\n'
    os.makedirs(link_dir, exist_ok=True)
    if os.path.lexists(wrapper) and (os.path.islink(wrapper) or
            open(wrapper).read() != body):
        os.remove(wrapper)
    if not os.path.exists(wrapper):
        with open(wrapper, 'w') as f:
            f.write(body)
        os.chmod(wrapper, 0o755)
    return link_dir


def _warn_if_stale(options, tree, command):
    """Warn when the --sanitize stamp is from another commit or dirty."""
    short = sanitizer_mode(options)['short']
    from _afwdev.build.cmake import SANITIZE_STAMP_NAME
    try:
        with open(os.path.join(tree, SANITIZE_STAMP_NAME),
                encoding='utf-8') as f:
            stamp = json.load(f)
    except (OSError, ValueError):
        return
    root = (options or {}).get('afw_package_dir_path') or os.getcwd()
    try:
        head = subprocess.run(['git', 'rev-parse', 'HEAD'], cwd=root,
            capture_output=True, text=True, check=True).stdout.strip()
    except (OSError, subprocess.CalledProcessError):
        head = None
    if head and stamp.get('commit') and stamp['commit'] != head:
        msg.warn(short + ' build is from commit ' + stamp['commit'][:10] +
            ', HEAD is ' + head[:10] + '. If C changed since, rebuild: ' +
            command)
    elif stamp.get('dirty'):
        msg.warn(short + ' build was made from a tree with uncommitted '
            'changes (' + str(stamp.get('built')) + '). If C changed '
            'since, rebuild: ' + command)


def prepare(options):
    """Point this process (and every child) at the build tree, or exit."""
    tree = tree_dir(options)
    command = build_command(options)
    if not os.path.isfile(os.path.join(tree, 'CMakeCache.txt')):
        msg.error_exit('--build-tree needs the cmake tree ' + tree +
            '. Build it first: ' + command)
    bin_dirs, lib_dirs = _scan(tree)
    libafw_dir = os.path.join(tree, 'src', 'afw')
    if not os.path.exists(os.path.join(libafw_dir, 'libafw.so')) or \
            not bin_dirs:
        msg.error_exit('--build-tree: ' + tree + ' has no built libafw / '
            'afw. Build it first: ' + command)

    sanitize = cache_value(tree, 'AFWDEV_SANITIZE') or ''
    mode = sanitizer_mode(options)
    if mode:
        if mode['sanitizer'] not in sanitize.split(';'):
            msg.error_exit(tree + ' is not a ' + mode['name'] + ' build. '
                'Rebuild: ' + command)
        _warn_if_stale(options, tree, command)
    elif sanitize:
        modes = [m for m, v in sorted(SANITIZER_MODES.items())
            if v['sanitizer'] in sanitize.split(';')]
        msg.error_exit(tree + ' is a sanitizer build (' + sanitize + '); '
            'use --env-mode ' + (' / '.join(modes) or 'asan / tsan') +
            ' for it.')

    path = list(bin_dirs)
    link_dir = _afwdev_link_dir(options, tree)
    if link_dir:
        path.append(link_dir)
    os.environ['PATH'] = os.pathsep.join(path + [os.environ.get('PATH', '')])
    os.environ['LD_LIBRARY_PATH'] = os.pathsep.join(
        lib_dirs + [d for d in
            os.environ.get('LD_LIBRARY_PATH', '').split(os.pathsep) if d])
    os.environ['AFW_LIB_DIR'] = libafw_dir
    os.environ['AFW_LIB_DIRS'] = os.pathsep.join(lib_dirs)
    include_dirs = _include_dirs(tree)
    if include_dirs:
        os.environ['AFW_INCLUDE_DIRS'] = os.pathsep.join(include_dirs)
    msg.highlighted_info('Build-tree test environment: ' + tree + ' (' +
        str(len(lib_dirs)) + ' library dirs)')
