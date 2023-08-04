#!/usr/bin/env python3

#
# This test afw --local 1.
#
# This script will search for files in the same directory whose names match the
# pattern "local_test_*_input.txt". The "local_test_*" part is the test
# name.
#
# There should be 2 other files with names that are the test name plus
# "_expect.txt" and "_description.txt". "_expect.txt" is what is expected to be
# written to stdout. "_description.txt" is the description for the test.
#
# The actual data written to stdout is store in a file with the test name plus
# "_actual.txt". This can be helpful while developing a test to determine what
# should be in the "_input.txt" file.
#

import os
import subprocess
import glob

response = {
    "description": "Test afw --local",
    "tests": []
}


def run():
    # Find local tests and perform
    for input_path in sorted(glob.glob("local_test_*_input.txt")):

        test = input_path.replace('_input.txt','')
        passed = False

        # input
        with open(input_path, 'rb') as fd:
            input_test = fd.read()

        # description
        description_path = test + '_description.txt'
        if os.path.exists(description_path):
            with open(description_path) as fd:
                description = fd.read()
        else:
            description = test.replace('_', ' ')

        # expect
        expect_path = test + '_expect.txt'
        if os.path.exists(expect_path):
            with open(expect_path, 'rb') as fd:
                expected = fd.read()
        else:
            expected = 'Need ' + expect_path

        # skip?
        ignore_path = test + '_ignore.txt'
        if os.path.exists(ignore_path):
            skip = True
            passed = True
        
        # not skip
        else:
            skip = False

            # execute afwdev command
            result = subprocess.run(
                [
                    "afw",
                    "--local",
                    "1",
                ],
                input=input_test,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE)

            # Compare to expected result
            with open(test + '_actual.txt', 'wb') as fd:
                fd.write(result.stdout)
            if result.stdout == expected:
                passed = True

        # add test to response
        response["tests"].append({
            "description": description,
            "skip": skip,
            "passed": passed,
            "test": test
        })

    # print response
    return response