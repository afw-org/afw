#!/usr/bin/env python3

import os
from _afwdev.common import msg

def run(options, srcdirs):

    # first check that node_modules exist
    if not os.path.exists('node_modules'):
        # create it
        msg.highlighted_info('node_modules not found.  Run npm install first.')
        os.system('npm install')        

    # npm run test
    os.system('npm run test')