#! /usr/bin/env python3

import os
import sys
import time
import multiprocessing
import random
from functools import partial

from _afwdev.common import msg, nfc
from _afwdev.test.common import \
    get_test_environment, parse_test_run, print_test_response, find_test_groups, \
    load_test_environments, load_test_group_config, run_test, before_all, \
    before_each, after_all, after_each


# This routine runs all tests that belong to a test group
def run_test_group(testGroup, options, testEnvironments, test_working_directory):

    failed = 0
    skipped = 0
    passed = 0    

    _, root, tests = testGroup       

    testGroupConfig = load_test_group_config(root)
    if testGroupConfig:        
        msg.debug("Loaded test group configuration from " + root)

    # if tags were specified, make sure this test group matches
    if options.get("tags"):
        if not any(tag in testGroupConfig.get("Tags", []) for tag in options.get("tags")):
            msg.debug("  Skipping test group because it doesn't match the specified tags")

    # remember the current working directory
    pwd = os.getcwd()

    # get the test environment for this test group
    testEnvironment = get_test_environment(root, testEnvironments, testGroupConfig, test_working_directory)    
    if testEnvironment:
        if not testEnvironment.get('cwd'):
            msg.error("Test environment '" + testEnvironment['name'] + "' has no 'cwd' set")            
            return None
        msg.debug("Using test environment: " + testEnvironment['name'] + ', cwd = ' + testEnvironment['cwd'])        

    try:
        # switch to working directory for this environment
        if testEnvironment:            
            os.chdir(testEnvironment['cwd'])

        before_all(root, testGroupConfig, testEnvironment)
            
        for test in tests:
            before_each(root, testGroupConfig, testEnvironment)
            
            # run_test()
            #
            # Runs the test and parse the output by invoking the underlying 
            # mode's run_test() routine. It passes along the test, options,
            # testEnvironment, and testGroupConfig objects. It expects the mode
            # to return a tuple containing the following:
            #
            #   response: the response from the test run (_AdaptiveTestScriptResult_)
            #   error: any error message or exception from the test run
            #   debug: any debug output from the test run that should be displayed
            #          to the user, under debug mode to help understand a problem.
            start = time.time()
            response, error, debug = run_test(test, options, testEnvironment, testGroupConfig)
            end = time.time()                        

            # parse the test run results
            test_run = parse_test_run(test, options, response, error)                                    
            hasFailures, allSuccess, \
                numFailures, numSkipped, numPassed, allSkipped = test_run                

            failed += numFailures
            passed += numPassed
            skipped += numSkipped            

            if error != None:                      
                msg.highlighted_info("{}  ({}ms)".format(os.path.relpath(test, pwd), round((end - start) * 1000)))  
                if error:
                    # The error could be from a Python exception, or an afw error object
                    # The best solution will be to make separate Exception classes for
                    # each type of error, so we can handle them differently.
                    str = error
                    try:
                        str = nfc.json_loads(error).get('message')
                    except:
                        str = error
                    msg.error("\n    \u2717 {}\n".format(str))                   
                if debug:
                    # replace newlines with newlines and indentation
                    debug = debug.replace('\n', '\n      ')
                    msg.error("      {}\n".format(debug))

            after_each(root, testGroupConfig, testEnvironment)

            # if we only report errors, then skip successful tests
            if options.get('errors') and not hasFailures:
                continue

            # already reported test name above
            if error == None:
                msg.highlighted_info("{}  ({}ms)".format(os.path.relpath(test, pwd), round((end - start) * 1000)))

                if debug:
                    msg.debug('---\n' + debug + '\n---\n')

            print_test_response(options, test, response, hasFailures, allSuccess, allSkipped)

            bail = options.get('bail', 0)
            if bail > 0 and failed >= bail:
                msg.highlighted_info("")          

                # still make sure to cleanup by running after_all
                after_all(root, testGroupConfig, testEnvironment)  

                raise Exception("Bailing due to test failure")
                    
            msg.highlighted_info("")

        after_all(root, testGroupConfig, testEnvironment)
    
    finally:
        # always switch back to original working directory        
        os.chdir(pwd)

    return testGroup, passed, skipped, failed


# Creates a known, temporary folder to persist test output
def allocate_working_directory():

    working_directory = "/tmp/afwdev_test_output"

    # if folder already exists, remove it first
    if os.path.exists(working_directory):
        msg.highlighted_info("Removing previous working directory: " + working_directory)        
        os.system("rm -rf " + working_directory)
    
    # create folder
    os.mkdir(working_directory)

    # create a timestamp file
    timestamp = time.strftime("%Y-%m-%d_%H-%M-%S")
    with nfc.open(working_directory + "/timestamp", 'w') as f:
        f.write(timestamp)

    return working_directory


# This is the main entry point for the test runner
#
# This routine will find all test groups and run them sequentially, or in 
# parallel depending on the options.
def run(options, srcdirs):

    allTestGroups = []
    allTestResults = {}
    testEnvironments = []
    
    # always include the python client for bindings in the system path
    if os.path.exists('src/afw_client/python'):
        # append full path to sys.path, so python test modules can import directly
        sys.path.append(os.path.abspath('src/afw_client/python'))    

    # and the afw generated bindings
    if os.path.exists('src/afw/generated/python_bindings'):
        # append full path to sys.path, so python test modules can import directly
        sys.path.append(os.path.abspath('src/afw/generated'))    

    test_working_directory = allocate_working_directory()

    # for each srcdir find all tests    
    for srcdir, srcdirPath, _, generated_tests, manual_tests in srcdirs:
        
        srcDirTestGroups = []
        if os.path.exists(generated_tests):
            testGroups = find_test_groups(srcdir, generated_tests)
            srcDirTestGroups += testGroups

        if os.path.exists(manual_tests):
            testGroups = find_test_groups(srcdir, manual_tests)
            srcDirTestGroups += testGroups

        # if this source directory has python bindings, include them for tests
        if os.path.exists(srcdirPath + '/generated/python_bindings'):
            # append full path to sys.path, so python test modules can import directly
            sys.path.append(os.path.abspath(srcdirPath + '/generated'))                

        allTestGroups += srcDirTestGroups       

        # Load any test environments
        testEnvironments += load_test_environments(srcdirPath)   
                
    # determine if we're running these in parallel jobs
    test_jobs = options.get('test_jobs')
    if test_jobs == None and options['afwdev_settings'].get('test_jobs_argument'):
        # parse "n" from "--jobs n" if n is present
        if options['afwdev_settings']['test_jobs_argument'].startswith('--jobs '):
            test_jobs = int(options['afwdev_settings']['test_jobs_argument'].split('--jobs ')[1])
        else:
            test_jobs = 0

    if test_jobs != None:
        # run in parallel
        test_jobs = test_jobs if test_jobs > 0 else None

        msg.highlighted_info("Running {} test groups in parallel with {} processes".format(len(allTestGroups), test_jobs if test_jobs else os.cpu_count()))
        
        pool = multiprocessing.Pool(processes=test_jobs)                   

        # run allTestGroups in parallel     
        results = []
        terminate = False
        
        pool_results = pool.imap_unordered(
            partial(run_test_group, 
                options=options, 
                testEnvironments=testEnvironments, 
                test_working_directory=test_working_directory
            ), allTestGroups
        )
        pool.close()

        try:
            for res in pool_results:
                if not res:
                    raise Exception("Test group returned no results")
                else:
                    # append to results
                    results.append(res)
                    
        except KeyboardInterrupt:
            msg.error("Caught KeyboardInterrupt, terminating test runner")
            terminate = True

        except Exception as e:
            msg.error("Test runner caught Exception: " + str(e))
            terminate = True

        if terminate:
            pool.terminate()
            pool.join()

            sys.exit(1)

        pool.join()
        
        for testGroup, passed, skipped, failed in results:
            _srcdir = testGroup[0]

            if allTestResults.get(_srcdir):
                allTestResults[_srcdir][0] += passed
                allTestResults[_srcdir][1] += skipped
                allTestResults[_srcdir][2] += failed
            else:
                allTestResults[_srcdir] = [passed, skipped, failed]             

    else:
        # run sequentially
        try:
            for testGroup in allTestGroups:
                _, passed, skipped, failed = run_test_group(
                    testGroup, 
                    options, 
                    testEnvironments, 
                    test_working_directory
                )
                _srcdir = testGroup[0]

                if allTestResults.get(_srcdir):
                    allTestResults[_srcdir][0] += passed
                    allTestResults[_srcdir][1] += skipped
                    allTestResults[_srcdir][2] += failed
                else:
                    allTestResults[_srcdir] = [passed, skipped, failed]

        except KeyboardInterrupt:
            msg.error("Caught KeyboardInterrupt, terminating test runner")
            sys.exit(1)

        except Exception as e:
            msg.error("Test runner caught Exception: " + str(e))
            sys.exit(1)

    return allTestResults