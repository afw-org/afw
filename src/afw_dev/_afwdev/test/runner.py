#! /usr/bin/env python3

##
# @ingroup afwdev_test
#

##
# @file runner.py
# @brief This file contains the main functions for running tests.
# @details Runs discovered test groups sequentially or via multiprocessing.Pool.
#          Parallelism is per group. Each group restores cwd and os.environ
#          in a finally block.
#

import os
import shutil
import sys
import time
import multiprocessing
from functools import partial

from _afwdev.common import msg, nfc
from _afwdev.common.errors import (
    AfwdevError,
    AfwdevRunnerError,
    error_message,
    error_to_dict,
)
from _afwdev.test.common import \
    get_test_environment, parse_test_run, print_test_response, find_test_groups, \
    load_test_environments, load_test_group_config, run_test, before_all, \
    before_each, after_all, after_each, test_group_matches_tags, \
    test_path_for_display, clip_detail


##
# @brief Build a short detail string for the failure digest
#
def _failure_detail(error, response, numFailures):
    if error is not None:
        return error_message(error) or str(error)
    if response is not None and response.get('tests'):
        bits = []
        for tc in response.get('tests') or []:
            if tc.get('skip'):
                continue
            if tc.get('passed', False) is False:
                err = tc.get('error')
                msg_line = error_message(err)
                if msg_line:
                    bits.append(msg_line)
                elif tc.get('description') and tc.get('description') != tc.get(
                        'test'):
                    bits.append(tc.get('description'))
                else:
                    bits.append(tc.get('test') or '?')
        if bits:
            shown = bits[:3]
            extra = len(bits) - len(shown)
            text = "; ".join(shown)
            if extra > 0:
                text += "; +{} more".format(extra)
            return text
    if response is not None:
        top = response.get('error')
        msg_line = error_message(top)
        if msg_line:
            return msg_line
    if numFailures:
        return "{} failed".format(numFailures)
    return "failed"


##
# @brief This routine runs all tests that belong to a test group
# @param testGroup The test group to run
# @param options The options dictionary
# @param testEnvironments The list of test environments
# @param work_dir_prefix The prefix for the working directory
# @return (testGroup, passed, skipped, failed, failures) where failures is a
#         list of dicts for the end-of-run digest (empty when none failed)
#
def run_test_group(testGroup, options, testEnvironments, work_dir_prefix):

    failed = 0
    skipped = 0
    passed = 0
    failures = []

    # save the current environment variables
    prevEnvVars = os.environ.copy()

    # remember the current working directory
    pwd = os.getcwd()

    srcdir, root, tests = testGroup

    testGroupConfig = load_test_group_config(root)
    if testGroupConfig:        
        msg.debug("Loaded test group configuration from " + root)

        # check to see if any environment variables need to be set
        if testGroupConfig.get('EnvVars'):
            for key, value in testGroupConfig.get('EnvVars').items():
                os.environ[key] = value

    # if --tags was specified (non-default), skip groups that do not match
    if not test_group_matches_tags(options, testGroupConfig):
        msg.debug("  Skipping test group because it doesn't match the specified tags")
        return testGroup, 0, 0, 0, []

    # get the test environment for this test group
    testEnvironment = get_test_environment(testGroup, testEnvironments, testGroupConfig, work_dir_prefix)    
    if testEnvironment:
        if not testEnvironment.get('work_dir'):
            msg.error("Test environment '" + testEnvironment['name'] + "' has no 'work_dir' set")            
            return testGroup, 0, 0, 1, [{
                'test': test_path_for_display(root, pwd),
                'detail': "environment '{}' has no work_dir".format(
                    testEnvironment.get('name')),
                'srcdir': srcdir,
            }]
        msg.debug("Using test environment: " + testEnvironment['name'] + ', work_dir = ' + testEnvironment['work_dir'])        

    test_group_start = time.time()

    try:
        # switch to working directory for this environment
        if testEnvironment:            
            os.chdir(testEnvironment['work_dir'])

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

            test_display = test_path_for_display(test, pwd)
            duration_ms = round((end - start) * 1000)

            # Quiet human chatter when summary is the sole stdout artifact
            quiet_console = (options.get('output') == '-')

            if not quiet_console and msg.is_debug_mode() and (debug or error):
                msg.highlighted_info("{}  ({}ms)".format(test_display, duration_ms)) 

            if error is not None and not quiet_console:
                # Process death / runner exception: always show path + message.
                # (Assertion failures use print_test_response below.)
                err_str = error_message(error) or str(error)
                msg.error("\n    \u2717 {}\n".format(err_str))
                msg.error("      test:  {}\n".format(test_display))
                msg.error("      group: {}\n".format(
                    test_path_for_display(root, pwd)))
                if testEnvironment and testEnvironment.get('work_dir'):
                    msg.error("      cwd:   {}\n".format(
                        testEnvironment['work_dir']))

            if not quiet_console and msg.is_debug_mode() and debug:
                msg.debug(debug)

            after_each(root, testGroupConfig, testEnvironment)

            if hasFailures:
                failures.append({
                    'test': test_display,
                    'detail': clip_detail(
                        _failure_detail(error, response, numFailures)),
                    'srcdir': srcdir,
                    'group': test_path_for_display(root, pwd),
                })

            # Default is errors-only; --show-all prints successful tests too
            errors_only = options.get('errors', True) and not options.get('show_all')
            if errors_only and not hasFailures:
                continue

            if quiet_console:
                continue

            # Path for assertion failures / --show-all (process errors already
            # printed identity above).
            if error is None:
                msg.highlighted_info("{}  ({}ms)".format(test_display, duration_ms))

                if debug:
                    msg.debug('---\n' + debug + '\n---\n')

            print_test_response(options, test, response, hasFailures, allSuccess, allSkipped)

            bail = options.get('bail', 0)            
            if bail > 0 and failed >= bail:
                msg.highlighted_info("")          

                # still make sure to cleanup by running after_all
                after_all(root, testGroupConfig, testEnvironment)  

                raise AfwdevRunnerError("Bailing due to test failure")
                    
            msg.highlighted_info("")

        after_all(root, testGroupConfig, testEnvironment)
    
    finally:
        # always switch back to original working directory        
        os.chdir(pwd)

        # and restore environment variables
        os.environ.clear()
        os.environ.update(prevEnvVars)

    test_group_end = time.time()

    if msg.is_debug_mode():
        msg.highlighted_info("Test group {} took {}ms".format(root, round((test_group_end - test_group_start) * 1000)))

    return testGroup, passed, skipped, failed, failures


##
# @brief Creates a known, temporary folder to persist test output
# @param options The options dictionary
#
def allocate_working_directory(options):

    # use a temporary directory for test output
    tmpdir = options.get('tmpdir')
    working_directory = tmpdir + "/afwdev_test_output"

    # if folder already exists, remove it first
    if os.path.exists(working_directory):
        if options.get('output') != '-':
            msg.highlighted_info(
                "Removing previous working directory: " + working_directory)
        shutil.rmtree(working_directory)
    
    # create folder
    os.mkdir(working_directory)

    # create a timestamp file
    timestamp = time.strftime("%Y-%m-%d_%H-%M-%S")
    with nfc.open(working_directory + "/timestamp", 'w') as f:
        f.write(timestamp)

    return working_directory


##
# @brief This is the main entry point for the test runner
# @details This routine will find all test groups and run them sequentially, or 
#          in  parallel depending on the options.
#
def run(options, srcdirs):

    allTestGroups = []
    allTestResults = {}
    allFailures = []
    testEnvironments = []
    
    # always include the python client for bindings in the system path
    if os.path.exists('src/afw_client/python'):
        # append full path to sys.path, so python test modules can import directly
        sys.path.append(os.path.abspath('src/afw_client/python'))    

    # and the afw generated bindings
    if os.path.exists('src/afw/generated/python_bindings'):
        # append full path to sys.path, so python test modules can import directly
        sys.path.append(os.path.abspath('src/afw/generated'))    

    work_dir_prefix = allocate_working_directory(options)

    # for each srcdir find all tests    
    for srcdir, srcdirPath, _, manual_tests in srcdirs:
        
        srcDirTestGroups = []

        if os.path.exists(manual_tests):
            testGroups = find_test_groups(options, srcdir, manual_tests)
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
                work_dir_prefix=work_dir_prefix
            ), allTestGroups
        )
        pool.close()

        try:
            for res in pool_results:
                if not res:
                    raise AfwdevRunnerError("Test group returned no results")
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
        
        for testGroup, passed, skipped, failed, group_failures in results:
            _srcdir = testGroup[0]

            if allTestResults.get(_srcdir):
                allTestResults[_srcdir][0] += passed
                allTestResults[_srcdir][1] += skipped
                allTestResults[_srcdir][2] += failed
            else:
                allTestResults[_srcdir] = [passed, skipped, failed]
            if group_failures:
                allFailures.extend(group_failures)

    else:
        # run sequentially
        try:
            for testGroup in allTestGroups:
                _, passed, skipped, failed, group_failures = run_test_group(
                    testGroup, 
                    options, 
                    testEnvironments, 
                    work_dir_prefix
                )
                _srcdir = testGroup[0]

                if allTestResults.get(_srcdir):
                    allTestResults[_srcdir][0] += passed
                    allTestResults[_srcdir][1] += skipped
                    allTestResults[_srcdir][2] += failed
                else:
                    allTestResults[_srcdir] = [passed, skipped, failed]
                if group_failures:
                    allFailures.extend(group_failures)

        except KeyboardInterrupt:
            msg.error("Caught KeyboardInterrupt, terminating test runner")
            sys.exit(1)

        except Exception as e:
            msg.error("Test runner caught Exception: " + str(e))
            sys.exit(1)

    return allTestResults, allFailures