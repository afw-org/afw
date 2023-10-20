#! /usr/bin/env python3

## 
# @file test.py
# @ingroup afwdev_test
# @brief This file contains the main entry point for the "test" subcommand.
#

import sys
import time
import fnmatch

from _afwdev.common import msg, package
from _afwdev.test import watch, runner, js

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

    if options.get('javascript'):

        js.run(options, srcdirs)

    elif options.get('watch'):

        watch.run(options, srcdirs)

    else:

        start = time.time()
        results = runner.run(options, srcdirs)
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
        msg.highlighted_info("Time:          {}s".format(
            round(end - start, 2)))    

        if total_failed > 0:
            sys.exit(1)
        else:
            sys.exit(0)   