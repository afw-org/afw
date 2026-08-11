#!/usr/bin/env python3

##
# @file python.py
# @ingroup afwdev_test_modes
# @brief This file defines the run method for running tests under the 
#        "python" test mode
# @details These tests are run directly under the python interpreter and 
#          therefore only Python scripts (.py) are supported.
#

import os
import json
import importlib
import threading
from contextlib import redirect_stdout, redirect_stderr

from _afwdev.common import msg
from _afwdev.common.errors import wrap_exception

##
# @brief Runs the tests under the python interpreter.
# @param test The test to run.
# @param options The options dictionary.
# @param testEnvironment The test environment.
# @param testGroupConfig The test group configuration.
#
def run_test(test, options, testEnvironment=None, testGroupConfig=None):

    response = None
    error = None
    debug = None    
    stdout = None

    stderr_p = os.pipe()
    stdout_p = os.pipe()

    stderr_w = os.fdopen(stderr_p[1], 'w')
    stdout_w = os.fdopen(stdout_p[1], 'w')    
    stderr_w_closed = stdout_w_closed = False

    stdout_r = os.fdopen(stdout_p[0], 'r')
    stderr_r = os.fdopen(stderr_p[0], 'r')

    try:   
        msg.debug("Running python test: %s" % test)
        # Drain stdout/stderr pipes in a background thread so child processes
        # spawned by tests (e.g. Session("local") → afw with stdout=sys.stdout)
        # cannot fill the OS pipe buffer and deadlock while run() holds the
        # writer ends. Reading only after run() returns is a classic hang.
        drain_buf_out = []
        drain_buf_err = []

        def _drain(fd, buf):
            try:
                while True:
                    chunk = fd.read(65536)
                    if not chunk:
                        break
                    buf.append(chunk)
            except Exception:
                pass

        with redirect_stdout(stdout_w):
            with redirect_stderr(stderr_w):
                t_out = threading.Thread(
                    target=_drain, args=(stdout_r, drain_buf_out), daemon=True)
                t_err = threading.Thread(
                    target=_drain, args=(stderr_r, drain_buf_err), daemon=True)
                t_out.start()
                t_err.start()

                test_module = importlib.machinery.SourceFileLoader("test", test).load_module()                                                              
                if hasattr(test_module, 'run'): 
                    # uncomment these, if we want to pass along the same data to the actual python test module                   
                    response = test_module.run(
                        #options=options, 
                        #testEnvironment=testEnvironment, 
                        #testGroupConfig=testGroupConfig, 
                        #msg=msg
                    )
                else:
                    # FIXME: This should be a test failure.
                    #msg.error("No run() method found in test module " + test + "!")
                    pass

                stdout_w.close()
                stdout_w_closed = True
                stderr_w.close()    
                stderr_w_closed = True

                t_out.join(timeout=5.0)
                t_err.join(timeout=5.0)

                stdout = "".join(drain_buf_out)
                debug = "".join(drain_buf_err)

                if not hasattr(test_module, 'run'):                                        
                    response = json.loads(stdout)        
        
    except Exception as e:
        if stdout:
            debug = (debug or "") + stdout
        error = wrap_exception(e)

    finally:
        try:
            stdout_r.close()
            stderr_r.close()

            if not stdout_w_closed:
                stdout_w.close()
            if not stderr_w_closed:
                stderr_w.close()        
        except Exception:
            pass

    return response, error, debug