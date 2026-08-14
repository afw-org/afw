#! /usr/bin/env python3
##
# @file parser.py
# @ingroup afwdev_cli
# @brief Build the afwdev argparse parser and populate the options dict.
# @details Subcommand and option metadata come from cli.info. After parse_args,
#          set_package_and_args_options() resolves the package directory,
#          loads afwdev settings, and copies each arg's optionName into
#          options. Dispatch uses registry.SUBCOMMAND_HANDLERS. See options.py
#          for the shared options keys.
#

import argparse
from _afwdev.common import msg, package
from _afwdev.cli.info import (
    _afwdev_overall_info,
    _info_debug,
    _info_quiet,
    _info_verbose,
    _info_version,
    _info_version_string,
    _info_noprompt,
    _info_prompt,
    _subcommand_infos,
    afwdev_info,
)
from _afwdev.cli.registry import SUBCOMMAND_HANDLERS, SUBCOMMAND_INFO_BY_NAME
from _afwdev.cli.settings import get_afwdev_settings
from _afwdev.cli.version import get_version, get_version_string


def add_args(subparsers, info):
    parser = subparsers.add_parser(
        info['subcommand'],
        help=info['help'],
        description=info['description'].format(**info))

    for arg in info['args']:
        if arg.get('arg') is None:
            continue

        if arg.get('short'):
            args = [arg.get('short'), arg.get('arg')]
        else:
            args = [arg.get('arg')]

        help = arg.get('help').format(**info)
        if arg.get('default') is not None:
            help += (
                ' The default is "' +
                str(arg.get('default')).replace('{thing}', info['thing']) +
                '".')

        if arg.get('const'):
            if arg.get('int', False):
                parser.add_argument(
                    *args,
                    type=int,
                    nargs=arg.get('nargs'),
                    action=arg.get('action'),
                    default=arg.get('default'),
                    const=arg.get('const', None),
                    help=help)
            else:
                parser.add_argument(
                    *args,
                    nargs=arg.get('nargs'),
                    action=arg.get('action'),
                    default=arg.get('default'),
                    const=arg.get('const', None),
                    help=help)

        elif arg.get('action'):
            parser.add_argument(
                *args,
                action=arg.get('action'),
                default=arg.get('default'),
                help=help)

        else:
            parser.add_argument(
                *args,
                nargs=arg.get('nargs'),
                help=help)

    handler = SUBCOMMAND_HANDLERS.get(info['subcommand'])
    if handler is None:
        raise RuntimeError(
            'No handler registered for subcommand ' + info['subcommand'])
    parser.set_defaults(func=handler)
    return parser


def set_package_and_args_options(args, options):
    vs = vars(args)

    info = SUBCOMMAND_INFO_BY_NAME.get(args.SUBCOMMAND)
    if info is None:
        msg.error_exit(args.SUBCOMMAND + ' is not a known subcommand')

    options['afwdev_info'] = afwdev_info

    options['is_core_afw_package'] = False
    if not info.get('newPackageDirPath', False):
        package_dir = package.find_afw_package_dir()
        if package_dir is None:
            msg.error_exit(
                'The current working directory must be inside of an AFW package')
        options['afw_package_dir_path'] = package_dir

        package.set_is_core_afw_package(options)
        get_afwdev_settings(options)

    options['subcommand_info'] = info
    for arg in info['args']:
        v = None

        if arg.get('arg') is None:
            if arg.get('default') is None:
                msg.error_exit('default required if arg is not specified')
            v = arg.get('default').replace(
                '{thing}', info['thing']).format(**options)

        elif arg['arg'].startswith('--'):
            v = vs.get(arg['arg'][2:].replace('-', '_'))
            if v is None:
                v = arg.get('default')
                if not isinstance(v, str):
                    continue
                if v is not None:
                    v = v.replace('{thing}', info['thing']).format(**options)
                if (vs.get('prompt', False) or
                    (not vs.get('noprompt', False)
                     and not arg.get('noprompt', False))
                ):
                    i = input(
                        arg.get('help').replace(
                            '{thing}', info['thing']).format(**options) +
                        '\nEnter ' + arg.get('optionName') +
                        ' or press <enter> to use default.\n' +
                        'Default is: ' + v + '\n> ')
                    if i != '':
                        v = i

        else:
            v = vs.get(arg['arg'])

        if v is not None and isinstance(v, list) and arg.get('nargs', 0) == 1:
            v = v[0]

        if v is None:
            msg.warn(arg['optionName'] + ' is None')
        options[arg['optionName']] = v


def setup_parser(options):
    parser = argparse.ArgumentParser(
        formatter_class=argparse.RawTextHelpFormatter,
        **_afwdev_overall_info
    )

    verbosity_group = parser.add_mutually_exclusive_group()
    verbosity_group.add_argument(
        "--debug",
        help=_info_debug['help'],
        action=_info_debug['action'])
    verbosity_group.add_argument(
        "-q", "--quiet",
        help=_info_quiet['help'],
        action=_info_quiet['action'])
    verbosity_group.add_argument(
        "--verbose",
        help=_info_verbose['help'],
        action=_info_verbose['action'])

    parser.add_argument(
        "-v", "--version",
        help=_info_version['help'],
        action=_info_version['action'],
        version=get_version(options))

    parser.add_argument(
        "--version-string",
        help=_info_version_string['help'],
        action=_info_version_string['action'],
        version=get_version_string(options))

    parser.add_argument(
        "--noprompt",
        help=_info_noprompt['help'],
        action=_info_noprompt['action'])

    parser.add_argument(
        "--prompt",
        help=_info_prompt['help'],
        action=_info_prompt['action'])

    subparsers = parser.add_subparsers(
        title='subcommands',
        dest="SUBCOMMAND",
        help="Additional help",
        description="Valid subcommands")
    # Always alphabetical so new subcommands (e.g. blast) do not depend on
    # list order in info.py for --help readability.
    for info in sorted(
            _subcommand_infos, key=lambda i: i.get('subcommand') or ''):
        add_args(subparsers, info)

    return parser
