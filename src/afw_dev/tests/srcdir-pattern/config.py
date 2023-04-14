#!/usr/bin/env python3

# test configuration settings
Environment = "afwdev-srcdir-pattern"

from asyncio.subprocess import DEVNULL
import os
import shutil
import subprocess
import sys

from pprint import pprint

def before_all():
    test_dir = "/tmp/afw-test-srcdir-pattern"
    package = "test-package"
    package_dir = test_dir
    extension = "hello_world_extension"
    extension_dir = test_dir + "/src/" + extension
    test_extension_dir = extension_dir + "/tests"
    orig_test_dir = os.path.dirname(os.path.realpath(__file__))

    # remove any old tests
    if os.path.isdir(test_dir):
        try:
            shutil.rmtree(test_dir)
        except shutil.Error as error:
            print("Error removing " + test_dir)
            pprint(error)
            sys.exit(1)

    # create a new package for the tests
    result = subprocess.run(["afwdev", "--noprompt", 
        "make-afw-package", package, test_dir], stdout=DEVNULL, stderr=DEVNULL)

    if result.returncode != 0:
        print("Error creating package " + package)
        sys.exit(result.returncode)

    # create new extension 'afw' to test with
    result = subprocess.run(["afwdev", "--noprompt", 
        "make-extension", extension], cwd=test_dir, stdout=DEVNULL, stderr=DEVNULL)

    if result.returncode != 0:
        print("Error creating extension " + extension)
        sys.exit(result.returncode)

    # create new tests directory
    result = subprocess.run(["mkdir", "-p", test_extension_dir],
        stdout=DEVNULL, stderr=DEVNULL)

    if result.returncode != 0:
        print("Error creating " + test_extension_dir)
        sys.exit(result.returncode)

    # Python scripts starting with an underscore in this directory will be
    # copied to the tests directory for the extension we're testing again. Only
    # scripts that start with underscores will be copied. Scripts that only
    # start with ON underscore will be copied with the underscore removed from
    # the beginning of the file name. The name of scripts starting with more
    # than one underscore will be copied as is.
    #
    # e.g. _foo.py will be copied to foo.py and __bar__.py will be copied
    # without changing the name.
    #
    # foo.py will be run as a test and __bar__.py can be used to include
    # things like common variables, functions, etc.

    directory = os.path.dirname(os.path.realpath(__file__))
    # print(directory)

    for fn in os.listdir(directory):
        if not (fn.startswith("_") and fn.endswith(".py")):
            continue
        
        new_fn = fn

        if not fn.startswith("__"):
            new_fn = new_fn[1:]

        file = test_extension_dir + "/" + new_fn

        # print("Copying test " + fn + " to " + file)
        result = subprocess.run(["cp", directory + "/" + fn, file])
        
        if result.returncode != 0:
            # print("Error copying " + directory + "/" + fn + " to " + file)
            sys.exit(result.returncode)

        result = subprocess.run(["chmod", "+x", file])
        
        if result.returncode != 0:
            # print("Error making " + file + " executable")
            sys.exit(result.returncode)