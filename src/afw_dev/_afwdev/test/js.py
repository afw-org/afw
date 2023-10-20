#!/usr/bin/env python3

##
# @ingroup afwdev_test
#

##
# @file js.py
# @brief This file defines the run methods for running javascript tests.
#

import os
from _afwdev.common import msg

##
# @brief Runs the Javascript tests
# @details This routine is the main entry point for the "test" subcommand, 
#          specifically for running javascript tests.
# @param options The options dictionary.
# @param srcdirs The list of source directories to run tests for.
#
def run(options, srcdirs):

    # first check that node_modules exist
    if not os.path.exists('node_modules'):
        # create it
        msg.highlighted_info('node_modules not found.  Running npm install first.')
        os.system('npm install') 

    # check if src/afw_client/javascript/build exists
    if not os.path.exists('src/afw_client/javascript/build'):
        # building dependencies
        msg.highlighted_info('Building package dependencies.')
        os.system('npm run build:packages')

    # determine if running in parallel
    cmd = 'npm run test'
    if options.get('build_make_jobs') == 0:
        cmd = 'npm run test-p'
        
    # setup verbosity
    if msg.is_verbose_mode():
        os.environ['DEBUG_PRINT_LIMIT'] = '100000'
        cmd += ' -- --verbose'
    else:
        os.environ['DEBUG_PRINT_LIMIT'] = '0'

    # run the tests
    msg.highlighted_info('Running javascript tests.')
    os.system(cmd)