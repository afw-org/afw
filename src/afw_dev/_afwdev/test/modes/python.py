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
import sys
import json
import importlib.util
import threading
from contextlib import redirect_stdout, redirect_stderr

from _afwdev.common import msg
from _afwdev.common.errors import AfwdevRunnerError, wrap_exception
from _afwdev.test import context as test_context


def _module_name_for_test(test):
    # Each file needs its own module name. Loading every .py as "test"
    # reuses sys.modules["test"] and can inherit run() from the previous
    # file in the same process.
    return "_afwdev_python_test_" + str(abs(hash(os.path.abspath(test))))


def _load_test_module(mod_name, path):
    spec = importlib.util.spec_from_file_location(mod_name, path)
    if spec is None or spec.loader is None:
        raise AfwdevRunnerError("Unable to load python test: " + path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[mod_name] = module
    spec.loader.exec_module(module)
    return module


def _response_from_stdout(path, stdout):
    text = (stdout or "").strip()
    if not text:
        raise AfwdevRunnerError(
            "No run() and no JSON on stdout: " + path)
    try:
        return json.loads(text)
    except ValueError as e:
        raise AfwdevRunnerError(
            "No run() and stdout was not test result JSON: " + path
        ) from e

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

                mod_name = _module_name_for_test(test)
                if mod_name in sys.modules:
                    del sys.modules[mod_name]
                test_module = _load_test_module(mod_name, test)
                has_run = hasattr(test_module, 'run') and callable(
                    test_module.run)
                if has_run:
                    # c_probe (and similar helpers) read test context so
                    # --env-mode valgrind can wrap a compiled probe.
                    test_context.push(
                        options=options,
                        test=test,
                        testEnvironment=testEnvironment,
                        testGroupConfig=testGroupConfig,
                    )
                    try:
                        response = test_module.run()
                    finally:
                        test_context.pop()
                    if response is None:
                        raise AfwdevRunnerError(
                            "run() returned None: " + test)

                stdout_w.close()
                stdout_w_closed = True
                stderr_w.close()    
                stderr_w_closed = True

                t_out.join(timeout=5.0)
                t_err.join(timeout=5.0)

                stdout = "".join(drain_buf_out)
                debug = "".join(drain_buf_err)

                if not has_run:
                    response = _response_from_stdout(test, stdout)
                sys.modules.pop(mod_name, None) 
        
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