#! /usr/bin/env python3

##
# @file build.py
# @ingroup afwdev_build
# @brief This file contains the main entry point for the "build" subcommand.
# @details Orchestrates the cmake, docs, js, and docker build contexts.
#          Context flags (build_cmake, build_docs, ...) are independent.
#          --cdev enables clean, generate, install, and parallel jobs (-j)
#          for C/Python work; --fulldev enables all contexts plus generate,
#          clean, install, scan, and -j (full package dev install). --all
#          enables every context only (not generate/install). With no context
#          selected, cmake is the default. When --generate is set,
#          `afwdev generate` runs first.
#

##
# @defgroup afwdev_build_docs docs
# @ingroup afwdev_build
# @brief The afwdev build --docs subcommand
# @details The afwdev build subcommand is used to build different parts of 
#        Adaptive Framework packages. The --docs option builds the 
#        documentation for the Adaptive Framework package.
#

import os
import sys
import shlex
import shutil
import subprocess
from glob import glob

from _afwdev.common import msg, package, resources
from _afwdev.build import js, cmake, docker
from _afwdev.build.docs import docs

_BUILD_TYPE_CONTEXTS = (
    'cmake',
    'docker',
    'docs',
    'js',
)

# --cdev and --fulldev both turn these on. --all does not.
_BUILD_CONVENIENCE_SWITCHES = (
    'clean',
    'generate',
    'install',
)


def apply_build_profile_flags(options):
    """Enable flags implied by --cdev / --fulldev / --all / default cmake.

    Both --cdev and --fulldev set --install (and --generate, --clean, -j).
    --all only selects build contexts.
    """

    def _ensure_parallel_jobs():
        if options.get('build_make_jobs') is None:
            options['build_make_jobs'] = 0

    # --fulldev: full package dev install profile (sibling of --cdev).
    # All contexts + generate + clean + install + clang scan + parallel jobs.
    if options.get('build_fulldev', False):
        options['build_all'] = True
        for build_convenience_switch in _BUILD_CONVENIENCE_SWITCHES:
            options['build_' + build_convenience_switch] = True
        options['build_scan'] = True
        _ensure_parallel_jobs()

    # --all sets all build type contexts (does not enable generate/install).
    if options.get('build_all', False):
        for build_type_context in _BUILD_TYPE_CONTEXTS:
            options['build_' + build_type_context] = True

    # --cdev sets convenience switches for C/Python day-to-day work.
    if options.get('build_cdev', False):
        for build_convenience_switch in _BUILD_CONVENIENCE_SWITCHES:
            options['build_' + build_convenience_switch] = True
        _ensure_parallel_jobs()

    # Default context switch is --cmake if no other build context switches
    # are specified.
    none_set = True
    for build_type_context in _BUILD_TYPE_CONTEXTS:
        if options.get('build_' + build_type_context, False):
            none_set = False
            break
    if none_set:
        options['build_cmake'] = True


##
# @brief The main entry point for the "build" subcommand
# @details This routine is called during "afwdev build" in order to build
#          Adaptive Framework packages, including different parts requiring 
#          different build parts and steps.
# @param options The options dictionary.
#
def run(options):   

    # Unless verbose or debug mode, stdout will be sent to dev/null.
    # Errors will still got to stderr either way.
    stdout_capture = subprocess.DEVNULL
    if msg.is_verbose_mode() or msg.is_debug_mode():
        stdout_capture = None

    apply_build_profile_flags(options)


    # Set build directories:
    options['build_directory_rpath'] = 'build/'
    options['build_directory'] = options['afw_package_dir_path'] + 'build/'
    for build_type_context in _BUILD_TYPE_CONTEXTS:
        options['build_directory_rpath_' + build_type_context] = \
            options['build_directory_rpath'] + build_type_context + '/'
        options['build_directory_' + build_type_context] = \
            options['build_directory'] + build_type_context + '/'

    # Remove build directories for all specified build contexts. 
    if options.get('build_clean', False):
        for build_type_context in _BUILD_TYPE_CONTEXTS:
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
    if options.get('build_cmake', False):
        cmake.build(options)
        
    if options.get('build_docs', False):
        docs.build(options)

    if options.get('build_docker', False):
        docker.build(options)

    if options.get('build_js', False):
        js.build(options)
