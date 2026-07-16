#! /usr/bin/env python3
##
# @file version.py
# @ingroup afwdev_cli
#

from _afwdev.common import resources


def get_version(options):
    opt = options.copy()
    info = resources.get_afwdev_git_info(opt)
    return info.get('version_with_git_info')


def get_version_string(options):
    opt = options.copy()
    info = resources.get_afwdev_git_info(opt)
    return info.get('version_string')
