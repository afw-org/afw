#!/usr/bin/env python3

# This test will be copied to the test extension's test directory
# and will be executed when 'afwdev test' is executed from the test
# package's directory.

# TODO: Maybe add another test like this but have one of "passed" set to False

import sys
import os
import json

test_dir = "/tmp/afw-test-srcdir-pattern"
package = "test-package"
package_dir = test_dir
extension = "hello_world_extension"
extension_dir = test_dir + "/src/" + extension
test_extension_dir = extension_dir + "/tests"
orig_test_dir = os.path.dirname(os.path.realpath(__file__))

def run():
    response = {
        "description": "Test for testing... testing",
        "tests": [{
            "description": "Test 1 in " + test_extension_dir,
            "test": "subcommand-test-option-srcdir-pattern",
            "passed": True
        }, {
            "description": "Test 2 in " + test_extension_dir,
            "test": "subcommand-test-option-srcdir-pattern",
            "passed": True
        }]
    }

    return response