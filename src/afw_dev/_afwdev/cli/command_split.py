#! /usr/bin/env python3
##
# @file command_split.py
# @ingroup afwdev_cli
# @brief Split a shell-like command string on && into argv lists.
#

import shlex


def split_commands(string):
    """Return list of argv lists, splitting on standalone && tokens."""
    commands = []
    command = []
    commands.append(command)
    for token in shlex.split(string):
        if token == '&&':
            command = []
            commands.append(command)
        else:
            command.append(token)
    return commands
