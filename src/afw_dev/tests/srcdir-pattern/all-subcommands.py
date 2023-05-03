#!/usr/bin/env python3

import os
import subprocess

test_dir = "/tmp/afw-test-srcdir-pattern"
package = "test-package"
package_dir = test_dir
extension = "hello_world_extension"
extension_dir = test_dir + "/src/" + extension
test_extension_dir = extension_dir + "/tests"
orig_test_dir = os.path.dirname(os.path.realpath(__file__))

response = {
    "description": "afwdev srcdir-pattern CLI option",
    "tests": []
}

#
# Test the srcdir-option option for the 'generate' subcommand
#
def run():

    # execute afwdev generate command for everything without the srcdir-pattern option
    result = subprocess.run(["afwdev", "generate" ], 
        cwd=package_dir, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    # append the response
    response["tests"].append({
        "description": "'afwdev generate' without the srcdir-pattern option" +
            " in " + test_dir,
        "passed": result.returncode == 0,
        "test": "subcommand-generate-option-srcdir-pattern"
    })

    # execute afwdev generate command for everything using "\*" as the srcdir-pattern
    result = subprocess.run(["afwdev", "generate", "--srcdir-pattern", "\*" ], 
        cwd=package_dir, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    # append the response
    response["tests"].append({
        "description": "'afwdev generate --srcdir-pattern \*' in " + test_dir,
        "passed": result.returncode == 0,
        "test": "subcommand-generate-option-srcdir-pattern"
    })

    # execute afwdev generate command for a subset of things using the extension
    # name as the srcdir-pattern
    result = subprocess.run(["afwdev", "generate", "--srcdir-pattern", extension], 
        cwd=package_dir, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    # append the response
    response["tests"].append({
        "description": "'afwdev generate --srcdir-pattern " + extension + 
            "' in " + test_dir,
        "passed": result.returncode == 0,
        "test": "subcommand-generate-option-srcdir-pattern"
    })

    return response