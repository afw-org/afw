#! /usr/bin/env python3
##
# @file misc.py
# @ingroup afwdev_cli
# @brief Handlers for build, ebnf, for, task, settings, and afwdev-parser-info.
#

import subprocess
from _afwdev.build import build, ebnf_diagrams
from _afwdev.common import msg, nfc
from _afwdev.cli.command_split import split_commands
from _afwdev.cli.info import afwdev_info


##
# @brief Subcommand afwdev-parser-info
#
def subcommand_afwdev_parser_info(args, options):
    options['subcommand'] = "afw-parser-info"
    with nfc.open(options['output_path'], 'w') as fd:
        nfc.json_dump(afwdev_info, fd, indent=4, sort_keys=True)
    msg.success('Parser info written to:  ' + options['output_path'])


##
# @brief Subcommand build
#
def subcommand_build(args, options):
    options['subcommand'] = "build"
    build.run(options)
    msg.success('Build successful')


##
# @brief Subcommand ebnf
#
def subcommand_ebnf(args, options):
    options['subcommand'] = "ebnf"
    ebnf_diagrams.build(options)
    msg.success("Generate EBNF syntax diagrams successful")


##
# @brief Subcommand for
#
def subcommand_for(args, options):
    options['subcommand'] = "for"
    errors = 0

    forSet = options['afwdev_settings']['forSets']
    if not forSet.get(options['set_name']):
        msg.error_exit("Can't locate set " + options['set_name'])
    paths = forSet.get(options['set_name'])

    commands = split_commands(options['command'])

    for path in paths:
        msg.highlighted_info('Entering ' + path + ':')
        cwd = path
        failed = False
        for command in commands:
            try:
                rc = subprocess.run(command, cwd=cwd)
                if rc.returncode != 0:
                    failed = True
            except Exception as e:
                msg.error('Command failed in ' + cwd + ': ' + str(e))
                failed = True
            if failed:
                errors += 1
                if not options.get('ignore_errors', False):
                    msg.error_exit(
                        'Error occurred while processing command in ' + cwd)
                else:
                    msg.warn(
                        'Ignored an error that occurred while processing '
                        'command in ' + cwd)
            msg.highlighted_info('')

    if errors != 0:
        if errors == 1:
            msg.warn('Ignored 1 error')
        else:
            msg.warn('Ignored ' + str(errors) + ' errors')


##
# @brief Subcommand settings
#
def subcommand_settings(args, options):
    options['subcommand'] = "settings"
    print(nfc.json_dumps(options['afwdev_settings'], indent=2, sort_keys=True))


##
# @brief Subcommand task
#
def subcommand_task(args, options):
    options['subcommand'] = "task"

    string = options['afwdev_settings'].get('tasks', {}).get(options['task_name'])
    if not string:
        msg.error_exit("Task " + options['task_name'] + " doesn't exist")

    commands = split_commands(string)

    failed = False
    try:
        for command in commands:
            msg.info('Running ' + ' '.join(command))
            rc = subprocess.run(command)
            if rc.returncode != 0:
                failed = True
                break
    except Exception as e:
        msg.error('Task ' + options['task_name'] + ' raised: ' + str(e))
        failed = True

    if failed:
        msg.error_exit("Task " + options['task_name'] + " failed")
