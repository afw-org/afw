#!/usr/bin/env python3

##
# @file msg.py
# @ingroup afwdev_common
# @brief This file contains some common functions for message logging.
#

import os
import sys

global show_debug
global show_highlight_info
global show_info
global show_success
global is_debug
global is_quiet
global is_verbose

show_debug = False
show_highlight_info = True
show_info = True
show_success = True
is_debug = False
is_quiet = False
is_verbose = False

def is_debug_mode():
    global is_debug
    return is_debug

def is_quiet_mode():
    global is_quiet
    return is_quiet

def is_verbose_mode():
    global is_verbose
    return is_verbose

def set_verbosity(options):
    global show_debug
    global show_highlight_info
    global show_info
    global show_success
    global is_debug
    global is_quiet
    global is_verbose

    show_debug = False
    show_highlight_info = False
    show_info = False
    show_success = False

    # If quiet, set and return
    if options.get('quiet', False):
        is_quiet = True
        return

    # If not quiet, show highlighted info and success msgs also
    show_highlight_info = True
    show_success = True

    # If verbose, also show nonhighlighted info msgs also
    if options.get('verbose', False):
        is_verbose = True
        show_info = True
        return

    # If debug, also show nonhighlighted and debug msgs also
    if options.get('debug', False):
        is_debug = True
        is_verbose = True
        show_info = True
        show_debug = True
        return

    # If normal, show what's already set
    return


def debug(message, empty_before=False, empty_after=False, end='\n'):
    global show_debug
    if show_debug:
        if empty_before:
            print('', end=end)
        if os.name == 'posix':
            print('\033[94m' + message + '\033[0m', end=end)
        else:
            print(message)
        if empty_after:
            print('', end=end)

def error(message, empty_before=False, empty_after=False, end='\n'):
    if empty_before:
        print('', end=end)
    if os.name == 'posix':
        print('\033[91m' + message + '\033[0m', end=end)
    else:
        print('! ' + message)
    if empty_after:
        print('', end=end)


def error_exit(message, empty_before=False, empty_after=False):
    error(message, empty_before, empty_after)
    sys.exit(1)


def highlighted_info(message, empty_before=False, empty_after=False, end='\n'):
    global show_highlight_info
    if show_highlight_info:
        if empty_before:
            print('', end=end)
        if os.name == 'posix':
            print('\033[1m' + message + '\033[0m', end=end)
        else:
            print('> ' + message, end=end)
        if empty_after:
            print('', end=end)


def info(message, empty_before=False, empty_after=False, end='\n'):
    global show_info
    if show_info:
        if empty_before:
            print('', end=end)
        if os.name == 'posix':
            print('\033[94m' + message + '\033[0m', end=end)
        else:
            print(message, end=end)
        if empty_after:
            print('', end=end)


def success(message, empty_before=False, empty_after=False, end='\n'):
    global show_success
    if show_success:
        if empty_before:
            print('', end=end)
        if os.name == 'posix':
            print('\033[92m' + message + '\033[0m', end=end)
        else:
            print('. ' + message, end=end)
        if empty_after:
            print('', end=end)


def warn(message, empty_before=False, empty_after=False, end='\n'):
    if empty_before:
        print('', end=end)
    if os.name == 'posix':
        print('\033[93m' + message + '\033[0m', end=end)
    else:
        print('? ' + message, end=end)
    if empty_after:
        print('', end=end)

