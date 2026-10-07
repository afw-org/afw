#! /usr/bin/env python3

##
# @ingroup afwdev_test
#

##
# @file runner.py
# @brief This file contains the main functions for running tests.
# @details Runs discovered test groups sequentially or via multiprocessing.Pool.
#          Parallelism is per group. Each group restores cwd and os.environ
#          in a finally block. With -j, each worker buffers stdout/stderr and
#          the parent prints one group at a time so FAIL blocks do not
#          interleave.
#

import io
import os
import sys
import time
import multiprocessing
import resource
from functools import partial

from _afwdev.common import msg
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
    test_path_for_display, clip_detail, outcome_flag, errors_only_console, \
    xctx_bytes_from_response, xctx_chunk_bytes_from_response, \
    format_test_timing
from _afwdev.test.history import env_mode, file_record, fuzz_from_response
from _afwdev.test import failure_log
from _afwdev.test import run_dir
from _afwdev.test import family


def _children_cpu_ms():
    """User + system CPU of reaped children, in ms.

    Each test's processes (afw, afwfcgi, clients) are reaped before
    run_test returns, and a -j worker runs one test at a time, so the
    change around run_test is that test's CPU.
    """
    r = resource.getrusage(resource.RUSAGE_CHILDREN)
    return round((r.ru_utime + r.ru_stime) * 1000)


##
# @brief Build a short detail string for the failure digest
#
def _failure_detail(error, response, numFailures):
    if error is not None:
        return error_message(error) or str(error)
    if response is not None and response.get('tests'):
        bits = []
        for tc in response.get('tests') or []:
            if outcome_flag(tc.get('skip'), False):
                continue
            if not outcome_flag(tc.get('passed'), False):
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


def _with_buffered_stdio(fn):
    """Run fn() with stdout/stderr captured. Returns (result, text)."""
    buf = io.StringIO()
    old_out, old_err = sys.stdout, sys.stderr
    sys.stdout = sys.stderr = buf
    try:
        result = fn()
    finally:
        sys.stdout = old_out
        sys.stderr = old_err
    return result, buf.getvalue()


##
# @brief Run all tests that belong to a test group
# @return (testGroup, passed, skipped, failed, failures, captured)
#         captured is worker stdout/stderr when -j buffered, else "".
#
def run_test_group(testGroup, options, testEnvironments, work_dir_prefix):
    if options.get('_buffer_group_output'):
        result, captured = _with_buffered_stdio(
            lambda: _run_test_group_guarded(
                testGroup, options, testEnvironments, work_dir_prefix))
        return result + (captured,)
    result = _run_test_group_guarded(
        testGroup, options, testEnvironments, work_dir_prefix)
    return result + ("",)


def _run_test_group_guarded(testGroup, options, testEnvironments,
        work_dir_prefix):
    """A test or config.py that calls sys.exit() fails its group.

    Under -j the group runs in a multiprocessing.Pool worker. If the
    worker exited, the pool would lose the task and the run would wait
    forever for its result.
    """
    env = os.environ.copy()
    pwd = os.getcwd()
    try:
        return _run_test_group_body(
            testGroup, options, testEnvironments, work_dir_prefix)
    except SystemExit as e:
        os.chdir(pwd)
        os.environ.clear()
        os.environ.update(env)
        srcdir, root, _tests = testGroup
        detail = "test group called sys.exit({})".format(e.code)
        failure = {
            'test': test_path_for_display(root, pwd),
            'detail': detail,
            'srcdir': srcdir,
            'group': test_path_for_display(root, pwd),
        }
        failure_log.record(
            options, test_path_for_display(root, pwd), detail)
        msg.error("FAIL " + test_path_for_display(root, pwd) + ": " + detail)
        return testGroup, 0, 0, 1, [failure], 0, []


def _run_test_group_body(testGroup, options, testEnvironments, work_dir_prefix):

    failed = 0
    skipped = 0
    passed = 0
    failures = []
    max_xctx_bytes = 0
    file_records = []

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
        return testGroup, 0, 0, 0, [], 0, []

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
            }], 0, []
        msg.debug("Using test environment: " + testEnvironment['name'] + ', work_dir = ' + testEnvironment['work_dir'])        

    test_group_start = time.time()

    try:
        # switch to working directory for this environment
        if testEnvironment:            
            os.chdir(testEnvironment['work_dir'])

        before_all(root, testGroupConfig, testEnvironment)
            
        # Files in one group share one work directory and one conf.
        # -j runs groups in parallel. It must not run these files together.
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
            cpu_start = _children_cpu_ms()
            response, error, debug = run_test(test, options, testEnvironment, testGroupConfig)
            cpu_ms = _children_cpu_ms() - cpu_start
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
            xctx_bytes = xctx_bytes_from_response(response)
            xctx_chunk_bytes = xctx_chunk_bytes_from_response(response)
            if xctx_bytes is not None:
                max_xctx_bytes = max(max_xctx_bytes, xctx_bytes)
            record = file_record(
                test_display, duration_ms, xctx_bytes,
                numPassed, numSkipped, numFailures,
                xctx_chunk_bytes=xctx_chunk_bytes, cpu_ms=cpu_ms,
                fuzz=fuzz_from_response(response))
            file_records.append(record)
            marker = family.line_marker(record)

            # Quiet human chatter when summary is the sole stdout artifact
            quiet_console = (options.get('output') == '-')

            if not quiet_console and msg.is_debug_mode() and (debug or error):
                msg.highlighted_info("{}  {}".format(
                    test_display,
                    format_test_timing(
                        duration_ms, xctx_bytes, xctx_chunk_bytes))) 

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

            # Full valgrind XML stays under --debug (firehose). The fail
            # line already has kind + top frames from valgrind_error_message.
            if not quiet_console and msg.is_debug_mode() and debug:
                msg.debug(debug)

            after_each(root, testGroupConfig, testEnvironment)

            if hasFailures:
                detail_text = _failure_detail(error, response, numFailures)
                failures.append({
                    'test': test_display,
                    'detail': clip_detail(detail_text),
                    'srcdir': srcdir,
                    'group': test_path_for_display(root, pwd),
                })
                failure_log.record(
                    options,
                    name=test_display,
                    message=detail_text,
                    err=error,
                )

            # Default is errors-only; --show-all or a real --test-pattern
            # prints successful tests too.
            errors_only = errors_only_console(options)
            if errors_only and not hasFailures:
                # Out-of-family memory is shown even when passes are not.
                if marker and not quiet_console:
                    msg.warn("{}  {}{}".format(
                        test_display,
                        format_test_timing(
                            duration_ms, xctx_bytes, xctx_chunk_bytes),
                        marker))
                continue

            if quiet_console:
                continue

            # Path for assertion failures / --show-all (process errors already
            # printed identity above).
            if error is None:
                msg.highlighted_info("{}  {}{}".format(
                    test_display,
                    format_test_timing(
                        duration_ms, xctx_bytes, xctx_chunk_bytes),
                    marker))

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

    return testGroup, passed, skipped, failed, failures, max_xctx_bytes, file_records


##
# @brief This run's work directory (run_dir.py), made on first use.
# @param options The options dictionary
#
def allocate_working_directory(options):
    path = run_dir.current(options)
    if path:
        return path
    return run_dir.create(options, env_mode(options))


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
    max_xctx_bytes = 0
    all_file_records = []
    
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

        worker_options = dict(options)
        worker_options['_buffer_group_output'] = True
        
        pool = multiprocessing.Pool(
            processes=test_jobs, initializer=family.init_worker,
            initargs=family.worker_args())

        # run allTestGroups in parallel     
        results = []
        terminate = False
        
        pool_results = pool.imap_unordered(
            partial(run_test_group, 
                options=worker_options, 
                testEnvironments=testEnvironments, 
                work_dir_prefix=work_dir_prefix
            ), allTestGroups
        )
        pool.close()
        # Workers only exit at the end, so a new worker pid means one
        # died. Its task is lost and imap would wait forever.
        worker_pids = {p.pid for p in getattr(pool, "_pool", []) or []}

        try:
            while True:
                try:
                    res = pool_results.next(timeout=5)
                except StopIteration:
                    break
                except multiprocessing.TimeoutError:
                    now = {p.pid for p in getattr(pool, "_pool", []) or []}
                    if now - worker_pids:
                        raise AfwdevRunnerError(
                            "A test worker process died, so its test group "
                            "has no result. Run without -j to find it.")
                    continue
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
            # Aggressive shutdown: after Ctrl-C, workers can be stuck in
            # waitpid (e.g. Session("local").close) or already zombie; plain
            # pool.join() has been observed to hang indefinitely.
            try:
                pool.terminate()
            except Exception:
                pass
            try:
                for p in getattr(pool, "_pool", []) or []:
                    if p.is_alive():
                        try:
                            p.kill()
                        except Exception:
                            pass
            except Exception:
                pass
            try:
                pool.join()
            except Exception:
                pass
            sys.exit(1)

        pool.join()
        
        for testGroup, passed, skipped, failed, group_failures, group_xctx, group_files, captured in results:
            if captured:
                sys.stdout.write(captured)
                if not captured.endswith('\n'):
                    sys.stdout.write('\n')
                sys.stdout.flush()
            _srcdir = testGroup[0]

            if allTestResults.get(_srcdir):
                allTestResults[_srcdir][0] += passed
                allTestResults[_srcdir][1] += skipped
                allTestResults[_srcdir][2] += failed
            else:
                allTestResults[_srcdir] = [passed, skipped, failed]
            if group_failures:
                allFailures.extend(group_failures)
            if group_xctx:
                max_xctx_bytes = max(max_xctx_bytes, group_xctx)
            if group_files:
                all_file_records.extend(group_files)

    else:
        # run sequentially
        try:
            for testGroup in allTestGroups:
                _, passed, skipped, failed, group_failures, group_xctx, group_files, _captured = run_test_group(
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
                if group_xctx:
                    max_xctx_bytes = max(max_xctx_bytes, group_xctx)
                if group_files:
                    all_file_records.extend(group_files)

        except KeyboardInterrupt:
            msg.error("Caught KeyboardInterrupt, terminating test runner")
            sys.exit(1)

        except Exception as e:
            msg.error("Test runner caught Exception: " + str(e))
            sys.exit(1)

    return allTestResults, allFailures, max_xctx_bytes, all_file_records