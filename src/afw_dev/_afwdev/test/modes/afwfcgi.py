#!/usr/bin/env python3

import os

from afw import Session
from python_bindings.script import evaluate_script
from _afwdev.common import msg

#
# The afwfcgi env mode will use the Python bindings to connect to a local
# nginx/afwfcgi service to process test scripts. This is only appropriate 
# for adaptive scripts, and specifically ones that do not require a custom
# afw.conf file.
#
# You need to be running afwfcgi and nginx prior to using this test mode, 
# and it should be listening on port 8080.
def run_test(test, options, testEnvironment=None, testGroupConfig=None):

    session = None
    result = None
    error = None
    debug = None
    stderr = os.pipe()
    stdout = os.pipe()
    pwd = os.getcwd()    

    if not test.endswith(".as"):
        msg.debug("Skipping test script (not adaptive script): %s" % test)
        return None, None, None

    try:        
        cwd = os.path.dirname(test)        
        if testEnvironment:
            cwd = testEnvironment['cwd']

        # change to cwd
        os.chdir(cwd)        

        # check if file afw.conf exists in current directory
        afw_conf = os.path.join(cwd, 'afw.conf')
        if not os.path.exists(afw_conf):
            session = Session(url="http://localhost:8080/afw", stdout=stdout[1], stderr=stderr[1])
        else:
            msg.debug("Skipping test script with custom afw.conf: %s" % test)
            return None, None, None

        source = open(test).read()      
        # look at the first line
        shebang = source.splitlines()[0]
        if '--syntax' in shebang:
            syntax = shebang.split('--syntax')[1].strip()

        if not syntax or syntax == 'test_script':         
            msg.debug("Running test script: %s" % test)
            result = evaluate_script(session, source)                    

    except Exception as e:
        msg.debug(str(e))
        error = e

    finally:
        os.chdir(pwd)

        if session:
            session.close()

            # read stdout and stderr
            os.close(stdout[1])
            os.close(stderr[1])

            stdout = os.fdopen(stdout[0]).read()            
            debug = os.fdopen(stderr[0]).read()            

            # print stdout to debug
            msg.debug(stdout)                

    return result, error, debug