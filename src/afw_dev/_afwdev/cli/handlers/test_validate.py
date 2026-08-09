#! /usr/bin/env python3
##
# @file test_validate.py
# @ingroup afwdev_cli
# @brief Handlers for the test and validate subcommands.
#

import sys
from _afwdev.test import test
from _afwdev.validate import validate
from _afwdev import blast as blast_mod


##
# @defgroup afwdev_test test
# @ingroup afwdev
# @brief The afwdev test subcommand
# @details The afwdev test subcommand is used to test Adaptive Framework,
#          both manually, or from automated processes, such as GitHub Actions.
#          This includes tests for the C adaptive functions and Javascript
#          tests.
#
def subcommand_test(args, options):
    options['srcdir_pattern'] = options.get(
        'srcdir_pattern', '\\*').replace('\\', '')
    options['subcommand'] = "test"
    test.run(options)


##
# @defgroup afwdev_blast blast
# @ingroup afwdev
# @brief On-demand suite firehose at afwfcgi (experimental)
# @details Not part of test -j. Random Adaptive test_scripts for a duration
#          or request count; attach (--url) or managed spawn (--conf).
#
def subcommand_blast(args, options):
    options['srcdir_pattern'] = options.get(
        'srcdir_pattern', '\\*').replace('\\', '')
    options['subcommand'] = "blast"
    blast_mod.run(options)


##
# @defgroup afwdev_validate validate
# @ingroup afwdev
# @brief The afwdev validate subcommand
# @details The afwdev validate subcommand is used to validate JSON files.
#
def subcommand_validate(args, options):
    options['subcommand'] = "validate"
    exit_code = validate.run(options)
    sys.exit(exit_code)
