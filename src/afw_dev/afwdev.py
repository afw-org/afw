#! /usr/bin/env python3

##
# @file afwdev.py
# @defgroup afwdev afwdev
# @ingroup afw_included_commands
# @brief Command afwdev is a development tool used to create, extend,
#        maintain, and test an Adaptive Framework (AFW) Package.
# @details An AFW package can be the one that holds the AFW core (afw) or other
#          AFW packages that contains AFW applications, commands, extensions,
#          layouts, servers, test data, etc.
#
# Implementation lives under _afwdev/cli/ (parser, metadata, handlers) and
# _afwdev/scaffold/ (make-* / add-*). Shared options keys are documented in
# _afwdev/cli/options.py. This module is the zipapp entry point (afwdev:main)
# and re-exports afwdev_info for tooling.
#

from _afwdev.cli.entry import main
from _afwdev.cli.info import afwdev_info

__all__ = ['main', 'afwdev_info']

if __name__ == "__main__":
    main()
