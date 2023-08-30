#!/usr/bin/env python3

# Tests written in python will be executed by this mode implementation.

import os
import subprocess
import shlex
import json
from contextlib import redirect_stdout, redirect_stderr

from _afwdev.common import msg, nfc

def _process_commands_file(response, test, options):

    line_number = 0
    description = None
    first_empty_line_occurred = False

    with nfc.open(test, 'r') as fd:
        cwd = '/tmp'
        for line in fd:
            passed = False
            error = None
            line_number += 1

            #
            # Comment lines start with a '#'. The comments at the top of the
            # file are used as the description of the overall test. Comments
            # that occur after the first empty line are used as the description
            # of the individual tests
            #
            # Comments that span multiple lines are joined together with a
            # single space character.
            #
            # Beginning and trailing empty comment lines are ignored. The other
            # empty comment lines adds a newline to the description.
            #
            if line.startswith('#'):
                text = line[1:].strip()
                if len(text) > 0:
                    if description is None:
                        description = text
                    else:
                        if not description.endswith('\n'):
                            description += ' '
                        description += text
                else:
                    if description is not None:
                        description += '\n'
                continue

            #
            # Non-empty, non-comment lines that do not start with 'cd' are
            # executed as shell commands. Lines that start with 'cd' are
            # used to determine the current working directory for the subsequent
            # commands.
            #
            else:
                try:
                    split_line = shlex.split(line)
                    if len(split_line) == 0:
                        # The description before the first empty line is used
                        # as the description of the overall test.
                        if not first_empty_line_occurred:
                            if description is not None:
                                response["description"] = description
                                description = None
                            first_empty_line_occurred = True
                        continue
                    if split_line[0] == 'cd':
                        cwd = split_line[1]
                        passed = True
                    else:
                        rc = subprocess.run(
                            split_line,
                            cwd=cwd,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE)
                        if rc.returncode == 0:
                            passed = True
                        else:
                            error = rc.stderr.decode('utf-8')

                except Exception as e:
                    error: str(e)

                # If a description was not provided for the test, use the
                # command line as the description.
                if description is None:
                    description = line
                if description.endswith('\n'):
                    description = description[0:-1]

                # Start object for this test.
                t = {
                    "description": description,
                    "passed": passed,
                    "test": "Line %d: %s" % (line_number, line.strip()),
                }

                # If there is an error, add it to the test object.
                if error is not None:
                    t["error"] = { "message": error }

                # Add the test object to the response.                      
                response["tests"].append(t)

                # Reset the description for the next test.
                description = None

def run_test(test, options, testEnvironment=None, testGroupConfig=None):

    # print("\ntestEnvironment: ")
    # print(json.dumps(testEnvironment, indent=4))
    # print("\ntestGroupConfig: ")
    # print(json.dumps(testGroupConfig, indent=4))

    response = None
    error = None
    debug = None    

    stderr_p = os.pipe()
    stdout_p = os.pipe()

    stderr_w = os.fdopen(stderr_p[1], 'w')
    stdout_w = os.fdopen(stdout_p[1], 'w')    
    stderr_w_closed = stdout_w_closed = False

    stdout_r = os.fdopen(stdout_p[0], 'r')
    stderr_r = os.fdopen(stderr_p[0], 'r')
   
    response = {
        "description": "Run list of commands",
        "tests": []
    }

    try:
        msg.debug("Running test commands file: %s" % test)
        with redirect_stdout(stdout_w):
            with redirect_stderr(stderr_w):

                _process_commands_file(response, test, options)
                             
                stdout_w.close()
                stdout_w_closed = True
                stderr_w.close()    
                stderr_w_closed = True
                
                debug = stderr_r.read()  

    except Exception as e:   
        error = e

    finally:
        try:
            stdout_r.close()
            stderr_r.close()

            if not stdout_w_closed:
                stdout_w.close()
            if not stderr_w_closed:
                stderr_w.close()        
        except:
            pass

    return response, error, debug