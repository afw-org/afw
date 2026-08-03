#! /usr/bin/env python3

##
# @defgroup afwdev_test_modes modes
# @brief This module defines the different test modes.
# @details Tests can be run in one or more different "modes", which provides 
#          different environments settings. For example, running from the 
#          command-line, running under FastCGI, or running under Valgrind.
#
#          Running tests under different modes exercises both the code being 
#          tested, as well as the environment expected to execute the code.
#
#          In addition, modes provides alternate ways to run tests that may be 
#          more convenient for the test writer. Some tests are easier to write 
#          in Adaptive Script, while others are easier to write in Python.
# @ingroup afwdev_test
#

## 
# @file test.py
# @ingroup afwdev_test
# @brief This file contains the main entry point for the "test" subcommand.
# @details Selects matching srcdirs, then list, watch, javascript, or the
#          runner. Modes such as afw and valgrind live under test/modes/.
#

import os
import sys
import time
import fnmatch

from _afwdev.common import msg, nfc, package
from _afwdev.test import watch, runner, js
from _afwdev.test.common import (
    find_test_groups, load_test_group_config, test_group_matches_tags,
    print_failure_digest)


##
# @brief List matching tests without running them
#
def _list_tests(options, srcdirs):
    count = 0
    for srcdir, srcdirPath, _, manual_tests in srcdirs:
        if not os.path.exists(manual_tests):
            continue
        for testGroup in find_test_groups(options, srcdir, manual_tests):
            _, root, tests = testGroup
            testGroupConfig = load_test_group_config(root)
            if not test_group_matches_tags(options, testGroupConfig):
                continue
            for test in tests:
                msg.highlighted_info(os.path.relpath(test))
                count += 1
    msg.highlighted_info(str(count) + ' test(s) listed')
    sys.exit(0)


##
# @brief Optionally write a JSON results summary to --output
#
def _write_output_summary(options, summary):
    output = options.get('output') or 'stdout'
    if output == 'stdout':
        return
    with nfc.open(output, 'w') as fd:
        nfc.json_dump(summary, fd, indent=2, sort_keys=True)
    msg.highlighted_info('Wrote test summary to ' + output)


## 
# @brief The main entry point for the "test" subcommand
# @details This routine is the main entry point for the "test" subcommand. 
#          Depending on its arguments, it can run tests in a variety of ways, 
#          depending on the options:
#
#          "watch" will wait for file system changes and run the test that has 
#          changed.
#
#          "run" (default) will simply run all requested tests.
#
#          "list" will list matching tests and exit.
#
#          "javascript" will run javascript tests.
#
#          This routine also collects some stats from test runs to report to the 
#          user.
# @param options The options dictionary.
# 
def run(options):

    total_passed = 0
    total_failed = 0
    total_skipped = 0
    total_tests = 0
    
    total_srcdirs = 0
    srcdirs_passed = 0
    srcdirs_failed = 0
    srcdirs_skipped = 0   

    srcdirs = []
    for srcdir in package.get_afw_package(options)['srcdirs']:
        total_srcdirs += 1

        if not fnmatch.fnmatch(srcdir, options['srcdir_pattern']):
            srcdirs_skipped += 1
            srcdirs_passed += 1
            continue
        
        package.set_options_from_existing_package_srcdir(
            options, srcdir, set_all=True)   

        objects_dir = options['srcdir_path'] + 'generate/objects/'
        manual_tests = options['srcdir_path'] + 'tests'

        srcdirs.append(
            (
                srcdir, 
                options['srcdir_path'], 
                objects_dir, 
                manual_tests
            )
        )

    if options.get('list'):
        _list_tests(options, srcdirs)

    if options.get('javascript'):

        js.run(options, srcdirs)

    elif options.get('watch'):

        watch.run(options, srcdirs)

    else:

        start = time.time()
        results, failures = runner.run(options, srcdirs)
        end = time.time()

        # iterate over results dict and print results
        for srcdir, stats in results.items():            
            passed, skipped, failed = stats

            total_passed += passed
            total_skipped += skipped
            total_failed += failed
            total_tests += passed + skipped + failed

            if failed > 0:
                srcdirs_failed += 1

        srcdirs_passed = total_srcdirs - (srcdirs_failed + srcdirs_skipped)
        elapsed = round(end - start, 2)

        # Print summary
        msg.highlighted_info("")
        msg.highlighted_info("Source Dirs:   ", end="")
        if srcdirs_failed > 0:
            msg.error("{} failed".format(srcdirs_failed), end="")
            msg.highlighted_info(", ", end="")
        if srcdirs_skipped > 0:
            msg.warn("{} skipped".format(srcdirs_skipped), end="")
            msg.highlighted_info(", ", end="")
        if srcdirs_passed > 0:
            msg.success("{} passed".format(srcdirs_passed), end="")
            msg.highlighted_info(", ", end="")    

        msg.highlighted_info("{} total".format(total_srcdirs))

        msg.highlighted_info("Tests:         ", end="")
        if total_failed > 0:
            msg.error("{} failed".format(total_failed), end="")
            msg.highlighted_info(", ", end="")
        if total_skipped > 0:
            msg.warn("{} skipped".format(total_skipped), end="")
            msg.highlighted_info(", ", end="")
        if total_passed > 0:
            msg.success("{} passed".format(total_passed), end="")
            msg.highlighted_info(", ", end="")    
                
        msg.highlighted_info("{} total".format(total_tests))
        msg.highlighted_info("Time:          {}s".format(elapsed))

        # Console-only digest so parallel -j runs still end with greppable paths
        print_failure_digest(failures)

        _write_output_summary(options, {
            'srcdirs': {
                'passed': srcdirs_passed,
                'failed': srcdirs_failed,
                'skipped': srcdirs_skipped,
                'total': total_srcdirs,
            },
            'tests': {
                'passed': total_passed,
                'failed': total_failed,
                'skipped': total_skipped,
                'total': total_tests,
            },
            'time_seconds': elapsed,
            'by_srcdir': {
                srcdir: {
                    'passed': stats[0],
                    'skipped': stats[1],
                    'failed': stats[2],
                }
                for srcdir, stats in results.items()
            },
        })

        if total_failed > 0:
            sys.exit(1)
        else:
            sys.exit(0) 