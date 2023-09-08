#!/usr/bin/env python3

import os
import subprocess

from _afwdev.common import msg, nfc

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

            msg.debug("Running test script: %s" % test)
            p = subprocess.run([
                'valgrind', 
                '--suppressions={0}/src/afw_dev/_afwdev/test/valgrind.suppress'.format(afw_package_dir_path), 
                '--xml=yes', 
                '--xml-fd=2', 
                '--show-possibly-lost=no'
                ] + afw_cmd + [test], cwd=work_dir, stdout = subprocess.PIPE, stderr=subprocess.PIPE)
        else:
            msg.debug("Running test: %s" % test)            
            p = subprocess.run(afw_cmd + [test], cwd=work_dir, stdout = subprocess.PIPE, stderr=subprocess.PIPE)

        if p.returncode < 0:
            raise Exception("Process was terminated with return code {}".format(p.returncode))

        stderr = p.stderr.decode("utf-8")
        stdout = p.stdout.decode("utf-8")              
        
        if not (test.endswith(".as") and syntax != "test_script"):
            # parse the output of the test script
            response = nfc.json_loads(stdout)        
    
    except Exception as e:                
        if stdout:            
            msg.debug(stdout)       
        error = e     

    # if the stderr stream reports valgrind errors, then wrap them in an Exception
    if stderr != None and stderr.find("<error>") > -1:
        # we have a valgrind error
        error = Exception("Valgrind Error(s) detected.")   

    return response, error, stderr