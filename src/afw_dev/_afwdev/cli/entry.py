#! /usr/bin/env python3
##
# @file entry.py
# @ingroup afwdev_cli
# @brief main() for the afwdev command.
#

import os
import sys
import zipfile
from _afwdev.common import msg, nfc
from _afwdev.cli.parser import setup_parser, set_package_and_args_options


def main():
    options = {}

    MIN_PYTHON = (3, 6)
    if sys.version_info < MIN_PYTHON:
        print('afwdev requires Python %s.%s or newer.' % MIN_PYTHON)
        sys.exit(1)

    options['afwdev_path'] = os.path.realpath(sys.argv[0])
    options['afwdev_dir_path'] = os.path.dirname(options['afwdev_path']) + '/'
    options['afwdev_is_zipped'] = zipfile.is_zipfile(options['afwdev_path'])
    if (not options['afwdev_is_zipped'] and
            not os.path.exists(options['afwdev_dir_path'] + '_resources')):
        print(options['afwdev_path'])
        print('afwdev can not find its resources')
        sys.exit(1)

    parser = setup_parser(options)
    args = parser.parse_args()

    if 'func' not in args:
        msg.error_exit("subcommand required")

    options['verbose'] = args.verbose
    options['quiet'] = args.quiet
    options['debug'] = args.debug
    msg.set_verbosity(options)

    set_package_and_args_options(args, options)

    if msg.is_debug_mode():
        msg.debug("\nOptions before calling process function:")
        msg.debug(nfc.json_dumps(options, sort_keys=True, indent=4))

    args.func(args, options)
