#!/usr/bin/env python3

import os
from _afwdev.common import msg

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