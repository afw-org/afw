#! /usr/bin/env python3
##
# @file common.py
# @ingroup afwdev_scaffold
#

from _afwdev.common import msg


def msg_added_files(options):
    msg.success('Review the following files, paying close attention to @todo comments:')
    added = options['added_files']
    added.sort()
    for file in added:
        msg.success('  ' + file)
