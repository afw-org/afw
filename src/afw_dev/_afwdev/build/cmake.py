#!/usr/bin/env python3

##
# @file cmake.py
# @ingroup afwdev_build
# @brief This file contains the main entry point for the "cmake" build.
# @details The "cmake" build builds all C-related source code into their 
#          appropriate binary libraries and executables.
#

import subprocess
import os
import sys
from _afwdev.common import msg, package

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

    _configure_command = ['cmake']
    #FIXME MIght want to figure out something to pass flags cmake understands.
    # if msg.is_verbose:
    #     _configure_command.extend(['--verbose'])
    _configure_command.extend(['-S', '.', '-B', options['build_directory_rpath_cmake']])
    if options.get('build_prefix') is not None:
        _configure_command.extend(['-DCMAKE_INSTALL_PREFIX=' + options.get('build_prefix')])
    if options.get('build_package', False) and options.get('build_prefix') is not None:
        # The CPACK_INSTALL_PREFIX is used to specify where files get installed into target system
        _configure_command.extend(['-DCPACK_INSTALL_PREFIX=' + options.get('build_prefix')])
        # the CPACK_PACKAGING_INSTALL_PREFIX is used to specify where files get located in the package (internally)
        _configure_command.extend(['-DCPACK_PACKAGING_INSTALL_PREFIX=' + options.get('build_prefix')])
    if options.get('build_scan') is True:
        _configure_command.extend(['-DCMAKE_EXPORT_COMPILE_COMMANDS=YES'])
    msg.highlighted_info('Running ' + str(" ".join(_configure_command)))
    rc = subprocess.run(_configure_command,
        cwd=options['afw_package_dir_path'],
        stdout=stdout_capture)
    if rc.returncode != 0:
        msg.error_exit("CMake configure failed " + str(rc))

    # make
    _make_command = ['cmake', '--build', options['build_directory_rpath_cmake']]
    if msg.is_verbose:
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
        msg.error_exit("CMake configure failed " + str(rc))

    # cpack
    if options.get('build_package', False):
        _package_command = ['cpack']
        if msg.is_verbose:
            _package_command.extend(['--verbose'])        
        msg.highlighted_info('Running ' + str(" ".join(_package_command)))
        rc = subprocess.run(_package_command,
            cwd=options['afw_package_dir_path'] + 'build/cmake',
            stdout=stdout_capture)
        if rc.returncode != 0:
            msg.error_exit("cpack failed " + str(rc))
        

    # if --scan was specified, run analyze-build
    if options.get('build_scan', True):
        # on Ubuntu, the analyze-build symlink is broken, so
        # we need to check if analyze-build-14 exists first
        _analyze_command = ['analyze-build']
        if os.path.exists('/usr/bin/analyze-build-14'):
            _analyze_command = ['analyze-build-14']

        _analyze_command.extend(['--cdb', 
            'build/cmake/compile_commands.json', 
            #'--disable-checker', 'deadcode.DeadStores', 
            '--status-bugs', '--verbose'])
        
        maxloop = options.get('build_maxloop', '10')
        _analyze_command.extend(['--maxloop', maxloop])

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

        if msg.is_verbose:
            _install_command.extend(['--verbose'])

        msg.highlighted_info('Running ' + str(" ".join(_install_command)))
        rc = subprocess.run(_install_command,cwd=options['afw_package_dir_path'], stdout=stdout_capture, stderr=subprocess.PIPE)
        if rc.returncode != 0:
            msg.error(rc.stderr.decode(sys.stderr.encoding))
            msg.error("If the problem is a permission denied error and you can use sudo, try running with the --sudo parameter specified on afwdev. This will only use sudo for the install step.")
            msg.error_exit("CMake install failed " + str(rc))
