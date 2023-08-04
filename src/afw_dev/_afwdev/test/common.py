#! /usr/bin/env python3

import os
import subprocess
import importlib
import re
from os.path import exists

from _afwdev.common import msg, nfc

# common routine for calling before/after functions
def test_wrapper(root, testGroupConfig, testEnvironment, action):
    cwd = root
    if testGroupConfig and testGroupConfig.get(action):
        testGroupConfig[action]()        

    elif exists(root + '/' + action + '.py'):
        msg.debug("Running " + action + ".py")
        subprocess.call(['python3', root + '/' + action + '.py'])

# function that is run before all tests in a test group
def before_all(root, testGroupConfig, testEnvironment):

    test_wrapper(root, testGroupConfig, testEnvironment, 'before_all')

# function that is run before each test in a test group
def before_each(root, testGroupConfig, testEnvironment):

    test_wrapper(root, testGroupConfig, testEnvironment, 'before_each')

# function that is run after each test in a test group
def after_each(root, testGroupConfig, testEnvironment):

    test_wrapper(root, testGroupConfig, testEnvironment, 'after_each')

# function that is run after all tests in a test group
def after_all(root, testGroupConfig, testEnvironment):

    test_wrapper(root, testGroupConfig, testEnvironment, 'after_all')


# This routine checks to determine if a discovered file is a test file.
# Some names are reserved for specific purposes, and are ignored as a test 
# file. For now, we only look for .as,.py,.sh scripts, ignoring '_' prefixed.
def is_test_file(file):    
    skip = [        
        "config.py"
    ]

    if (file.endswith(".as") or
        file.endswith(".py") or
        file.endswith(".sh") or
        re.match("^commands_.+\.txt$", file)):
        if file in skip or file.startswith("_"):
            return False        
        return True
    return False

# For a given testGroup and a list of all testEnvironments, this routine will
# return the matching test environment, if any.
def get_test_environment(path, testEnvironments, testGroupConfig, test_working_directory):

    testEnv = None
    
    if testEnvironments and testGroupConfig:
        environment = testGroupConfig.get('environment')
        if environment:
            for env in testEnvironments:
                if env['name'] == environment:
                    testEnv = env

    # get the directory path, relative to srcdir path    
    relpath = os.path.relpath(path, os.getcwd() + "/src")

    if not testEnv:        
        # if we don't have a configured test environment, create a default one
        testEnv = {
            'name': relpath,
            'path': path
        }

    # set the output working directory
    # Note: The relpath gets "flattened" by subbing '/' for '.'
    #       this is because the test runner will create a directory
    #       for each test group, and we want to make sure that the
    #       directory names do not overlap.
    testEnv['cwd'] = test_working_directory + "/" + relpath.replace("/", ".")

    # check if we've created a directory for this testEnv yet    
    if testEnv and not os.path.isdir(testEnv['cwd']):
        msg.debug("Creating test environment directory: " + testEnv['cwd'])        

        os.makedirs(testEnv['cwd'])        

        # copy everything from testEnv['path'] to testEnv['cwd']
        if testEnv.get('path'):
            msg.debug("Copying test environment files from: " + testEnv['path'])
            os.system("cp -r " + testEnv['path'] + "/* " + testEnv['cwd'])    

    return testEnv


# Given a source directory, this routine will find and load test environment 
# configurations within the srcdir/tests/environments/. Test groups will use
# use these environments when they are run.
#
# These environments allow test groups to share/use common configurations.
def load_test_environments(srcdir):

    testEnvironments = []

    # look for test/environments/ directory
    environmentsDir = srcdir + '/tests/environments'    
    if exists(environmentsDir):
        for environment in os.listdir(environmentsDir):            
            if os.path.isdir(environmentsDir + '/' + environment):                
                config = {}
                config['name'] = environment
                config['path'] = environmentsDir + '/' + environment

                # look for afw.conf
                if exists(environmentsDir + '/' + environment + '/afw.conf'):
                    config['afw_conf'] = environmentsDir + '/' + environment + '/afw.conf'

                testEnvironments.append(config)

    return testEnvironments


# if a test group has a 'config.py' file, this provides the test runner 
# with some configuration for this test group, including before/after 
# functions to call and default test environments.
def load_test_group_config(root):

    # check if this group has a config.py definition
    fullPath = root + '/config.py'
    if exists(fullPath):

        # import the test config module
        test_config = importlib.machinery.SourceFileLoader("config", fullPath).load_module()

        groupConfig = {
            'before_all': test_config.before_all if hasattr(test_config, 'before_all') else None,
            'after_all': test_config.after_all if hasattr(test_config, 'after_all') else None,
            'before_each': test_config.before_each if hasattr(test_config, 'before_each') else None,
            'after_each': test_config.after_each if hasattr(test_config, 'after_each') else None,
            'environment': test_config.Environment if hasattr(test_config, 'Environment') else None
        }                        
        
        return groupConfig
        


# Within a <srcdir>/tests/ directory, this routine will find all test groups
# and return a list of test groups. Each test group is a tuple containing
# the source directory, the test group directory, and a list of test files
def find_test_groups(srcdir, tests_dir):
    testGroups = []

    for root, dirs, files in os.walk(tests_dir):
        # exclude subdirectory environments 
        dirs[:] = [d for d in dirs if d != 'environments']

        tests = []
        containsTests = False    
        for f in files:            
            if is_test_file(f):
                containsTests = True
                tests.append(os.path.join(root, f))

        if containsTests:
            testGroups.append(
                (srcdir, root, tests)
            )

    return testGroups

# This is the main routine to run an individual test script inside a test 
# group. After running the test, it returns a tuple containing the test
# response, any errors, and the stderr output.
def run_test(test, options, testEnvironment=None, testGroupConfig=None):

    # look at the test file extension to determine how to run it
    # some files, such as .py files have to be run in 'python' mode
    if test.endswith(".py"):
        mode = "python"

    # Files whose name starts with 'commands_' and has the .txt file
    # extension are run in 'commands' mode.
    elif re.match(".*/commands_.+\.txt$", test):
        mode = "commands"

    else:
        # dynamically import the environment mode from the 'mode' directory
        mode = options.get('mode')    

    if mode:        
        try:
            mode = importlib.import_module('_afwdev.test.modes.' + mode)        

            # invoke the run_test method
            return mode.run_test(test, options, testEnvironment, testGroupConfig)

        except ImportError as e:
            msg.error("Unable to load mode: " + mode)
            msg.error(str(e))
    else:
        msg.error("No mode specified for test: " + test)

    return (None, None, None)


# Given a sourceLocation path, return the individual source "file" that 
# it refers to, in order to help with comparing two different sourceLocations
def get_source_location_basename(sourceLocation):

    # sourceLocation may look like /path/to/basename+offset(line:col)
    # parse out the basename
    if '/' in sourceLocation:
        basename = sourceLocation.split('/')[-1]
    else:
        basename = sourceLocation

    if '+' in basename:
        basename = basename.split('+')[0]

    return basename


def compare_source_locations(sourceLocation1, sourceLocation2):
    
    basename1 = get_source_location_basename(sourceLocation1)
    basename2 = get_source_location_basename(sourceLocation2)

    return basename1 == basename2

def get_source_location_coordinates(source, sourceLocation):    

    if source != None:
        cursor = 0

        sourceLines = source.split("\n")
        for index, sourceLine in enumerate(sourceLines):
            cursor += len(sourceLine) + 1

            if cursor > sourceLocation:
                offset = len(sourceLine) - (cursor - sourceLocation) + 1
                return index + 1, offset
            
    return None, None

# Formats a chunk of source code, with an optional title and 
# highlightOffset, to indicate where in the source code the 
# user should focus on.
def format_source_code(source, title = None, highlightOffset = 0, maxLines = 10, message = None):

    if source != None:
        if title:
            msg.error("    {}\n".format(title))

        sourceLines = source.split("\n")
        if highlightOffset:
            cursor = 0           
            outputLines = []             

            for index, sourceLine in enumerate(sourceLines):
                cursor += len(sourceLine) + 1                                

                if cursor > highlightOffset:
                    offset = len(sourceLine) - (cursor - highlightOffset) + 1
                    if offset >= 0:
                        outputLines.append("  > | " + sourceLine)                        
                        outputLines.append("    | " + (" " * offset) + "^") 

                        if message:
                            outputLines.append("")
                            outputLines.append("      " + message) 
                            outputLines.append("")

                        highlightLine = index
                    else:
                        outputLines.append("    | " + sourceLine)                        
                else:
                    outputLines.append("    | " + sourceLine)     

            numLines = len(outputLines)
            if numLines > maxLines:
                cut = int(maxLines / 2)
                start = highlightLine - cut
                end = highlightLine + cut + 2
                if message:
                    # adjust for the error message
                    end = end + 3

                if start < 0:
                    start = 0
                if end > numLines:
                    end = numLines

                outputLines = outputLines[start:end]

                if start > 0:
                    outputLines.insert(0, "    | ...")
                if end < numLines:
                    outputLines.append("    | ...")                                             

            for line in outputLines:
                msg.error(line)

            msg.error("\n")            

        else:            
            if maxLines > 0:                
                for sourceLine in sourceLines[0:maxLines]:
                    msg.error("    | " + sourceLine)
                if len(sourceLines) > maxLines:
                    msg.error("    | ...")
            else:
                for sourceLine in sourceLines:
                    msg.error("    | " + sourceLine)

            msg.error("\n")

def get_rel_source_location_nav(test, testCase):

    tc_sourceLocation = testCase.get('sourceLocation')
    tc_sourceLineNumberInTestScript = testCase.get('sourceLineNumberInTestScript', 0)
    tc_sourceColumnNumberInTestScript = testCase.get('sourceColumnNumberInTestScript', 0)

    # format sourceLocation so that it's printed value can be easily navigated to              
    if tc_sourceLocation:                                  
        tc_sourceLocationCoordinates = str(tc_sourceLineNumberInTestScript) + ":" + str(tc_sourceColumnNumberInTestScript)        
        tc_sourceLocationNav = test + ':' + tc_sourceLocationCoordinates

        return tc_sourceLocationNav
    
def get_rel_error_source_location_nav(test, testCase):

    tc_sourceLineNumberInTestScript = testCase.get('sourceLineNumberInTestScript', 0)
    error = testCase.get('error', None)

    if error:
        lineNumber = error.get("parserLineNumber")
        columnNumber = error.get("parserColumnNumber")
        offset = error.get("offset")

        if not lineNumber or not columnNumber:
            if offset:
                source = testCase.get('source')
                lineNumber, columnNumber = get_source_location_coordinates(source, offset)

        if not lineNumber or not columnNumber:
            return None

        # format sourceLocation so that it's printed value can be easily navigated to              
        tc_sourceErrorLocationCoordinates = str(
            tc_sourceLineNumberInTestScript + lineNumber - 1
            ) + ":" + str(columnNumber)
        tc_sourceLocationNav = test + ':' + tc_sourceErrorLocationCoordinates

        return tc_sourceLocationNav

    return None


# This routine will print the result of a test that failed, including any error details that came back
def print_test_failure(test, testCase):

    sourceLocation = testCase.get("sourceLocation")
    sourceLocationNav = get_rel_source_location_nav(test, testCase)
    sourceErrorLocationNav = get_rel_error_source_location_nav(test, testCase)
    error = testCase.get("error")

    if error:
        message = error.get("message")
        if message:
            msg.error("    " + message + "\n")        

        errorSourceLocation = error.get("sourceLocation")    
        parserCursor = error.get("parserCursor")
        errorOffset = error.get("offset")        

        source = ""
        sourceLineOffset = 0

        if sourceLocation and errorSourceLocation:
            hasMatchingSourceLocations = compare_source_locations(sourceLocation, errorSourceLocation)            
        else:
            hasMatchingSourceLocations = False

        # if parserCursor was provided, we have a syntax error and use parserSource
        if parserCursor:
            source = error.get('parserSource')
            sourceLineOffset = parserCursor

        # otherwise use the test source, if the sourceLocation value matches the error sourceLocation
        elif hasMatchingSourceLocations is not None and errorOffset is not None:
            source = testCase.get('source')
            sourceLineOffset = errorOffset       

        if sourceLineOffset: 
            if sourceErrorLocationNav:   
                title="Test Failed at: \n\t{}\n\t└─ {}".format(sourceErrorLocationNav, sourceLocationNav)
            else:
                title="Test Failed at: {}".format(sourceLocationNav)
            
            format_source_code(source, highlightOffset=sourceLineOffset, title=title, message=message)

        else:
            # In this case, we couldn't determine the exact source location of the error
            # This may be due to the error occurring in a model or external script that
            # we can't access in this context.            
            if errorSourceLocation is not None and errorOffset is not None:
                msg.error("    Error occurred at source location: {}+{}\n".format(errorSourceLocation, errorOffset))   
                format_source_code(testCase.get("source"), "Test Failed at: {}".format(sourceLocationNav))
            elif errorSourceLocation is not None:
                msg.error("Test Failed at: {}".format(sourceLocationNav) + " (id=" + error.get("id") + ")")
                

    else:
        # if there was no error object, look for expect/result and dump the source
        if (testCase.get("expect") != None):            
            msg.error("    Expected: " + nfc.json_dumps(testCase.get('expect')))
            msg.error("    Result:   " + nfc.json_dumps(testCase.get("result")) + "\n")            

        format_source_code(testCase.get("source"), "Test Failed at {}".format(sourceLocationNav))

# This routine will print the results of a test run
def print_test_response(options, test, response, hasFailures, allSuccess, allSkipped):

    if response != None:     
        description = response.get('description')  
        if allSkipped:              
            msg.warn("  SKIP", end="")            
        elif hasFailures:
            msg.error("  FAIL", end="")            
        elif allSuccess:
            msg.success("  PASS", end="")        
        else:
            msg.warn("  PASS", end="")

        msg.highlighted_info(" {}".format(description))    

        if allSkipped:
            return        
            
        for testCase in response['tests']:
            
            tc_skip = testCase.get('skip')
            tc_passed = testCase.get('passed', False)                
            tc_description = testCase.get('description')
            tc_test = testCase.get('test')               

            if tc_skip == True:                                            
                if not options.get('errors'):
                    msg.warn("    \u25cb", end="")
                    msg.highlighted_info(" {}".format(tc_test))
                    if (msg.is_verbose_mode()):
                        print("\033[2m      {}\033[0m\n".format(tc_description))
                    msg.debug(nfc.json_dumps(testCase, sort_keys=True, indent=4))                  
            elif tc_passed == True:
                if not options.get('errors'):
                    msg.success("    \u2713", end="")
                    msg.highlighted_info(" {}".format(tc_test))
                    if (msg.is_verbose_mode()):
                        print("\033[2m      {}\033[0m\n".format(tc_description))
                    msg.debug(nfc.json_dumps(testCase, sort_keys=True, indent=4))                            
            else:
                # failed test
                msg.error("    \u2717", end="")
                msg.highlighted_info(" {}".format(tc_test))
                if (msg.is_verbose_mode()):
                    print("\033[2m      {}\033[0m\n".format(tc_description))                
                msg.debug(nfc.json_dumps(testCase, sort_keys=True, indent=4))
                
                # print test errors, if in verbose mode
                if msg.is_verbose_mode():
                    print_test_failure(test, testCase)


# This routine parses a test run and returns the appropriate counters summing 
# the number of tests, failures, and errors
def parse_test_run(test, options, response, error):

    allSkipped = False
    hasFailures = False
    allSuccess = True
    numFailures = 0
    numSkipped = 0
    numPassed = 0

    if response == None and error == None:
        # treat these tests as skipped
        allSkipped = True

    if response != None:
        # check if all tests were skipped
        allSkipped = response.get("skip", False)

        if not "tests" in response:
            msg.debug("Parsing test run, returned no tests: {}".format(test))            
            msg.debug(nfc.json_dumps(response, sort_keys=True, indent=4))

            pass

        for testCase in response.get("tests"):
            if allSkipped or testCase.get("skip") == True:
                allSuccess = False
                numSkipped += 1
            elif testCase.get("passed", False) == False:
                hasFailures = True
                allSuccess = False
                numFailures += 1
            else:
                numPassed += 1

    if error != None:
        hasFailures = True
        allSuccess = False
        numFailures += 1

    return hasFailures, allSuccess, numFailures, numSkipped, numPassed, allSkipped  
