#! /usr/bin/env python3
##
# @file prime.py
# @ingroup afwdev_scaffold
# @brief Handlers for prime-* subcommands (closet files, no generate).
#

from _afwdev.scaffold import c_probe as c_probe_scaffold


##
# @brief Subcommand prime-test-c-probe
#
def subcommand_prime_test_c_probe(args, options):
    c_probe_scaffold.prime_test_c_probe(options)
