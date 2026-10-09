#!/usr/bin/env python3

##
# @file tsan.py
# @ingroup afwdev_test_modes
# @brief This file defines the run method for running tests under the
#        "tsan" test mode.
# @details Adaptive Scripts (.as) run under the afw command line tool from
#          the ThreadSanitizer build (build/tsan/cmake, made by
#          `afwdev build --cdev --sanitize thread`). test.py has already
#          put that tree first on PATH, set TSAN_OPTIONS / UBSAN_OPTIONS
#          and capped the stack limit for the whole run
#          (_afwdev.test.sanitize), so python tests, C probes and
#          orchestrated afwfcgi use the same build. Same run as the asan
#          mode: the afw mode, with a sanitizer report on stderr turned
#          into a short failure. Races need more than one thread, so the
#          reports come from multi-threaded afwfcgi tests.
#

from _afwdev.test.modes.asan import run_test  # noqa: F401
