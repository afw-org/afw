#!/usr/bin/env python3

##
# @file afw.py
# @ingroup afwdev_test_modes
# @brief This file defines the run method for running tests through the "afw" 
#        mode.
# @details These tests are run directly under the afw command-line tool. 
#          Because of this, only Adaptive Scripts (.as) are supported.
#

import os
import subprocess

from _afwdev.common import msg, nfc
from _afwdev.common.errors import (
    AfwAdaptiveError,
    AfwdevProcessError,
    adaptive_error_from_response,
    wrap_exception,
)
from _afwdev.test.common import format_abnormal_process_exit

##
# @brief Runs the tests over the afw command line tool.
# @param test The test to run.
# @param options The options dictionary.
# @param testEnvironment The test environment.
# @param testGroupConfig The test group configuration.
#
def run_test(test, options, testEnvironment=None, testGroupConfig=None):
    
    # Run the test and parse the output
    response = None
    error = None
    stdout = None
    debug = None
    syntax = None
    p = None
    work_dir = None

    if not test.endswith(".as"):
        msg.debug("Skipping test script (not adaptive script): %s" % test)
        return None, None, None

    try:
        response = error = stdout = debug = p = None
        afw_cmd = []
        #interpreter = None
        with nfc.open(test) as f:
            shebang_line_parts = f.readline().split()    
            #interpreter = shebang_line_parts[0:2]       
            afw_cmd = shebang_line_parts[2:]      
            if '--syntax' in afw_cmd:
                # get the next part after --syntax
                syntax = afw_cmd[afw_cmd.index('--syntax') + 1]   
            work_dir = os.path.dirname(test)   

            # if we have a testGroupConfig, use it
            if testEnvironment:                
                work_dir = testEnvironment['work_dir']
                if testEnvironment.get('afw_conf'):
                    afw_cmd.append('--conf')
                    afw_cmd.append('afw.conf')
        
            msg.debug("Running test script: %s" % test)
            p = subprocess.run(afw_cmd + [test], cwd=work_dir, stdout = subprocess.PIPE, stderr=subprocess.PIPE)

        debug = p.stderr.decode("utf-8")
        stdout = p.stdout.decode("utf-8")         

        if p.returncode < 0:
            return None, AfwdevProcessError(
                format_abnormal_process_exit(p.returncode),
                returncode=p.returncode,
            ), debug

        if syntax == "test_script":
            # parse the output of the test script
            response = nfc.json_loads(stdout)
        elif p.returncode != 0:
            # Non-test_script: shell may exit 1 for non-integer returns
            # (e.g. return_const_object.as). Only treat as failure when
            # stdout is a structured Adaptive error payload (issue #61).
            try:
                body = nfc.json_loads(stdout) if stdout else None
            except Exception:
                body = None
            if body is not None:
                adapt = adaptive_error_from_response(body)
                if adapt is not None:
                    return None, adapt, debug
            # Otherwise leave response/error None → skipped (historical)

    except Exception as e:
        if stdout:
            debug = (debug or "") + stdout
        error = wrap_exception(e)

    return response, error, debug