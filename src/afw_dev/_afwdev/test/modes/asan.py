#!/usr/bin/env python3

##
# @file asan.py
# @ingroup afwdev_test_modes
# @brief This file defines the run method for running tests under the
#        "asan" test mode.
# @details Adaptive Scripts (.as) run under the afw command line tool from
#          the AddressSanitizer build (build/asan/cmake, made by
#          `afwdev build --cdev --sanitize address`). test.py has already
#          put that tree first on PATH and set ASAN_OPTIONS /
#          UBSAN_OPTIONS for the whole run (_afwdev.test.sanitize), so
#          python tests, C probes and orchestrated afwfcgi use the same
#          build. This mode runs the afw mode and turns a sanitizer report
#          on stderr into a short failure.
#

from _afwdev.common.errors import AfwdevProcessError
from _afwdev.test.modes import afw as afw_mode
from _afwdev.test.sanitize import sanitizer_report_summary


##
# @brief Runs the test under the ASan build of afw.
# @param test The test to run.
# @param options The options dictionary.
# @param testEnvironment The test environment.
# @param testGroupConfig The test group configuration.
#
def run_test(test, options, testEnvironment=None, testGroupConfig=None):
    response, error, debug = afw_mode.run_test(
        test, options, testEnvironment, testGroupConfig)
    summary = sanitizer_report_summary(debug)
    if summary:
        error = AfwdevProcessError('Sanitizer report:\n' + summary)
    return response, error, debug
