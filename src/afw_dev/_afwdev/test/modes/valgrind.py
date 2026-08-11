#!/usr/bin/env python3

##
# @file valgrind.py
# @ingroup afwdev_test_modes
# @brief This file defines the run method for running tests under the
#        "valgrind" test mode
# @details These tests are run under afw command-line, using valgrind.
#          The advantage of using valgrind is to perform comprehensive 
#          memory checking on each test.
#
#          Only Adaptive Scripts (.as) files are supported.
#

import os
import subprocess

from _afwdev.common import msg, nfc, resources
from _afwdev.common.errors import AfwdevProcessError, wrap_exception
from _afwdev.test.common import format_abnormal_process_exit

##
# @brief Runs the tests under the afw command line tool and valgrind.
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
    stderr = None
    syntax = True
    p = None
    work_dir = None
    
    try:
        response = error = stdout = stderr = p = None
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
                if testEnvironment.get('afw_conf') and test.endswith(".as"):
                    afw_cmd.append('--conf')
                    afw_cmd.append('afw.conf')
                                                    

        # only run valgrind on Adaptive Script (.as) files
        if test.endswith(".as"):
            # when using valgrind, we have to run afw manually under valgrind
                
            # determine the path to valgrind.suppress
            # FIXME this needs to be a resource that we load/copy into the temp dir
            afw_package_dir_path = options.get("afw_package_dir_path")
            resources.copy_resources(options, "test/", todir=work_dir)            

            msg.debug("Running test script: %s" % test)
            # Cap runtime so a stuck/valgrind-thrashing test cannot hang the
            # parallel pool forever (seen with --env-mode valgrind -j).
            # Per-test default is generous: full suite is still slow overall.
            test_timeout = float(options.get("valgrind_test_timeout_s") or 300)
            try:
                p = subprocess.run([
                    'valgrind',
                    '--suppressions={0}/valgrind.suppress'.format(work_dir),
                    '--xml=yes',
                    '--xml-fd=2',
                    '--show-possibly-lost=no'
                    ] + afw_cmd + [test],
                    cwd=work_dir,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                    timeout=test_timeout)
            except subprocess.TimeoutExpired as e:
                raise AfwdevProcessError(
                    "valgrind test timed out after {}s: {}".format(
                        test_timeout, test)
                ) from e
        else:
            msg.debug("Skipping test script (not adaptive script): %s" % test)
            return None, None, None

        if p.returncode < 0:
            raise AfwdevProcessError(
                format_abnormal_process_exit(p.returncode),
                returncode=p.returncode,
            )

        stderr = p.stderr.decode("utf-8")
        stdout = p.stdout.decode("utf-8")              
        
        if not (test.endswith(".as") and syntax != "test_script"):
            # parse the output of the test script
            response = nfc.json_loads(stdout)        
    
    except Exception as e:                
        if stdout:            
            msg.debug(stdout)       
        error = wrap_exception(e)

    # if the stderr stream reports valgrind errors, then wrap them
    if stderr != None and stderr.find("<error>") > -1:
        error = AfwdevProcessError("Valgrind Error(s) detected.")

    return response, error, stderr