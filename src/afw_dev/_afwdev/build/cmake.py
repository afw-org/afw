#!/usr/bin/env python3

##
# @file cmake.py
# @ingroup afwdev_build
# @brief This file contains the main entry point for the "cmake" build.
# @details The "cmake" build builds all C-related source code into their 
#          appropriate binary libraries and executables. Order is configure,
#          build, then optional cpack, AFW printf scan + analyze-build
#          (--scan), and install.
#

import datetime
import glob
import json
import subprocess
import os
import sys
import re
import tempfile
from _afwdev.common import msg, package
from _afwdev.build import printf_scan


def _highest_versioned(prefix):
    """Return the prefix-<N> path with the largest N, or None."""
    best = None
    for path in glob.glob(prefix + '-[0-9]*'):
        suffix = path[len(prefix) + 1:]
        if suffix.isdigit() and (best is None or int(suffix) > best[0]):
            best = (int(suffix), path)
    return best[1] if best else None

_C_DEFINE_RE = re.compile(r'^[A-Za-z_][A-Za-z0-9_]*(?:=[A-Za-z0-9_]+)?$')
_CDEV_DEBUG_DEFINES = (
    'AFW_DEBUG_EVALUATION',
    'AFW_DEBUG_LOCK',
    'AFW_DEBUG_POOL',
)

# --cdev / --fulldev also define this when the compiler finds
# <valgrind/memcheck.h>: valgrind then sees inside pools (see
# afw_memory_annotate_internal.h). Without the headers it is left out
# with a warning, so a machine without valgrind still builds.
_CDEV_VALGRIND_DEFINE = 'AFW_VALGRIND_POOL'


def valgrind_headers_available(extra_cflags=()):
    """True if the C compiler can compile #include <valgrind/memcheck.h>."""
    cc = os.environ.get('CC', 'cc')
    with tempfile.TemporaryDirectory(prefix='afwdev_valgrind_') as d:
        src = os.path.join(d, 'probe.c')
        with open(src, 'w') as f:
            f.write('#include <valgrind/memcheck.h>\n'
                'int main(void) { return 0; }\n')
        try:
            rc = subprocess.run([cc] + list(extra_cflags) +
                ['-c', '-o', os.path.join(d, 'probe.o'), src],
                capture_output=True, text=True)
        except OSError:
            return False
    return rc.returncode == 0


def _cmake_install_prefix(options):
    if options.get('build_prefix'):
        return options.get('build_prefix')
    cache = os.path.join(
        options.get('afw_package_dir_path') or '',
        'build', 'cmake', 'CMakeCache.txt')
    if os.path.isfile(cache):
        try:
            with open(cache, encoding='utf-8', errors='replace') as f:
                for line in f:
                    if line.startswith('CMAKE_INSTALL_PREFIX:'):
                        return line.split('=', 1)[1].strip()
        except OSError:
            pass
    return '/usr/local'


def installed_include_dir(options):
    """Prefix include dir cmake --install writes public headers into."""
    prefix = _cmake_install_prefix(options)
    subdir = 'afw'
    try:
        afw_package = package.get_afw_package(options)
        subdir = afw_package.get('installPackageSubdir') or subdir
    except Exception:
        pass
    if subdir:
        return os.path.join(prefix, 'include', subdir)
    return os.path.join(prefix, 'include')


# Basenames cmake used to install that are no longer PUBLIC_HEADER.
# Keep in sync with src/afw/CMakeLists.txt FILTER excludes and the old
# generated names in whats-new.md (Upgrade hygiene). log_deprecated* is
# gone from the tree; still pruned from leftover prefix installs.
_LEFTOVER_HEADER_EXACT = frozenset((
    'afw_declare_helpers.h',
    'afw_model_location.h',
    'afw_array_template.h',
    # Renamed to *_internal.h; the old public names can still sit in the prefix.
    'afw_function_bindings.h',
    'afw_const_objects.h',
    'afw_generated.h',
))


def is_leftover_installed_header(name):
    """True if this basename should not remain in the install include dir."""
    if not name or not name.endswith('.h'):
        return False
    if name.endswith('_internal.h') or '_internal_' in name:
        return True
    if name in _LEFTOVER_HEADER_EXACT:
        return True
    if name.startswith('afw_log_deprecated'):
        return True
    if name.startswith('skeleton_'):
        return True
    if name.endswith('_declare_helpers.h'):
        return True
    return False


def prune_leftover_installed_headers(include_dir, sudo=False):
    """Remove leftover headers cmake install no longer ships.

    PUBLIC_HEADER install is additive: files dropped from the public list
    stay in the prefix. --cdev/--fulldev both --install; this makes that
    install match the current public header set.
    """
    if not include_dir or not os.path.isdir(include_dir):
        return []
    try:
        names = os.listdir(include_dir)
    except OSError:
        return []
    removed = []
    for name in names:
        if not is_leftover_installed_header(name):
            continue
        path = os.path.join(include_dir, name)
        msg.highlighted_info('Removing leftover installed header ' + path)
        if sudo:
            rc = subprocess.run(['sudo', 'rm', '-f', path])
            if rc.returncode == 0:
                removed.append(path)
            else:
                msg.error('Could not remove leftover ' + path)
            continue
        try:
            os.remove(path)
            removed.append(path)
        except OSError as e:
            msg.error('Could not remove leftover ' + path + ': ' + str(e))
    return removed


# Written into a --sanitize prefix so the sanitizer test mode can refuse a
# missing or stale build.
SANITIZE_STAMP_NAME = 'afwdev-sanitize.json'


def check_sanitizer_runtime(sanitizers):
    """Exit unless the C compiler can link a program with these sanitizers.

    Catches a toolchain without the runtime (musl/Alpine has no ASan
    runtime) before a long cmake run fails halfway.
    """
    cc = os.environ.get('CC', 'cc')
    flag = '-fsanitize=' + ','.join(sanitizers)
    with tempfile.TemporaryDirectory(prefix='afwdev_sanitize_') as d:
        src = os.path.join(d, 'probe.c')
        with open(src, 'w') as f:
            f.write('int main(void) { return 0; }\n')
        try:
            rc = subprocess.run([cc, flag, '-o', os.path.join(d, 'probe'),
                src], capture_output=True, text=True)
        except OSError as e:
            msg.error_exit('--sanitize: cannot run ' + cc + ': ' + str(e))
    if rc.returncode != 0:
        msg.error_exit('--sanitize: ' + cc + ' cannot link with ' + flag +
            ' (no sanitizer runtime for this toolchain?)\n' +
            (rc.stderr or '').strip())


def write_sanitize_stamp(options):
    """Record the sanitizers and source commit in the --sanitize build tree.

    --env-mode asan / tsan read it to warn about a stale build.
    """
    stamp_dir = options['build_directory_cmake']
    root = options['afw_package_dir_path']
    commit = None
    dirty = None
    try:
        commit = subprocess.run(['git', 'rev-parse', 'HEAD'], cwd=root,
            capture_output=True, text=True, check=True).stdout.strip()
        dirty = bool(subprocess.run(['git', 'status', '--porcelain'],
            cwd=root, capture_output=True, text=True,
            check=True).stdout.strip())
    except (OSError, subprocess.CalledProcessError):
        pass
    stamp = {
        'sanitizers': list(options['build_sanitizers']),
        'commit': commit,
        'dirty': dirty,
        'built': datetime.datetime.now(datetime.timezone.utc).isoformat(),
        'buildDirectory': options['build_directory_cmake'],
    }
    os.makedirs(stamp_dir, exist_ok=True)
    with open(os.path.join(stamp_dir, SANITIZE_STAMP_NAME), 'w') as f:
        json.dump(stamp, f, indent=4)
        f.write('\n')


##
# @brief The main entry point for the "cmake" build.
# @param options The options dictionary.
#
def build(options):

    # Unless verbose or debug mode, stdout will be sent to dev/null.
    # Errors will still got to stderr either way.
    stdout_capture = subprocess.DEVNULL
    if msg.is_verbose_mode() or msg.is_debug_mode():
        stdout_capture = None

    if options.get('build_sanitizers'):
        check_sanitizer_runtime(options['build_sanitizers'])

    _configure_command = ['cmake']
    # if msg.is_verbose:
    #     _configure_command.extend(['--verbose'])
    _configure_command.extend(['-S', '.', '-B', options['build_directory_rpath_cmake']])
    _c_defines = []
    for _d in options.get('build_define') or []:
        if not _C_DEFINE_RE.fullmatch(_d):
            msg.error_exit(
                'Invalid --define ' + str(_d) +
                ' (expected NAME or NAME=VALUE with letters, digits, underscore)')
        _c_defines.append(_d)
    if options.get('build_cdev') or options.get('build_fulldev'):
        for _name in _CDEV_DEBUG_DEFINES:
            if not any(_d.split('=', 1)[0] == _name for _d in _c_defines):
                _c_defines.append(_name)
        if not any(_d.split('=', 1)[0] == _CDEV_VALGRIND_DEFINE
                for _d in _c_defines):
            if valgrind_headers_available():
                _c_defines.append(_CDEV_VALGRIND_DEFINE)
            else:
                msg.warn('valgrind headers (<valgrind/memcheck.h>) not '
                    'found: building without ' + _CDEV_VALGRIND_DEFINE +
                    ', so valgrind will not see inside pools. Install '
                    'valgrind-devel (valgrind-dev on Alpine; the valgrind '
                    'package on Ubuntu).')
    if _c_defines:
        # Semicolon list: add_compile_definitions in the root CMakeLists.
        _configure_command.extend(['-DAFWDEV_C_DEFINES=' + ';'.join(_c_defines)])
    if options.get('build_sanitizers'):
        # Semicolon list: -fsanitize options in the root CMakeLists.
        _configure_command.extend(['-DAFWDEV_SANITIZE=' +
            ';'.join(options['build_sanitizers'])])
    if options.get('build_prefix') is not None:
        _configure_command.extend(['-DCMAKE_INSTALL_PREFIX=' + options.get('build_prefix')])
    if options.get('build_package', False) and options.get('build_prefix') is not None:
        # The CPACK_INSTALL_PREFIX is used to specify where files get installed into target system
        _configure_command.extend(['-DCPACK_INSTALL_PREFIX=' + options.get('build_prefix')])
        # the CPACK_PACKAGING_INSTALL_PREFIX is used to specify where files get located in the package (internally)
        _configure_command.extend(['-DCPACK_PACKAGING_INSTALL_PREFIX=' + options.get('build_prefix')])
    # Always emit compile_commands.json so clangd (and other IDEs) can resolve
    # Go to Definition. Previously this ran only for --scan / --fulldev, so a
    # later --cdev --clean left a dangling compile_commands.json symlink.
    _configure_command.extend(['-DCMAKE_EXPORT_COMPILE_COMMANDS=YES'])
    msg.highlighted_info('Running ' + str(" ".join(_configure_command)))
    rc = subprocess.run(_configure_command,
        cwd=options['afw_package_dir_path'],
        stdout=stdout_capture)
    if rc.returncode != 0:
        msg.error_exit("CMake configure failed " + str(rc))

    # make
    _make_command = ['cmake', '--build', options['build_directory_rpath_cmake']]
    if msg.is_verbose_mode():
        _make_command.extend(['--verbose'])
    if options.get('build_make_jobs') is None:
        if options['afwdev_settings'].get('make_jobs_argument'):
            _make_command.extend(options['afwdev_settings']['make_jobs_argument'].replace('--jobs', '--parallel').split(' '))
    elif options.get('build_make_jobs') == 0:
        _make_command.extend(['--parallel'])
    else:
        _make_command.extend(['--parallel', str(options.get('build_make_jobs'))])
        
    msg.highlighted_info('Running ' + str(" ".join(_make_command)))
    rc = subprocess.run(_make_command,
        cwd=options['afw_package_dir_path'],
        stdout=stdout_capture)
    if rc.returncode != 0:
        msg.error_exit("CMake build failed " + str(rc))

    if options.get('build_sanitizers'):
        write_sanitize_stamp(options)

    # cpack
    if options.get('build_package', False):
        _package_command = ['cpack']
        if msg.is_verbose_mode():
            _package_command.extend(['--verbose'])        
        msg.highlighted_info('Running ' + str(" ".join(_package_command)))
        rc = subprocess.run(_package_command,
            cwd=options['afw_package_dir_path'] + 'build/cmake',
            stdout=stdout_capture)
        if rc.returncode != 0:
            msg.error_exit("cpack failed " + str(rc))
        

    # if --scan was specified, typed AFW printf check then analyze-build
    if options.get('build_scan') is True:
        printf_scan.run_printf_scan(options)

    if options.get('build_scan') is True:
        # Ubuntu ships analyze-build only as analyze-build-<llvm major>
        # (the plain name is a broken symlink on 22.04, absent on 24.04),
        # so prefer the highest versioned one; other distros use the
        # plain name.
        _analyze_command = [_highest_versioned('/usr/bin/analyze-build')
            or 'analyze-build']

        _analyze_command.extend(['--cdb', 
            'build/cmake/compile_commands.json', 
            #'--disable-checker', 'deadcode.DeadStores', 
            '--status-bugs', '--verbose'])
        
        maxloop = options.get('build_maxloop', '10')
        _analyze_command.extend(['--maxloop', str(maxloop)])

        msg.highlighted_info('Running ' + str(" ".join(_analyze_command)))
        rc = subprocess.run(_analyze_command,
            cwd=options['afw_package_dir_path'])
        if rc.returncode < 0:
            msg.error_exit("analyze-build failed " + str(rc))
        if rc.returncode != 0:
            msg.error_exit("analyze-build failed. Number of bugs detected: " + str(rc.returncode))
    
    # make install. If it fails with 'Permission denied' do sudo make install
    if options.get('build_install', False):
        _install_command = ['cmake', '--install', options['build_directory_rpath_cmake']]

        # If --sudo argument was specified, call cmake with sudo command.
        if options.get('build_sudo', False):
            _install_command = ['sudo'] + _install_command

        if msg.is_verbose_mode():
            _install_command.extend(['--verbose'])

        msg.highlighted_info('Running ' + str(" ".join(_install_command)))
        rc = subprocess.run(_install_command,cwd=options['afw_package_dir_path'], stdout=stdout_capture, stderr=subprocess.PIPE)
        if rc.returncode != 0:
            msg.error(rc.stderr.decode(sys.stderr.encoding))
            msg.error("If the problem is a permission denied error and you can use sudo, try running with the --sudo parameter specified on afwdev. This will only use sudo for the install step.")
            msg.error_exit("CMake install failed " + str(rc))
        prune_leftover_installed_headers(
            installed_include_dir(options),
            sudo=bool(options.get('build_sudo')))

