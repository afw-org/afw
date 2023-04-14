#! /usr/bin/env python3

import os
import sys
import shlex
import shutil
import subprocess
from glob import glob

from _afwdev.common import msg, package, resources
from _afwdev.build import js, cmake, docker
from _afwdev.docs import docs

# Build the afw package
# @todo add support of other build types
def run(options):   

    # Unless verbose or debug mode, stdout will be sent to dev/null.
    # Errors will still got to stderr either way.
    stdout_capture = subprocess.DEVNULL
    if msg.is_verbose_mode() or msg.is_debug_mode():
        stdout_capture = None

    # Build type contexts.
    build_type_contexts = [
        'cmake',
        'docker',
        'docs',
        'js'
    ]

    # Convenience switches.
    build_convenience_switches = [
        'clean',
        'generate',
        'install'
    ]

    # --all sets all build type contexts.
    if options.get('build_all', False):
        for build_type_context in build_type_contexts:
            options['build_' + build_type_context] = True

    # --full sets all convenience switches.
    if options.get('build_full', False):
        for build_convenience_switch in build_convenience_switches:
            options['build_' + build_convenience_switch] = True

    # Default context switch is --cmake if no other build context switches
    # are specified.
    none_set = True
    for build_type_context in build_type_contexts:
        if options.get('build_' + build_type_context, False):
            none_set = False
            break
    if none_set:
        options['build_cmake'] = True


    # Set build directories:
    options['build_directory_rpath'] = 'build/'
    options['build_directory'] = options['afw_package_dir_path'] + 'build/'
    for build_type_context in build_type_contexts:
        options['build_directory_rpath_' + build_type_context] = \
            options['build_directory_rpath'] + build_type_context + '/'
        options['build_directory_' + build_type_context] = \
            options['build_directory'] + build_type_context + '/'

    # Remove build directories for all specified build contexts. 
    if options.get('build_clean', False):
        for build_type_context in build_type_contexts:
            if options.get('build_' + build_type_context, False):
                _build_directory = options['build_directory_' + build_type_context]
                if os.path.exists(_build_directory):
                    msg.highlighted_info("Removing " + _build_directory)
                    shutil.rmtree(_build_directory, ignore_errors=True)  

    # generate \*
    if options.get('build_generate', False):
        _generate_command = [options['afwdev_path'], 'generate']
        msg.highlighted_info('Running ' + str(" ".join(_generate_command)))
        rc = subprocess.run(_generate_command,
            cwd=options['afw_package_dir_path'],
            stdout=stdout_capture)
        if rc.returncode != 0:
            msg.error_exit("Generate failed " + str(rc))

    # call the appropriate build function for each build context
    if options.get('build_docs', False):
        docs.build(options)

    if options.get('build_docker', False):
        docker.build(options)

    if options.get('build_js', False):
        js.build(options)

    if options.get('build_cmake', False):
        cmake.build(options)
