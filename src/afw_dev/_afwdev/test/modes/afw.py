#!/usr/bin/env python3

import os
import subprocess

from _afwdev.common import msg, nfc

def run_test(test, options, testEnvironment=None, testGroupConfig=None):
    
    # Run the test and parse the output
    response = None
    error = None
    stdout = None
    debug = None
    syntax = None
    p = None
    cwd = None

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
            cwd = os.path.dirname(test)   

            # if we have a testGroupConfig, use it
            if testEnvironment:                
                cwd = testEnvironment['cwd']
                if testEnvironment.get('afw_conf'):
                    afw_cmd.append('--conf')
                    afw_cmd.append('afw.conf')
        
            p = subprocess.run(afw_cmd + [test], cwd=cwd, stdout = subprocess.PIPE, stderr=subprocess.PIPE)

        debug = p.stderr.decode("utf-8")
        stdout = p.stdout.decode("utf-8")         

        if p.returncode < 0:
            return None, "Process exited abnormally with return code {}".format(p.returncode), debug                      
        
        if syntax == "test_script":
            # parse the output of the test script
            response = nfc.json_loads(stdout)        
    
    except Exception as e:        
        if stdout:
            debug += stdout
        error = e        

    return response, error, debug